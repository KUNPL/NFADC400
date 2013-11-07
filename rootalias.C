//////////////////////////////////////
//                                  //
//   rootalias for NFADC400 Data    //
//                                  //
//  Author: Genie Jhang             //
// Contact: geniejhang@majimak.com  //
//    Date: 2013. 11. 06            //
//                                  //
//////////////////////////////////////

#include "NFADC400Constants.hh"

#include "Riostream.h"

#include <fstream>

using std::ofstream;

class NFADC400Header;

class NFADC400Event1;
class NFADC400Event2;
class NFADC400Event4;
class NFADC400Event8;
class NFADC400Event16;
class NFADC400Event32;
class NFADC400Event64;
class NFADC400Event128;
class NFADC400Event256;
class NFADC400Event512;
class NFADC400Event1024;
class NFADC400Event2048;
class NFADC400Event4096;

TFile *fFile;
TTree *fEventTree[2][4];
NFADC400Header *fHeader[2];
TClonesArray *fEvents;
TCanvas *fCvs;
TGraph *fGraph;
NFADC400Event1 *events1;
NFADC400Event2 *events2;
NFADC400Event4 *events4;
NFADC400Event8 *events8;
NFADC400Event16 *events16;
NFADC400Event32 *events32;
NFADC400Event64 *events64;
NFADC400Event128 *events128;
NFADC400Event256 *events256;
NFADC400Event512 *events512;
NFADC400Event1024 *events1024;
NFADC400Event2048 *events2048;
NFADC400Event4096 *events4096;

// User use functions
void load(TString file);
void plot(Int_t module, Int_t channel, Int_t eventNum);
void convert();

// Internel use functions
void Initialize();
void PrepareCanvas(Bool_t reset = 0);
Short_t *LoadData(Int_t, Int_t);
void WriteAsAscii(Int_t, Int_t, Int_t, Int_t, ofstream &);

// User use functions
void load(TString file)
{
  Initialize();

  gSystem -> Load("libNFADC400");

  if (fFile != NULL)
    delete fFile;

  fFile = new TFile(file);

  cout << endl;
  cout << " ====================================================" << endl;
  cout << endl;
  cout << "    Data file information" << endl;
  for (Int_t iModule = 0; iModule < 2; iModule++) {
    fHeader[iModule] = (NFADC400Header *) fFile -> Get(Form("Mod%d", iModule + 1));

    if (fHeader[iModule]) {
      Int_t numEvents = fHeader[iModule] -> GetNumEvents();
      Int_t space = 0;

      cout << endl;
      cout << "    Module " << iModule + 1 << " (# of events: " << numEvents << ")" << endl;
      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        Int_t count = 0;
        if (fHeader[iModule] -> GetAC(iChannel)) {
          cout << "    Channel: " << iChannel + 1 << " ";
          fEventTree[iModule][iChannel] = (TTree *) fFile -> Get(Form("Mod%dCh%d", iModule + 1, iChannel + 1));
          fEventTree[iModule][iChannel] -> SetBranchAddress("events", &fEvents);
          count++;
        }
      }
      cout << endl;
    }
  }
  cout << endl;
  cout << "    Try 'plot(eventNum, moduleNum, channelNum)'." << endl;
  cout << "    For example, the first event in Channel 1" << endl;
  cout << "    of Module 2, 'plot(2, 1, 0)'" << endl;
  cout << endl;
  cout << "    To see the module setting information," << endl;
  cout << "    for example, that of module 2, type" << endl;
  cout << "    'Mod2 -> Print()' after load()." << endl;
  cout << endl;
  cout << "    To see the channel setting information," << endl;
  cout << "    for example, that of channel 1 of module 2" << endl;
  cout << "    type 'Mod2 -> Print(1)' after load()." << endl;
  cout << endl;
  cout << " ====================================================" << endl;
  cout << endl;
}

void plot(Int_t module, Int_t channel, Int_t eventNum)
{
  module -= 1;
  channel -= 1;

  gSystem -> Load("libNFADC400");

  fEventTree[module][channel] -> GetEntry(0);

  Int_t numEvents = fEvents -> GetEntries();
  if (eventNum >= numEvents) {
    cout << " == There're " << numEvents << " events in Module " << module << " - Channel " << channel << "!" << endl;
    
    return;
  }

  if (fCvs != NULL)
    PrepareCanvas(1);
  else
    PrepareCanvas();

  Int_t recordingLength = fHeader[module] -> GetRL();
  Int_t numData = recordingLength*128;
  Short_t *adc = LoadData(eventNum, recordingLength);

  for (Int_t iData = 0; iData < numData; iData++)
    fGraph -> SetPoint(iData, iData, adc[iData]);

  fGraph -> Draw("APL same");
  fGraph -> SetTitle(Form("Module %d - Channel %d - Event %d", module + 1, channel + 1, eventNum));
  fGraph -> SetLineColor(2);
  fGraph -> SetMarkerColor(2);
  fGraph -> GetXaxis() -> SetTitle("Time Bucket (2.5 ns)");
  fGraph -> GetXaxis() -> CenterTitle();
  fGraph -> GetXaxis() -> SetLimits(-10, recordingLength*128 + 10);
  fGraph -> GetXaxis() -> SetRangeUser(-5, numData + 5);
  fGraph -> GetYaxis() -> SetTitle("ADC Channel (A.U.)");
  fGraph -> GetYaxis() -> CenterTitle();
  fGraph -> GetYaxis() -> SetLimits(-15, 1024 + 15);
  fGraph -> GetYaxis() -> SetRangeUser(-5, 1024 + 5);

  delete adc;
}

void convert()
{
  cout << endl;
  cout << " ================================================" << endl;
  cout << endl;
  cout << "   Note that the module and channel numbers" << endl;
  cout << "   are recorded to start form 0!" << endl;
  cout << endl;
  cout << " ================================================" << endl;
  cout << endl;

  gSystem -> Load("libNFADC400");

  for (Int_t iModule = 0; iModule < 2; iModule++) {
    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      if (fEventTree[iModule][iChannel] == NULL)
        continue;

      TString filename = fFile -> GetName();
      ofstream ascii(Form("%s-Mod%d-Ch%d.txt", (filename.Remove(filename.Length() - 5, 5)).Data(), iModule, iChannel));

      Int_t recordingLength = fHeader[iModule] -> GetRL();
      ascii << recordingLength*128 << " // # of lines in a event" << endl;
      ascii << setw(8) << "# module" << setw(12) << "channel" << setw(13) << "event" << setw(13) << "bin" << setw(13) << "adc" << endl;

      fEventTree[iModule][iChannel] -> GetEntry(0);
      Int_t numEvents = fEvents -> GetEntries();
      WriteAsAscii(iModule, iChannel, recordingLength, numEvents, ascii);
      ascii.close();
    }
  }
}

// Internal use functions
void Initialize()
{
  fFile = NULL;

  for (Int_t iModule = 0; iModule < 2; iModule++) {
    fHeader[iModule] = NULL;

    for (Int_t iChannel = 0; iChannel < 4; iChannel++)
      fEventTree[iModule][iChannel] = NULL;
  }

  fEvents = NULL;

  fCvs = NULL;
  fGraph = NULL;

  events1 = NULL;
  events2 = NULL;
  events4 = NULL;
  events8 = NULL;
  events16 = NULL;
  events32 = NULL;
  events64 = NULL;
  events128 = NULL;
  events256 = NULL;
  events512 = NULL;
  events1024 = NULL;
  events2048 = NULL;
  events4096 = NULL;
}

void PrepareCanvas(Bool_t reset)
{
  if (reset) {
    delete fGraph;

    fGraph = new TGraph();
  } else {
    gStyle -> SetPadLeftMargin(0.09);
    gStyle -> SetPadRightMargin(0.03);
    gStyle -> SetPadBottomMargin(0.1);
    gStyle -> SetTitleSize(0.05, "x");
    gStyle -> SetTitleOffset(1.00, "x");
    gStyle -> SetTitleSize(0.05, "y");
    gStyle -> SetTitleOffset(0.80, "y");
    fCvs = new TCanvas("fCvs", "", 800, 450);
    fGraph = new TGraph();
  }
}

Short_t *LoadData(Int_t eventNum, Int_t recordingLength)
{
  Short_t *adc = NULL;

  if (recordingLength == 1) {
    events1 = (NFADC400Event1 *) fEvents -> At(eventNum);
    adc = new Short_t[RL1];

    for (Int_t iData = 0; iData < RL1; iData++)
      adc[iData] = events1 -> GetADC(iData);
  } else if (recordingLength == 2) {
    events2 = (NFADC400Event2 *) fEvents -> At(eventNum);
    adc = new Short_t[RL2];

    for (Int_t iData = 0; iData < RL2; iData++)
      adc[iData] = events2 -> GetADC(iData);
  } else if (recordingLength == 4) {
    events4 = (NFADC400Event4 *) fEvents -> At(eventNum);
    adc = new Short_t[RL4];

    for (Int_t iData = 0; iData < RL4; iData++)
      adc[iData] = events4 -> GetADC(iData);
  } else if (recordingLength == 8) {
    events8 = (NFADC400Event8 *) fEvents -> At(eventNum);
    adc = new Short_t[RL8];

    for (Int_t iData = 0; iData < RL8; iData++)
      adc[iData] = events8 -> GetADC(iData);
  } else if (recordingLength == 16) {
    events16 = (NFADC400Event16 *) fEvents -> At(eventNum);
    adc = new Short_t[RL16];

    for (Int_t iData = 0; iData < RL16; iData++)
      adc[iData] = events16 -> GetADC(iData);
  } else if (recordingLength == 32) {
    events32 = (NFADC400Event32 *) fEvents -> At(eventNum);
    adc = new Short_t[RL32];

    for (Int_t iData = 0; iData < RL32; iData++)
      adc[iData] = events32 -> GetADC(iData);
  } else if (recordingLength == 64) {
    events64 = (NFADC400Event64 *) fEvents -> At(eventNum);
    adc = new Short_t[RL64];

    for (Int_t iData = 0; iData < RL64; iData++)
      adc[iData] = events64 -> GetADC(iData);
  } else if (recordingLength == 128) {
    events128 = (NFADC400Event128 *) fEvents -> At(eventNum);
    adc = new Short_t[RL128];

    for (Int_t iData = 0; iData < RL128; iData++)
      adc[iData] = events128 -> GetADC(iData);
  } else if (recordingLength == 256) {
    events256 = (NFADC400Event256 *) fEvents -> At(eventNum);
    adc = new Short_t[RL256];

    for (Int_t iData = 0; iData < RL256; iData++)
      adc[iData] = events256 -> GetADC(iData);
  } else if (recordingLength == 512) {
    events512 = (NFADC400Event512 *) fEvents -> At(eventNum);
    adc = new Short_t[RL512];

    for (Int_t iData = 0; iData < RL512; iData++)
      adc[iData] = events512 -> GetADC(iData);
  } else if (recordingLength == 1024) {
    events1024 = (NFADC400Event1024 *) fEvents -> At(eventNum);
    adc = new Short_t[RL1024];

    for (Int_t iData = 0; iData < RL1024; iData++)
      adc[iData] = events1024 -> GetADC(iData);
  } else if (recordingLength == 2048) {
    events2048 = (NFADC400Event2048 *) fEvents -> At(eventNum);
    adc = new Short_t[RL2048];

    for (Int_t iData = 0; iData < RL2048; iData++)
      adc[iData] = events2048 -> GetADC(iData);
  } else if (recordingLength == 4096) {
    events4096 = (NFADC400Event4096 *) fEvents -> At(eventNum);
    adc = new Short_t[RL4096];

    for (Int_t iData = 0; iData < RL4096; iData++)
      adc[iData] = events4096 -> GetADC(iData);
  }

  return adc;
}

void WriteAsAscii(Int_t module, Int_t channel, Int_t recordingLength, Int_t numEvents, ofstream &ascii)
{
  if (recordingLength == 1) {
    for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
      events1 = (NFADC400Event1 *) fEvents -> At(iEvent);
      for (Int_t iData = 0; iData < RL1; iData++)
        ascii << setw(8) << module << setw(12) << channel << setw(13) << iEvent << setw(13) << iData << setw(13) << events1 -> GetADC(iData) << endl;
    }
  } else if (recordingLength == 2) {
    for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
      events2 = (NFADC400Event2 *) fEvents -> At(iEvent);
      for (Int_t iData = 0; iData < RL2; iData++)
        ascii << setw(8) << module << setw(12) << channel << setw(13) << iEvent << setw(13) << iData << setw(13) << events2 -> GetADC(iData) << endl;
    }
  } else if (recordingLength == 4) {
    for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
      events4 = (NFADC400Event4 *) fEvents -> At(iEvent);
      for (Int_t iData = 0; iData < RL4; iData++)
        ascii << setw(8) << module << setw(12) << channel << setw(13) << iEvent << setw(13) << iData << setw(13) << events4 -> GetADC(iData) << endl;
    }
  } else if (recordingLength == 8) {
    for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
      events8 = (NFADC400Event8 *) fEvents -> At(iEvent);
      for (Int_t iData = 0; iData < RL8; iData++)
        ascii << setw(8) << module << setw(12) << channel << setw(13) << iEvent << setw(13) << iData << setw(13) << events8 -> GetADC(iData) << endl;
    }
  } else if (recordingLength == 16) {
    for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
      events16 = (NFADC400Event16 *) fEvents -> At(iEvent);
      for (Int_t iData = 0; iData < RL16; iData++)
        ascii << setw(8) << module << setw(12) << channel << setw(13) << iEvent << setw(13) << iData << setw(13) << events16 -> GetADC(iData) << endl;
    }
  } else if (recordingLength == 32) {
    for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
      events32 = (NFADC400Event32 *) fEvents -> At(iEvent);
      for (Int_t iData = 0; iData < RL32; iData++)
        ascii << setw(8) << module << setw(12) << channel << setw(13) << iEvent << setw(13) << iData << setw(13) << events32 -> GetADC(iData) << endl;
    }
  } else if (recordingLength == 64) {
    for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
      events64 = (NFADC400Event64 *) fEvents -> At(iEvent);
      for (Int_t iData = 0; iData < RL64; iData++)
        ascii << setw(8) << module << setw(12) << channel << setw(13) << iEvent << setw(13) << iData << setw(13) << events64 -> GetADC(iData) << endl;
    }
  } else if (recordingLength == 128) {
    for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
      events128 = (NFADC400Event128 *) fEvents -> At(iEvent);
      for (Int_t iData = 0; iData < RL128; iData++)
        ascii << setw(8) << module << setw(12) << channel << setw(13) << iEvent << setw(13) << iData << setw(13) << events128 -> GetADC(iData) << endl;
    }
  } else if (recordingLength == 256) {
    for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
      events256 = (NFADC400Event256 *) fEvents -> At(iEvent);
      for (Int_t iData = 0; iData < RL256; iData++)
        ascii << setw(8) << module << setw(12) << channel << setw(13) << iEvent << setw(13) << iData << setw(13) << events256 -> GetADC(iData) << endl;
    }
  } else if (recordingLength == 512) {
    for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
      events512 = (NFADC400Event512 *) fEvents -> At(iEvent);
      for (Int_t iData = 0; iData < RL512; iData++)
        ascii << setw(8) << module << setw(12) << channel << setw(13) << iEvent << setw(13) << iData << setw(13) << events512 -> GetADC(iData) << endl;
    }
  } else if (recordingLength == 1024) {
    for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
      events1024 = (NFADC400Event1024 *) fEvents -> At(iEvent);
      for (Int_t iData = 0; iData < RL1024; iData++)
        ascii << setw(8) << module << setw(12) << channel << setw(13) << iEvent << setw(13) << iData << setw(13) << events1024 -> GetADC(iData) << endl;
    }
  } else if (recordingLength == 2048) {
    for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
      events2048 = (NFADC400Event2048 *) fEvents -> At(iEvent);
      for (Int_t iData = 0; iData < RL2048; iData++)
        ascii << setw(8) << module << setw(12) << channel << setw(13) << iEvent << setw(13) << iData << setw(13) << events2048 -> GetADC(iData) << endl;
    }
  } else if (recordingLength == 4096) {
    for (Int_t iEvent = 0; iEvent < numEvents; iEvent++) {
      events4096 = (NFADC400Event4096 *) fEvents -> At(iEvent);
      for (Int_t iData = 0; iData < RL4096; iData++)
        ascii << setw(8) << module << setw(12) << channel << setw(13) << iEvent << setw(13) << iData << setw(13) << events4096 -> GetADC(iData) << endl;
    }
  }
}
