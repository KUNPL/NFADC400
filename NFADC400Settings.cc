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
    fUseSameChannelSetting[iModule] = 0;

    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fValueAC[iModule][iChannel] = 0;
      fValueDSM[iModule][iChannel] = 0;
      fValueIP[iModule][iChannel] = 0;
      fValueID[iModule][iChannel] = 0;
      fValueAO[iModule][iChannel] = 0;
      fValueThres[iModule][iChannel] = 0;
      fValueRL[iModule][iChannel] = 0;
    }

    fValueCLT[iModule] = 0;
    fUseSameCGroupSetting[iModule] = 0;

    for (Int_t iCGroup = 0; iCGroup < 2; iCGroup++) {
      fValueDT[iModule][iCGroup] = 0;
      fValueCW[iModule][iCGroup] = 0;

      fValueTMWidth[iModule][iCGroup] = 0;
      fValueTMWidthOption[iModule][iCGroup] = 0;
      fValueTMWidthThres[iModule][iCGroup] = 0;
      fValueTMCount[iModule][iCGroup] = 0;
      fValueTMCountOption[iModule][iCGroup] = 0;
      fValueTMCountThres[iModule][iCGroup][0] = 0;
      fValueTMCountThres[iModule][iCGroup][1] = 0;
      fValueTMCountInterval[iModule][iCGroup][0] = 0;
      fValueTMCountInterval[iModule][iCGroup][1] = 0;
    }
  }

  fValueNumEvents = 0;
}

void NFADC400Settings::UseSameModuleSetting()
{
  fIsActive[1] = fIsActive[0];
  
  if (fUseSameChannelSetting[0]) {
    fValueAC[1][0] = fValueAC[0][0];
    fValueDSM[1][0] = fValueDSM[0][0];
    fValueIP[1][0] = fValueIP[0][0];
    fValueID[1][0] = fValueID[0][0];
    fValueAO[1][0] = fValueAO[0][0];
    fValueThres[1][0] = fValueThres[0][0];
    fValueRL[1][0] = fValueRL[0][0];

    UseSameChannelSetting(0);
    UseSameChannelSetting(1);
  } else {
    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fValueAC[1][iChannel] = fValueAC[0][iChannel];
      fValueDSM[1][iChannel] = fValueDSM[0][iChannel];
      fValueIP[1][iChannel] = fValueIP[0][iChannel];
      fValueID[1][iChannel] = fValueID[0][iChannel];
      fValueAO[1][iChannel] = fValueAO[0][iChannel];
      fValueThres[1][iChannel] = fValueThres[0][iChannel];
      fValueRL[1][iChannel] = fValueRL[0][iChannel];
    }
  }

  fValueCLT[1] = fValueCLT[0];

  if (fUseSameCGroupSetting[0]) {
    fValueDT[1][0] = fValueDT[0][0];
    fValueCW[1][0] = fValueCW[0][0];

    fValueTMWidth[1][0] = fValueTMWidth[0][0];
    fValueTMWidthOption[1][0] = fValueTMWidthOption[0][0];
    fValueTMWidthThres[1][0] = fValueTMWidthThres[0][0];
    fValueTMCount[1][0] = fValueTMCount[0][0];
    fValueTMCountOption[1][0] = fValueTMCountOption[0][0];
    for (Int_t iElement = 0; iElement < 2; iElement++) {
      fValueTMCountThres[1][0][iElement] = fValueTMCountThres[0][0][iElement];
      fValueTMCountInterval[1][0][iElement] = fValueTMCountInterval[0][0][iElement];
    }

    UseSameCGroupSetting(0);
    UseSameCGroupSetting(1);
  } else {
    for (Int_t iCGroup = 0; iCGroup < 2; iCGroup++) {
      fValueDT[1][iCGroup] = fValueDT[0][iCGroup];
      fValueCW[1][iCGroup] = fValueCW[0][iCGroup];

      fValueTMWidth[1][iCGroup] = fValueTMWidth[0][iCGroup];
      fValueTMWidthOption[1][iCGroup] = fValueTMWidthOption[0][iCGroup];
      fValueTMWidthThres[1][iCGroup] = fValueTMWidthThres[0][iCGroup];
      fValueTMCount[1][iCGroup] = fValueTMCount[0][iCGroup];
      fValueTMCountOption[1][iCGroup] = fValueTMCountOption[0][iCGroup];
      for (Int_t iElement = 0; iElement < 2; iElement++) {
        fValueTMCountThres[1][iCGroup][iElement] = fValueTMCountThres[0][iCGroup][iElement];
        fValueTMCountInterval[1][iCGroup][iElement] = fValueTMCountInterval[0][iCGroup][iElement];
      }
    }
  }
}

void NFADC400Settings::UseSameChannelSetting(Int_t module)
{
  for (Int_t iChannel = 1; iChannel < 4; iChannel++) {
    fValueAC[module][iChannel] = fValueAC[module][0];
    fValueDSM[module][iChannel] = fValueDSM[module][0];
    fValueIP[module][iChannel] = fValueIP[module][0];
    fValueID[module][iChannel] = fValueID[module][0];
    fValueAO[module][iChannel] = fValueAO[module][0];
    fValueThres[module][iChannel] = fValueThres[module][0];
    fValueRL[module][iChannel] = fValueRL[module][0];
  }
}

void NFADC400Settings::UseSameCGroupSetting(Int_t module)
{
  fValueDT[module][1] = fValueDT[module][0];
  fValueCW[module][1] = fValueCW[module][0];

  fValueTMWidth[module][1] = fValueTMWidth[module][0];
  fValueTMWidthOption[module][1] = fValueTMWidthOption[module][0];
  fValueTMWidthThres[module][1] = fValueTMWidth[module][0];
  fValueTMCount[module][1] = fValueTMCount[module][0];
  fValueTMCountOption[module][1] = fValueTMCountOption[module][0];
  for (Int_t iElement = 0; iElement < 2; iElement++) {
    fValueTMCountThres[module][1][iElement] = fValueTMCountThres[module][0][iElement];
    fValueTMCountInterval[module][1][iElement] = fValueTMCountInterval[module][0][iElement];
  }
}
