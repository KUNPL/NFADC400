/////////////////////////////////////
//                                 //
//       FADC400 module GUI        //
//    Data Taking Process Class    //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 11. 01           //
//                                 //
/////////////////////////////////////

#ifndef __FADC400PROCESS_H_
#define __FADC400PROCESS_H_

#include "FADC400Constants.hh"

class TFile;
class FADC400Header;
class TClonesArray;

class FADC400Process 
{
  public:
    FADC400Process(FADC400Settings);
    ~FADC400Process();

  private:
    void Initialize();
    void TakeData();
    void SaveHeader();

    void DataRL1(Int_t, Int_t, Int_t);
    void DataRL2(Int_t, Int_t, Int_t);
    void DataRL4(Int_t, Int_t, Int_t);
    void DataRL8(Int_t, Int_t, Int_t);
    void DataRL16(Int_t, Int_t, Int_t);
    void DataRL32(Int_t, Int_t, Int_t);
    void DataRL64(Int_t, Int_t, Int_t);
    void DataRL128(Int_t, Int_t, Int_t);
    void DataRL256(Int_t, Int_t, Int_t);
    void DataRL512(Int_t, Int_t, Int_t);

    Int_t fNKUSB;
    Int_t fModuleID[2];
    Int_t fActiveModule[2];
    Int_t fActiveChannel[2][4];
    Int_t fEventNum[2][4];

    TFile *fOutFile;
    FADC400Header *fHeader[2];
    TClonesArray *fEvent[2][4];
};

#endif
