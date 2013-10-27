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

    void SetDT(const Char_t *);
    void SetDTApplied(Int_t);
    void SetCW(const Char_t *);
    void SetCWApplied(Int_t);

    void SetCLT(Int_t);

    void SetTriggerModeCount(Bool_t);
    void SetTMCOption(Int_t);
    void SetTriggerModeWidth(Bool_t);
    void SetTMWOption(Int_t);

  private:
    void Initialize();
    Bool_t fIsDebug;

    // Module
    TGGroupFrame *fModuleFrame;
    TGButton *fSameModuleSettingButton;
    Bool_t fUseSameModuleSetting;
    TGTab *fModuleTab;
    TGCompositeFrame *fFADC[2];

    // Channel setting frame
    TGGroupFrame *fChannelFrame[2];
    TGButton *fSameChannelSettingButton[2];
    Bool_t fUseSameChannelSetting[2];

    // Channel Tab
    TGTab *fChannelTab[2];
    TGCompositeFrame *fCh[2][4];

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

    // Trigger Frame
    TGGroupFrame *fTriggerFrame[2];

    TGLabel *fTextFor;

    // Deadtime
    TGLabel *fTextDT;
    TGNumberEntryField *fDT[2];
    Int_t fValueDT[2];
    TGHButtonGroup *fDTAppliedGroup;
    TGRadioButton *fDTApplied[2][3];
    Int_t fValueDTApplied[2];

    // Coincidence Width
    TGLabel *fTextCW;
    TGNumberEntryField *fCW[2];
    Int_t fValueCW[2];
    TGHButtonGroup *fCWAppliedGroup;
    TGRadioButton *fCWApplied[2][3];
    Int_t fValueCWApplied[2];

    // Contidion Lookup Table
    TGLabel *fTextCLT;
    TGHButtonGroup *fCLTGroup;
    TGRadioButton *fCLT[2][2];
    Int_t fValueCLT[2];

    // Trigger Mode Frame
    TGGroupFrame *fTModeFrame[2];

    TGTab *fTMTab;
    TGCompositeFrame *fTMCG[2][2];

    // Trigger Mode Channel Group Tab
    TGCheckButton *fTMCount[2][2];
    Bool_t fValueTMCount[2][2];
    TGButtonGroup *fTMCountOptionGroup[2][2];
    TGRadioButton *fTMCountOption[2][2][4];
    Int_t fValueTMCountOption[2][2];

    TGCheckButton *fTMWidth[2][2];
    Bool_t fValueTMWidth[2][2];
    TGButtonGroup *fTMWidthOptionGroup[2][2];
    TGRadioButton *fTMWidthOption[2][2][4];
    Int_t fValueTMWidthOption[2][2];


  ClassDef(FADC400, 1);
};

#endif
