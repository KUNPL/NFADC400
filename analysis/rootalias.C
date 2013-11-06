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

class NFADC400Header;

TFile *fFile;
TTree *fEventTree[2][4];
NFADC400Header *fHeader[2];
TClonesArray *fEvents;
TCanvas *fCvs;
TGraph *fGraph;

// User use
void load(TString file);
void plot(Int_t eventNum, Int_t module, Int_t channel);

// Internel use
void Initialize();
void PrepareCanvas(Bool_t reset = 0);
Short_t *LoadData(Int_t, Int_t);

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
    // Right below will be used in release version
    // fHeader[iModule] = (NFADC400Header *) fFile -> Get(Form("Mod%d", iModule + 1));
    fHeader[iModule] = (NFADC400Header *) fFile -> Get(Form("module%d", iModule + 1)); 

    if (fHeader[iModule]) {
      Int_t numEvents = fHeader[iModule] -> GetNumEvents();
      Int_t space = 0;

      cout << endl;
      cout << "    Module " << iModule + 1 << " (# of events: " << numEvents << ")" << endl;
      for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
        Int_t count = 0;
        if (fHeader[iModule] -> GetAC(iChannel)) {
          cout << "    Channel: " << iChannel + 1 << " ";
          // Right below will be used in release version
          // fEventTree[iModule][iChannel] = (TTree *) fFile -> Get(Form("Mod%dCh%d", iModule + 1, iChannel + 1));
          fEventTree[iModule][iChannel] = (TTree *) fFile -> Get(Form("Mod%d-Ch%d", iModule, iChannel));
          fEventTree[iModule][iChannel] -> SetBranchAddress("events", &fEvents);
          count++;
        }
      }
      cout << endl;
    }
  }
  cout << endl;
  cout << " ====================================================" << endl;
  cout << endl;
}

void plot(Int_t eventNum, Int_t module, Int_t channel)
{
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
    NFADC400Event1 *events = (NFADC400Event1 *) fEvents -> At(eventNum);
    adc = new Short_t[RL1];

    for (Int_t iData = 0; iData < RL1; iData++)
      adc[iData] = events -> GetADC(iData);
  } else if (recordingLength == 2) {
    NFADC400Event2 *events = (NFADC400Event2 *) fEvents -> At(eventNum);
    adc = new Short_t[RL2];

    for (Int_t iData = 0; iData < RL2; iData++)
      adc[iData] = events -> GetADC(iData);
  } else if (recordingLength == 4) {
    NFADC400Event4 *events = (NFADC400Event4 *) fEvents -> At(eventNum);
    adc = new Short_t[RL4];

    for (Int_t iData = 0; iData < RL4; iData++)
      adc[iData] = events -> GetADC(iData);
  } else if (recordingLength == 8) {
    NFADC400Event8 *events = (NFADC400Event8 *) fEvents -> At(eventNum);
    adc = new Short_t[RL8];

    for (Int_t iData = 0; iData < RL8; iData++)
      adc[iData] = events -> GetADC(iData);
  } else if (recordingLength == 16) {
    NFADC400Event16 *events = (NFADC400Event16 *) fEvents -> At(eventNum);
    adc = new Short_t[RL16];

    for (Int_t iData = 0; iData < RL16; iData++)
      adc[iData] = events -> GetADC(iData);
  } else if (recordingLength == 32) {
    NFADC400Event32 *events = (NFADC400Event32 *) fEvents -> At(eventNum);
    adc = new Short_t[RL32];

    for (Int_t iData = 0; iData < RL32; iData++)
      adc[iData] = events -> GetADC(iData);
  } else if (recordingLength == 64) {
    NFADC400Event64 *events = (NFADC400Event64 *) fEvents -> At(eventNum);
    adc = new Short_t[RL64];

    for (Int_t iData = 0; iData < RL64; iData++)
      adc[iData] = events -> GetADC(iData);
  } else if (recordingLength == 128) {
    NFADC400Event128 *events = (NFADC400Event128 *) fEvents -> At(eventNum);
    adc = new Short_t[RL128];

    for (Int_t iData = 0; iData < RL128; iData++)
      adc[iData] = events -> GetADC(iData);
  } else if (recordingLength == 256) {
    NFADC400Event256 *events = (NFADC400Event256 *) fEvents -> At(eventNum);
    adc = new Short_t[RL256];

    for (Int_t iData = 0; iData < RL256; iData++)
      adc[iData] = events -> GetADC(iData);
  } else if (recordingLength == 512) {
    NFADC400Event512 *events = (NFADC400Event512 *) fEvents -> At(eventNum);
    adc = new Short_t[RL512];

    for (Int_t iData = 0; iData < RL512; iData++)
      adc[iData] = events -> GetADC(iData);
  } else if (recordingLength == 1024) {
    NFADC400Event1024 *events = (NFADC400Event1024 *) fEvents -> At(eventNum);
    adc = new Short_t[RL1024];

    for (Int_t iData = 0; iData < RL1024; iData++)
      adc[iData] = events -> GetADC(iData);
  } else if (recordingLength == 2048) {
    NFADC400Event2048 *events = (NFADC400Event2048 *) fEvents -> At(eventNum);
    adc = new Short_t[RL2048];

    for (Int_t iData = 0; iData < RL2048; iData++)
      adc[iData] = events -> GetADC(iData);
  } else if (recordingLength == 4096) {
    NFADC400Event4096 *events = (NFADC400Event4096 *) fEvents -> At(eventNum);
    adc = new Short_t[RL4096];

    for (Int_t iData = 0; iData < RL4096; iData++)
      adc[iData] = events -> GetADC(iData);
  }

  return adc;
}
