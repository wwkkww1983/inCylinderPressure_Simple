//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "lPrint.h"
#include "lFormMain.h"
#include "DrawCoor.h"
#include "LPPFDRead.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPrint *FormPrint;
const int IFONTHEIGHT     = -12;
const int IDECREASEHEIGHT = 2;
const int iTopStep        = 20;
//---------------------------------------------------------------------------
__fastcall TFormPrint::TFormPrint(TComponent* Owner)
    : TForm(Owner)
{
    iSizeCoef      = 0;
    bPrint         = false;
    iDrawType      = -1;
    bDrag          = false;
    iscrollPosi    = 0;
    iImagePosi     = 0;
    iIPFactor      = 1;
    fXPPfactor     = 1.0;
    fYPPfactor     = 1.0;
    fFontSize      = 10;
    fFontHeight    = IFONTHEIGHT;
    bFirstPage     = true;
    bListChanged   = true;
    bPrintDrawGrid = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormPrint::SetpsfFrequency(float* fc,int n)
{
    for(int i=0;i<n;i++)
    {
        psfFrequency[i] = fc[i];
    }
}

void __fastcall TFormPrint::SetpsfGConst(float* fc,int n)
{
    for(int i=0;i<n;i++)
    {
        psfGConst[i]=fc[i];
    }
}

void __fastcall TFormPrint::ToolButtonExitClick(TObject *Sender)
{
    ModalResult= mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormPrint::FormCreate(TObject *Sender)
{
    ComboBoxPrintContent->ItemIndex = 0;
    ComboBoxPercent->ItemIndex      = 0;
    
}
//---------------------------------------------------------------------------
void __fastcall TFormPrint::ToolButtonPrinterSetupClick(TObject *Sender)
{
    PrinterSetupDialog1->Execute();    
}
//---------------------------------------------------------------------------


void __fastcall TFormPrint::DrawCurve(void)
{
    //TODO: Add your source code here
    ZoomOutDraw();
    TRect rect;

    ImageMain->Canvas->Brush->Color=clWhite;
    rect=ImageMain->ClientRect;
    ImageMain->Canvas->FillRect(rect);
    ImageBackground->Canvas->Brush->Color=clWhite;
    rect=ImageBackground->ClientRect;
    ImageBackground->Canvas->FillRect(rect);

    rect.Top=rect.Top+5;
    rect.Left=rect.Left+5;
    rect.Bottom=rect.Bottom-5;
    rect.Right=rect.Right-5;

    ImageBackground->Canvas->Pen->Color=clRed;
    ImageBackground->Canvas->Rectangle(rect);
    ImageBackground->Canvas->MoveTo(rect.Left,rect.Top);
    ImageBackground->Canvas->LineTo(rect.Right,rect.Bottom);
    ImageBackground->Canvas->MoveTo(rect.Left,rect.Bottom);
    ImageBackground->Canvas->LineTo(rect.Right,rect.Top);
    ImageBackground->Canvas->Ellipse(rect);
    TRect trect;

    trect=ImageBackground->ClientRect;
    rect=ImageMain->ClientRect;
    ImageMain->Canvas->CopyRect(rect,ImageBackground->Canvas,trect);

}
void __fastcall TFormPrint::FormActivate(TObject *Sender)
{
//    DrawCurve();
    ComboBoxPrintContent->ItemIndex = 1;
    iXStep = iDrawWidth/10;
    iYStep = iDrawHeight/10;
    iWidthBackup  = PanelDraw->Width;
    iHeightBackup = PanelDraw->Height;
    if(bPrintDrawGrid)
    {
        N1->Caption = "关闭绘图网格";
    }
    else
    {
        N1->Caption  = "打开绘图网格";
    }

    iImagePosiMax = ImageMain->Height-(PanelBackground->Height-CoolBar1->Height) + iTopStep;
    ZoomOutDraw();
    switch(iDrawType)
    {
        case 1:
            DrawFirstCCurve();
            break;
        case 2:
            ComboBoxListChange(NULL);
            break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormPrint::ScrollBarDrawChange(TObject *Sender)
{
//    ImageMain->Top=PanelBackground->Top -ScrollBarDraw->Position;
//    ImageRight->Top=  ImageMain->Top+10;
//    ImageBottom->Top= -ImageMain->Top+950;
    PanelDraw->Top=PanelBackground->Top+iTopStep+CoolBar1->Height-ScrollBarDraw->Position;
//    ScrollBarDraw->Position=iscrollPosi;
 //   iscrollPosi=ScrollBarDraw->Position;
//    Label1->Caption=iscrollPosi;
}
//---------------------------------------------------------------------------


void __fastcall TFormPrint::ZoomOutDraw(void)
{
    //TODO: Add your source code here
    TRect rect;
    float fratio;
    fratio = (float)iDrawHeight/(float)iDrawWidth;

    ImageMain->Width = iDrawWidth+iSizeCoef*iXStep;
    ImageMain->Height = (int)((float)ImageMain->Width*fratio);

    PanelDraw->Height = ImageMain->Height+10;
    PanelDraw->Width = ImageMain->Width+10;
    PanelDraw->Top = PanelBackground->Top+CoolBar1->Height+iTopStep;
    PanelDraw->Left = PanelBackground->Left+(PanelBackground->Width-PanelDraw->Width)/2;

//    PanelDraw->Top=Y0;
//    PanelDraw->Left=X0+(iWidth-ImageMain->Width)/2;
    if(PanelDraw->Height-PanelBackground->Height<0)
    {
        ScrollBarDraw->Max=0;
    }
    else
    {
        ScrollBarDraw->Max = PanelDraw->Height-PanelBackground->Height+CoolBar1->Height+100;
    }


//    ImageMain->Height=iHeightBackup+iSizeCoef*iXStep;
//    ImageMain->Width=iWidthBackup+iSizeCoef*iYStep;
    ImageMain->Left = 0;
    ImageMain->Top = 0;

    ImageRight->Height = ImageMain->Height;
    ImageRight->Top = ImageMain->Top+10;
    ImageRight->Left = ImageMain->Left+ImageMain->Width;

    ImageBottom->Top = ImageMain->Top+ImageMain->Height;
    ImageBottom->Width = ImageMain->Width;

    TRect trect;
    trect = ImageBackground->ClientRect;
    rect = ImageMain->ClientRect;
    ImageMain->Canvas->CopyRect(rect,ImageBackground->Canvas,trect);

//    ImageMain->Canvas->Brush->Color=clWhite;
//    rect=ImageMain->ClientRect;
//    ImageMain->Canvas->FillRect(rect);

    ImageRight->Canvas->Brush->Color = (TColor)RGB(70,70,70);//clBlack;
    rect = ImageRight->ClientRect;
    ImageRight->Canvas->FillRect(rect);

    ImageBottom->Canvas->Brush->Color = (TColor)RGB(70,70,70); //clBlack;
    rect = ImageBottom->ClientRect;
    ImageBottom->Canvas->FillRect(rect);
    iImagePosiMax = PanelDraw->Height-(PanelBackground->Height-CoolBar1->Height);
    if(iImagePosiMax<0)
    {
        iImagePosiMax=0;
    }
}
void __fastcall TFormPrint::ToolButtonZoomInClick(TObject *Sender)
{
     if(iSizeCoef == 0) return;
     iSizeCoef++;
     ComboBoxPercent->ItemIndex = -iSizeCoef;
     iImagePosi = 0;
     ZoomOutDraw();

}
//---------------------------------------------------------------------------

void __fastcall TFormPrint::ToolButtonZoomOutClick(TObject *Sender)
{
      if(iSizeCoef == -8) return;
      iSizeCoef --;
      ComboBoxPercent->ItemIndex = -iSizeCoef;
      iImagePosi = 0;
      ZoomOutDraw();
}
//---------------------------------------------------------------------------

void __fastcall TFormPrint::ComboBoxPercentChange(TObject *Sender)
{
     iSizeCoef = -ComboBoxPercent->ItemIndex;
     ZoomOutDraw();
}
//---------------------------------------------------------------------------


void __fastcall TFormPrint::DrawContent(TObject* Sender,TRect& rect)
{
    //TODO: Add your source code here
//    ClearBackground(Sender,rect); //将背景画成白色
    switch(iDrawType)
    {
        case 0:
            DrawResultText(Sender,rect);
            break;
        case 1:
            DrawCurrentCurve(Sender,rect);
            break;
        case 2:
            try
            {
                DrawListCurve(Sender,rect);
            }
            catch(...)
            {
            }
            break;
    }
    if(bPrint==false)   //如果不是打印状态，那么就绘图结果显示到屏幕上。
    {
        ZoomOutDraw();
    }
}
void __fastcall TFormPrint::ToolButtonPrintClick(TObject *Sender)
{
    bPrint = true;
    iIPFactor=-1;
    TRect rect;
    if(PrintDialog1->Execute())
    {

        Prntr = Printer();
        int iXFBL,iYFBL;         // 象素/英寸 DPI
        int iPageWidth,iPageHeight; //象素/页宽  或  象素/页高
        float fWidthInch,fHeightInch;

//-------------------------------------------------------------------------
        Prntr->BeginDoc();
//-------------------------------------------------------------------------
//获取X方向和Y方向的每英吋象素点数,将其转换为每毫米的点数
        iXFBL = GetDeviceCaps(Prntr->Handle,LOGPIXELSX);
        iYFBL = GetDeviceCaps(Prntr->Handle,LOGPIXELSY);

        iPageWidth = GetDeviceCaps(Prntr->Handle,PHYSICALWIDTH);
        iPageHeight = GetDeviceCaps(Prntr->Handle,PHYSICALHEIGHT);

        FormMain->iPrintHeight = iPrintHeight;

        fWidthInch = (float)iPageWidth/(float)iXFBL*25.4;
        fHeightInch = (float)iPageHeight/(float)iYFBL*25.4;

        rect.Top = 0;
        rect.Left = 0;
        rect.Right = (int)(fWidthInch*10);
//        rect.Bottom = -(int)(fHeightInch*100);
        rect.Bottom = -(int)(fHeightInch*10);
//        fPrintRatio = (float)rect.Right/650.0;
//        ::SetMapMode(Prntr->Handle,MM_LOENGLISH);
        ::SetMapMode(Prntr->Handle,MM_LOMETRIC);
//        Prntr->Canvas->Font->Size=1.5;
//*************************************************************************
        //Set your Printing Code here.
        //向打印机上输出图形
        PrintContent(Prntr,rect);

//*************************************************************************
//      Prntr->Canvas->MoveTo(0,0);
//      Prntr->Canvas->LineTo(100,-100);
//      Prntr->Canvas->MoveTo(200,-200);
///     Prntr->Canvas->LineTo(400,-200);

//------------------------------------------------------------------------
        Prntr->EndDoc();
//------------------------------------------------------------------------
    }
    bPrint = false;
    iIPFactor = 1;
}
//---------------------------------------------------------------------------


void __fastcall TFormPrint::DrawResultText(TObject* Sender,TRect& rect)
{
//先关闭此项打印功能，待调通后再解决它

    //TODO: Add your source code here
/*
    if(!FormMain->bAnalysisResult) return;
    int iItem;
    AnsiString strFileTemp;
    iItem = ComboBoxList->ItemIndex;
//    if(iItem!=iLibPosi)
//    {
//        Application->MessageBox("请先进行分析，然后打印!","提示",MB_OK|MB_ICONINFORMATION);
//        return;
//    }
    TCanvas * myCanvas;

    if(bPrint)
    {
        myCanvas=(TCanvas*)(((TPrinter*) Sender)->Canvas);
    }
    else
    {
        myCanvas=(TCanvas*)(((TImage*) Sender)->Canvas);
    }
    strPersonName = FormMain->ComboBoxP1Personnel->Text;
    DrawTitle(myCanvas,rect);
    if(bListChanged) return;
    if(bPrint)
//        myCanvas->Font->Size=fFontSize;
    {
        myCanvas->Font->Height=fFontHeight;
    }
    int iwidth,iallwidth;
    int iPrinter=1;
    if(bPrint)
    {
         iPrinter=-1;
         myCanvas->Font->Height=fFontHeight;
    }
    else
    {
         myCanvas->Font->Size=fFontSize;
    }
    AnsiString strTemp;
    TRect nowrect;
    nowrect=rect;
    int x,y,istep,iSmallStep;
    int iHeight=myCanvas->TextHeight("柴油机");
    istep=iPrinter*iHeight*5/4;
    iSmallStep=iPrinter*iHeight/4;
    x=nowrect.Left;
    y=nowrect.Top+iHeight;
    myCanvas->MoveTo(nowrect.Left,y);
    myCanvas->LineTo(nowrect.Right,y);
    y+=istep;
    myCanvas->TextOut(x,y,"缸号　波形峰值　持续时间　波形系数　结论");
    y+=istep;
    myCanvas->MoveTo(nowrect.Left,y);
    myCanvas->LineTo(nowrect.Right,y);
    AnsiString strTLL;
    int iposi,ixstep;
    for(int i=0;i<FormMain->ilDieselCount;i++)
    {
        if(i==0)
        {
            strTemp.printf(" %02d　　%.2f　　  %.2f　　  %.2f　　",
                        FormMain->myWaveInfo[i].ilDieselNum,
                        FormMain->myWaveInfo[i].flWavePeak,
                        FormMain->myWaveInfo[i].flWavePeriod,
                        FormMain->myWaveInfo[i].flWaveFormCoef
                        );

                ixstep=myCanvas->TextWidth(strTemp);
        }
        bool bexist=FormMain->myWaveInfo[i].flWavePeak<0.002;
        bool bother=FormMain->myWaveInfo[i].flWavePeriod<0.002;
        bexist=bexist&&bother;
        bother=FormMain->myWaveInfo[i].flWavePeakCoef<0.002;
        bexist=bexist&&bother;
        if(bexist) continue;
        y+=istep;
        strTemp=FormMain->myWaveInfo[i].strResult;
        iposi=strTemp.Pos("* *");
        if(iposi>0)
        {
            strTLL=strTemp.SubString(1,iposi);
            strTemp.printf(" %02d　　%.2f　　  %.2f　　  %.2f　　%s",
                        FormMain->myWaveInfo[i].ilDieselNum,
                        FormMain->myWaveInfo[i].flWavePeak,
                        FormMain->myWaveInfo[i].flWavePeriod,
                        FormMain->myWaveInfo[i].flWaveFormCoef,
                        strTLL.c_str()
                        );
            myCanvas->TextOut(x,y,strTemp);

            strTemp=FormMain->myWaveInfo[i].strResult;
            strTLL=strTemp.SubString(iposi+1,strTemp.Length()-iposi);
            iposi=strTLL.Pos("* *");
            y+=istep;
            if(iposi>0)
            {
                 strTemp=strTLL.SubString(1,iposi);
                 myCanvas->TextOut(x+ixstep,y,strTemp);
                 strTemp=strTLL.SubString(iposi+1,strTLL.Length()-iposi);
                 y+=istep;
                 myCanvas->TextOut(x+ixstep,y,strTemp);
            }
            else
                myCanvas->TextOut(x+ixstep,y,strTLL);
        }
        else
        {
            strTemp.printf(" %02d　　%.2f　　  %.2f　　  %.2f　　%s",
                        FormMain->myWaveInfo[i].ilDieselNum,
                        FormMain->myWaveInfo[i].flWavePeak,
                        FormMain->myWaveInfo[i].flWavePeriod,
                        FormMain->myWaveInfo[i].flWaveFormCoef,
                        FormMain->myWaveInfo[i].strResult.c_str()
                        );
            myCanvas->TextOut(x,y,strTemp);
        }
    }
    y+=istep;
    myCanvas->MoveTo(nowrect.Left,y);
    myCanvas->LineTo(nowrect.Right,y);

    StatusBar1->Panels->Items[0]->Text="";
    StatusBar1->Refresh();
  */
}

void __fastcall TFormPrint::DrawCurrentCurve(TObject* Sender,TRect& rect)
{
    //TODO: Add your source code here
/*
    int icol,irow;
    float ftemp,fTimeInterval;
    int xnum,ynum,ipart,xstep,ystep;
    TRect rectB;

    int iItem=ComboBoxFile->ItemIndex;
    FormMain->strFileName=ComboBoxFile->Items->Strings[iItem];
    if(!FormMain->ReadPipeData(false))
        return;

    TCanvas * myCanvas;
    if(bPrint)
        myCanvas=(TCanvas*)(((TPrinter*) Sender)->Canvas);
    else
        myCanvas=(TCanvas*)(((TImage*) Sender)->Canvas);
    strPersonName = FormMain->LFHS.strPersonnel;
    DrawTitle(myCanvas,rect);
    AnsiString strFileTemp;
    rectB=rect;
    rectB.Left=rect.Left+20;
    rectB.Top=60*fYPPfactor;
    rectB.Right=rect.Right-20;
    rectB.Bottom=rect.Bottom*2/5;
    LDrawCurve * ldc;
    ldc=new LDrawCurve;
    fTimeInterval=1.0/(FormMain->LFHS.siFrequency);
    ldc->SetsgfFrequency(psfFrequency,22);
    ldc->SetsfGConst(psfGConst,9);
    ldc->SetsbGaugeAdjust(FormMain->bGaugeAdjust);
    ldc->SetsfPeakLevel(FormMain->fPeakLevel);
    ldc->SetDrawCoef(true,true);
    if(bPrint)
        ldc->SetiDeviceType(0);  //向打印机对象中画图
    else
        ldc->SetiDeviceType(2); //向TImage对象中画图
    ldc->SetbDrawText(false);
    ldc->SetbShowTitle(false);
    ldc->SetbMinZero(true);
    ldc->SetXYLabel("采样时间 /ms","转换电压 /V");
    ldc->SetfMMValue(FormMain->fYMin,FormMain->fYMax);
    ldc->SetXYPrecision(2,2);
    ldc->SetColor(clBlack,clBlue,clBlack);
    ldc->SetDrawRect(rectB);
    ldc->SetbDrawGrid(bPrintDrawGrid);
    ldc->SetbFromBegining(false);
    ldc->SetData(fTimeInterval,FormMain->fYPipeData,FormMain->iChoiceStart,FormMain->iChoiceEnd);
    ldc->LDraw(Sender);
//    ldc->GetXYStep(fXStep,fYStep);
//    ldc->GetiYMiddle(iMiddle);
    ldc->GetRect(rect);
    delete ldc;

   if(bPrint)
        myCanvas->Font->Height=fFontHeight;
   int iwidth,iallwidth;
   int iPrinter=1;
   if(bPrint)
   {
        iPrinter=-1;
        myCanvas->Font->Height=fFontHeight;
   }
   else
   {
        myCanvas->Font->Size=fFontSize;
   }

   int ilen,x0,y0,iposi,istepy,istepx;
   AnsiString strTrainType;
   istepx=myCanvas->TextHeight("东风")+4;
   istepy=istepx *fYPPfactor;
   switch(FormMain->LFHS.siTrainType)
   {
        case 0:
            strTrainType="东风4A";
            break;
        case 1:
            strTrainType="东风4B";
            break;
        case 2:
            strTrainType="东风4C";
            break;
        case 3:
            strTrainType="东风4D";
            break;
        case 4:
            strTrainType="东风4D";
            break;
        case 5:
            strTrainType="东风7C";
            break;
        case 6:
            strTrainType="东风8B";
            break;
        case 7:
            strTrainType="东风11";
            break;
        default:
            strTrainType="车型未知";
            break;
   }

   iposi=0;
   x0=rectB.Left+40*fXPPfactor;
   y0=rectB.Bottom+10*fYPPfactor;
   strFileTemp="机车车型: ";
   strFileTemp+=strTrainType;
   ilen=myCanvas->TextWidth(strFileTemp);
   if(ilen>iposi)
    iposi=ilen;
   myCanvas->TextOut(x0,y0,strFileTemp);
   strFileTemp = FormMain->LFHS.strTrainNum;
   strFileTemp = strFileTemp.SubString(1,4);
   strFileTemp = "机车车号: "+strFileTemp;
//   strFileTemp.printf("机车车号: %s",FormMain->LFHS.strTrainNum);
   ilen=myCanvas->TextWidth(strFileTemp);
   if(ilen>iposi)
    iposi=ilen;
   myCanvas->TextOut(x0,y0+istepy,strFileTemp);

   strFileTemp.printf("放大倍数: %.1f",(1.0+(float)FormMain->LFHS.siGauge/5.0));
   ilen=myCanvas->TextWidth(strFileTemp);
   if(ilen>iposi)
    iposi=ilen;
   ilen=iposi;
   myCanvas->TextOut(x0,y0+2*istepy,strFileTemp);
   strFileTemp=FormMain->myWaveInfo[31].strResult;
   iposi=strFileTemp.Pos("* *");
    AnsiString strTLL;
       if(iposi>0)
       {
           
            strTLL=strFileTemp.SubString(1,iposi);
            strTLL=AnsiString("结论：")+strTLL;
            myCanvas->TextOut(x0,y0+3*istepy,strTLL);
            strTLL=strFileTemp.SubString(iposi+1,strFileTemp.Length()-iposi);
            iposi=strTLL.Pos("* *");
            int iist=myCanvas->TextWidth("结论：");
            if(iposi>0)
            {
                 strFileTemp=strTLL.SubString(1,iposi);
                 myCanvas->TextOut(x0+iist,y0+4*istepy,strFileTemp);
                 strFileTemp=strTLL.SubString(iposi+1,strTLL.Length()-iposi);
                 myCanvas->TextOut(x0+iist,y0+5*istepy,strFileTemp);
            }
            else
            {
                myCanvas->TextOut(x0+iist,y0+4*istepy,strTLL);
            }
        }
        else
        {
            strFileTemp.printf("结论：%s",FormMain->myWaveInfo[31].strResult.c_str());
            myCanvas->TextOut(x0,y0+3*istepy,strFileTemp);
        }

   x0+=ilen+istepx*fXPPfactor;
   iposi=0;

   strFileTemp.printf("柴油机缸号: %d",FormMain->LFHS.siDieselNum);
   ilen=myCanvas->TextWidth(strFileTemp);
   if(ilen>iposi)
    iposi=ilen;
   myCanvas->TextOut(x0,y0,strFileTemp);

   strFileTemp.printf("柴油机转速: %d r/min",FormMain->LFHS.siSSpeed);
//   strFileTemp.printf("柴油机转速: 430 r/min");
   ilen=myCanvas->TextWidth(strFileTemp);
   if(ilen>iposi)
    iposi=ilen;
   myCanvas->TextOut(x0,y0+istepy,strFileTemp);

   strFileTemp.printf("测量时间:   %d年%02d月%02d日",
                                FormMain->LFHS.siSYear,
                                FormMain->LFHS.siSMonth,
                                FormMain->LFHS.siSDay);
   ilen=myCanvas->TextWidth(strFileTemp);
   if(ilen>iposi)
    iposi=ilen;
   myCanvas->TextOut(x0,y0+2*istepy,strFileTemp);

   x0+=iposi+istepx*fXPPfactor;

   strFileTemp.printf("波形峰值: %.2f",FormMain->fWavePeak);
   myCanvas->TextOut(x0,y0,strFileTemp);

   strFileTemp.printf("持续时间: %.2f",FormMain->fWavePeriod);
   myCanvas->TextOut(x0,y0+istepy,strFileTemp);

   strFileTemp.printf("波形系数: %.2f",FormMain->fWaveFormCoef);
   myCanvas->TextOut(x0,y0+2*istepy,strFileTemp);
*/
}

void __fastcall TFormPrint::ComboBoxPrintContentChange(TObject *Sender)
{
    TRect rect;
    AnsiString strTemp;
    bPrint        = false;
    bool bSuccess = true;
    TCursor oldCursor;
    oldCursor      = Screen->Cursor;
    Screen->Cursor = crHourGlass;
    Application->ProcessMessages();
    if(iDrawType == 2)
    {
        int iCount = ComboBoxList->Items->Count;
        if((iCount <= 0))
        {
            return;
        }
    }
    int iIndex = ComboBoxPrintContent->ItemIndex;
    switch(iIndex)
    {
        case 0:
        {
            FormMain->AllDrawKind = drawOriginal;  //原始数据
            break;
        }
        case 1:
        {
            FormMain->AllDrawKind = drawPT;        //修改数据
            break;
        }
        case 2:
        {
            FormMain->AllDrawKind = drawPV;        //容积压力图
            break;
        }
        case 3:
        {
            FormMain->AllDrawKind = drawLogVLogP;  //容积压力对数图
            break;
        }
        case 4:
        {
            FormMain->AllDrawKind = drawDPDF;     //压力升高率
            break;
        }
        case 5:
        {
            FormMain->AllDrawKind = drawHRR;      //放热率
            break;
        }
        case 6:
        {
            FormMain->AllDrawKind = drawCHRR;     //累积放热率
            break;
        }
        case 7:
        {
            FormMain->AllDrawKind = drawFFTP;     //压力频谱
            break;
        }
/*
        case 8:
        {
            FormMain->AllDrawKind = drawTT;       //气缸温度
            break;
        }
*/
        case 8:
        {
            FormMain->AllDrawKind = drawDW;      //画瞬时功
            break;
        }
        case 9:
        {
            FormMain->AllDrawKind = drawW;    //画累积功
            break;
        }

        case 10:
        {
            FormMain->AllDrawKind = drawMAX;      //画各缸最大值
            break;
        }
        case 11:
        {
            FormMain->AllDrawKind = drawPWave;    //画油管压力波
            break;
        }
        case 12:
        {
            FormMain->AllDrawKind = drawInfo;    //打印基本参数
            break;
        }
        case 13:
        {
            FormMain->AllDrawKind = drawPWaveResult;  //打印压力波分析结论
            break;
        }
    }
    try
    {
        if( bSuccess )
        {
            DrawNewListCurve(ImageBackground,rect);
        }
    }
    __finally
    {
        Screen->Cursor=oldCursor;
        Application->ProcessMessages();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormPrint::ComboBoxListChange(TObject *Sender)
{
    TRect rect;
    iDrawType = 2;
    bListChanged = true;
    AnsiString strT;
    int iIndex;
    iIndex = ComboBoxList->ItemIndex;
    if(iIndex<0)
    {
        return;
    }
    strT = ComboBoxList->Items->Strings[iIndex];
    if(FileExists(strT))
    {
        bPrint = false;   //此处是屏幕输出图形
//        FormMain->AllDrawKind = drawPT;
        ComboBoxFile->Items->LoadFromFile(strT);
        FormMain->strPSTName = strT;
        rect = ImageBackground->ClientRect;
        fXPPfactor = 1.0;
        fYPPfactor = 1.0;
        DrawNewListCurve(ImageBackground,rect);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormPrint::ComboBoxFileChange(TObject *Sender)
{
    bPrint=false;
    TRect rect;
    AnsiString strTT;
    ComboBoxPrintContent->ItemIndex=-1;
    int iIndex;
    iIndex = ComboBoxFile->ItemIndex;
    if(iIndex<0)
    {
        return;
    }
    iDrawType = 1;
    strTT = ComboBoxFile->Items->Strings[iIndex];
    if(FileExists(strTT))
    {
        FormMain->strPCPName = strTT;
        rect=ImageBackground->ClientRect;
//        AdjustDrawRect(rect);
        fXPPfactor = 1.0;
        fYPPfactor = 1.0;
        DrawNewCurrentCurve(ImageBackground,rect);
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormPrint::ClearBackground(TObject* Sender, TRect& rect)
{
    //TODO: Add your source code here
    TCanvas * myCanvas;

    if(bPrint)
    {
        myCanvas=(TCanvas*)(((TPrinter*) Sender)->Canvas);
    }
    else
    {
        myCanvas=(TCanvas*)(((TImage*) Sender)->Canvas);
    }
    myCanvas->Brush->Color=clWhite;
    myCanvas->FillRect(rect);
}

void __fastcall TFormPrint::DrawListCurve(TObject* Sender, TRect& rect)
{
    //TODO: Add your source code here

        int icol,irow;
        int iDieselNum;
        float fPeriod;
        int iChoicedNum;
        int xnum,ynum,ipart,xstep,ystep;
        bool bOut=false;
//        char charDriver;
        AnsiString strTemp;
//清除画面
        TCanvas * myCanvas;
        if(bPrint)
        {
            myCanvas=(TCanvas*)(((TPrinter*) Sender)->Canvas);
        }
        else
        {
            myCanvas=(TCanvas*)(((TImage*) Sender)->Canvas);
        }
        strPersonName = FormMain->ComboBoxPersonnel->Text;
        DrawTitle(myCanvas,rect);

//        AnsiString strFileTemp;
        iChoicedNum = ComboBoxList->ItemIndex;
        strTemp     = ComboBoxList->Items->Strings[iChoicedNum];
        charDriver  = strTemp.c_str()[0];
        AnsiString strpath,strTL;
// strTL=::ExtractFileDrive(LoadFileName);
//首先判断文件的存在位置
        ListBoxBuffer->Items->Clear();
        ListBoxBuffer->Items->LoadFromFile(strTemp);
        int iCount    = ListBoxBuffer->Items->Count;
        int iFilePosi = 0;
        strTL         = ListBoxBuffer->Items->Strings[0];
        if(!FileExists(strTL))
        {
            strpath          = ::ExtractFilePath(strTemp);
            strTL.c_str()[0] = strpath.c_str()[0];
            if(!FileExists(strTL))
            {
                iFilePosi = 2;
            }
            else
            {
                iFilePosi = 1;      //在路径基本一样，就是磁盘符不是
            }
        }
        else
        {
            iFilePosi = 0;         //路径文件正确
        }

        if(iFilePosi != 0)
        {
            for(int ii=0; ii<iCount; ii++)
            {
                if(iFilePosi == 2)
                {
                    strTL = ::ExtractFilePath(strTemp);
                }
                else
                {
                    strTL = ::ExtractFilePath(ListBoxBuffer->Items->Strings[ii]);
                    strTL.c_str()[0] = charDriver;
                }
                strpath = ::ExtractFileName(ListBoxBuffer->Items->Strings[ii]);
                ListBoxBuffer->Items->Delete(ii);
                strTL += strpath;
                ListBoxBuffer->Items->Insert(ii,strTL);
            }
        }

        iChoicedNum    = ListBoxBuffer->Items->Count;
        if(iChoicedNum == 0)
        {
             return;   //如果没有列表则退出
        }
        iChoicedNum = 16;
        if(FormMain->bDrawStyle)
        {
//                ynum = iChoicedNum/2;
//                ipart = iChoicedNum%2;
                ynum = 8;
                xnum = 2;
        }
        else
        {
                xnum = 1;
                ynum = 16;
        }
        iChoicedNum = ListBoxBuffer->Items->Count;
        TRect nowrect;

/*
        rect.Left=PaintBoxAll->Left;
        rect.Top=PaintBoxAll->Top;
        rect.Bottom=PaintBoxAll->Top+iMainPrnSign*(PaintBoxAll->Height);
        rect.Right=PaintBoxAll->Left+PaintBoxAll->Width;
*/
        xstep = ( rect.Right  - rect.Left ) / xnum;
        ystep = ( rect.Bottom - rect.Top  ) / ynum;

        LDrawCurve *lldc;
        lldc=new LDrawCurve;
//        lldc->SetsbC(bpCPeak,bpCPeriod,bpCCoef);
//        lldc->SetsgfFrequency(psfFrequency,22);
//        lldc->SetsfGConst(psfGConst,9);
//        lldc->SetsbGaugeAdjust(FormMain->bGaugeAdjust);
//        lldc->SetsfPeakLevel(FormMain->fPeakLevel);
        lldc->SetDrawCoef(false,false);
        if(bPrint)
        {
            lldc->SetiDeviceType(0);
        }
        else
        {
            lldc->SetiDeviceType(2) ;  // TImage  bPBorPrn(bMainPBorPrn);
        }

//        lldc->SetiDeviceType(2);  //PaintBox (bMainPBorPrn);
        lldc->SetbDrawText(false);
        lldc->SetbMinZero(true);
        lldc->SetColor(clBlack,clWhite,clBlack);
        lldc->SetiLabelFormat(2);  //只显示缸号
        lldc->SetbShowInfo(true);
//        lldc->SetfParams(FormMain->fLowPeak,FormMain->fHighPeak,
//                         FormMain->fLowTime,FormMain->fHighTime ,
//                         FormMain->fLowPeakCoef,FormMain->fHighPeakCoef,
//                         FormMain->fLowLastFormCoef);
        ipart=0;
        for(irow=0;irow<ynum;irow++)
        {
                for(icol=0;icol<xnum;icol++)
                {
                        nowrect.Left=rect.Left+icol*xstep;
                        nowrect.Top=rect.Top+irow*ystep;
                        nowrect.Bottom=nowrect.Top+ystep;
                        nowrect.Right=nowrect.Left+xstep;
                        lldc->SetbShowTitle(true);
                   //     SetTrainInfo(char* trainnum,int dieselnum);
                        lldc->SetDrawRect(nowrect);

                        lldc->SetfMMValue(0.0,4.1);
                        strTemp=ListBoxBuffer->Items->Strings[ipart];
                     //   strTemp.c_str()[0]=charDriver;
                        if(!FileExists(strTemp))
                        {
                         //   Application->MessageBox("列表内文件的路径不对！","提示",MB_OK|MB_ICONINFORMATION);
                            ipart++;
                            if(ipart==iChoicedNum)
                            {
                                bOut=true;
                                break;
                            }
                            continue;
                        }
                        lldc->ReadAllData(strTemp);
//                        ldc->GetPeriod(iDieselNum,fPeriod);
//                        if(iDieselNum<10)
//                            strTTemp.printf("  0%d           %.2f",iDieselNum,fPeriod);
//                        else
//                            strTTemp.printf("  %d           %.2f",iDieselNum,fPeriod);
//                        MmoResult->Lines->Add(strTTemp);
                        lldc->LDraw(Sender);
                        ipart++;
                        if(ipart==iChoicedNum)
                        {
                                bOut=true;
                                break;
                        }
                }
                if(bOut) break;
        }
        delete lldc;
}


void __fastcall TFormPrint::DrawCompareCurve(TObject* Sender, TRect& rect)
{
    //TODO: Add your source code here

}

void __fastcall TFormPrint::DrawResultCurve(TObject* Sender, TRect& rect)
{
    //TODO: Add your source code here
    if(!FormMain->bPSTOpened) return;
    int iItem;
    AnsiString strFileTemp;
/*
    iItem=ComboBoxList->ItemIndex;
    if(iItem!=iLibPosi)
    {
        Application->MessageBox("请先进行分析，然后打印!","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }
*/
    AnsiString strTemp;
//清除画面
    TCanvas * myCanvas;
    if(bPrint)
        myCanvas=(TCanvas*)(((TPrinter*) Sender)->Canvas);
    else
           myCanvas=(TCanvas*)(((TImage*) Sender)->Canvas);
    strPersonName = FormMain->ComboBoxPersonnel->Text;
    DrawTitle(myCanvas,rect);
/*
    myCanvas->Brush->Color=clWhite;
    myCanvas->FillRect(rect);

//写上标题

    myCanvas->Font->Size=16;
    myCanvas->TextOut(30,30,FormMain->strUserTitle);
    myCanvas->MoveTo(30,100);
    myCanvas->LineTo(530,100);
*/

//    strTemp=ComboBoxList->Items->Strings[iItem];
//    charDriver=strTemp.c_str()[0];
//    ListBoxBuffer->Items->LoadFromFile(strTemp);

    int icol,irow;
    int iDieselNum;
    float fPeriod;

    int xnum,ynum,ipart,xstep,ystep;
    xnum=1;
    int iitemp=ListBoxBuffer->Items->Count;
    if(iitemp<=8) //如果波形较少，就以单页打印
    {
        ipart=0;
        ynum=iitemp;
    }
    else
    {
        if(bFirstPage)
        {
            ynum=iitemp/2+iitemp%2;
            ipart=0;
        }
        else
        {
            ynum=iitemp/2;
            ipart=iitemp/2+iitemp%2;
        }
    }
    TRect rectB,nowrect;
    rectB=rect;
    rectB.Right=(rect.Left+rect.Right)/2;
    xstep=(rectB.Right-rectB.Left)/xnum;
    ystep=(rectB.Bottom-rectB.Top)/ynum;
    AnsiString strTTemp;
    LDrawCurve *lldc;
    lldc=new LDrawCurve;
    lldc->SetsbC(bpCPeak,bpCPeriod,bpCCoef);
    lldc->SetsgfFrequency(psfFrequency,22);
    lldc->SetsfGConst(psfGConst,9);
//    lldc->SetsbGaugeAdjust(FormMain->bGaugeAdjust);
//    lldc->SetsfPeakLevel(FormMain->fPeakLevel);
    lldc->SetDrawCoef(false,false);
    if(bPrint)
        lldc->SetiDeviceType(0);
    else
        lldc->SetiDeviceType(2) ;  // TImage  bPBorPrn(bMainPBorPrn);

//    lldc->SetiDeviceType(2) ;  // TImage  bPBorPrn(bMainPBorPrn);
    lldc->SetbDrawText(false);
    lldc->SetbMinZero(true);
    lldc->SetColor(clBlack,clWhite,clBlack);
    //ipart=0;
    for(icol=ipart;icol<ipart+ynum;icol++)
    {
        nowrect.Left=rectB.Left;
        nowrect.Top=rectB.Top+(icol-ipart)*ystep;
        nowrect.Bottom=nowrect.Top+ystep;
        nowrect.Right=nowrect.Left+xstep;
        lldc->SetbShowTitle(true);
        lldc->SetDrawRect(nowrect);
        lldc->SetfMMValue(0.0,5.0);
        strFileTemp=ListBoxBuffer->Items->Strings[icol];
//        strFileTemp.c_str()[0]=charDriver;
        if(!FileExists(strFileTemp))
        {
//             Application->MessageBox("列表内文件的路径不对！","提示",MB_OK|MB_ICONINFORMATION);
//             delete lldc;
//             return;
            continue;
        }
        lldc->SetiLabelFormat(2);    //1：显示车号和缸号   2：缸号
        lldc->ReadAllData(strFileTemp);
        lldc->LDraw(Sender);
   }
   delete lldc;

   rectB.Left=rectB.Right+5;
   rectB.Right=rect.Right;
   if(bPrint)
//        myCanvas->Font->Size=fFontSize;
        myCanvas->Font->Height=fFontHeight;
   int iwidth,iallwidth;
//   int iPrinter=1;
   if(bPrint)
   {
//        iPrinter=-1;
        myCanvas->Font->Height=fFontHeight;
   }
   else
   {
        myCanvas->Font->Size=fFontSize;
   }
   for(icol=ipart;icol<ipart+ynum;icol++)
   {
//        bool bexist=FormMain->myWaveInfo[icol].flWavePeak<0.002;
//        bool bother=FormMain->myWaveInfo[icol].flWavePeriod<0.002;
//        bexist=bexist&&bother;
//        bother=FormMain->myWaveInfo[icol].flWavePeakCoef<0.002;
//        bexist=bexist&&bother;
//        if(bexist) continue;
        nowrect.Left=rectB.Left;
        nowrect.Top=rectB.Top+(icol-ipart)*ystep+5;
/*
        strFileTemp.printf("峰值系数:%.2f",FormMain->myWaveInfo[icol].flWavePeak);
        myCanvas->TextOut(nowrect.Left,nowrect.Top+5*iPrinter,strFileTemp);
        iallwidth=nowrect.Left;
        iwidth=myCanvas->TextWidth(strFileTemp)+10;
        iallwidth+=iwidth;
        strFileTemp.printf("时间系数:%.2f",FormMain->myWaveInfo[icol].flWavePeriod);
        myCanvas->TextOut(iallwidth,nowrect.Top+5*iPrinter,strFileTemp);
        iwidth=myCanvas->TextWidth(strFileTemp)+10;
        iallwidth+=iwidth;
        strFileTemp.printf("波形系数:%.2f",FormMain->myWaveInfo[icol].flWavePeakCoef);
        myCanvas->TextOut(iallwidth,nowrect.Top+5*iPrinter,strFileTemp);
*/

//        strFileTemp.printf("波形峰值:%.2f",
//                               FormMain->myWaveInfo[icol].flWavePeak
//                           );
//        myCanvas->TextOut(nowrect.Left,nowrect.Top+2*iPrinter,strFileTemp);

//        strFileTemp.printf("持续时间:%.2f",
//                                FormMain->myWaveInfo[icol].flWavePeriod);
//        iwidth=myCanvas->TextHeight(strFileTemp)+2;
//        myCanvas->TextOut(nowrect.Left,nowrect.Top+(2+iwidth)*iPrinter,strFileTemp);
//        iwidth+=iwidth;
//        strFileTemp.printf("波形系数:%.2f",FormMain->myWaveInfo[icol].flWaveFormCoef);
//        myCanvas->TextOut(nowrect.Left,nowrect.Top+(2+iwidth)*iPrinter,strFileTemp);

//        strFileTemp.printf("结论: ");
//        iwidth=(myCanvas->TextHeight("波形")+2);
//        iallwidth=2*iwidth+4;
//        strFileTemp+=FormMain->myWaveInfo[icol].strResult;
//        int iposi=strFileTemp.Pos("* *");
//        if(iposi>0)
//        {
//            AnsiString strTLL;
//            strTLL=strFileTemp.SubString(1,iposi);
 //           myCanvas->TextOut(nowrect.Left,nowrect.Top+iPrinter*(iwidth+iallwidth),strTLL);
//            strTLL=strFileTemp.SubString(iposi+1,strFileTemp.Length()-iposi);
//            iposi=strTLL.Pos("* *");
//            if(iposi>0)
//            {
//                 strFileTemp=strTLL.SubString(1,iposi);
//                 myCanvas->TextOut(nowrect.Left+myCanvas->TextWidth("结论: "),
//                                    nowrect.Top+iPrinter*(2*iwidth+iallwidth),strFileTemp);
 //                strFileTemp=strTLL.SubString(iposi+1,strTLL.Length()-iposi);
//                 myCanvas->TextOut(nowrect.Left+myCanvas->TextWidth("结论: "),
//                                    nowrect.Top+iPrinter*(3*iwidth+iallwidth),strFileTemp);
//            }
//            else
//                myCanvas->TextOut(nowrect.Left+myCanvas->TextWidth("结论: "),
//                                nowrect.Top+iPrinter*(2*iwidth+iallwidth),strTLL);
//        }
//        else
//           myCanvas->TextOut(nowrect.Left,nowrect.Top+iPrinter*(iwidth+iallwidth),strFileTemp);
    }
//    FormMain->bAnalysisResult=bAnalysisResultBackup;
//    bListChanged=false;
//    StatusBar1->Panels->Items[0]->Text="";
//    StatusBar1->Refresh();

}


void __fastcall TFormPrint::DrawTitle(TCanvas * myCanvas,TRect& rect)
{
    //TODO: Add your source code here
//    cPPWFDRead myReader;
    AnsiString strTemp;
    TRect rectback;
    rectback     = rect;
    rectback.Top = 0;
    myCanvas->Brush->Color = clWhite;
    myCanvas->FillRect(rectback);
    //读出文件头信息    绘图之前以读过了
//    myReader.ReadPipeFileHead(FormMain->LFHS);
    //写上标题
    AnsiString strMyTitle = "       机车柴油机燃烧系统检测结果";
    strMyTitle            = FormMain->strUserTitle + strMyTitle;
    myCanvas->Font->Name  = "隶书";
    myCanvas->Font->Style = TFontStyles();
    if(!bPrint)
    {
        myCanvas->Font->Height = fFontHeight;
    }
    else
    {
        myCanvas->Font->Height = (fFontHeight + IDECREASEHEIGHT)*fXPPfactor;
/*
//------------------------------------------------------------------------------
        float ftemp;
        if(fXPPfactor>ftemp)
        {
            myCanvas->Font->Height = (fFontHeight + IDECREASEHEIGHT)*ftemp;
        }
        else
        {
            myCanvas->Font->Height = (fFontHeight + IDECREASEHEIGHT)*fXPPfactor;
        }
//==============================================================================
*/
    }
    int iFontWidth = myCanvas->TextWidth(strMyTitle);
    int ix2posi    = fXPPfactor*(iDrawWidth-iWBorder);
    if(ix2posi > rect.Right)
    {
        ix2posi = rect.Right;
    }
    myCanvas->TextOut((fXPPfactor*iDrawWidth-iFontWidth)/2,fYPPfactor*iHBorder,strMyTitle);
    myCanvas->MoveTo(fXPPfactor*iWBorder,fYPPfactor*40);
    myCanvas->LineTo(ix2posi,fYPPfactor*40);

//    int icount=ComboBoxList->Items->Count;
    int index      = ComboBoxList->ItemIndex;
//    int icountfile = ComboBoxFile->Items->Count;
    int indexfile  = ComboBoxFile->ItemIndex;
    AnsiString strAll;
    bool bFile     = false;
    if(indexfile >= 0)
    {
         strAll = ExtractFileName(ComboBoxFile->Items->Strings[indexfile]);
         bFile  = true;
    }
    else if(index >= 0)
    {
          strAll = ExtractFileName(ComboBoxList->Items->Strings[index]);
    }

    else return;
    AnsiString strtrainType,strtrainNum,strDate,strPerson,strTTT;
    char chTempp;
    int  iPosi;
    strtrainType = strAll.SubString(1,7);
    if(strtrainType.c_str()[4] == '5')
    {
        iPosi = 5;  //东风5
    }
    else if( strtrainType.c_str()[4] == '7')
    {
        chTempp  = strtrainType.c_str()[5];
        if(  ( chTempp >= '0' )
           &&( chTempp <= '9')
        )
        {
            iPosi = 5;
        }
        else
        {
            iPosi = 6;
        }
    }
    else if( strtrainType.c_str()[4] == '8')
    {
        chTempp  = strtrainType.c_str()[5];
        if(  ( chTempp >= '0' )
           &&( chTempp <= '9')
        )
        {
            iPosi = 5;
        }
        else
        {
            iPosi = 6;
        }
    }
    else if( strtrainType.c_str()[4] == '1')           //可能是东风11
    {
        chTempp  = strtrainType.c_str()[6];
        if(  ( chTempp >= '0'  )                     //此为东风11
           &&( chTempp <= '9' )
        )
        {
            iPosi = 6;
        }
        else
        {
            iPosi = 7;
        }
    }
    else
    {
        iPosi = 6;
    }

    strtrainType = strtrainType.SubString(1,iPosi);
    strtrainType = AnsiString("机车车型: ") + strtrainType;
    strtrainNum  = strAll.SubString(iPosi + 1,4);
    strtrainNum  = AnsiString("机车车号: ") + strtrainNum;
    strPerson    = AnsiString("检测人员: ") + strPersonName;
    if(bFile)
    {
        strDate = strAll.SubString(iPosi + 11,10);
    }
    else
    {
        int iposi;
        iposi   = strAll.Pos(".pst");
        strDate = strAll.SubString(iposi - 10,10);
    }
    strDate = AnsiString("测量时间: ") + strDate;
    myCanvas->Font->Name = "宋体";
/*
    if(bPrint)
    {
        myCanvas->Font->Height=fFontHeight;
    }
    else
    {
        myCanvas->Font->Size=fFontSize;
    }
*/
    if(iDrawType != 4)
    {
        int yposi,xposi;
        xposi = fXPPfactor       * iWTextPosi;
        yposi = (iHTextPosi + 2) * fYPPfactor;

        myCanvas->TextOut(xposi,yposi,strtrainType);

        int istep = fXPPfactor*20;

        int iFontWidth = myCanvas->TextWidth(strtrainType);

        xposi += iFontWidth;
        xposi += istep;

        myCanvas->TextOut(xposi,yposi,strtrainNum);

        iFontWidth = myCanvas->TextWidth(strtrainNum);
        xposi += iFontWidth;
        xposi += istep;

        myCanvas->TextOut(xposi,yposi,strDate);

        iFontWidth = myCanvas->TextWidth(strDate);
        xposi     += iFontWidth;
        xposi     += istep;

        myCanvas->TextOut(xposi,yposi,strPerson);

        int iFontHeight = myCanvas->TextHeight(strDate);
        xposi    = fXPPfactor  * iWTextPosi;
        yposi    += fSign(fYPPfactor)*iFontHeight;
        yposi    += fYPPfactor *  4;
//        yposi    += fYPPfactor * 4;

        strTemp = "修程:　　 　　　";
        myCanvas->TextOut(xposi, yposi, strTemp);

        iFontWidth = myCanvas->TextWidth(strTemp);
        xposi += iFontWidth;
        xposi += istep;
        strTemp = "专业技术人员:　　　　　　　　　　　　";
        myCanvas->TextOut(xposi, yposi, strTemp);

        iFontWidth = myCanvas->TextWidth(strTemp);
        xposi += iFontWidth;
        xposi += istep;
        strTemp = "验收员:　　　　";
        myCanvas->TextOut(xposi, yposi, strTemp);
    }
    else
    {
        int yposi,xposi;
        int istep = fXPPfactor*20;
        xposi     = fXPPfactor*iWTextPosi+3*istep;
        yposi     = (iHTextPosi+5)*fYPPfactor;

        int iFontWidth = myCanvas->TextWidth(strtrainType);
        xposi         += iFontWidth;
        iFontWidth     = myCanvas->TextWidth(strtrainNum);
        xposi         += iFontWidth;
        iFontWidth     = myCanvas->TextWidth(strDate);
        xposi         += iFontWidth;
        myCanvas->TextOut(xposi,yposi,strPerson);
        int iFontHeight = myCanvas->TextHeight(strDate);
        xposi    = fXPPfactor  * iWTextPosi;
        yposi    += iFontHeight;
        yposi    += fYPPfactor * 4;

        strTemp = "修程:　　 　　　";
        myCanvas->TextOut(xposi, yposi, strTemp);

        iFontWidth = myCanvas->TextWidth(strTemp);
        xposi += iFontWidth;
        xposi += istep;
        strTemp = "专业技术人员:　　　　　　　　　　　　";
        myCanvas->TextOut(xposi, yposi, strTemp);

        iFontWidth = myCanvas->TextWidth(strTemp);
        xposi += iFontWidth;
        xposi += istep;
        strTemp = "验收员：　　　　";
        myCanvas->TextOut(xposi, yposi, strTemp);
    }

}

void __fastcall TFormPrint::AdjustDrawRect(TRect& rect)
{
    //TODO: Add your source code here
    if(!bPrint)
    {
        fXPPfactor=1.0;
        fYPPfactor=1.0;
        rect=ImageBackground->ClientRect;
        rect.Top=(rect.Top + iHCurvePosi);
        rect.Bottom=(rect.Bottom - 2*iHBorder);
        rect.Left+=iWBorder;
        rect.Right-=iWBorder;
    }
    else
    {
        TRect rectB;
        int iBorder;
        rectB=ImageBackground->ClientRect;

        fXPPfactor=(float)(rect.Right-rect.Left)/(float)(rectB.Right-rect.Left);
        fYPPfactor = (float)(rect.Bottom-rect.Top)/(float)(rectB.Bottom-rectB.Top);
//计算 rect.Top
        iBorder=(int)(fYPPfactor*(float)iHCurvePosi);
        rect.Top=iBorder;
//计算 rect.Bottom
        iBorder=(int)(iHBorder*fYPPfactor);
        rect.Bottom = (rect.Bottom - 2*iBorder);
        iBorder=(int)((float)iWBorder*fXPPfactor);
//计算 rect.Left和rect.Right
        rect.Left += iBorder;
        rect.Right -= 2*iBorder;

    }
    FormMain->fXFactor = fXPPfactor;
    FormMain->fYFactor = fYPPfactor;

}



void __fastcall TFormPrint::ImageMainMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if( FormMain->AllDrawKind == drawInfo )     //画油管压力波
    {
        TRect rect;
        bDrawBasicCurve = !bDrawBasicCurve;
        FormMain->SetbDrawBasicCurve( bDrawBasicCurve );
        DrawNewListCurve(ImageBackground,rect);
    }
/*
    bDrag=true;
    OldCursor=ImageMain->Cursor;
    ImageMain->Cursor=crHandPoint;
    Application->ProcessMessages();
    pointOriginal.x=X;
    pointOriginal.y=Y;
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormPrint::ImageMainMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
/*
    if(bDrag)
    {
        bDrag=false;
//        ImageMain->Cursor=OldCursor;
//        int iStep=pointOriginal.y-Y;
//        pointOriginal.y=Y;
//        iscrollPosi+=iStep;
//        ScrollBarDraw->Position=iscrollPosi;
//        Application->ProcessMessages();
    }
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormPrint::ImageMainMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
/*
    if(bDrag)
    {
        int y0;
        y0=Y;
        if(iImagePosiMax==0) return;
        if(pointOriginal.y==y0) return;
        int iStep=Y-pointOriginal.y;
        pointOriginal.x=X;
        pointOriginal.y=Y;
       //     iStep=(int)(1.1*(float)iStep);
        iImagePosi+=iStep;
        if(iImagePosi<=iImagePosiMax)
        {
            ImageMain->Top+=iStep;
            ImageRight->Top+=iStep;
            ImageBottom->Top+=iStep;
        }
    }
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormPrint::FormClose(TObject *Sender,
      TCloseAction &Action)
{
//    FormMain->bAnalysisResult = bAnalysisResultBackup;
//    FormMain->bLibCompare = bLibCompareBackup;
}
//---------------------------------------------------------------------------

void __fastcall TFormPrint::N1Click(TObject *Sender)
{
    if(bPrintDrawGrid)
    {
        bPrintDrawGrid = false;
        N1->Caption    = "绘制网格线";
    }
    else
    {
        bPrintDrawGrid = true;
        N1->Caption    = "取消网格线";
    }
    FormMain->SetbGridLine(bPrintDrawGrid);
    ZoomOutDraw();
    switch(iDrawType)
    {
        case 1:
            DrawFirstCCurve();
            break;
        case 2:
            ComboBoxListChange(NULL);
            break;
    }
}
//---------------------------------------------------------------------------




void __fastcall TFormPrint::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(Shift.Contains(ssCtrl)&&(Key=='x'||Key=='X'))
    {
        ModalResult=mrOk;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormPrint::DrawNewListCurve(TObject* Sender, TRect& rect)
{
    //TODO: Add your source code here
    AdjustDrawRect(rect);
    TCanvas * myCanvas;
    if(bPrint)
    {
        FormMain->emDDevice = ddPrinter;
        myCanvas = (TCanvas*)(((TPrinter*) Sender)->Canvas);
    }
    else
    {
        FormMain->emDDevice = ddImage;
        myCanvas = (TCanvas*)(((TImage*) Sender)->Canvas);
    }
    DrawTitle(myCanvas,rect);
    FormMain->bFromPrinter = true;
    FormMain->DrawPSTAll(Sender,rect);
    FormMain->bFromPrinter = false;
    if(!bPrint)
    {
        TRect trect;
        trect = ImageBackground->ClientRect;
        rect = ImageMain->ClientRect;
        ImageMain->Canvas->CopyMode = cmSrcCopy;
        ImageMain->Canvas->CopyRect(rect,ImageBackground->Canvas,trect);
        ImageMain->Refresh();
    }
}

void __fastcall TFormPrint::DrawNewCurrentCurve(TObject* Sender, TRect& rect)
{
    //TODO: Add your source code here
    AdjustDrawRect(rect);  //调整坐标
    TCanvas * myCanvas;
    if(bPrint)
    {
        FormMain->emDDevice = ddPrinter;
        myCanvas=(TCanvas*)(((TPrinter*) Sender)->Canvas);
    }
    else
    {
        FormMain->emDDevice = ddImage;
        myCanvas=(TCanvas*)(((TImage*) Sender)->Canvas);
    }
    for(int i=0;i<8;i++)
    {
        FormMain->bRectSelected[i] = true;
    }
    FormMain->iAdjustDrawNum = 8;
    if(!FormMain->InitParameters())
    {
        return;
    }
//    if(!FormMain->lDS.SetRealPressure(FormMain->strPCPName.c_str()))
    if(!FormMain->lDS.LoadPCPFile(FormMain->strPCPName.c_str()))
    {
        Application->MessageBox("PCP数据文件打开时出错!","提示",MB_OK|MB_ICONINFORMATION);
        return;
    }
/*
    FormMain->lDS.SetiIteration(4);
    FormMain->lDS.ProcessPressure(2);  //三点重复平均,光顺压力曲线

    FormMain->lDS.CalculateLogVLogP(); //计算气缸容积、LogV及LogP
    FormMain->lDS.CalculateDPDF();     //计算压力升高率
    FormMain->lDS.CalculateFFTP();     //计算频域数据
    FormMain->lDS.TJHeatRelease();     //计算放热率
    FormMain->lDS.CalculateCHRR();     //计算累积放热率
*/
    FormMain->iCCStart = FormMain->lDS.iStartFai;
    FormMain->iCCEnd = FormMain->lDS.iEndFai;
//-------------------------------------------
//以下是软件环境方面的设置
    FormMain->iChoiceStart = 0;
    FormMain->iChoiceEnd = FormMain->lDS.iPLength;
    FormMain->iCurveShift = 1;
    DrawTitle(myCanvas,rect);
    FormMain->DrawAdjustCurve(Sender,rect);
    if(!bPrint)
    {
        TRect trect;
        trect = ImageBackground->ClientRect;
        rect = ImageMain->ClientRect;
        ImageMain->Canvas->CopyMode = cmSrcCopy;
        ImageMain->Canvas->CopyRect(rect,((TImage*)Sender)->Canvas,trect);
        ImageMain->Refresh();
    }
}

void __fastcall TFormPrint::DrawFirstCCurve(void)
{
    //TODO: Add your source code here
    bPrint     = false;  //由屏幕输出图形
    TRect      rect;
    AnsiString strTT;
    ComboBoxPrintContent->ItemIndex = -1;
    iDrawType  = 1;
    rect       = ImageBackground->ClientRect;
    DrawNewCurrentCurve(ImageBackground,rect);
}

void __fastcall TFormPrint::PrintContent(TObject* Sender, TRect& rect)
{
    //TODO: Add your source code here
    switch(iDrawType)
    {
        case 1:   //用于绘制单独一个缸的波形形
            DrawNewCurrentCurve(Sender,rect);
            break;
        case 2:
        {
            try
            {
                DrawNewListCurve(Sender,rect);
            }
            catch(...)
            {
            }
            break;
        }
    }
    if(bPrint==false)   //如果不是打印状态，那么就绘图结果显示到屏幕上。
    {
        ZoomOutDraw();
    }
}
void __fastcall TFormPrint::ToolButton7Click(TObject *Sender)
{
/*
    Prntr = Printer();
//    TRect r = Rect(200,200,Prntr->PageWidth - 200,Prntr->PageHeight - 200);
    Prntr->BeginDoc();
    ::SetMapMode(Prntr->Handle,MM_LOMETRIC);
//    Prntr->Canvas->TextRect(r, 200, 200, Memo1->Lines->Text);
    Prntr->Canvas->Pen->Width = 1;
    Prntr->Canvas->Pen->Color = clGray;
    for(int i=0;i<5;i++)
    {
        switch(i)
        {
            case 0:
            {
                Prntr->Canvas->Pen->Style = psSolid;
                break;
            }
            case 1:
            {
                Prntr->Canvas->Pen->Style = psDot;
                break;
            }
            case 2:
            {
                Prntr->Canvas->Pen->Style = psDash;
                break;
            }
            case 3:
            {
                Prntr->Canvas->Pen->Style = psDashDot;
                break;
            }
            case 4:
            {
                Prntr->Canvas->Pen->Style = psDashDotDot;
                break;
            }
        }
        Prntr->Canvas->MoveTo(200,-200+i*20);
        Prntr->Canvas->LineTo(400,-200+i*20);
    }

    AnsiString strMyTitle="机车柴油机测试结果";
    Prntr->Canvas->Font->Name="隶书";
    for(int i = 30; i<40;i++)
    {
        strMyTitle = strMyTitle + " " + AnsiString(i);
        Prntr->Canvas->Font->Height = -i;
        Prntr->Canvas->TextOut(100,50 - i*30,strMyTitle);
    }
    Prntr->EndDoc();
*/
}
//---------------------------------------------------------------------------


float __fastcall TFormPrint::fSign(float fvalue)
{
    //TODO: Add your source code here
    float ftemp;
    if(fvalue < 0.0)
    {
        ftemp = -1.0;
    }
    else
    {
        ftemp = 1.0;
    }
    return ftemp;
}
