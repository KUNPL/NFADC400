/////////////////////////////////////
//                                 //
//        FADC400 module GUI       //
//    Event Data Container Class   //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 11. 03           //
//                                 //
/////////////////////////////////////

#ifndef _FADC400EVENT_H_
#define _FADC400EVENT_H_

#include "TNamed.h"
#include "FADC400Constants.hh"

class FADC400Event1 : public TNamed
{
  public:
    FADC400Event1() { Initialize(); };
    virtual ~FADC400Event1() { Initialize(); };

    void SetWidthTrigger(Short_t value)     { widthTrigger = value; }
    void SetCountTrigger(Short_t value)     { countTrigger = value; }
    void SetTriggerInCounts(Short_t value)  { triggerInCounts = value; }
    void SetTriggerTime(ULong64_t value)    { triggerTime = value; }
    void SetADC(Int_t index, Short_t value) { adc[index] = value; }

    Short_t  GetWidthTrigger()    { return widthTrigger; }
    Short_t  GetCountTrigger()    { return countTrigger; }
    Short_t  GetTriggerInCounts() { return triggerInCounts; }
   Long64_t  GetTriggerTime()     { return triggerTime; }
    Short_t *GetADC()             { return adc; }
    Short_t  GetADC(Int_t index)  { return adc[index]; }

  private:
    void Initialize()
    {
      widthTrigger = 0;
      countTrigger = 0;
      triggerInCounts = 0;
      triggerTime = 0;

      for (Int_t iADC = 0; iADC < RL1; iADC++)
        adc[iADC] = 0;
    }

    Short_t widthTrigger;
    Short_t countTrigger;
    Short_t triggerInCounts;
    ULong64_t triggerTime;
    Short_t adc[RL1];

  ClassDef(FADC400Event1, 1);
};

class FADC400Event2 : public TNamed
{
  public:
    FADC400Event2() { Initialize(); };
    virtual ~FADC400Event2() { Initialize(); };

    void SetWidthTrigger(Short_t value)     { widthTrigger = value; }
    void SetCountTrigger(Short_t value)     { countTrigger = value; }
    void SetTriggerInCounts(Short_t value)  { triggerInCounts = value; }
    void SetTriggerTime(ULong64_t value)    { triggerTime = value; }
    void SetADC(Int_t index, Short_t value) { adc[index] = value; }

    Short_t  GetWidthTrigger()    { return widthTrigger; }
    Short_t  GetCountTrigger()    { return countTrigger; }
    Short_t  GetTriggerInCounts() { return triggerInCounts; }
   Long64_t  GetTriggerTime()     { return triggerTime; }
    Short_t *GetADC()             { return adc; }
    Short_t  GetADC(Int_t index)  { return adc[index]; }

  private:
    void Initialize()
    {
      widthTrigger = 0;
      countTrigger = 0;
      triggerInCounts = 0;
      triggerTime = 0;

      for (Int_t iADC = 0; iADC < RL2; iADC++)
        adc[iADC] = 0;
    }

    Short_t widthTrigger;
    Short_t countTrigger;
    Short_t triggerInCounts;
    ULong64_t triggerTime;
    Short_t adc[RL2];

  ClassDef(FADC400Event2, 1);
};

class FADC400Event4 : public TNamed
{
  public:
    FADC400Event4() { Initialize(); };
    virtual ~FADC400Event4() { Initialize(); };

    void SetWidthTrigger(Short_t value)     { widthTrigger = value; }
    void SetCountTrigger(Short_t value)     { countTrigger = value; }
    void SetTriggerInCounts(Short_t value)  { triggerInCounts = value; }
    void SetTriggerTime(ULong64_t value)    { triggerTime = value; }
    void SetADC(Int_t index, Short_t value) { adc[index] = value; }

    Short_t  GetWidthTrigger()    { return widthTrigger; }
    Short_t  GetCountTrigger()    { return countTrigger; }
    Short_t  GetTriggerInCounts() { return triggerInCounts; }
   Long64_t  GetTriggerTime()     { return triggerTime; }
    Short_t *GetADC()             { return adc; }
    Short_t  GetADC(Int_t index)  { return adc[index]; }

  private:
    void Initialize()
    {
      widthTrigger = 0;
      countTrigger = 0;
      triggerInCounts = 0;
      triggerTime = 0;

      for (Int_t iADC = 0; iADC < RL4; iADC++)
        adc[iADC] = 0;
    }

    Short_t widthTrigger;
    Short_t countTrigger;
    Short_t triggerInCounts;
    ULong64_t triggerTime;
    Short_t adc[RL4];

  ClassDef(FADC400Event4, 1);
};

class FADC400Event8 : public TNamed
{
  public:
    FADC400Event8() { Initialize(); };
    virtual ~FADC400Event8() { Initialize(); };

    void SetWidthTrigger(Short_t value)     { widthTrigger = value; }
    void SetCountTrigger(Short_t value)     { countTrigger = value; }
    void SetTriggerInCounts(Short_t value)  { triggerInCounts = value; }
    void SetTriggerTime(ULong64_t value)    { triggerTime = value; }
    void SetADC(Int_t index, Short_t value) { adc[index] = value; }

    Short_t  GetWidthTrigger()    { return widthTrigger; }
    Short_t  GetCountTrigger()    { return countTrigger; }
    Short_t  GetTriggerInCounts() { return triggerInCounts; }
   Long64_t  GetTriggerTime()     { return triggerTime; }
    Short_t *GetADC()             { return adc; }
    Short_t  GetADC(Int_t index)  { return adc[index]; }

  private:
    void Initialize()
    {
      widthTrigger = 0;
      countTrigger = 0;
      triggerInCounts = 0;
      triggerTime = 0;

      for (Int_t iADC = 0; iADC < RL8; iADC++)
        adc[iADC] = 0;
    }

    Short_t widthTrigger;
    Short_t countTrigger;
    Short_t triggerInCounts;
    ULong64_t triggerTime;
    Short_t adc[RL8];

  ClassDef(FADC400Event8, 1);
};

class FADC400Event16 : public TNamed
{
  public:
    FADC400Event16() { Initialize(); };
    virtual ~FADC400Event16() { Initialize(); };

    void SetWidthTrigger(Short_t value)     { widthTrigger = value; }
    void SetCountTrigger(Short_t value)     { countTrigger = value; }
    void SetTriggerInCounts(Short_t value)  { triggerInCounts = value; }
    void SetTriggerTime(ULong64_t value)    { triggerTime = value; }
    void SetADC(Int_t index, Short_t value) { adc[index] = value; }

    Short_t  GetWidthTrigger()    { return widthTrigger; }
    Short_t  GetCountTrigger()    { return countTrigger; }
    Short_t  GetTriggerInCounts() { return triggerInCounts; }
   Long64_t  GetTriggerTime()     { return triggerTime; }
    Short_t *GetADC()             { return adc; }
    Short_t  GetADC(Int_t index)  { return adc[index]; }

  private:
    void Initialize()
    {
      widthTrigger = 0;
      countTrigger = 0;
      triggerInCounts = 0;
      triggerTime = 0;

      for (Int_t iADC = 0; iADC < RL16; iADC++)
        adc[iADC] = 0;
    }

    Short_t widthTrigger;
    Short_t countTrigger;
    Short_t triggerInCounts;
    ULong64_t triggerTime;
    Short_t adc[RL16];

  ClassDef(FADC400Event16, 1);
};

class FADC400Event32 : public TNamed
{
  public:
    FADC400Event32() { Initialize(); };
    virtual ~FADC400Event32() { Initialize(); };

    void SetWidthTrigger(Short_t value)     { widthTrigger = value; }
    void SetCountTrigger(Short_t value)     { countTrigger = value; }
    void SetTriggerInCounts(Short_t value)  { triggerInCounts = value; }
    void SetTriggerTime(ULong64_t value)    { triggerTime = value; }
    void SetADC(Int_t index, Short_t value) { adc[index] = value; }

    Short_t  GetWidthTrigger()    { return widthTrigger; }
    Short_t  GetCountTrigger()    { return countTrigger; }
    Short_t  GetTriggerInCounts() { return triggerInCounts; }
   Long64_t  GetTriggerTime()     { return triggerTime; }
    Short_t *GetADC()             { return adc; }
    Short_t  GetADC(Int_t index)  { return adc[index]; }

  private:
    void Initialize()
    {
      widthTrigger = 0;
      countTrigger = 0;
      triggerInCounts = 0;
      triggerTime = 0;

      for (Int_t iADC = 0; iADC < RL32; iADC++)
        adc[iADC] = 0;
    }

    Short_t widthTrigger;
    Short_t countTrigger;
    Short_t triggerInCounts;
    ULong64_t triggerTime;
    Short_t adc[RL32];

  ClassDef(FADC400Event32, 1);
};

class FADC400Event64 : public TNamed
{
  public:
    FADC400Event64() { Initialize(); };
    virtual ~FADC400Event64() { Initialize(); };

    void SetWidthTrigger(Short_t value)     { widthTrigger = value; }
    void SetCountTrigger(Short_t value)     { countTrigger = value; }
    void SetTriggerInCounts(Short_t value)  { triggerInCounts = value; }
    void SetTriggerTime(ULong64_t value)    { triggerTime = value; }
    void SetADC(Int_t index, Short_t value) { adc[index] = value; }

    Short_t  GetWidthTrigger()    { return widthTrigger; }
    Short_t  GetCountTrigger()    { return countTrigger; }
    Short_t  GetTriggerInCounts() { return triggerInCounts; }
   Long64_t  GetTriggerTime()     { return triggerTime; }
    Short_t *GetADC()             { return adc; }
    Short_t  GetADC(Int_t index)  { return adc[index]; }

  private:
    void Initialize()
    {
      widthTrigger = 0;
      countTrigger = 0;
      triggerInCounts = 0;
      triggerTime = 0;

      for (Int_t iADC = 0; iADC < RL64; iADC++)
        adc[iADC] = 0;
    }

    Short_t widthTrigger;
    Short_t countTrigger;
    Short_t triggerInCounts;
    ULong64_t triggerTime;
    Short_t adc[RL64];

  ClassDef(FADC400Event64, 1);
};

class FADC400Event128 : public TNamed
{
  public:
    FADC400Event128() { Initialize(); };
    virtual ~FADC400Event128() { Initialize(); };

    void SetWidthTrigger(Short_t value)     { widthTrigger = value; }
    void SetCountTrigger(Short_t value)     { countTrigger = value; }
    void SetTriggerInCounts(Short_t value)  { triggerInCounts = value; }
    void SetTriggerTime(ULong64_t value)    { triggerTime = value; }
    void SetADC(Int_t index, Short_t value) { adc[index] = value; }

    Short_t  GetWidthTrigger()    { return widthTrigger; }
    Short_t  GetCountTrigger()    { return countTrigger; }
    Short_t  GetTriggerInCounts() { return triggerInCounts; }
   Long64_t  GetTriggerTime()     { return triggerTime; }
    Short_t *GetADC()             { return adc; }
    Short_t  GetADC(Int_t index)  { return adc[index]; }

  private:
    void Initialize()
    {
      widthTrigger = 0;
      countTrigger = 0;
      triggerInCounts = 0;
      triggerTime = 0;

      for (Int_t iADC = 0; iADC < RL128; iADC++)
        adc[iADC] = 0;
    }

    Short_t widthTrigger;
    Short_t countTrigger;
    Short_t triggerInCounts;
    ULong64_t triggerTime;
    Short_t adc[RL128];

  ClassDef(FADC400Event128, 1);
};

class FADC400Event256 : public TNamed
{
  public:
    FADC400Event256() { Initialize(); };
    virtual ~FADC400Event256() { Initialize(); };

    void SetWidthTrigger(Short_t value)     { widthTrigger = value; }
    void SetCountTrigger(Short_t value)     { countTrigger = value; }
    void SetTriggerInCounts(Short_t value)  { triggerInCounts = value; }
    void SetTriggerTime(ULong64_t value)    { triggerTime = value; }
    void SetADC(Int_t index, Short_t value) { adc[index] = value; }

    Short_t  GetWidthTrigger()    { return widthTrigger; }
    Short_t  GetCountTrigger()    { return countTrigger; }
    Short_t  GetTriggerInCounts() { return triggerInCounts; }
   Long64_t  GetTriggerTime()     { return triggerTime; }
    Short_t *GetADC()             { return adc; }
    Short_t  GetADC(Int_t index)  { return adc[index]; }

  private:
    void Initialize()
    {
      widthTrigger = 0;
      countTrigger = 0;
      triggerInCounts = 0;
      triggerTime = 0;

      for (Int_t iADC = 0; iADC < RL256; iADC++)
        adc[iADC] = 0;
    }

    Short_t widthTrigger;
    Short_t countTrigger;
    Short_t triggerInCounts;
    ULong64_t triggerTime;
    Short_t adc[RL256];

  ClassDef(FADC400Event256, 1);
};

class FADC400Event512 : public TNamed
{
  public:
    FADC400Event512() { Initialize(); };
    virtual ~FADC400Event512() { Initialize(); };

    void SetWidthTrigger(Short_t value)     { widthTrigger = value; }
    void SetCountTrigger(Short_t value)     { countTrigger = value; }
    void SetTriggerInCounts(Short_t value)  { triggerInCounts = value; }
    void SetTriggerTime(ULong64_t value)    { triggerTime = value; }
    void SetADC(Int_t index, Short_t value) { adc[index] = value; }

    Short_t  GetWidthTrigger()    { return widthTrigger; }
    Short_t  GetCountTrigger()    { return countTrigger; }
    Short_t  GetTriggerInCounts() { return triggerInCounts; }
   Long64_t  GetTriggerTime()     { return triggerTime; }
    Short_t *GetADC()             { return adc; }
    Short_t  GetADC(Int_t index)  { return adc[index]; }

  private:
    void Initialize()
    {
      widthTrigger = 0;
      countTrigger = 0;
      triggerInCounts = 0;
      triggerTime = 0;

      for (Int_t iADC = 0; iADC < RL512; iADC++)
        adc[iADC] = 0;
    }

    Short_t widthTrigger;
    Short_t countTrigger;
    Short_t triggerInCounts;
    ULong64_t triggerTime;
    Short_t adc[RL512];

  ClassDef(FADC400Event512, 1);
};

#endif
