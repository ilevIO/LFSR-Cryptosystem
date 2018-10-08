//---------------------------------------------------------------------------

#include <vcl.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLFSRCrypt *LFSRCrypt;
/*Vars:*/
uint64_t Key = -1;
std::vector<int> LFSR1;
std::vector<int> LFSR2;
std::vector<int> LFSR3;
UnicodeString KeyStr = "";
UnicodeString FInPath;
uint64_t Mask;
bool CorrectKey = true;
//---------------------------------------------------------------------------
__fastcall TLFSRCrypt::TLFSRCrypt(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TLFSRCrypt::btnOpenClick(TObject *Sender)
{
	OpenDialog->Execute();
	FInPath = OpenDialog->FileName;
	if (FInPath!="") {
		RadioButton->Checked = true;
	}
	else
		RadioButton->Checked = false;
}
//---------------------------------------------------------------------------
void InitLabLFSR()
{
	LFSR1.clear();
  LFSR1.push_back(30);
  LFSR1.push_back(16);
  LFSR1.push_back(15);
  LFSR1.push_back(1);
	LFSR2.clear();
  LFSR2.push_back(38);
  LFSR2.push_back(6);
  LFSR2.push_back(5);
  LFSR2.push_back(1);
	LFSR3.clear();
  LFSR3.push_back(28);
  LFSR3.push_back(3);
}

int gi;
int gResult;
int gtmpR;
int gj;
unsigned char GenerateKey(std::vector<int> &LFSR, uint64_t *Res)
{
  gi=0;
  gResult=0;
  while (gi<8)
  {
	gtmpR=0;
	gj=0;
	while(gj<LFSR.size())
	{
	  gtmpR=gtmpR ^ (*Res >> (LFSR[gj]-1));
	  gj++;
	}
	//gtmpR = (*Res>>29)^(*Res>>15)^(*Res>>14);
	gResult=(gResult << 1) | ((*Res >> (LFSR[0]-1)) & 1);
	*Res=(*Res << 1) | (gtmpR & 1) & Mask;
	gi++;
  }
  return gResult;
}
uint64_t TranslateKey(UnicodeString KeyStr)
{
	int i=1;
	uint64_t Key=0;
	AnsiString tmpStr = KeyStr.c_str();
	CorrectKey = true;
	while ((i<=KeyStr.Length())&&(i<64))
	{
		if ((KeyStr[i]=='1') || (KeyStr[i]=='0')) {
			Key=(Key << 1) + (tmpStr[i]-48);
		}
		else
		{
			ShowMessage("Incorrect key");
			Key = 0;
			i = 64;
			CorrectKey=false;
		}
		i++;
	}
	return Key;
}

std::string ToBinary(unsigned char Num)
{
	char str[9];
	char Cha = 0;
	int i = 0;
	while (i<8)
	{
		_asm
		{
			//push al
			shr byte[Num], 1
			mov al, 0
			adc al, 30h
			mov byte[Cha], al
			//pop al
		}
		str[7-i] = Cha;
		i++;
	}
	str[8] = 0;
	return std::string(str);
}

void __fastcall TLFSRCrypt::btnEncipherClick(TObject *Sender)
{
	KeyStr=KeyIn->Text;
	uint64_t Key=TranslateKey(KeyStr);
	if ((LFSR1.size()>0)&&(FInPath!="")&&(CorrectKey))
	{
		AnsiString FilePath = FInPath.c_str();
		FILE *FileIn = fopen(FilePath.c_str(), "r+b");
		fseek(FileIn, 0, SEEK_END);
		int FileSize = (int) ftell(FileIn);
		fseek(FileIn, 0, SEEK_SET);
		if (FileSize>0)
		{
            SaveDialog->FileName = "";
			SaveDialog->Execute();
			if (SaveDialog->FileName!="")
			{
				FilePath = SaveDialog->FileName.c_str();
				FILE *FileOut = fopen(FilePath.c_str(), "w+b");
				//ofstream FileOut (SaveDialog->FileName.c_str(), ios::binary|ios::out);
				Mask=(~0) << LFSR1[0];
				Mask=~Mask;
				unsigned char tmp1;
				unsigned char tmpIn;
				uint64_t Res1=Key;
				uint64_t Res2=Key;
				uint64_t Res3=Key;
				unsigned char BufOut;
				int i = 0;
				int D = FileSize/100 + (FileSize<100);
				ProgressBar->Position = 0;
				std::string BinKey = "";
				std::string BinFileOut = "";
				std::string BinFileIn = "";
				if (!(CheckBoxGeffe->Checked)) {
					while ((i<FileSize)&&(i<20))     //To show first 20 bytes of binary of key and files
					{
						tmp1=GenerateKey(LFSR1, &Res1);
						BinKey = BinKey + ToBinary(tmp1);
						tmpIn=getc(FileIn);
						BinFileIn = BinFileIn + ToBinary(tmpIn);
						BufOut=tmpIn ^ tmp1;

						fprintf(FileOut, "%c", BufOut);
						BinFileOut = BinFileOut +  ToBinary(BufOut);

						if (i % D==0)     //SLOW
							ProgressBar->Position=ProgressBar->Position+1;
						i++;
					}
					while (i<FileSize)
					{
						tmp1=GenerateKey(LFSR1, &Res1);

						tmpIn=getc(FileIn);
						BufOut=tmpIn ^ tmp1;

						fprintf(FileOut, "%c", BufOut);
						if (i % D==0)     //SLOW
							ProgressBar->Position=ProgressBar->Position+1;
						i++;

					}
				}
				else
				{
					unsigned char tmp2;
					unsigned char tmp3;
					if ((LFSR2.size()>0) && (LFSR3.size()>0)) {

						while (i<FileSize)
						{
							tmp1=GenerateKey(LFSR1, &Res1);
							tmp2=GenerateKey(LFSR2, &Res2);
							tmp3=GenerateKey(LFSR3, &Res3);
							tmpIn=getc(FileIn);
							BufOut=tmpIn ^ ((tmp1 & tmp2)|((!tmp1) & tmp3));
							fprintf(FileOut, "%c", BufOut);
							if (i % D==0)     //SLOW
								ProgressBar->Position=ProgressBar->Position+1;
							i++;
						}
					}
					else
						ShowMessage("Введите все многочлены");
				}
				fclose(FileIn);
				fclose(FileOut);
				//KostylOut = KostylOut
				if (i>20) {
					ShowMessage(UnicodeString(BinFileIn.c_str())+"...: Исходный файл\n"
					+UnicodeString(BinFileOut.c_str())+"...: Выходной файл\n"
					+UnicodeString(BinKey.c_str())+"...: Ключ");
				}
				else
					ShowMessage(UnicodeString(BinFileIn.c_str())+": Исходный файл\n"
					+UnicodeString(BinFileOut.c_str())+": Выходной файл\n"
					+UnicodeString(BinKey.c_str())+": Ключ");
			}
			else
			{
				fclose(FileIn);
				ShowMessage("Проверьте файл и многочлен");
			}
		}
		else
		{
			ShowMessage ("Пустой файл");
			fclose(FileIn);
		}

	}
	else
		ShowMessage("Проверьте ключ, многочлен и файл");
}
//---------------------------------------------------------------------------
void __fastcall TLFSRCrypt::KeyInChange(TObject *Sender)
{
	KeyStr=KeyIn->Text;
}
//---------------------------------------------------------------------------


void __fastcall TLFSRCrypt::FormCreate(TObject *Sender)
{
	InitLabLFSR;
	btnLabPolyClick(Application);
}

void SetPolynome (std::vector<int> &LFSR, AnsiString StrIn)
{
  LFSR.clear();
  int i=1;
  int z=1;
  enum {Coeff, Degree, Letter, Null} PrevChar = Null;
  int LastCoeff=1;
  char LastChar = 0;
  int Num;
  int LastDeg = 0;

  while (i<=StrIn.Length())
  {

	if(StrIn[i]!=' ')
	{
		if (StrIn[i]=='x')//((StrIn[i] >='a') && (StrIn<='z'))     //=x
		{
			LastChar=StrIn[i];
			PrevChar=Letter;
			i++;
			while((i<=StrIn.Length())&&(StrIn[i] == ' '))
			  i++;
			if ((i<=StrIn.Length())&&(StrIn[i]=='^'))
			{
			  i++;
			  while ((i<=StrIn.Length())&&(StrIn[i] == ' ') || (StrIn[i]=='('))
				i++;
			  if ((StrIn[i] >='0') && (StrIn[i]<='9'))
			  {
				Num=StrToInt(StrIn[i]);
				i++;
				while ((i<=StrIn.Length()) &&(StrIn[i] >='0') && (StrIn[i]<='9'))
				{
				  Num=Num*10+StrToInt(StrIn[i]);
				  i++;
				}
				LastDeg=Num;
			  }
			}
			else
			{
			  LastDeg=1;
			  i--;
			}
			PrevChar=Degree;
			LFSR.push_back(LastDeg);
			//int Check = LFSR[0];
			//int f = Check;
			//Check = LFSR[LFSR.size()-1];
			//f = Check;
		}
		else if (StrIn[i]!='1')
		{
			ShowMessage("Incorrect polynome");
			i = StrIn.Length();
		}
	}
	else
	{
	  //char *tmpStr = (char*)malloc(sizeof(char));
	  //strncpy(tmpStr, StrIn.c_str(), StrIn.Length());
	  std::string tmpStr2 = StrIn.c_str();
	  tmpStr2.erase(i, 1);
	  StrIn = AnsiString(tmpStr2.c_str());
	}
	i++;
  }

}
//---------------------------------------------------------------------------

void __fastcall TLFSRCrypt::btnAcceptClick(TObject *Sender)
{
	SetPolynome(LFSR1, Polynom1->Text.c_str());
	if (CheckBoxGeffe->Checked) {
		SetPolynome(LFSR2, Polynom2->Text.c_str());
		SetPolynome(LFSR3, Polynom3->Text.c_str());
		//ShowMessage("Polynomes set");
	}
	//else
		//ShowMessage("Polynome set");
      int i=0;
  int j=1;
  Polynom1->Text="";
  while (i<LFSR1.size())
  {
	Polynom1->Text=Polynom1->Text+"x^"+IntToStr(LFSR1[i])+'+';
	i++;
  }
  Polynom1->Text=Polynom1->Text+'1';

  if (CheckBoxGeffe->Checked)
  {
	  Polynom2->Text="";
	  i=0;
	  while (i<LFSR2.size())
	  {
		Polynom2->Text=Polynom2->Text+"x^"+IntToStr(LFSR2[i])+'+';
		i++;
	  }
	  Polynom2->Text=Polynom2->Text+'1';

	  Polynom3->Text="";
	  i=0;
	  while (i<LFSR3.size())
	  {
		Polynom3->Text=Polynom3->Text+"x^"+IntToStr(LFSR3[i])+'+';
		i++;
	  }
	  Polynom3->Text=Polynom3->Text+'1';
  }
}
//---------------------------------------------------------------------------

void __fastcall TLFSRCrypt::btnLabPolyClick(TObject *Sender)
{
	//InitLabLFSR;
  	LFSR1.clear();
  LFSR1.push_back(30);
  LFSR1.push_back(16);
  LFSR1.push_back(15);
  LFSR1.push_back(1);
	LFSR2.clear();
  LFSR2.push_back(38);
  LFSR2.push_back(6);
  LFSR2.push_back(5);
  LFSR2.push_back(1);
	LFSR3.clear();
  LFSR3.push_back(28);
  LFSR3.push_back(3);
  int i=0;
  int j=1;
  Polynom1->Text="";
  while (i<LFSR1.size())
  {
	Polynom1->Text=Polynom1->Text+"x^"+IntToStr(LFSR1[i])+'+';
	i++;
  }
  Polynom1->Text=Polynom1->Text+'1';

  if (CheckBoxGeffe->Checked)
  {
	  Polynom2->Text="";
	  i=0;
	  while (i<LFSR2.size())
	  {
		Polynom2->Text=Polynom2->Text+"x^"+IntToStr(LFSR2[i])+'+';
		i++;
	  }
	  Polynom2->Text=Polynom2->Text+'1';

	  Polynom3->Text="";
	  i=0;
	  while (i<LFSR3.size())
	  {
		Polynom3->Text=Polynom3->Text+"x^"+IntToStr(LFSR3[i])+'+';
		i++;
	  }
	  Polynom3->Text=Polynom3->Text+'1';
  }
}
//---------------------------------------------------------------------------


void __fastcall TLFSRCrypt::CheckBoxGeffeClick(TObject *Sender)
{
	/*if (CheckBoxGeffe->Checked) {
		RadioButton1->Checked = 1;
		RadioButton2->Checked = 1;
	}
	else {
		RadioButton1->Checked = 0;
		RadioButton2->Checked = 0;
	}                    */
}
//---------------------------------------------------------------------------



