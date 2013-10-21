// Mainframe macro generated from application: /Applications/ROOT-v5.34.10/bin/root.exe
// By ROOT version 5.34/10 on 2013-10-21 12:28:22

#ifndef ROOT_TGDockableFrame
#include "TGDockableFrame.h"
#endif
#ifndef ROOT_TGMenu
#include "TGMenu.h"
#endif
#ifndef ROOT_TGMdiDecorFrame
#include "TGMdiDecorFrame.h"
#endif
#ifndef ROOT_TG3DLine
#include "TG3DLine.h"
#endif
#ifndef ROOT_TGMdiFrame
#include "TGMdiFrame.h"
#endif
#ifndef ROOT_TGMdiMainFrame
#include "TGMdiMainFrame.h"
#endif
#ifndef ROOT_TGMdiMenu
#include "TGMdiMenu.h"
#endif
#ifndef ROOT_TGListBox
#include "TGListBox.h"
#endif
#ifndef ROOT_TGNumberEntry
#include "TGNumberEntry.h"
#endif
#ifndef ROOT_TGScrollBar
#include "TGScrollBar.h"
#endif
#ifndef ROOT_TGComboBox
#include "TGComboBox.h"
#endif
#ifndef ROOT_TGuiBldHintsEditor
#include "TGuiBldHintsEditor.h"
#endif
#ifndef ROOT_TGuiBldNameFrame
#include "TGuiBldNameFrame.h"
#endif
#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGFileDialog
#include "TGFileDialog.h"
#endif
#ifndef ROOT_TGShutter
#include "TGShutter.h"
#endif
#ifndef ROOT_TGButtonGroup
#include "TGButtonGroup.h"
#endif
#ifndef ROOT_TGCanvas
#include "TGCanvas.h"
#endif
#ifndef ROOT_TGFSContainer
#include "TGFSContainer.h"
#endif
#ifndef ROOT_TGuiBldEditor
#include "TGuiBldEditor.h"
#endif
#ifndef ROOT_TGColorSelect
#include "TGColorSelect.h"
#endif
#ifndef ROOT_TGButton
#include "TGButton.h"
#endif
#ifndef ROOT_TGFSComboBox
#include "TGFSComboBox.h"
#endif
#ifndef ROOT_TGLabel
#include "TGLabel.h"
#endif
#ifndef ROOT_TRootGuiBuilder
#include "TRootGuiBuilder.h"
#endif
#ifndef ROOT_TGTab
#include "TGTab.h"
#endif
#ifndef ROOT_TGListView
#include "TGListView.h"
#endif
#ifndef ROOT_TGStatusBar
#include "TGStatusBar.h"
#endif
#ifndef ROOT_TGListTree
#include "TGListTree.h"
#endif
#ifndef ROOT_TGuiBldGeometryFrame
#include "TGuiBldGeometryFrame.h"
#endif
#ifndef ROOT_TGToolBar
#include "TGToolBar.h"
#endif
#ifndef ROOT_TGuiBldDragManager
#include "TGuiBldDragManager.h"
#endif
#ifndef ROOT_TGObject
#include "TGObject.h"
#endif

#include "Riostream.h"

void test()
{

   // main frame
   TGMainFrame *fMainFrame756 = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
   fMainFrame756->SetName("fMainFrame756");
   fMainFrame756->SetLayoutBroken(kTRUE);

   // horizontal frame
   TGHorizontalFrame *fHorizontalFrame560 = new TGHorizontalFrame(fMainFrame756,136,136,kHorizontalFrame);
   fHorizontalFrame560->SetName("fHorizontalFrame560");
   fHorizontalFrame560->SetLayoutBroken(kTRUE);

   fMainFrame756->AddFrame(fHorizontalFrame560, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fHorizontalFrame560->MoveResize(8,8,136,136);

   // vertical frame
   TGVerticalFrame *fVerticalFrame571 = new TGVerticalFrame(fMainFrame756,192,144,kVerticalFrame);
   fVerticalFrame571->SetName("fVerticalFrame571");
   fVerticalFrame571->SetLayoutBroken(kTRUE);

   fMainFrame756->AddFrame(fVerticalFrame571, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fVerticalFrame571->MoveResize(176,16,192,144);

   // tab widget
   TGTab *fTab580 = new TGTab(fMainFrame756,240,104);

   // container of "Tab1"
   TGCompositeFrame *fCompositeFrame583;
   fCompositeFrame583 = fTab580->AddTab("Tab1");
   fCompositeFrame583->SetLayoutManager(new TGVerticalLayout(fCompositeFrame583));
   TGTextButton *fTextButton622 = new TGTextButton(fCompositeFrame583,"fTextButton622");
   fTextButton622->SetTextJustify(36);
   fTextButton622->SetMargins(0,0,0,0);
   fTextButton622->SetWrapLength(-1);
   fTextButton622->Resize(90,22);
   fCompositeFrame583->AddFrame(fTextButton622, new TGLayoutHints(kLHintsNormal));
   TGRadioButton *fTextButton639 = new TGRadioButton(fCompositeFrame583,"fTextButton639");
   fTextButton639->SetTextJustify(36);
   fTextButton639->SetMargins(0,0,0,0);
   fTextButton639->SetWrapLength(-1);
   fCompositeFrame583->AddFrame(fTextButton639, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


   // container of "Tab2"
   TGCompositeFrame *fCompositeFrame585;
   fCompositeFrame585 = fTab580->AddTab("Tab2");
   fCompositeFrame585->SetLayoutManager(new TGVerticalLayout(fCompositeFrame585));


   fTab580->SetTab(0);

   fTab580->Resize(fTab580->GetDefaultSize());
   fMainFrame756->AddFrame(fTab580, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTab580->MoveResize(64,240,240,104);

   // "fGroupFrame594" group frame
   TGGroupFrame *fGroupFrame594 = new TGGroupFrame(fMainFrame756,"fGroupFrame594");
   fGroupFrame594->SetLayoutBroken(kTRUE);
   TGTextButton *fTextButton603 = new TGTextButton(fGroupFrame594,"fTextButton603");
   fTextButton603->SetTextJustify(36);
   fTextButton603->SetMargins(0,0,0,0);
   fTextButton603->SetWrapLength(-1);
   fTextButton603->Resize(92,40);
   fGroupFrame594->AddFrame(fTextButton603, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButton603->MoveResize(8,32,92,40);

   fGroupFrame594->SetLayoutManager(new TGVerticalLayout(fGroupFrame594));
   fGroupFrame594->Resize(119,96);
   fMainFrame756->AddFrame(fGroupFrame594, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fGroupFrame594->MoveResize(352,216,119,96);

   fMainFrame756->SetMWMHints(kMWMDecorAll,
                        kMWMFuncAll,
                        kMWMInputModeless);
   fMainFrame756->MapSubwindows();

   fMainFrame756->Resize(fMainFrame756->GetDefaultSize());
   fMainFrame756->MapWindow();
   fMainFrame756->Resize(490,372);
}  
