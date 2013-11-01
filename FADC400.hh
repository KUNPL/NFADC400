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

    void SetAC(Bool_t);
    void SetDSM(Int_t);
    void SetIP(Int_t);
    void SetID(const Char_t *);
    void SetAO(const Char_t *);
    void SetThres(const Char_t *);
    void SetRL(Int_t);

    void SetCLT(Int_t);

    void SetSameCGroupSetting(Bool_t);

    void SetDT(const Char_t *);
    void SetCW(const Char_t *);

    void SetTriggerModeWidth(Bool_t);
    void SetTMWOption(Int_t);
    void SetTMWThres(const Char_t *);

    void SetTriggerModeCount(Bool_t);
    void SetTMCOption(Int_t);
    void SetTMCThres(const Char_t *);
    void SetTMCInterval(const Char_t *);

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
    TGComboBox *fAddress[2];
    TGButton *fActive[2];

    // Active Channel
    TGLabel *fTextAC;
    TGCheckButton *fAC[2][4];

    // Data saving mode
    TGLabel *fTextDSM;
    TGComboBox *fDSM[2][4];

    // Input polarity
    TGLabel *fTextIP;
    TGComboBox *fIP[2][4];

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

    // Contidion Lookup Table
    TGLabel *fTextCLT;
    TGComboBox *fCLT[2];

    // Trigger Mode Frame
    TGGroupFrame *fTModeFrame[2];

    TGCheckButton *fSameCGroupSettingButton[2];

    // Trigger Mode Channel Group Tab
    TGTab *fTMTab[2];
    TGCompositeFrame *fTMCG[2][2];

    // Deadtime
    TGLabel *fTextDT;
    TGNumberEntryField *fDT[2][2];

    // Coincidence Width
    TGLabel *fTextCW;
    TGNumberEntryField *fCW[2][2];

    // Width Trigger mode
    TGCheckButton *fTMWidth[2][2];
    TGComboBox *fTMWidthOption[2][2];
    TGLabel *fTextTWidth;
    TGNumberEntryField *fTMWidthThres[2][2];

    // Count Trigger mode
    TGCheckButton *fTMCount[2][2];
    TGLabel *fTextTMCount;
    TGButtonGroup *fTMCountOptionGroup[2][2];
    TGRadioButton *fTMCountOption[2][2][4];
    TGNumberEntryField *fTMCountThres[2][2][3];
    TGNumberEntryField *fTMCountInterval[2][2][3];

    // The number of events to take
    TGLabel *fTextNumEvents;
    TGNumberEntryField *fNumEvents;

    // Buttons
    TGTextButton *fLoad;
    TGTextButton *fSave;
    TGTextButton *fStart;

    // Copyright
    TGLabel *fCopyright;

  ClassDef(FADC400, 1);
};

#endif
