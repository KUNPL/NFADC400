/////////////////////////////////////
//                                 //
//       NFADC400 module GUI       //
//      Setting Control Class      //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 10. 28           //
//                                 //
/////////////////////////////////////

#include "NFADC400Settings.hh"

void NFADC400Settings::Initialize()
{
  fUseSameModuleSetting = 0;
  
  for (Int_t iModule = 0; iModule < 2; iModule++) {
    fValueAddress[iModule] = 0;
    fIsActive[iModule] = 0;

    fValueRL[iModule] = 0;

    fUseSameChannelSetting[iModule] = 0;
    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fValueIP[iModule][iChannel] = 0;
      fValueID[iModule][iChannel] = 0;
      fValueAO[iModule][iChannel] = 0;
      fValueIAG[iModule][iChannel] = 0;
    }

    fValueCLT[iModule] = 0;
    fValueTOW[iModule] = 0;
    fValueTDC[iModule] = 0;

    fUseSameChannelTriggerSetting[iModule] = 0;
    for (Int_t iChannel = 0; iChannel < 2; iChannel++) {
      fValueThres[iModule][iChannel] = 0;
      fValueDT[iModule][iChannel] = 0;
      fValueCW[iModule][iChannel] = 0;

      fValueTMWidth[iModule][iChannel] = 0;
      fValueTMWidthThres[iModule][iChannel] = 0;
      fValueTMCount[iModule][iChannel] = 0;
      fValueTMCountThres[iModule][iChannel] = 0;
      fValueTMCountInterval[iModule][iChannel] = 0;
    }
  }

  fValueNumEvents = 0;
}

void NFADC400Settings::UseSameModuleSetting()
{
  fIsActive[1] = fIsActive[0];
  fValueRL[1] = fValueRL[0];
  
  if (fUseSameChannelSetting[0]) {
    fValueIP[1][0] = fValueIP[0][0];
    fValueID[1][0] = fValueID[0][0];
    fValueAO[1][0] = fValueAO[0][0];
    fValueIAG[1][0] = fValueIAG[0][0];

    UseSameChannelSetting(0);
    UseSameChannelSetting(1);
  } else {
    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fValueIP[1][iChannel] = fValueIP[0][iChannel];
      fValueID[1][iChannel] = fValueID[0][iChannel];
      fValueAO[1][iChannel] = fValueAO[0][iChannel];
      fValueIAG[1][iChannel] = fValueIAG[0][iChannel];
    }
  }

  fValueCLT[1] = fValueCLT[0];
  fValueTOW[1] = fValueTOW[0];
  fValueTDC[1] = fValueTDC[0];

  if (fUseSameChannelTriggerSetting[0]) {
    fValueThres[1][0] = fValueThres[0][0];
    fValueDT[1][0] = fValueDT[0][0];
    fValueCW[1][0] = fValueCW[0][0];

    fValueTMWidth[1][0] = fValueTMWidth[0][0];
    fValueTMWidthThres[1][0] = fValueTMWidthThres[0][0];
    fValueTMCount[1][0] = fValueTMCount[0][0];
    fValueTMCountThres[1][0] = fValueTMCountThres[0][0];
    fValueTMCountInterval[1][0] = fValueTMCountInterval[0][0];

    UseSameChannelTriggerSetting(0);
    UseSameChannelTriggerSetting(1);
  } else {
    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fValueThres[1][iChannel] = fValueThres[0][iChannel];
      fValueDT[1][iChannel] = fValueDT[0][iChannel];
      fValueCW[1][iChannel] = fValueCW[0][iChannel];

      fValueTMWidth[1][iChannel] = fValueTMWidth[0][iChannel];
      fValueTMWidthThres[1][iChannel] = fValueTMWidthThres[0][iChannel];
      fValueTMCount[1][iChannel] = fValueTMCount[0][iChannel];
      fValueTMCountThres[1][iChannel] = fValueTMCountThres[0][iChannel];
      fValueTMCountInterval[1][iChannel] = fValueTMCountInterval[0][iChannel];
    }
  }
}

void NFADC400Settings::UseSameChannelSetting(Int_t module)
{
  for (Int_t iChannel = 1; iChannel < 4; iChannel++) {
    fValueIP[module][iChannel] = fValueIP[module][0];
    fValueID[module][iChannel] = fValueID[module][0];
    fValueAO[module][iChannel] = fValueAO[module][0];
    fValueIAG[module][iChannel] = fValueIAG[module][0];
  }
}

void NFADC400Settings::UseSameChannelTriggerSetting(Int_t module)
{
  for (Int_t iChannel = 1; iChannel < 4; iChannel++) {
    fValueDT[module][iChannel] = fValueDT[module][0];
    fValueCW[module][iChannel] = fValueCW[module][0];

    fValueTMWidth[module][iChannel] = fValueTMWidth[module][0];
    fValueTMWidthThres[module][iChannel] = fValueTMWidth[module][0];
    fValueTMCount[module][iChannel] = fValueTMCount[module][0];
    fValueTMCountThres[module][iChannel] = fValueTMCountThres[module][0];
    fValueTMCountInterval[module][iChannel] = fValueTMCountInterval[module][0];
  }
}
