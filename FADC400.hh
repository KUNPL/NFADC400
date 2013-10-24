/////////////////////////////////////
//                                 //
//        FADC400 module GUI       //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 10. 21           //
//                                 //
/////////////////////////////////////

#ifndef _FADC400_H_
#define _FADC400_H_

#include "TString.h"

#include "TGWindow.h"
#include "TGFrame.h"
#include "TGTab.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGComboBox.h"
#include "TGButtonGroup.h"

class FADC400 : public TGMainFrame
{
  public:
    FADC400(const TGWindow *, UInt_t, UInt_t);
    virtual ~FADC400();

    void SetSameModuleSetting(Bool_t);

    void SetAddress(Int_t);
    void SetActive(Bool_t);
    void SetSameChannelSetting(Bool_t);

    void SetDSM(Int_t i);

  private:
    void Initialize();
    Bool_t fIsDebug;

    TGGroupFrame *fModuleFrame;
    TGButton *fSameModuleSettingButton;
    Bool_t fUseSameModuleSetting;

    TGTab *fModuleTab;
    TGCompositeFrame *fFADC[2];

    TGLabel *fTextAddress;

    TGComboBox *fCBAddress[2];
    Int_t fValueAddress[2];
    TGButton *fActive[2];
    Bool_t fIsActive[2];

    TGLabel *fTextDSM;
    TGHButtonGroup *fDSMGroup;
    TGRadioButton *fDSM[2][4][2];
    Bool_t fValueDSM[2][4];

    TGLabel *fTextIP;
    TGLabel *fTextID;
    TGLabel *fTextAO;
    TGLabel *fTextThres;
    TGLabel *fTextRL;

    TGGroupFrame *fChannelFrame[2];
    TGButton *fSameChannelSettingButton[2];
    Bool_t fUseSameChannelSetting[2];

    TGTab *fChannelTab[2];
    TGCompositeFrame *fCh[2][4];

  ClassDef(FADC400, 1);
};

#endif
