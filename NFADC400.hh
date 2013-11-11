/////////////////////////////////////
//                                 //
//       NFADC400 module GUI       //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 10. 21           //
//                                 //
/////////////////////////////////////

#ifndef _NFADC400_H_
#define _NFADC400_H_

#include "TString.h"

#include "TGWindow.h"
#include "TGFrame.h"
#include "TGTab.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGComboBox.h"
#include "TGButtonGroup.h"
#include "TGNumberEntry.h"

#include "NFADC400Settings.hh"

class NFADC400 : public TGMainFrame
{
  public:
    NFADC400(const TGWindow *, UInt_t, UInt_t);
    virtual ~NFADC400();

    void SetSameModuleSetting(Bool_t);

    void SetAddress(Int_t);
    void SetActive(Bool_t);
    void SetSameChannelSetting(Bool_t);

    void SetAC(Bool_t);
    void SetIP(Int_t);
    void SetID(const Char_t *);
    void SetAO(const Char_t *);
    void SetIAG(const Char_t *);
    void SetRL(Int_t);

    void SetTLT(Int_t);
    void SetTOW(const Char_t *);
    void SetTDC(Int_t);

    void SetSameChannelTriggerSetting(Bool_t);

    void SetThres(const Char_t *);
    void SetDT(const Char_t *);
    void SetCW(const Char_t *);

    void SetTriggerModeWidth(Bool_t);
    void SetTMWThres(const Char_t *);

    void SetTriggerModeCount(Bool_t);
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
    NFADC400Settings fSettings;
    Bool_t fIsDebug;

    // Module
    TGGroupFrame *fModuleFrame;
    TGCheckButton *fSameModuleSettingButton;

    // Module tab
    TGTab *fModuleTab;
    TGCompositeFrame *fModule[2];

    // Module setting frame
    TGGroupFrame *fModuleSettingFrame[2];

    // Channel
    TGLabel *fTextAddress;
    TGComboBox *fAddress[2];
    TGButton *fActive[2];

    // Channel setting frame
    TGGroupFrame *fChannelFrame[2];
    TGCheckButton *fSameChannelSettingButton[2];

    // Recording length
    TGLabel *fTextRL;
    TGComboBox *fRL[2];

    // Channel tab
    TGTab *fChannelTab[2];
    TGCompositeFrame *fCh[2][4];

    // Active channel
    TGCheckButton *fAC[2][4];

    // Input polarity
    TGLabel *fTextIP;
    TGComboBox *fIP[2][4];

    // Input delay
    TGLabel *fTextID;
    TGNumberEntryField *fID[2][4];

    // ADC Offset
    TGLabel *fTextAO;
    TGNumberEntryField *fAO[2][4];

    // Input amplifier gain
    TGLabel *fTextIAG;
    TGNumberEntryField *fIAG[2][4];

    // Trigger Output Frame
    TGGroupFrame *fTriggerFrame[2];

    // Contidion Lookup Table
    TGLabel *fTextTLT[2];
    TGComboBox *fTLT[2];

    // Trigger Output Width
    TGLabel *fTextTOW;
    TGNumberEntryField *fTOW[2];

    // Trigger Daisy Chain
    TGLabel *fTextTDC;
    TGComboBox *fTDC[2];

    // Trigger Mode Frame
    TGGroupFrame *fTModeFrame[2];
    TGCheckButton *fSameChannelTriggerSettingButton[2];

    // Trigger Mode Channel Tab
    TGTab *fTMTab[4];
    TGCompositeFrame *fTM[2][4];

    // Threshold
    TGLabel *fTextThres;
    TGNumberEntryField *fThres[2][4];

    // Deadtime
    TGLabel *fTextDT;
    TGNumberEntryField *fDT[2][4];

    // Coincidence Width
    TGLabel *fTextCW;
    TGNumberEntryField *fCW[2][4];

    // Width Trigger mode
    TGCheckButton *fTMWidth[2][4];
    TGNumberEntryField *fTMWidthThres[2][4];

    // Count Trigger mode
    TGCheckButton *fTMCount[2][4];
    TGNumberEntryField *fTMCountThres[2][4];
    TGNumberEntryField *fTMCountInterval[2][4];

    // The number of events to take
    TGLabel *fTextNumEvents;
    TGNumberEntryField *fNumEvents;

    // Buttons
    TGTextButton *fLoad;
    TGTextButton *fSave;
    TGTextButton *fStart;

    // Copyright
    TGLabel *fCopyright;

  ClassDef(NFADC400, 1);
};

#endif
