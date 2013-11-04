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
#include "FADC400Header.hh"
#include "FADC400Event.hh"

#include "TString.h"
#include "TSystem.h"
#include "Riostream.h"
#include "TFile.h"
#include "TMath.h"
#include "TDatime.h"
#include "TClonesArray.h"

FADC400Process::FADC400Process(FADC400Settings settings)
{
  Initialize();

  for (Int_t iModule = 0; iModule < 2; iModule++) {
    fActiveModule[iModule] = settings.fIsActive[iModule];

    if (fActiveModule[iModule])
      fModuleID[iModule] = settings.fValueAddress[iModule];
  }

  fEventsToTake = settings.fValueNumEvents;

  TDatime date; 
  TString filename = Form("FADC400_%d_%d.root", date.GetDate(), date.GetTime());
  fOutFile = new TFile(filename.Data(), "recreate");

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

  // VME Open
  fAdc.VMEopen(fNKUSB);

  // Downlaod FPGA program into Xilinx for all 4 channels because of trigger lookup table
  for (Int_t iModule = 0; iModule < 2; iModule++) {
    if (fActiveModule[iModule]) {
      cout << " ======== Start Downloading FPGA program of Module " << iModule + 1 << "========" << endl;
      ULong_t d1;
      ULong_t da = 0xFADC0;
      ULong_t dd = 0xFADC3;

      cout << " - Checking FPGA program status before downloading." << endl;
      for (Int_t iChannel = 1; iChannel <= 4; iChannel++) {
        d1 = fAdc.FADC400read_STAT(fNKUSB, fModuleID[iModule], iChannel);
        cout << Form(" Status at Module%d Channel%d : %X", iModule + 1, iChannel, d1) << endl;
      }

      if (fAdc.FADC400read_STAT(fNKUSB, fModuleID[iModule], 1) == da && fAdc.FADC400read_STAT(fNKUSB, fModuleID[iModule], 4) == dd)
        cout << " - FPGA program downloaded! Skip download!" << endl;
      else {
        for (Int_t iChannel = 1; iChannel <= 4; iChannel++)
          fAdc.FADC400download(fNKUSB, fModuleID[iModule], iChannel);

        cout << " - Checking FPGA program status after downloading." << endl;
        for (Int_t iChannel = 1; iChannel <= 4; iChannel++) {
          d1 = fAdc.FADC400read_STAT(fNKUSB, fModuleID[iModule], iChannel);
          printf(" Status at Module%d Ch%d : %X \n", iModule + 1, iChannel, d1);
        }
      }
      cout << " ========= End Downloading FPGA program of Module " << iModule + 1 << "=========" << endl;

      // Set reset mode, (device number, mid, channel, reset with daisy chain, timer reset, event number reset, register reset)
      fAdc.FADC400write_RM(fNKUSB, fModuleID[iModule], 0, 0, 0, 1, 0);

      // Reset FADC
      fAdc.FADC400reset(fNKUSB, fModuleID[iModule], 0);

      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        fActiveChannel[iModule][iChannel] = settings.fValueAC[iModule][iChannel];

        // Set data saving mode
        fAdc.FADC400write_DSM(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueDSM[iModule][iChannel]); 

        // Set input polarity
        fAdc.FADC400write_POL(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueIP[iModule][iChannel]);

        // Set input delay
        fAdc.FADC400write_DLY(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueID[iModule][iChannel]);

        // Set ADC offset
        fAdc.FADC400write_DAC(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueAO[iModule][iChannel]);

        // Set discriminator threshold
        fAdc.FADC400write_THR(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueThres[iModule][iChannel]);

        // Set recording length
        fAdc.FADC400write_RL(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueRL[iModule][iChannel]);

      }

      // Calibrate FADC
      fAdc.FADC400calib(fNKUSB, fModuleID[iModule], 0);

      // Measure and show pedestal
      cout << " ======== Start measuring pedestal ========" << endl;
      fAdc.FADC400measure_PED(fNKUSB, fModuleID[iModule], 0);
      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        ULong_t pedestal = fAdc.FADC400read_PED(fNKUSB, fModuleID[iModule], iChannel + 1);
        cout << Form(" Pedestal Module%d Channel%d = %ld", iModule + 1, iChannel + 1, pedestal) << endl;
      }
      cout << " ========= End measuring pedestal =========" << endl;

      // Set trigger lookup table register
      fAdc.FADC400write_TLT(fNKUSB, fModuleID[iModule], settings.fValueCLT[iModule]); 

      for (Int_t iCGroup = 0; iCGroup < 2; iCGroup++) {
        ULong_t groupIdx = TMath::Power(3., iCGroup);
        // Set trigger deadtime
        fAdc.FADC400write_DT(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueDT[iModule][iCGroup]);

        // Set coincidence width
        fAdc.FADC400write_CW(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueCW[iModule][iCGroup]);

        // Set trigger mode (device, mid, ch, enable pulse width, pulse width check channel(0:or,1:1,2:2,3:1&2), enable pulse count, #pulse check channel)
        Bool_t isActiveTMW = settings.fValueTMWidth[iModule][iCGroup];
        Int_t TMWOption = settings.fValueTMWidthOption[iModule][iCGroup];
        Bool_t isActiveTMC = settings.fValueTMCount[iModule][iCGroup];
        Int_t TMCOption = settings.fValueTMCountOption[iModule][iCGroup];
        fAdc.FADC400write_TM(fNKUSB, fModuleID[iModule], groupIdx, isActiveTMW, TMWOption, isActiveTMC, TMCOption);

        // Set pulse width threshold
        fAdc.FADC400write_PWT(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMWidthThres[iModule][iCGroup]); 

        if (TMCOption < 3) {
          if (TMCOption == 0) {
            // Set pulse count threshold
            fAdc.FADC400write_PCTX(fNKUSB, fModuleID[iModule], groupIdx, 0);
            fAdc.FADC400write_PCTY(fNKUSB, fModuleID[iModule], groupIdx, 0);
            fAdc.FADC400write_PCTXY(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountThres[iModule][iCGroup][0]); 

            // Set pulse count interval
            fAdc.FADC400write_PCIX(fNKUSB, fModuleID[iModule], groupIdx, 0);
            fAdc.FADC400write_PCIY(fNKUSB, fModuleID[iModule], groupIdx, 0);
            fAdc.FADC400write_PCIXY(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountInterval[iModule][iCGroup][0]);
          } else if (TMCOption == 1) {
            // Set pulse count threshold
            fAdc.FADC400write_PCTX(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountThres[iModule][iCGroup][0]); 
            fAdc.FADC400write_PCTY(fNKUSB, fModuleID[iModule], groupIdx, 0);
            fAdc.FADC400write_PCTXY(fNKUSB, fModuleID[iModule], groupIdx, 0);

            // Set pulse count interval
            fAdc.FADC400write_PCIX(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountInterval[iModule][iCGroup][0]);
            fAdc.FADC400write_PCIY(fNKUSB, fModuleID[iModule], groupIdx, 0);
            fAdc.FADC400write_PCIXY(fNKUSB, fModuleID[iModule], groupIdx, 0);
          } else if (TMCOption == 2) {
            // Set pulse count threshold
            fAdc.FADC400write_PCTX(fNKUSB, fModuleID[iModule], groupIdx, 0);
            fAdc.FADC400write_PCTY(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountThres[iModule][iCGroup][0]); 
            fAdc.FADC400write_PCTXY(fNKUSB, fModuleID[iModule], groupIdx, 0);

            // Set pulse count interval
            fAdc.FADC400write_PCIX(fNKUSB, fModuleID[iModule], groupIdx, 0);
            fAdc.FADC400write_PCIY(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountInterval[iModule][iCGroup][0]);
            fAdc.FADC400write_PCIXY(fNKUSB, fModuleID[iModule], groupIdx, 0);
          }
        } else {
          // Set pulse count threshold
          fAdc.FADC400write_PCTX(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountThres[iModule][iCGroup][0]); 
          fAdc.FADC400write_PCTY(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountThres[iModule][iCGroup][1]); 
          fAdc.FADC400write_PCTXY(fNKUSB, fModuleID[iModule], groupIdx, 0);

          // Set pulse count interval
          fAdc.FADC400write_PCIX(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountInterval[iModule][iCGroup][0]);
          fAdc.FADC400write_PCIY(fNKUSB, fModuleID[iModule], groupIdx, settings.fValueTMCountInterval[iModule][iCGroup][1]);
          fAdc.FADC400write_PCIXY(fNKUSB, fModuleID[iModule], groupIdx, 0);
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

void FADC400Process::Initialize()
{
  fNKUSB = 0;
  fOutFile = NULL;
  fEventsToTake = 0;

  for (Int_t iModule = 0; iModule < 2; iModule++) {
    fModuleID[iModule] = -1;
    fActiveModule[iModule] = -1;

    fHeader[iModule] = NULL;

    for (Int_t iChannel = 0; iChannel < 2; iChannel++) {
      fActiveChannel[iModule][iChannel] = 0;
      fEvent[iModule][iChannel] = NULL;
    }
  }
}

void FADC400Process::SaveHeader()
{
  cout << " ======== Start writing header ========" << endl;
  for (Int_t iModule = 0; iModule < 2; iModule++) {
    if (fActiveModule[iModule]) {
      fOutFile -> cd();

      fHeader[iModule] = new FADC400Header();
      fHeader[iModule] -> SetName(Form("module%d", iModule));
      fHeader[iModule] -> SetNumEvents(fEventsToTake);

      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        fHeader[iModule] -> SetAC(iChannel, fActiveChannel[iModule][iChannel]);
        fHeader[iModule] -> SetDSM(iChannel, fAdc.FADC400read_DSM(fNKUSB, fModuleID[iModule], iChannel + 1));
        fHeader[iModule] -> SetIP(iChannel, fAdc.FADC400read_POL(fNKUSB, fModuleID[iModule], iChannel + 1));
        fHeader[iModule] -> SetID(iChannel, fAdc.FADC400read_DLY(fNKUSB, fModuleID[iModule], iChannel + 1));
        fHeader[iModule] -> SetAO(iChannel, fAdc.FADC400read_DAC(fNKUSB, fModuleID[iModule], iChannel + 1));
        fHeader[iModule] -> SetThreshold(iChannel, fAdc.FADC400read_THR(fNKUSB, fModuleID[iModule], iChannel + 1));
        fHeader[iModule] -> SetRL(iChannel, fAdc.FADC400read_RL(fNKUSB, fModuleID[iModule], iChannel + 1));
      }

      fHeader[iModule] -> SetCLT(fAdc.FADC400read_TLT(fNKUSB, fModuleID[iModule]));

      for (Int_t iCGroup = 0; iCGroup < 4; iCGroup++) {
        ULong_t groupIdx = TMath::Power(3., iCGroup);

        fHeader[iModule] -> SetDT(iCGroup, fAdc.FADC400read_DT(fNKUSB, fModuleID[iModule], groupIdx));
        fHeader[iModule] -> SetCW(iCGroup, fAdc.FADC400read_CW(fNKUSB, fModuleID[iModule], groupIdx));

        ULong_t triggerMode = fAdc.FADC400read_TM(fNKUSB, fModuleID[iModule], groupIdx);
        Bool_t widthMode = ((triggerMode & 0x20) >> 5);
        Int_t widthOption = ((triggerMode & 0x18) >> 3);
        Bool_t countMode = ((triggerMode & 0x4) >> 2);
        Int_t countOption = (triggerMode & 0x3);
        fHeader[iModule] -> SetTMWidth(iCGroup, widthMode);
        fHeader[iModule] -> SetTMWidthOption(iCGroup, widthOption);
        fHeader[iModule] -> SetTMWidthThreshold(iCGroup, fAdc.FADC400read_PWT(fNKUSB, fModuleID[iModule], groupIdx));
        fHeader[iModule] -> SetTMCount(iCGroup, countMode);
        fHeader[iModule] -> SetTMCountOption(iCGroup, countOption);
        if (countOption == 0) {
          fHeader[iModule] -> SetTMCountThreshold(iCGroup, 0, fAdc.FADC400read_PCTXY(fNKUSB, fModuleID[iModule], groupIdx));
          fHeader[iModule] -> SetTMCountInterval(iCGroup, 0, fAdc.FADC400read_PCIXY(fNKUSB, fModuleID[iModule], groupIdx));
        } else if (countOption == 1) {
          fHeader[iModule] -> SetTMCountThreshold(iCGroup, 0, fAdc.FADC400read_PCTX(fNKUSB, fModuleID[iModule], groupIdx));
          fHeader[iModule] -> SetTMCountInterval(iCGroup, 0, fAdc.FADC400read_PCIX(fNKUSB, fModuleID[iModule], groupIdx));
        } else if (countOption == 2) {
          fHeader[iModule] -> SetTMCountThreshold(iCGroup, 0, fAdc.FADC400read_PCTY(fNKUSB, fModuleID[iModule], groupIdx));
          fHeader[iModule] -> SetTMCountInterval(iCGroup, 0, fAdc.FADC400read_PCIY(fNKUSB, fModuleID[iModule], groupIdx));
        } else {
          fHeader[iModule] -> SetTMCountThreshold(iCGroup, 0, fAdc.FADC400read_PCTX(fNKUSB, fModuleID[iModule], groupIdx));
          fHeader[iModule] -> SetTMCountInterval(iCGroup, 0, fAdc.FADC400read_PCIX(fNKUSB, fModuleID[iModule], groupIdx));
          fHeader[iModule] -> SetTMCountThreshold(iCGroup, 1, fAdc.FADC400read_PCTY(fNKUSB, fModuleID[iModule], groupIdx));
          fHeader[iModule] -> SetTMCountInterval(iCGroup, 1, fAdc.FADC400read_PCIY(fNKUSB, fModuleID[iModule], groupIdx));
        }
      }

      fHeader[iModule] -> Write();
    }
  }
  cout << " ========= End writing header =========" << endl;
}

void FADC400Process::TakeData()
{
  cout << " ======== Start taking data ========" << endl;

  Bool_t channelFlag[2][4] = {0};

  for (Int_t iModule = 0; iModule < 2; iModule++) {
    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      if (fActiveChannel[iModule][iChannel]) {
        if (fActiveModule[iModule])
          channelFlag[iModule][iChannel] = 1;

        Int_t recordingLength = fHeader[iModule] -> GetRL(iChannel);
        fEvent[iModule][iChannel] = new TClonesArray(Form("FADC400Event%d", recordingLength), fHeader[iModule] -> GetNumEvents());
        fAdc.FADC400startL(fNKUSB, fModuleID[iModule], iChannel + 1); 
        fAdc.FADC400startH(fNKUSB, fModuleID[iModule], iChannel + 1); 
      }
    }
  }

  Int_t bufferNum[2][4] = {0};
  Int_t dataPoint[2][4] = {0};
  Int_t isFill[2][4];
  for (Int_t iModule = 0; iModule < 2; iModule++) {
    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      isFill[iModule][iChannel] = 1;
      fEventNum[iModule][iChannel] = 0;
    }
  }

  Bool_t overallFlag = 1;
  while (overallFlag) {
    Bool_t check = 0;
    for (Int_t iModule = 0; iModule < 2; iModule++)
      for (Int_t iChannel = 0; iChannel < 4; iChannel++)
        check = (check | channelFlag[iModule][iChannel]);
    overallFlag = check;

    for (Int_t iModule = 0; iModule < 2; iModule++) {
      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        if (channelFlag[iModule][iChannel]) {
          if (bufferNum[iModule][iChannel] == 0)
            isFill[iModule][iChannel] = !(fAdc.FADC400read_RunL(fNKUSB, fModuleID[iModule], iChannel + 1));
          else if (bufferNum[iModule][iChannel] == 1)
            isFill[iModule][iChannel] = !(fAdc.FADC400read_RunH(fNKUSB, fModuleID[iModule], iChannel + 1));
        }
      }
    }

    for (Int_t iModule = 0; iModule < 2; iModule++) {
      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        if (channelFlag[iModule][iChannel] && isFill[iModule][iChannel]) {
          Int_t recordingLength = fHeader[iModule] -> GetRL(iChannel);
          switch (recordingLength) {
            case 1: DataRL1(iModule, iChannel, bufferNum[iModule][iChannel]); break;
            case 2: DataRL2(iModule, iChannel, bufferNum[iModule][iChannel]); break;
            case 4: DataRL4(iModule, iChannel, bufferNum[iModule][iChannel]); break;
            case 8: DataRL8(iModule, iChannel, bufferNum[iModule][iChannel]); break;
            case 16: DataRL16(iModule, iChannel, bufferNum[iModule][iChannel]); break;
            case 32: DataRL32(iModule, iChannel, bufferNum[iModule][iChannel]); break;
            case 64: DataRL64(iModule, iChannel, bufferNum[iModule][iChannel]); break;
            case 128: DataRL128(iModule, iChannel, bufferNum[iModule][iChannel]); break;
            case 256: DataRL256(iModule, iChannel, bufferNum[iModule][iChannel]); break;
            case 512: DataRL512(iModule, iChannel, bufferNum[iModule][iChannel]); break;
            default: break;
          }

          if (fEventNum[iModule][iChannel] >= fHeader[iModule] -> GetNumEvents())
            channelFlag[iModule][iChannel] = 0;
          else {
            if (bufferNum[iModule][iChannel] == 0) {
              bufferNum[iModule][iChannel] = 1;
              fAdc.FADC400startL(fNKUSB, fModuleID[iModule], iChannel + 1); 
            } else if (bufferNum[iModule][iChannel] == 1) {
              bufferNum[iModule][iChannel] = 0;
              fAdc.FADC400startH(fNKUSB, fModuleID[iModule], iChannel + 1); 
            }
          }
        }
      }
    }
  }

  for (Int_t iModule = 0; iModule < 2; iModule++)
    for (Int_t iChannel = 0; iChannel < 4; iChannel++)
      if (fActiveChannel[iModule][iChannel])
        fEvent[iModule][iChannel] -> Write();

  fOutFile -> Write();
  cout << " ========= End taking data =========" << endl;
}

void FADC400Process::DataRL1(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL(iChannel);
  Int_t numEvents = 512/recordingLength;
  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    FADC400Event1 *event = (FADC400Event1 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[8];
    fAdc.FADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 8; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.FADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x80));
    event -> SetCountTrigger((tPattern & 0x40));
    event -> SetTriggerInCounts((tPattern & 0x1F));

    UShort_t data[RL1] = {0};
    fAdc.FADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL1; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    fEventNum[iModule][iChannel]++;
    if (fEventNum[iModule][iChannel] >= fHeader[iModule] -> GetNumEvents())
      break;
  }
}

void FADC400Process::DataRL2(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL(iChannel);
  Int_t numEvents = 512/recordingLength;
  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    FADC400Event2 *event = (FADC400Event2 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[8];
    fAdc.FADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 8; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.FADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x80));
    event -> SetCountTrigger((tPattern & 0x40));
    event -> SetTriggerInCounts((tPattern & 0x1F));

    UShort_t data[RL2] = {0};
    fAdc.FADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL2; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    fEventNum[iModule][iChannel]++;
    if (fEventNum[iModule][iChannel] >= fHeader[iModule] -> GetNumEvents())
      break;
  }
}

void FADC400Process::DataRL4(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL(iChannel);
  Int_t numEvents = 512/recordingLength;
  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    FADC400Event4 *event = (FADC400Event4 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[8];
    fAdc.FADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 8; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.FADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x80));
    event -> SetCountTrigger((tPattern & 0x40));
    event -> SetTriggerInCounts((tPattern & 0x1F));

    UShort_t data[RL4] = {0};
    fAdc.FADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL4; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    fEventNum[iModule][iChannel]++;
    if (fEventNum[iModule][iChannel] >= fHeader[iModule] -> GetNumEvents())
      break;
  }
}

void FADC400Process::DataRL8(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL(iChannel);
  Int_t numEvents = 512/recordingLength;
  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    FADC400Event8 *event = (FADC400Event8 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[8];
    fAdc.FADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 8; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.FADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x80));
    event -> SetCountTrigger((tPattern & 0x40));
    event -> SetTriggerInCounts((tPattern & 0x1F));

    UShort_t data[RL8] = {0};
    fAdc.FADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL8; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    fEventNum[iModule][iChannel]++;
    if (fEventNum[iModule][iChannel] >= fHeader[iModule] -> GetNumEvents())
      break;
  }
}
 
void FADC400Process::DataRL16(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL(iChannel);
  Int_t numEvents = 512/recordingLength;
  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    FADC400Event16 *event = (FADC400Event16 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[8];
    fAdc.FADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 8; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.FADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x80));
    event -> SetCountTrigger((tPattern & 0x40));
    event -> SetTriggerInCounts((tPattern & 0x1F));

    UShort_t data[RL16] = {0};
    fAdc.FADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL16; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    fEventNum[iModule][iChannel]++;
    if (fEventNum[iModule][iChannel] >= fHeader[iModule] -> GetNumEvents())
      break;
  }
}

void FADC400Process::DataRL32(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL(iChannel);
  Int_t numEvents = 512/recordingLength;
  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    FADC400Event32 *event = (FADC400Event32 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[8];
    fAdc.FADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 8; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.FADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x80));
    event -> SetCountTrigger((tPattern & 0x40));
    event -> SetTriggerInCounts((tPattern & 0x1F));

    UShort_t data[RL32] = {0};
    fAdc.FADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL32; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    fEventNum[iModule][iChannel]++;
    if (fEventNum[iModule][iChannel] >= fHeader[iModule] -> GetNumEvents())
      break;
  }
}
 
void FADC400Process::DataRL64(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL(iChannel);
  Int_t numEvents = 512/recordingLength;
  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    FADC400Event64 *event = (FADC400Event64 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[8];
    fAdc.FADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 8; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.FADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x80));
    event -> SetCountTrigger((tPattern & 0x40));
    event -> SetTriggerInCounts((tPattern & 0x1F));

    UShort_t data[RL64] = {0};
    fAdc.FADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL64; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    fEventNum[iModule][iChannel]++;
    if (fEventNum[iModule][iChannel] >= fHeader[iModule] -> GetNumEvents())
      break;
  }
}

void FADC400Process::DataRL128(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL(iChannel);
  Int_t numEvents = 512/recordingLength;
  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    FADC400Event128 *event = (FADC400Event128 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[8];
    fAdc.FADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 8; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.FADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x80));
    event -> SetCountTrigger((tPattern & 0x40));
    event -> SetTriggerInCounts((tPattern & 0x1F));

    UShort_t data[RL128] = {0};
    fAdc.FADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL128; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    fEventNum[iModule][iChannel]++;
    if (fEventNum[iModule][iChannel] >= fHeader[iModule] -> GetNumEvents())
      break;
  }
}

void FADC400Process::DataRL256(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL(iChannel);
  Int_t numEvents = 512/recordingLength;
  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    FADC400Event256 *event = (FADC400Event256 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[8];
    fAdc.FADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 8; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.FADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x80));
    event -> SetCountTrigger((tPattern & 0x40));
    event -> SetTriggerInCounts((tPattern & 0x1F));

    UShort_t data[RL256] = {0};
    fAdc.FADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL256; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    fEventNum[iModule][iChannel]++;
    if (fEventNum[iModule][iChannel] >= fHeader[iModule] -> GetNumEvents())
      break;
  }
}

void FADC400Process::DataRL512(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL(iChannel);
  Int_t numEvents = 512/recordingLength;
  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    FADC400Event512 *event = (FADC400Event512 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[8];
    fAdc.FADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 8; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.FADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x80));
    event -> SetCountTrigger((tPattern & 0x40));
    event -> SetTriggerInCounts((tPattern & 0x1F));

    UShort_t data[RL512] = {0};
    fAdc.FADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL512; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    fEventNum[iModule][iChannel]++;
    if (fEventNum[iModule][iChannel] >= fHeader[iModule] -> GetNumEvents())
      break;
  }
}
