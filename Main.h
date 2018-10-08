//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TLFSRCrypt : public TForm
{
__published:	// IDE-managed Components
	TMemo *KeyIn;
	TBitBtn *btnEncipher;
	TBitBtn *btnOpen;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TProgressBar *ProgressBar;
	TMemo *Polynom1;
	TMemo *Polynom2;
	TMemo *Polynom3;
	TBitBtn *btnAccept;
	TBitBtn *btnLabPoly;
	TCheckBox *CheckBoxGeffe;
	TRadioButton *RadioButton;
	void __fastcall btnOpenClick(TObject *Sender);
	void __fastcall btnEncipherClick(TObject *Sender);
	void __fastcall KeyInChange(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnAcceptClick(TObject *Sender);
	void __fastcall btnLabPolyClick(TObject *Sender);
	void __fastcall CheckBoxGeffeClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TLFSRCrypt(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLFSRCrypt *LFSRCrypt;
//---------------------------------------------------------------------------
#endif
