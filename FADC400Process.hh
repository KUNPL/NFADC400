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
#include "FADC400Header.hh"

class TFile;
class TTree;

class FADC400Process 
{
  public:
    FADC400Process(FADC400Settings);
    ~FADC400Process();

  private:
    void Initialize();
    void TakeData();
    void SaveHeader();

    Int_t fNKUSB;
    Int_t fModuleID[2];
    Int_t fActiveModule[2];
    Int_t fActiveChannel[2][4];

    TFile *outFile;
    FADC400Header *header[2];
    TTree *data[2][4];
};

#endif
