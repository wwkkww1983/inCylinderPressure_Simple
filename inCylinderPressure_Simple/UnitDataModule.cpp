//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitDataModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDM *DM;
//---------------------------------------------------------------------------
__fastcall TDM::TDM(TComponent* Owner)
    : TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
