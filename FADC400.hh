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

#include "TGWindow.h"
#include "TGFrame.h"
#include "TGTab.h"
#include "TGButton.h"

class FADC400 : public TGMainFrame
{
  public:
    FADC400(const TGWindow *, UInt_t, UInt_t);
    virtual ~FADC400();

    void SetSameModuleSetting(Bool_t);

  private:
    Bool_t fIsDebug;

    TGGroupFrame *fModuleFrame;
    TGButton *fSameModuleSettingButton;
    TGTab *fModuleTab;
    TGCompositeFrame *fFADC1;
    TGCompositeFrame *fFADC2;


  ClassDef(FADC400, 1);
};

#endif
