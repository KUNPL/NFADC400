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
