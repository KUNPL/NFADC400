/////////////////////////////////////
//                                 //
//        FADC400 module GUI       //
//       Setting Control Class     //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 10. 28           //
//                                 //
/////////////////////////////////////

#include "FADC400Settings.hh"

void FADC400Settings::Initialize()
{
  fUseSameModuleSetting = 0;
  
  for (Int_t iModule = 0; iModule < 2; iModule++) {
    fValueAddress[iModule] = 0;
    fIsActive[iModule] = 0;
    fUseSameChannelSetting[iModule] = 0;

    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fValueDSM[iModule][iChannel] = 0;
      fValueIP[iModule][iChannel] = 0;
      fValueID[iModule][iChannel] = 0;
      fValueAO[iModule][iChannel] = 0;
      fValueThres[iModule][iChannel] = 0;
      fValueRL[iModule][iChannel] = 0;
    }

    fValueDT[iModule] = 0;
    fValueDTApplied[iModule] = 0;
    fValueCW[iModule] = 0;
    fValueCWApplied[iModule] = 0;
    fValueCLT[iModule] = 0;
    fUseSameCGroupSetting[iModule] = 0;

    for (Int_t iCGroup = 0; iCGroup < 2; iCGroup++) {
      fValueTMCount[iModule][iCGroup] = 0;
      fValueTMCountOption[iModule][iCGroup] = 0;
      fValueTMWidth[iModule][iCGroup] = 0;
      fValueTMWidthOption[iModule][iCGroup] = 0;
    }
  }
}
