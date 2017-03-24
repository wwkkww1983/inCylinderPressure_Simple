object FormEngine: TFormEngine
  Left = 751
  Top = 31
  Width = 254
  Height = 172
  BorderIcons = []
  Caption = '柴油机整体参数'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 6
    Top = 62
    Width = 101
    Height = 15
    Caption = '转速(r/min)：'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = '宋体'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 6
    Top = 10
    Width = 45
    Height = 15
    Caption = '车型：'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = '宋体'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 6
    Top = 36
    Width = 45
    Height = 15
    Caption = '车号：'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = '宋体'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 6
    Top = 114
    Width = 131
    Height = 15
    Caption = '指示油耗(g/kWh)：'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = '宋体'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 6
    Top = 88
    Width = 107
    Height = 15
    Caption = '指示功率(kW)：'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = '宋体'
    Font.Style = []
    ParentFont = False
  end
  object EditDieselSpeed: TEdit
    Left = 140
    Top = 56
    Width = 100
    Height = 25
    TabStop = False
    Color = clInfoBk
    Font.Charset = ANSI_CHARSET
    Font.Color = clNavy
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = 'EditDieselSpeed'
  end
  object EditTrainType: TEdit
    Left = 140
    Top = 4
    Width = 100
    Height = 25
    TabStop = False
    Color = clInfoBk
    Font.Charset = ANSI_CHARSET
    Font.Color = clNavy
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Text = 'Edit1'
  end
  object EditTrainNumber: TEdit
    Left = 140
    Top = 30
    Width = 100
    Height = 25
    TabStop = False
    Color = clInfoBk
    Font.Charset = ANSI_CHARSET
    Font.Color = clNavy
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Text = 'Edit1'
  end
  object EditPower: TEdit
    Left = 140
    Top = 82
    Width = 100
    Height = 25
    TabStop = False
    Color = clInfoBk
    Font.Charset = ANSI_CHARSET
    Font.Color = clNavy
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    Text = 'Edit1'
  end
  object EditOilConsume: TEdit
    Left = 140
    Top = 108
    Width = 100
    Height = 25
    TabStop = False
    Color = clInfoBk
    Font.Charset = ANSI_CHARSET
    Font.Color = clNavy
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    Text = 'Edit1'
  end
end
