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

class FADC400 : public TGMainFrame
{
  public:
    FADC400(const TGWindow *, UInt_t, UInt_t);
    virtual ~FADC400();

    void SetSameModuleSetting(Bool_t);

    void SetAddress1(Int_t);
    void SetActive1(Bool_t);
    void SetSameChannelSetting1(Bool_t);

    void SetAddress2(Int_t);
    void SetActive2(Bool_t);
    void SetSameChannelSetting2(Bool_t);

  private:
    Bool_t fIsDebug;

    TGGroupFrame *fModuleFrame;
    TGButton *fSameModuleSettingButton;
    Bool_t fUseSameModuleSetting;

    TGTab *fModuleTab;
    TGCompositeFrame *fFADC1;
    TGCompositeFrame *fFADC2;

    // Variables used in common to two modules
    TGLabel *fTextAddress;

    TGLabel *fTextDSM;
    TGLabel *fTextIP;
    TGLabel *fTextID;
    TGLabel *fTextAO;
    TGLabel *fTextThres;
    TGLabel *fTextRL;

    // Module 1
    TGComboBox *fCBAddress1;
    Int_t fValueAddress1;
    TGButton *fActive1;
    Bool_t fIsActive1;

    TGGroupFrame *fChannelFrame1;
    TGButton *fSameChannelSettingButton1;
    Bool_t fUseSameChannelSetting1;

    TGTab *fChannelTab1;
    TGCompositeFrame *fCh11;
    TGCompositeFrame *fCh12;
    TGCompositeFrame *fCh13;
    TGCompositeFrame *fCh14;

    // Module 2
    TGComboBox *fCBAddress2;
    Int_t fValueAddress2;
    TGButton *fActive2;
    Bool_t fIsActive2;

    TGGroupFrame *fChannelFrame2;
    TGButton *fSameChannelSettingButton2;
    Bool_t fUseSameChannelSetting2;

    TGTab *fChannelTab2;
    TGCompositeFrame *fCh21;
    TGCompositeFrame *fCh22;
    TGCompositeFrame *fCh23;
    TGCompositeFrame *fCh24;


  ClassDef(FADC400, 1);
};

#endif
