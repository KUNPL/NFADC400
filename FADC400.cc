/////////////////////////////////////
//                                 //
//        FADC400 module GUI       //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 10. 21           //
//                                 //
/////////////////////////////////////

#include <cmath>
#include <cstdlib>

using std::atoi;

#include "TApplication.h"
#include "Riostream.h"
#include "TSystem.h"
#include "TGClient.h"

#include "FADC400.hh"

ClassImp(FADC400);

FADC400::FADC400(const TGWindow *window, UInt_t width, UInt_t height)
:TGMainFrame(window, width, height, kMainFrame)
{
  Initialize();

	FILE *fp = fopen("DEBUG", "r");
	if (fp) {
		fclose(fp);
		fIsDebug = kTRUE;
	} else  
		fIsDebug = kFALSE;

  SetWindowName("FADC400 Controller");
  SetName("fMainWindow");
  SetLayoutBroken(kTRUE);

  // == Start of Module Frame =====================================================
  fModuleFrame = new TGGroupFrame(this, "Module");
  fModuleFrame -> SetLayoutBroken(kTRUE);

  Int_t moduleFrameHMargin = 5;
  Int_t moduleFrameVMargin = 5;
  Int_t moduleFrameWidth = width - 2*moduleFrameHMargin;
  Int_t moduleFrameHeight = 374;
  fModuleFrame -> MoveResize(moduleFrameHMargin, moduleFrameVMargin, moduleFrameWidth, moduleFrameHeight);

  fSameModuleSettingButton = new TGCheckButton(fModuleFrame, "Use the same setting for all modules.");
  fSameModuleSettingButton -> Connect("Toggled(Bool_t)", "FADC400", this, "SetSameModuleSetting(Bool_t)");
  fSameModuleSettingButton -> Move(10, 20);

  fModuleTab = new TGTab(fModuleFrame);
  fFADC[0] = fModuleTab -> AddTab("FADC400 Module 1");
  fFADC[0] -> SetLayoutBroken(kTRUE);
  fFADC[1] = fModuleTab -> AddTab("FADC400 Module 2");
  fFADC[1] -> SetLayoutBroken(kTRUE);

  fModuleFrame -> AddFrame(fModuleTab, new TGLayoutHints(kLHintsLeft|kLHintsTop));
  Int_t moduleTabHMargin = 10;
  Int_t moduleTabWidth = moduleFrameWidth - 2*moduleTabHMargin;
  Int_t moduleTabHeight = moduleFrameHeight - 65;
  fModuleTab -> MoveResize(moduleTabHMargin, 45, moduleTabWidth, moduleTabHeight);
  AddFrame(fModuleFrame, new TGLayoutHints(kLHintsLeft|kLHintsTop));

  // == Start of Modules  =======================================================
  for (Int_t iModule = 0; iModule < 2; iModule++) {
    fTextAddress = new TGLabel(fFADC[iModule], "Address:");
    fTextAddress -> Move(10, 10);

    fCBAddress[iModule] = new TGComboBox(fFADC[iModule], iModule);
    for (Int_t iAddress = 0; iAddress < 6; iAddress++)
      fCBAddress[iModule] -> AddEntry(Form("%d", iAddress), iAddress);
  
    fCBAddress[iModule] -> Select(3 + iModule);
    fCBAddress[iModule] -> Connect("Selected(Int_t)", "FADC400", this, "SetAddress(Int_t)");
    fCBAddress[iModule] -> MoveResize(70, 8, 50, 20);
    fFADC[iModule] -> AddFrame(fCBAddress[iModule]);

    fActive[iModule] = new TGCheckButton(fFADC[iModule], "Active", iModule);
    fActive[iModule] -> Connect("Toggled(Bool_t)", "FADC400", this, "SetActive(Bool_t)");
    fActive[iModule] -> Move(140, 10);

    // == Start of Channel Setting Frame ========================================
    fChannelFrame[iModule] = new TGGroupFrame(fFADC[iModule], "Channel Setting");
    fChannelFrame[iModule] -> SetLayoutBroken(kTRUE);

    Int_t channelFrameHMargin = 10;
    Int_t channelFrameWidth = 355;
    Int_t channelFrameHeight = 247;
    fChannelFrame[iModule] -> MoveResize(channelFrameHMargin, 35, channelFrameWidth, channelFrameHeight);
    fFADC[iModule] -> AddFrame(fChannelFrame[iModule]);

    // == Start of Channel Tab ==================================================
    fSameChannelSettingButton[iModule] = new TGCheckButton(fChannelFrame[iModule], "Use the same settings for all channel.", iModule);
    fSameChannelSettingButton[iModule] -> Connect("Toggled(Bool_t)", "FADC400", this, "SetSameChannelSetting(Bool_t)");
    fSameChannelSettingButton[iModule] -> Move(10, 20);

    fChannelTab[iModule] = new TGTab(fChannelFrame[iModule]);
    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fCh[iModule][iChannel] = fChannelTab[iModule] -> AddTab(Form("Channel %d", iChannel + 1));
      fCh[iModule][iChannel] -> SetLayoutBroken(kTRUE);
    }

    fChannelFrame[iModule] -> AddFrame(fChannelTab[iModule], new TGLayoutHints(kLHintsLeft|kLHintsTop));
    Int_t channelTabHMargin = 10;
    Int_t channelTabWidth = channelFrameWidth - 2*channelTabHMargin;
    Int_t channelTabHeight = channelFrameHeight - 62;
    fChannelTab[iModule] -> MoveResize(channelTabHMargin, 45, channelTabWidth, channelTabHeight);
    fFADC[iModule] -> AddFrame(fChannelFrame[iModule], new TGLayoutHints(kLHintsLeft|kLHintsTop));

    for (Int_t iChannel = 0; iChannel < 4; iChannel++ ) {
      Int_t widgetID = iModule*100 + iChannel*10;

      // == Start of Data saving mode =============================================
      fTextDSM = new TGLabel(fCh[iModule][iChannel], "Data saving mode");
      fTextDSM -> Move(10, 10);
      fDSMGroup = new TGHButtonGroup(fCh[iModule][iChannel]);
      fDSMGroup -> SetLayoutBroken(kTRUE);
      fDSM[iModule][iChannel][0] = new TGRadioButton(fDSMGroup, "Raw", widgetID);
      fDSM[iModule][iChannel][0] -> MoveResize(0, 0, 50, 11);
      fDSM[iModule][iChannel][0] -> SetState(kButtonDown);
      fDSM[iModule][iChannel][1] = new TGRadioButton(fDSMGroup, "Smooth", widgetID + 1);
      fDSM[iModule][iChannel][1] -> MoveResize(60, 0, 60, 11);
      fDSMGroup -> Connect("Clicked(Int_t)", "FADC400", this, "SetDSM(Int_t)");
      fDSMGroup -> MoveResize(200, 13, 120, 11);
      fDSMGroup -> Show();
      fCh[iModule][iChannel] -> AddFrame(fDSMGroup);
      // == End of Data saving mode ===============================================

      // == Start of Input polarity ===============================================
      fTextIP = new TGLabel(fCh[iModule][iChannel], "Input polarity");
      fTextIP -> Move(10, 35);
      fIPGroup = new TGHButtonGroup(fCh[iModule][iChannel]);
      fIPGroup -> SetLayoutBroken(kTRUE);
      fIP[iModule][iChannel][0] = new TGRadioButton(fIPGroup, "(-)", widgetID);
      fIP[iModule][iChannel][0] -> MoveResize(0, 0, 50, 11);
      fIP[iModule][iChannel][0] -> SetState(kButtonDown);
      fIP[iModule][iChannel][1] = new TGRadioButton(fIPGroup, "(+)", widgetID + 1);
      fIP[iModule][iChannel][1] -> MoveResize(60, 0, 50, 11);
      fIPGroup -> Connect("Clicked(Int_t)", "FADC400", this, "SetIP(Int_t)");
      fIPGroup -> MoveResize(200, 38, 120, 11);
      fIPGroup -> Show();
      fCh[iModule][iChannel] -> AddFrame(fIPGroup);
      // == End of Input polarity =================================================

      // == Start of Input delay ==================================================
      fTextID = new TGLabel(fCh[iModule][iChannel], "Input delay (0 ~ 40900) (ns)");
      fTextID -> Move(10, 60);
      fID[iModule][iChannel] = new TGNumberEntryField(fCh[iModule][iChannel], widgetID, 0, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 40900);
      fID[iModule][iChannel] -> Connect("TextChanged(const Char_t *)", "FADC400", this, "SetID(const Char_t *)");
      fID[iModule][iChannel] -> MoveResize(241, 60, 80, 18);
      fCh[iModule][iChannel] -> AddFrame(fID[iModule][iChannel]);
      // == End of Input delay ====================================================

      // == Start of ADC Offset ===================================================
      fTextAO = new TGLabel(fCh[iModule][iChannel], "ADC Offset (0 ~ 4095) (ns)");
      fTextAO -> Move(10, 85);
      fAO[iModule][iChannel] = new TGNumberEntryField(fCh[iModule][iChannel], widgetID, 0, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 4095);
      fAO[iModule][iChannel] -> Connect("TextChanged(const Char_t *)", "FADC400", this, "SetAO(const Char_t *)");
      fAO[iModule][iChannel] -> MoveResize(241, 85, 80, 18);
      fCh[iModule][iChannel] -> AddFrame(fAO[iModule][iChannel]);
      // == End of ADC Offset =====================================================

      // == Start of Threshold ====================================================
      fTextThres = new TGLabel(fCh[iModule][iChannel], "Threshold (0 ~ 1023)");
      fTextThres -> Move(10, 110);
      fThres[iModule][iChannel] = new TGNumberEntryField(fCh[iModule][iChannel], widgetID, 0, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 1023);
      fThres[iModule][iChannel] -> Connect("TextChanged(const Char_t *)", "FADC400", this, "SetThres(const Char_t *)");
      fThres[iModule][iChannel] -> MoveResize(241, 110, 80, 18);
      fCh[iModule][iChannel] -> AddFrame(fThres[iModule][iChannel]);
      // == End of Threshold ======================================================

      // == Start of Recording length =============================================
      fTextRL = new TGLabel(fCh[iModule][iChannel], "Recording length (us)");
      fTextRL -> Move(10, 135);
      fRL[iModule][iChannel] = new TGComboBox(fCh[iModule][iChannel], widgetID);
      for (Int_t iRL = 0; iRL < 10; iRL++)
        fRL[iModule][iChannel] -> AddEntry(Form("%.2f", 2.56*pow(2., iRL)), pow(2., iRL));
      fRL[iModule][iChannel] -> Select(1);
      fRL[iModule][iChannel] -> Connect("Selected(Int_t)", "FADC400", this, "SetRL(Int_t)");
      fRL[iModule][iChannel] -> MoveResize(241, 135, 80, 18);
      fCh[iModule][iChannel] -> AddFrame(fRL[iModule][iChannel]);
      // == End of Recording length ===============================================

      // == Start of Tab and Return pressed on TextEntries ========================
      fID[iModule][iChannel] -> Connect("TabPressed()", "TGNumberEntryField", fAO[iModule][iChannel], "SetFocus()");
      fID[iModule][iChannel] -> Connect("ReturnPressed()", "TGNumberEntryField", fAO[iModule][iChannel], "SetFocus()");
      fAO[iModule][iChannel] -> Connect("TabPressed()", "TGNumberEntryField", fThres[iModule][iChannel], "SetFocus()");
      fAO[iModule][iChannel] -> Connect("ReturnPressed()", "TGNumberEntryField", fThres[iModule][iChannel], "SetFocus()");
      fThres[iModule][iChannel] -> Connect("TabPressed()", "TGNumberEntryField", fID[iModule][iChannel], "SetFocus()");
      fThres[iModule][iChannel] -> Connect("ReturnPressed()", "TGNumberEntryField", fID[iModule][iChannel], "SetFocus()");
      // == End of Tab and Return pressed on TextEntries ==========================
    }
    // == End of Channel Tab ====================================================
    // == End of Channel Setting Frame ==========================================

    // == Start of Trigger Frame ================================================
    fTriggerFrame[iModule] = new TGGroupFrame(fFADC[iModule], "Trigger Setting");
    fTriggerFrame[iModule] -> SetLayoutBroken(kTRUE);

    Int_t triggerFrameHMargin = 10;
    Int_t triggerFrameWidth = 355;
    Int_t triggerFrameHeight = 277;
    fTriggerFrame[iModule] -> MoveResize(channelFrameWidth + 1.5*triggerFrameHMargin, 5, triggerFrameWidth, triggerFrameHeight);
    fFADC[iModule] -> AddFrame(fTriggerFrame[iModule]);

    // == Start of Deadtime =====================================================
    fTextDT = new TGLabel(fTriggerFrame[iModule], "Deadtime (0 ~ 104448000) (ns)");
    fTextDT -> Move(10, 20);
    fDT[iModule] = new TGNumberEntryField(fTriggerFrame[iModule], iModule, 0, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 104448000);
    fDT[iModule] -> Connect("TextChanged(const Char_t *)", "FADC400", this, "SetDT(const Char_t *)");
    fDT[iModule] -> MoveResize(250, 20, 80, 18);
    fTriggerFrame[iModule] -> AddFrame(fDT[iModule]);
    fTextFor = new TGLabel(fTriggerFrame[iModule], "for");
    fTextFor -> Move(30, 38);
    fDTAppliedGroup = new TGHButtonGroup(fTriggerFrame[iModule]);
    fDTAppliedGroup -> SetLayoutBroken(kTRUE);
    fDTApplied[iModule][0] = new TGRadioButton(fDTAppliedGroup, "Ch 1&&2", iModule*100);
    fDTApplied[iModule][0] -> MoveResize(0, 0, 65, 11);
    fDTApplied[iModule][0] -> SetState(kButtonDown);
    fDTApplied[iModule][1] = new TGRadioButton(fDTAppliedGroup, "Ch 3&&4", iModule*100 + 1);
    fDTApplied[iModule][1] -> MoveResize(85, 0, 65, 11);
    fDTApplied[iModule][2] = new TGRadioButton(fDTAppliedGroup, "All Channels", iModule*100 + 2);
    fDTApplied[iModule][2] -> MoveResize(170, 0, 95, 11);
    fDTAppliedGroup -> Connect("Clicked(Int_t)", "FADC400", this, "SetDTApplied(Int_t)");
    fDTAppliedGroup -> MoveResize(55, 40, 270, 11);
    fDTAppliedGroup -> Show();
    fTriggerFrame[iModule] -> AddFrame(fDTAppliedGroup);
    // == End of Deadtime =======================================================

    // == Start of Coincidence width ============================================
    fTextCW = new TGLabel(fTriggerFrame[iModule], "Coincidence width (160 ~ 40800) (ns)");
    fTextCW -> Move(10, 60);
    fCW[iModule] = new TGNumberEntryField(fTriggerFrame[iModule], iModule, 160, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 160, 40800);
    fCW[iModule] -> Connect("TextChanged(const Char_t *)", "FADC400", this, "SetCW(const Char_t *)");
    fCW[iModule] -> MoveResize(250, 60, 80, 18);
    fTriggerFrame[iModule] -> AddFrame(fCW[iModule]);
    fTextFor = new TGLabel(fTriggerFrame[iModule], "for");
    fTextFor -> Move(30, 78);
    fCWAppliedGroup = new TGHButtonGroup(fTriggerFrame[iModule]);
    fCWAppliedGroup -> SetLayoutBroken(kTRUE);
    fCWApplied[iModule][0] = new TGRadioButton(fCWAppliedGroup, "Ch 1&&2", iModule*100);
    fCWApplied[iModule][0] -> MoveResize(0, 0, 65, 11);
    fCWApplied[iModule][0] -> SetState(kButtonDown);
    fCWApplied[iModule][1] = new TGRadioButton(fCWAppliedGroup, "Ch 3&&4", iModule*100 + 1);
    fCWApplied[iModule][1] -> MoveResize(85, 0, 65, 11);
    fCWApplied[iModule][2] = new TGRadioButton(fCWAppliedGroup, "All Channels", iModule*100 + 2);
    fCWApplied[iModule][2] -> MoveResize(170, 0, 95, 11);
    fCWAppliedGroup -> Connect("Clicked(Int_t)", "FADC400", this, "SetCWApplied(Int_t)");
    fCWAppliedGroup -> MoveResize(55, 80, 270, 11);
    fCWAppliedGroup -> Show();
    fTriggerFrame[iModule] -> AddFrame(fCWAppliedGroup);
    // == End of Deadtime =======================================================

    // == Start of Tab and Return pressed on TextEntries ========================
    fDT[iModule] -> Connect("TabPressed()", "TGNumberEntryField", fCW[iModule], "SetFocus()");
    fDT[iModule] -> Connect("ReturnPressed()", "TGNumberEntryField", fCW[iModule], "SetFocus()");
    fCW[iModule] -> Connect("TabPressed()", "TGNumberEntryField", fDT[iModule], "SetFocus()");
    fCW[iModule] -> Connect("ReturnPressed()", "TGNumberEntryField", fDT[iModule], "SetFocus()");
    // == End of Tab and Return pressed on TextEntries ==========================

    // == Start of Condition Lookup Table =======================================
    fTextCLT = new TGLabel(fTriggerFrame[iModule], "Condition lookup table");
    fTextCLT -> Move(10, 100);
    fCLTGroup = new TGHButtonGroup(fTriggerFrame[iModule]);
    fCLTGroup -> SetLayoutBroken(kTRUE);
    fCLT[iModule][0] = new TGRadioButton(fCLTGroup, "AND", iModule*100);
    fCLT[iModule][0] -> MoveResize(0, 0, 50, 11);
    fCLT[iModule][0] -> SetState(kButtonDown);
    fCLT[iModule][1] = new TGRadioButton(fCLTGroup, "OR", iModule*100 + 1);
    fCLT[iModule][1] -> MoveResize(70, 0, 40, 11);
    fCLTGroup -> Connect("Clicked(Int_t)", "FADC400", this, "SetCLT(Int_t)");
    fCLTGroup -> MoveResize(222, 102, 110, 11);
    fCLTGroup -> Show();
    fTriggerFrame[iModule] -> AddFrame(fCLTGroup);
    // == End of Condition Lookup Table =========================================

    // == Start of Trigger Mode Frame ===========================================
    fTModeFrame[iModule] = new TGGroupFrame(fTriggerFrame[iModule], "Mode");
    fTModeFrame[iModule] -> SetLayoutBroken(kTRUE);

    Int_t tmodeFrameHMargin = 10;
    Int_t tmodeFrameWidth = triggerFrameWidth - 2*tmodeFrameHMargin;
    Int_t tmodeFrameHeight = triggerFrameHeight - 129;
    fTModeFrame[iModule] -> MoveResize(tmodeFrameHMargin, 119, tmodeFrameWidth, tmodeFrameHeight);
    fTriggerFrame[iModule] -> AddFrame(fTModeFrame[iModule]);

    fSameCGroupSettingButton[iModule] = new TGCheckButton(fTModeFrame[iModule], "Use the same setting for all channel group.", iModule);
    fSameCGroupSettingButton[iModule] -> Connect("Toggled(Bool_t)", "FADC400", this, "SetSameCGroupSetting(Bool_t)");
    fSameCGroupSettingButton[iModule] -> Move(10, 20);

    // == Start of Trigger Mode Channel Group ===================================
    fTMTab[iModule] = new TGTab(fTModeFrame[iModule]);
    fTMCG[iModule][0] = fTMTab[iModule] -> AddTab("Channel 1 & 2");
    fTMCG[iModule][0] -> SetLayoutBroken(kTRUE);
    fTMCG[iModule][1] = fTMTab[iModule] -> AddTab("Channel 3 & 4");
    fTMCG[iModule][1] -> SetLayoutBroken(kTRUE);

    // == Start of Trigger Mode Channel Group Tab ===============================
    fTriggerFrame[iModule] -> AddFrame(fTMTab[iModule], new TGLayoutHints(kLHintsLeft|kLHintsTop));
    Int_t tmodeTabHMargin = 10;
    Int_t tmodeTabWidth = tmodeFrameWidth - 2*tmodeTabHMargin;
    Int_t tmodeTabHeight = tmodeFrameHeight - 60;
    fTMTab[iModule] -> MoveResize(tmodeTabHMargin, 42, tmodeTabWidth, tmodeTabHeight);
    fTriggerFrame[iModule] -> AddFrame(fTMTab[iModule]);

    for (Int_t iCGroup = 0; iCGroup < 2; iCGroup++) {
      Int_t widgetID = iModule*100 + iCGroup*10;

      fTMCount[iModule][iCGroup] = new TGCheckButton(fTMCG[iModule][iCGroup], "Count", widgetID);
      fTMCount[iModule][iCGroup] -> Connect("Toggled(Bool_t)", "FADC400", this, "SetTriggerModeCount(Bool_t)");
      fTMCount[iModule][iCGroup] -> Move(15, 4);

      Int_t lowCh = pow(3., iCGroup);
      Int_t highCh = pow(2., iCGroup + 1);
      fTMCountOptionGroup[iModule][iCGroup] = new TGButtonGroup(fTMCG[iModule][iCGroup]);
      fTMCountOptionGroup[iModule][iCGroup] -> SetLayoutBroken(kTRUE);
      fTMCountOption[iModule][iCGroup][0] = new TGRadioButton(fTMCountOptionGroup[iModule][iCGroup], Form("%d + %d", lowCh, highCh), widgetID);
      fTMCountOption[iModule][iCGroup][0] -> SetEnabled(kFALSE);
      fTMCountOption[iModule][iCGroup][0] -> Move(50, 0);
      fTMCountOption[iModule][iCGroup][1] = new TGRadioButton(fTMCountOptionGroup[iModule][iCGroup], Form("%d", lowCh), widgetID + 1);
      fTMCountOption[iModule][iCGroup][1] -> SetEnabled(kFALSE);
      fTMCountOption[iModule][iCGroup][1] -> Move(0, 0);
      fTMCountOption[iModule][iCGroup][2] = new TGRadioButton(fTMCountOptionGroup[iModule][iCGroup], Form("%d", highCh), widgetID + 2);
      fTMCountOption[iModule][iCGroup][2] -> SetEnabled(kFALSE);
      fTMCountOption[iModule][iCGroup][2] -> Move(0, 20);
      fTMCountOption[iModule][iCGroup][3] = new TGRadioButton(fTMCountOptionGroup[iModule][iCGroup], Form("%d OR %d", lowCh, highCh), widgetID + 3);
      fTMCountOption[iModule][iCGroup][3] -> SetEnabled(kFALSE);
      fTMCountOption[iModule][iCGroup][3] -> Move(50, 20);
      fTMCountOptionGroup[iModule][iCGroup] -> Connect("Clicked(Int_t)", "FADC400", this, "SetTMCOption(Int_t)");
      fTMCountOptionGroup[iModule][iCGroup] -> MoveResize(25, 21, 110, 35);
      fTMCountOptionGroup[iModule][iCGroup] -> Show();
      fTMCG[iModule][iCGroup] -> AddFrame(fTMCountOptionGroup[iModule][iCGroup]);

      fTMWidth[iModule][iCGroup] = new TGCheckButton(fTMCG[iModule][iCGroup], "Width", widgetID);
      fTMWidth[iModule][iCGroup] -> Connect("Toggled(Bool_t)", "FADC400", this, "SetTriggerModeWidth(Bool_t)");
      fTMWidth[iModule][iCGroup] -> Move(155, 4);
      fTMWidthOptionGroup[iModule][iCGroup] = new TGButtonGroup(fTMCG[iModule][iCGroup]);
      fTMWidthOptionGroup[iModule][iCGroup] -> SetLayoutBroken(kTRUE);
      fTMWidthOption[iModule][iCGroup][0] = new TGRadioButton(fTMWidthOptionGroup[iModule][iCGroup], Form("%d OR %d", lowCh, highCh), widgetID);
      fTMWidthOption[iModule][iCGroup][0] -> SetEnabled(kFALSE);
      fTMWidthOption[iModule][iCGroup][0] -> Move(50, 0);
      fTMWidthOption[iModule][iCGroup][1] = new TGRadioButton(fTMWidthOptionGroup[iModule][iCGroup], Form("%d", lowCh), widgetID + 1);
      fTMWidthOption[iModule][iCGroup][1] -> SetEnabled(kFALSE);
      fTMWidthOption[iModule][iCGroup][1] -> Move(0, 0);
      fTMWidthOption[iModule][iCGroup][2] = new TGRadioButton(fTMWidthOptionGroup[iModule][iCGroup], Form("%d", highCh), widgetID + 2);
      fTMWidthOption[iModule][iCGroup][2] -> SetEnabled(kFALSE);
      fTMWidthOption[iModule][iCGroup][2] -> Move(0, 20);
      fTMWidthOption[iModule][iCGroup][3] = new TGRadioButton(fTMWidthOptionGroup[iModule][iCGroup], Form("%d AND %d", lowCh, highCh), widgetID + 3);
      fTMWidthOption[iModule][iCGroup][3] -> SetEnabled(kFALSE);
      fTMWidthOption[iModule][iCGroup][3] -> Move(50, 20);
      fTMWidthOptionGroup[iModule][iCGroup] -> Connect("Clicked(Int_t)", "FADC400", this, "SetTMWOption(Int_t)");
      fTMWidthOptionGroup[iModule][iCGroup] -> MoveResize(175, 21, 120, 35);
      fTMWidthOptionGroup[iModule][iCGroup] -> Show();
      fTMCG[iModule][iCGroup] -> AddFrame(fTMWidthOptionGroup[iModule][iCGroup]);
    }
    // == End of Trigger Mode Channel Group Tab =================================
    // == End of Trigger Mode Channel Group =====================================
    // == End of Trigger Mode Frame =============================================
    // == End of Trigger Frame ==================================================
  }
  // == End of Modules ============================================================
  // == End of Module Frame =======================================================

  MapSubwindows();
  MapWindow();
}

FADC400::~FADC400()
{
  Cleanup();
}

void FADC400::Initialize()
{
  // Initialize internal variables
}

void FADC400::SetSameModuleSetting(Bool_t value)
{
  if (fIsDebug) {
    cout << "==============================" << endl;
    cout << " SetSameModuleSetting is " << ( value ? "On!" : "Off!" ) << endl;
    cout << "==============================" << endl;
  }

  fModuleTab -> SetEnabled(1, !value);
  fModuleTab -> SetTab(0, kFALSE);

  fUseSameModuleSetting = value;
}

void FADC400::SetAddress(Int_t value)
{
  TGComboBox *object = (TGComboBox *) gTQSender;
  Int_t module = object -> WidgetId();

  if (fIsDebug) {
    cout << "======================================" << endl;
    cout << " SetAddress module:" << module + 1;
    cout << " is " << value << "!" << endl;
    cout << "======================================" << endl;
  }

  fValueAddress[module] = value;
}

void FADC400::SetActive(Bool_t value)
{
  TGCheckButton *object = (TGCheckButton *) gTQSender;
  Int_t module = object -> WidgetId();

  if (fIsDebug) {
    cout << "====================================" << endl;
    cout << " SetActive module:" << module + 1;
    cout << " is " << ( value ? "Activated!" : "Deactivated!" ) << endl;
    cout << "====================================" << endl;
  }

  fIsActive[module] = value;
}

void FADC400::SetSameChannelSetting(Bool_t value)
{
  TGCheckButton *object = (TGCheckButton *) gTQSender;
  Int_t module = object -> WidgetId();

  if (fIsDebug) {
    cout << "========================================" << endl;
    cout << " SetSameChannelSetting module:" << module + 1;
    cout << " is " << ( value ? "On!" : "Off!" ) << endl;
    cout << "========================================" << endl;
  }

  fChannelTab[module] -> SetEnabled(1, !value);
  fChannelTab[module] -> SetEnabled(2, !value);
  fChannelTab[module] -> SetEnabled(3, !value);
  fChannelTab[module] -> SetTab(0, kFALSE);

  fUseSameChannelSetting[module] = value;
}

void FADC400::SetDSM(Int_t value)
{
  Int_t module = value/100;
  Int_t channel = (value%100)/10;
  Int_t mode = value%10;

  if (fIsDebug) {
    cout << "======================================" << endl;
    cout << " SetDSM module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << ( mode ? "Smooth!" : "Raw!" ) << endl;
    cout << "======================================" << endl;
  }

  fValueDSM[module][channel] = mode;
}

void FADC400::SetIP(Int_t value)
{
  Int_t module = value/100;
  Int_t channel = (value%100)/10;
  Int_t mode = value%10;

  if (fIsDebug) {
    cout << "==================================" << endl;
    cout << " SetIP module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << ( mode ? "(+)!" : "(-)!" ) << endl;
    cout << "==================================" << endl;
  }

  fValueIP[module][channel] = mode;
}

void FADC400::SetID(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channel = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "====================================" << endl;
    cout << " SetID module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << atoi(value) << "!" << endl;
    cout << "====================================" << endl;
  }

  fValueID[module][channel] = atoi(value);
}

void FADC400::SetAO(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channel = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "===================================" << endl;
    cout << " SetAO module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << atoi(value) << "!" << endl;
    cout << "===================================" << endl;
  }

  fValueAO[module][channel] = atoi(value);
}

void FADC400::SetThres(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channel = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "======================================" << endl;
    cout << " SetThres module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << atoi(value) << "!" << endl;
    cout << "======================================" << endl;
  }

  fValueThres[module][channel] = atoi(value);
}

void FADC400::SetRL(Int_t value)
{
  TGComboBox *object = (TGComboBox *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channel = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "==============================================" << endl;
    cout << " SetRL module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << value << " (" << 2.56*value << " us)" << endl;
    cout << "==============================================" << endl;
  }

  fValueRL[module][channel] = value;
}

void FADC400::SetDT(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t module = object -> WidgetId();

  if (fIsDebug) {
    cout << "==============================" << endl;
    cout << " SetDT module:" << module + 1;
    cout << " is " << atoi(value) << "!" << endl;
    cout << "==============================" << endl;
  }

  fValueDT[module] = atoi(value);
}

void FADC400::SetDTApplied(Int_t value)
{
  Int_t module = value/100;
  Int_t mode = value%10;

  if (fIsDebug) {
    cout << "=============================================" << endl;
    cout << " SetDTApplied module:" << module + 1;
    cout << " is ";
    if (mode == 0)
      cout << "for channel 1 & 2!";
    else if (mode == 1)
      cout << "for channel 3 & 4!";
    else
      cout << "for all channels!";
    cout << endl;
    cout << "=============================================" << endl;
  }

  fValueDTApplied[module] = mode;
}

void FADC400::SetCW(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t module = object -> WidgetId();

  if (fIsDebug) {
    cout << "==========================" << endl;
    cout << " SetCW module:" << module + 1;
    cout << " is " << atoi(value) << "!" << endl;
    cout << "==========================" << endl;
  }

  fValueCW[module] = atoi(value);
}

void FADC400::SetCWApplied(Int_t value)
{
  Int_t module = value/100;
  Int_t mode = value%10;

  if (fIsDebug) {
    cout << "=============================================" << endl;
    cout << " SetCWApplied module:" << module + 1;
    cout << " is ";
    if (mode == 0)
      cout << "for channel 1 & 2!";
    else if (mode == 1)
      cout << "for channel 3 & 4!";
    else
      cout << "for all channels!";
    cout << endl;
    cout << "=============================================" << endl;
  }

  fValueCWApplied[module] = mode;
}

void FADC400::SetCLT(Int_t value)
{
  Int_t module = value/100;
  Int_t mode = value%10;

  if (fIsDebug) {
    cout << "=========================" << endl;
    cout << " SetCLT module:" << module + 1;
    cout << " is " << ( mode ? "OR!" : "AND!" ) << endl;
    cout << "=========================" << endl;
  }

  fValueCLT[module] = mode;
}

void FADC400::SetSameCGroupSetting(Bool_t value)
{
  TGCheckButton *object = (TGCheckButton *) gTQSender;
  Int_t module = object -> WidgetId();

  if (fIsDebug) {
    cout << "========================================" << endl;
    cout << " SetSameCGroupSetting module: " << module + 1;
    cout << " is " << ( value ? "On!" : "Off!" ) << endl;
    cout << "========================================" << endl;
  }

  fTMTab[module] -> SetEnabled(1, !value);
  fTMTab[module] -> SetTab(0, kFALSE);

  fUseSameCGroupSetting[module] = value;
}

void FADC400::SetTriggerModeCount(Bool_t value)
{
  TGCheckButton *object = (TGCheckButton *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channelGroup = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "=====================================================" << endl;
    cout << " SetTriggerModeCount module:" << module + 1;
    cout << " channelGroup:" << channelGroup + 1;
    cout << " is " << ( value ? "On!" : "Off!" ) << endl;
    cout << "=====================================================" << endl;
  }

  for (Int_t iOption = 0; iOption < 4; iOption++)
    fTMCountOption[module][channelGroup][iOption] -> SetEnabled(value);

  if (value)
    fTMCountOption[module][channelGroup][1] -> SetOn(kTRUE, kTRUE);

  fValueTMCount[module][channelGroup] = value;
}

void FADC400::SetTMCOption(Int_t value)
{
  Int_t module = value/100;
  Int_t channelGroup = (value%100)/10;
  Int_t mode = value%10;

  if (fIsDebug) {
    cout << "=================================================" << endl;
    cout << " SetTMCOption module:" << module + 1;
    cout << " channelGroup:" << channelGroup + 1;
    cout << " is ";
    
    Int_t lowCh = pow(3., channelGroup);
    Int_t highCh = pow(2., channelGroup + 1);
    if (mode == 0)
      cout << lowCh << " + " << highCh << "!" << endl;
    else if (mode == 1)
      cout << lowCh << "!" << endl;
    else if (mode == 2)
      cout << highCh << "!" << endl;
    else
      cout << lowCh << " OR " << highCh << "!" << endl;
    cout << "=================================================" << endl;
  }

  fValueTMCountOption[module][channelGroup] = mode;
}

void FADC400::SetTriggerModeWidth(Bool_t value)
{
  TGCheckButton *object = (TGCheckButton *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channelGroup = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "=====================================================" << endl;
    cout << " SetTriggerModeWidth module:" << module + 1;
    cout << " channelGroup:" << channelGroup + 1;
    cout << " is " << ( value ? "On!" : "Off!" ) << endl;
    cout << "=====================================================" << endl;
  }

  for (Int_t iOption = 0; iOption < 4; iOption++)
    fTMWidthOption[module][channelGroup][iOption] -> SetEnabled(value);

  if (value)
    fTMWidthOption[module][channelGroup][1] -> SetOn(kTRUE, kTRUE);

  fValueTMWidth[module][channelGroup] = value;
}

void FADC400::SetTMWOption(Int_t value)
{
  Int_t module = value/100;
  Int_t channelGroup = (value%100)/10;
  Int_t mode = value%10;

  if (fIsDebug) {
    cout << "==================================================" << endl;
    cout << " SetTMWOption module:" << module + 1;
    cout << " channelGroup:" << channelGroup + 1;
    cout << " is ";
    
    Int_t lowCh = pow(3., channelGroup);
    Int_t highCh = pow(2., channelGroup + 1);
    if (mode == 0)
      cout << lowCh << " OR " << highCh << "!" << endl;
    else if (mode == 1)
      cout << lowCh << "!" << endl;
    else if (mode == 2)
      cout << highCh << "!" << endl;
    else
      cout << lowCh << " AND " << highCh << "!" << endl;
    cout << "==================================================" << endl;
  }

  fValueTMWidthOption[module][channelGroup] = mode;
}

Int_t main(int argc, char **argv)
{
	TApplication theApp("FADC400GUI", &argc, argv);
	new FADC400(gClient -> GetRoot(), 768, 450);
	theApp.Run();

	return 0;
}
