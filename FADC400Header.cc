/////////////////////////////////////
//                                 //
//       FADC400 module GUI        //
//   Header Data Container Class   //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 11. 01           //
//                                 //
/////////////////////////////////////

#include "FADC400Header.hh"

ClassImp(FADC400Header);

FADC400Header::FADC400Header()
{
  Initialize();
}

FADC400Header::~FADC400Header()
{
}

void FADC400Header::Initialize()
{
    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fDSM[iChannel] = 0;
      fIP[iChannel] = 0;
      fID[iChannel] = 0;
      fAO[iChannel] = 0;
      fThreshold[iChannel] = 0;
      fRL[iChannel] = 0;
    }

    // Triggers' Header
    fCLT = 0;
    for (Int_t iCGroup = 0; iCGroup < 2; iCGroup++) {
      fDT[iCGroup] = 0;
      fCW[iCGroup] = 0;
      fTMWidth[iCGroup] = 0;
      fTMWidthOption[iCGroup] = 0;
      fTMWidthThreshold[iCGroup] = 0;
      fTMCount[iCGroup] = 0;
      fTMCountOption[iCGroup] = 0;
      fTMCountThreshold[iCGroup][0] = 0;
      fTMCountThreshold[iCGroup][1] = 0;
      fTMCountInterval[iCGroup][0] = 0;
      fTMCountInterval[iCGroup][1] = 0;
    }
}
