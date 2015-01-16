/////////////////////////////////////
//                                 //
//       NFADC400 module GUI       //
//    Event Data Container Class   //
//                                 //
//  Author: Genie Jhang            //
// Contact: geniejhang@majimak.com //
//    Date: 2013. 11. 03           //
//                                 //
/////////////////////////////////////

#ifndef _NFADC400EVENT_H_
#define _NFADC400EVENT_H_

#include "TNamed.h"
#include "NFADC400Constants.hh"

class NFADC400Event : public TNamed
{
  public:
    NFADC400Event() {};
    virtual ~NFADC400Event() {};

    void SetEventID(ULong64_t value)        { fEventID = value; }
    void SetNumData(Int_t value)            { fNumData = value; }
    void SetWidthTrigger(Short_t value)     { fWidthTrigger = value; }
    void SetCountTrigger(Short_t value)     { fCountTrigger = value; }
    void SetTriggerTime(ULong64_t value)    { fTriggerTime = value; }
    virtual void SetADC(Int_t index, Double_t value) = 0;

    virtual Double_t *GetADC() = 0;
    virtual Double_t  GetADC(Int_t index) = 0;

  ULong64_t GetEventID()         { return fEventID; }
      Int_t GetNumData()         { return fNumData; }
    Short_t GetWidthTrigger()    { return fWidthTrigger; }
    Short_t GetCountTrigger()    { return fCountTrigger; }
  ULong64_t GetTriggerTime()     { return fTriggerTime; }

    void Initialize()
    {
      fEventID = 0;
      fNumData = 0;
      fWidthTrigger = 0;
      fCountTrigger = 0;
      fTriggerTime = 0;
    }

  private:
  ULong64_t fEventID;
      Int_t fNumData;
    Short_t fWidthTrigger;
    Short_t fCountTrigger;
  ULong64_t fTriggerTime;

  ClassDef(NFADC400Event, 2);
};

class NFADC400Event1 : public NFADC400Event
{
  public:
    NFADC400Event1() { Initialize(); InitializeADC(); };
    virtual ~NFADC400Event1() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Double_t value) { fAdc[index] = value; }

    Double_t *GetADC()             { return fAdc; }
    Double_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL1);

      for (Int_t iADC = 0; iADC < RL1; iADC++)
        fAdc[iADC] = 0;
    }

    Double_t fAdc[RL1];

  ClassDef(NFADC400Event1, 2);
};

class NFADC400Event2 : public NFADC400Event
{
  public:
    NFADC400Event2() { Initialize(); InitializeADC(); };
    virtual ~NFADC400Event2() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Double_t value) { fAdc[index] = value; }

    Double_t *GetADC()             { return fAdc; }
    Double_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL2);

      for (Int_t iADC = 0; iADC < RL2; iADC++)
        fAdc[iADC] = 0;
    }

    Double_t fAdc[RL2];

  ClassDef(NFADC400Event2, 2);
};

class NFADC400Event4 : public NFADC400Event
{
  public:
    NFADC400Event4() { Initialize(); InitializeADC(); };
    virtual ~NFADC400Event4() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Double_t value) { fAdc[index] = value; }

    Double_t *GetADC()             { return fAdc; }
    Double_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL4);

      for (Int_t iADC = 0; iADC < RL4; iADC++)
        fAdc[iADC] = 0;
    }

    Double_t fAdc[RL4];

  ClassDef(NFADC400Event4, 2);
};

class NFADC400Event8 : public NFADC400Event
{
  public:
    NFADC400Event8() { Initialize(); InitializeADC(); };
    virtual ~NFADC400Event8() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Double_t value) { fAdc[index] = value; }

    Double_t *GetADC()             { return fAdc; }
    Double_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL8);

      for (Int_t iADC = 0; iADC < RL8; iADC++)
        fAdc[iADC] = 0;
    }

    Double_t fAdc[RL8];

  ClassDef(NFADC400Event8, 2);
};

class NFADC400Event16 : public NFADC400Event
{
  public:
    NFADC400Event16() { Initialize(); InitializeADC(); };
    virtual ~NFADC400Event16() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Double_t value) { fAdc[index] = value; }

    Double_t *GetADC()             { return fAdc; }
    Double_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL16);

      for (Int_t iADC = 0; iADC < RL16; iADC++)
        fAdc[iADC] = 0;
    }

    Double_t fAdc[RL16];

  ClassDef(NFADC400Event16, 2);
};

class NFADC400Event32 : public NFADC400Event
{
  public:
    NFADC400Event32() { Initialize(); InitializeADC(); };
    virtual ~NFADC400Event32() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Double_t value) { fAdc[index] = value; }

    Double_t *GetADC()             { return fAdc; }
    Double_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL32);

      for (Int_t iADC = 0; iADC < RL32; iADC++)
        fAdc[iADC] = 0;
    }

    Double_t fAdc[RL32];

  ClassDef(NFADC400Event32, 2);
};

class NFADC400Event64 : public NFADC400Event
{
  public:
    NFADC400Event64() { Initialize(); InitializeADC(); };
    virtual ~NFADC400Event64() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Double_t value) { fAdc[index] = value; }

    Double_t *GetADC()             { return fAdc; }
    Double_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL64);

      for (Int_t iADC = 0; iADC < RL64; iADC++)
        fAdc[iADC] = 0;
    }

    Double_t fAdc[RL64];

  ClassDef(NFADC400Event64, 2);
};

class NFADC400Event128 : public NFADC400Event
{
  public:
    NFADC400Event128() { Initialize(); InitializeADC(); };
    virtual ~NFADC400Event128() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Double_t value) { fAdc[index] = value; }

    Double_t *GetADC()             { return fAdc; }
    Double_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL128);

      for (Int_t iADC = 0; iADC < RL128; iADC++)
        fAdc[iADC] = 0;
    }

    Double_t fAdc[RL128];

  ClassDef(NFADC400Event128, 2);
};

class NFADC400Event256 : public NFADC400Event
{
  public:
    NFADC400Event256() { Initialize(); InitializeADC(); };
    virtual ~NFADC400Event256() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Double_t value) { fAdc[index] = value; }

    Double_t *GetADC()             { return fAdc; }
    Double_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL256);

      for (Int_t iADC = 0; iADC < RL256; iADC++)
        fAdc[iADC] = 0;
    }

    Double_t fAdc[RL256];

  ClassDef(NFADC400Event256, 2);
};

class NFADC400Event512 : public NFADC400Event
{
  public:
    NFADC400Event512() { Initialize(); InitializeADC(); };
    virtual ~NFADC400Event512() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Double_t value) { fAdc[index] = value; }

    Double_t *GetADC()             { return fAdc; }
    Double_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL512);

      for (Int_t iADC = 0; iADC < RL512; iADC++)
        fAdc[iADC] = 0;
    }

    Double_t fAdc[RL512];

  ClassDef(NFADC400Event512, 2);
};

class NFADC400Event1024 : public NFADC400Event
{
  public:
    NFADC400Event1024() { Initialize(); InitializeADC(); };
    virtual ~NFADC400Event1024() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Double_t value) { fAdc[index] = value; }

    Double_t *GetADC()             { return fAdc; }
    Double_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL1024);

      for (Int_t iADC = 0; iADC < RL1024; iADC++)
        fAdc[iADC] = 0;
    }

    Double_t fAdc[RL1024];

  ClassDef(NFADC400Event1024, 2);
};

class NFADC400Event2048 : public NFADC400Event
{
  public:
    NFADC400Event2048() { Initialize(); InitializeADC(); };
    virtual ~NFADC400Event2048() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Double_t value) { fAdc[index] = value; }

    Double_t *GetADC()             { return fAdc; }
    Double_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL2048);

      for (Int_t iADC = 0; iADC < RL2048; iADC++)
        fAdc[iADC] = 0;
    }

    Double_t fAdc[RL2048];

  ClassDef(NFADC400Event2048, 2);
};

class NFADC400Event4096 : public NFADC400Event
{
  public:
    NFADC400Event4096() { Initialize(); InitializeADC(); };
    virtual ~NFADC400Event4096() { Initialize(); InitializeADC(); };

    void SetADC(Int_t index, Double_t value) { fAdc[index] = value; }

    Double_t *GetADC()             { return fAdc; }
    Double_t  GetADC(Int_t index)  { return fAdc[index]; }

  private:
    void InitializeADC()
    {
      SetNumData(RL4096);

      for (Int_t iADC = 0; iADC < RL4096; iADC++)
        fAdc[iADC] = 0;
    }

    Double_t fAdc[RL4096];

  ClassDef(NFADC400Event4096, 2);
};

#endif
