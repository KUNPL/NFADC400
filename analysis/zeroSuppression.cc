#include "NFADC400Constants.hh"
#include "NFADC400Header.hh"
#include "NFADC400Event.hh"

#include "Riostream.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TString.h"

void DataRL1(ULong64_t numEvents, TClonesArray *writeArray, NFADC400Event *event)
{
  NFADC400Event1 *oldEvent = (NFADC400Event1 *) event;
  NFADC400Event1 *newEvent = (NFADC400Event1 *) writeArray -> ConstructedAt(numEvents);

  newEvent -> SetEventID(oldEvent -> GetEventID());
  newEvent -> SetNumData(oldEvent -> GetNumData());
  newEvent -> SetWidthTrigger(oldEvent -> GetWidthTrigger());
  newEvent -> SetCountTrigger(oldEvent -> GetCountTrigger());
  newEvent -> SetTriggerTime(oldEvent -> GetTriggerTime());
  
  for (Int_t iTb = 0; iTb < oldEvent -> GetNumData(); iTb++) 
    newEvent -> SetADC(iTb, oldEvent -> GetADC(iTb));
}

void DataRL2(ULong64_t numEvents, TClonesArray *writeArray, NFADC400Event* event)
{
  NFADC400Event2 *oldEvent = (NFADC400Event2 *) event;
  NFADC400Event2 *newEvent = (NFADC400Event2 *) writeArray -> ConstructedAt(numEvents);

  newEvent -> SetEventID(oldEvent -> GetEventID());
  newEvent -> SetNumData(oldEvent -> GetNumData());
  newEvent -> SetWidthTrigger(oldEvent -> GetWidthTrigger());
  newEvent -> SetCountTrigger(oldEvent -> GetCountTrigger());
  newEvent -> SetTriggerTime(oldEvent -> GetTriggerTime());
  
  for (Int_t iTb = 0; iTb < oldEvent -> GetNumData(); iTb++) 
    newEvent -> SetADC(iTb, oldEvent -> GetADC(iTb));
}

void DataRL4(ULong64_t numEvents, TClonesArray *writeArray, NFADC400Event* event)
{
  NFADC400Event4 *oldEvent = (NFADC400Event4 *) event;
  NFADC400Event4 *newEvent = (NFADC400Event4 *) writeArray -> ConstructedAt(numEvents);

  newEvent -> SetEventID(oldEvent -> GetEventID());
  newEvent -> SetNumData(oldEvent -> GetNumData());
  newEvent -> SetWidthTrigger(oldEvent -> GetWidthTrigger());
  newEvent -> SetCountTrigger(oldEvent -> GetCountTrigger());
  newEvent -> SetTriggerTime(oldEvent -> GetTriggerTime());
  
  for (Int_t iTb = 0; iTb < oldEvent -> GetNumData(); iTb++) 
    newEvent -> SetADC(iTb, oldEvent -> GetADC(iTb));
}

void DataRL8(ULong64_t numEvents, TClonesArray *writeArray, NFADC400Event* event)
{
  NFADC400Event8 *oldEvent = (NFADC400Event8 *) event;
  NFADC400Event8 *newEvent = (NFADC400Event8 *) writeArray -> ConstructedAt(numEvents);

  newEvent -> SetEventID(oldEvent -> GetEventID());
  newEvent -> SetNumData(oldEvent -> GetNumData());
  newEvent -> SetWidthTrigger(oldEvent -> GetWidthTrigger());
  newEvent -> SetCountTrigger(oldEvent -> GetCountTrigger());
  newEvent -> SetTriggerTime(oldEvent -> GetTriggerTime());
  
  for (Int_t iTb = 0; iTb < oldEvent -> GetNumData(); iTb++) 
    newEvent -> SetADC(iTb, oldEvent -> GetADC(iTb));
}

void DataRL16(ULong64_t numEvents, TClonesArray *writeArray, NFADC400Event* event)
{
  NFADC400Event16 *oldEvent = (NFADC400Event16 *) event;
  NFADC400Event16 *newEvent = (NFADC400Event16 *) writeArray -> ConstructedAt(numEvents);

  newEvent -> SetEventID(oldEvent -> GetEventID());
  newEvent -> SetNumData(oldEvent -> GetNumData());
  newEvent -> SetWidthTrigger(oldEvent -> GetWidthTrigger());
  newEvent -> SetCountTrigger(oldEvent -> GetCountTrigger());
  newEvent -> SetTriggerTime(oldEvent -> GetTriggerTime());
  
  for (Int_t iTb = 0; iTb < oldEvent -> GetNumData(); iTb++) 
    newEvent -> SetADC(iTb, oldEvent -> GetADC(iTb));
}

void DataRL32(ULong64_t numEvents, TClonesArray *writeArray, NFADC400Event* event)
{
  NFADC400Event32 *oldEvent = (NFADC400Event32 *) event;
  NFADC400Event32 *newEvent = (NFADC400Event32 *) writeArray -> ConstructedAt(numEvents);

  newEvent -> SetEventID(oldEvent -> GetEventID());
  newEvent -> SetNumData(oldEvent -> GetNumData());
  newEvent -> SetWidthTrigger(oldEvent -> GetWidthTrigger());
  newEvent -> SetCountTrigger(oldEvent -> GetCountTrigger());
  newEvent -> SetTriggerTime(oldEvent -> GetTriggerTime());
  
  for (Int_t iTb = 0; iTb < oldEvent -> GetNumData(); iTb++) 
    newEvent -> SetADC(iTb, oldEvent -> GetADC(iTb));
}

void DataRL64(ULong64_t numEvents, TClonesArray *writeArray, NFADC400Event* event)
{
  NFADC400Event64 *oldEvent = (NFADC400Event64 *) event;
  NFADC400Event64 *newEvent = (NFADC400Event64 *) writeArray -> ConstructedAt(numEvents);

  newEvent -> SetEventID(oldEvent -> GetEventID());
  newEvent -> SetNumData(oldEvent -> GetNumData());
  newEvent -> SetWidthTrigger(oldEvent -> GetWidthTrigger());
  newEvent -> SetCountTrigger(oldEvent -> GetCountTrigger());
  newEvent -> SetTriggerTime(oldEvent -> GetTriggerTime());
  
  for (Int_t iTb = 0; iTb < oldEvent -> GetNumData(); iTb++) 
    newEvent -> SetADC(iTb, oldEvent -> GetADC(iTb));
}

void DataRL128(ULong64_t numEvents, TClonesArray *writeArray, NFADC400Event* event)
{
  NFADC400Event128 *oldEvent = (NFADC400Event128 *) event;
  NFADC400Event128 *newEvent = (NFADC400Event128 *) writeArray -> ConstructedAt(numEvents);

  newEvent -> SetEventID(oldEvent -> GetEventID());
  newEvent -> SetNumData(oldEvent -> GetNumData());
  newEvent -> SetWidthTrigger(oldEvent -> GetWidthTrigger());
  newEvent -> SetCountTrigger(oldEvent -> GetCountTrigger());
  newEvent -> SetTriggerTime(oldEvent -> GetTriggerTime());
  
  for (Int_t iTb = 0; iTb < oldEvent -> GetNumData(); iTb++) 
    newEvent -> SetADC(iTb, oldEvent -> GetADC(iTb));
}

void DataRL256(ULong64_t numEvents, TClonesArray *writeArray, NFADC400Event* event)
{
  NFADC400Event256 *oldEvent = (NFADC400Event256 *) event;
  NFADC400Event256 *newEvent = (NFADC400Event256 *) writeArray -> ConstructedAt(numEvents);

  newEvent -> SetEventID(oldEvent -> GetEventID());
  newEvent -> SetNumData(oldEvent -> GetNumData());
  newEvent -> SetWidthTrigger(oldEvent -> GetWidthTrigger());
  newEvent -> SetCountTrigger(oldEvent -> GetCountTrigger());
  newEvent -> SetTriggerTime(oldEvent -> GetTriggerTime());
  
  for (Int_t iTb = 0; iTb < oldEvent -> GetNumData(); iTb++) 
    newEvent -> SetADC(iTb, oldEvent -> GetADC(iTb));
}

void DataRL512(ULong64_t numEvents, TClonesArray *writeArray, NFADC400Event* event)
{
  NFADC400Event512 *oldEvent = (NFADC400Event512 *) event;
  NFADC400Event512 *newEvent = (NFADC400Event512 *) writeArray -> ConstructedAt(numEvents);

  newEvent -> SetEventID(oldEvent -> GetEventID());
  newEvent -> SetNumData(oldEvent -> GetNumData());
  newEvent -> SetWidthTrigger(oldEvent -> GetWidthTrigger());
  newEvent -> SetCountTrigger(oldEvent -> GetCountTrigger());
  newEvent -> SetTriggerTime(oldEvent -> GetTriggerTime());
  
  for (Int_t iTb = 0; iTb < oldEvent -> GetNumData(); iTb++) 
    newEvent -> SetADC(iTb, oldEvent -> GetADC(iTb));
}

void DataRL1024(ULong64_t numEvents, TClonesArray *writeArray, NFADC400Event* event)
{
  NFADC400Event1024 *oldEvent = (NFADC400Event1024 *) event;
  NFADC400Event1024 *newEvent = (NFADC400Event1024 *) writeArray -> ConstructedAt(numEvents);

  newEvent -> SetEventID(oldEvent -> GetEventID());
  newEvent -> SetNumData(oldEvent -> GetNumData());
  newEvent -> SetWidthTrigger(oldEvent -> GetWidthTrigger());
  newEvent -> SetCountTrigger(oldEvent -> GetCountTrigger());
  newEvent -> SetTriggerTime(oldEvent -> GetTriggerTime());
  
  for (Int_t iTb = 0; iTb < oldEvent -> GetNumData(); iTb++) 
    newEvent -> SetADC(iTb, oldEvent -> GetADC(iTb));
}

void DataRL2048(ULong64_t numEvents, TClonesArray *writeArray, NFADC400Event* event)
{
  NFADC400Event2048 *oldEvent = (NFADC400Event2048 *) event;
  NFADC400Event2048 *newEvent = (NFADC400Event2048 *) writeArray -> ConstructedAt(numEvents);

  newEvent -> SetEventID(oldEvent -> GetEventID());
  newEvent -> SetNumData(oldEvent -> GetNumData());
  newEvent -> SetWidthTrigger(oldEvent -> GetWidthTrigger());
  newEvent -> SetCountTrigger(oldEvent -> GetCountTrigger());
  newEvent -> SetTriggerTime(oldEvent -> GetTriggerTime());
  
  for (Int_t iTb = 0; iTb < oldEvent -> GetNumData(); iTb++) 
    newEvent -> SetADC(iTb, oldEvent -> GetADC(iTb));
}

void DataRL4096(ULong64_t numEvents, TClonesArray *writeArray, NFADC400Event* event)
{
  NFADC400Event4096 *oldEvent = (NFADC400Event4096 *) event;
  NFADC400Event4096 *newEvent = (NFADC400Event4096 *) writeArray -> ConstructedAt(numEvents);

  newEvent -> SetEventID(oldEvent -> GetEventID());
  newEvent -> SetNumData(oldEvent -> GetNumData());
  newEvent -> SetWidthTrigger(oldEvent -> GetWidthTrigger());
  newEvent -> SetCountTrigger(oldEvent -> GetCountTrigger());
  newEvent -> SetTriggerTime(oldEvent -> GetTriggerTime());
  
  for (Int_t iTb = 0; iTb < oldEvent -> GetNumData(); iTb++) 
    newEvent -> SetADC(iTb, oldEvent -> GetADC(iTb));
}


Int_t main(Int_t argc, Char_t ** argv) {
  if (argc < 2) {
    cout << "===============================================================" << endl;
    cout << "  Usage: " << endl;
    cout << "    " << argv[0] << " FILENAME" << endl;
    cout << endl;  
    cout << "    Modify zeroSuppression.setting file to change settings." << endl;
    cout << "===============================================================" << endl;

    return 0;
  }

  Double_t fPedestalIn = 0;
  Double_t fThreshold = 0;
  Bool_t fSelectedMod[2] = {0};
  Bool_t fSelectedCh[2][4] = {{0}};

  ifstream setting("zeroSuppression.setting");
  if (setting.is_open()) {
    setting >> fPedestalIn >> fThreshold;
    for (Int_t iItem = 0; iItem < 8; iItem++)
      setting >> fSelectedCh[iItem/4][iItem%4];

    for (Int_t iCh = 0; iCh < 4; iCh++) {
      fSelectedMod[0] |= fSelectedCh[0][iCh];
      fSelectedMod[1] |= fSelectedCh[1][iCh];
    }

    cout << "== Pedestal: ";
    if (fPedestalIn == 0)
      cout << "values in header" << endl;
    else 
      cout << fPedestalIn << endl;
    cout << "== Threshold: " << fThreshold << endl;
    for (Int_t iMod = 0; iMod < 2; iMod++) {
      cout << "== Module " << iMod + 1 << ": " << (fSelectedMod[iMod] ? "Used" : "Not used") << endl;
      for (Int_t iCh = 0; iCh < 4; iCh++)
        cout << "=== Ch " << iCh + 1 << ": " << (fSelectedCh[iMod][iCh] ? "Used" : "Not used") << endl;
    }
    cout << endl;
  } else {
    cout << "== Error!: Setting file doesn't exist!" << endl;
    cout << endl;
    cout << "== Terminate program!" << endl;

    return 0;
  }

  ULong64_t fNumEvents[2] = {0};
  Double_t fNumTbs[2] = {0};
  Double_t fPedestal[2][4] = {{0}};

  TClonesArray *fEventArray[2][4] = {{NULL}};

  TFile *openFile = new TFile(argv[1]);
  TTree *openTree[2][4] = {{NULL}};
  NFADC400Header *fHeader[2] = {NULL};
  for (Int_t iMod = 0; iMod < 2; iMod++) {
    if (fSelectedMod[iMod]) {
      fHeader[iMod] = (NFADC400Header *) openFile -> Get(Form("Mod%d", iMod + 1));
      fNumEvents[iMod] = fHeader[iMod] -> GetNumEvents();
      fNumTbs[iMod] = fHeader[iMod] -> GetRL()*128;
      for (Int_t iCh = 0; iCh < 4; iCh++) {
        if (fSelectedCh[iMod][iCh]) {
          openTree[iMod][iCh] = (TTree *) openFile -> Get(Form("Mod%dCh%d", iMod + 1, iCh + 1));
          openTree[iMod][iCh] -> SetBranchAddress("events", &fEventArray[iMod][iCh]);
          openTree[iMod][iCh] -> GetEntry(0);
        }

        if (fPedestalIn == 0)
          fPedestal[iMod][iCh] = fHeader[iMod] -> GetPedestal(iCh);
        else
          fPedestal[iMod][iCh] = fPedestalIn;
      }
    }
  }

  if (fSelectedMod[0] & fSelectedMod[1]) {
    cout << "== Error!: The event numbers of each module are different!" << endl;
    cout << "=== Event number in module 1: " << fNumEvents[0] << endl;
    cout << "=== Event number in module 2: " << fNumEvents[1] << endl;
    cout << endl;
    cout << "== Terminate program!" << endl;

    return 0;
  }

  TString outFile = argv[1];
  outFile.ReplaceAll(".root", ".zeroSuppressed.root");
  TFile *writeFile = new TFile(outFile.Data(), "recreate");
  TTree *writeTree[2][4] = {{NULL}};
  TClonesArray *writeArray[2][4] = {{NULL}};
  for (Int_t iMod = 0; iMod < 2; iMod++) {
    if (fSelectedMod[iMod]) {
      for (Int_t iCh = 0; iCh < 4; iCh++) {
        if (fSelectedCh[iCh]) {
          writeTree[iMod][iCh] = new TTree(Form("Mod%dCh%d", iMod + 1, iCh + 1), "");
          writeArray[iMod][iCh] = new TClonesArray(Form("NFADC400Event%d", (Int_t)(fNumTbs[iMod]/128)));
          writeTree[iMod][iCh] -> Branch("events", "TClonesArray", &writeArray[iMod][iCh]);
        }
      }
    }
  }

  ULong64_t fNewNumEvents[2] = {0};
  NFADC400Event *event[2][4] = {{NULL}};

  for (Int_t iEvent = 0; iEvent < (fSelectedMod[0] ? fNumEvents[0] : fNumEvents[1]); iEvent++) {
    Bool_t passCriterion = kFALSE;

    for (Int_t iMod = 0; iMod < 2; iMod++) {
      if (fSelectedMod[iMod]) {
        for (Int_t iCh = 0; iCh < 4; iCh++) {
          if (fSelectedCh[iCh]) {
            event[iMod][iCh] = (NFADC400Event *) fEventArray[iMod][iCh] -> At(iEvent);
            Double_t *adc = event[iMod][iCh] -> GetADC();

            Double_t peak = 0;
            for (Int_t iTb = 0; iTb < fNumTbs[iMod]; iTb++) {
              peak = fPedestal[iMod][iCh] - adc[iTb];

              if (peak > fThreshold) {
                passCriterion = kTRUE;
                break;
              }
            }

            if (passCriterion == kTRUE)
              break;
          }
        }
      }

      if (passCriterion == kTRUE)
        break;
    }

    if (!passCriterion)
      continue;

    for (Int_t iMod = 0; iMod < 2; iMod++) {
      if (fSelectedMod[iMod]) {
        for (Int_t iCh = 0; iCh < 4; iCh++) {
          if (fSelectedCh[iCh]) {
            event[iMod][iCh] = (NFADC400Event *) fEventArray[iMod][iCh] -> At(iEvent);
            switch ((Int_t)(fNumTbs[iMod]/128)) {
              case 1: DataRL1(fNewNumEvents[iMod], writeArray[iMod][iCh], event[iMod][iCh]); break;
              case 2: DataRL2(fNewNumEvents[iMod], writeArray[iMod][iCh], event[iMod][iCh]); break;
              case 4: DataRL4(fNewNumEvents[iMod], writeArray[iMod][iCh], event[iMod][iCh]); break;
              case 8: DataRL8(fNewNumEvents[iMod], writeArray[iMod][iCh], event[iMod][iCh]); break;
              case 16: DataRL16(fNewNumEvents[iMod], writeArray[iMod][iCh], event[iMod][iCh]); break;
              case 32: DataRL32(fNewNumEvents[iMod], writeArray[iMod][iCh], event[iMod][iCh]); break;
              case 64: DataRL64(fNewNumEvents[iMod], writeArray[iMod][iCh], event[iMod][iCh]); break;
              case 128: DataRL128(fNewNumEvents[iMod], writeArray[iMod][iCh], event[iMod][iCh]); break;
              case 256: DataRL256(fNewNumEvents[iMod], writeArray[iMod][iCh], event[iMod][iCh]); break;
              case 512: DataRL512(fNewNumEvents[iMod], writeArray[iMod][iCh], event[iMod][iCh]); break;
              case 1024: DataRL1024(fNewNumEvents[iMod], writeArray[iMod][iCh], event[iMod][iCh]); break;
              case 2048: DataRL2048(fNewNumEvents[iMod], writeArray[iMod][iCh], event[iMod][iCh]); break;
              case 4096: DataRL4096(fNewNumEvents[iMod], writeArray[iMod][iCh], event[iMod][iCh]); break;
            }
          }
        }

      fNewNumEvents[iMod]++;
      }
    }
  }

  for (Int_t iMod = 0; iMod < 2; iMod++) {
    if (fSelectedMod[iMod]) {
      fHeader[iMod] -> SetNumEvents(fNewNumEvents[iMod]);
      fHeader[iMod] -> Write();
      
      for (Int_t iCh = 0; iCh < 4; iCh++) {
        if (fSelectedCh[iCh]) {
//          writeArray[iMod][iCh] -> Write();
          writeTree[iMod][iCh] -> Fill();
        }
      }
    }
  }

  for (Int_t iMod = 0; iMod < 2; iMod++) {
    if (fSelectedMod[iMod]) {
      for (Int_t iCh = 0; iCh < 4; iCh++) {
        if (fSelectedCh[iCh]) {
	  delete fEventArray[iMod][iCh];
          delete openTree[iMod][iCh];
        }
      }
    }
  }
  delete openFile;
  writeFile -> Write();

  cout << "== Success! Skimming out completed!" << endl;

  return 0;
}
