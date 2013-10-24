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
  fFADC[0] = fModuleTab -> AddTab("FADC400 Module 1");
  fFADC[0] -> SetLayoutBroken(kTRUE);
  fFADC[1] = fModuleTab -> AddTab("FADC400 Module 2");
  fFADC[1] -> SetLayoutBroken(kTRUE);

  fModuleFrame -> AddFrame(fModuleTab, new TGLayoutHints(kLHintsLeft|kLHintsTop));
  Int_t moduleTabHMargin = 10;
  Int_t moduleTabWidth = moduleFrameWidth - 2*moduleTabHMargin;
  Int_t moduleTabHeight = moduleFrameHeight - 90;
  fModuleTab -> MoveResize(moduleTabHMargin, 45, moduleTabWidth, moduleTabHeight);
  AddFrame(fModuleFrame, new TGLayoutHints(kLHintsLeft|kLHintsTop));

  // ==== Start of Modules  =======================================================
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

    // ====== Start of Channel Setting Frame ========================================
    fChannelFrame[iModule] = new TGGroupFrame(fFADC[iModule], "Channel Setting");
    fChannelFrame[iModule] -> SetLayoutBroken(kTRUE);

    Int_t channelFrameHMargin = 10;
    Int_t channelFrameVMargin = 10;
    Int_t channelFrameWidth = moduleTabWidth - 2.5*moduleTabHMargin;
    Int_t channelFrameHeight = 400;
    fChannelFrame[iModule] -> MoveResize(channelFrameHMargin, 45, channelFrameWidth, channelFrameHeight);
    fFADC[iModule] -> AddFrame(fChannelFrame[iModule]);

    // ======== Start of Channel Tab ==================================================
    fSameChannelSettingButton[iModule] = new TGCheckButton(fChannelFrame[iModule], "Use the same settings for all channel.", iModule);
    fSameChannelSettingButton[iModule] -> Connect("Toggled(Bool_t)", "FADC400", this, "SetSameChannelSetting(Bool_t)");
    fSameChannelSettingButton[iModule] -> Move(10, 20);

    fChannelTab[iModule] = new TGTab(fChannelFrame[iModule]);
    for (Int_t iChannel = 0; iChannel < 4; iChannel++) {
      fCh[iModule][iChannel] = fChannelTab[iModule] -> AddTab(Form("Channel %d", iChannel));
      fCh[iModule][iChannel] -> SetLayoutBroken(kTRUE);
    }

    fChannelFrame[iModule] -> AddFrame(fChannelTab[iModule], new TGLayoutHints(kLHintsLeft|kLHintsTop));
    Int_t channelTabHMargin = 10;
    Int_t channelTabWidth = channelFrameWidth - 2*channelTabHMargin;
    Int_t channelTabHeight = 300;
    fChannelTab[iModule] -> MoveResize(channelTabHMargin, 45, channelTabWidth, channelTabHeight);
    fFADC[iModule] -> AddFrame(fChannelFrame[iModule], new TGLayoutHints(kLHintsLeft|kLHintsTop));

    for (Int_t iChannel = 0; iChannel < 4; iChannel++ ) {
      fTextDSM = new TGLabel(fCh[iModule][iChannel], "Data saving mode");
      fTextDSM -> Move(10, 10);
      fTextIP = new TGLabel(fCh[iModule][iChannel], "Input polarity");
      fTextIP -> Move(10, 30);
      fTextID = new TGLabel(fCh[iModule][iChannel], "Input delay (0 ~ 40900) (ns)");
      fTextID -> Move(10, 50);
      fTextAO = new TGLabel(fCh[iModule][iChannel], "ADC Offset (0 ~ 4095) (ns)");
      fTextAO -> Move(10, 70);
      fTextThres = new TGLabel(fCh[iModule][iChannel], "Threshold (0 ~ 1023)");
      fTextThres -> Move(10, 90);
      fTextRL = new TGLabel(fCh[iModule][iChannel], "Recording length (us)");
      fTextRL -> Move(10, 110);
    }
    // ======== End of Channel Tab ====================================================
    // ====== End of Channel Setting Frame ==========================================
  }
  // ==== End of Modules  =========================================================
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

void FADC400::SetAddress(Int_t value)
{
  TGComboBox *object = (TGComboBox *) gTQSender;
  Int_t module = object -> WidgetId();

  if (fIsDebug) {
    cout << "==============================" << endl;
    cout << " SetAddress" << module << " is " << value << "!" << endl;
    cout << "==============================" << endl;
  }

  fValueAddress[module] = value;
}

void FADC400::SetActive(Bool_t value)
{
  TGCheckButton *object = (TGCheckButton *) gTQSender;
  Int_t module = object -> WidgetId();

  if (fIsDebug) {
    cout << "============================" << endl;
    cout << " SetActive" << module << " is " << ( value ? "Activated!" : "Deactivated!" ) << endl;
    cout << "============================" << endl;
  }

  fIsActive[module] = value;
}

void FADC400::SetSameChannelSetting(Bool_t value)
{
  TGCheckButton *object = (TGCheckButton *) gTQSender;
  Int_t module = object -> WidgetId();

  if (fIsDebug) {
    cout << "================================" << endl;
    cout << " SetSameChannelSetting" << module << " is " << ( value ? "On!" : "Off!" ) << endl;
    cout << "================================" << endl;
  }

  fUseSameChannelSetting[module] = value;
}

Int_t main(int argc, char **argv)
{
	TApplication theApp("FADC400GUI", &argc, argv);
	new FADC400(gClient -> GetRoot(), 800, 1000);
	theApp.Run();

	return 0;
}
