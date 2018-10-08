object LFSRCrypt: TLFSRCrypt
  Left = 0
  Top = 0
  Caption = 'LFSRCrypt'
  ClientHeight = 487
  ClientWidth = 887
  Color = clWindowFrame
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object KeyIn: TMemo
    Left = 472
    Top = 157
    Width = 393
    Height = 89
    Lines.Strings = (
      'KeyIn')
    TabOrder = 0
    OnChange = KeyInChange
  end
  object btnEncipher: TBitBtn
    Left = 472
    Top = 252
    Width = 393
    Height = 25
    Caption = 'Crypt and Save'
    TabOrder = 2
    OnClick = btnEncipherClick
  end
  object btnOpen: TBitBtn
    Left = 472
    Top = 41
    Width = 369
    Height = 101
    Caption = 'Open File'
    TabOrder = 3
    OnClick = btnOpenClick
  end
  object ProgressBar: TProgressBar
    Left = 0
    Top = 304
    Width = 865
    Height = 73
    MarqueeInterval = 1
    Step = 1
    TabOrder = 1
  end
  object Polynom1: TMemo
    Left = 0
    Top = 41
    Width = 433
    Height = 48
    Lines.Strings = (
      'Polynom1')
    TabOrder = 4
  end
  object Polynom2: TMemo
    Left = 0
    Top = 144
    Width = 433
    Height = 48
    Color = cl3DDkShadow
    Lines.Strings = (
      'Polynom2')
    TabOrder = 5
  end
  object Polynom3: TMemo
    Left = 0
    Top = 198
    Width = 433
    Height = 48
    Color = cl3DDkShadow
    Lines.Strings = (
      'Polynom3')
    TabOrder = 6
  end
  object btnAccept: TBitBtn
    Left = 0
    Top = 252
    Width = 433
    Height = 25
    Caption = 'Accept'
    TabOrder = 7
    OnClick = btnAcceptClick
  end
  object btnLabPoly: TBitBtn
    Left = 0
    Top = 10
    Width = 433
    Height = 25
    Caption = 'Set Standart'
    TabOrder = 8
    OnClick = btnLabPolyClick
  end
  object CheckBoxGeffe: TCheckBox
    Left = 8
    Top = 121
    Width = 185
    Height = 17
    Caption = 'Geffe Box'
    TabOrder = 9
    OnClick = CheckBoxGeffeClick
  end
  object RadioButton: TRadioButton
    Left = 847
    Top = 121
    Width = 25
    Height = 17
    Enabled = False
    TabOrder = 10
  end
  object OpenDialog: TOpenDialog
    Left = 440
    Top = 112
  end
  object SaveDialog: TSaveDialog
    Left = 440
    Top = 256
  end
end
