void sample() {
  gSystem -> Load("libNFADC400");

  TFile *file = new TFile("sample.root");

  TClonesArray *events = NULL;
  TTree *tree = (TTree *) file -> Get(Form("Mod%dCh%d", 2, 1));
    tree -> SetBranchAddress("events", &events);

  tree -> GetEntry(0);

  NFADC400Event4 *anEvent = NULL;
  Int_t numEvents = events -> GetEntries();
  for (Int_t i = 0; i < numEvents; i++) {
    anEvent = (NFADC400Event4 *) events -> At(i);

    Int_t numData = anEvent -> GetNumData();
    for (Int_t j = 0; j < numData; j++)
      cout << setw(5) << i << setw(5) << j << setw(5) << anEvent -> GetADC(j) << endl;
  }
}


