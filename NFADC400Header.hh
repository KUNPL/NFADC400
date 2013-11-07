/////////////////////////////////////
//                                 //
//      NFADC400 module GUI        //
//   Header Data Container Class   //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 11. 01           //
//                                 //
/////////////////////////////////////

#ifndef _NFADC400HEADER_H_
#define _NFADC400HEADER_H_

#include "TNamed.h"

class NFADC400Header : public TNamed
{
  public:
    NFADC400Header();
    ~NFADC400Header();

        void SetRL(Int_t value) { fRL = value; }
       Int_t GetRL()            { return fRL; }

        void SetAC(Int_t channel, Bool_t value) { fAC[channel] = value; }
        void SetIP(Int_t channel, Bool_t value) { fIP[channel] = value; }
        void SetID(Int_t channel, Int_t value)  { fID[channel] = value; }
        void SetAO(Int_t channel, Int_t value)  { fAO[channel] = value; }
        void SetIAG(Int_t channel, Int_t value) { fIAG[channel] = value; }

      Bool_t GetAC(Int_t channel)               { return fAC[channel]; }
      Bool_t GetIP(Int_t channel)               { return fIP[channel]; }
       Int_t GetID(Int_t channel)               { return fID[channel]; }
       Int_t GetAO(Int_t channel)               { return fAO[channel]; }
       Int_t GetIAG(Int_t channel)              { return fIAG[channel]; }

        void SetCLT(Bool_t value)   { fCLT = value; }
        void SetTOW(Int_t value)    { fTOW = value; }
        void SetTDC(Bool_t value)   { fTDC = value; }

      Bool_t GetCLT()               { return fCLT; }
       Int_t SetTOW()               { return fTOW; }
      Bool_t SetTDC()               { return fTDC; }

        void SetThreshold(Int_t channel, Int_t value)           { fThreshold[channel] = value; }
        void SetDT(Int_t channel, Int_t value)                  { fDT[channel] = value; }
        void SetCW(Int_t channel, Int_t value)                  { fCW[channel] = value; }
        void SetTMWidth(Int_t channel, Bool_t value)            { fTMWidth[channel] = value; }
        void SetTMWidthThreshold(Int_t channel, Double_t value) { fTMWidthThreshold[channel] = value; }
        void SetTMCount(Int_t channel, Bool_t value)            { fTMCount[channel] = value; }
        void SetTMCountThreshold(Int_t channel, Int_t value)    { fTMCountThreshold[channel] = value; }
        void SetTMCountInterval(Int_t channel, Int_t value)     { fTMCountInterval[channel] = value; }

       Int_t GetThreshold(Int_t channel)        { return fThreshold[channel]; }
       Int_t GetDT(Int_t channel)               { return fDT[channel]; }
       Int_t GetCW(Int_t channel)               { return fCW[channel]; }
      Bool_t GetTMWidth(Int_t channel)          { return fTMWidth[channel]; }
    Double_t GetTMWidthThreshold(Int_t channel) { return fTMWidthThreshold[channel]; }
      Bool_t GetTMCount(Int_t channel)          { return fTMCount[channel]; }
       Int_t GetTMCountThreshold(Int_t channel) { return fTMCountThreshold[channel]; }
       Int_t GetTMCountInterval(Int_t channel)  { return fTMCountInterval[channel]; }

        void SetNumEvents(Int_t value) { fNumEvents = value; }
       Int_t GetNumEvents()            { return fNumEvents; }

        void Print(Int_t channel = 0);

  private:
    void Initialize();

    Int_t fNumEvents;

    Int_t fRL;

    // Channels' Header
    Bool_t fAC[4];
    Bool_t fIP[4];
    Int_t fID[4];
    Int_t fAO[4];
    Int_t fIAG[4];

    // Triggers' Header
    Bool_t fCLT;
    Int_t fTOW;
    Bool_t fTDC;
    Int_t fThreshold[4];
    Int_t fDT[4];
    Int_t fCW[4];
    Bool_t fTMWidth[4];
    Double_t fTMWidthThreshold[4];
    Bool_t fTMCount[4];
    Int_t fTMCountThreshold[4];
    Int_t fTMCountInterval[4];
  
  ClassDef(NFADC400Header, 1);
};

#endif
