/////////////////////////////////////
//                                 //
//       FADC400 module GUI        //
//    Data Taking Process Class    //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 11. 01           //
//                                 //
/////////////////////////////////////

#include "FADC400Process.hh"

#include "TString.h"
#include "TSystem.h"
#include "Riostream.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TDatime.h"

FADC400Process::FADC400Process(FADC400Settings settings)
{
  for (Int_t iModule = 0; iModule < 2; iModule++) {
    fActive[iModule] = settings.fIsActive[iModule];

    if (fActive[iModule])
      fAddress[iModule] = settings.fValueAddress[iModule];
  }

  TDatime date; 
  TString filename = Form("FADC400_%d_%d.root", date.GetDate(), date.GetTime);
  outFile = new TFile(filename.Data(), "recreate");

  // get NKHOME enviernment
  TString mypath = gSystem -> Getenv("NKHOME");
  cout << "NKHOME pass : " << mypath << endl;
  TString myvme = mypath + TString("/lib/libNotice6UVME_root.so");
  TString myfadc = mypath + TString("/lib/libNoticeFADC400_root.so");

  // Loading VME & FADC lib.
  gSystem->Load(myvme);
  gSystem->Load(myfadc);

  // DON'T CHANGE THIS - THIS IS FIXED VALUE
  fNKUSB = 0x0;
  // =======================================

  // NOTICE KOREA FADC400 Class
  NKFADC400 adc;

  // VME Open
  adc.VMEopen(fNKUSB);

  // Downlaod FPGA program into Xilinx for all 4 channels because of trigger lookup table
  for (Int_t iModule = 0; iModule < 2; iModule++) {
    if (fActive[iModule]) {
      cout << " ======== Start Downloading FPGA program of Module " << iModule + 1 << "========" << endl;
      ULong_t d1;
      ULong_t da = 0xFADC0;
      ULong_t dd = 0xFADC3;

      cout << " - Checking FPGA program status before downloading." << endl;
      for (Int_t iChannel = 1; iChannel <= 4; iChannel++) {
        d1 = adc.FADC400read_STAT(fNKUSB, fModuleID[iModule], iChannel);
        cout << Form(" Status at Module%d Channel%d : %X", iModule + 1, iChannel, d1) << endl;
      }

      if (adc.FADC400read_STAT(fNKUSB, fModuleID[iModule], 1) == da && adc.FADC400read_STAT(fNKUSB, fModuleID[iModule], 4) == dd)
        cout << " - FPGA program downloaded! Skip download!" << endl;
      else {
        for (Int_t iChannel = 1; iChannel <= 4; iChannel++)
          adc.FADC400download(fNKUSB, fModuleID[iModule], iChannel);

        cout << " - Checking FPGA program status after downloading." << endl;
        for (Int_t iChannel = 1; iChannel <= 4; iChannel++) {
          d1 = adc.FADC400read_STAT(fNKUSB, fModuleID[iModule], iChannel);
          printf(" Status at Module%d Ch%d : %X \n", iModule + 1, iChannel, d1);
        }
      }
      cout << " ========= End Downloading FPGA program of Module " << iModule + 1 << "=========" << endl;

      // Set reset mode, (device number, mid, channel, reset with daisy chain, timer reset, event number reset, register reset)
      adc.FADC400write_RM(fNKUSB, fModuleID[iModule], 0, 0, 0, 1, 0);

      // Reset FADC
      adc.FADC400reset(fNKUSB, fModuleID[iModule], 0);

      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        // Set data saving mode
        adc.FADC400write_DSM(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueDSM[iModule][iChannel]); 

        // Set input polarity
        adc.FADC400write_POL(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueIP[iModule][iChannel]);

        // Set input delay
        adc.FADC400write_DLY(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueID[iModule][iChannel]);

        // Set ADC offset
        adc.FADC400write_DAC(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueAO[iModule][iChannel]);

        // Set discriminator threshold
        adc.FADC400write_THR(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueThres[iModule][iChannel]);

        // Set recording length
        adc.FADC400write_RL(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueRL[iModule][iChannel]);

      }

      // Calibrate FADC
      adc.FADC400calib(fNKUSB, fModuleID[iModule], 0);

      // Measure and show pedestal
      cout << " ======== Start measuring pedestal ========" << endl;
      adc.FADC400measure_PED(fNKUSB, fModuleID[iModule], 0);
      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        ULong_t pedestal = adc.FADC400read_PED(fNKUSB, fModule[iModule], iChannel + 1);
        cout << Form(" Pedestal Module%d Channel%d = %ld", iModule + 1, iChannel + 1, pedestal) << endl;
      }
      cout << " ========= End measuring pedestal =========" << endl;

      // Set trigger lookup table register
      adc.FADC400write_TLT(fNKUSB, fModuleID[iModule], settings.fValueCLT[iModule]); 

      for (Int_t iCGroup = 0; iCGroup < 2; iCGroup++) {
        ULong_t groupIdx = TMath::Power(3., iCGroup);
        // Set trigger deadtime
        adc.FADC400write_DT(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueDT[iModule][iCGroup]);

        // Set coincidence width
        adc.FADC400write_CW(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueCW[iModule][iCGroup]);

        // Set trigger mode (device, mid, ch, enable pulse width, pulse width check channel(0:or,1:1,2:2,3:1&2), enable pulse count, #pulse check channel)
        Bool_t isActiveTMW = settings.fValueTMWidth[iModule][iCGroup];
        Int_t TMWOption = settings.fValueTMWidthOption[iModule][iCGroup];
        Bool_t isActiveTMC = settings.fValueTMCount[iModule][iCGroup];
        Int_t TMCOption = settings.fValueTMCountOption[iModule][iCGroup];
        adc.FADC400write_TM(fNKUSB, fModuleID[iModule], groupIdx, isActiveTMW, TMWOption, isActiveTMC, TMCOption);

        // Set pulse width threshold
        adc.FADC400write_PWT(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMWidthThres[iModule][iCGroup]); 

        if (TMCOption < 3) {
          if (TMCOption == 0) {
            // Set pulse count threshold
            kadc.FADC400write_PCTX(fNKUSB, fModuleID[iModule], groupIdx, 0);
            kadc.FADC400write_PCTY(fNKUSB, fModuleID[iModule], groupIdx, 0);
            kadc.FADC400write_PCTXY(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountThres[iModule][iCGroup][0]); 

            // Set pulse count interval
            kadc.FADC400write_PCIX(fNKUSB, fModuleID[iModule], groupIdx, 0);
            kadc.FADC400write_PCIY(fNKUSB, fModuleID[iModule], groupIdx, 0);
            kadc.FADC400write_PCIXY(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountInterval[iModule][iCGroup][0]);
          } else if (TMCOption == 1) {
            // Set pulse count threshold
            kadc.FADC400write_PCTX(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountThres[iModule][iCGroup][0]); 
            kadc.FADC400write_PCTY(fNKUSB, fModuleID[iModule], groupIdx, 0);
            kadc.FADC400write_PCTXY(fNKUSB, fModuleID[iModule], groupIdx, 0);

            // Set pulse count interval
            kadc.FADC400write_PCIX(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountInterval[iModule][iCGroup][0]);
            kadc.FADC400write_PCIY(fNKUSB, fModuleID[iModule], groupIdx, 0);
            kadc.FADC400write_PCIXY(fNKUSB, fModuleID[iModule], groupIdx, 0);
          } else if (TMCOption == 2) {
            // Set pulse count threshold
            kadc.FADC400write_PCTX(fNKUSB, fModuleID[iModule], groupIdx, 0);
            kadc.FADC400write_PCTY(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountThres[iModule][iCGroup][0]); 
            kadc.FADC400write_PCTXY(fNKUSB, fModuleID[iModule], groupIdx, 0);

            // Set pulse count interval
            kadc.FADC400write_PCIX(fNKUSB, fModuleID[iModule], groupIdx, 0);
            kadc.FADC400write_PCIY(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountInterval[iModule][iCGroup][0]);
            kadc.FADC400write_PCIXY(fNKUSB, fModuleID[iModule], groupIdx, 0);
          }

        } else {
          // Set pulse count threshold
          kadc.FADC400write_PCTX(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountThres[iModule][iCGroup][0]); 
          kadc.FADC400write_PCTY(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountThres[iModule][iCGroup][1]); 
          kadc.FADC400write_PCTXY(fNKUSB, fModuleID[iModule], groupIdx, 0);

          // Set pulse count interval
          kadc.FADC400write_PCIX(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountInterval[iModule][iCGroup][0]);
          kadc.FADC400write_PCIY(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountInterval[iModule][iCGroup][1]);
          kadc.FADC400write_PCIXY(fNKUSB, fModuleID[iModule], groupIdx, 0);
        }
      }
    }
  }

  SaveHeader();
  TakeData();
}

FADC400Process::~FADC400Process()
{
}

void FADC400Process::SaveHeader()
{
  for (Int_t iModule = 0; iModule < 2; iModule++) {
    if (fActive[iModule]) {
      outFile -> cd();

      header[iModule] = new FADC400Header();
      header[iModule] -> SetName(Form("module%d", iModule));

      // Reading the setting parameters from module register, fill them to the header.
    } else
      header[iModule] = NULL;
  }
}

void FADC400Process::TakeData()
{
}
