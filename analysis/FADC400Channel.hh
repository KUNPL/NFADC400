/////////////////////////////////////
//                                 //
//       FADC400 module GUI        //
//   Channel Data Container Class  //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 11. 01           //
//                                 //
/////////////////////////////////////

#ifndef _FADC400CHANNEL_H_
#define _FADC400CHANNEL_H_

#include "TROOT.h"

class FADC400Channel
{
  public:
    FADC400Channel();
    ~FADC400Channel();

  private:
    void Initialize();

    UChar_t fTriggerPattern;
    UChar_t fTag[7];
    UShort_t FADC[0x80000];
};

#endif
