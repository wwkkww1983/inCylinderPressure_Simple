object FormPcpInfo: TFormPcpInfo
  Left = 536
  Top = 121
  Width = 197
  Height = 182
  BorderIcons = []
  Caption = '数据信息'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 14
    Top = 12
    Width = 63
    Height = 13
    Caption = '柴油机转速:'
  end
  object Label2: TLabel
    Left = 14
    Top = 64
    Width = 63
    Height = 13
    Caption = '压力升高率:'
  end
  object Label3: TLabel
    Left = 14
    Top = 38
    Width = 51
    Height = 13
    Caption = '气缸压力:'
  end
  object Label4: TLabel
    Left = 14
    Top = 90
    Width = 63
    Height = 13
    Caption = '供油提前角:'
  end
  object Label5: TLabel
    Left = 14
    Top = 116
    Width = 63
    Height = 13
    Caption = '燃烧开始角:'
  end
  object PanelSpeed: TPanel
    Left = 104
    Top = 6
    Width = 77
    Height = 23
    BevelInner = bvLowered
    Color = clBackground
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
  end
  object PanelDPMax: TPanel
    Left = 104
    Top = 58
    Width = 77
    Height = 23
    BevelInner = bvLowered
    Color = clBackground
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
  end
  object PanelPMax: TPanel
    Left = 104
    Top = 32
    Width = 77
    Height = 23
    BevelInner = bvLowered
    Color = clBackground
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
  end
  object PanelOilAngle: TPanel
    Left = 104
    Top = 84
    Width = 77
    Height = 23
    BevelInner = bvLowered
    Color = clBackground
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
  end
  object PanelCombustionAngle: TPanel
    Left = 104
    Top = 110
    Width = 77
    Height = 23
    BevelInner = bvLowered
    Color = clBackground
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
  end
  object EditOilAngle: TEdit
    Left = 102
    Top = 84
    Width = 79
    Height = 21
    TabOrder = 5
    Text = 'EditOilAngle'
  end
  object EditCombustionAngle: TEdit
    Left = 102
    Top = 110
    Width = 79
    Height = 21
    TabOrder = 6
    Text = 'Edit1'
  end
end
