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

#ifndef _FADC400SETTINGS_H_
#define _FADC400SETTINGS_H_

#include "TROOT.h"

class FADC400Settings
{
  public:
    void Initialize();
    void UseSameModuleSetting();
    void UseSameChannelSetting(Int_t);
    void UseSameCGroupSetting(Int_t);

    Bool_t fUseSameModuleSetting;
    Int_t fValueAddress[2];
    Bool_t fIsActive[2];
    Bool_t fUseSameChannelSetting[2];
    Bool_t fValueAC[2][4];
    Bool_t fValueDSM[2][4];
    Bool_t fValueIP[2][4];
    Int_t fValueID[2][4];
    Int_t fValueAO[2][4];
    Int_t fValueThres[2][4];
    Int_t fValueRL[2][4];
    Bool_t fValueCLT[2];
    Bool_t fUseSameCGroupSetting[2];
    Int_t fValueDT[2][2];
    Int_t fValueCW[2][2];
    Bool_t fValueTMWidth[2][2];
    Int_t fValueTMWidthOption[2][2];
    Double_t fValueTMWidthThres[2][2];
    Bool_t fValueTMCount[2][2];
    Int_t fValueTMCountOption[2][2];
    Int_t fValueTMCountThres[2][2][2];
    Int_t fValueTMCountInterval[2][2][2];

    Int_t fValueNumEvents;
};

#endif
