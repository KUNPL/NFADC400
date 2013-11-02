/////////////////////////////////////
//                                 //
//       FADC400 module GUI        //
//   Header Data Container Class   //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 11. 01           //
//                                 //
/////////////////////////////////////

#ifndef _FADC400HEADER_H_
#define _FADC400HEADER_H_

#include "TNamed.h"

class FADC400Header : public TNamed
{
  public:
    FADC400Header();
    ~FADC400Header();

        void SetAC(Int_t channel, Bool_t value)       { fAC[channel] = value; }
        void SetDSM(Int_t channel, Bool_t value)      { fDSM[channel] = value; }
        void SetIP(Int_t channel, Bool_t value)       { fIP[channel] = value; }
        void SetID(Int_t channel, Int_t value)        { fID[channel] = value; }
        void SetAO(Int_t channel, Int_t value)        { fAO[channel] = value; }
        void SetThreshold(Int_t channel, Int_t value) { fThreshold[channel] = value; }
        void SetRL(Int_t channel, Int_t value)        { fThreshold[channel] = value; }

      Bool_t GetAC(Int_t channel)        { return fAC[channel]; }
      Bool_t GetDSM(Int_t channel)       { return fDSM[channel]; }
      Bool_t GetIP(Int_t channel)        { return fIP[channel]; }
       Int_t GetID(Int_t channel)        { return fID[channel]; }
       Int_t GetAO(Int_t channel)        { return fAO[channel]; }
       Int_t GetThreshold(Int_t channel) { return fThreshold[channel]; }
       Int_t GetRL(Int_t channel)        { return fRL[channel]; }

        void SetCLT(Bool_t value)   { fCLT = value; }
      Bool_t GetCLT()               { return fCLT; }

        void SetDT(Int_t cgroup, Int_t value)                             { fDT[cgroup] = value; }
        void SetCW(Int_t cgroup, Int_t value)                             { fCW[cgroup] = value; }
        void SetTMWidth(Int_t cgroup, Bool_t value)                       { fTMWidth[cgroup] = value; }
        void SetTMWidthOption(Int_t cgroup, Int_t value)                  { fTMWidthOption[cgroup] = value; }
        void SetTMWidthThreshold(Int_t cgroup, Double_t value)            { fTMWidthThreshold[cgroup] = value; }
        void SetTMCount(Int_t cgroup, Bool_t value)                       { fTMCount[cgroup] = value; }
        void SetTMCountOption(Int_t cgroup, Int_t value)                  { fTMCountOption[cgroup] = value; }
        void SetTMCountThreshold(Int_t cgroup, Int_t option, Int_t value) { fTMCountThreshold[cgroup][option] = value; }
        void SetTMCountInterval(Int_t cgroup, Int_t option, Int_t value)  { fTMCountInterval[cgroup][option] = value; }

       Int_t GetDT(Int_t cgroup)                             { return fDT[cgroup]; }
       Int_t GetCW(Int_t cgroup)                             { return fCW[cgroup]; }
      Bool_t GetTMWidth(Int_t cgroup)                        { return fTMWidth[cgroup]; }
       Int_t GetTMWidthOption(Int_t cgroup)                  { return fTMWidthOption[cgroup]; }
    Double_t GetTMWidthThreshold(Int_t cgroup)               { return fTMWidthThreshold[cgroup]; }
      Bool_t GetTMCount(Int_t cgroup)                        { return fTMCount[cgroup]; }
       Int_t GetTMCountOption(Int_t cgroup)                  { return fTMCountOption[cgroup]; }
       Int_t GetTMCountThreshold(Int_t cgroup, Int_t option) { return fTMCountThreshold[cgroup][option]; }
       Int_t GetTMCountInterval(Int_t cgroup, Int_t option)  { return fTMCountInterval[cgroup][option]; }

  private:
    void Initialize();

    Int_t fNumEvents;

    // Channels' Header
    Bool_t fAC[4];
    Bool_t fDSM[4];
    Bool_t fIP[4];
    Int_t fID[4];
    Int_t fAO[4];
    Int_t fThreshold[4];
    Int_t fRL[4];

    // Triggers' Header
    Bool_t fCLT;
    Int_t fDT[2];
    Int_t fCW[2];
    Bool_t fTMWidth[2];
    Int_t fTMWidthOption[2];
    Double_t fTMWidthThreshold[2];
    Bool_t fTMCount[2];
    Int_t fTMCountOption[2];
    Int_t fTMCountThreshold[2][2];
    Int_t fTMCountInterval[2][2];
  
  ClassDef(FADC400Header, 1);
};

#endif
