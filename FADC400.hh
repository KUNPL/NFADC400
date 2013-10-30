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

#include "FADC400Settings.hh"

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

    void SetSameCGroupSetting(Bool_t);
    void SetTriggerModeCount(Bool_t);
    void SetTMCOption(Int_t);
    void SetTriggerModeWidth(Bool_t);
    void SetTMWOption(Int_t);

    void SetNumEvents(const Char_t *);

    void SaveSettings();
    void LoadSettings();
    void Start();

    void SetSettingsFromUI();
    void SetSettingsToUI();

  private:
    void Initialize();
    FADC400Settings fSettings;
    Bool_t fIsDebug;

    // Module
    TGGroupFrame *fModuleFrame;
    TGCheckButton *fSameModuleSettingButton;
    TGTab *fModuleTab;
    TGCompositeFrame *fFADC[2];

    // Channel setting frame
    TGGroupFrame *fChannelFrame[2];
    TGCheckButton *fSameChannelSettingButton[2];

    // Channel Tab
    TGTab *fChannelTab[2];
    TGCompositeFrame *fCh[2][4];

    // Channel
    TGLabel *fTextAddress;
    TGComboBox *fCBAddress[2];
    TGButton *fActive[2];

    // Data saving mode
    TGLabel *fTextDSM;
    TGHButtonGroup *fDSMGroup;
    TGRadioButton *fDSM[2][4][2];

    // Input polarity
    TGLabel *fTextIP;
    TGHButtonGroup *fIPGroup;
    TGRadioButton *fIP[2][4][2];

    // Input delay
    TGLabel *fTextID;
    TGNumberEntryField *fID[2][4];

    // ADC Offset
    TGLabel *fTextAO;
    TGNumberEntryField *fAO[2][4];

    // Threshold
    TGLabel *fTextThres;
    TGNumberEntryField *fThres[2][4];

    // Recording length
    TGLabel *fTextRL;
    TGComboBox *fRL[2][4];

    // Trigger Output Frame
    TGGroupFrame *fTriggerFrame[2];

    TGLabel *fTextFor;

    // Deadtime
    TGLabel *fTextDT;
    TGNumberEntryField *fDT[2];
    TGHButtonGroup *fDTAppliedGroup;
    TGRadioButton *fDTApplied[2][3];

    // Coincidence Width
    TGLabel *fTextCW;
    TGNumberEntryField *fCW[2];
    TGHButtonGroup *fCWAppliedGroup;
    TGRadioButton *fCWApplied[2][3];

    // Contidion Lookup Table
    TGLabel *fTextCLT;
    TGHButtonGroup *fCLTGroup;
    TGRadioButton *fCLT[2][2];

    // Trigger Mode Frame
    TGGroupFrame *fTModeFrame[2];

    TGTab *fTMTab[2];
    TGCompositeFrame *fTMCG[2][2];

    TGCheckButton *fSameCGroupSettingButton[2];

    // Trigger Mode Channel Group Tab
    TGCheckButton *fTMCount[2][2];
    TGButtonGroup *fTMCountOptionGroup[2][2];
    TGRadioButton *fTMCountOption[2][2][4];

    TGCheckButton *fTMWidth[2][2];
    TGButtonGroup *fTMWidthOptionGroup[2][2];
    TGRadioButton *fTMWidthOption[2][2][4];

    // The number of events to take
    TGLabel *fTextNumEvents;
    TGNumberEntryField *fNumEvents;

    // Buttons
    TGTextButton *fLoad;
    TGTextButton *fSave;
    TGTextButton *fStart;

  ClassDef(FADC400, 1);
};

#endif
