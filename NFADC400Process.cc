/////////////////////////////////////
//                                 //
//       NFADC400 module GUI       //
//    Data Taking Process Class    //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 11. 01           //
//                                 //
/////////////////////////////////////

#include "NFADC400Process.hh"
#include "NFADC400Header.hh"
#include "NFADC400Event.hh"

#include "TString.h"
#include "TSystem.h"
#include "Riostream.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TDatime.h"
#include "TClonesArray.h"
#include "TString.h"

NFADC400Process::NFADC400Process(NFADC400Settings settings)
{
  Initialize();

  for (Int_t iModule = 0; iModule < 2; iModule++) {
    fActiveModule[iModule] = settings.fIsActive[iModule];

    if (fActiveModule[iModule])
      fModuleID[iModule] = settings.fValueAddress[iModule];
  }

  fEventsToTake = settings.fValueNumEvents;

  TDatime date; 
  TString filename = Form("NFADC400_%d_%d.root", date.GetDate(), date.GetTime());
  fOutFile = new TFile(filename.Data(), "recreate");

  // get NKHOME enviernment
  TString mypath = gSystem -> Getenv("NKHOME");
  cout << "NKHOME pass : " << mypath << endl;
  TString myvme = mypath + TString("/lib/libNotice6UVME_root.so");
  TString myfadc = mypath + TString("/lib/libNoticeNFADC400_root.so");

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
      cout << " ======== Start Checking FPGA Status of Module " << iModule + 1 << " ========" << endl;
      ULong_t status = fAdc.NFADC400read_STAT(fNKUSB, fModuleID[iModule]);

      if (status != 0xFADC) {
        if ((status & 0xFFF) == 0xADC) {
          cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
          cout << "!!                                   !!" << endl;
          cout << "!! One or more FPGA are not working! !!" << endl;
          cout << "!!        Call NOTICE KOREA!         !!" << endl;
          cout << "!!                                   !!" << endl;
          cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        } else {
          cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
          cout << "!!                                                    !!" << endl;
          cout << "!! Check USB cable or module to backplane connection! !!" << endl;
          cout << "!!   If the error keeps showing, Call NOTICE KOREA!   !!" << endl;
          cout << "!!                                                    !!" << endl;
          cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        }

        cout << " ======== Fail to check FPGA Status of Module " << iModule + 1 << " =========" << endl;

        return;
      }
      cout << "  FADC400 Modules Status: Ok!" << endl;
      cout << " ========= End Checking FPGA Status of Module " << iModule + 1 << " =========" << endl;

      // set reset mode, arguments = (device number, mid, timer reset, event number reset, register reset)
      fAdc.NFADC400write_RM(fNKUSB, fModuleID[iModule], 0, 1, 0);

      // Reset FADC
      fAdc.NFADC400reset(fNKUSB, fModuleID[iModule]);

      for (Int_t iChannel = 0; iChannel < 4; iChannel++)
        fActiveChannel[iModule][iChannel] = settings.fValueAC[iModule][iChannel];

      // Set recording length
      fAdc.NFADC400write_RL(fNKUSB, fModuleID[iModule], settings.fValueRL[iModule]);

      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        // Set input polarity
        fAdc.NFADC400write_POL(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueIP[iModule][iChannel]);

        // Set input delay
        fAdc.NFADC400write_DLY(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueID[iModule][iChannel]);

        // Set ADC offset
        fAdc.NFADC400write_DACOFF(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueAO[iModule][iChannel]);

        // Set input amplifier gain
        fAdc.NFADC400write_DACGAIN(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueAO[iModule][iChannel]);
      }

      // Measure and show pedestal
      cout << " ======== Start measuring pedestal ========" << endl;
      fAdc.NFADC400measure_PED(fNKUSB, fModuleID[iModule], 0);
      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        ULong_t pedestal = fAdc.NFADC400read_PED(fNKUSB, fModuleID[iModule], iChannel + 1);
        cout << Form(" Pedestal Module%d Channel%d = %ld", iModule + 1, iChannel + 1, pedestal) << endl;
      }
      cout << " ========= End measuring pedestal =========" << endl;

      // Set trigger lookup table register (0xFFFE: OR, 0x8000: AND)
      if (settings.fValueCLT[iModule])
        fAdc.NFADC400write_TLT(fNKUSB, fModuleID[iModule], 0xFFFE); 
      else
        fAdc.NFADC400write_TLT(fNKUSB, fModuleID[iModule], 0x8000); 

      // Set trigger output width 
      fAdc.NFADC400write_TOW(fNKUSB, fModuleID[iModule], settings.fValueTOW[iModule]);

      // Set trigger daisy chain
      if (settings.fValueTDC[iModule])
        fAdc.NFADC400disable_DCE(fNKUSB, fModuleID[iModule]);
      else
        fAdc.NFADC400enable_DCE(fNKUSB, fModuleID[iModule]);

      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        // Set discriminator threshold
        fAdc.NFADC400write_THR(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueThres[iModule][iChannel]);

        // Set trigger deadtime
        fAdc.NFADC400write_DT(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueDT[iModule][iChannel]);

        // Set coincidence width
        fAdc.NFADC400write_CW(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueCW[iModule][iChannel]);

        // Set trigger mode (device, mid, ch, enable pulse width, enable pulse count)
        Bool_t isActiveTMW = settings.fValueTMWidth[iModule][iChannel];
        Bool_t isActiveTMC = settings.fValueTMCount[iModule][iChannel];
        fAdc.NFADC400write_TM(fNKUSB, fModuleID[iModule], iChannel + 1, isActiveTMW, isActiveTMC);

        // Set pulse width threshold
        fAdc.NFADC400write_PWT(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueTMWidthThres[iModule][iChannel]); 

        // Set pulse count threshold
        fAdc.NFADC400write_PCT(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueTMCountThres[iModule][iChannel]);

        // Set pulse count interval
        fAdc.NFADC400write_PCI(fNKUSB, fModuleID[iModule], iChannel + 1, settings.fValueTMCountInterval[iModule][iChannel]);
      }
    }
  }

  SaveHeader();
  TakeData();
  fAdc.VMEclose(fNKUSB);
  delete this;
}

NFADC400Process::~NFADC400Process()
{
}

void NFADC400Process::Initialize()
{
  fNKUSB = 0;
  fOutFile = NULL;
  fEventsToTake = 0;

  for (Int_t iModule = 0; iModule < 2; iModule++) {
    fModuleID[iModule] = -1;
    fActiveModule[iModule] = -1;

    fHeader[iModule] = NULL;

    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fActiveChannel[iModule][iChannel] = 0;
      fEvent[iModule][iChannel] = NULL;
      fEventTree[iModule][iChannel] = NULL;
    }
  }
}

void NFADC400Process::SaveHeader()
{
  cout << " ======== Start writing header ========" << endl;
  for (Int_t iModule = 0; iModule < 2; iModule++) {
    if (fActiveModule[iModule]) {
      fOutFile -> cd();

      fHeader[iModule] = new NFADC400Header();
      fHeader[iModule] -> SetName(Form("Mod%d", iModule + 1));
      fHeader[iModule] -> SetNumEvents(fEventsToTake);

      fHeader[iModule] -> SetRL(fAdc.NFADC400read_RL(fNKUSB, fModuleID[iModule]));

      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        fHeader[iModule] -> SetAC(iChannel, fActiveChannel[iModule][iChannel]);
        fHeader[iModule] -> SetIP(iChannel, fAdc.NFADC400read_POL(fNKUSB, fModuleID[iModule], iChannel + 1));
        fHeader[iModule] -> SetID(iChannel, fAdc.NFADC400read_DLY(fNKUSB, fModuleID[iModule], iChannel + 1));
        fHeader[iModule] -> SetAO(iChannel, fAdc.NFADC400read_DACOFF(fNKUSB, fModuleID[iModule], iChannel + 1));
        fHeader[iModule] -> SetIAG(iChannel, fAdc.NFADC400read_DACGAIN(fNKUSB, fModuleID[iModule], iChannel + 1));
      }

      fHeader[iModule] -> SetCLT(fAdc.NFADC400read_TLT(fNKUSB, fModuleID[iModule]));
      fHeader[iModule] -> SetTOW(fAdc.NFADC400read_TOW(fNKUSB, fModuleID[iModule]));
      fHeader[iModule] -> SetTDC(fAdc.NFADC400read_DCE(fNKUSB, fModuleID[iModule]));

      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        fHeader[iModule] -> SetThreshold(iChannel, fAdc.NFADC400read_THR(fNKUSB, fModuleID[iModule], iChannel + 1));
        fHeader[iModule] -> SetDT(iChannel, fAdc.NFADC400read_DT(fNKUSB, fModuleID[iModule], iChannel + 1));
        fHeader[iModule] -> SetCW(iChannel, fAdc.NFADC400read_CW(fNKUSB, fModuleID[iModule], iChannel + 1));

        ULong_t triggerMode = fAdc.NFADC400read_TM(fNKUSB, fModuleID[iModule], iChannel + 1);
        Bool_t widthMode = ((triggerMode & 0x2) >> 1);
        Bool_t countMode = (triggerMode & 0x1);
        fHeader[iModule] -> SetTMWidth(iChannel, widthMode);
        fHeader[iModule] -> SetTMWidthThreshold(iChannel, fAdc.NFADC400read_PWT(fNKUSB, fModuleID[iModule], iChannel + 1));
        fHeader[iModule] -> SetTMCount(iChannel, countMode);
        fHeader[iModule] -> SetTMCountThreshold(iChannel, fAdc.NFADC400read_PCT(fNKUSB, fModuleID[iModule], iChannel + 1));
        fHeader[iModule] -> SetTMCountInterval(iChannel, fAdc.NFADC400read_PCI(fNKUSB, fModuleID[iModule], iChannel + 1));
      }

      fHeader[iModule] -> Write();
    }
  }
  cout << " ========= End writing header =========" << endl;
}

void NFADC400Process::TakeData()
{
  cout << " ======== Start taking data ========" << endl;

  Bool_t channelFlag[2][4] = {0};

  for (Int_t iModule = 0; iModule < 2; iModule++) {
    if (fActiveModule[iModule]) {
      Int_t recordingLength = fHeader[iModule] -> GetRL();

      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        if (fActiveChannel[iModule][iChannel]) {
          channelFlag[iModule][iChannel] = 1;

          fOutFile -> cd();
          fEvent[iModule][iChannel] = new TClonesArray(Form("NFADC400Event%d", recordingLength), fHeader[iModule] -> GetNumEvents());
          fEventTree[iModule][iChannel] = new TTree(Form("Mod%dCh%d", iModule + 1, iChannel + 1), Form("Data of Module %d - Channel %d", iModule + 1, iChannel + 1));
          fEventTree[iModule][iChannel] -> Branch("events", "TClonesArray", fEvent[iModule][iChannel]);
        }
      }

      fAdc.NFADC400startL(fNKUSB, fModuleID[iModule]); 
      fAdc.NFADC400startH(fNKUSB, fModuleID[iModule]); 
    }
  }

  Int_t bufferNum[2] = {0};
  Int_t isFill[2];
  for (Int_t iModule = 0; iModule < 2; iModule++) {
    isFill[iModule] = 1;

    for (Int_t iChannel = 0; iChannel < 4; iChannel++)
      fEventNum[iModule][iChannel] = 0;
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
          if (bufferNum[iModule] == 0)
            isFill[iModule] = !(fAdc.NFADC400read_RunL(fNKUSB, fModuleID[iModule]));
          else if (bufferNum[iModule] == 1)
            isFill[iModule] = !(fAdc.NFADC400read_RunH(fNKUSB, fModuleID[iModule]));
        }
      }
    }

    for (Int_t iModule = 0; iModule < 2; iModule++) {
      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        if (channelFlag[iModule][iChannel] && isFill[iModule]) {
          Int_t recordingLength = fHeader[iModule] -> GetRL();
          switch (recordingLength) {
            case 1: DataRL1(iModule, iChannel, bufferNum[iModule]); break;
            case 2: DataRL2(iModule, iChannel, bufferNum[iModule]); break;
            case 4: DataRL4(iModule, iChannel, bufferNum[iModule]); break;
            case 8: DataRL8(iModule, iChannel, bufferNum[iModule]); break;
            case 16: DataRL16(iModule, iChannel, bufferNum[iModule]); break;
            case 32: DataRL32(iModule, iChannel, bufferNum[iModule]); break;
            case 64: DataRL64(iModule, iChannel, bufferNum[iModule]); break;
            case 128: DataRL128(iModule, iChannel, bufferNum[iModule]); break;
            case 256: DataRL256(iModule, iChannel, bufferNum[iModule]); break;
            case 512: DataRL512(iModule, iChannel, bufferNum[iModule]); break;
            case 1024: DataRL1024(iModule, iChannel, bufferNum[iModule]); break;
            case 2048: DataRL2048(iModule, iChannel, bufferNum[iModule]); break;
            case 4096: DataRL4096(iModule, iChannel, bufferNum[iModule]); break;
            default: break;
          }

          if (fEventNum[iModule][iChannel] >= fHeader[iModule] -> GetNumEvents())
            channelFlag[iModule][iChannel] = 0;
          else {
            if (bufferNum[iModule] == 0) {
              bufferNum[iModule] = 1;
              fAdc.NFADC400startL(fNKUSB, fModuleID[iModule]); 
            } else if (bufferNum[iModule] == 1) {
              bufferNum[iModule] = 0;
              fAdc.NFADC400startH(fNKUSB, fModuleID[iModule]); 
            }
          }
        }
      }
    }
  }

  for (Int_t iModule = 0; iModule < 2; iModule++)
    for (Int_t iChannel = 0; iChannel < 4; iChannel++)
      if (fActiveChannel[iModule][iChannel]) {
        fOutFile -> cd();
        fEventTree[iModule][iChannel] -> Fill();
      }

  fOutFile -> Write();
  cout << " ========= End taking data =========" << endl;
}

void NFADC400Process::DataRL1(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL();
  Int_t numEvents = 0;
  if (recordingLength < 16)
    numEvents = 512;
  else
    numEvents = 0x1000/recordingLength;

  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    NFADC400Event1 *event = (NFADC400Event1 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[6];
    fAdc.NFADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 6; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.NFADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x2) >> 1);
    event -> SetCountTrigger((tPattern & 0x1));

    UShort_t data[RL1] = {0};
    fAdc.NFADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL1; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    if (fEventNum[iModule][iChannel] + iEvent + 1 >= fHeader[iModule] -> GetNumEvents()) {
      fEventNum[iModule][iChannel] += iEvent + 1;
      break;
    }
  }

  fEventNum[iModule][iChannel] += numEvents;
}

void NFADC400Process::DataRL2(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL();
  Int_t numEvents = 0;
  if (recordingLength < 16)
    numEvents = 512;
  else
    numEvents = 0x1000/recordingLength;

  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    NFADC400Event2 *event = (NFADC400Event2 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[6];
    fAdc.NFADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 6; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.NFADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x2) >> 1);
    event -> SetCountTrigger((tPattern & 0x1));

    UShort_t data[RL2] = {0};
    fAdc.NFADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL2; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    if (fEventNum[iModule][iChannel] + iEvent + 1 >= fHeader[iModule] -> GetNumEvents()) {
      fEventNum[iModule][iChannel] += iEvent + 1;
      break;
    }
  }

  fEventNum[iModule][iChannel] += numEvents;
}

void NFADC400Process::DataRL4(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL();
  Int_t numEvents = 0;
  if (recordingLength < 16)
    numEvents = 512;
  else
    numEvents = 0x1000/recordingLength;

  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    NFADC400Event4 *event = (NFADC400Event4 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[6];
    fAdc.NFADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 6; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.NFADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x2) >> 1);
    event -> SetCountTrigger((tPattern & 0x1));

    UShort_t data[RL4] = {0};
    fAdc.NFADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL4; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    if (fEventNum[iModule][iChannel] + iEvent + 1 >= fHeader[iModule] -> GetNumEvents()) {
      fEventNum[iModule][iChannel] += iEvent + 1;
      break;
    }
  }

  fEventNum[iModule][iChannel] += numEvents;
}

void NFADC400Process::DataRL8(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL();
  Int_t numEvents = 0;
  if (recordingLength < 16)
    numEvents = 512;
  else
    numEvents = 0x1000/recordingLength;

  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    NFADC400Event8 *event = (NFADC400Event8 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[6];
    fAdc.NFADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 6; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.NFADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x2) >> 1);
    event -> SetCountTrigger((tPattern & 0x1));

    UShort_t data[RL8] = {0};
    fAdc.NFADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL8; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    if (fEventNum[iModule][iChannel] + iEvent + 1 >= fHeader[iModule] -> GetNumEvents()) {
      fEventNum[iModule][iChannel] += iEvent + 1;
      break;
    }
  }

  fEventNum[iModule][iChannel] += numEvents;
}

void NFADC400Process::DataRL16(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL();
  Int_t numEvents = 0;
  if (recordingLength < 16)
    numEvents = 512;
  else
    numEvents = 0x1000/recordingLength;

  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    NFADC400Event16 *event = (NFADC400Event16 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[6];
    fAdc.NFADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 6; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.NFADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x2) >> 1);
    event -> SetCountTrigger((tPattern & 0x1));

    UShort_t data[RL16] = {0};
    fAdc.NFADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL16; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    if (fEventNum[iModule][iChannel] + iEvent + 1 >= fHeader[iModule] -> GetNumEvents()) {
      fEventNum[iModule][iChannel] += iEvent + 1;
      break;
    }
  }

  fEventNum[iModule][iChannel] += numEvents;
}

void NFADC400Process::DataRL32(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL();
  Int_t numEvents = 0;
  if (recordingLength < 16)
    numEvents = 512;
  else
    numEvents = 0x1000/recordingLength;

  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    NFADC400Event32 *event = (NFADC400Event32 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[6];
    fAdc.NFADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 6; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.NFADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x2) >> 1);
    event -> SetCountTrigger((tPattern & 0x1));

    UShort_t data[RL32] = {0};
    fAdc.NFADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL32; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    if (fEventNum[iModule][iChannel] + iEvent + 1 >= fHeader[iModule] -> GetNumEvents()) {
      fEventNum[iModule][iChannel] += iEvent + 1;
      break;
    }
  }

  fEventNum[iModule][iChannel] += numEvents;
}

void NFADC400Process::DataRL64(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL();
  Int_t numEvents = 0;
  if (recordingLength < 16)
    numEvents = 512;
  else
    numEvents = 0x1000/recordingLength;

  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    NFADC400Event64 *event = (NFADC400Event64 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[6];
    fAdc.NFADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 6; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.NFADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x2) >> 1);
    event -> SetCountTrigger((tPattern & 0x1));

    UShort_t data[RL64] = {0};
    fAdc.NFADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL64; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    if (fEventNum[iModule][iChannel] + iEvent + 1 >= fHeader[iModule] -> GetNumEvents()) {
      fEventNum[iModule][iChannel] += iEvent + 1;
      break;
    }
  }

  fEventNum[iModule][iChannel] += numEvents;
}

void NFADC400Process::DataRL128(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL();
  Int_t numEvents = 0;
  if (recordingLength < 16)
    numEvents = 512;
  else
    numEvents = 0x1000/recordingLength;

  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    NFADC400Event128 *event = (NFADC400Event128 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[6];
    fAdc.NFADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 6; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.NFADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x2) >> 1);
    event -> SetCountTrigger((tPattern & 0x1));

    UShort_t data[RL128] = {0};
    fAdc.NFADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL128; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    if (fEventNum[iModule][iChannel] + iEvent + 1 >= fHeader[iModule] -> GetNumEvents()) {
      fEventNum[iModule][iChannel] += iEvent + 1;
      break;
    }
  }

  fEventNum[iModule][iChannel] += numEvents;
}

void NFADC400Process::DataRL256(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL();
  Int_t numEvents = 0;
  if (recordingLength < 16)
    numEvents = 512;
  else
    numEvents = 0x1000/recordingLength;

  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    NFADC400Event256 *event = (NFADC400Event256 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[6];
    fAdc.NFADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 6; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.NFADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x2) >> 1);
    event -> SetCountTrigger((tPattern & 0x1));

    UShort_t data[RL256] = {0};
    fAdc.NFADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL256; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    if (fEventNum[iModule][iChannel] + iEvent + 1 >= fHeader[iModule] -> GetNumEvents()) {
      fEventNum[iModule][iChannel] += iEvent + 1;
      break;
    }
  }

  fEventNum[iModule][iChannel] += numEvents;
}

void NFADC400Process::DataRL512(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL();
  Int_t numEvents = 0;
  if (recordingLength < 16)
    numEvents = 512;
  else
    numEvents = 0x1000/recordingLength;

  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    NFADC400Event512 *event = (NFADC400Event512 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[6];
    fAdc.NFADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 6; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.NFADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x2) >> 1);
    event -> SetCountTrigger((tPattern & 0x1));

    UShort_t data[RL512] = {0};
    fAdc.NFADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL512; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    if (fEventNum[iModule][iChannel] + iEvent + 1 >= fHeader[iModule] -> GetNumEvents()) {
      fEventNum[iModule][iChannel] += iEvent + 1;
      break;
    }
  }

  fEventNum[iModule][iChannel] += numEvents;
}

void NFADC400Process::DataRL1024(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL();
  Int_t numEvents = 0;
  if (recordingLength < 16)
    numEvents = 512;
  else
    numEvents = 0x1000/recordingLength;

  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    NFADC400Event1024 *event = (NFADC400Event1024 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[6];
    fAdc.NFADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 6; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.NFADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x2) >> 1);
    event -> SetCountTrigger((tPattern & 0x1));

    UShort_t data[RL1024] = {0};
    fAdc.NFADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL1024; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    if (fEventNum[iModule][iChannel] + iEvent + 1 >= fHeader[iModule] -> GetNumEvents()) {
      fEventNum[iModule][iChannel] += iEvent + 1;
      break;
    }
  }

  fEventNum[iModule][iChannel] += numEvents;
}

void NFADC400Process::DataRL2048(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL();
  Int_t numEvents = 0;
  if (recordingLength < 16)
    numEvents = 512;
  else
    numEvents = 0x1000/recordingLength;

  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    NFADC400Event2048 *event = (NFADC400Event2048 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[6];
    fAdc.NFADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 6; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.NFADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x2) >> 1);
    event -> SetCountTrigger((tPattern & 0x1));

    UShort_t data[RL2048] = {0};
    fAdc.NFADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL2048; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    if (fEventNum[iModule][iChannel] + iEvent + 1 >= fHeader[iModule] -> GetNumEvents()) {
      fEventNum[iModule][iChannel] += iEvent + 1;
      break;
    }
  }

  fEventNum[iModule][iChannel] += numEvents;
}

void NFADC400Process::DataRL4096(Int_t iModule, Int_t iChannel, Int_t bufferNum) {
  Int_t recordingLength = fHeader[iModule] -> GetRL();
  Int_t numEvents = 0;
  if (recordingLength < 16)
    numEvents = 512;
  else
    numEvents = 0x1000/recordingLength;

  for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
    NFADC400Event4096 *event = (NFADC400Event4096 *) fEvent[iModule][iChannel] -> ConstructedAt(fEventNum[iModule][iChannel] + iEvent);

    Char_t tTimeChar[6];
    fAdc.NFADC400read_TTIME(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent, tTimeChar);

    ULong64_t tTime = 0;
    for (Int_t iBit = 0; iBit < 6; iBit++)
      tTime += (((Int_t) (tTimeChar[iBit] & 0xFF)) << 8*iBit);

    event -> SetTriggerTime(tTime);

    Char_t tPattern;
    tPattern = fAdc.NFADC400read_TPattern(fNKUSB, fModuleID[iModule], iChannel + 1, bufferNum*numEvents + iEvent);

    event -> SetWidthTrigger((tPattern & 0x2) >> 1);
    event -> SetCountTrigger((tPattern & 0x1));

    UShort_t data[RL4096] = {0};
    fAdc.NFADC400read_BUFFER(fNKUSB, fModuleID[iModule], iChannel + 1, recordingLength, bufferNum*numEvents + iEvent, data);

    for (Int_t iData = 0; iData < RL4096; iData++)
      event -> SetADC(iData, (data[iData] & 0x3FF));

    event = NULL;

    if (fEventNum[iModule][iChannel] + iEvent + 1 >= fHeader[iModule] -> GetNumEvents()) {
      fEventNum[iModule][iChannel] += iEvent + 1;
      break;
    }
  }

  fEventNum[iModule][iChannel] += numEvents;
}
