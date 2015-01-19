/////////////////////////////////////
//                                 //
//       NFADC400 module GUI       //
//    Data Taking Process Class    //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 11. 01           //
//                                 //
/////////////////////////////////////

#ifndef _NFADC400PROCESS_H_
#define _NFADC400PROCESS_H_

#include "NFADC400Constants.hh"
#include "NFADC400Settings.hh"

#include "NoticeNFADC400.h"
#include "TROOT.h"

class TFile;
class TTree;
class NFADC400Header;
class TClonesArray;

class NFADC400Process 
{
  public:
    NFADC400Process(NFADC400Settings);
    ~NFADC400Process();

  private:
    void Initialize();
    void TakeData();
    void SaveHeader();

    void DataRL1(Int_t, Int_t);
    void DataRL2(Int_t, Int_t);
    void DataRL4(Int_t, Int_t);
    void DataRL8(Int_t, Int_t);
    void DataRL16(Int_t, Int_t);
    void DataRL32(Int_t, Int_t);
    void DataRL64(Int_t, Int_t);
    void DataRL128(Int_t, Int_t);
    void DataRL256(Int_t, Int_t);
    void DataRL512(Int_t, Int_t);
    void DataRL1024(Int_t, Int_t);
    void DataRL2048(Int_t, Int_t);
    void DataRL4096(Int_t, Int_t);

    NKNFADC400 fAdc; // NOTICE KOREA NFADC400 Class
    Int_t fNKUSB;
    Int_t fModuleID[2];
    Int_t fActiveModule[2];
    Int_t fActiveChannel[2][4];
    Int_t fEventsToTake;
    Int_t fEventNum[2];

    TFile *fOutFile;
    NFADC400Header *fHeader[2];
    TTree *fEventTree[2][4];
    TClonesArray *fEvent[2][4];
};

#endif
