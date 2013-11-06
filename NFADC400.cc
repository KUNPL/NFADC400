/////////////////////////////////////
//                                 //
//       NFADC400 module GUI       //
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

#include "NFADC400.hh"
#include "NFADC400Process.hh"

ClassImp(NFADC400);

NFADC400::NFADC400(const TGWindow *window, UInt_t width, UInt_t height)
:TGMainFrame(window, width, height, kMainFrame)
{
  Initialize();

	FILE *fp = fopen("DEBUG", "r");
	if (fp) {
		fclose(fp);
		fIsDebug = kTRUE;
	} else  
		fIsDebug = kFALSE;

  SetWindowName("NFADC400 Controller");
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
  fSameModuleSettingButton -> Connect("Toggled(Bool_t)", "NFADC400", this, "SetSameModuleSetting(Bool_t)");
  fSameModuleSettingButton -> Move(10, 20);

  fModuleTab = new TGTab(fModuleFrame);
  fModule[0] = fModuleTab -> AddTab("NFADC400 Module 1");
  fModule[0] -> SetLayoutBroken(kTRUE);
  fModule[1] = fModuleTab -> AddTab("NFADC400 Module 2");
  fModule[1] -> SetLayoutBroken(kTRUE);

  fModuleFrame -> AddFrame(fModuleTab, new TGLayoutHints(kLHintsLeft|kLHintsTop));
  Int_t moduleTabHMargin = 8;
  Int_t moduleTabWidth = moduleFrameWidth - 2*moduleTabHMargin;
  Int_t moduleTabHeight = moduleFrameHeight - 61;
  fModuleTab -> MoveResize(moduleTabHMargin, 45, moduleTabWidth, moduleTabHeight);
  AddFrame(fModuleFrame, new TGLayoutHints(kLHintsLeft|kLHintsTop));

  // == Start of Modules  =======================================================
  for (Int_t iModule = 0; iModule < 2; iModule++) {

    // == Start of Module Setting Frame =========================================
    fModuleSettingFrame[iModule] = new TGGroupFrame(fModule[iModule], "Module Setting");
    fModuleSettingFrame[iModule] -> SetLayoutBroken(kTRUE);

    Int_t moduleFrameHMargin = 5;
    Int_t moduleFrameWidth = 286;
    Int_t moduleFrameHeight = 55;
    fModuleSettingFrame[iModule] -> MoveResize(moduleFrameHMargin, 5, moduleFrameWidth, moduleFrameHeight);
    fModule[iModule] -> AddFrame(fModuleSettingFrame[iModule]);

    fActive[iModule] = new TGCheckButton(fModuleSettingFrame[iModule], "Active", iModule);
    fActive[iModule] -> Connect("Toggled(Bool_t)", "NFADC400", this, "SetActive(Bool_t)");
    fActive[iModule] -> Move(40, 21);

    fTextAddress = new TGLabel(fModuleSettingFrame[iModule], "Address:");
    fTextAddress -> Move(130, 22);

    fAddress[iModule] = new TGComboBox(fModuleSettingFrame[iModule], iModule);
    for (Int_t iAddress = 0; iAddress < 6; iAddress++)
      fAddress[iModule] -> AddEntry(Form("%d", iAddress), iAddress);
  
    fAddress[iModule] -> Connect("Selected(Int_t)", "NFADC400", this, "SetAddress(Int_t)");
    fAddress[iModule] -> Select(3 + iModule);
    fAddress[iModule] -> MoveResize(185, 20, 50, 20);
    fModuleSettingFrame[iModule] -> AddFrame(fAddress[iModule]);
    // == End of Module Setting Frame ===========================================

    // == Start of Channel Setting Frame ========================================
    fChannelFrame[iModule] = new TGGroupFrame(fModule[iModule], "Channel Setting");
    fChannelFrame[iModule] -> SetLayoutBroken(kTRUE);

    Int_t channelFrameHMargin = 5;
    Int_t channelFrameWidth = 286;
    Int_t channelFrameHeight = 226;
    fChannelFrame[iModule] -> MoveResize(channelFrameHMargin, 53, channelFrameWidth, channelFrameHeight);
    fModule[iModule] -> AddFrame(fChannelFrame[iModule]);

    // == Start of Recording length =============================================
    fTextRL = new TGLabel(fChannelFrame[iModule], "Recording length (us)");
    fTextRL -> Move(10, 20);
    fRL[iModule] = new TGComboBox(fChannelFrame[iModule], iModule);
    for (Int_t iRL = 0; iRL < 13; iRL++)
      fRL[iModule] -> AddEntry(Form("%.2f", 0.32*pow(2., iRL)), pow(2., iRL));
    fRL[iModule] -> Connect("Selected(Int_t)", "NFADC400", this, "SetRL(Int_t)");
    fRL[iModule] -> Select(1);
    fRL[iModule] -> MoveResize(208, 20, 70, 18);
    fChannelFrame[iModule] -> AddFrame(fRL[iModule]);
    // == End of Recording length ===============================================

    fSameChannelSettingButton[iModule] = new TGCheckButton(fChannelFrame[iModule], "Use the same settings for all channels.", iModule);
    fSameChannelSettingButton[iModule] -> Connect("Toggled(Bool_t)", "NFADC400", this, "SetSameChannelSetting(Bool_t)");
    fSameChannelSettingButton[iModule] -> Move(10, 45);

    // == Start of Channel Tab ==================================================
    fChannelTab[iModule] = new TGTab(fChannelFrame[iModule]);
    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fCh[iModule][iChannel] = fChannelTab[iModule] -> AddTab(Form("Ch %d", iChannel + 1));
      fCh[iModule][iChannel] -> SetLayoutBroken(kTRUE);
    }

    fChannelFrame[iModule] -> AddFrame(fChannelTab[iModule], new TGLayoutHints(kLHintsLeft|kLHintsTop));
    Int_t channelTabHMargin = 8;
    Int_t channelTabWidth = channelFrameWidth - 2*channelTabHMargin;
    Int_t channelTabHeight = channelFrameHeight - 80;
    fChannelTab[iModule] -> MoveResize(channelTabHMargin, 65, channelTabWidth, channelTabHeight);
    fModule[iModule] -> AddFrame(fChannelFrame[iModule], new TGLayoutHints(kLHintsLeft|kLHintsTop));

    for (Int_t iChannel = 0; iChannel < 4; iChannel++ ) {
      Int_t widgetID = iModule*100 + iChannel*10;

      // == Start of Active Channel ===============================================
      fAC[iModule][iChannel] = new TGCheckButton(fCh[iModule][iChannel], "Active", widgetID);
      fAC[iModule][iChannel] -> Connect("Toggled(Bool_t)", "NFADC400", this, "SetAC(Bool_t)");
      fAC[iModule][iChannel] -> Move(7, 7);
      // == End of Active Channel =================================================

      // == Start of Input polarity ===============================================
      fTextIP = new TGLabel(fCh[iModule][iChannel], "Input polarity");
      fTextIP -> Move(7, 29);
      fIP[iModule][iChannel] = new TGComboBox(fCh[iModule][iChannel], widgetID);
      fIP[iModule][iChannel] -> AddEntry("(-)", 0);
      fIP[iModule][iChannel] -> AddEntry("(+)", 1);
      fIP[iModule][iChannel] -> Connect("Selected(Int_t)", "NFADC400", this, "SetIP(Int_t)");
      fIP[iModule][iChannel] -> Select(0);
      fIP[iModule][iChannel] -> MoveResize(187, 29, 70, 18);
      fCh[iModule][iChannel] -> AddFrame(fIP[iModule][iChannel]);
      // == End of Input polarity =================================================

      // == Start of Input delay ==================================================
      fTextID = new TGLabel(fCh[iModule][iChannel], "Input delay (0 - 40940) (ns)");
      fTextID -> Move(7, 51);
      fID[iModule][iChannel] = new TGNumberEntryField(fCh[iModule][iChannel], widgetID, 10240, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 40940);
      fID[iModule][iChannel] -> Connect("TextChanged(const Char_t *)", "NFADC400", this, "SetID(const Char_t *)");
      fID[iModule][iChannel] -> MoveResize(187, 51, 70, 18);
      fCh[iModule][iChannel] -> AddFrame(fID[iModule][iChannel]);
      // == End of Input delay ====================================================

      // == Start of ADC Offset ===================================================
      fTextAO = new TGLabel(fCh[iModule][iChannel], "ADC offset (0 - 4095) (ns)");
      fTextAO -> Move(7, 73);
      fAO[iModule][iChannel] = new TGNumberEntryField(fCh[iModule][iChannel], widgetID, 0, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 4095);
      fAO[iModule][iChannel] -> Connect("TextChanged(const Char_t *)", "NFADC400", this, "SetAO(const Char_t *)");
      fAO[iModule][iChannel] -> MoveResize(187, 73, 70, 18);
      fCh[iModule][iChannel] -> AddFrame(fAO[iModule][iChannel]);
      // == End of ADC Offset =====================================================

      // == Start of Input amplifier gain =========================================
      fTextIAG = new TGLabel(fCh[iModule][iChannel], "Input amplifier gain (0 - 4095)");
      fTextIAG -> Move(7, 95);
      fIAG[iModule][iChannel] = new TGNumberEntryField(fCh[iModule][iChannel], widgetID, 0, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 4095);
      fIAG[iModule][iChannel] -> Connect("TextChanged(const Char_t *)", "NFADC400", this, "SetIAG(const Char_t *)");
      fIAG[iModule][iChannel] -> MoveResize(187, 95, 70, 18);
      fCh[iModule][iChannel] -> AddFrame(fIAG[iModule][iChannel]);
      // == End of Input amplifier gain ===========================================

      // == Start of Tab and Return pressed on TextEntries ========================
      fID[iModule][iChannel] -> Connect("TabPressed()", "TGNumberEntryField", fAO[iModule][iChannel], "SetFocus()");
      fID[iModule][iChannel] -> Connect("ReturnPressed()", "TGNumberEntryField", fAO[iModule][iChannel], "SetFocus()");
      fAO[iModule][iChannel] -> Connect("TabPressed()", "TGNumberEntryField", fIAG[iModule][iChannel], "SetFocus()");
      fAO[iModule][iChannel] -> Connect("ReturnPressed()", "TGNumberEntryField", fIAG[iModule][iChannel], "SetFocus()");
      fIAG[iModule][iChannel] -> Connect("TabPressed()", "TGNumberEntryField", fID[iModule][iChannel], "SetFocus()");
      fIAG[iModule][iChannel] -> Connect("ReturnPressed()", "TGNumberEntryField", fID[iModule][iChannel], "SetFocus()");
      // == End of Tab and Return pressed on TextEntries ==========================
    }
    // == End of Channel Tab ====================================================
    // == End of Channel Setting Frame ==========================================

    // == Start of Trigger Output Frame =========================================
    fTriggerFrame[iModule] = new TGGroupFrame(fModule[iModule], "Trigger Output Setting");
    fTriggerFrame[iModule] -> SetLayoutBroken(kTRUE);

    Int_t triggerFrameHMargin = 5;
    Int_t triggerFrameWidth = 390;
    Int_t triggerFrameHeight = 274;
    fTriggerFrame[iModule] -> MoveResize(channelFrameWidth + 2*triggerFrameHMargin, 5, triggerFrameWidth, triggerFrameHeight);
    fModule[iModule] -> AddFrame(fTriggerFrame[iModule]);

    // == Start of Condition Lookup Table =======================================
    fTextCLT[iModule] = new TGLabel(fTriggerFrame[iModule], "When trigger signals exist from all channels coincide.");
    fTextCLT[iModule] -> Move(70, 25);
    fTextCLT[iModule] -> SetTextJustify(kTextLeft);
    fCLT[iModule] = new TGComboBox(fTriggerFrame[iModule], iModule);
    fCLT[iModule] -> AddEntry("AND", 0);
    fCLT[iModule] -> AddEntry("OR", 1);
    fCLT[iModule] -> Connect("Selected(Int_t)", "NFADC400", this, "SetCLT(Int_t)");
    fCLT[iModule] -> Select(0);
    fCLT[iModule] -> MoveResize(10, 25, 55, 18);
    fTriggerFrame[iModule] -> AddFrame(fCLT[iModule]);
    // == End of Condition Lookup Table =========================================

    // == Start of Trigger Output Width =========================================
    fTextTOW = new TGLabel(fTriggerFrame[iModule], "Trigger output width (40 - 5100) (ns)");
    fTextTOW -> Move(7, 47);
    fTOW[iModule] = new TGNumberEntryField(fTriggerFrame[iModule], iModule, 960, TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 40, 5100);
    fTOW[iModule] -> Connect("TextChanged(const Char_t *)", "NFADC400", this, "SetTOW(const Char_t *)");
    fTOW[iModule] -> MoveResize(307, 47, 75, 18);
    fTriggerFrame[iModule] -> AddFrame(fTOW[iModule]);
    // == End of Trigger Output Width ===========================================

    // == Start of Trigger Daisy Chain ==========================================
    fTextTDC = new TGLabel(fTriggerFrame[iModule], "Trigger daisy chain");
    fTextTDC -> Move(7, 69);
    fTDC[iModule] = new TGComboBox(fTriggerFrame[iModule], iModule);
    fTDC[iModule] -> AddEntry("Enabled", 0);
    fTDC[iModule] -> AddEntry("Disabled", 1);
    fTDC[iModule] -> Connect("Selected(Int_t)", "NFADC400", this, "SetTDC(Int_t)");
    fTDC[iModule] -> Select(0);
    fTDC[iModule] -> MoveResize(307, 69, 75, 18);
    fTriggerFrame[iModule] -> AddFrame(fTDC[iModule]);
    // == End of Trigger Daisy Chain ============================================

    // == Start of Use The Same Channel Setting =================================
    fSameChannelTriggerSettingButton[iModule] = new TGCheckButton(fTriggerFrame[iModule], "Use the same setting for all channels.", iModule);
    fSameChannelTriggerSettingButton[iModule] -> Connect("Toggled(Bool_t)", "NFADC400", this, "SetSameChannelTriggerSetting(Bool_t)");
    fSameChannelTriggerSettingButton[iModule] -> Move(10, 94);
    // == End of Use the same channel setting ===================================

    // == Start of Trigger Mode Channel Group ===================================
    fTMTab[iModule] = new TGTab(fTriggerFrame[iModule]);
    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fTM[iModule][iChannel] = fTMTab[iModule] -> AddTab(Form("Ch %d", iChannel + 1));
      fTM[iModule][iChannel] -> SetLayoutBroken(kTRUE);
    }

    // == Start of Trigger Mode Channel Tab =====================================
    fTriggerFrame[iModule] -> AddFrame(fTMTab[iModule], new TGLayoutHints(kLHintsLeft|kLHintsTop));
    Int_t tmodeTabHMargin = 8;
    Int_t tmodeTabWidth = triggerFrameWidth - 2*tmodeTabHMargin;
    Int_t tmodeTabHeight = triggerFrameHeight - 129;
    fTMTab[iModule] -> MoveResize(tmodeTabHMargin, 114, tmodeTabWidth, tmodeTabHeight);
    fTriggerFrame[iModule] -> AddFrame(fTMTab[iModule]);

    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      Int_t widgetID = iModule*100 + iChannel*10;

      // == Start of Threshold ====================================================
      fTextThres = new TGLabel(fTM[iModule][iChannel], "Threshold (0 - 1023)");
      fTextThres -> Move(7, 7);
      fThres[iModule][iChannel] = new TGNumberEntryField(fTM[iModule][iChannel], widgetID, 1023, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 1023);
      fThres[iModule][iChannel] -> Connect("TextChanged(const Char_t *)", "NFADC400", this, "SetThres(const Char_t *)");
      fThres[iModule][iChannel] -> MoveResize(283, 7, 80, 18);
      fTM[iModule][iChannel] -> AddFrame(fThres[iModule][iChannel]);
      // == End of Threshold ======================================================

      // == Start of Deadtime =====================================================
      fTextDT = new TGLabel(fTM[iModule][iChannel], "Deadtime (0 - 10444800) (ns)");
      fTextDT -> Move(7, 29);
      fDT[iModule][iChannel] = new TGNumberEntryField(fTM[iModule][iChannel], widgetID, 0, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 10444800);
      fDT[iModule][iChannel] -> Connect("TextChanged(const Char_t *)", "NFADC400", this, "SetDT(const Char_t *)");
      fDT[iModule][iChannel] -> MoveResize(283, 29, 80, 18);
      fTM[iModule][iChannel] -> AddFrame(fDT[iModule][iChannel]);
      // == End of Deadtime =======================================================

      // == Start of Coincidence width ============================================
      fTextCW = new TGLabel(fTM[iModule][iChannel], "Coincidence width (160 - 40800) (ns)");
      fTextCW -> Move(7, 51);
      fCW[iModule][iChannel] = new TGNumberEntryField(fTM[iModule][iChannel], widgetID, 960, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 160, 40800);
      fCW[iModule][iChannel] -> Connect("TextChanged(const Char_t *)", "NFADC400", this, "SetCW(const Char_t *)");
      fCW[iModule][iChannel] -> MoveResize(283, 51, 80, 18);
      fTM[iModule][iChannel] -> AddFrame(fCW[iModule][iChannel]);
      // == End of Coincidence width ==============================================

      // == Start of Tab and Return pressed on TextEntries ========================
      fThres[iModule][iChannel] -> Connect("TabPressed()", "TGNumberEntryField", fDT[iModule][iChannel], "SetFocus()");
      fThres[iModule][iChannel] -> Connect("ReturnPressed()", "TGNumberEntryField", fDT[iModule][iChannel], "SetFocus()");
      fDT[iModule][iChannel] -> Connect("TabPressed()", "TGNumberEntryField", fCW[iModule][iChannel], "SetFocus()");
      fDT[iModule][iChannel] -> Connect("ReturnPressed()", "TGNumberEntryField", fCW[iModule][iChannel], "SetFocus()");
      fCW[iModule][iChannel] -> Connect("TabPressed()", "TGNumberEntryField", fThres[iModule][iChannel], "SetFocus()");
      fCW[iModule][iChannel] -> Connect("ReturnPressed()", "TGNumberEntryField", fThres[iModule][iChannel], "SetFocus()");
      // == End of Tab and Return pressed on TextEntries ==========================

      // == Start of Width Trigger mode ===========================================
      fTMWidth[iModule][iChannel] = new TGCheckButton(fTM[iModule][iChannel], "Width with threshold (2.5 - 1277.5) (ns)", widgetID);
      fTMWidth[iModule][iChannel] -> Connect("Toggled(Bool_t)", "NFADC400", this, "SetTriggerModeWidth(Bool_t)");
      fTMWidth[iModule][iChannel] -> Move(7, 73);
      fTMWidthThres[iModule][iChannel] = new TGNumberEntryField(fTM[iModule][iChannel], widgetID, 100, TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 2.5, 1277.5);
      fTMWidthThres[iModule][iChannel] -> Connect("TextChanged(const Char_t *)", "NFADC400", this, "SetTMWThres(const Char_t *)");
      fTMWidthThres[iModule][iChannel] -> MoveResize(283, 73, 80, 18);
      fTMWidthThres[iModule][iChannel] -> SetEnabled(kFALSE);
      fTM[iModule][iChannel] -> AddFrame(fTMWidthThres[iModule][iChannel]);
      // == End of Width Trigger mode =============================================

      // == Start of Count Trigger mode ===========================================
      fTMCount[iModule][iChannel] = new TGCheckButton(fTM[iModule][iChannel], "Counts        (1 - 15) in               (40 - 10200) ns.", widgetID);
      fTMCount[iModule][iChannel] -> Connect("Toggled(Bool_t)", "NFADC400", this, "SetTriggerModeCount(Bool_t)");
      fTMCount[iModule][iChannel] -> Move(7, 95);

      fTMCountThres[iModule][iChannel] = new TGNumberEntryField(fTM[iModule][iChannel], widgetID, 1, TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 1, 15);
      fTMCountThres[iModule][iChannel] -> Connect("TextChanged(const Char_t *)", "NFADC400", this, "SetTMCThres(const Char_t *)");
      fTMCountThres[iModule][iChannel] -> MoveResize(70, 95, 25, 18);
      fTMCountThres[iModule][iChannel] -> SetEnabled(kFALSE);
      fTM[iModule][iChannel] -> AddFrame(fTMCountThres[iModule][iChannel]);

      fTMCountInterval[iModule][iChannel] = new TGNumberEntryField(fTM[iModule][iChannel], widgetID, 1000, TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 40, 10200);
      fTMCountInterval[iModule][iChannel] -> Connect("TextChanged(const Char_t *)", "NFADC400", this, "SetTMCInterval(const Char_t *)");
      fTMCountInterval[iModule][iChannel] -> MoveResize(160, 95, 50, 18);
      fTMCountInterval[iModule][iChannel] -> SetEnabled(kFALSE);
      fTM[iModule][iChannel] -> AddFrame(fTMCountInterval[iModule][iChannel]);
      // == End of Count Trigger mode =============================================

      // == Start of Tab and Return pressed on TextEntries ========================
      fTMCountThres[iModule][iChannel] -> Connect("TabPressed()", "TGNumberEntryField", fTMCountInterval[iModule][iChannel], "SetFocus()");
      fTMCountThres[iModule][iChannel] -> Connect("ReturnPressed()", "TGNumberEntryField", fTMCountInterval[iModule][iChannel], "SetFocus()");
      fTMCountInterval[iModule][iChannel] -> Connect("TabPressed()", "TGNumberEntryField", fTMCountThres[iModule][iChannel], "SetFocus()");
      fTMCountInterval[iModule][iChannel] -> Connect("ReturnPressed()", "TGNumberEntryField", fTMCountThres[iModule][iChannel], "SetFocus()");
      // == End of Tab and Return pressed on TextEntries ==========================
    }
    // == End of Trigger Mode Channel Tab =======================================
    // == End of Trigger Mode Channel ===========================================
    // == End of Trigger Output Frame ===========================================
  }
  // == End of Modules ============================================================
  // == End of Module Frame =======================================================

  // == Start of Number of Events =================================================
  fTextNumEvents = new TGLabel(this, "# of events");
  fTextNumEvents -> Move(525, 377);

  fNumEvents = new TGNumberEntryField(this, -1, 1000, TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMax, 0, 1569325055);
  fNumEvents -> Connect("TextChanged(const Char_t *)", "NFADC400", this, "SetNumEvents(const Char_t *)");
  fNumEvents -> MoveResize(525, 393, 80, 18);
  AddFrame(fNumEvents);
  // == End of Number of Events ===================================================

  // == Start of Buttons ==========================================================
  fLoad = new TGTextButton(this, "Load\nSettings");
  fLoad -> Connect("Clicked()", "NFADC400", this, "LoadSettings()");
  fLoad -> MoveResize(5, 370, 80, 50);

  fSave = new TGTextButton(this, "Save\nSettings");
  fSave -> Connect("Clicked()", "NFADC400", this, "SaveSettings()");
  fSave -> MoveResize(85, 370, 80, 50);

  fStart = new TGTextButton(this, "Start");
  fStart -> Connect("Clicked()", "NFADC400", this, "Start()");
  fStart -> MoveResize(614, 370, 100, 50);
  // == End of Buttons ============================================================

  // == Start of Copyright ========================================================
  fCopyright = new TGLabel(this, "Property of Korea University Nuclear Physics Laboratory");
  fCopyright -> Move(187, 405);
  fCopyright = new TGLabel(this, "Designed and developed by Genie Jhang");
  fCopyright -> Move(230, 380);
  // == End of Copyright ==========================================================

  Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
  DontCallClose();

  MapSubwindows();
  MapWindow();
}

NFADC400::~NFADC400()
{
  Cleanup();
}

void NFADC400::Initialize()
{
  // Initialize internal variables
  fSettings.Initialize();
}

void NFADC400::SetSameModuleSetting(Bool_t value)
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

void NFADC400::SetAddress(Int_t value)
{
  TGComboBox *object = (TGComboBox *) gTQSender;
  Int_t module = object -> WidgetId();

  if (fIsDebug) {
    cout << "===========================" << endl;
    cout << " SetAddress module:" << module + 1;
    cout << " is " << value << "!" << endl;
    cout << "===========================" << endl;
  }

  fSettings.fValueAddress[module] = value;
}

void NFADC400::SetActive(Bool_t value)
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

void NFADC400::SetSameChannelSetting(Bool_t value)
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

void NFADC400::SetAC(Bool_t value)
{
  TGCheckButton *object = (TGCheckButton *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channel = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "==================================" << endl;
    cout << " SetAC module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << ( value ? "On!" : "Off!" ) << endl;
    cout << "==================================" << endl;
  }

  fSettings.fValueAC[module][channel] = value;
}

void NFADC400::SetIP(Int_t value)
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

void NFADC400::SetID(const Char_t *value)
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

void NFADC400::SetAO(const Char_t *value)
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

void NFADC400::SetIAG(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channel = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "====================================" << endl;
    cout << " SetIAG module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << atoi(value) << "!" << endl;
    cout << "====================================" << endl;
  }

  fSettings.fValueIAG[module][channel] = atoi(value);
}

void NFADC400::SetRL(Int_t value)
{
  TGComboBox *object = (TGComboBox *) gTQSender;
  Int_t module = object -> WidgetId();

  if (fIsDebug) {
    cout << "=====================================" << endl;
    cout << " SetRL module:" << module + 1;
    cout << " is " << value << " (" << 0.32*value << " us)" << endl;
    cout << "=====================================" << endl;
  }

  fSettings.fValueRL[module] = value;
}

void NFADC400::SetThres(const Char_t *value)
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

void NFADC400::SetDT(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channel = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "========================================" << endl;
    cout << " SetDT module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << atoi(value) << "!" << endl;
    cout << "========================================" << endl;
  }

  fSettings.fValueDT[module][channel] = atoi(value);
}

void NFADC400::SetCW(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channel = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "====================================" << endl;
    cout << " SetCW module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << atoi(value) << "!" << endl;
    cout << "====================================" << endl;
  }

  fSettings.fValueCW[module][channel] = atoi(value);
}

void NFADC400::SetCLT(Int_t value)
{
  TGComboBox *object = (TGComboBox *) gTQSender;
  Int_t module = object -> WidgetId();

  if (fIsDebug) {
    cout << "=========================" << endl;
    cout << " SetCLT module:" << module + 1;
    cout << " is " << ( value ? "OR!" : "AND!" ) << endl;
    cout << "=========================" << endl;
  }

  if (value == 0)
    fTextCLT[module] -> SetText("When trigger signals from all the channels coincide.");
  else
    fTextCLT[module] -> SetText("Whenever a trigger signal exists from single channel.");

  fSettings.fValueCLT[module] = value;
}

void NFADC400::SetTOW(const Char_t * value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t module = object -> WidgetId();

  if (fIsDebug) {
    cout << "==========================" << endl;
    cout << " SetTOW module:" << module + 1;
    cout << " is " << atoi(value) << "!" << endl;
    cout << "==========================" << endl;
  }

  fSettings.fValueTOW[module] = atoi(value);
}

void NFADC400::SetTDC(Int_t value)
{
  TGComboBox *object = (TGComboBox *) gTQSender;
  Int_t module = object -> WidgetId();

  if (fIsDebug) {
    cout << "===================================" << endl;
    cout << " SetTDC module:" << module + 1;
    cout << " is " << ( value ? "Disabled!" : "Enabled!" ) << endl;
    cout << "===================================" << endl;
  }

  fSettings.fValueTDC[module] = value;
}

void NFADC400::SetSameChannelTriggerSetting(Bool_t value)
{
  TGCheckButton *object = (TGCheckButton *) gTQSender;
  Int_t module = object -> WidgetId();

  if (fIsDebug) {
    cout << "================================================" << endl;
    cout << " SetSameChannelTriggerSetting module: " << module + 1;
    cout << " is " << ( value ? "On!" : "Off!" ) << endl;
    cout << "================================================" << endl;
  }

  for (Int_t channel = 1; channel < 4; channel++)
    fTMTab[module] -> SetEnabled(channel, !value);
  fTMTab[module] -> SetTab(0, kFALSE);

  fSettings.fUseSameChannelTriggerSetting[module] = value;
}

void NFADC400::SetTriggerModeWidth(Bool_t value)
{
  TGCheckButton *object = (TGCheckButton *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channel = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "================================================" << endl;
    cout << " SetTriggerModeWidth module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << ( value ? "On!" : "Off!" ) << endl;
    cout << "================================================" << endl;
  }

  fTMWidthThres[module][channel] -> SetEnabled(value);
  fTMWidthThres[module][channel] -> SetFocus();

  fSettings.fValueTMWidth[module][channel] = value;
}

void NFADC400::SetTMWThres(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channel = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "==============================================" << endl;
    cout << " SetTMWThres module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << atof(value) << " ns!" << endl;
    cout << "==============================================" << endl;
  }

  fSettings.fValueTMWidthThres[module][channel] = atof(value);
}

void NFADC400::SetTriggerModeCount(Bool_t value)
{
  TGCheckButton *object = (TGCheckButton *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channel = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "================================================" << endl;
    cout << " SetTriggerModeCount module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << ( value ? "On!" : "Off!" ) << endl;
    cout << "================================================" << endl;
  }

  fTMCountThres[module][channel] -> SetEnabled(value);
  fTMCountInterval[module][channel] -> SetEnabled(value);

  fSettings.fValueTMCount[module][channel] = value;
}

void NFADC400::SetTMCThres(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channel = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "=======================================" << endl;
    cout << " SetTMCThres module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << atoi(value) << "!" << endl;
    cout << "=======================================" << endl;
  }

  fSettings.fValueTMCountThres[module][channel] = atoi(value);
}

void NFADC400::SetTMCInterval(const Char_t *value)
{
  TGNumberEntryField *object = (TGNumberEntryField *) gTQSender;
  Int_t widgetID = object -> WidgetId();

  Int_t module = widgetID/100;
  Int_t channel = (widgetID%100)/10;

  if (fIsDebug) {
    cout << "=============================================" << endl;
    cout << " SetTMCInterval module:" << module + 1;
    cout << " channel:" << channel + 1;
    cout << " is " << atoi(value) << "!" << endl;
    cout << "=============================================" << endl;
  }

  fSettings.fValueTMCountInterval[module][channel] = atoi(value);
}

void NFADC400::SaveSettings()
{
  SetSettingsFromUI();

  // Open dialog for saving action.
  TGFileInfo fileInfo;
  const Char_t *fileType[4] = {"NFADC400CFG files", "*.nfadc400cfg", 0, 0}; 
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
  TString extension = savingFile(savingFile.Length() - 12, 12);
  if (!extension.EqualTo(".nfadc400cfg")) {
    filenameWithPath.Append(".nfadc400cfg");
    savingFile.Append(".nfadc400cfg");
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

void NFADC400::SetNumEvents(const Char_t *value)
{
  if (fIsDebug) {
    cout << "=============================" << endl;
    cout << " SetNumEvents";
    cout << " is " << atoi(value) << "!" << endl;
    cout << "=============================" << endl;
  }

  fSettings.fValueNumEvents = atoi(value);
}

void NFADC400::LoadSettings()
{
  // Open dialog for loading action.
  TGFileInfo fileInfo;
  const Char_t *fileType[4] = {"NFADC400CFG files", "*.nfadc400cfg", 0, 0}; 
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
  TString extension = filenameWithPath(filenameWithPath.Length() - 12, 12);
  if (!extension.EqualTo(".nfadc400cfg")) {
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

void NFADC400::Start()
{
  SetSettingsFromUI();
  new NFADC400Process(fSettings);
}

void NFADC400::SetSettingsFromUI()
{
  // Confirm the values in NumberEntryFields
  for (Int_t iModule = 0; iModule < 2; iModule++) {
    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fID[iModule][iChannel] -> TextChanged();
      fAO[iModule][iChannel] -> TextChanged();
      fIAG[iModule][iChannel] -> TextChanged();
    }

    fTOW[iModule] -> TextChanged();

    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fThres[iModule][iChannel] -> TextChanged();
      fDT[iModule][iChannel] -> TextChanged();
      fCW[iModule][iChannel] -> TextChanged();

      fTMWidthThres[iModule][iChannel] -> TextChanged();

      fTMCountThres[iModule][iChannel] -> TextChanged();
      fTMCountInterval[iModule][iChannel] -> TextChanged();
    }
  }

  fNumEvents -> TextChanged();

  for (Int_t iModule = 0; iModule < 2; iModule++) {
    if (fSettings.fUseSameChannelTriggerSetting[iModule])
      fSettings.UseSameChannelTriggerSetting(iModule);

    if (fSettings.fUseSameChannelSetting[iModule])
      fSettings.UseSameChannelSetting(iModule);
  }

  if (fSettings.fUseSameModuleSetting) 
    fSettings.UseSameModuleSetting();
}

void NFADC400::SetSettingsToUI()
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

    fRL[iModule] -> Select(fSettings.fValueRL[iModule]);

    if (fSettings.fUseSameChannelSetting[iModule])
      fSameChannelSettingButton[iModule] -> SetState(kButtonDown, kTRUE);
    else
      fSameChannelSettingButton[iModule] -> SetState(kButtonUp, kTRUE);

    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      if (fSettings.fValueAC[iModule][iChannel])
        fAC[iModule][iChannel] -> SetState(kButtonDown, kTRUE);
      else
        fAC[iModule][iChannel] -> SetState(kButtonUp, kTRUE);

      fIP[iModule][iChannel] -> Select(fSettings.fValueIP[iModule][iChannel]);
      fID[iModule][iChannel] -> SetText(Form("%d", fSettings.fValueID[iModule][iChannel]), kFALSE);
      fAO[iModule][iChannel] -> SetText(Form("%d", fSettings.fValueAO[iModule][iChannel]), kFALSE);
      fIAG[iModule][iChannel] -> SetText(Form("%d", fSettings.fValueIAG[iModule][iChannel]), kFALSE);
    }

    fCLT[iModule] -> Select(fSettings.fValueCLT[iModule]);
    fTOW[iModule] -> SetText(Form("%d", fSettings.fValueTOW[iModule]), kFALSE);
    fTDC[iModule] -> Select(fSettings.fValueTDC[iModule]);

    if (fSettings.fUseSameChannelTriggerSetting[iModule])
      fSameChannelTriggerSettingButton[iModule] -> SetState(kButtonDown, kTRUE);
    else
      fSameChannelTriggerSettingButton[iModule] -> SetState(kButtonUp, kTRUE);

    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fThres[iModule][iChannel] -> SetText(Form("%d", fSettings.fValueThres[iModule][iChannel]), kFALSE);
      fDT[iModule][iChannel] -> SetText(Form("%d", fSettings.fValueDT[iModule][iChannel]), kFALSE);
      fCW[iModule][iChannel] -> SetText(Form("%d", fSettings.fValueCW[iModule][iChannel]), kFALSE);

      if (fSettings.fValueTMWidth[iModule][iChannel]) {
        fTMWidth[iModule][iChannel] -> SetState(kButtonDown, kTRUE);
        fTMWidthThres[iModule][iChannel] -> SetText(Form("%.1f", fSettings.fValueTMWidthThres[iModule][iChannel]));
      } else
        fTMWidth[iModule][iChannel] -> SetState(kButtonUp, kTRUE);

      if (fSettings.fValueTMCount[iModule][iChannel]) {
        fTMCount[iModule][iChannel] -> SetState(kButtonDown, kTRUE);

        fTMCountThres[iModule][iChannel] -> SetText(Form("%d", fSettings.fValueTMCountThres[iModule][iChannel]));
        fTMCountInterval[iModule][iChannel] -> SetText(Form("%d", fSettings.fValueTMCountInterval[iModule][iChannel]));
      } else
        fTMCount[iModule][iChannel] -> SetState(kButtonUp, kTRUE);
    }
  }

  fNumEvents -> SetText(Form("%d", fSettings.fValueNumEvents), kFALSE);
}

Int_t main(int argc, char **argv)
{
//  gEnv->SetValue("Gui.DefaultFont", "-adobe-segoe UI-medium-r-*-*-12-*-*-*-*-*-iso8859-1");
	TApplication theApp("NFADC400GUI", &argc, argv);
	new NFADC400(gClient -> GetRoot(), 720, 425);
	theApp.Run();

	return 0;
}
