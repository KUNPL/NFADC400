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
  Int_t moduleFrameHeight = 800;
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
    Int_t channelFrameWidth = moduleTabWidth - 2.5*moduleTabHMargin;
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

  fUseSameChannelSetting[module] = value;
}

void FADC400::SetDSM(Int_t value)
{
  Int_t module = value/100;
  Int_t channel = (value%100)/10;
  Int_t mode = value%10;

  if (fIsDebug) {
    cout << "======================================" << endl;
    cout << " SetDSM module:" << module;
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
    cout << " SetIP module:" << module;
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
    cout << " SetID module:" << module;
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
    cout << " SetAO module:" << module;
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
    cout << " SetThres module:" << module;
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
    cout << " SetRL module:" << module;
    cout << " channel:" << channel + 1;
    cout << " is " << value << " (" << 2.56*value << " us)" << endl;
    cout << "==============================================" << endl;
  }

  fValueRL[module][channel] = value;
}

Int_t main(int argc, char **argv)
{
	TApplication theApp("FADC400GUI", &argc, argv);
	new FADC400(gClient -> GetRoot(), 410, 1000);
	theApp.Run();

	return 0;
}
