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
#include "TGNumberEntry.h"

class FADC400 : public TGMainFrame
{
  public:
    FADC400(const TGWindow *, UInt_t, UInt_t);
    virtual ~FADC400();

    void SetSameModuleSetting(Bool_t);

    void SetAddress(Int_t);
    void SetActive(Bool_t);
    void SetSameChannelSetting(Bool_t);

    void SetDSM(Int_t);
    void SetIP(Int_t);
    void SetID(const Char_t *);
    void SetAO(const Char_t *);
    void SetThres(const Char_t *);
    void SetRL(Int_t);

  private:
    void Initialize();
    Bool_t fIsDebug;

    // Module
    TGGroupFrame *fModuleFrame;
    TGButton *fSameModuleSettingButton;
    Bool_t fUseSameModuleSetting;
    TGTab *fModuleTab;
    TGCompositeFrame *fFADC[2];

    // Channel
    TGLabel *fTextAddress;
    TGComboBox *fCBAddress[2];
    Int_t fValueAddress[2];
    TGButton *fActive[2];
    Bool_t fIsActive[2];

    // Data saving mode
    TGLabel *fTextDSM;
    TGHButtonGroup *fDSMGroup;
    TGRadioButton *fDSM[2][4][2];
    Bool_t fValueDSM[2][4];

    // Input polarity
    TGLabel *fTextIP;
    TGHButtonGroup *fIPGroup;
    TGRadioButton *fIP[2][4][2];
    Bool_t fValueIP[2][4];

    // Input delay
    TGLabel *fTextID;
    TGNumberEntryField *fID[2][4];
    Int_t fValueID[2][4];

    // ADC Offset
    TGLabel *fTextAO;
    TGNumberEntryField *fAO[2][4];
    Int_t fValueAO[2][4];

    // Threshold
    TGLabel *fTextThres;
    TGNumberEntryField *fThres[2][4];
    Int_t fValueThres[2][4];

    // Recording length
    TGLabel *fTextRL;
    TGComboBox *fRL[2][4];
    Int_t fValueRL[2][4];

    TGGroupFrame *fChannelFrame[2];
    TGButton *fSameChannelSettingButton[2];
    Bool_t fUseSameChannelSetting[2];

    TGTab *fChannelTab[2];
    TGCompositeFrame *fCh[2][4];

  ClassDef(FADC400, 1);
};

#endif
