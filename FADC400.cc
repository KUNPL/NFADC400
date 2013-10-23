/////////////////////////////////////
//                                 //
//        FADC400 module GUI       //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 10. 21           //
//                                 //
/////////////////////////////////////

#include "TApplication.h"
#include "Riostream.h"
#include "TSystem.h"
#include "TGClient.h"

#include "FADC400.hh"

ClassImp(FADC400);

FADC400::FADC400(const TGWindow *window, UInt_t width, UInt_t height)
:TGMainFrame(window, width, height, kMainFrame)
{
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
  Int_t moduleFrameHeight = height - 2*moduleFrameVMargin;
  fModuleFrame -> MoveResize(moduleFrameHMargin, moduleFrameVMargin, moduleFrameWidth, moduleFrameHeight);

  fSameModuleSettingButton = new TGCheckButton(fModuleFrame, "Use the same setting for all modules.");
  fSameModuleSettingButton -> Connect("Toggled(Bool_t)", "FADC400", this, "SetSameModuleSetting(Bool_t)");
  fSameModuleSettingButton -> Move(10, 20);

  fModuleTab = new TGTab(fModuleFrame);
  fFADC1 = fModuleTab -> AddTab("FADC400 Module 1");
  fFADC1 -> SetLayoutBroken(kTRUE);
  fFADC2 = fModuleTab -> AddTab("FADC400 Module 2");
  fFADC2 -> SetLayoutBroken(kTRUE);

  fModuleFrame -> AddFrame(fModuleTab, new TGLayoutHints(kLHintsLeft|kLHintsTop));
  Int_t moduleTabHMargin = 10;
  Int_t moduleTabWidth = moduleFrameWidth - 2*moduleTabHMargin;
  Int_t moduleTabHeight = moduleFrameHeight - 90;
  fModuleTab -> MoveResize(moduleTabHMargin, 45, moduleTabWidth, moduleTabHeight);
  AddFrame(fModuleFrame, new TGLayoutHints(kLHintsLeft|kLHintsTop));

  // ==== Start of Module 1 =======================================================
  fTextAddress = new TGLabel(fFADC1, "Address:");
  fTextAddress -> Move(10, 10);

  fCBAddress1 = new TGComboBox(fFADC1);
  for (Int_t i = 0; i < 6; i++)
    fCBAddress1 -> AddEntry(Form("%d", i), i);
  fCBAddress1 -> Select(3);
  fCBAddress1 -> Connect("Selected(Int_t)", "FADC400", this, "SetAddress1(Int_t)");
  fCBAddress1 -> MoveResize(70, 8, 50, 20);
  fFADC1 -> AddFrame(fCBAddress1);

  fActive1 = new TGCheckButton(fFADC1, "Active");
  fActive1 -> Connect("Toggled(Bool_t)", "FADC400", this, "SetActive1(Bool_t)");
  fActive1 -> Move(140, 10);

  // ====== Start of Channel Setting Frame ========================================
  fChannelFrame1 = new TGGroupFrame(fFADC1, "Channel Setting");
  fChannelFrame1 -> SetLayoutBroken(kTRUE);

  Int_t channelFrameHMargin = 10;
  Int_t channelFrameVMargin = 10;
  Int_t channelFrameWidth = moduleTabWidth - 2.5*moduleTabHMargin;
  Int_t channelFrameHeight = 400;
  fChannelFrame1 -> MoveResize(channelFrameHMargin, 45, channelFrameWidth, channelFrameHeight);
  fFADC1 -> AddFrame(fChannelFrame1);

  // ======== Start of Channel Tab ==================================================
  fSameChannelSettingButton1 = new TGCheckButton(fChannelFrame1, "Use the same settings for all channel.");
  fSameChannelSettingButton1 -> Connect("Toggled(Bool_t)", "FADC400", this, "SetSameChannelSetting1(Bool_t)");
  fSameChannelSettingButton1 -> Move(10, 20);

  fChannelTab1 = new TGTab(fChannelFrame1);
  fCh11 = fChannelTab1 -> AddTab("Channel 1");
  fCh11 -> SetLayoutBroken(kTRUE);
  fCh12 = fChannelTab1 -> AddTab("Channel 2");
  fCh12 -> SetLayoutBroken(kTRUE);
  fCh13 = fChannelTab1 -> AddTab("Channel 3");
  fCh13 -> SetLayoutBroken(kTRUE);
  fCh14 = fChannelTab1 -> AddTab("Channel 4");
  fCh14 -> SetLayoutBroken(kTRUE);

  fChannelFrame1 -> AddFrame(fChannelTab1, new TGLayoutHints(kLHintsLeft|kLHintsTop));
  Int_t channelTabHMargin = 10;
  Int_t channelTabWidth = channelFrameWidth - 2*channelTabHMargin;
  Int_t channelTabHeight = 300;
  fChannelTab1 -> MoveResize(channelTabHMargin, 45, channelTabWidth, channelTabHeight);
  fFADC1 -> AddFrame(fChannelFrame1, new TGLayoutHints(kLHintsLeft|kLHintsTop));

  fTextDSM = new TGLabel(fCh11, "Data saving mode");
  fTextDSM -> Move(10, 10);
  fTextIP = new TGLabel(fCh11, "Input polarity");
  fTextIP -> Move(10, 30);
  fTextID = new TGLabel(fCh11, "Input delay (0 ~ 40900) (ns)");
  fTextID -> Move(10, 50);
  fTextAO = new TGLabel(fCh11, "ADC Offset (0 ~ 4095) (ns)");
  fTextAO -> Move(10, 70);
  fTextThres = new TGLabel(fCh11, "Threshold (0 ~ 1023)");
  fTextThres -> Move(10, 90);
  fTextRL = new TGLabel(fCh11, "Recording length (us)");
  fTextRL -> Move(10, 110);
  // ======== End of Channel Tab ====================================================
  // ====== End of Channel Setting Frame ==========================================
  // ==== End of Module 1 =========================================================

  // ==== Start of Module 2 =======================================================
  fTextAddress = new TGLabel(fFADC2, "Address:");
  fTextAddress -> Move(10, 10);
  fFADC2 -> AddFrame(fTextAddress);

  fCBAddress2 = new TGComboBox(fFADC2);
  for (Int_t i = 0; i < 6; i++)
    fCBAddress2 -> AddEntry(Form("%d", i), i);
  fCBAddress2 -> Select(4);
  fCBAddress2 -> Connect("Selected(Int_t)", "FADC400", this, "SetAddress2(Int_t)");
  fCBAddress2 -> MoveResize(70, 8, 50, 20);
  fFADC2 -> AddFrame(fCBAddress2);

  fActive2 = new TGCheckButton(fFADC2, "Active");
  fActive2 -> Connect("Toggled(Bool_t)", "FADC400", this, "SetActive2(Bool_t)");
  fActive2 -> Move(140, 10);

  // ====== Start of Channel Setting Frame ========================================
  fChannelFrame2 = new TGGroupFrame(fFADC2, "Channel Setting");
  fChannelFrame2 -> SetLayoutBroken(kTRUE);

  fChannelFrame2 -> MoveResize(channelFrameHMargin, 45, channelFrameWidth, channelFrameHeight);
  fFADC2 -> AddFrame(fChannelFrame2);

  // ======== Start of Channel Tab ==================================================
  fSameChannelSettingButton2 = new TGCheckButton(fChannelFrame2, "Use the same settings for all channel.");
  fSameChannelSettingButton2 -> Connect("Toggled(Bool_t)", "FADC400", this, "SetSameChannelSetting2(Bool_t)");
  fSameChannelSettingButton2 -> Move(10, 20);

  fChannelTab2 = new TGTab(fChannelFrame2);
  fCh21 = fChannelTab2 -> AddTab("Channel 1");
  fCh21 -> SetLayoutBroken(kTRUE);
  fCh22 = fChannelTab2 -> AddTab("Channel 2");
  fCh22 -> SetLayoutBroken(kTRUE);
  fCh23 = fChannelTab2 -> AddTab("Channel 3");
  fCh23 -> SetLayoutBroken(kTRUE);
  fCh24 = fChannelTab2 -> AddTab("Channel 4");
  fCh24 -> SetLayoutBroken(kTRUE);

  fChannelFrame2 -> AddFrame(fChannelTab2, new TGLayoutHints(kLHintsLeft|kLHintsTop));
  fChannelTab2 -> MoveResize(channelTabHMargin, 45, channelTabWidth, channelTabHeight);
  fFADC2 -> AddFrame(fChannelFrame2, new TGLayoutHints(kLHintsLeft|kLHintsTop));
  // ======== End of Channel Tab ====================================================
  // ====== End of Channel Setting Frame ==========================================
  // ==== End of Module 1 =========================================================
  // ==== End of Module 2 =========================================================

  // == End of Module Frame =======================================================

  MapSubwindows();
  MapWindow();
}

FADC400::~FADC400()
{
  Cleanup();
}

void FADC400::SetSameModuleSetting(Bool_t value)
{
  if (fIsDebug) {
    cout << "==============================" << endl;
    cout << " SetSameModuleSetting is " << ( value ? "On!" : "Off!" ) << endl;
    cout << "==============================" << endl;
  }

  fUseSameModuleSetting = value;
}

void FADC400::SetAddress1(Int_t value)
{
  if (fIsDebug) {
    cout << "==============================" << endl;
    cout << " SetAddress1 is " << value << "!" << endl;
    cout << "==============================" << endl;
  }

  fValueAddress1 = value;
}

void FADC400::SetActive1(Bool_t value)
{
  if (fIsDebug) {
    cout << "============================" << endl;
    cout << " SetActive1 is " << ( value ? "Activated!" : "Deactivated!" ) << endl;
    cout << "============================" << endl;
  }

  fIsActive1 = value;
}

void FADC400::SetSameChannelSetting1(Bool_t value)
{
  if (fIsDebug) {
    cout << "================================" << endl;
    cout << " SetSameChannelSetting1 is " << ( value ? "On!" : "Off!" ) << endl;
    cout << "================================" << endl;
  }

  fUseSameChannelSetting1 = value;
}

void FADC400::SetAddress2(Int_t value)
{
  if (fIsDebug) {
    cout << "======================" << endl;
    cout << " SetAddress2 is " << value << "!" << endl;
    cout << "======================" << endl;
  }

  fValueAddress2 = value;
}

void FADC400::SetActive2(Bool_t value)
{
  if (fIsDebug) {
    cout << "============================" << endl;
    cout << " SetActive2 is " << ( value ? "Activated!" : "Deactivated!" ) << endl;
    cout << "============================" << endl;
  }

  fIsActive2 = value;
}

void FADC400::SetSameChannelSetting2(Bool_t value)
{
  if (fIsDebug) {
    cout << "================================" << endl;
    cout << " SetSameChannelSetting2 is " << ( value ? "On!" : "Off!" ) << endl;
    cout << "================================" << endl;
  }

  fUseSameChannelSetting2 = value;
}

Int_t main(int argc, char **argv)
{
	TApplication theApp("FADC400GUI", &argc, argv);
	new FADC400(gClient -> GetRoot(), 800, 1000);
	theApp.Run();

	return 0;
}
