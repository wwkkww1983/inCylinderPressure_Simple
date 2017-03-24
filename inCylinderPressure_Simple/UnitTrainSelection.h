//---------------------------------------------------------------------------

#ifndef UnitTrainSelectionH
#define UnitTrainSelectionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormTrainSelection : public TForm
{
__published:	// IDE-managed Components
    TButton *Button1;
    TButton *Button2;
    TGroupBox *GroupBoxMain;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
    TCheckBox **CheckBoxSelected;
    int        iTrainNums;
    bool       bFormEntered;
public:		// User declarations
    __fastcall TFormTrainSelection(TComponent* Owner);
    void __fastcall SetiTrainNums( int itn)
    {
        iTrainNums = itn;
    };
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTrainSelection *FormTrainSelection;
//---------------------------------------------------------------------------
#endif
