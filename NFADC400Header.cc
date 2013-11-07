/////////////////////////////////////
//                                 //
//      NFADC400 module GUI        //
//   Header Data Container Class   //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 11. 01           //
//                                 //
/////////////////////////////////////

#include "NFADC400Header.hh"

#include "Riostream.h"

ClassImp(NFADC400Header);

NFADC400Header::NFADC400Header()
{
  Initialize();
}

NFADC400Header::~NFADC400Header()
{
}

void NFADC400Header::Initialize()
{
  fNumEvents = 0;

  fRL = 0;

  for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
    fAC[iChannel] = 0;
    fIP[iChannel] = 0;
    fID[iChannel] = 0;
    fAO[iChannel] = 0;
    fIAG[iChannel] = 0;
  }

  // Triggers' Header
  fCLT = 0;
  fTOW = 0;
  fTDC = 0;

  for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
    fThreshold[iChannel] = 0;
    fDT[iChannel] = 0;
    fCW[iChannel] = 0;
    fTMWidth[iChannel] = 0;
    fTMWidthThreshold[iChannel] = 0;
    fTMCount[iChannel] = 0;
    fTMCountThreshold[iChannel] = 0;
    fTMCountInterval[iChannel] = 0;
  }
}

void NFADC400Header::Print(Int_t channel)
{
  if (channel > 4 || channel < 0)
    return;

  if (channel == 0) {
    TString name = this -> GetName();

    cout << endl;
    cout << " =============" << endl;
    cout << " = " << name.ReplaceAll("Mod", "Module: ") << " ="<< endl;
    cout << " ============================================" << endl;
    cout << setw(30) << "Recording length: " << 0.32*fRL << " us" << endl;
    cout << setw(30) << "Number of data point: " << 128*fRL << endl;
    cout << endl;
    cout << setw(30) << "Trigger lookup table: " << (fCLT ? "OR" : "AND") << endl;
    cout << setw(30) << "Trigger output width: " << fTOW << " ns" << endl;
    cout << setw(30) << "Trigger daisy chain: " << (fTDC ? "disabled" : "enabled") << endl;
    cout << " ============================================" << endl;
    cout << endl;
  } else {
    channel -= 1;

    cout << endl;
    cout << " ==============" << endl;
    cout << " = Channel: " << channel + 1 << " =" << setw(26) << "Active: " << (fAC[channel] ? "yes" : "no") << endl;
    cout << " ============================================" << endl;
    if (fAC[channel]) {
      cout << setw(30) << "Input polarity: " << (fIP[channel] ? "positive" : "negative") << endl;
      cout << setw(30) << "Input delay: " << fID[channel] << " ns" << endl;
      cout << setw(30) << "ADC offset: " << fAO[channel] << endl;
      cout << setw(30) << "Input amplifier gain: " << fIAG[channel] << endl;
      cout << " --------------------------------------------" << endl;
      cout << "  Trigger Output Setting" << endl;
      cout << " --------------------------------------------" << endl;
      cout << setw(30) << "Threshold: " << fThreshold[channel] << endl;
      cout << setw(30) << "Deadtime: " << fDT[channel] << endl;
      cout << setw(30) << "Coincidence width: " << fCW[channel] << endl;
      cout << setw(30) << "Width trigger mode: " << (fTMWidth[channel] ? "active" : "inactive") << endl;
      cout << setw(30) << "Width trigger threshold: " << fTMWidthThreshold[channel] << " ns" << endl;
      cout << setw(30) << "Count trigger mode: " << (fTMCount[channel] ? "active" : "inactive") << endl;
      cout << setw(30) << "Count trigger threshold: " << fTMCountThreshold[channel] << endl;
      cout << setw(30) << "Count trigger interval: " << fTMCountInterval[channel] << " ns" << endl;
      cout << " --------------------------------------------" << endl;
    }
    cout << " ============================================" << endl;
    cout << endl;
  }
}
