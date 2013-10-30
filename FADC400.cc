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
#include <fstream>

using std::atoi;
using std::atof;
using std::ios;
using std::ifstream;
using std::ofstream;

#include "TApplication.h"
#include "Riostream.h"
#include "TSystem.h"
#include "TGClient.h"

#include "TGFileDialog.h"
#include "TObjArray.h"
#include "TObjString.h"

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
  Int_t moduleFrameHeight = 367;
  fModuleFrame -> MoveResize(moduleFrameHMargin, moduleFrameVMargin, moduleFrameWidth, moduleFrameHeight);

  fSameModuleSettingButton = new TGCheckButton(fModuleFrame, "Use the same setting for all modules. (Check this after setting address of the other module.)");
  fSameModuleSettingButton -> Connect("Toggled(Bool_t)", "FADC400", this, "SetSameModuleSetting(Bool_t)");
  fSameModuleSettingButton -> Move(10, 20);

  fModuleTab = new TGTab(fModuleFrame);
  fFADC[0] = fModuleTab -> AddTab("FADC400 Module 1");
  fFADC[0] -> SetLayoutBroken(kTRUE);
  fFADC[1] = fModuleTab -> AddTab("FADC400 Module 2");
  fFADC[1] -> SetLayoutBroken(kTRUE);

  fModuleFrame -> AddFrame(fModuleTab, new TGLayoutHints(kLHintsLeft|kLHintsTop));
  Int_t moduleTabHMargin = 8;
  Int_t moduleTabWidth = moduleFrameWidth - 2*moduleTabHMargin;
  Int_t moduleTabHeight = moduleFrameHeight - 61;
  fModuleTab -> MoveResize(moduleTabHMargin, 45, moduleTabWidth, moduleTabHeight);
  AddFrame(fModuleFrame, new TGLayoutHints(kLHintsLeft|kLHintsTop));

  // == Start of Modules  =======================================================
  for (Int_t iModule = 0; iModule < 2; iModule++) {
    fTextAddress = new TGLabel(fFADC[iModule], "Address:");
    fTextAddress -> Move(10, 10);

    fAddress[iModule] = new TGComboBox(fFADC[iModule], iModule);
    for (Int_t iAddress = 0; iAddress < 6; iAddress++)
      fAddress[iModule] -> AddEntry(Form("%d", iAddress), iAddress);
  
    fAddress[iModule] -> Connect("Selected(Int_t)", "FADC400", this, "SetAddress(Int_t)");
    fAddress[iModule] -> Select(3 + iModule);
    fAddress[iModule] -> MoveResize(70, 8, 50, 20);
    fFADC[iModule] -> AddFrame(fAddress[iModule]);

    fActive[iModule] = new TGCheckButton(fFADC[iModule], "Active", iModule);
    fActive[iModule] -> Connect("Toggled(Bool_t)", "FADC400", this, "SetActive(Bool_t)");
    fActive[iModule] -> Move(140, 10);

    // == Start of Channel Setting Frame ========================================
    fChannelFrame[iModule] = new TGGroupFrame(fFADC[iModule], "Channel Setting");
    fChannelFrame[iModule] -> SetLayoutBroken(kTRUE);

    Int_t channelFrameHMargin = 5;
    Int_t channelFrameWidth = 286;
    Int_t channelFrameHeight = 247;
    fChannelFrame[iModule] -> MoveResize(channelFrameHMargin, 35, channelFrameWidth, channelFrameHeight);
    fFADC[iModule] -> AddFrame(fChannelFrame[iModule]);

    // == Start of Channel Tab ==================================================
    fSameChannelSettingButton[iModule] = new TGCheckButton(fChannelFrame[iModule], "Use the same settings for all channel.", iModule);
    fSameChannelSettingButton[iModule] -> Connect("Toggled(Bool_t)", "FADC400", this, "SetSameChannelSetting(Bool_t)");
    fSameChannelSettingButton[iModule] -> Move(10, 20);

    fChannelTab[iModule] = new TGTab(fChannelFrame[iModule]);
    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fCh[iModule][iChannel] = fChannelTab[iModule] -> AddTab(Form("Ch %d", iChannel + 1));
      fCh[iModule][iChannel] -> SetLayoutBroken(kTRUE);
    }

    fChannelFrame[iModule] -> AddFrame(fChannelTab[iModule], new TGLayoutHints(kLHintsLeft|kLHintsTop));
    Int_t channelTabHMargin = 8;
    Int_t channelTabWidth = channelFrameWidth - 2*channelTabHMargin;
    Int_t channelTabHeight = channelFrameHeight - 60;
    fChannelTab[iModule] -> MoveResize(channelTabHMargin, 45, channelTabWidth, channelTabHeight);
    fFADC[iModule] -> AddFrame(fChannelFrame[iModule], new TGLayoutHints(kLHintsLeft|kLHintsTop));

    for (Int_t iChannel = 0; iChannel < 4; iChannel++ ) {
      Int_t widgetID = iModule*100 + iChannel*10;

      // == Start of Data saving mode =============================================
      fTextDSM = new TGLabel(fCh[iModule][iChannel], "Data saving mode");
      fTextDSM -> Move(10, 10);
      fDSM[iModule][iChannel] = new TGComboBox(fCh[iModule][iChannel], widgetID);
      fDSM[iModule][iChannel] -> AddEntry("Raw", 0);
      fDSM[iModule][iChannel] -> AddEntry("Smooth", 1);
      fDSM[iModule][iChannel] -> Connect("Selected(Int_t)", "FADC400", this, "SetDSM(Int_t)");
      fDSM[iModule][iChannel] -> Select(0);
      fDSM[iModule][iChannel] -> MoveResize(185, 10, 70, 18);
      fCh[iModule][iChannel] -> AddFrame(fDSM[iModule][iChannel]);
      // == End of Data saving mode ===============================================

      // == Start of Input polarity ===============================================
      fTextIP = new TGLabel(fCh[iModule][iChannel], "Input polarity");
      fTextIP -> Move(10, 35);
      fIP[iModule][iChannel] = new TGComboBox(fCh[iModule][iChannel], widgetID);
      fIP[iModule][iChannel] -> AddEntry("(-)", 0);
      fIP[iModule][iChannel] -> AddEntry("(+)", 1);
      fIP[iModule][iChannel] -> Connect("Selected(Int_t)", "FADC400", this, "SetIP(Int_t)");
      fIP[iModule][iChannel] -> Select(0);
      fIP[iModule][iChannel] -> MoveResize(185, 35, 70, 18);
      fCh[iModule][iChannel] -> AddFrame(fIP[iModule][iChannel]);
      // == End of Input polarity =================================================

      // == Start of Input delay ==================================================
      fTextID = new TGLabel(fCh[iModule][iChannel], "Input delay (0 ~ 40900) (ns)");
      fTextID -> Move(10, 60);
      fID[iModule][iChannel] = new TGNumberEntryField(fCh[iModule][iChannel], widgetID, 0, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 40900);
      fID[iModule][iChannel] -> Connect("TextChanged(const Char_t *)", "FADC400", this, "SetID(const Char_t *)");
      fID[iModule][iChannel] -> MoveResize(185, 60, 70, 18);
      fCh[iModule][iChannel] -> AddFrame(fID[iModule][iChannel]);
      // == End of Input delay ====================================================

      // == Start of ADC Offset ===================================================
      fTextAO = new TGLabel(fCh[iModule][iChannel], "ADC Offset (0 ~ 4095) (ns)");
      fTextAO -> Move(10, 85);
      fAO[iModule][iChannel] = new TGNumberEntryField(fCh[iModule][iChannel], widgetID, 0, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 4095);
      fAO[iModule][iChannel] -> Connect("TextChanged(const Char_t *)", "FADC400", this, "SetAO(const Char_t *)");
      fAO[iModule][iChannel] -> MoveResize(185, 85, 70, 18);
      fCh[iModule][iChannel] -> AddFrame(fAO[iModule][iChannel]);
      // == End of ADC Offset =====================================================

      // == Start of Threshold ====================================================
      fTextThres = new TGLabel(fCh[iModule][iChannel], "Threshold (0 ~ 1023)");
      fTextThres -> Move(10, 110);
      fThres[iModule][iChannel] = new TGNumberEntryField(fCh[iModule][iChannel], widgetID, 0, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 1023);
      fThres[iModule][iChannel] -> Connect("TextChanged(const Char_t *)", "FADC400", this, "SetThres(const Char_t *)");
      fThres[iModule][iChannel] -> MoveResize(185, 110, 70, 18);
      fCh[iModule][iChannel] -> AddFrame(fThres[iModule][iChannel]);
      // == End of Threshold ======================================================

      // == Start of Recording length =============================================
      fTextRL = new TGLabel(fCh[iModule][iChannel], "Recording length (us)");
      fTextRL -> Move(10, 135);
      fRL[iModule][iChannel] = new TGComboBox(fCh[iModule][iChannel], widgetID);
      for (Int_t iRL = 0; iRL < 10; iRL++)
        fRL[iModule][iChannel] -> AddEntry(Form("%.2f", 2.56*pow(2., iRL)), pow(2., iRL));
      fRL[iModule][iChannel] -> Connect("Selected(Int_t)", "FADC400", this, "SetRL(Int_t)");
      fRL[iModule][iChannel] -> Select(1);
      fRL[iModule][iChannel] -> MoveResize(185, 135, 70, 18);
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

    // == Start of Trigger Output Frame =========================================
    fTriggerFrame[iModule] = new TGGroupFrame(fFADC[iModule], "Trigger Output Setting");
    fTriggerFrame[iModule] -> SetLayoutBroken(kTRUE);

    Int_t triggerFrameHMargin = 5;
    Int_t triggerFrameWidth = 440;
    Int_t triggerFrameHeight = 277;
    fTriggerFrame[iModule] -> MoveResize(channelFrameWidth + 2*triggerFrameHMargin, 5, triggerFrameWidth, triggerFrameHeight);
    fFADC[iModule] -> AddFrame(fTriggerFrame[iModule]);

    // == Start of Condition Lookup Table =======================================
    fTextCLT = new TGLabel(fTriggerFrame[iModule], "Trigger off if trigger signal exists from Ch 1&2");
    fTextCLT -> Move(10, 22);
    fTextCLT = new TGLabel(fTriggerFrame[iModule], "Ch 3&4.");
    fTextCLT -> Move(325, 22);
    fCLT[iModule] = new TGComboBox(fTriggerFrame[iModule], iModule*100);
    fCLT[iModule] -> AddEntry("AND", 0);
    fCLT[iModule] -> AddEntry("OR", 1);
    fCLT[iModule] -> Connect("Selected(Int_t)", "FADC400", this, "SetCLT(Int_t)");
    fCLT[iModule] -> Select(0);
    fCLT[iModule] -> MoveResize(265, 22, 55, 18);
    fTriggerFrame[iModule] -> AddFrame(fCLT[iModule]);
    // == End of Condition Lookup Table =========================================

    fSameCGroupSettingButton[iModule] = new TGCheckButton(fTriggerFrame[iModule], "Use the same setting for all channel group.", iModule);
    fSameCGroupSettingButton[iModule] -> Connect("Toggled(Bool_t)", "FADC400", this, "SetSameCGroupSetting(Bool_t)");
    fSameCGroupSettingButton[iModule] -> Move(10, 44);

    // == Start of Trigger Mode Channel Group ===================================
    fTMTab[iModule] = new TGTab(fTriggerFrame[iModule]);
    fTMCG[iModule][0] = fTMTab[iModule] -> AddTab("Channel 1 & 2");
    fTMCG[iModule][0] -> SetLayoutBroken(kTRUE);
    fTMCG[iModule][1] = fTMTab[iModule] -> AddTab("Channel 3 & 4");
    fTMCG[iModule][1] -> SetLayoutBroken(kTRUE);

    // == Start of Trigger Mode Channel Group Tab ===============================
    fTriggerFrame[iModule] -> AddFrame(fTMTab[iModule], new TGLayoutHints(kLHintsLeft|kLHintsTop));
    Int_t tmodeTabHMargin = 8;
    Int_t tmodeTabWidth = triggerFrameWidth - 2*tmodeTabHMargin;
    Int_t tmodeTabHeight = triggerFrameHeight - 83;
    fTMTab[iModule] -> MoveResize(tmodeTabHMargin, 68, tmodeTabWidth, tmodeTabHeight);
    fTriggerFrame[iModule] -> AddFrame(fTMTab[iModule]);

    for (Int_t iCGroup = 0; iCGroup < 2; iCGroup++) {
      Int_t widgetID = iModule*100 + iCGroup*10;

      // == Start of Deadtime =====================================================
      fTextDT = new TGLabel(fTMCG[iModule][iCGroup], "Deadtime (0 ~ 104448000) (ns)");
      fTextDT -> Move(10, 10);
      fDT[iModule][iCGroup] = new TGNumberEntryField(fTMCG[iModule][iCGroup], widgetID, 0, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 104448000);
      fDT[iModule][iCGroup] -> Connect("TextChanged(const Char_t *)", "FADC400", this, "SetDT(const Char_t *)");
      fDT[iModule][iCGroup] -> MoveResize(310, 10, 100, 18);
      fTMCG[iModule][iCGroup] -> AddFrame(fDT[iModule][iCGroup]);
      // == End of Deadtime =======================================================

      // == Start of Coincidence width ============================================
      fTextCW = new TGLabel(fTMCG[iModule][iCGroup], "Coincidence width (160 ~ 40800) (ns)");
      fTextCW -> Move(10, 35);
      fCW[iModule][iCGroup] = new TGNumberEntryField(fTMCG[iModule][iCGroup], widgetID, 160, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 160, 40800);
      fCW[iModule][iCGroup] -> Connect("TextChanged(const Char_t *)", "FADC400", this, "SetCW(const Char_t *)");
      fCW[iModule][iCGroup] -> MoveResize(310, 35, 100, 18);
      fTMCG[iModule][iCGroup] -> AddFrame(fCW[iModule][iCGroup]);
      // == End of Deadtime =======================================================

      // == Start of Tab and Return pressed on TextEntries ========================
      fDT[iModule][iCGroup] -> Connect("TabPressed()", "TGNumberEntryField", fCW[iModule][iCGroup], "SetFocus()");
      fDT[iModule][iCGroup] -> Connect("ReturnPressed()", "TGNumberEntryField", fCW[iModule][iCGroup], "SetFocus()");
      fCW[iModule][iCGroup] -> Connect("TabPressed()", "TGNumberEntryField", fDT[iModule][iCGroup], "SetFocus()");
      fCW[iModule][iCGroup] -> Connect("ReturnPressed()", "TGNumberEntryField", fDT[iModule][iCGroup], "SetFocus()");
      // == End of Tab and Return pressed on TextEntries ==========================

      Int_t lowCh = pow(3., iCGroup);
      Int_t highCh = pow(2., iCGroup + 1);

      fTMWidth[iModule][iCGroup] = new TGCheckButton(fTMCG[iModule][iCGroup], "Width of", widgetID);
      fTMWidth[iModule][iCGroup] -> Connect("Toggled(Bool_t)", "FADC400", this, "SetTriggerModeWidth(Bool_t)");
      fTMWidth[iModule][iCGroup] -> Move(10, 60);
      fTMWidthOption[iModule][iCGroup] = new TGComboBox(fTMCG[iModule][iCGroup], widgetID);
      fTMWidthOption[iModule][iCGroup] -> MoveResize(85, 60, 75, 18);
      fTMWidthOption[iModule][iCGroup] -> AddEntry(Form("%d OR %d", lowCh, highCh), 0);
      fTMWidthOption[iModule][iCGroup] -> AddEntry(Form("%d", lowCh), 1);
      fTMWidthOption[iModule][iCGroup] -> AddEntry(Form("%d", highCh), 2);
      fTMWidthOption[iModule][iCGroup] -> AddEntry(Form("%d AND %d", lowCh, highCh), 3);
      fTMWidthOption[iModule][iCGroup] -> Connect("Selected(Int_t)", "FADC400", this, "SetTMWOption(Int_t)");
      fTMWidthOption[iModule][iCGroup] -> Select(0);
      fTMWidthOption[iModule][iCGroup] -> SetEnabled(kFALSE);
      fTMCG[iModule][iCGroup] -> AddFrame(fTMWidthOption[iModule][iCGroup]);
      fTextTWidth = new TGLabel(fTMCG[iModule][iCGroup], "w/ Threshold (2.5 ~ 1277.5) (ns)");
      fTextTWidth -> Move(170, 60);
      fTMWidthThres[iModule][iCGroup] = new TGNumberEntryField(fTMCG[iModule][iCGroup], widgetID, 2.5, TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 2.5, 1277.5);
      fTMWidthThres[iModule][iCGroup] -> Connect("TextChanged(const Char_t *)", "FADC400", this, "SetTMWThres(const Char_t *)");
      fTMWidthThres[iModule][iCGroup] -> MoveResize(360, 60, 50, 18);
      fTMWidthThres[iModule][iCGroup] -> SetEnabled(kFALSE);
      fTMCG[iModule][iCGroup] -> AddFrame(fTMWidthThres[iModule][iCGroup]);

      fTMCount[iModule][iCGroup] = new TGCheckButton(fTMCG[iModule][iCGroup], "Count", widgetID);
      fTMCount[iModule][iCGroup] -> Connect("Toggled(Bool_t)", "FADC400", this, "SetTriggerModeCount(Bool_t)");
      fTMCount[iModule][iCGroup] -> Move(10, 85);

      fTMCountOptionGroup[iModule][iCGroup] = new TGButtonGroup(fTMCG[iModule][iCGroup]);
      fTMCountOptionGroup[iModule][iCGroup] -> SetLayoutBroken(kTRUE);
      fTMCountOption[iModule][iCGroup][0] = new TGRadioButton(fTMCountOptionGroup[iModule][iCGroup], Form("Counts         from Ch %d + Ch %d in               ns.", lowCh, highCh), widgetID);
      fTMCountOption[iModule][iCGroup][0] -> SetEnabled(kFALSE);
      fTMCountOption[iModule][iCGroup][0] -> Move(30, 0);
      fTMCountOption[iModule][iCGroup][1] = new TGRadioButton(fTMCountOptionGroup[iModule][iCGroup], Form("Counts         from      Ch %d      in               ns.", lowCh), widgetID + 1);
      fTMCountOption[iModule][iCGroup][1] -> SetEnabled(kFALSE);
      fTMCountOption[iModule][iCGroup][1] -> Move(30, 20);
      fTMCountOption[iModule][iCGroup][2] = new TGRadioButton(fTMCountOptionGroup[iModule][iCGroup], Form("Counts         from      Ch %d      in               ns.", highCh), widgetID + 2);
      fTMCountOption[iModule][iCGroup][2] -> SetEnabled(kFALSE);
      fTMCountOption[iModule][iCGroup][2] -> Move(30, 40);
      fTMCountOption[iModule][iCGroup][3] = new TGRadioButton(fTMCountOptionGroup[iModule][iCGroup], "", widgetID + 3);
      fTMCountOption[iModule][iCGroup][3] -> SetEnabled(kFALSE);
      fTMCountOption[iModule][iCGroup][3] -> Move(15, 30);
      fTMCountOptionGroup[iModule][iCGroup] -> Connect("Clicked(Int_t)", "FADC400", this, "SetTMCOption(Int_t)");
      fTMCountOptionGroup[iModule][iCGroup] -> MoveResize(50, 105, 400, 100);
      fTMCountOptionGroup[iModule][iCGroup] -> Show();
      fTMCG[iModule][iCGroup] -> AddFrame(fTMCountOptionGroup[iModule][iCGroup]);

      fTextTMCount = new TGLabel(fTMCG[iModule][iCGroup], "(1 ~ 15)");
      fTextTMCount -> Move(135, 90);

      fTextTMCount = new TGLabel(fTMCG[iModule][iCGroup], "(40 ~ 10200)");
      fTextTMCount -> Move(279, 90);

      for (Int_t iOption = 0; iOption < 3; iOption++) {
        fTMCountThres[iModule][iCGroup][iOption] = new TGNumberEntryField(fTMCG[iModule][iCGroup], widgetID + iOption, 1, TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 1, 15);
        fTMCountThres[iModule][iCGroup][iOption] -> Connect("TextChanged(const Char_t *)", "FADC400", this, "SetTMCThres(const Char_t *)");
        fTMCountThres[iModule][iCGroup][iOption] -> MoveResize(145, 105 + iOption*20, 25, 18);
        fTMCountThres[iModule][iCGroup][iOption] -> SetEnabled(kFALSE);
        fTMCG[iModule][iCGroup] -> AddFrame(fTMCountThres[iModule][iCGroup][iOption]);

        fTMCountInterval[iModule][iCGroup][iOption] = new TGNumberEntryField(fTMCG[iModule][iCGroup], widgetID + iOption, 40, TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 40, 10200);
        fTMCountInterval[iModule][iCGroup][iOption] -> Connect("TextChanged(const Char_t *)", "FADC400", this, "SetTMCInterval(const Char_t *)");
        fTMCountInterval[iModule][iCGroup][iOption] -> MoveResize(290, 105 + iOption*20, 50, 18);
        fTMCountInterval[iModule][iCGroup][iOption] -> SetEnabled(kFALSE);
        fTMCG[iModule][iCGroup] -> AddFrame(fTMCountInterval[iModule][iCGroup][iOption]);
      }
    }
    // == End of Trigger Mode Channel Group Tab =================================
    // == End of Trigger Mode Channel Group =====================================
    // == End of Trigger Output Frame ===========================================
  }
  // == End of Modules ============================================================
  // == End of Module Frame =======================================================

  // == Start of Number of Events =================================================
  fTextNumEvents = new TGLabel(this, "# of events");
  fTextNumEvents -> Move(568, 387);

  fNumEvents = new TGNumberEntryField(this, -1, 1000, TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMax, 0, 1569325055);
  fNumEvents -> Connect("TextChanged(const Char_t *)", "FADC400", this, "SetNumEvents(const Char_t *)");
  fNumEvents -> MoveResize(572, 403, 80, 18);
  AddFrame(fNumEvents);
  // == End of Number of Events ===================================================

  // == Start of Buttons ==========================================================
  fLoad = new TGTextButton(this, "Load Settings");
  fLoad -> Connect("Clicked()", "FADC400", this, "LoadSettings()");
  fLoad -> MoveResize(5, 380, 100, 50);

  fSave = new TGTextButton(this, "Save Settings");
  fSave -> Connect("Clicked()", "FADC400", this, "SaveSettings()");
  fSave -> MoveResize(105, 380, 100, 50);

  fStart = new TGTextButton(this, "Start");
  fStart -> Connect("Clicked()", "FADC400", this, "Start()");
  fStart -> MoveResize(662, 380, 100, 50);
  // == End of Buttons ============================================================

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
  fSettings.Initialize();
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

  fSettings.fUseSameModuleSetting = value;
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

  fSettings.fValueAddress[module] = value;
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

  fSettings.fIsActive[module] = value;
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

  fSettings.fUseSameChannelSetting[module] = value;
}

void FADC400::SetDSM(Int_t value)
{
  TGComboBox *object = (TGComboBox *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channel = (widgetID%100)/10;
  Int_t mode = value;

  if (fIsDebug) {
    cout << "======================================" << endl;
    cout << " SetDSM module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << ( mode ? "Smooth!" : "Raw!" ) << endl;
    cout << "======================================" << endl;
  }

  fSettings.fValueDSM[module][channel] = mode;
}

void FADC400::SetIP(Int_t value)
{
  TGComboBox *object = (TGComboBox *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channel = (widgetID%100)/10;
  Int_t mode = value;

  if (fIsDebug) {
    cout << "==================================" << endl;
    cout << " SetIP module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << ( mode ? "(+)!" : "(-)!" ) << endl;
    cout << "==================================" << endl;
  }

  fSettings.fValueIP[module][channel] = mode;
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

  fSettings.fValueID[module][channel] = atoi(value);
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

  fSettings.fValueAO[module][channel] = atoi(value);
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

  fSettings.fValueThres[module][channel] = atoi(value);
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

  fSettings.fValueRL[module][channel] = value;
}

void FADC400::SetDT(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t cgroup = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "=======================================" << endl;
    cout << " SetDT module:" << module + 1;
    cout << " cgroup:" << cgroup + 1;
    cout << " is " << atoi(value) << "!" << endl;
    cout << "=======================================" << endl;
  }

  fSettings.fValueDT[module] = atoi(value);
}

void FADC400::SetCW(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t cgroup = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "===================================" << endl;
    cout << " SetCW module:" << module + 1;
    cout << " cgroup:" << cgroup + 1;
    cout << " is " << atoi(value) << "!" << endl;
    cout << "===================================" << endl;
  }

  fSettings.fValueCW[module] = atoi(value);
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

  fSettings.fValueCLT[module] = mode;
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

  fSettings.fUseSameCGroupSetting[module] = value;
}

void FADC400::SetTriggerModeWidth(Bool_t value)
{
  TGCheckButton *object = (TGCheckButton *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t cgroup = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "=====================================================" << endl;
    cout << " SetTriggerModeWidth module:" << module + 1;
    cout << " cgroup:" << cgroup + 1;
    cout << " is " << ( value ? "On!" : "Off!" ) << endl;
    cout << "=====================================================" << endl;
  }

  fTMWidthOption[module][cgroup] -> SetEnabled(value);
  fTMWidthOption[module][cgroup] -> Select(0);
  fTMWidthThres[module][cgroup] -> SetEnabled(value);

  fSettings.fValueTMWidth[module][cgroup] = value;
}

void FADC400::SetTMWOption(Int_t value)
{
  TGComboBox *object = (TGComboBox *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t cgroup = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "==================================================" << endl;
    cout << " SetTMWOption module:" << module + 1;
    cout << " cgroup:" << cgroup + 1;
    cout << " is ";
    
    Int_t lowCh = pow(3., cgroup);
    Int_t highCh = pow(2., cgroup + 1);
    if (value == 0)
      cout << lowCh << " OR " << highCh << "!" << endl;
    else if (value == 1)
      cout << lowCh << "!" << endl;
    else if (value == 2)
      cout << highCh << "!" << endl;
    else
      cout << lowCh << " AND " << highCh << "!" << endl;
    cout << "==================================================" << endl;
  }

  fSettings.fValueTMWidthOption[module][cgroup] = value;
}

void FADC400::SetTMWThres(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t cgroup = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "=============================================" << endl;
    cout << " SetTMWThres module:" << module + 1;
    cout << " cgroup:" << cgroup + 1;
    cout << " is " << atof(value) << " ns!" << endl;
    cout << "=============================================" << endl;
  }

//  fSettings.f[module][cgroup] = atof(value);
}

void FADC400::SetTriggerModeCount(Bool_t value)
{
  TGCheckButton *object = (TGCheckButton *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t cgroup = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "=====================================================" << endl;
    cout << " SetTriggerModeCount module:" << module + 1;
    cout << " cgroup:" << cgroup + 1;
    cout << " is " << ( value ? "On!" : "Off!" ) << endl;
    cout << "=====================================================" << endl;
  }

  for (Int_t option = 0; option < 4; option++) {
    fTMCountOption[module][cgroup][option] -> SetEnabled(value);

    if (option < 3) {
      fTMCountThres[module][cgroup][option] -> SetEnabled(value);
      fTMCountInterval[module][cgroup][option] -> SetEnabled(value);
    }
  }

  if (value)
    fTMCountOption[module][cgroup][0] -> SetOn(kTRUE, kTRUE);

  fSettings.fValueTMCount[module][cgroup] = value;
}

void FADC400::SetTMCOption(Int_t value)
{
  Int_t module = value/100;
  Int_t cgroup = (value%100)/10;
  Int_t mode = value%10;

  if (fIsDebug) {
    cout << "=================================================" << endl;
    cout << " SetTMCOption module:" << module + 1;
    cout << " cgroup:" << cgroup + 1;
    cout << " is ";
    
    Int_t lowCh = pow(3., cgroup);
    Int_t highCh = pow(2., cgroup + 1);
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

  if (mode == 3) {
    for (Int_t option = 1; option < 3; option++) {
      fTMCountThres[module][cgroup][option] -> SetEnabled(kTRUE);
      fTMCountInterval[module][cgroup][option] -> SetEnabled(kTRUE);
    }
      fTMCountThres[module][cgroup][0] -> SetEnabled(kFALSE);
      fTMCountInterval[module][cgroup][0] -> SetEnabled(kFALSE);
  } else {
    for (Int_t option = 0; option < 3; option++) {
      if (option == mode) {
        fTMCountThres[module][cgroup][option] -> SetEnabled(kTRUE);
        fTMCountInterval[module][cgroup][option] -> SetEnabled(kTRUE);
      } else {
        fTMCountThres[module][cgroup][option] -> SetEnabled(kFALSE);
        fTMCountInterval[module][cgroup][option] -> SetEnabled(kFALSE);
      }
    }
  }

  fSettings.fValueTMCountOption[module][cgroup] = mode;
}

void FADC400::SetTMCThres(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t cgroup = (widgetID%100)/10;
  Int_t option = widgetID%10;

  if (fIsDebug) {
    cout << "===================================================" << endl;
    cout << " SetTMCThres module:" << module + 1;
    cout << " cgroup:" << cgroup + 1;
    cout << " option:";

    Int_t lowCh = pow(3., cgroup);
    Int_t highCh = pow(2., cgroup + 1);
//    if (option != 2) { when the mode is not 3
      if (option == 0)
        cout << lowCh << "+" << highCh;
      else if (option == 1)
        cout << lowCh;
      else if (option == 2)
        cout << highCh;

      cout << " is " << atoi(value) << "!" << endl;
        /* when the mode is 3
    } else {
      cout << lowCh << "AND" << highCh;
      cout << " is " << atoi(value) << "!" << endl;
    }
    */
    cout << "===================================================" << endl;

  }
}

void FADC400::SetTMCInterval(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t cgroup = (widgetID%100)/10;
  Int_t option = widgetID%10;

  if (fIsDebug) {
    cout << "==========================================================" << endl;
    cout << " SetTMCInterval module:" << module + 1;
    cout << " cgroup:" << cgroup + 1;
    cout << " option: ";

    Int_t lowCh = pow(3., cgroup);
    Int_t highCh = pow(2., cgroup + 1);
//    if (option != 2) { when the mode is not 3
      if (option == 0)
        cout << lowCh << "+" << highCh;
      else if (option == 1)
        cout << lowCh;
      else if (option == 2)
        cout << highCh;

      cout << " is " << atoi(value) << "!" << endl;
        /* when the mode is 3
    } else {
      cout << lowCh << "AND" << highCh;
      cout << " is " << atoi(value) << "!" << endl;
    }
    */
    cout << "==========================================================" << endl;
  }
}

void FADC400::SaveSettings()
{
  SetSettingsFromUI();

  // Open dialog for saving action.
  TGFileInfo fileInfo;
  const Char_t *fileType[4] = {"FADC400CFG files", "*.fadc400cfg", 0, 0}; 
  fileInfo.fFileTypes = fileType;
  new TGFileDialog(gClient -> GetRoot(), this, kFDSave, &fileInfo);

  // If user clicks cancel, do nothing.
  if (fileInfo.fFilename == NULL)
    return;

  // Get the file name with path
  TString filenameWithPath = fileInfo.fFilename;
	TObjArray *decomposedFileNameWithPath = filenameWithPath.Tokenize("/");
  TString savingFile = ((TObjString *) decomposedFileNameWithPath -> Last()) -> GetString();

  // If the extension is not "fadc400cfg", add it.
  TString extension = savingFile(savingFile.Length() - 11, 11);
  if (!extension.EqualTo(".fadc400cfg")) {
    filenameWithPath.Append(".fadc400cfg");
    savingFile.Append(".fadc400cfg");
  }

  delete decomposedFileNameWithPath;
  
  ofstream saving(filenameWithPath.Data(), ios::out|ios::binary);
  saving.write(reinterpret_cast<Char_t *>(&fSettings), sizeof(fSettings));
  saving.close();

  if (fIsDebug) {
    cout << "===================================" << endl;
    cout << " Parameters' setting is saved into" << endl;
    cout << " " << savingFile.Data() << "!" << endl;
    cout << "===================================" << endl;
  }
}

void FADC400::SetNumEvents(const Char_t *value)
{
  if (fIsDebug) {
    cout << "=============================" << endl;
    cout << " SetNumEvents";
    cout << " is " << atoi(value) << "!" << endl;
    cout << "=============================" << endl;
  }

  fSettings.fValueNumEvents = atoi(value);
}

void FADC400::LoadSettings()
{
  // Open dialog for loading action.
  TGFileInfo fileInfo;
  const Char_t *fileType[4] = {"FADC400CFG files", "*.fadc400cfg", 0, 0}; 
  fileInfo.fFileTypes = fileType;
  new TGFileDialog(gClient -> GetRoot(), this, kFDOpen, &fileInfo);

  // If user clicks cancel, do nothing.
  if (fileInfo.fFilename == NULL)
    return;

  // Get the file name with path
  TString filenameWithPath = fileInfo.fFilename;
	TObjArray *decomposedFileNameWithPath = filenameWithPath.Tokenize("/");
  TString loadingFile = ((TObjString *) decomposedFileNameWithPath -> Last()) -> GetString();

  delete decomposedFileNameWithPath;

  // If the extension is not "fadc400cfg", it is not loaded.
  TString extension = filenameWithPath(filenameWithPath.Length() - 11, 11);
  if (!extension.EqualTo(".fadc400cfg")) {
    cout << "====================================" << endl;
    cout << " The file you chose cannot be used!" << endl;
    cout << "====================================" << endl;

    return;
  } else {
    ifstream loading(filenameWithPath.Data(), ios::in|ios::binary);
    loading.read(reinterpret_cast<Char_t *>(&fSettings), sizeof(fSettings));

    if (fIsDebug) {
      cout << "====================================" << endl;
      cout << " Parameters' setting is loaded from" << endl;
      cout << " " << loadingFile.Data() << "!" << endl;
      cout << "====================================" << endl;
    }

    SetSettingsToUI();
  }
}

void FADC400::Start()
{
  SetSettingsFromUI();
}

void FADC400::SetSettingsFromUI()
{
  // Confirm the values in NumberEntryFields
  for (Int_t iModule = 0; iModule < 2; iModule++) {
    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fID[iModule][iChannel] -> TextChanged();
      fAO[iModule][iChannel] -> TextChanged();
      fThres[iModule][iChannel] -> TextChanged();
    }

    for (Int_t iCGroup = 0; iCGroup < 2; iCGroup++) {
      fDT[iModule][iCGroup] -> TextChanged();
      fCW[iModule][iCGroup] -> TextChanged();
    }
  }

  fNumEvents -> TextChanged();

  for (Int_t iModule = 0; iModule < 2; iModule++) {
    if (fSettings.fUseSameCGroupSetting[iModule])
      fSettings.UseSameCGroupSetting(iModule);

    if (fSettings.fUseSameChannelSetting[iModule])
      fSettings.UseSameChannelSetting(iModule);
  }

  if (fSettings.fUseSameModuleSetting) 
    fSettings.UseSameModuleSetting();
}

void FADC400::SetSettingsToUI()
{
  if (fSettings.fUseSameModuleSetting)
    fSameModuleSettingButton -> SetState(kButtonDown, kTRUE);
  else
    fSameModuleSettingButton -> SetState(kButtonUp, kTRUE);

  for (Int_t iModule = 0; iModule < 2; iModule++) {
    fAddress[iModule] -> Select(fSettings.fValueAddress[iModule]);
    if (fSettings.fIsActive[iModule])
      fActive[iModule] -> SetState(kButtonDown, kTRUE);
    else
      fActive[iModule] -> SetState(kButtonUp, kTRUE);

    if (fSettings.fUseSameChannelSetting[iModule])
      fSameChannelSettingButton[iModule] -> SetState(kButtonDown, kTRUE);
    else
      fSameChannelSettingButton[iModule] -> SetState(kButtonUp, kTRUE);

    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fDSM[iModule][iChannel] -> Select(fSettings.fValueDSM[iModule][iChannel]);
      fIP[iModule][iChannel] -> Select(fSettings.fValueIP[iModule][iChannel]);
      fID[iModule][iChannel] -> SetText(Form("%d", fSettings.fValueID[iModule][iChannel]), kFALSE);
      fAO[iModule][iChannel] -> SetText(Form("%d", fSettings.fValueAO[iModule][iChannel]), kFALSE);
      fThres[iModule][iChannel] -> SetText(Form("%d", fSettings.fValueThres[iModule][iChannel]), kFALSE);
      fRL[iModule][iChannel] -> Select(fSettings.fValueRL[iModule][iChannel]);
    }

    fCLT[iModule] -> Select(fSettings.fValueCLT[iModule]);

    if (fSettings.fUseSameCGroupSetting[iModule])
      fSameCGroupSettingButton[iModule] -> SetState(kButtonDown, kTRUE);
    else
      fSameCGroupSettingButton[iModule] -> SetState(kButtonUp, kTRUE);

    for (Int_t iCGroup = 0; iCGroup < 2; iCGroup++) {
      fDT[iModule][iCGroup] -> SetText(Form("%d", fSettings.fValueDT[iModule]), kFALSE);
//      fDTApplied[iModule][fSettings.fValueDTApplied[iModule]] -> SetState(kButtonDown, kTRUE);
      fCW[iModule][iCGroup] -> SetText(Form("%d", fSettings.fValueCW[iModule]), kFALSE);
//      fCWApplied[iModule][fSettings.fValueCWApplied[iModule]] -> SetState(kButtonDown, kTRUE);

      if (fSettings.fValueTMCount[iModule][iCGroup]) {
        fTMCount[iModule][iCGroup] -> SetState(kButtonDown, kTRUE);
        fTMCountOption[iModule][iCGroup][fSettings.fValueTMCountOption[iModule][iCGroup]] -> SetState(kButtonDown, kTRUE);
      } else {
        fTMCount[iModule][iCGroup] -> SetState(kButtonUp, kTRUE);
      }

      if (fSettings.fValueTMWidth[iModule][iCGroup]) {
        fTMWidth[iModule][iCGroup] -> SetState(kButtonDown, kTRUE);
        fTMWidthOption[iModule][iCGroup] -> Select(fSettings.fValueTMWidthOption[iModule][iCGroup]);
      } else {
        fTMWidth[iModule][iCGroup] -> SetState(kButtonUp, kTRUE);
      }
    }
  }

  fNumEvents -> SetText(Form("%d", fSettings.fValueNumEvents), kFALSE);
}

Int_t main(int argc, char **argv)
{
	TApplication theApp("FADC400GUI", &argc, argv);
	new FADC400(gClient -> GetRoot(), 770, 440);
	theApp.Run();

	return 0;
}
