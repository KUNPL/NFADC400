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

  fSameModuleSettingButton = new TGCheckButton(fModuleFrame, "Use the same setting for all modules");
  fSameModuleSettingButton -> Connect("Toggled(Bool_t)", "FADC400", this, "SetSameModuleSetting(Bool_t)");
  fSameModuleSettingButton -> Move(10, 20);

  Int_t moduleFrameHMargin = 5;
  Int_t moduleFrameVMargin = 5;
  Int_t moduleFrameWidth = width - 2*moduleFrameHMargin;
  Int_t moduleFrameHeight = height - 2*moduleFrameVMargin;
  fModuleFrame -> MoveResize(moduleFrameHMargin, moduleFrameVMargin, moduleFrameWidth, moduleFrameHeight);

  fModuleTab = new TGTab(fModuleFrame);
  fFADC1 = fModuleTab -> AddTab("FADC400 Module 1");
  fFADC2 = fModuleTab -> AddTab("FADC400 Module 2");

  fModuleFrame -> AddFrame(fModuleTab, new TGLayoutHints(kLHintsLeft|kLHintsTop));
  Int_t moduleTabHMargin = 10;
  Int_t moduleTabVMargin = 10;
  Int_t moduleTabWidth = moduleFrameWidth - 2*moduleTabHMargin;
  Int_t moduleTabHeight = moduleFrameHeight - 9*moduleTabVMargin;
  fModuleTab -> MoveResize(10, 45, moduleTabWidth, moduleTabHeight);
  AddFrame(fModuleFrame, new TGLayoutHints(kLHintsLeft|kLHintsTop));

  // == End of Module Frame =======================================================

  MapSubwindows();
  MapWindow();
}

FADC400::~FADC400()
{
  Cleanup();
}

void FADC400::SetSameModuleSetting(Bool_t isSet)
{
  if (fIsDebug) {
    cout << "==============================" << endl;
    cout << " SetSameModuleSetting is " << ( isSet ? "On!" : "Off!" ) << endl;
    cout << "==============================" << endl;
  }

  // Some code will be here
}

Int_t main(int argc, char **argv)
{
	TApplication theApp("FADC400GUI", &argc, argv);
	new FADC400(gClient -> GetRoot(), 800, 1000);
	theApp.Run();

	return 0;
}
