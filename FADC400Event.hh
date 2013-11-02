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

class FADC400Event : public TNamed
{
  public:
    FADC400Event() {};
    virtual ~FADC400Event() {};

    void SetNumData(Int_t value)            { fNumData = value; }
    void SetWidthTrigger(Short_t value)     { fWidthTrigger = value; }
    void SetCountTrigger(Short_t value)     { fCountTrigger = value; }
    void SetTriggerInCounts(Short_t value)  { fTriggerInCounts = value; }
    void SetTriggerTime(ULong64_t value)    { fTriggerTime = value; }

      Int_t GetNumData()         { return fNumData; }
    Short_t GetWidthTrigger()    { return fWidthTrigger; }
    Short_t GetCountTrigger()    { return fCountTrigger; }
    Short_t GetTriggerInCounts() { return fTriggerInCounts; }
   Long64_t GetTriggerTime()     { return fTriggerTime; }

    void Initialize()
    {
      fNumData = 0;
      fWidthTrigger = 0;
      fCountTrigger = 0;
      fTriggerInCounts = 0;
      fTriggerTime = 0;
    }

  private:
    Int_t fNumData;
    Short_t fWidthTrigger;
    Short_t fCountTrigger;
    Short_t fTriggerInCounts;
    ULong64_t fTriggerTime;

  ClassDef(FADC400Event, 1);
};

class FADC400Event1 : public FADC400Event
{
  public:
    FADC400Event1() { Initialize(); InitializeADC(); };
    virtual ~FADC400Event1() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Short_t value) { fAdc[index] = value; }

    Short_t *GetADC()             { return fAdc; }
    Short_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL1);

      for (Int_t iADC = 0; iADC < RL1; iADC++)
        fAdc[iADC] = 0;
    }

    Short_t fAdc[RL1];

  ClassDef(FADC400Event1, 1);
};

class FADC400Event2 : public FADC400Event
{
  public:
    FADC400Event2() { Initialize(); InitializeADC(); };
    virtual ~FADC400Event2() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Short_t value) { fAdc[index] = value; }

    Short_t *GetADC()             { return fAdc; }
    Short_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL2);

      for (Int_t iADC = 0; iADC < RL2; iADC++)
        fAdc[iADC] = 0;
    }

    Short_t fAdc[RL2];

  ClassDef(FADC400Event2, 1);
};

class FADC400Event4 : public FADC400Event
{
  public:
    FADC400Event4() { Initialize(); InitializeADC(); };
    virtual ~FADC400Event4() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Short_t value) { fAdc[index] = value; }

    Short_t *GetADC()             { return fAdc; }
    Short_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL4);

      for (Int_t iADC = 0; iADC < RL4; iADC++)
        fAdc[iADC] = 0;
    }

    Short_t fAdc[RL4];

  ClassDef(FADC400Event4, 1);
};

class FADC400Event8 : public FADC400Event
{
  public:
    FADC400Event8() { Initialize(); InitializeADC(); };
    virtual ~FADC400Event8() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Short_t value) { fAdc[index] = value; }

    Short_t *GetADC()             { return fAdc; }
    Short_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL8);

      for (Int_t iADC = 0; iADC < RL8; iADC++)
        fAdc[iADC] = 0;
    }

    Short_t fAdc[RL8];

  ClassDef(FADC400Event8, 1);
};

class FADC400Event16 : public FADC400Event
{
  public:
    FADC400Event16() { Initialize(); InitializeADC(); };
    virtual ~FADC400Event16() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Short_t value) { fAdc[index] = value; }

    Short_t *GetADC()             { return fAdc; }
    Short_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL16);

      for (Int_t iADC = 0; iADC < RL16; iADC++)
        fAdc[iADC] = 0;
    }

    Short_t fAdc[RL16];

  ClassDef(FADC400Event16, 1);
};

class FADC400Event32 : public FADC400Event
{
  public:
    FADC400Event32() { Initialize(); InitializeADC(); };
    virtual ~FADC400Event32() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Short_t value) { fAdc[index] = value; }

    Short_t *GetADC()             { return fAdc; }
    Short_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL32);

      for (Int_t iADC = 0; iADC < RL32; iADC++)
        fAdc[iADC] = 0;
    }

    Short_t fAdc[RL32];

  ClassDef(FADC400Event32, 1);
};

class FADC400Event64 : public FADC400Event
{
  public:
    FADC400Event64() { Initialize(); InitializeADC(); };
    virtual ~FADC400Event64() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Short_t value) { fAdc[index] = value; }

    Short_t *GetADC()             { return fAdc; }
    Short_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL64);

      for (Int_t iADC = 0; iADC < RL64; iADC++)
        fAdc[iADC] = 0;
    }

    Short_t fAdc[RL64];

  ClassDef(FADC400Event64, 1);
};

class FADC400Event128 : public FADC400Event
{
  public:
    FADC400Event128() { Initialize(); InitializeADC(); };
    virtual ~FADC400Event128() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Short_t value) { fAdc[index] = value; }

    Short_t *GetADC()             { return fAdc; }
    Short_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL128);

      for (Int_t iADC = 0; iADC < RL128; iADC++)
        fAdc[iADC] = 0;
    }

    Short_t fAdc[RL128];

  ClassDef(FADC400Event128, 1);
};

class FADC400Event256 : public FADC400Event
{
  public:
    FADC400Event256() { Initialize(); InitializeADC(); };
    virtual ~FADC400Event256() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Short_t value) { fAdc[index] = value; }

    Short_t *GetADC()             { return fAdc; }
    Short_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL256);

      for (Int_t iADC = 0; iADC < RL256; iADC++)
        fAdc[iADC] = 0;
    }

    Short_t fAdc[RL256];

  ClassDef(FADC400Event256, 1);
};

class FADC400Event512 : public FADC400Event
{
  public:
    FADC400Event512() { Initialize(); InitializeADC(); };
    virtual ~FADC400Event512() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Short_t value) { fAdc[index] = value; }

    Short_t *GetADC()             { return fAdc; }
    Short_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL512);

      for (Int_t iADC = 0; iADC < RL512; iADC++)
        fAdc[iADC] = 0;
    }

    Short_t fAdc[RL512];

  ClassDef(FADC400Event512, 1);
};

#endif
