/*
//---------------------------------------------------------------------------
#define NO_WIN32_LEAN_AND_MEAN
#include <shlobj.h>
#include <vcl.h>

#include <sys\stat.h>
//#include <WindowsX.h>
#include <registry.hpp>
#include <io.h>
#include <assert.h>

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>


#pragma hdrstop
*/
//---------------------------------------------------------------------------
#define NO_WIN32_LEAN_AND_MEAN
//---------------------------------------------------------------------------
#include <shlobj.h>
#include <vcl.h>
#include <registry.hpp>
#include <IniFiles.hpp>
#include <math.h>
#include <process.h>
#include <dir.h>
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <htmlhelp.h>
//#include "Excel_2K_SRVR.h"
//#include <Printers.hpp>
#pragma hdrstop
#include <vcl\utilcls.h>
#include "lFormMain.h"
#include "UDirectory.h"
#include "UIndicator.h"
#include "DrawCoor.h"
#include "lPrint.h"
#include "UGoBack.h"
#include "lAbout.h"
#include "LPPFDRead.h"
#include "lPath.h"
#include "dll.h"
#include "lFilterSetup.h"
#include "DataProcess.h"
#include "uSetupColor.h"
#include "UResult.h"
#include "UnitFileInfo.h"
#include "lFormReport.h"
#include "lFormBackup.h"
#include "UnitEnlarge.h"
#include "uFormOrderInfo.h"
#include "uEngine.h"
#include "UnitOption.h"
#include "UnitPcpInfo.h"
#include "UnitDataModule.h"
#include "UnitAddToBase.h"
#include "UAnalysisResult.h"
#include "UAllow.h"
#include "UnitTrainSelection.h"
#include "lDetect.h"
#include "UnitTreeview.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
const float fGuiYiCoef1 = 2000.0;
const float fGuiYiCoef2 = 2000.0;
const float fGuiYiCoef3 = 5.0;      //��ѹֵ��
const int iHalfWin      = 7;
const int iTopStep      = 20;
const int iTimeRes      = 8;
const int iTrainTypeNumber = 9;
const int IFONTHEIGHT = -12;
const int IDECREASEHEIGHT = 2;

const TColor clColor[] = {  clRed,
                          clGreen,
                          clBlue,
                          clYellow,
                          clBlack,
                          clGray,
                          clInfoBk,
                          clBackground
                        };


int LineType[]={        psSolid,       //	A solid line.
                        psDash,         //	A line made up of a series of dashes.
                        psDot,          //	A line made up of a series of dots.
                        psDashDot,      //	A line made up of alternating dashes and dots.
                        psDashDotDot     //
                };

/*
TColor MAIN_CURVECOLOR[] = {
                                   RGB(132,23,4),
                                   RGB(243,43,7),
                                   RGB(219,113,43),
                                   RGB(37,124,54),
                                   RGB(49,164,72),
                                   RGB(26,54,151),
                                   RGB(88,119,226),
                                   RGB(165,171,131),
                                   RGB(129,173,158),
                                   RGB(89,138,122),
                                   RGB(135,137,180),
                                   RGB(182,133,168),
                                   RGB(150,90,134),
                                   RGB(128,64,198),
                                   RGB(238,106,208),
                                   RGB(27,153,214)
                            };
*/
const short siFrequencyArray[] = {
                                10,   50,   100,  200,  300,    //0  - 4
                                400,  500,  600,  700,  800,    //5  - 9
                                900,  1000, 2000, 3000, 4000,   //10 - 14
                                5000, 6000, 7000, 8000, 9000,   //15 - 19
                                10000,12000,15000,16000,18000   //20 - 22 ����23��ѡ��
                            };
const float XANGLE = 10.0;
const float YANGLE = -10.0;
const int ALL_FILE_NUM = 999;
const float f_DELTASPEED = 50.0;   //���ת�ٲ�С��50 r/minʱ������Ϊ��ͬһ����

const AnsiString strColumnTitle[] = { // ��һ��
                                    "��",         //0
                                    "��ѹ��",       //1
                                    "��ѹ",       //2
                                    "ƽ��ָ",       //3
                                    "ѹ���� ",      //4
                                    "ѹ����",       //5
                                    "���͡�",       //6
                                    "���͡�",       //7
                                    "ȼ�ա�",       //8
                                    "ȼ�ա�",       //9
                                    "������",       //10
                                    "������",       //11
                                    "�ۻ���",       //12
                                    "ѭ�� ",       //13
                                    "���ͻ�",       //14
                                    //�ڶ���
                                    "��",
                                    "���ֵ",
                                    "��λ",
                                    "ʾѹ��",
                                    "���ֵ",
                                    "��λ",
                                    "��ʼ��",
                                    "������",
                                    "��ʼ��",
                                    "������",
                                    "���ֵ",
                                    "��λ",
                                    "������",
                                    "��",
                                    "ת��"
                                  };
const  AnsiString strColumnUnit[] = {
                                    "",
                                    "(MPa)",
                                    "(CA)",
                                    "(MPa)",
                                    "(MPa/CA)",
                                    "(CA)",
                                    "(CA)",
                                    "(CA)",
                                    "(CA)",
                                    "(CA)",
                                    "(kJ/CA)",
                                    "(CA)",
                                    "(kJ)",
                                    "(kJ)",
                                    "(r/min)"
                                    };

TFormMain *FormMain;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
    : TForm(Owner)
{
//    fDetectLevel         = 5.0;
    bPipeCompared        = false;
    iMTrainInfoNums      = 0;

    bProcessing = true;
//  �˲����йصĲ���
    iNowSelectTrainType = 3;
    iNowDetectInfo      = 3;
    bAddMInfo  = true;
    fPl = 500.0;
    fSl = 2000.0;
    fPh = 4000.0;
    fSh = 5000.0;
    iFilterLen = 55;       //�ݶ�Ϊ55��
    bFiltered = false;
    iWindowChoice = 2;     //ѡ������
    iNum = 55;
    
    for(int i=0; i<iPASSNUM; i++)
    {
        fPassData[i] = NULL;
    }
    bITF = false;
    iCalculateMethod = 0;  //����ѧ�����ڵļ��㷽��
    bStarted = false;
    bAbstractDrawFF = false;
    bOriginalDrawAll = false;
    bAbstractDrawAll = false;
    bButtonPressed = false;
    bDriveInited = false;
    bFileOpened = false;
    iMainPrnSign = 1;
    bLoadList = false;
    bDrawStyle = true;
    iPreProMethod = 0;
    adjustDrawKind = drawAll;

    iCircleDim = 6;
    bPCPGridLine = true;
    bPSTGridLine = false;
    clGridLineColor = clGray;
    bAllHRR = false;
    bAdjustHRR = false;
    bOneOfAll = false;
    bPSTOpened = false;
    bLoadOriginal = false;
    bDrawModePressed = false;
    bBackMousePoint = false;
    bFirstMove = true;
    bShowAll = false;
//------------------------------------------
//  ����ӡģ�鹫�õ�ȫ�ֱ���
//------------------------------------------
    bPSTGridLine    = false;
    iCurrentDraw = 0;  // 1:���Ƶ����б��ļ�����2�����Ƶ��ǵ����ļ�  0��ǰ��δ��ͼ
    bDrawStyle = true;          //true:˫�д�ӡ��false�����д�ӡ
    strUserTitle = "����������ѧ�о�Ժ���������о���";
    strSoftTitle = "�������ͻ�ȼ��ϵͳ��������";
//------------------------------------------
// �����ļ��б������ȫ�ֱ�������ʼ��״̬
//----------------------------------------
    ListStringList=NULL;
    ListStringList= new TStringList();
    iListCount = 0;
    fXFactor = 1.0;
    fYFactor = 1.0;
    bParameterInited = false;
    iDrawLineWidth   = 1;
    clInletColor      = clGray;
    clCompressColor   = clMaroon;
    clCombustionColor = clRed;
    clOutletColor     = clBlack;
    clBackground      = clWhite;
    clLineColor       = clBlack;
    clTextColor       = clBlack;
//----------------------------------------
//  ���������б��ļ��ı�����ʼ��
    ListStringList      = NULL;
    ppr                 = NULL;
    ListStringList      = new TStringList();
    strPresetPath       = "e:\\��ѹ�������";
    strPSTFileDirectory = "e:\\��ѹ�������";    //��һ�δ��ļ���·��
    iFileExt            = 2;                     //ȱʡ����´� *.PST�ļ�
    bRegHave            = false;
    iKeyPressPosi       = 0;
    AllDrawKind         = drawPT;
    bDrawPWave          = false;
    bDrawCWave          = false;
    bCursorLine         = false;
    bCursorLineChanged  = false;
    bFromPrinter        = false;

    fXAngle             = XANGLE;
    fYAngle             = YANGLE;
    iXALength           = 320;
    iYALength           = 150;
    iX0Step             = 2;
    iY0Step             = 2;
    bShowCut            = true;

    fstartrt     = 0.0;
    fendrt       = 1.0;
    short int           siListFileNum[100];
    ListPath      = new TStringList;
    ListList      = new TStringList;
    colorTV       = clInfoBk;
    clDatabaseColor = (TColor)15385757;
    bDataBackup    = false;
    iDeletePosi    = 0;
    iBackupMode    = 0;
    iEnlargeSize   = 0;
    fTVStart       = 0.0;
    fTVEnd         = 1.0;
    bDataShowAll   = false;
    bAnalysising   = false;
    fDrawConst     = 1.2;
    strBasicFontName = "Arial";
    iBasicFontSize   = 9;
    bBasicFSBold       = true;
    bBasicFSItalic     = false;
    bBasicFSUnderline  = false;
    bBasicFSStrikeOut  = false;
    bSaveOrig          = true;    //����ȷ���Ƿ񱣴�ԭʼ����
    clBasicFSColor     = clBlack;
    strPerson          = "����Ц";
    
    mainCurveColor[0]  = (TColor)RGB(80,80,80);
    mainCurveColor[1]  = clMaroon;
    mainCurveColor[2]  = clGreen;
    mainCurveColor[3]  = clOlive;
    mainCurveColor[4]  = clNavy;
    mainCurveColor[5]  = clPurple;
    mainCurveColor[6]  = clTeal;
    mainCurveColor[7]  = clLime,
    mainCurveColor[8]  = (TColor)RGB(100,0,0);
    mainCurveColor[9]  = clYellow;
    mainCurveColor[10] = (TColor)RGB(0,100,0) ;
    mainCurveColor[11] = clFuchsia;
    mainCurveColor[12] = (TColor)RGB(100,100,0);
    mainCurveColor[13] = clAqua,
    mainCurveColor[14] = (TColor)RGB(0,100,100);
    mainCurveColor[15] = clRed;
    AllDrawKind = drawPT;
    for(int i = 0; i < 32; i++)
    {
        pArrayMS[i] = NULL;
    }
    strLibResult        = new TStringList();
    
//�������ͻ��������Ͳ���
//1 ����4A
    myDieselInfo[0].chCommandMark = 0x99;             //ӦΪ0x99
    myDieselInfo[0].chDieselGroup = 0;                //���
	myDieselInfo[0].fDiameter     = 0.24;             //����ֱ��
	myDieselInfo[0].fStroke       = 0.275;            //�������
	myDieselInfo[0].fLMDa         = 0.474137931;      //�������˱�
	myDieselInfo[0].fCRatio       = 12.5;             //ѹ����
	myDieselInfo[0].fPodLength    = 0.57;             //���˳���
	myDieselInfo[0].fInCloseAngle = -138.0;           //�����ſ����Ƕ�
	myDieselInfo[0].fOutOpenAngle = 130.0;            //�����ŹرսǶ�
	myDieselInfo[0].fCAING        = -22.0;            //���ͽǶ�
    myDieselInfo[0].fCompressMax  = 3.0;              //ѹ��ѹ�����ֵ��
    myDieselInfo[0].fCompressMin  = 2.6;              //ѹ��ѹ����Сֵ
    myDieselInfo[0].fOilAngleMax  = -24.0;            //������ǰ�����ֵ��
    myDieselInfo[0].fOilAngleMin  = -14.0;            //������ǰ����Сֵ��
    myDieselInfo[0].fCombustionAngleMax = -10.0;      //ȼ����ǰ�����ֵ��
    myDieselInfo[0].fCombustionAngleMin = -1.0;       //ȼ����ǰ����Сֵ
    myDieselInfo[0].fDetectLevel        = 6.0;        //ѹ���������ж�����

//2 ����4B
    myDieselInfo[1].chCommandMark = 0x99;               //ӦΪ0x99
    myDieselInfo[1].chDieselGroup = 1;                  //���
	myDieselInfo[1].fDiameter     = 0.24;               //����ֱ��
	myDieselInfo[1].fStroke       = 0.275;              //�������
	myDieselInfo[1].fLMDa         = 0.474137931;        //�������˱�
	myDieselInfo[1].fCRatio       = 12.5;               //ѹ����
	myDieselInfo[1].fPodLength    = 0.57;               //���˳���
	myDieselInfo[1].fInCloseAngle = -138.0;             //�����ſ����Ƕ�
	myDieselInfo[1].fOutOpenAngle = 130.0;              //�����ŹرսǶ�
	myDieselInfo[1].fCAING        = -22.0;              //���ͽǶ�
    myDieselInfo[1].fCompressMax  = 3.0;                //ѹ��ѹ�����ֵ��
    myDieselInfo[1].fCompressMin  = 2.6;                //ѹ��ѹ����Сֵ
    myDieselInfo[1].fOilAngleMax  = -24.0;              //������ǰ�����ֵ��
    myDieselInfo[1].fOilAngleMin  = -14.0;              //������ǰ����Сֵ��
    myDieselInfo[1].fCombustionAngleMax = -10.0;        //ȼ����ǰ�����ֵ��
    myDieselInfo[1].fCombustionAngleMin = -1.0;         //ȼ����ǰ����Сֵ
    myDieselInfo[1].fDetectLevel        = 6.0;        //ѹ���������ж�����
//3 ����4c
    myDieselInfo[2].chCommandMark = 0x99;               //ӦΪ0x99
    myDieselInfo[2].chDieselGroup = 2;                  //���
	myDieselInfo[2].fDiameter     = 0.24;               //����ֱ��
	myDieselInfo[2].fStroke       = 0.275;              //�������
	myDieselInfo[2].fLMDa         = 0.474137931;        //�������˱�
	myDieselInfo[2].fCRatio       = 12.5;               //ѹ����
	myDieselInfo[2].fPodLength    = 0.57;               //���˳���
	myDieselInfo[2].fInCloseAngle = -138.0;             //�����ſ����Ƕ�
	myDieselInfo[2].fOutOpenAngle = 130.0;              //�����ŹرսǶ�
	myDieselInfo[2].fCAING        = -22.0;                  //���ͽǶ�
    myDieselInfo[2].fCompressMax  = 3.0;            //ѹ��ѹ�����ֵ��
    myDieselInfo[2].fCompressMin  = 2.6;            //ѹ��ѹ����Сֵ
    myDieselInfo[2].fOilAngleMax  = -24.0;            //������ǰ�����ֵ��
    myDieselInfo[2].fOilAngleMin  = -14.0;            //������ǰ����Сֵ��
    myDieselInfo[2].fCombustionAngleMax = -10.0;     //ȼ����ǰ�����ֵ��
    myDieselInfo[2].fCombustionAngleMin = -1.0;     //ȼ����ǰ����Сֵ
    myDieselInfo[2].fDetectLevel        = 6.0;        //ѹ���������ж�����

//4 ����4D
    myDieselInfo[3].chCommandMark = 0x99;   //ӦΪ0x99
    myDieselInfo[3].chDieselGroup = 3;   //���
	myDieselInfo[3].fDiameter     = 0.24;               //����ֱ��
	myDieselInfo[3].fStroke       = 0.275;                 //�������
	myDieselInfo[3].fLMDa         = 0.474137931;                   //�������˱�
	myDieselInfo[3].fCRatio       = 12.5;                 //ѹ����
	myDieselInfo[3].fPodLength    = 0.57;              //���˳���
	myDieselInfo[3].fInCloseAngle = -138.0;           //�����ſ����Ƕ�
	myDieselInfo[3].fOutOpenAngle = 130.0;           //�����ŹرսǶ�
	myDieselInfo[3].fCAING        = -22.0;                  //���ͽǶ�
    myDieselInfo[3].fCompressMax  = 3.0;            //ѹ��ѹ�����ֵ��
    myDieselInfo[3].fCompressMin  = 2.6;            //ѹ��ѹ����Сֵ
    myDieselInfo[3].fOilAngleMax  = -24.0;            //������ǰ�����ֵ��
    myDieselInfo[3].fOilAngleMin  = -14.0;            //������ǰ����Сֵ��
    myDieselInfo[3].fCombustionAngleMax = -10.0;     //ȼ����ǰ�����ֵ��
    myDieselInfo[3].fCombustionAngleMin = -1.0;     //ȼ����ǰ����Сֵ
    myDieselInfo[3].fDetectLevel        = 6.0;        //ѹ���������ж�����

//5 ����8B
    myDieselInfo[4].chCommandMark = 0x99;   //ӦΪ0x99
    myDieselInfo[4].chDieselGroup = 4;   //���
	myDieselInfo[4].fDiameter     = 0.28;               //����ֱ��
	myDieselInfo[4].fStroke       = 0.285;                 //�������
	myDieselInfo[4].fLMDa         = 0.4913793;                   //�������˱�
	myDieselInfo[4].fCRatio       = 12.8;                 //ѹ����
	myDieselInfo[4].fPodLength    = 0.58;              //���˳���
	myDieselInfo[4].fInCloseAngle = -138.0;           //�����ſ����Ƕ�
	myDieselInfo[4].fOutOpenAngle = 130.0;           //�����ŹرսǶ�
	myDieselInfo[4].fCAING        = -22.0;                  //���ͽǶ�
    myDieselInfo[4].fCompressMax  = 3.0;            //ѹ��ѹ�����ֵ��
    myDieselInfo[4].fCompressMin  = 2.6;            //ѹ��ѹ����Сֵ
    myDieselInfo[4].fOilAngleMax  = -24.0;            //������ǰ�����ֵ��
    myDieselInfo[4].fOilAngleMin  = -14.0;            //������ǰ����Сֵ��
    myDieselInfo[4].fCombustionAngleMax = -10.0;     //ȼ����ǰ�����ֵ��
    myDieselInfo[4].fCombustionAngleMin = -1.0;     //ȼ����ǰ����Сֵ
    myDieselInfo[4].fDetectLevel        = 6.0;        //ѹ���������ж�����
//6 ����11
    myDieselInfo[5].chCommandMark = 0x99;   //ӦΪ0x99
    myDieselInfo[5].chDieselGroup = 5;   //���
	myDieselInfo[5].fDiameter     = 0.28;               //����ֱ��
	myDieselInfo[5].fStroke       = 0.285;                 //�������
	myDieselInfo[5].fLMDa         = 0.4913793;                   //�������˱�
	myDieselInfo[5].fCRatio       = 12.8;                 //ѹ����
	myDieselInfo[5].fPodLength    = 0.58;              //���˳���
	myDieselInfo[5].fInCloseAngle = -138.0;           //�����ſ����Ƕ�
	myDieselInfo[5].fOutOpenAngle = 130.0;           //�����ŹرսǶ�
	myDieselInfo[5].fCAING        = -22.0;                  //���ͽǶ�
    myDieselInfo[5].fCompressMax  = 3.0;            //ѹ��ѹ�����ֵ��
    myDieselInfo[5].fCompressMin  = 2.6;            //ѹ��ѹ����Сֵ
    myDieselInfo[5].fOilAngleMax  = -24.0;            //������ǰ�����ֵ��
    myDieselInfo[5].fOilAngleMin  = -14.0;            //������ǰ����Сֵ��
    myDieselInfo[5].fCombustionAngleMax = -10.0;     //ȼ����ǰ�����ֵ��
    myDieselInfo[5].fCombustionAngleMin = -1.0;     //ȼ����ǰ����Сֵ
    myDieselInfo[5].fDetectLevel        = 6.0;        //ѹ���������ж�����
//7 ����A
    myDieselInfo[6].chCommandMark = 0x99;   //ӦΪ0x99
    myDieselInfo[6].chDieselGroup = 6;   //���
	myDieselInfo[6].fDiameter     = 0.24;               //����ֱ��
	myDieselInfo[6].fStroke       = 0.275;                 //�������
	myDieselInfo[6].fLMDa         = 0.474137931;                   //�������˱�
	myDieselInfo[6].fCRatio       = 12.5;                 //ѹ����
	myDieselInfo[6].fPodLength    = 0.57;              //���˳���
	myDieselInfo[6].fInCloseAngle = -138.0;           //�����ſ����Ƕ�
	myDieselInfo[6].fOutOpenAngle = 130.0;           //�����ŹرսǶ�
	myDieselInfo[6].fCAING        = -22.0;                  //���ͽǶ�
    myDieselInfo[6].fCompressMax  = 3.0;            //ѹ��ѹ�����ֵ��
    myDieselInfo[6].fCompressMin  = 2.6;            //ѹ��ѹ����Сֵ
    myDieselInfo[6].fOilAngleMax  = -24.0;            //������ǰ�����ֵ��
    myDieselInfo[6].fOilAngleMin  = -14.0;            //������ǰ����Сֵ��
    myDieselInfo[6].fCombustionAngleMax = -10.0;     //ȼ����ǰ�����ֵ��
    myDieselInfo[6].fCombustionAngleMin = -1.0;     //ȼ����ǰ����Сֵ
    myDieselInfo[6].fDetectLevel        = 6.0;        //ѹ���������ж�����

//8 ����B
    myDieselInfo[7].chCommandMark = 0x99;   //ӦΪ0x99
    myDieselInfo[7].chDieselGroup = 7;   //���
	myDieselInfo[7].fDiameter     = 0.24;               //����ֱ��
	myDieselInfo[7].fStroke       = 0.275;                 //�������
	myDieselInfo[7].fLMDa         = 0.474137931;                   //�������˱�
	myDieselInfo[7].fCRatio       = 12.5;                 //ѹ����
	myDieselInfo[7].fPodLength    = 0.57;              //���˳���
	myDieselInfo[7].fInCloseAngle = -138.0;           //�����ſ����Ƕ�
	myDieselInfo[7].fOutOpenAngle = 130.0;           //�����ŹرսǶ�
	myDieselInfo[7].fCAING        = -22.0;                  //���ͽǶ�
    myDieselInfo[7].fCompressMax  = 3.0;            //ѹ��ѹ�����ֵ��
    myDieselInfo[7].fCompressMin  = 2.6;            //ѹ��ѹ����Сֵ
    myDieselInfo[7].fOilAngleMax  = -24.0;            //������ǰ�����ֵ��
    myDieselInfo[7].fOilAngleMin  = -14.0;            //������ǰ����Сֵ��
    myDieselInfo[7].fCombustionAngleMax = -10.0;     //ȼ����ǰ�����ֵ��
    myDieselInfo[7].fCombustionAngleMin = -1.0;     //ȼ����ǰ����Сֵ
    myDieselInfo[7].fDetectLevel        = 6.0;        //ѹ���������ж�����

//9 ����C
    myDieselInfo[8].chCommandMark = 0x99;   //ӦΪ0x99
    myDieselInfo[8].chDieselGroup = 8;   //���
	myDieselInfo[8].fDiameter     = 0.24;               //����ֱ��
	myDieselInfo[8].fStroke       = 0.275;                 //�������
	myDieselInfo[8].fLMDa         = 0.474137931;                   //�������˱�
	myDieselInfo[8].fCRatio       = 12.5;                 //ѹ����
	myDieselInfo[8].fPodLength    = 0.57;              //���˳���
	myDieselInfo[8].fInCloseAngle = -138.0;           //�����ſ����Ƕ�
	myDieselInfo[8].fOutOpenAngle = 130.0;           //�����ŹرսǶ�
	myDieselInfo[8].fCAING        = -22.0;                  //���ͽǶ�
    myDieselInfo[8].fCompressMax  = 3.0;            //ѹ��ѹ�����ֵ��
    myDieselInfo[8].fCompressMin  = 2.6;            //ѹ��ѹ����Сֵ
    myDieselInfo[8].fOilAngleMax  = -24.0;            //������ǰ�����ֵ��
    myDieselInfo[8].fOilAngleMin  = -14.0;            //������ǰ����Сֵ��
    myDieselInfo[8].fCombustionAngleMax = -10.0;     //ȼ����ǰ�����ֵ��
    myDieselInfo[8].fCombustionAngleMin = -1.0;     //ȼ����ǰ����Сֵ
    myDieselInfo[8].fDetectLevel        = 6.0;        //ѹ���������ж�����

//10������D
    myDieselInfo[9].chCommandMark = 0x99;   //ӦΪ0x99
    myDieselInfo[9].chDieselGroup = 9;   //���
	myDieselInfo[9].fDiameter     = 0.24;               //����ֱ��
	myDieselInfo[9].fStroke       = 0.275;                 //�������
	myDieselInfo[9].fLMDa         = 0.474137931;                   //�������˱�
	myDieselInfo[9].fCRatio       = 12.5;                 //ѹ����
	myDieselInfo[9].fPodLength    = 0.57;              //���˳���
	myDieselInfo[9].fInCloseAngle = -138.0;           //�����ſ����Ƕ�
	myDieselInfo[9].fOutOpenAngle = 130.0;           //�����ŹرսǶ�
	myDieselInfo[9].fCAING        = -22.0;                  //���ͽǶ�
    myDieselInfo[9].fCompressMax  = 3.0;            //ѹ��ѹ�����ֵ��
    myDieselInfo[9].fCompressMin  = 2.6;            //ѹ��ѹ����Сֵ
    myDieselInfo[9].fOilAngleMax  = -24.0;            //������ǰ�����ֵ��
    myDieselInfo[9].fOilAngleMin  = -14.0;            //������ǰ����Сֵ��
    myDieselInfo[9].fCombustionAngleMax = -10.0;     //ȼ����ǰ�����ֵ��
    myDieselInfo[9].fCombustionAngleMin = -1.0;     //ȼ����ǰ����Сֵ
    myDieselInfo[9].fDetectLevel        = 6.0;        //ѹ���������ж�����

//11������E
    myDieselInfo[10].chCommandMark = 0x99;   //ӦΪ0x99
    myDieselInfo[10].chDieselGroup = 10;   //���
	myDieselInfo[10].fDiameter     = 0.24;               //����ֱ��
	myDieselInfo[10].fStroke       = 0.275;                 //�������
	myDieselInfo[10].fLMDa         = 0.474137931;                   //�������˱�
	myDieselInfo[10].fCRatio       = 12.5;                 //ѹ����
	myDieselInfo[10].fPodLength    = 0.57;              //���˳���
	myDieselInfo[10].fInCloseAngle = -138.0;           //�����ſ����Ƕ�
	myDieselInfo[10].fOutOpenAngle = 130.0;           //�����ŹرսǶ�
	myDieselInfo[10].fCAING        = -22.0;                  //���ͽǶ�
    myDieselInfo[10].fCompressMax  = 3.0;            //ѹ��ѹ�����ֵ��
    myDieselInfo[10].fCompressMin  = 2.6;            //ѹ��ѹ����Сֵ
    myDieselInfo[10].fOilAngleMax  = -24.0;            //������ǰ�����ֵ��
    myDieselInfo[10].fOilAngleMin  = -14.0;            //������ǰ����Сֵ��
    myDieselInfo[10].fCombustionAngleMax = -10.0;     //ȼ����ǰ�����ֵ��
    myDieselInfo[10].fCombustionAngleMin = -1.0;     //ȼ����ǰ����Сֵ
    myDieselInfo[10].fDetectLevel        = 6.0;        //ѹ���������ж�����
    
    strArrayTrainType[0] = "����A";
    strArrayTrainType[1] = "����B";
    strArrayTrainType[2] = "����C";
    strArrayTrainType[3] = "����D";
    strArrayTrainType[4] = "����E";
    
//---------------------------------------------------------
    bExitOut             = false;     //���˳�
    bFirstTime           = false;
    iSSoftNumbers        = 1971356;
    clBorderColor        = 0x00000000;
//==========================================================

    strData              = NULL;      //������Ϊ��
    strData = new unsigned char[131072];
    if(strData == NULL)
    {
        Application->MessageBox("�޷������ڴ棬InitPart!","��ʾ",MB_OK|MB_ICONINFORMATION);
        Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormCreate(TObject *Sender)
{

    int isize;
//    isize = sizeof(myDieselInfo)/sizeof(StructDieselInfo);

    clListCurveColor[0] = clBlack;
    clListCurveColor[1] = clRed;
    clListCurveColor[2] = clGreen;
    clListCurveColor[3] = clBlue;
    clListCurveColor[4] = clAppWorkSpace;
    clListCurveColor[5] = clFuchsia;
    clListCurveColor[6] = clOlive;
    clListCurveColor[7] = clPurple;
// �����ݲ�ѯ���Ϳؼ���������
//----------------------------------------------------

//====================================================
    CurrentIndex          = "\\*.PST";
    String strExeFileName = ::ParamStr(0);
    String strNowPath     = ::ExtractFilePath(strExeFileName);
    strExeFileName        = strNowPath;
    strExeFilePath        = strNowPath;
// �ڴ˴��������ݵı���·��

//    strFileDirectory = strNowPath;

//    int ilen = strNowPath.Length();

    ComboBox1->Items->Clear();
    ComboBox1->Items->Add(CurrentDir);
    ComboBox1->Text = CurrentDir;

    UpdateListView();
    ComState = csNone;

    CheckBoxAdjustCPT->Checked = true;
    CheckBoxAdjustDPDF->Checked = true;
    CheckBoxAdjustHRR->Checked = true;
    CheckBoxAdjustCHRR->Checked = true;

    for( int i=0;i<9;i++)
    {
        bRectSelected[i] = false;
    }

    bRectSelected[1] = true;
    bRectSelected[4] = true;
    bRectSelected[5] = true;
    bRectSelected[6] = true;
//�ж������Ƿ�Ϸ���װ�ģ�����������
    if(!RegSoftwareValid())
    {
//        Application->MessageBox("�Ƿ��û�!\n\n�޷�ʹ��!","��ʾ",MB_OK|MB_ICONINFORMATION);
//        exit(0);
    }
    if(!RegReadInfo())
    {
        strPresetPath.c_str()[0] = strFileDirectory.c_str()[0];
        strPSTFileDirectory      = strPresetPath;
        bExcel                   = true;     //��������ʱ����EXCEL�ļ�
//---------------------------------------------------------------
        dtSStartTime             = Now();   //�����İ�װ����
        dtLLastTime              = Now();   //���������һ��ʹ������
//===============================================================

        if(bRegHave)
        {
            if(!DirectoryExists(strPresetPath))
            {
                if (!CreateDir(strPresetPath))
                {
                    MessageDlg("Ŀ¼�������� |��Ŀ¼|", mtConfirmation, TMsgDlgButtons() << mbOK, 0);
                }
            }
        }
    }
    else
    {
        bFInstalled = false;
    }
    if(DirectoryExists(strPSTFileDirectory))
    {
        strFileDirectory = strPSTFileDirectory;
        DirectoryListBoxFile->Directory = strFileDirectory;
    }

    if(!FormAdd())
    {
        bSecondTime = false;
    }
    else
    {
        bSecondTime = true;
    }
    isSoftwareValid();   //�ж������Ƿ���

    CurrentDir = strPresetPath;   //.SubString(1,ilen-1);
    Application->OnHint           = DisplayHint;

    CheckBoxbSaveOrig->Checked    = bSaveOrig;
    CheckBoxAddMInfo->Checked     = bAddMInfo;
    CheckBoxComputerTime->Checked = bProcessing;

    bCombustion         = false;
    iTVCurrPosi           = 0;
    iTVLastPosi           = 0;
    IniFileRead();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::AddCode(AnsiString& strsecret)
{
    //TODO: Add your source code here
    int len=strsecret.Length();
    for(int n=0;n<len;n++)
    {
        strsecret.c_str()[n]+=0;
    }
}

void __fastcall TFormMain::DeleteCode(AnsiString& strsecret)
{
    //TODO: Add your source code here
    int len=strsecret.Length();
    for(int n=0;n<len;n++)
    {
        strsecret.c_str()[n]-=0;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormDelete(void)
{
    //TODO: Add your source code here
    TRegistry *Reg = new TRegistry();
    Reg->RootKey   = HKEY_LOCAL_MACHINE;
    if(!Reg->OpenKey(AnsiString("\\SoftWare\\llhwstars\\PLUSM"),false))
    {
        if( Reg->OpenKey(AnsiString("\\SoftWare\\llhwstars\\PLUSM"),true))
        {
            Reg->WriteInteger("030452",iSListNumber);    //Installed
            Reg->WriteInteger("117906",iSBorderColor);   //Target
            Reg->WriteInteger("384097",iDeleteOld);
            Reg->CloseKey();
    	    delete Reg;
            Reg  = NULL;
    	    return;
        }
    }
    else
    {
        if(bFirstTime)
        {
            if(iDeleteOld == 0x8000000)
            {
                iSListNumber = 100;
                iSBorderColor = 200;
            }
            Reg->WriteInteger("030452",iSListNumber);
            Reg->WriteInteger("117906",iSBorderColor);
            Reg->WriteInteger("384097",iDeleteOld);
        }
        else
        {
            Reg->WriteInteger("030452",iSListNumber);
            Reg->WriteInteger("117906",iSBorderColor);
            Reg->WriteInteger("384097",iDeleteOld);
        }
        Reg->CloseKey();
        delete Reg;
    }
}

bool __fastcall TFormMain::FormAdd(void)
{
    //TODO: Add your source code here
    TRegistry *Reg = new TRegistry();
    Reg->RootKey   = HKEY_LOCAL_MACHINE;
    if(Reg->OpenKey(AnsiString("\\SoftWare\\llhwstars\\PLUSM"),false))
    {
    	try
        {
    		iSListNumber  = Reg->ReadInteger("030452");    //��һ��ʹ�ð�װ����
            iSBorderColor = Reg->ReadInteger("117906");     //��������
            iDeleteOld    = Reg->ReadInteger("384097");
        }
        catch(...)
        {
        	Reg->CloseKey();
            delete Reg;
            Reg  = NULL;
            return false;
        }
    	Reg->CloseKey();
    }
    else
    {
        iSListNumber = 100;
        iSBorderColor = 200;
        iDeleteOld = 400;
    	delete Reg;
        Reg  = NULL;
    	return false;
    }
    delete Reg;
    return true;
}


void __fastcall TFormMain::SoftRestrict(void)
{
    //TODO: Add your source code here
//    String strExeFileName=::ParamStr(0);
    String strNowPath = strExeFilePath;  //::ExtractFilePath(strExeFileName);
//    strExeFileName=strNowPath;
    unsigned long SaveResult,DataWeWant,DataWe;
    short int SaveDogBytes,DogBytes,DogData;
    char SaveDogData[256],conDogData[256];
    float ftemp1=3.1415926,ftemp2=10.0,ftemp3=1.50,ftemp4=0.5,ftemp5;
    int i1=55,i2=2000,i3;
    int iNum=40;
    randomize();
    DogBytes=random(64);
    DataWe=random(10000);
    for(int i=0;i<DogBytes;i++)
    {
        conDogData[i]=random(255);
    }
    for(int i=0;i<DogBytes;i++)
    {
        SaveDogData[i]=conDogData[i];
    }
    ftemp5=ftemp1*sin(ftemp2)/ftemp3;
    ftemp3=ftemp5*pow(fabs(ftemp5),ftemp4);
    ftemp5=sqrt(fabs(ftemp3));
    i3=i1&i2;
    i3=random(1000);
    if(bOperateFirst)//����ǵ�һ�ΰ�װ  �ڵ�ǰ��װ·����û��DLL�ļ�
    {

        strNowPath = strExeFilePath + "lbcSrl51.dll";
        if(FileExists(strNowPath))//���������һ����˵�����������������°�װ
        {
              ;
        }

        FILE* fp;
        String stemp;
        stemp="This is a test!";
        if(FileExists(strNowPath))
        {
            iSoftNum=0;
        }
        else
        {
            if((fp=fopen(strNowPath.c_str(),"wb"))==NULL)
            {
                exit(0);
            }
            fwrite(stemp.c_str(),15,1000,fp);
            fclose(fp);
        }
    }
    for(int i=0;i<i3;i++)
    {
        if(i3%3==0)
        {
            i3=i*i+157;
        }
    }
    DataWeWant=SaveResult^SaveResult;
    ftemp5=ftemp1*sqrt(fabs(ftemp2))/ftemp3;
    if(bFInstalled&&(!bSecondTime))  //����ǵ�һ��ʹ�ñ�����,
    {
        iSoftNum        = iSoftRNum;
        iiGridLineColor = iSoftRNum;
        bSEnabled       = true;
        bSInformed      = false;
    }
    else
    {
        int len = strSoftTarget.Length();
        for(int i=0;i<len;i++)
        {
            strSoftTarget.c_str()[i]-=len-i;
        }
        len = strSoftInstall.Length();
        for(int i=0;i<len;i++)
        {
            strSoftInstall.c_str()[i]-=i;
        }
        iSoftNum=iSoftNum&0x7fffff00;
        iSoftNum=iSoftNum>>16;
        iiGridLineColor=iiGridLineColor&0x7fffff00;
        iiGridLineColor=iiGridLineColor>>8;
        ftemp3=ftemp5*pow(fabs(ftemp5),ftemp4);
        ftemp5=sqrt(fabs(ftemp3))+iSoftNum;
        iSoftNum/=4;
        ftemp3=ftemp5*fabs(ftemp4);
        iiGridLineColor/=64;
        iSoftNum=iSoftNum&0x0000ffff;
        iiGridLineColor=iiGridLineColor&0x0000ffff;
 //       iSoftNum=29;
 //       iiGridLineColor=29;
        if(iSoftNum!=iiGridLineColor) //���˸Ĺ���Щ��ֵ��
        {
             bSEnabled=false;
        }
        else if(iSoftNum>iSoftRNum||iiGridLineColor>iSoftRNum)
        {
            bSEnabled=false;
        }
        else  if(iSoftNum<iSoftINum&&iSoftNum>0)
        {

            bSEnabled=true;
            bSInformed=true;

        }
        else if(iSoftNum==0)
        {
            bSEnabled=false;
            iSoftNum=1;
        }
        else
        {
            if(strSoftTarget<strSoftInstall)
            {
                bSEnabled=true;
                bSInformed=true;
            }
            else
            {
                bSEnabled=true;
                bSInformed=false;
            }
        }
        bSInformedBK=bSInformed;
    }
}

int __fastcall TFormMain::isSoftwareValid(void)
{
	//TODO: Add your source code here
    int iret = 0;

 //��ʼ��ʱ����
    int iNums_NEED = 600;  //ʹ��600�κ�ʼ��ʾ������ϵ
    int iNums_STOP = 800;  //ʹ��800�κ󽫲�������ʹ��

    int iTime_NEED = 180;  //������6���º�ʼ��ʾ
    int iTime_STOP = 300;  //����  10���º�ֹͣʹ��

    TRegistry *Reg = new TRegistry();
    Reg->RootKey   = HKEY_LOCAL_MACHINE;

//������ʹ�õĿ��Ʒ����ǣ�����100���Ժ�ʼ��ʾ������200�κ�������
//        TDateTime dtTemp(usiHour,usiMinute,usiSecond, 0);
//        dtLevel   = dtTemp;

    TDateTime dtNowNow;
    dtNowNow = Now();
    int iTimeSpeed;
    iTimeSpeed = dtNowNow - dtSStartTime;   //����������ʹ������
    if(bFirstTime)
    {
        if((iSListNumber == 18)&&(iSBorderColor == 0x800000))
        {
            iListNumber = 18;
            clBorderColor = 0x800000;
        }
    }

    if((iListNumber != 18)              //���iListNumber = 18 ���� clBorderColor = 0x800000
        ||(clBorderColor != 0x800000)
      )
    {
        int iNumb, iMaxNumb;
        iNumb = iSSoftNumbers - 1971325;
        iNumb = iNumb - 31;
        iMaxNumb = (iNums_STOP + 1)*(iNums_STOP + 1);
        if((iNumb < 0)||(iNumb > iMaxNumb))
        {
            iNumb = iMaxNumb;
        }
        iNumb = int(sqrt(iNumb) + 0.5);

        if(   (iTimeSpeed > iTime_NEED)      //�����ﵽҪ��ʾ��ʱ��
            ||(dtNowNow   < dtLLastTime)    //�޸��˼������ʵʱʱ��
            ||(iNumb      > iNums_NEED)       //ʹ�ô����ﵽ��ʾ�Ĵ���
          )
        {
            FormRight = new TFormRight(FormMain);
            if(FormRight != NULL)
            {
                FormRight->ShowModal();
                if(FormRight->bResult)
                {
                    clBorderColor = 0x800000;
                    iListNumber   = 18;
                    iSBorderColor = 0x800000;
                    iDeleteOld    = 0x4000000;
                    iSListNumber  = 18;
                    delete FormRight;
                    FormRight = NULL;
                }
                else
                {
                    Application->MessageBox("�뼰ʱ�빩������ϵ!!!\n\n��ϵ�绰��\n\n(·��)021-74329,021-49713\n(�е�)010-51874329,010-51849713","��ʾ",MB_OK|MB_ICONINFORMATION);
                    delete FormRight;
                    FormRight = NULL;
                    if(  ( iTimeSpeed > iTime_STOP )    //�ﵽҪ��ʾ������
                       ||( dtNowNow   < dtLLastTime )    //��ǰ����С����һ�ε�����
                       ||( iNumb      >=  iNums_STOP )   //ʹ�ô�������Ҫ������ʾ�Ĵ���
                       ||( iDeleteOld == 0x8000000 )
                      )
                    {
                        iDeleteOld = 0x8000000;
                        bExitOut = true;
                        exit(-1);
                    }
                }
            }
            else
            {
                Close();
            }
        }
        iNumb ++;
        iNumb *= iNumb;
        iNumb += 31;
        iSSoftNumbers = iNumb + 1971325;
    }
    else
    {
        iret = 1000;    //����ʹ��
    }

    return iret;
}

void __fastcall TFormMain::MoveButton(int iPosi,bool bUpOrDown)
{
    //TODO: Add your source code here
    //iPosi: ��������ʾ�ǰ��˵�iPosi�İ���
    //bUpOrDown:�� ��ʾ�Ǻ��ϻ���  false :��    true:����

}

void __fastcall TFormMain::BitBtn7Click(TObject *Sender)
{
/*
    float fV,fDV,ftemp;
    float fS,fYP,fLT,fFI,fDI,fST,fMaxV,fMinV,fMinDV;
    float fMinDVPosi,fP0,fP,fDP,fV0,fMaxP,fT0,fT,fMaxT;
    int iPStartAngle;  //�����ŹرսǶ�
    int iPEndAngle;    //�����Ŵ򿪽Ƕ�
    float fMIndex;
    float fSpeed = 1000.0;
    float fSampleRate = 12000.0;  //����Ƶ��Ϊ10kHz
    float fDltAngle;
    float fMedia[100];
    int iP1Posi,iP2Posi,iP3Posi;
    bool bP1Posi=false,bP2Posi=false,bP3Posi=false;
    fMP = new float[2000];
    if(fMP == NULL)
    {
        return;
    }
    fMV = new float[2000];
    if(fMV == NULL)
    {
        return;
    }
    lDS.fDiameter =  StrToFloat(EditDiameter->Text)/1000.0;
    lDS.fSLength  =  StrToFloat(EditSLength->Text)/1000.0;
    lDS.fPodLength = StrToFloat(EditPodLength->Text)/1000.0;
    lDS.fCRatio   =  StrToFloat(EditCRatio->Text);
    lDS.fTWater = StrToFloat(EditTWater->Text);
    lDS.fTAir   = StrToFloat(EditTAir->Text);
    lDS.fPAir   = StrToFloat(EditPAir->Text);
    lDS.fInOpenAngle = StrToFloat(EditInOpenAngle->Text);
    lDS.fInCloseAngle = StrToFloat(EditInCloseAngle->Text);
    lDS.fOutOpenAngle = StrToFloat(EditOutOpenAngle->Text);
    lDS.fOutCloseAngle = StrToFloat(EditOutCloseAngle->Text);
    lDS.fTWall = StrToFloat(EditTWall->Text);
    lDS.fTTop  = StrToFloat(EditTTop->Text);
    lDS.fTBottom = StrToFloat(EditTBottom->Text);
    fMIndex = StrToFloat( EditMIndex->Text );
    fSampleRate = 1000.0*StrToFloat(EditSampleRate->Text);
    iPStartAngle = (int)(180.0+lDS.fInCloseAngle)*2.0;
    iPEndAngle   = (int)(540.0-lDS.fInOpenAngle)*2.0;
    fT0 = lDS.fTAir;
    fP0 = lDS.fPAir;
    fP = fP0;
    fMaxP = fP0;
    fMaxT = fT0;
    fDP = 0.0;
    fS = lDS.fSLength;
    fYP = lDS.fCRatio;
    fLT = lDS.fSLength/(2.0*lDS.fPodLength);
    fFI = 0.0;
    fDI = lDS.fDiameter;
    Chart1->Series[0]->Clear();
    Chart2->Series[0]->Clear();
    Chart1->LeftAxis->AutomaticMinimum = false;
    Chart1->LeftAxis->Minimum = 0;


//    Chart1->LeftAxis->Labels = true;
    Chart1->LeftAxis->Title->Caption = "�����ݻ� /��";
    Chart1->BottomAxis->Title->Caption = "����ת�� /CA";
    Chart2->LeftAxis->Title->Caption = "�����ݻ��仯��  / ��/CA";
    Chart2->BottomAxis->Title->Caption = "����ת�� /CA";

    Chart3->Series[0]->Clear();
//    Chart1->LeftAxis->Labels = true;
    Chart3->LeftAxis->Title->Caption = "����ѹ�� /bar";
    Chart3->BottomAxis->Title->Caption = "����ת�� /CA";
    Chart4->Series[0]->Clear();
    Chart4->LeftAxis->Title->Caption = "����ѹ���仯��  / bar/CA";
    Chart4->BottomAxis->Title->Caption = "����ת�� /CA";

    Chart5->Series[0]->Clear();
    Chart5->LeftAxis->Title->Caption = "�����¶�  /��";
    Chart5->BottomAxis->Title->Caption = "����ת�� /CA";

    for(int i=0;i<1440;i++)
    {

        ftemp = PI/180.0*fFI;
        fST = sin(ftemp);
        fV = PI*fDI*fDI/4.0*(fS/(fYP-1.0)+fS/2.0*((1.0+1.0/fLT)-cos(ftemp)-1.0/fLT*sqrt(1.0-fLT*fLT*fST*fST) ));
        fDV = PI*PI*fDI*fDI*fS/(8.0*180.0)*(fST+fLT/2.0*sin(2.0*ftemp)/sqrt(1.0-fLT*fLT*fST*fST));
        fV = fV*1000.0;
        fDV = fDV*1000.0;
        Chart1->Series[0]->AddXY(fFI,fV,"",clBlack);
        Chart2->Series[0]->AddXY(fFI,fDV,"",clBlue);

// �Ȱ����ȹ��̼������ѹ��
        if( (i>iPStartAngle) && (i<=iPEndAngle))
        {
            fP = fP0*pow(fV0/fV,fMIndex);
            fT = fP*fV*(fT+273.0)/(fP0*fV0);
            fT = fT -273.0;
            fDP = (fP - fP0)*2.0;
        }
        else
        {
            fP = fP0;
            fDP = 0.0;
            fT = fT0;
        }
        if( fMaxP<fP)
        {
            fMaxP = fP;
        }
        if(fMaxT < fT)
        {
            fMaxT = fT;
        }
        fMP[i] = fP;
        fMV[i/2] = fV;
        Chart3->Series[0]->AddXY(fFI,fP,"",clBlack);
//        Chart4->Series[0]->AddXY(fFI,fDP,"",clBlue);
        Chart5->Series[0]->AddXY(fFI,fT,"",clRed);
        if(i==0)
        {
            fMinV = fV;
            fMinDV = fDV;
        }
        else if(i == 360)
        {
            fMaxV = fV;
        }
        else if(i<720)
        {
            if(fDV<fMinDV)
            {
                fMinDV = fDV;
                fMinDVPosi = fFI;
            }
        }
        fFI = fFI + 0.5;
        fV0 = fV;
        fP0 = fP;
        fT0 = fT;
    }

    for(int i=0;i<1440;i++)
    {
        if(fMP[i]>10.0&&(!bP1Posi))
        {
            iP1Posi = i;
            bP1Posi = true;
        }
        if(fMP[i]>20.0&&(!bP2Posi))
        {
            iP2Posi = i;
            bP2Posi = true;
            break;
        }
    }
    fDltAngle = (iP2Posi - iP1Posi)/fSampleRate*6.0*fSpeed;
    for(int i=0;i<720;i++)
    {
         fMV[i]=fMV[i+250];
    }
    for(int i=0;i<100;i++)
    {
        fT = fMV[i]/fMV[i+(int)fDltAngle];
        fT = pow(fT,fMIndex);
        Chart4->Series[0]->AddXY(i,fT,"",clBlack);
        fMedia[i] = fT;
    }

    for(int i=0;i<100;i++)
    {
        if(fMedia[i]>2.0)
        {
            iP3Posi = i;
            break;
        }
    }
    iP3Posi = 110 - iP3Posi; //��P2�㿪ʼ�㣬��תiP3Posi������ת�Ǻ󵽴���ֹ��
    fDltAngle = (float)iP3Posi/(6.0*fSpeed)*fSampleRate;
    AnsiString strT = "��С�ݻ�Ϊ:";
    strT.printf("�����ݻ�������ת�ǵı仯:  ��С�ݻ���%.3f L; ����ݻ�:%.3f L",fMinV,fMaxV);
    Chart1->Title->Text->Strings[0] =strT;
    strT.printf("�ݻ��仯�����ֵΪ:%.3f L/�ȣ� λ����ֹ��ǰ%.2f��",fabs(fMinDV),360.0-fMinDVPosi);
    Chart2->Title->Text->Strings[0] = strT;
    strT.printf("����ѹ�����ߣ����ѹ��Ϊ%.2f bar",fMaxP);
    Chart3->Title->Text->Strings[0] = strT;
    Chart4->Title->Text->Strings[0] = "����ѹ���仯������";
    strT.printf("�����¶ȱ仯����, ����¶�Ϊ%.2f",fMaxT);
    Chart5->Title->Text->Strings[0] = strT;
    PageControlMain->ActivePageIndex = 0;
    strT.printf("����10bar���پ���%d��������󵽴���ֹ��\n\nP1���ڵ���Ϊ%d",(int)fDltAngle,iP1Posi);
    Application->MessageBox(strT.c_str(),"",MB_OK);
    delete []fMP;
    delete []fMV;
    fMP = NULL;
    fMV = NULL;
*/
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::BitBtnReadPClick(TObject *Sender)
{
/*
    float ftemp,ftemp0;
    fEE = StrToFloat(EditCRatio->Text);
    fStroke = StrToFloat(EditSLength->Text)/1000.0;
    fDiameter = StrToFloat(EditDiameter->Text)/1000.0;
    fLMDa = StrToFloat(EditSLength->Text)/StrToFloat(EditPodLength->Text);
    fLowPressure = StrToFloat(EditLowPressure->Text);
    int iIndex;
    iIndex = FileListBox1->ItemIndex;
    if(iIndex<0) return;
    gstrFileName = "";
    gstrFileName = FileListBox1->FileName;
    if(GetRealPressure())
    {
        Chart1->Series[0]->Clear();
        Chart2->Series[0]->Clear();
        Chart3->Series[0]->Clear();
        Chart4->Series[0]->Clear();
        Chart5->Series[0]->Clear();
        for(int i=0;i<iPLength;i++)
        {                                   //log
            Chart1->Series[0]->AddXY(fFFFai[i],fP[i],"",clBlack);
        }
        for(int i=1;i<iPLength;i++)
        {
            ftemp = (fP[i]-fP[i-1])/(fFFFai[i]-fFFFai[i-1]);
            Chart2->Series[0]->AddXY(fFFFai[i],ftemp,"",clBlack);
        }
        for(int i=0;i<iPLength;i++)
        {
            SmoothCurve(ftemp,i);
            fSP[i] = ftemp;
            Chart3->Series[0]->AddXY(fFFFai[i],ftemp,"",clBlack);
            if(i==0)
            {
                ftemp0 = ftemp;
            }
            else
            {
                float ftemp4;
                ftemp4 = ftemp;
                ftemp = (ftemp - ftemp0)/(fFFFai[i] - fFFFai[i-1]);
                Chart4->Series[0]->AddXY(fFFFai[i],ftemp,"",clBlack);
                ftemp0 = ftemp4;
            }
        }

        for(int i=0;i<iPLength/2;i++)
        {
        //    SmoothCurve(ftemp,i);
            float v1;
            v1 = GetVolumn(fFFFai[i],ftemp);
            Chart5->Series[0]->AddXY(log(v1),log(fSP[i]*1000000.0),"",clBlack);
*/
/*            if(i==0)
            {
                ftemp0 = ftemp;
            }
            else
            {
                float ftemp4;
                ftemp4 = ftemp;
                ftemp = (ftemp - ftemp0)/(fFFFai[i] - fFFFai[i-1]);
                Chart4->Series[0]->AddXY(fFFFai[i],ftemp,"",clBlack);
                ftemp0 = ftemp4;
            }
*/
/*        }

    }

    int inum = 0;
    float fA0,fA1;
    float ftempx,ftempy,fdelta;
    float fsumx = 0.0,fsumy = 0.0,fsumxy = 0.0,fsumx2 = 0.0;
    AnsiString strTemp;
    Memo1->Lines->Clear();
    for(int j=-10;j<11;j++)
    {
        fsumx = 0.0;
        fsumy = 0.0;
        fsumxy = 0.0;
        fsumx2 = 0.0;
        inum = 0;
        for(int i=90;i<iPLength/2-20;i++)
        {
            float v1,ftemp2;
            v1 = GetVolumn(fFFFai[i+j],ftemp2);
            inum++;
            ftempy = log(fSP[i]*1000000.0);
            ftempx = log(v1);
            fsumx += ftempx;
            fsumy += ftempy;
            fsumxy += ftempx*ftempy;
            fsumx2 += ftempx*ftempx;
        }
        fA1 = (fsumxy - fsumx*fsumy/(float)inum)/(fsumx2-fsumx*fsumx/(float)inum);
        fA0 = fsumy/(float)inum - fsumx/(float)(inum)*fA1;
        strTemp = "j = "+AnsiString(j);
        Memo1->Lines->Add(strTemp);
        strTemp = "y = "+AnsiString(fA0) + "+("+AnsiString(fA1)+")*x";
        Memo1->Lines->Add(strTemp);
        fdelta = 0.0;
        inum = 0;
        for(int i=90;i<iPLength/2-20;i++)
        {
            float v1,ftemp2;
            v1 = GetVolumn(fFFFai[i+j],ftemp2);
            inum++;
            ftempy = log(fSP[i]*1000000.0);
            ftempx = log(v1);
            fsumx = ftempy-(fA0+fA1*ftempx);
            fdelta +=fsumx*fsumx;
        }
        fdelta = sqrt(fdelta/(float)inum);
        strTemp = "Delta = "+AnsiString(fdelta);
        Memo1->Lines->Add(strTemp);
        Memo1->Lines->Add("-------------------------------");
        Memo1->Lines->Add("");
    }
    PageControlMain->ActivePageIndex = 0;
*/
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::FormActivate(TObject *Sender)
{
/*
    int iCount,iIndex;
    iCount = FileListBox1->Items->Count;
    if(iCount>=0)
    {
        FileListBox1->ItemIndex = 0;
    }
*/
    if(bStarted)
    {
        return;
    }
    FormTV = new TFormTV(FormMain);
    if( FormTV != NULL)
    {
        FormTV->HostDockSite = PanelTV;
        FormTV->Show();
    }
    ImageAll->Visible    = true;
    TabSheetParameter->TabVisible    = false;
    TabSheetDataBrowse->TabVisible   = false;
    TabSheetDataRecieve->TabVisible  = false;
    TabSheetOpenFile->TabVisible     = false;
    TabSheetCurveShow->TabVisible    = false;
    TabSheetDatabase->TabVisible     = false;

    TabSheetPCP->TabVisible          = false;
    TabSheetORG->TabVisible          = false;
    TabSheetPST->TabVisible          = false;
    TabSheet3->TabVisible            = false;
    TabSheet4->TabVisible            = false;
    TabSheet5->TabVisible            = false;
    TabSheetPipe->TabVisible         = false;
    TabSheetOilAngle->TabVisible     = false;
    TabSheet8->TabVisible            = false;
//    TabSheet8->TabVisible            = false;
    
    PageControl2->ActivePageIndex    = 0;

    PageControlMain->ActivePageIndex  = 4;
    PageControlCurve->ActivePageIndex = 2;
    ComboBoxIndex->ItemIndex         = 2;
    CurrentIndex                     = "\\*.PST";
//    ComboBoxIndex->Refresh();
    UpdateListView();
    SGDirectory->ColWidths[0] = 48;
    SGDirectory->ColWidths[1] = 64;
    SGDirectory->ColWidths[2] = 64;
    SGDirectory->ColWidths[3] = 36;
    SGDirectory->ColWidths[4] = 64;
    SGDirectory->ColWidths[5] = 64;
    SGDirectory->ColWidths[6] = 64;
    SGDirectory->ColWidths[7] = 64;
    SGDirectory->ColWidths[8] = 64;
    SGDirectory->ColWidths[9] = 140;

    SGDirectory->Cells[0][0] = "�ļ���";  //strTrainType;
    SGDirectory->Cells[1][0] = "��������";  //strTrainType;
    SGDirectory->Cells[2][0] = "��������";  //strTrainNumber;
    SGDirectory->Cells[3][0] = "�׺�";//strDieselNumber;
    SGDirectory->Cells[4][0] = "ͨ��һ";    //strPPSignalType[0];
    SGDirectory->Cells[5][0] = "ͨ����";    //strPPSignalType[1];
    SGDirectory->Cells[6][0] = "ͨ����";    //strPPSignalType[1];
    SGDirectory->Cells[7][0] = "����Ƶ��";   //strFrequency;
    SGDirectory->Cells[8][0] = "��������";  //strSampleLength;
    SGDirectory->Cells[9][0] = "����ʱ��";  //strMTime;

    SGParams->ColWidths[0] = 48;
    SGParams->ColWidths[3] = 48;
    SGParams->ColWidths[8] = 80;
/*
    SGParams->ColWidths[1] = 64;
    SGParams->ColWidths[2] = 64;
    SGParams->ColWidths[3] = 36;
    SGParams->ColWidths[4] = 64;
    SGParams->ColWidths[5] = 64;
    SGParams->ColWidths[6] = 64;
    SGParams->ColWidths[7] = 64;
    SGParams->ColWidths[8] = 64;
    SGParams->ColWidths[9] = 140;
    SGParams->ColWidths[9] = 140;
*/
    SGParams->Cells[0][0]  = "�ļ���";  //strTrainType;
    SGParams->Cells[1][0]  = "��������";  //strTrainType;
    SGParams->Cells[2][0]  = "��������";  //strTrainNumber;
    SGParams->Cells[3][0]  = "�׺�";//strDieselNumber;
    SGParams->Cells[4][0]  = "ת��";    //strPPSignalType[1];
    SGParams->Cells[5][0]  = "����״̬";    //strPPSignalType[0];
    SGParams->Cells[6][0]  = "ȼ��״̬";    //strPPSignalType[1];
    SGParams->Cells[7][0]  = "����ѹ";    //strPPSignalType[1];
    SGParams->Cells[8][0]  = "���ѹ����";   //strFrequency;
    SGParams->Cells[9][0]  = "����ʼ��";  //strSampleLength;
    SGParams->Cells[10][0] = "ȼ��ʼ��";  //strMTime;

    /*
//---------------------------------------------------------
// ������������еı������
    StringGridResult->ColWidths[0] = 24;
    StringGridResult->ColWidths[1] = 48;
    StringGridResult->ColWidths[2] = 48;
    StringGridResult->ColWidths[3] = 48;
    StringGridResult->ColWidths[4] = 58;

    StringGridResult->ColWidths[5] = 48;
    StringGridResult->ColWidths[6] = 48;
    StringGridResult->ColWidths[7] = 48;
    StringGridResult->ColWidths[8] = 48;
    StringGridResult->ColWidths[9] = 48;

    StringGridResult->ColWidths[10] = 48;
    StringGridResult->ColWidths[11] = 48;
    StringGridResult->ColWidths[12] = 48;
    StringGridResult->ColWidths[13] = 48;
    StringGridResult->ColWidths[14] = 48;
    for( int i = 0; i < 15; i ++)
    {
        StringGridResult->Cells[i][0] = strColumnTitle[i];
        StringGridResult->Cells[i][1] = strColumnTitle[i + 15];
        StringGridResult->Cells[i][2] = strColumnUnit[i];
    }
//=====================================================
 */
    EditPathName->Text       = strPresetPath;

// ��ʼ����·��
    try
    {
        if(DirectoryExists(strPresetPath))
        {
            if(DirectoryExists(strPSTFileDirectory))
            {
                FileListBoxFile->Directory  = strPSTFileDirectory;
            }
        }
    }
    catch(...)
    {
    }
    FilterComboBoxFile->ItemIndex = iFileExt;
    FileListBoxFile->Mask         = FilterComboBoxFile->Mask;
    FileListBoxFile->Update();

    PanelAll->Color    = clBackground;
    PanelSingle->Color = clBackground;
    ComboBoxPersonnel->Text = strPerson;
    if(!bStarted)
    {
        DatabaseOpen();
    }
    InitParameters();   //�Լ���ϵͳ�����ݽ��г�ʼ��
    Params_Load();
    bStarted = true;
}
//---------------------------------------------------------------------------

bool __fastcall TFormMain::SetParameters(void)
{
//TODO: Add your source code here
// ������Ĳ������벢���õ�������ȥ
// �������ϵ����������ø�����ϵͳ
/*
    lDS.SetfLowPressure(ftemp);
    lDS.SetfP0(ftemp);
    lDS.SetfPAir(ftemp);
    lDS.SetfT0(ftemp);
    lDS.SetfTAir(ftemp);
    lDS.SetfDFai(ftemp);
    lDS.SetfKConst(ftemp);
    lDS.SetfKCoef(ftemp);
    lDS.SetfHRConst(ftemp);
    lDS.SetfLCoef(ftemp);
    lDS.SetfNC(ftemp);
    lDS.SetfMH(ftemp);
    lDS.SetfFI(ftemp);
    lDS.SetfTAU(ftemp);
    lDS.SetfHU(ftemp);
    lDS.SetfGF(ftemp);
    lDS.SetfCAING(ftemp);
    lDS.SetfTF(ftemp);
    lDS.SetfLO(ftemp);
    lDS.SetfALPHA(ftemp);
    lDS.SetfRMOL(ftemp);
    lDS.SetfGAMA(ftemp);
    lDS.SetfX0(ftemp);
    float ft[3];
    ft[0] = 400.0 + 273.15;
    ft[1] = 400.0 + 273.15;
    ft[2] = 400.0 + 273.15;
    lDS.SetfTW(ft);
*/
    return true;

}
bool __fastcall TFormMain::InitParameters(void)
{
// �������ϵ����������ø�����ϵͳ
    //TODO: Add your source code here
    //�ӽ�������������������ģ��
    float ftemp;
    float fSLength,fPodLength;
    if(EditLowPressure->Text == "")
    {
        ftemp = 1.0;
    }
    else
    {
        ftemp = StrToFloat(EditLowPressure->Text);
    }
    lDS.SetfLowPressure(ftemp);

// �����ǲ��ͻ��ṹ���������ڴ˴�����
/*
//  1 ����ֱ��
    if(EditDiameter->Text == "")
    {
        ftemp = 0.24;
    }
    else
    {
        ftemp = StrToFloat(EditDiameter->Text)/1000.0;
    }
    lDS.SetfDiameter(ftemp);

//  2 �������
    if(EditSLength->Text == "")
    {
        ftemp = 0.275;
    }
    else
    {
        ftemp = StrToFloat(EditSLength->Text)/1000.0;
    }
    fSLength = ftemp;
    lDS.SetfStroke(ftemp);

//  3�����˳���
    if(EditPodLength->Text == "")
    {
        ftemp = 0.58;
    }
    else
    {
        ftemp = StrToFloat(EditPodLength->Text)/1000.0;
    }
    fPodLength = ftemp;
    lDS.SetfPodLength(ftemp);

//  4 ѹ����
    if(EditCRatio->Text == "")
    {
        ftemp = 12.4;
    }
    else
    {
        ftemp = StrToFloat(EditCRatio->Text);
    }
    lDS.SetfCRatio(ftemp);

//  5  ��������˳���֮��
    ftemp = fSLength / fPodLength;
    lDS.SetfLMDa(ftemp);

//  �����ŹرյĽǶ�,���������ֹ���ĽǶ�,��һ������������ʱΪ��ֹ��ǰ������Ϊ��ֵ
//  6�������ſ����Ƕ�
    if(EditInOpenAngle->Text == "")
    {
        ftemp = 50.0;
    }
    else
    {
        ftemp = StrToFloat(EditInOpenAngle->Text);
    }
    ftemp = -ftemp;
    lDS.SetfInOpenAngle(ftemp);

//  7 �����ŹرսǶ�
    if(EditInCloseAngle->Text == "")
    {
        ftemp = 42;
    }
    else
    {
        ftemp = StrToFloat(EditInCloseAngle->Text);
    }
    ftemp -= 180.0;
    lDS.SetfInCloseAngle(ftemp);

//  8 �����Ŵ򿪽Ƕ�
    if(EditOutOpenAngle->Text == "")
    {
        ftemp = 50.0;
    }
    else
    {
        ftemp = StrToFloat(EditOutOpenAngle->Text);
    }
    ftemp = 180.0 - ftemp;
    lDS.SetfOutOpenAngle(ftemp);

//  9�������ŹرսǶ�
    if(EditOutCloseAngle->Text == "")
    {
        ftemp = 50.0;
    }
    else
    {
        ftemp = StrToFloat(EditOutCloseAngle->Text);
    }
    lDS.SetfOutCloseAngle(ftemp);
*/

    if(EditSpeed->Text == "")
    {
        ftemp = 430.0;
    }
    else
    {
        ftemp = StrToFloat(EditSpeed->Text);
    }
    lDS.SetfEn(ftemp);

    if(EditPAir->Text == "")
    {
        ftemp = 250000.0;
    }
    else
    {
        ftemp = StrToFloat(EditPAir->Text)*100000.0; //������ѹ����ʼΪ����ѹ��
    }
    lDS.SetfP0(ftemp);
    lDS.SetfPAir(ftemp);

    if( EditTAir->Text == "")
    {
        ftemp = 50.0;
    }
    else
    {
        ftemp = StrToFloat(EditTAir->Text); //�������¶ȳ�ʼΪ�����¶�
    }
    ftemp += 273.15;
    lDS.SetfT0(ftemp);
    lDS.SetfTAir(ftemp);


    if(EditDFai->Text == "")
    {
        ftemp = 1.0;
    }
    else
    {
        ftemp = StrToFloat(EditDFai->Text);
    }
    lDS.SetfDFai(ftemp);

    //fDFai = 1.0;

    if(EditMIndex->Text == "")
    {
        ftemp = 1.438;
    }
    else
    {
        ftemp = StrToFloat(EditMIndex->Text);
    }
    lDS.SetfKConst(ftemp);

    if(EditKCoef->Text == "")
    {
        ftemp = 0.000105;
    }
    else
    {
        ftemp = StrToFloat(EditKCoef->Text);
    }
    lDS.SetfKCoef(ftemp);

    if(EditHRConst->Text == "")
    {
        ftemp = 0.02;
    }
    else
    {
        ftemp = StrToFloat(EditHRConst->Text);
    }
    lDS.SetfHRConst(ftemp);

    if(EditLCoef->Text == "")
    {
        ftemp = 0.0035;
    }
    else
    {
        ftemp = StrToFloat(EditLCoef->Text);
    }
    lDS.SetfLCoef(ftemp);

    if(EditfNC->Text == "")
    {
        ftemp = 8.0;
    }
    else
    {
        ftemp = StrToFloat(EditfNC->Text);
    }
    lDS.SetfNC(ftemp);

    if(EditfMH->Text == "")
    {
        ftemp = 18;
    }
    else
    {
        ftemp = StrToFloat(EditfMH->Text);
    }
    lDS.SetfMH(ftemp);

    if(EditfFI->Text == "")
    {
        ftemp = 16;
    }
    else
    {
        ftemp = StrToFloat(EditfFI->Text);
    }
    lDS.SetfFI(ftemp);

    if(EditfTAU->Text == "")
    {
        ftemp = 4.0;
    }
    else
    {
        ftemp = StrToFloat(EditfTAU->Text);
    }
    lDS.SetfTAU(ftemp);

    if(EditfHU->Text == "")
    {
        ftemp = 41868.0;
    }
    else
    {
        ftemp = StrToFloat(EditfHU->Text)*1000.0;
    }
    lDS.SetfHU(ftemp);

    if(EditfGF->Text == "")
    {
        ftemp = 1.2*0.8*0.001;
    }
    else
    {
        ftemp = StrToFloat(EditfGF->Text)*0.8*0.001; //��λ�ǹ���
    }
    lDS.SetfGF(ftemp);
//    fGuessFuel = fGF;

    if(EditfCAING->Text == "")
    {
        ftemp = -20.0;
    }
    else
    {
        ftemp = StrToFloat(EditfCAING->Text);
    }
    lDS.SetfCAING(ftemp);

    if(EditfTF->Text == "")
    {
        ftemp = 27.0;
    }
    else
    {
        ftemp = StrToFloat(EditfTF->Text);
    }
    lDS.SetfTF(ftemp);

    if(EditfLO->Text == "")
    {
        ftemp = 0.512;
    }
    else
    {
        ftemp = StrToFloat(EditfLO->Text);
    }
    lDS.SetfLO(ftemp);

    if(EditfALPHA->Text == "")
    {
        ftemp = 1.4;
    }
    else
    {
        ftemp = StrToFloat(EditfALPHA->Text);
    }
    lDS.SetfALPHA(ftemp);

    if(EditfRMOL->Text == "")
    {
        ftemp = 8314.0;
    }
    else
    {
        ftemp = StrToFloat(EditfRMOL->Text);
    }
    lDS.SetfRMOL(ftemp);

    if(EditfGAMA->Text == "")
    {
        ftemp = 0.01;
    }
    else
    {
        ftemp = StrToFloat(EditfGAMA->Text);
    }
    lDS.SetfGAMA(ftemp);

    if(EditfX0->Text == "")
    {
        ftemp = 0.005;
    }
    else
    {
        ftemp = StrToFloat(EditfX0->Text)/1000.0;
    }
    lDS.SetfX0(ftemp);

    float ft[3];
    ft[0] = 400.0 + 273.15;
    ft[1] = 400.0 + 273.15;
    ft[2] = 400.0 + 273.15;
    lDS.SetfTW(ft);
    return true;
}


void __fastcall TFormMain::mnuFileExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnPerformenceTestClick(TObject *Sender)
{
    PageControlMain->ActivePageIndex = 0;    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnResultClick(TObject *Sender)
{
    PageControlMain->ActivePageIndex = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnSetParametersClick(TObject *Sender)
{
    PageControlMain->ActivePageIndex = 0;    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnOperationParametersClick(TObject *Sender)
{
    PageControlMain->ActivePageIndex = 0;    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnReadFileClick(TObject *Sender)
{
    PageControlMain->ActivePageIndex = 2;    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmPFaiClick(TObject *Sender)
{
    //��Ҫ�����Ǹ�����ֹ��
//    void Change_tdc(int num,float *Ca,float *P,float tdc)
//     From File   Z_TP.cpp
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmPRateClick(TObject *Sender)
{
   //��Ҫ����ѹ�������ʣ����һ�ͼ   Z_TP.cpp 546 line
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmFrequencyClick(TObject *Sender)
{
  //  int dis_data1(char *fname)  ��Z_TP����غ���    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmBFilterClick(TObject *Sender)
{
  //	b3_smoflt(p,1024,atoi(buff));  ��Z_TP.CPP�еĺ���
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmDZFilterClick(TObject *Sender)
{
//	if((get_D_Z(513,fr,fi))==NULL) break;     ���ɴ��躯��
//		Back_fil(1024,1);                    //���˲� 
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmTranslatorFunctionClick(TObject *Sender)
{
//������ͨ��ʱ����������Ƶ�׷��������˼���
//    dis_data1(filename[0]);//�����ļ���������Ƶ�� ������ͨ������
//	  for(i=0;i<513;i++) {pr[i]=fr[i]-pr[i];fi[i]-=pi[i];fr[i]=i;}
//   ���ϴ����ǽ��м���
//�������Ƕ����ף������ǼӼ���ϵ��ʵ��ӦΪ�������λ�ǼӼ���ϵ
//  Ȼ����д洢����ʾ

}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ppmHeatReleaseClick(TObject *Sender)
{
//		if(Rohr.Get_Data()==0) break;     //��������
//   	char yes[2];
//	    status_line("����ѹ����ֵ��?", yes,1);
//		status_line("������������ļ�����", filename[0],1);
//		rohr1(toupper(yes[0]),filename[0]); ����//�������Ƥ
//		data_show();
//		file_Num=1;
//		break;
//    AnsiString str;
//    str.UpperCase();

}
//---------------------------------------------------------------------------


void __fastcall TFormMain::UpdateTreeView(void)
{
    //TODO: Add your source code here
/*
    TreeView1->Items->Clear();
    TTreeNode *rNode,*mNode;
    rNode = TreeView1->Items->Add(TreeView1->Selected,"�ҵĵ���");  //������ڵ�
    rNode->ImageIndex = 0;
    rNode->SelectedIndex = 0;
    mNode = TreeView1->Items->AddChild(rNode,"A:");
    mNode->ImageIndex = 4;
    mNode->SelectedIndex = 4;
    for(int i=1;i<Drive1->Items->Count;i++)
    {
        mNode = TreeView1->Items->AddChild(rNode,AnsiString(char('B'+i))+":");
        mNode->ImageIndex = 3;
        mNode->SelectedIndex = 3;
        AddDirectory(AnsiString(char('B'+i))+":",mNode);
    }


    AddDirectory("A:",mNode); //��������Ŀ¼
*/
}

void __fastcall TFormMain::AddDirectory(AnsiString path, TTreeNode * fNode)
{
    //TODO: Add your source code here
/*
    TSearchRec sr;
    TTreeNode *mNode;
    if(FindFirst(path + "\\*.*",faDirectory,sr) == 0)
    {
        if(sr.Attr == faDirectory)
        {
            if((sr.Name != ".") && (sr.Name != ".."))
            {
                mNode = TreeView1->Items->AddChild(fNode,sr.Name);
                mNode->ImageIndex = 1;
                mNode->SelectedIndex = 2;
                AddDirectory(path +"\\"+sr.Name,mNode);
            }
        }
    }
    while(FindNext(sr) == 0)
    {
        if(sr.Attr == faDirectory)
        {
            if((sr.Name != ".")&&(sr.Name != ".."))
            {
                mNode = TreeView1->Items->AddChild(fNode,sr.Name);
                mNode->ImageIndex = 1;
                mNode->SelectedIndex = 2;
                AddDirectory(path+"\\"+sr.Name,mNode);
            }
        }
    }
    FindClose(sr);
*/
}

void __fastcall TFormMain::UpdateListView(void)
{
    //TODO: Add your source code here
    ListView1->Items->Clear();
    ListViewAddDirectory();
    ListViewAddFile();
}

void __fastcall TFormMain::ListViewAddDirectory(void)
{
    //TODO: Add your source code here
    AnsiString strNowD = CurrentDir,strIndex = "\\*.*";
    strNowD = strNowD + strIndex;
    TListItem *mItem;
    TSearchRec sr;
    if(FindFirst(strNowD,faDirectory,sr) == 0)
    {
        if((sr.Attr == faDirectory)&&(sr.Name != ".")&&(sr.Name != ".."))
        {
            mItem = ListView1->Items->Add();
            mItem->ImageIndex = 0;
            mItem->Caption = sr.Name;
            mItem->SubItems->Add("");
            mItem->SubItems->Add("�ļ���");
            mItem->SubItems->Add("");
        }
    }
    while(FindNext(sr) == 0)
    {
        if((sr.Attr == faDirectory) && (sr.Name != ".")&&( sr.Name != ".."))
        {
            mItem = ListView1->Items->Add();
            mItem->Caption = sr.Name;
            mItem->ImageIndex = 0;
            mItem->SubItems->Add("");
            mItem->SubItems->Add("�ļ���");
            mItem->SubItems->Add("");
        }
    }
    FindClose(sr);
}

void __fastcall TFormMain::ListViewAddFile(void)
{
    //TODO: Add your source code here
    TListItem * mItem;
    TSearchRec sr;
    AnsiString fileext;
//    TFilterComboBox
    if(FindFirst(CurrentDir+CurrentIndex,0,sr)==0)
    {
        if(sr.Attr != faDirectory)
        {
            fileext = ExtractFileExt(sr.Name).LowerCase();
            if(fileext == ".exe")
            {
                mItem = ListView1->Items->Add();
                mItem->Caption = sr.Name;
                mItem->ImageIndex = 2;
                mItem->SubItems->Add(AnsiString(sr.Size));
                mItem->SubItems->Add("Ӧ�ó���");
                mItem->SubItems->Add("��ִ���ļ�");
            }
            else if(fileext == ".org")
            {
                mItem = ListView1->Items->Add();
                mItem->Caption = sr.Name;
                mItem->ImageIndex = 2;
                mItem->SubItems->Add(AnsiString(sr.Size));
                mItem->SubItems->Add(fileext + "ԭʼ����");
                mItem->SubItems->Add("�ֳ�������ԭʼ����");
            }
            else if(fileext == ".dat") //�����ļ�
            {

                fileext.Delete(1,1);
                mItem = ListView1->Items->Add();
                mItem->Caption = sr.Name;
                mItem->ImageIndex = 5;
                mItem->SubItems->Add(AnsiString(sr.Size));
                mItem->SubItems->Add(fileext+"�ļ�");
                mItem->SubItems->Add("�ı���ʽ�������ļ�");
            }
            else if(fileext == ".pcp") //�����ļ�
            {

                fileext.Delete(1,1);
                mItem = ListView1->Items->Add();
                mItem->Caption = sr.Name;
                mItem->ImageIndex = 7;
                mItem->SubItems->Add(AnsiString(sr.Size));
                mItem->SubItems->Add(fileext+"�ļ�");
                mItem->SubItems->Add("��ȡ������ѹ������");
            }
            else if(fileext == ".pst") //ѹ���б��ļ�
            {

                fileext.Delete(1,1);
                mItem = ListView1->Items->Add();
                mItem->Caption = sr.Name;
                mItem->ImageIndex = 6;
                mItem->SubItems->Add(AnsiString(sr.Size));
                mItem->SubItems->Add(fileext+"�ļ�");
                mItem->SubItems->Add("����ѹ���б��ļ�");
            }
             else if(fileext == ".txt") //ѹ���б��ļ�
            {

                fileext.Delete(1,1);
                mItem = ListView1->Items->Add();
                mItem->Caption = sr.Name;
                mItem->ImageIndex = 3;
                mItem->SubItems->Add(AnsiString(sr.Size));
                mItem->SubItems->Add(fileext+"�ļ�");
                mItem->SubItems->Add("�ı���ʽ�������ļ�");
            }
             else if(fileext == ".doc") //ѹ���б��ļ�
            {

                fileext.Delete(1,1);
                mItem = ListView1->Items->Add();
                mItem->Caption = sr.Name;
                mItem->ImageIndex = 4;
                mItem->SubItems->Add(AnsiString(sr.Size));
                mItem->SubItems->Add(fileext+"�ļ�");
                mItem->SubItems->Add("Word�ĵ�");
            }
            else //ѹ���б��ļ�
            {

                fileext.Delete(1,1);
                mItem = ListView1->Items->Add();
                mItem->Caption = sr.Name;
                mItem->ImageIndex = 8;
                mItem->SubItems->Add(AnsiString(sr.Size));
                mItem->SubItems->Add(fileext+"�ļ�");
                mItem->SubItems->Add("�в�֪���ļ�����");
            }
        }
        while(FindNext(sr) == 0)
        {
            if(sr.Attr != faDirectory)
            {
                fileext = ExtractFileExt(sr.Name).LowerCase();
                if(fileext == ".exe")
                {
                    mItem = ListView1->Items->Add();
                    mItem->Caption = sr.Name;
                    mItem->ImageIndex = 2;
                    mItem->SubItems->Add(AnsiString(sr.Size));
                    mItem->SubItems->Add("Ӧ�ó���");
                    mItem->SubItems->Add("��ִ���ļ�");
                }
                 else if(fileext == ".org")
                {
                    mItem = ListView1->Items->Add();
                    mItem->Caption = sr.Name;
                    mItem->ImageIndex = 2;
                    mItem->SubItems->Add(AnsiString(sr.Size));
                    mItem->SubItems->Add(fileext + "ԭʼ����");
                    mItem->SubItems->Add("�ֳ�������ԭʼ����");
                }
                else if(fileext == ".dat") //�����ļ�
                {

                    fileext.Delete(1,1);
                    mItem = ListView1->Items->Add();
                    mItem->Caption = sr.Name;
                    mItem->ImageIndex = 5;
                    mItem->SubItems->Add(AnsiString(sr.Size));
                    mItem->SubItems->Add(fileext+"�ļ�");
                    mItem->SubItems->Add("�ı���ʽ�������ļ�");
                }
                else if(fileext == ".pcp") //�����ļ�
                {

                    fileext.Delete(1,1);
                    mItem = ListView1->Items->Add();
                    mItem->Caption = sr.Name;
                    mItem->ImageIndex = 7;
                    mItem->SubItems->Add(AnsiString(sr.Size));
                    mItem->SubItems->Add(fileext+"�ļ�");
                    mItem->SubItems->Add("��ȡ������ѹ�������ļ�");
                }
                else if(fileext == ".pst") //ѹ���б��ļ�
                {

                    fileext.Delete(1,1);
                    mItem = ListView1->Items->Add();
                    mItem->Caption = sr.Name;
                    mItem->ImageIndex = 6;
                    mItem->SubItems->Add(AnsiString(sr.Size));
                    mItem->SubItems->Add(fileext+"�ļ�");
                    mItem->SubItems->Add("����ѹ���б��ļ�");
                }
                 else if(fileext == ".txt") //ѹ���б��ļ�
                {

                    fileext.Delete(1,1);
                    mItem = ListView1->Items->Add();
                    mItem->Caption = sr.Name;
                    mItem->ImageIndex = 3;
                    mItem->SubItems->Add(AnsiString(sr.Size));
                    mItem->SubItems->Add(fileext+"�ļ�");
                    mItem->SubItems->Add("�ı��ļ�");
                }
                 else if(fileext == ".doc") //ѹ���б��ļ�
                {
                    fileext.Delete(1,1);
                    mItem = ListView1->Items->Add();
                    mItem->Caption = sr.Name;
                    mItem->ImageIndex = 4;
                    mItem->SubItems->Add(AnsiString(sr.Size));
                    mItem->SubItems->Add(fileext+"�ļ�");
                    mItem->SubItems->Add("Word�ĵ�");
                }
                else //���������͵��ļ�
                {

                    fileext.Delete(1,1);
                    mItem = ListView1->Items->Add();
                    mItem->Caption = sr.Name;
                    mItem->ImageIndex = 8;
                    mItem->SubItems->Add(AnsiString(sr.Size));
                    mItem->SubItems->Add(fileext+"�ļ�");
                    mItem->SubItems->Add("�в�֪���ļ�����");
                }
            }
        }
    }
    FindClose(sr);
}
void __fastcall TFormMain::Drive1Change(TObject *Sender)
{
    if(!bDriveInited)
    {
        bDriveInited = true;
        return;
    }
    CurrentDir = Drive1->Drive;
    CurrentDir +=":";
    UpdateListView();
    ComboBox1->Items->Add(CurrentDir);
    ComboBox1->Text = CurrentDir;
///    DWORD Total,Free;
//    Total = DiskSize(Drive1->Drive - 'A' +1);
//    Free =  DiskFree(Drive1->Drive - 'A' +1);    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TreeView1Click(TObject *Sender)
{
/*
    AnsiString dName[20],dir;
    int i=0, j;
    TTreeNode *mNode = TreeView1->Selected;
    if(mNode->Text != "�ҵĵ���")
    {
        if(mNode != NULL)
        {
            do
            {
                dName[i] = mNode->Text;
                mNode = mNode->Parent;
                i++;
            }while(mNode->Text != "�ҵĵ���");
            dir = dName[i-1];
            CurrentDir = dir;
        }
        UpdateListView();
    }
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ListView1DblClick(TObject *Sender)
{
    AnsiString FileStr;
    if(ListView1->Selected == NULL)
    {
        return;
    }
    FileStr = CurrentDir + "\\" + ListView1->Selected->Caption;
//    int ilen = FileStr.Length();
    ComboBox1->Text = FileStr;
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;    // Show hourglass cursor
    try
    {
        Screen->Cursor = crHourGlass;
        FillListClick();
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }

}
//---------------------------------------------------------------------------



void __fastcall TFormMain::FillListClick(void)
{
    //TODO: Add your source code here
    if(ListView1->SelCount == 1)
    {
        if(ListView1->Selected->ImageIndex ==0)
        {
            CurrentDir += "\\"+ListView1->Selected->Caption;
            UpdateListView();
        }
        else
        {
            AnsiString FileStr,strTemp;
            FileStr = CurrentDir + "\\" + ListView1->Selected->Caption;
            strTemp = ExtractFileExt(FileStr);
            strTemp = strTemp.UpperCase();
            if((strTemp != ".PCP")&&(strTemp != ".PST")&&(strTemp != ".ORG"))
            {
                ShellExecute(Handle,"open",FileStr.c_str(),
                         NULL,NULL,SW_SHOWNORMAL);
            }
            else
            {
                if(strTemp == ".PST")
                {
                    bLoadList = true;
                    bAllHRR = false;
                    strPSTName = FileStr;
                    if( FileExists( strPSTName ) )
                    {
                        ToolButton24->Down = false;
                        strValidPath = ExtractFilePath( strPSTName );
                        ListBoxPSTFile->Items->Add(strPSTName);
                        ListBoxPSTFile->Refresh();
                        ListBoxPCPFile->Items->LoadFromFile(strPSTName);
                        ListBoxPCPFile->Refresh();
                        iListDrawKind = 1;
                        int icount = ListBoxPSTFile->Items->Count;
                        ListBoxPSTFile->ItemIndex = icount - 1;
                        LoadCurrentBinPST();
                    }
//                    LoadCurrentPST();
                }
                else if(strTemp == ".PCP")
                {
                    strPCPName = FileStr;
//                    ToolButton26->Down = true;
//                    ToolButton26->Refresh();
                    LoadCurrentBinPCP(false);
                }
                else if(strTemp == ".ORG")
                {
                    strOriginalName = FileStr;
//                    ToolButton23->Down = true;
//                    ToolButton23->Refresh();
                    LoadCurrentBinORG();
//                    LoadCurrentOriginal();

                }
            }
        }
    }
}

void __fastcall TFormMain::ToolButton1Click(TObject *Sender)
{
    int i = CurrentDir.Length();
    while(i>0)
    {
        if(CurrentDir[i] == '\\')
        {
            CurrentDir.Delete(i,CurrentDir.Length()-i+1);
            UpdateListView();
            break;
        }
        i--;
    }
    ComboBox1->Text = CurrentDir;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButtonCPowerClick(TObject *Sender)
{
    if(AllDrawed(Sender,drawW))
    {
        return;
    }

    TCursor Save_Cursor = Screen->Cursor ;
    try
    {
        Screen->Cursor = crHourGlass;
        if(!DetectPSTStatus())
        {
            return;
        }
        bFileChanged = true;
//        PaintBoxAll->Refresh();
        DrawImageAll();
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtn1Click(TObject *Sender)
{
/*
    FormDirectory = new TFormDirectory(FormMain);
    if( FormDirectory == NULL)
    {
        return;
    }
    FormDirectory->strCurrentDir = CurrentDir;
    FormDirectory->ShowModal();
    delete FormDirectory;
    FormDirectory = NULL;
*/
/*
    OpenDialog1->Title = "����ļ���";
    OpenDialog1->FileName = "default.pcp";
    OpenDialog1->InitialDir = CurrentDir;
    if(OpenDialog1->Execute()) //ִ���˴򿪲���
    {
        AnsiString strTemp;
        int ilen;
        strTemp = OpenDialog1->FileName;
        CurrentDir = ::ExtractFilePath(strTemp);
        ilen = CurrentDir.Length();
        if(ilen>0)
        {
            CurrentDir = CurrentDir.SubString(1,ilen-1);
        }
        ComboBox1->Text = CurrentDir;
        ComboBox1->Items->Add(CurrentDir);
        UpdateListView();
        // TListView
  //    ListView1->Selected->Index = 0;
    }
*/
    AnsiString oldDir = CurrentDir;
    if(BrowseForFolder("��ѡ���ļ���","�ҵĵ���",oldDir))
    {
        if( (oldDir != "")&&( oldDir != CurrentDir))
        {
            int length;
            length = oldDir.Length();
            if((length == 3)&&(oldDir.c_str()[2] == '\\'))
            {
                oldDir = oldDir.SubString(1,2);
            }

            CurrentDir = oldDir;
            ComboBox1->Text = CurrentDir;
            ComboBox1->Items->Add(CurrentDir);
            UpdateListView();
        }
    }

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBox1Change(TObject *Sender)
{
    CurrentDir = ComboBox1->Text;
    UpdateListView();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxIndexChange(TObject *Sender)
{
    int iIndex ;
    iIndex = ComboBoxIndex->ItemIndex;
    switch(iIndex)
    {
        case 0:
            CurrentIndex ="\\*.ORG";
            break;
        case 1:
            CurrentIndex ="\\*.PCP";
            break;
        case 2:
            CurrentIndex ="\\*.PST";
            break;
        default:
            CurrentIndex ="\\*.PST";
            break;
    }
    UpdateListView();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtn3Click(TObject *Sender)
{
    PageControlMain->ActivePageIndex = 3;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionOperationScanDiskExecute(TObject *Sender)
{
    PageControlMain->ActivePageIndex = 3;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionOperationSetupExecute(TObject *Sender)
{
    ToolButton23Click(ToolButton23);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionProcessHeadReleaseExecute(TObject *Sender)
{
    PageControlMain->ActivePageIndex = 1;
    FunHeatRelease();   
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::FunHeatRelease(void)
{
    //TODO: Add your source code here
    if(!InitParameters())
    {
        return;
    }
/*  �˴��Ĵ����Ѹ��ڹ��캯����
    if(!lDS.GetfU(strNowPath.c_str()))
    {
        return;
    }
*/

    if(!lDS.SetRealPressure(strPCPName.c_str()))
    {
        return;
    }
    PageControlMain->ActivePageIndex = 1;
    Chart1->LeftAxis->AutomaticMinimum = true;
//  Chart1->LeftAxis->Minimum = 0.0;
    Chart1->Title->Text->Strings[0] = "����ѹ������";
    Chart1->BottomAxis->Title->Caption = "����ת��(CA)";
    Chart1->LeftAxis->Title->Caption = "����ѹ��(bar)";
    Chart1->Series[0]->Clear();
    Chart1->Series[1]->Clear();
    Chart1->Series[2]->Clear();
    Chart1->Series[3]->Clear();
    Chart1->Series[4]->Clear();
    Chart1->Series[5]->Clear();
    Chart1->Series[6]->Clear();
    Chart1->Series[7]->Clear();
     //����ѹ��������
    lDS.ProcessPressure(1);    //ԭʼ���ݼ���
    lDS.CalculateLogVLogP(); //���������ݻ���LogV��LogP
    lDS.CalculateDPDF();

    lDS.TJHeatRelease();
    lDS.CalculateCHRR();
    Chart2->Title->Text->Strings[0] = "����������";
    Chart2->BottomAxis->Title->Caption = "����ת��(CA)";
    Chart2->LeftAxis->Title->Caption = "������";
    Chart2->Series[0]->Clear();
    Chart2->Series[1]->Clear();
    Chart2->Series[2]->Clear();
    Chart2->Series[3]->Clear();
    for(int i=lDS.icStartPosi ;i<lDS.icEndPosi ;i++)
    {
        Chart1->Series[0]->AddXY(lDS.fFFFai[i],lDS.fDHDF[i],"",clRed);
        Chart1->Series[1]->AddXY(lDS.fFFFai[i],lDS.farDQE[i],"",clGreen);
        Chart1->Series[2]->AddXY(lDS.fFFFai[i],lDS.farDW[i],"",clBlue);
        Chart1->Series[3]->AddXY(lDS.fFFFai[i],lDS.farDQW[i],"",clBlack);
        Chart1->Series[4]->AddXY(lDS.fFFFai[i],lDS.fCDHDF[i],"",clRed);
        Chart1->Series[5]->AddXY(lDS.fFFFai[i],lDS.farQE[i],"",clGreen);
        Chart1->Series[6]->AddXY(lDS.fFFFai[i],lDS.farW[i],"",clBlue);
        Chart1->Series[7]->AddXY(lDS.fFFFai[i],lDS.farQW[i],"",clBlack);
    }
/*
    for(int i=lDS.icStartPosi ;i<lDS.icEndPosi ;i++)
    {
        Chart2->Series[0]->AddXY(lDS.fFFFai[i],lDS.fDHDF[i],"",clRed);
    }
    lDS.ZYFHeatRelease();
    for(int i=lDS.icStartPosi;i<lDS.icEndPosi;i++)
    {
        Chart2->Series[1]->AddXY(lDS.fFFFai[i],lDS.fDHDF[i],"",clGreen);
    }
    lDS.LrwHeatRelease();
    for(int i=lDS.icStartPosi;i<lDS.icEndPosi;i++)
    {
        Chart2->Series[3]->AddXY(lDS.fFFFai[i],lDS.fDHDF[i],"",clBlack);
    }
*/
    lDS.CarsHeatRelease();
    for(int i=lDS.icStartPosi;i<lDS.icEndPosi;i++)
    {
        Chart2->Series[0]->AddXY(lDS.fFFFai[i],lDS.fDHDF[i],"",clRed);
        Chart2->Series[1]->AddXY(lDS.fFFFai[i],lDS.farDQE[i],"",clGreen);
        Chart2->Series[2]->AddXY(lDS.fFFFai[i],lDS.farDW[i],"",clBlue);
        Chart2->Series[3]->AddXY(lDS.fFFFai[i],lDS.farDQW[i],"",clBlack);
    }

    /*
    lDS.ProcessPressure(1);   //�ߵ��˳����
    for(int i=0;i<lDS.iPLength;i++)
    {
        Chart1->Series[1]->AddXY(lDS.fFFFai[i],lDS.fNewP[i],"",clRed);
    }
    lDS.CalculateLogVLogP(); //���������ݻ���LogV��LogP
    lDS.CalculateDPDF();
    if(iCalculateMethod == 0)
    {
        lDS.TJHeatRelease();
    }
    else if(iCalculateMethod == 1)
    {
        lDS.ZYFHeatRelease();
    }
    else if( iCalculateMethod == 2)
    {
        lDS.CarsHeatRelease();
    }
    for(int i=0;i<lDS.iPLength;i++)
    {
        Chart2->Series[1]->AddXY(lDS.fFFFai[i],lDS.fDHDF[i],"",clRed);
    }

    lDS.SetiIteration(4);
    lDS.ProcessPressure(2); //�����ظ�ƽ��
    for(int i=0;i<lDS.iPLength;i++)
    {
        Chart1->Series[2]->AddXY(lDS.fFFFai[i],lDS.fNewP[i],"",clBlue);
    }
    lDS.CalculateLogVLogP(); //���������ݻ���LogV��LogP
    lDS.CalculateDPDF();
    if(iCalculateMethod == 0)
    {
        lDS.TJHeatRelease();
    }
    else if(iCalculateMethod == 1)
    {
        lDS.ZYFHeatRelease();
    }
    else if( iCalculateMethod == 2)
    {
        lDS.CarsHeatRelease();
    }
    for(int i=0;i<lDS.iPLength;i++)
    {
        Chart2->Series[2]->AddXY(lDS.fFFFai[i],lDS.fDHDF[i],"",clBlue);
    }
*/
//   Application->MessageBox("�����ʼ������!","��ʾ",MB_OK|MB_ICONINFORMATION);
}

void __fastcall TFormMain::ActionOperationReadDataExecute(TObject *Sender)
{
    PageControlMain->ActivePageIndex = 2;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::LMouseDown(TObject* Sender, TMouseButton Button, TShiftState Shift,TRect &rect, int X, int Y)
{
	//TODO: Add your source code here

    if(Button == mbLeft)  //������µ���������
    {
        TPoint pointMouse;
        pointMouse.x = X;
        pointMouse.y = Y;
       if(PtInRect(&rect,pointMouse))
       {
//            if(bGridLine&&bBackMousePoint)
            if(bBackMousePoint)
            {
                 int px,py;
                 px = pointMouseBackup.x;
                 py = pointMouseBackup.y;
//                 rl = rect.left;
//                 rb = rect.bottom;
                 TPaintBox*  ptPB = (TPaintBox*)Sender;
                 ptPB->Canvas->Pen->Color = clGridLineColor;
                 ptPB->Canvas->Pen->Mode  = pmNot;
//                 ptPB->Canvas->Pen->Style = (TPenStyle)LineType[iGridLineTypeIndex];
/*
                 ptPB->Canvas->MoveTo(rl,py);
                 ptPB->Canvas->LineTo(px,py);
                 ptPB->Canvas->MoveTo(px,py);
                 ptPB->Canvas->LineTo(px,rb);
*/
                 ptPB->Canvas->Ellipse(px-iCircleDim,py-iCircleDim,px+iCircleDim,py+iCircleDim);
                 if(bCursorLine)
                 {
                     ptPB->Canvas->MoveTo(iXMin, py);
                     ptPB->Canvas->LineTo(iXMax, py);
                     ptPB->Canvas->MoveTo(px,    iYMin);
                     ptPB->Canvas->LineTo(px,    iYMax);
                 }
                 bBackMousePoint = false;
            }
            pointMouseOrigin.x = X;
            pointMouseOrigin.y = Y;
            pointMouseOld.x    = X;
            pointMouseOld.y    = Y;
            bPBMouseDragging   = true;
       }
    }
}

void __fastcall TFormMain::LMouseMove(TObject * Sender, TShiftState Shift,TRect &rect, int X, int Y)
{
	//TODO: Add your source code here
    int posX,posY;
    TPoint pointMouse;
    pointMouse.x    = X;
    pointMouse.y    = Y;
    TPaintBox* ptPB = (TPaintBox*)Sender;
    if(bDrawed)
    {
        ptPB->Canvas->Pen->Color  = clGridLineColor;
        ptPB->Canvas->Pen->Mode   = pmNot;
//        ptPB->Canvas->Pen->Style  = (TPenStyle)LineType[iGridLineTypeIndex];
        if(bPBMouseDragging)
        {
            if(PtInRect(&rect,pointMouse))
            {
                PBDrawRect(Sender,pointMouseOrigin,pointMouseOld);
          //     ptPB->Canvas->Pen->Mode=pmNot;
                PBDrawRect(Sender,pointMouseOrigin,pointMouse);
                pointMouseOld.x = X;
                pointMouseOld.y = Y;
                posX = iCommonStart + (int)((float)(pointMouseOrigin.x - rect.left)/fXStep + 0.5);
                posY = iCommonStart + (int)((float)(pointMouse.x - rect.left)/fXStep + 0.5);
            }
        }
        else
        {
            if(PtInRect(&rect,pointMouse))
            {
                if(bBackMousePoint)
                {
                   int px,py;
//                   int rl,rb;
                   px = pointMouseBackup.x;
                   py = pointMouseBackup.y;
//                   rl = rect.left;
//                   rb = rect.bottom;
                   ptPB->Canvas->Ellipse(    px - iCircleDim,
                                             py - iCircleDim,
                                             px + iCircleDim,
                                             py + iCircleDim
                                         );
                   if(bCursorLine)
                   {
                        ptPB->Canvas->MoveTo(iXMin, py);
                        ptPB->Canvas->LineTo(iXMax, py);
                        ptPB->Canvas->MoveTo(px,    iYMin);
                        ptPB->Canvas->LineTo(px,    iYMax);
                   }
                }
                posX = iCommonStart + (int)((float)(X - rect.left)/fXStep + 0.5);
                posY = iMiddle - (int)(fYData[posX]*fYStep);
                char strr[30];
                AnsiString strt;
                sprintf(strr," X = %.2f,",fXData[posX]);
                strt = strr;
                sprintf(strr,"Y = %.3f ",fYData[posX]);
                strt += strr;
                StatusBarMain->Panels->Items[1]->Text = strt;
                StatusBarMain->Refresh();
                ptPB->Canvas->Ellipse(X-iCircleDim,posY-iCircleDim,X+iCircleDim,posY+iCircleDim);
                if(bCursorLine)
                {
                    ptPB->Canvas->MoveTo(iXMin, posY);
                    ptPB->Canvas->LineTo(iXMax, posY);
                    ptPB->Canvas->MoveTo(X,    iYMin);
                    ptPB->Canvas->LineTo(X,    iYMax);
                }
                pointMouseBackup.x = X;
                pointMouseBackup.y = posY;
                bBackMousePoint    = true;
            }
            else
            {
                if(bBackMousePoint)
                {
                    int px,py;
//                    int rl,rb;
                    px = pointMouseBackup.x;
                    py = pointMouseBackup.y;

//                    rl = rect.left;
//                    rb = rect.bottom;

                    ptPB->Canvas->Ellipse(  px - iCircleDim,
                                            py - iCircleDim,
                                            px + iCircleDim,
                                            py + iCircleDim
                                         );
                    if(bCursorLine)
                    {
                        ptPB->Canvas->MoveTo(iXMin, py);
                         ptPB->Canvas->LineTo(iXMax, py);
                         ptPB->Canvas->MoveTo(px,    iYMin);
                         ptPB->Canvas->LineTo(px,    iYMax);
                    }
                }
                StatusBarMain->Panels->Items[1]->Text = "";
                StatusBarMain->Refresh();
                bBackMousePoint = false;
            }
        }
    }
}
void __fastcall TFormMain::LMouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift,TRect &rect, int X, int Y)
{
    TPaintBox* ptPB = (TPaintBox*)Sender;
    if(Button == mbLeft)
    {
        int iStart,iEnd;
        TPoint pointMouse;
        pointMouse.x = X;
        pointMouse.y = Y;
        bPBMouseDragging = false;
        ptPB->Canvas->Pen->Color = clGridLineColor;
 //       ptPB->Canvas->Pen->Style = (TPenStyle)LineType[iGridLineTypeIndex];
        if(PtInRect(&rect,pointMouse))
        {
            ptPB->Canvas->Pen->Mode=pmNot;
            PBDrawRect(Sender,pointMouseOrigin,pointMouseOld);
            ptPB->Canvas->Pen->Mode=pmCopy;
            PBDrawRect(Sender,pointMouseOrigin,pointMouse);
            if(fXStep<0.001) return;
            iStart = iCommonStart + (int)((float)(pointMouseOrigin.x-rect.left)/fXStep+0.5);
            iEnd = iCommonStart + (int)((float)(X-rect.left)/fXStep+0.5);
            if((iEnd-iStart)<0)
            {
                int ittemp;
                ittemp=iStart;
                iStart=iEnd;
                iEnd=ittemp;

            }
            if((iEnd-iStart)>10)
            {
                if(!bDrawPart)
                {
                    iChoiceStart=iStart;
                    iChoiceEnd=iEnd;
                }
                else
                {
                    iCCStart = iStart;
                    iCCEnd = iEnd;
                }
                    bBackMousePoint=false;
            }
            else
            {
//                if(iChoiceEnd == iDataLength)
//                {
//                    return;
//                }
                if(!bDrawPart)
                {
                    iChoiceStart = 0;
                    iChoiceEnd = lDS.iPLength;
                }
                else
                {
                    iCCStart = lDS.iStartFai;
                    iCCEnd = lDS.iEndFai;
                }
		//        ScrBDraw->Max=0;
               //	 ptPB->Refresh();
        		 bBackMousePoint=false;

            }
//            bLookup=false;
            ptPB->Refresh();
//            ScrBDraw->Position=(iChoiceEnd+iChoiceStart)/2;

        }
        bBackMousePoint = false;
        Application->ProcessMessages();
    }
}

void __fastcall TFormMain::PBDrawRect(TObject* Sender,TPoint& LT, TPoint& TB)
{
        //TODO: Add your source code here
	   TPaintBox* ptb=(TPaintBox*)Sender;
       ptb->Canvas->MoveTo(LT.x,LT.y);
       ptb->Canvas->LineTo(LT.x,TB.y);
       ptb->Canvas->LineTo(TB.x,TB.y);
       ptb->Canvas->LineTo(TB.x,LT.y);
       ptb->Canvas->LineTo(LT.x,LT.y);
}

bool __fastcall TFormMain::InitMouseData(void)
{
    //TODO: Add your source code here
    if((!bFileOpened)&&(!bAdjustHRR)&&(!bLoadOriginal))
    {
       return false;
    }
    if(!bDrawed)
    {
        return false;
    }
    fYData = NULL;
    fXData = NULL;

    if(bRectSelected[0]&&( iAdjustDrawNum == 1)) //ԭʼ����
    {

        fXData = lDS.fFFFai;
        fYData = lDS.fOldP;
        iCommonStart = iChoiceStart;
        iCommonEnd = iChoiceEnd;
        bDrawPart = false;
    }
    else if(bRectSelected[1]&&( iAdjustDrawNum == 1)) //������������
    {

        fXData = lDS.fFFFai;
        fYData = lDS.fNewP;
        iCommonStart = iChoiceStart;
        iCommonEnd = iChoiceEnd;
        bDrawPart = false;
    }
    else if(bRectSelected[4]&&( iAdjustDrawNum == 1))//ѹ��������
    {
        fXData = lDS.fFFFai;
        fYData = lDS.farDPDF;
        iCommonStart = iChoiceStart;
        iCommonEnd = iChoiceEnd;
        bDrawPart = false;
    }
    else if(bRectSelected[5]&&( iAdjustDrawNum == 1)) //������
    {

        fXData = lDS.fFFFai;
        fYData = lDS.fDHDF;
        iCommonStart = iCCStart;
        iCommonEnd = iCCEnd;
        bDrawPart = true;
    }
    else if(bRectSelected[6]&&( iAdjustDrawNum == 1))//�ۻ�������
    {
        fXData = lDS.fFFFai;
        fYData = lDS.fCDHDF;
        iCommonStart = iCCStart;
        iCommonEnd = iCCEnd;
        bDrawPart = true;
    }
    else if(bRectSelected[7]&&( iAdjustDrawNum == 1)) //������
    {
        fXData = lDS.farFFTPr;
        fYData = lDS.farFFTPi;
        iCommonStart = iChoiceStart;
        iCommonEnd = iChoiceEnd;
        bDrawPart = false;
    }
    else if(bRectSelected[8]&&( iAdjustDrawNum == 1))//�����¶�
    {
        fXData = lDS.fFFFai;
        fYData = lDS.fTT;
        iCommonStart = iCCStart;
        iCommonEnd = iCCEnd;
        bDrawPart = true;

    }
    if((fYData == NULL)||(fXData == NULL))
    {
        return false;
    }
    return true;
}

void __fastcall TFormMain::PaintBoxAdjustTDCMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

    if(   (Button      == mbRight)  //������µ���������
      )
    {
        if(  ( CheckBoxDrawPipeWave->Checked )
           ||( CheckBoxAdjustPT->Checked ))
        {
            PaintBoxAdjustTDC->PopupMenu = PopupMenuInsert;
        }


        return;
    }
    if(InitMouseData())
    {
        LMouseDown(Sender,Button,Shift,rectA,X,Y);
    }
    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxAdjustTDCMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{

    if(InitMouseData())
    {
        LMouseMove(Sender,Shift,rectA,X,Y);
    }

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxAdjustTDCMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if( (Button == mbRight) )
    {
        return;
    }
    bCurveModi = true;
    if(InitMouseData())
    {
        LMouseUp(Sender,Button,Shift,rectA,X,Y);
        bPBMouseDragging = false;
        TPaintBox * ptPB = (TPaintBox*)Sender;
        int max,itemp;
        if(!bDrawPart)
        {
            max   = lDS.iPLength -(iChoiceEnd - iChoiceStart);
            itemp = max/10;
            if(itemp == 0)
            {
                itemp = 1;
            }
        }
        else
        {
            max   = lDS.iEndFai -(iCCEnd - iCCStart);
            itemp = max/10;
            if(itemp == 0)
            {
                itemp = 1;
            }
        }
        if(max > 10)
        {
            ScrollBarAdjust->Max = max;
            if(!bDrawPart)
            {
       	        ScrollBarAdjust->Position = iChoiceStart;
            }
            else
            {
                ScrollBarAdjust->Position = iCCStart;
            }
        }
        else
        {
            ptPB->Refresh();
        }
    }

}
//---------------------------------------------------------------------------
void __fastcall TFormMain::DrawAdjustCurve(TObject* Sender,TRect& rect)
{

        int icol,irow,iall,iindex = 0;
        int xstep,ystep,xnum,ynum,ipart = 0;
        if(iAdjustDrawNum == 0)
        {
            return;
        }                          //lwpig

        else if(iAdjustDrawNum == 1)
        {
            ynum = 1;
            xnum = 1;
        }
        else
        {

            ynum  = iAdjustDrawNum / 2;
            ipart = iAdjustDrawNum % 2;
            ynum  = ynum + ipart;
            xnum  = 2;
        }
        iDataLength = lDS.iPLength;
        xstep       = (rect.Right-rect.Left)/xnum;
        ystep       = (rect.Bottom-rect.Top)/ynum;
        for(irow = 0; irow < ynum; irow ++)
        {
            for(icol = 0; icol < xnum; icol ++)  //�������õ�ͼ�β������浽����������
            {
                iall = 2*irow + icol;
                rectAdjust[iall].Left   = rect.Left + icol*xstep;
                rectAdjust[iall].Top    = rect.Top  + irow*ystep;
                rectAdjust[iall].Bottom = rectAdjust[iall].Top + ystep;
                rectAdjust[iall].Right  = rectAdjust[iall].Left + xstep;
            }
        }

        bCombustion = lDS.GetbCombustion();

        LDrawCurve *lldc;
        lldc = new LDrawCurve;
        if(lldc == NULL)
        {
            Application->MessageBox("��ͼ����������޷�����ִ��!","��ʾ",MB_OK|MB_ICONINFORMATION);
            Close();
        }
//        lldc->SetsbC(bCPeak,bCPeriod,bCCoef);

        iall = 0;
        lldc->SetiDataLength(lDS.iPLength);       //�������ݳ���
        lldc->SetDrawCoef(true,true);
//       ��һ�������������� bLabelSign
//       �ڶ��������������� bNumSign;
        if(emDDevice == ddPaintBox)               //���û�ͼ����
        {
            lldc->SetiDeviceType(1);
        }
        else if(emDDevice == ddImage)
        {
            lldc->SetiDeviceType(2);
        }
        else if(emDDevice == ddPrinter)
        {
            lldc->SetiDeviceType(0);
        }
        lldc->SetfPFactor(fXFactor,fYFactor);     //����XY�����ͼ����
        lldc->SetbShowTitle(true);                //������ʾ����
        lldc->SetbDrawText(false);                //��ͼ���Ƿ���ʾ�ı�
        lldc->SetfOilGain(lDS.fOilGain);
//        lldc->SetbMinZero(true);                  //������СֵΪ0
        lldc->SetbMinZero(false);                  //������СֵΪ0
        lldc->SetbDrawGrid(bPCPGridLine);
        lldc->SetColor(clLineColor,clBackground,clTextColor);
        lldc->SetbShowInfo(true);
        lldc->SetTrainInfo(lDS.LFHS.strTrainNum,lDS.LFHS.siDieselNum );
        lldc->SetbLogValue(false,false);
        lldc->SetiDrawLineWidth(iDrawLineWidth);    //���û�ͼ�߿�
        lldc->SetbDifferentColor( true,               //�ֶ���ʾ
                                  clInletColor,       //��������ɫ
                                  clCompressColor,    //ѹ������ɫ
                                  clCombustionColor,  //ȼ������ɫ
                                  clOutletColor       //��������ɫ
                                  );
       lldc->SetfPWaveData(lDS.fpOilP);
       lldc->SetfPMinMax(lDS.fOilPMin,lDS.fOilPMax);
       lldc->SetfCPData( lDS.fNewP);                  //����ѹ������
       lldc->SetfCPMinMax( lDS.fNormalPMin,lDS.fNormalPMax );
       lldc->SetbSolidYMax(true);      //���ټ���Y������ֵ��

       if(bDrawPWave)
       {
           lldc->SetbDrawPWave(bDrawPWave);
           lldc->SetfPMinMax(lDS.fOilPMin,lDS.fOilPMax);
           lldc->SetfPWaveData( lDS.fpOilP );  //�����͹�ѹ������
       }

//��һ��ͼ�����ڻ���ԭʼ���ݡ�1
        iindex = 0;
        lldc->SetbShowMax(false);
        if(bRectSelected[iindex++])// all = 0,
        {
            lldc->SetXYPrecision(1,1);
            lldc->SetbDrawPWave(bDrawPWave);
            lldc->SetbDrawCWave(false);
            lldc->SetbSolidYMax(false);   //��Y�����ֵ�̶�
            lldc->SetiDataLength(lDS.iPLength );
            lldc->SetDrawRect(rectAdjust[iall]);
            lldc->SetfMMValue(0.0,140.0);
            lldc->SetfXMinMax(lDS.fFFFai[iChoiceStart],lDS.fFFFai[iChoiceEnd - 1]);
            lldc->SetfYMinMax(lDS.fMinOldP,lDS.fMaxOldP);
            lldc->SetfYVDataMax(lDS.fMaxOldP);
//            lldc->SetData(lDS.fFFFai,lDS.fOldP,iChoiceStart,iChoiceEnd,true);
            lldc->SetData(lDS.fFFFai,lDS.fpOilP,iChoiceStart,iChoiceEnd,true);
            lldc->SetbShowMax(false);
            lldc->SetbShowInfo(false);
            lldc->SetXYLabel("����ת��(CA)","��ѹ�͹�ѹ����");
            lldc->SetOilParametes(true, lDS.LFHS.fOilPeak, lDS.LFHS.fWavePeriod, lDS.LFHS.fWaveCoef);
            lldc->SetbFromBegining(false);
            lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
            lldc->LDraw(Sender);
            iall ++;
        }
//�ڶ���ͼ�����ڻ��ƴ������ѹ�����ݡ�2
        if(bRectSelected[iindex++])
        {
            lldc->SetXYPrecision(1,1);
            lldc->SetbDrawPWave(bDrawPWave);
            lldc->SetbDrawCWave(false);
            lldc->SetbSolidYMax(false);   //��Y�����ֵ�̶�
            lldc->SetiDataLength(lDS.iPLength);
            lldc->SetbMinZero(false);
            lldc->SetbLogValue(false,false);
            lldc->SetXYLabel("����ת��(CA)","����ѹ��(MPa)");
            lldc->SetfXMinMax(lDS.fFFFai[iChoiceStart],lDS.fFFFai[iChoiceEnd - 1]);
            lldc->SetfYMinMax(lDS.fNormalPMin,lDS.fNormalPMax);
//            lldc->SetfYVDataMax(lDS.fNormalPMax);
            lldc->SetfYVDataMax(lDS.fCPMax);
            lldc->SetDrawRect(rectAdjust[iall]);
            lldc->SetData(lDS.fFFFai,lDS.fNewP,iChoiceStart,iChoiceEnd,true);
            lldc->SetbFromBegining(false);
            lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
            lldc->LDraw(Sender);
            iall ++;
        }
//������ͼ�������ݻ�ѹ��ͼ�Ρ�3
        if(bRectSelected[iindex++]) //all = 1;
        {
            lldc->SetXYPrecision(1,1);
            lldc->SetbDrawPWave(false);
            lldc->SetbDrawCWave(false);
            lldc->SetbSolidYMax(false);   //��Y�����ֵ�̶�
            lldc->SetXYLabel("�����ݻ�(V)","����ѹ��(MPa)");
            lldc->SetDrawCoef(true,true);
            lldc->SetiDataLength(lDS.iPLength);
            lldc->SetDrawRect(rectAdjust[iall]);
            lldc->SetfXMinMax(lDS.fCVMin,lDS.fCVMax);
            lldc->SetfYMinMax(lDS.fNormalPMin,lDS.fNormalPMax);
            lldc->SetfYVDataMax(lDS.fNormalPMax);
            lldc->SetData(lDS.farV,lDS.fNewP,iChoiceStart,iChoiceEnd,true);
            lldc->SetbXVolumn(true);    //���ú�����Ϊ�ݻ�
            lldc->SetbFromBegining(true);
            lldc->LDraw(Sender);
            iall ++;
        }
// ���ĸ�ͼ�����ڻ����ݻ�ѹ���Ķ���ͼ
        if(bRectSelected[iindex++])
        {
            lldc->SetXYPrecision(1,1);
            lldc->SetbDrawPWave(false);
            lldc->SetbDrawCWave(false);
            lldc->SetbSolidYMax(false);   //��Y�����ֵ�̶�
            lldc->SetDrawCoef(true,true);
            lldc->SetDrawRect(rectAdjust[iall]);
            lldc->SetfXMinMax(lDS.fCVMin,lDS.fCVMax);
            lldc->SetfYMinMax(lDS.fNormalPMin,lDS.fNormalPMax);
            lldc->SetfYVDataMax(lDS.fNormalPMax);
            lldc->SetbLogValue(true,true);
            lldc->SetbXVolumn(true);    //���ú�����Ϊ�ݻ�
            lldc->SetiDataLength(lDS.iPLength);
            lldc->SetXYLabel("�����ݻ�����Log(V)","����ѹ������Log(P)");
            lldc->SetData(lDS.farLogV,lDS.farLogP,iChoiceStart,iChoiceEnd,true);
            lldc->SetbFromBegining(true);
            lldc->LDraw(Sender);
            iall++;
        }
//�����ͼ�����ڻ���ѹ��������
        if(bRectSelected[iindex++])
        {
            lldc->SetXYPrecision(1,2);
            lldc->SetbDrawPWave(bDrawPWave);
            lldc->SetbDrawCWave(bDrawCWave);
            lldc->SetbSolidYMax(false);   //��Y�����ֵ�̶�
            lldc->SetbMinZero(false);
            lldc->SetbLogValue(false,false);
            lldc->SetXYLabel("����ת��(CA)","ѹ��������(MPa/CA)");
            lldc->SetiDataLength(lDS.iPLength - 1);
            lldc->SetfXMinMax(lDS.fFFFai[iChoiceStart],lDS.fFFFai[iChoiceEnd - 1]);
            lldc->SetfYMinMax(lDS.fDPDFMin,lDS.fDPDFMax);
            lldc->SetfYVDataMax(lDS.fDPDFMax);
            lldc->SetDrawRect(rectAdjust[iall]);
            lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
            lldc->SetData(lDS.fFFFai,lDS.farDPDF,iChoiceStart,iChoiceEnd,true);
            lldc->SetbFromBegining(false);
            lldc->LDraw(Sender);
            iall ++;
        }
//������ͼ�����ڻ��Ʒ�����
        if(bRectSelected[iindex++])
        {
            if(lDS.LFHS.chCombustion != 0)
            {
                lldc->SetXYPrecision(1,1);
                lldc->SetbDrawPWave(bDrawPWave);
                lldc->SetbDrawCWave(bDrawCWave);
                lldc->SetbMinZero(false);
                lldc->SetbLogValue(false,false);
                lldc->SetXYLabel("����ת��(CA)","������");
                lldc->SetiDataLength(lDS.iPLength);

                lldc->SetfXMinMax(lDS.fFFFai[iCCStart],lDS.fFFFai[iCCEnd-1]);
                lldc->SetbSolidYMax(true);   //��Y�����ֵ�̶�
                if(lDS.fHRRMax < 50.0)
                {
                    lldc->SetfYMinMax(lDS.fHRRMin,50.0);
                }
                else
                {
                    lldc->SetfYMinMax(lDS.fHRRMin,lDS.fHRRMax);
                }
                lldc->SetfYVDataMax(lDS.fHRRMax);
                lldc->SetDrawRect(rectAdjust[iall]);
                lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                lldc->SetData(lDS.fFFFai,lDS.fDHDF,iCCStart,iCCEnd,true);
                lldc->SetbFromBegining(false);
                lldc->LDraw(Sender);
                iall ++;
            }
            else
            {
//                Application->MessageBox("��ȼ�չ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
            }
        }
//���߸�ͼ�����ڻ����ۻ�������
        if(bRectSelected[iindex++])
        {
            if(lDS.LFHS.chCombustion != 0)
            {
                lldc->SetXYPrecision(1,1);
                lldc->SetbDrawPWave(bDrawPWave);
                lldc->SetbDrawCWave(bDrawCWave);
                lldc->SetbSolidYMax(false);   //��Y�����ֵ�̶�
                lldc->SetbMinZero(false);
                lldc->SetbLogValue(false,false);
                lldc->SetXYLabel("����ת��(CA)","�ۻ�������");
                lldc->SetfXMinMax(lDS.fFFFai[iCCStart],lDS.fFFFai[iCCEnd-1]);
                lldc->SetfYMinMax(lDS.fCHRRMin,lDS.fCHRRMax);
                lldc->SetfYVDataMax(lDS.fCHRRMax);
                lldc->SetDrawRect(rectAdjust[iall]);
                lldc->SetiDataLength(lDS.iPLength);
                lldc->SetData(lDS.fFFFai,lDS.fCDHDF,iCCStart,iCCEnd,true);
                lldc->SetbFromBegining(false);
                lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                if((lDS.fCHRRMax - lDS.fCHRRMin) > 0.5)
                {
                    lldc->LDraw(Sender);
                }
                iall ++;
            }
            else
            {
//                StatusBarMain->
//                Application->MessageBox("��ȼ�չ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
            }

        }
//�ڰ˸�ͼ�����ڻ���ѹ��Ƶ��ͼ
       if(bRectSelected[iindex++])
        {
            lldc->SetXYPrecision(1,1);
            lldc->SetbDrawPWave(false);
            lldc->SetbDrawCWave(false);
            lldc->SetbSolidYMax(false);   //��Y�����ֵ�̶�
            lldc->SetiDataLength(lDS.iFFTLength);
            lldc->SetbMinZero(false);
            lldc->SetbLogValue(false,true);
            lldc->SetXYLabel("Ƶ��(Hz)","������");
            lldc->SetfYMinMax(lDS.fMinFFTP,lDS.fMaxFFTP);
            lldc->SetfYVDataMax(lDS.fMaxFFTP);
            lldc->SetDrawRect(rectAdjust[iall]);
            lldc->SetData(lDS.farFFTPr,lDS.farFFTPi,1,lDS.iFFTLength/2,true);
            lldc->SetbFromBegining(false);
            lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
            lldc->LDraw(Sender);
            iDataLength = 128;
            iall ++;
        }
//�ھŸ�ͼ���ڻ��������¶�ͼ
       if(bRectSelected[iindex++])
        {
            lldc->SetXYPrecision(1,1);        
            lldc->SetbDrawPWave(bDrawPWave);
            lldc->SetbDrawCWave(bDrawCWave);
            lldc->SetbSolidYMax(false);   //��Y�����ֵ�̶�
            lldc->SetbMinZero(false);
            lldc->SetbLogValue(false,false);
            lldc->SetiDataLength(lDS.iPLength);
            lldc->SetXYLabel("����ת��(CA)","�¶�");
            lldc->SetfXMinMax(lDS.fFFFai[iCCStart],lDS.fFFFai[iCCEnd-1]);
            lldc->SetfYMinMax(lDS.fTMin,lDS.fTMax);
            lldc->SetfYVDataMax(lDS.fTMax);
            lldc->SetDrawRect(rectAdjust[iall]);
            lldc->SetData(lDS.fFFFai,lDS.fTT,iCCStart,iCCEnd,true);
            lldc->SetbFromBegining(false);
            lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
            lldc->LDraw(Sender);
            iall ++;
        }
        lldc->GetXYStep( fXStep,fYStep );
        lldc->GetXYInfo(iXMin,iXMax,iYMin,iYMax);
        lldc->GetRect( rectA );
        lldc->GetiYMiddle( iMiddle );
        delete lldc;
        lldc = NULL;
        float fshift;
        AnsiString strTemp;
        fshift = float(lDS.iTDPosi - lDS.iTDPosiBackup);
        strTemp.printf("ƫ�ƣ�%.1fCA",fshift/(float)(lDS.iNumPerAngle));
        LabelCurveShift->Caption = strTemp;


}

void __fastcall TFormMain::PaintBoxAdjustTDCPaint(TObject *Sender)
{
    if((!bFileOpened)&&(!bAdjustHRR)&&(!bLoadOriginal))
    {
       return;
    }
    if(iAdjustDrawNum == 1)
    {
        ScrollBarAdjust->Visible = true;
    }
    else
    {
        ScrollBarAdjust->Visible = false;
    }
    bDrawed = false;
    emDDevice = ddPaintBox;
    iCurrentDraw = 1;
    TRect rect;
    rect.Left   = PaintBoxAdjustTDC->Left;
    rect.Top    = PaintBoxAdjustTDC->Top;
    rect.Bottom = PaintBoxAdjustTDC->Top  + iMainPrnSign*(PaintBoxAdjustTDC->Height);
    rect.Right  = PaintBoxAdjustTDC->Left + PaintBoxAdjustTDC->Width;
    DrawAdjustCurve(Sender,rect);
    bDrawed = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::RGPosiClick(TObject *Sender)
{
    PaintBoxAdjustTDC->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ShiftCurve(int iNum)
{
    //TODO: Add your source code here
    EditShift->Text = iCurveShift;
    lDS.ModifyTDC();
    lDS.CalculateLogVLogP(); //���������ݻ���LogV��LogP
    lDS.TJHeatRelease();     //���������
    lDS.CalculateCHRR();     //�����ۻ�������
    PaintBoxAdjustTDC->Refresh();
}

void __fastcall TFormMain::DrawPSTAll(TObject* Sender,TRect rect)
{
    //TODO: Add your source code here
//  �����˶��ͼ�ζԱȹ���
    int   icol,irow;
//    int   iDieselNum;
    int iDN = 0;
    float fPeriod;
    bool  bOut = false;
    float fmax,fmin;
    int   xnum,ynum,ipart = 0,xstep,ystep;
    bool bPMinMax = false;
    bool bMinMax  = false;
    AnsiString strListName;
    AnsiString strTempName;
    TRect rectt;
    rectt.Right  = PanelCompareBack->Width; //ImagePipe->Width;
    rectt.Bottom = ImagePipe->Height;
    rectt.Left   = 0;
    rectt.Top    = 0;
    if( AllDrawKind == drawPWaveResult )
    {
        rect.Left *= 1.8;
        rect.Right *= 0.93;
/*
        if( bFromPrinter )
        {
            ((TPrinter*)Sender)->Canvas->CopyRect(rect, ImagePipe->Canvas, rectt);
        }
        else
        {
*/
        if(emDDevice == ddImage)
        {
            ((TImage*)Sender)->Canvas->CopyRect(rect, ImagePipe->Canvas, rectt);
        }
        else if(emDDevice == ddPrinter)
        {
            ((TPrinter*)Sender)->Canvas->CopyRect(rect, ImagePipe->Canvas, rectt);
        }
//        }
        return;
    }
    if(!bPSTOpened)
    {
        RefreshStatus(strStatusInfo);
        return;
    }
/*
    if(!InitParameters())
    {
        return;
    }
*/
    if(bFromPrinter)
    {
        if(bDrawStyle)
        {
            ynum   = 8;
            xnum   = 2;
        }
        else
        {
             xnum  = 1;
             ynum  = 16;
        }
    }
    else
    {
/*
        if(bDrawStyle)
        {
            ynum   = iChoicedNum/2;
            ipart  = iChoicedNum%2;
            ynum   = ynum+ipart;
            xnum   = 2;
        }
        else
        {
             xnum  = 1;
             ynum  = iChoicedNum;
        }
*/
        if(iListDrawKind == 1)
        {
            if(bDrawStyle)
            {
                ynum  = iChoicedNum / 2;
                ipart = iChoicedNum % 2;
                ynum  = ynum + ipart;
                xnum  = 2;
            }
            else
            {
                xnum = 1;
                ynum = iChoicedNum;
            }
        }
        else   //����������ε�����
        {
            ynum = iListPageNum;
            ipart = 0;
            ynum = ynum + ipart;
            if(bListTogether)
            {
                xnum = 1;
            }
            else
            {
                xnum = iListDrawKind;
            }
        }
    }

    TRect nowrect;

    if(xnum == 0)
    {
        xnum = 1;
    }
    if(ynum == 0)
    {
         ynum = 1;
    }

    xstep = (rect.Right-rect.Left)/xnum;

    if( iListDrawKind == 1)
    {
        ystep = (rect.Bottom-rect.Top)/ynum;
    }
    else
    {
        if(!bListTogether)
        {
            ystep = (rect.Bottom - rect.Top - 20)/ynum;
        }
        else
        {
            ystep = (rect.Bottom - rect.Top)/ynum;
        }
    }
//    ystep = (rect.Bottom-rect.Top)/ynum;

    LDrawCurve *lldc;
    lldc = new LDrawCurve;
    if(lldc == NULL)
    {
        Close();
    }
    lldc->SetCurveColor(mainCurveColor, 16);  //���û�ͼ��ɫ
    if(bPSTDrawOne)
    {
        lldc->SetDrawCoef(true,true);         //��ʾ����
        lldc->SetbDrawGrid(bPCPGridLine);
//        lldc->SetDrawCoef(false,false);
    }
    else
    {
        lldc->SetDrawCoef(false,false);
        lldc->SetbDrawGrid(bPSTGridLine);
    }
 
    if(emDDevice == ddPaintBox)
    {
        lldc->SetiDeviceType(1);  //PaintBox (bMainPBorPrn);
    }
    else if(emDDevice == ddImage)
    {
        lldc->SetiDeviceType(2);
    }
    else if(emDDevice == ddPrinter)
    {
        lldc->SetiDeviceType(0);
    }
    lldc->SetfPFactor(fXFactor,fYFactor);
    lldc->SetbDrawText(false);
    lldc->SetbMinZero(true);
    lldc->SetColor(clLineColor,clBackground,clTextColor);
//    lldc->SetColor(clBlack,clWhite,clBlack);
    lldc->SetbShowInfo(false);
    lldc->SetiDrawLineWidth(iDrawLineWidth);    //���û�ͼ�߿�
    lldc->SetbDifferentColor( true,               //�ֶ���ʾ
                              clInletColor,       //��������ɫ
                              clCompressColor,    //ѹ������ɫ
                              clCombustionColor,  //ȼ������ɫ
                              clOutletColor       //��������ɫ
                              );
    lldc->SetbShowMax(false);   //�����Ƹ�ѹ�͹ܲ��β���
    ipart = 0;
    maxBuffer.iCyNums = 0;
//---------------------------------------------------------------------------
// 2005_05_26 ��Ҫ�ǻ�ͼʱ��һ��ͳһ�ķŴ�����С����
    for(irow = 0; irow<iChoicedNum; irow++ )
    {
        strListName = ListBoxPST->Items->Strings[irow];
        strTempName = ExtractFileName(strListName);
        strListName = strValidPath + strTempName;
        if(FileExists(strListName))
        {
            bool bSingleLoaded = false;
            bAdjustHRR = false;
            iDN ++;            //��¼�б��ļ��еĸ���
/*
            if(bPSTChanged)   //����б������ѱ仯
            {
                if(pArrayMS[irow] != NULL)
                {
                    delete pArrayMS[irow];
                }
                pArrayMS[irow] = new TMemoryStream();
                pArrayMS[irow]->LoadFromFile(strListName);
            }
            bSingleLoaded = lDS.LoadPCPFromRAM(pArrayMS[irow]);
*/
/*
            else
            {
                bSingleLoaded = lDS.LoadPCPFile(strListName.c_str());
            }
*/
            bSingleLoaded = lDS.LoadPCPFile(strListName.c_str());
            if(bSingleLoaded)
            {
//                if(lDS.LoadPCPFile(strListName.c_str()))   //�������ɹ�
//                {
//                    maxInfo[irow].iDieselNum  = irow + 1;
                    maxInfo[irow].iDieselNum  = lDS.LFHS.siDieselNum;
                    maxInfo[irow].fFaiPMax    = lDS.fPMaxFai;          //�������ѹ������Ӧ������ת��
                    maxInfo[irow].fFaiDPDFMax = lDS.fDPDFMaxFai;       //���ѹ�������ʶ�Ӧ������ת��
                    maxInfo[irow].fFaiDHDFMax = lDS.fDHDFMaxFai;       //�������ʶ�Ӧ������ת��
                    maxInfo[irow].fFaiTMax    = lDS.fTMaxFai;          //����¶ȶ�Ӧ������ת��
                    maxInfo[irow].fFDHDFMax   = lDS.fFirstDHDFMax;     //�����ʵ�һ��ֵ
                    maxInfo[irow].fFDHDFMaxFai= lDS.fFaiFirstDHDFMax;  //�����ʵ�һ��ֵ��Ӧ������ת��
  //20050526
//                    maxInfo[irow].fPMax       = lDS.fNormalPMax;       //�������ѹ��
                    maxInfo[irow].fPMax       = lDS.fCPMax;
                    maxInfo[irow].fWMax       = lDS.fCPower;             //���������Ĺ�
                    maxInfo[irow].fDHDFMax    = lDS.fHRRMax;           //��������
                    maxInfo[irow].fDPDFMax    = lDS.fDPDFMax;          //���ѹ��������
                    maxInfo[irow].fTMax       = lDS.fTMax;             //����¶�
//-----------------------------------------------------------------------------
//����ͳ�����ֵ����Сֵ����Ϣ
                    int inumm = maxBuffer.iCyNums;
                    if(inumm >= 16)  //��������˷�Χ�����˳�
                    {
                        break;
                    }
//------------------------------------------------------------------------------
                    maxBuffer.bCombustion          = lDS.GetbCombustion();  //״̬
                    maxBuffer.barCombustion[inumm] = lDS.GetbCombustion();  //ÿһ�׵�ȼ��״̬
//==============================================================================
//-------------------------------------------------------------------------------
                    maxBuffer.farCombustionPeriod[inumm] = lDS.LFHS.fCombustionPeriod;
                    maxBuffer.farMaxPAngle[inumm]  = lDS.LFHS.fFaiPMax;
                    maxBuffer.farMaxDPAngle[inumm] = lDS.LFHS.fFaiDPDFMax;
                    maxBuffer.farAverageP[inumm]   = lDS.LFHS.fAverageP;   //ƽ��ָʾѹ��
                    maxBuffer.farFlexP[inumm]      = (lDS.LFHS.fP1RMS/(lDS.LFHS.fCPMax))*100.0;            //����ѹ���Ĳ������
                    maxBuffer.farMaxDHAngle[inumm] = lDS.fFaiDHDFMax;
                    maxBuffer.farCombustionLevel[inumm] = lDS.LFHS.fCombustionLevel;
                    maxBuffer.COMBUSTION_LEVEL         = lDS.LFHS.COMBUSTION_LEVEL;
                    maxBuffer.barCombustion[inumm]      = (bool)lDS.LFHS.chCombustion;
//===============================================================================
                    maxBuffer.siarDieselNum[inumm] = lDS.LFHS.siDieselNum;
                    maxBuffer.farEngineSpeed[inumm] = lDS.LFHS.fEngineSpeed;
//                    maxBuffer.farPMax[inumm]     = lDS.fNormalPMax;         //�������ѹ��
                    maxBuffer.farPMax[inumm]     = lDS.fCPMax;         //�������ѹ��
                    maxBuffer.farPMin[inumm]     = lDS.fNormalPMin;         //��С����ѹ��
                    maxBuffer.farDPDFMax[inumm]  = lDS.fDPDFMax;      //���ѹ��������
                    maxBuffer.farDPDFMin[inumm]  = lDS.fDPDFMin;      //��Сѹ��������
                    maxBuffer.farDHDFMax[inumm]  = lDS.fHRRMax;      //��������
                    maxBuffer.farDHDFMin[inumm]  = lDS.fHRRMin;      //��С�����ʡ�

                    maxBuffer.farCDHDFMax[inumm] = lDS.fCHRRMax;      //��������
                    maxBuffer.farCDHDFMin[inumm] = lDS.fCHRRMin;      //��С������

                    maxBuffer.farTMax[inumm]     = lDS.fTMax;         //����¶�
                    maxBuffer.farTMin[inumm]     = lDS.fTMin;         //����¶�
                    maxBuffer.farWMax[inumm]     = lDS.fWMax;         //���������Ĺ�
                    maxBuffer.farWMin[inumm]     = lDS.fWMin;         //������������Сֵ

                    maxBuffer.farDWMax[inumm]    = lDS.fDWMax;
                    maxBuffer.farDWMin[inumm]    = lDS.fDWMin;
                    maxBuffer.farOilPMax[inumm]  = lDS.fOilPMax;       //�͹�ѹ���������ֵ
                    maxBuffer.farOilPMin[inumm]  = lDS.fOilPMin;       //�͹�ѹ��������Сֵ
                    maxBuffer.farOPMax[inumm]    = lDS.fMaxOldP;        //ԭʼ�������ֵ
                    maxBuffer.farOPMin[inumm]    = lDS.fMinOldP;        //ԭʼ������Сֵ
                    maxBuffer.farLogVMax[inumm]  = lDS.fLogVMax;
                    maxBuffer.farLogVMin[inumm]  = lDS.fLogVMin;
                    maxBuffer.farLogPMax[inumm]  = lDS.fLogPMax;
                    maxBuffer.farLogPMin[inumm]  = lDS.fLogPMin;
                    maxBuffer.farFFTMax[inumm]   = lDS.fMaxFFTP;   //20.0*log10(lDS.fMaxFFTP);
                    maxBuffer.farFFTMin[inumm]   = lDS.fMinFFTP;   //20.0*log10(lDS.fMinFFTP);
                    maxBuffer.farCombustionAngle[inumm] = lDS.LFHS.fCombustionAngle;
                    maxBuffer.farOilAngle[inumm]        = lDS.LFHS.fOilAngle;
                    maxBuffer.farWavePeriod[inumm]       = lDS.LFHS.fWavePeriod;
                    maxBuffer.farCPower[inumm]          = lDS.LFHS.fCPower;       //�����
//                maxBuffer.farVMax[iCyNums]    = ;         //�ݻ����ֵ
//                maxBuffer.farVMin[iCyNums]    = ;         //�ݻ���Сֵ
                    maxBuffer.iCyNums ++;
//                }
//-----------------------------------------------------------------------------
            }
        }
    }
//---------------------------------------
// �������ֵ�����ֵ�Լ���Сֵ����Сֵ
    if(maxBuffer.iCyNums == 0)
    {
        Application->MessageBox("û���ҵ���Ч����!\n\n�����б��ļ��ڵ��ļ��Ƿ����?!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    maxBuffer.GetMinMax();
    maxBuffer.SetbCombustion();
//==========================================================================
    GetTrainInfo(strPSTName);
    lldc->SetstrTrainType(strTrainType);
    lldc->SetstrTrainNumber(strTrainNumber);
    lldc->SetstrMDate(strMTime);

    if( iListDrawKind == 1)      // �����ͬһ̨��ͬһʱ�����������
    {
      if(AllDrawKind == drawMAX)
      {

        lldc->SetDrawRect(rect);
        lldc->lDrawMax(Sender,maxInfo,iChoicedNum);
      }
      else if(AllDrawKind == drawInfo)  //���ڻ��Ƹ����׵Ļ�����Ϣ
      {
        lldc->SetBasicFont( strBasicFontName,
                            iBasicFontSize,
                            bBasicFSBold,
                            bBasicFSItalic,
                            bBasicFSUnderline,
                            bBasicFSStrikeOut,
                            clBasicFSColor);

        lldc->SetDrawRect(rect);        //ʵ�����Ǹ�OuterRect����ֵ
        lldc->SetbCombustion(lDS.bOilSupply );
        lldc->SetbDrawBasicCurve( bDrawBasicCurve );
        lldc->lDrawBasicInfo(Sender,maxBuffer,lDS.LFHS);
      }
      else if(AllDrawKind == drawDetect) //�����жϲ����Լ���������
      {
        bDrawDetect = true;
        lldc->SetBasicFont( strBasicFontName,
                            iBasicFontSize,
                            bBasicFSBold,
                            bBasicFSItalic,
                            bBasicFSUnderline,
                            bBasicFSStrikeOut,
                            clBasicFSColor);

        lldc->SetDrawRect(rect);        //ʵ�����Ǹ�OuterRect����ֵ
        lldc->SetbCombustion(lDS.bOilSupply );
        lldc->SetbDrawBasicCurve( bDrawBasicCurve );
        lldc->SetbDrawDetect( bDrawDetect);
        lldc->lDrawDetect(Sender,maxBuffer,lDS.LFHS);
      }
      else
      {
        iaverNumber = 0;
        faverSpeed  = 0.0;
        faverPower  = 0.0;
        faverOil    = 0.0;
        for(irow=0;irow<ynum;irow++)
        {
            for(icol=0;icol<xnum;icol++)
            {
                if(bPSTDrawOne)
                {
                    nowrect.Left   = rect.Left;
                    nowrect.Top    = rect.Top;
                    nowrect.Bottom = rect.Bottom;
                    nowrect.Right  = rect.Right;
                }
                else
                {
                    nowrect.Left   = rect.Left+icol*xstep;
                    nowrect.Top    = rect.Top+irow*ystep;
                    nowrect.Bottom = nowrect.Top+ystep;
                    nowrect.Right  = nowrect.Left+xstep;
                }
                lldc->SetbShowTitle(true);
                lldc->SetDrawRect(nowrect);
                lldc->SetfMMValue(0.0,130);   //???????
                lldc->SetXYPrecision(1 , 2);
                if( (ipart < 0)|| (ipart >= iChoicedNum) )
                {
                    bOut = true;
                    break;
                }
                if( ipart < 16 )
                    lldc->SetstrPipeResult( strPipeDetectResult[ipart]);
                else
                    lldc->SetstrPipeResult("");
                strListName = ListBoxPST->Items->Strings[ipart];
                strTempName = ExtractFileName(strListName);
                strListName = strValidPath + strTempName;
                if(!FileExists(strListName))
                {
                    ipart ++;
                    if(ipart >= iChoicedNum)
                    {
                        ipart --;
                        bOut = true;
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }

                if(bPSTChanged)   //����б������ѱ仯
                {
                    if( ipart < 0)
                    {
                        ipart = 0;
                    }
                    else if( ipart > 31)
                    {
                        ipart = 31;
                    }
                    if(pArrayMS[ipart] != NULL)
                    {
                        delete pArrayMS[ipart];
                    }
                    pArrayMS[ipart] = new TMemoryStream();
                    pArrayMS[ipart]->LoadFromFile(strListName);
                }

//                bSingleLoaded = lDS.LoadPCPFromRAM(pArrayMS[irow]);
                lldc->SetPSTDrawInfo(bPSTDrawOne, ipart );  //���������Ƿ��ǽ�ͼ�λ�����һ��
/*
                if(!lDS.LoadPCPFile(strListName.c_str()))   //�������ɹ�
                {
                    continue;
                }
*/
                if(!lDS.LoadPCPFromRAM(pArrayMS[ipart]))   //�������ɹ�
                {
                    continue;
                }
                bCombustion = lDS.GetbCombustion();
                if(bCombustion)
                {
                    faverSpeed += lDS.getSpeed();
                    faverPower += lDS.getPower();
                    faverOil   += lDS.getOil();
                    iaverNumber ++;
                }

                iChoiceStart = fstartrt * lDS.iPLength;
                iChoiceEnd   = fendrt   * lDS.iPLength;
                lldc->SetfOilGain(lDS.fOilGain);
                lldc->SetfEngineSpeed(lDS.getSpeed());
                lldc->SetiDataLength(lDS.iPLength);
                lldc->SetTrainInfo(lDS.LFHS.strTrainNum, lDS.LFHS.siDieselNum);
                lldc->SetfPMinMax(maxBuffer.fOilPMin,maxBuffer.fOilPMax);
/*
                if(!bPMinMax)
                {
                    lldc->SetfPMinMax(lDS.fOilPMin,lDS.fOilPMax);
                    bPMinMax = true;
                }
*/
                lldc->SetfPWaveData( lDS.fpOilP );  //�����͹�ѹ������
                lldc->SetbDrawCWave( false );       //��ȡ������ѹ���Ļ���
                lldc->SetfCPData( lDS.fNewP);       //����ѹ������
                lldc->SetfCPMinMax( lDS.fNormalPMin,lDS.fNormalPMax );

                if( AllDrawKind == drawOriginal)
                {
                    lldc->SetXYPrecision(1 , 2);
                    lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetbShowInfo(true);
                    lldc->SetfYVDataMax(lDS.fMaxOldP);
                    lldc->SetXYLabel("����ת��(CA)","����ǰ����ѹ��(MPa)");
             //-----------------------------------------------------------
              //���������ʾ��Χ�̶���
                    lldc->SetbSolidYMax(true);
                    lldc->SetfYMinMax(maxBuffer.fOPMin, maxBuffer.fOPMax);
              //===========================================================

                    lldc->SetbFromBegining(false);
                    lldc->SetData(lDS.fFFFai,lDS.fOldP,iChoiceStart,iChoiceEnd,true);
//                    FillPSTSingle(lDS.iDieselNum, lDS.fFFFai,lDS.fOldP,iChoiceStart,iChoiceEnd);
                    strStatusInfo = "ԭʼ���� ʱ��ѹ��ͼ��(�����꣺����ת��,�����꣺����ѹ��)";
                }
                else if((AllDrawKind == drawPT)||(AllDrawKind == drawPPWave))
                {
                    lldc->SetXYLabel("����ת��(CA)","����������ѹ��(MPa)");
                    lldc->SetXYPrecision(1 , 2);
                    lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetbShowInfo(true);
//                    lldc->SetfYVDataMax(lDS.fNormalPMax);
                  lldc->SetfYVDataMax(lDS.fCPMax);
// ��ʾ�����ݲ�����������                  
              //-----------------------------------------------------------
              //���������ʾ��Χ�̶���
                    lldc->SetbSolidYMax(true);
                    lldc->SetfYMinMax(maxBuffer.fPMin, maxBuffer.fPMax);
              //===========================================================

                    lldc->SetbFromBegining(false);
                    lldc->SetData(lDS.fFFFai,lDS.fNewP,iChoiceStart,iChoiceEnd,true);
//                    FillPSTSingle(lDS.iDieselNum, lDS.fFFFai,lDS.fNewP,iChoiceStart,iChoiceEnd);

                    strStatusInfo = "����������� ʱ��ѹ��ͼ��(�����꣺����ת��,�����꣺����ѹ��)";
                }
                else if(AllDrawKind == drawPV)
                {
                    lldc->SetXYLabel("�����ݻ�(L)","����ѹ��(MPa)");
                    lldc->SetXYPrecision(3 , 2);
                    lldc->SetbDrawPWave(false);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbXVolumn(true);    //���ú�����Ϊ�ݻ�
                    lDS.CalculateLogVLogP(); //���������ݻ���LogV��LogP
                    lldc->SetbFromBegining(true);
              //-----------------------------------------------------------
              //���������ʾ��Χ�̶���
                    lldc->SetbSolidYMax(true);
                    lldc->SetfYMinMax(maxBuffer.fPMin, maxBuffer.fPMax);
              //===========================================================

//                    FillPSTSingle(lDS.iDieselNum, lDS.farV,lDS.fNewP,iChoiceStart,iChoiceEnd);
                    lldc->SetData(lDS.farV,lDS.fNewP,iChoiceStart,iChoiceEnd,true);
                    strStatusInfo = "�ݻ�ѹ��ͼ��(�����꣺�����ݻ�,�����꣺����ѹ��)";
                }
                else if(AllDrawKind == drawLogVLogP)
                {
                    lldc->SetXYLabel("�����ݻ�����","����ѹ������");
                    lldc->SetbDrawPWave(false);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbXVolumn(true);    //���ú�����Ϊ�ݻ�
                    lDS.CalculateLogVLogP(); //���������ݻ���LogV��LogP
                    lldc->SetbFromBegining(true);
              //-----------------------------------------------------------
              //���������ʾ��Χ�̶���
                    lldc->SetbSolidYMax(true);
                    lldc->SetXYPrecision(2 , 2);
                    lldc->SetfYMinMax(maxBuffer.fLogPMin, maxBuffer.fLogPMax);
              //===========================================================
//                    FillPSTSingle(lDS.iDieselNum, lDS.farLogV,lDS.farLogP,iChoiceStart,iChoiceEnd);

                    lldc->SetData(lDS.farLogV,lDS.farLogP,iChoiceStart,iChoiceEnd,true);
                    strStatusInfo = "�ݻ�ѹ������ͼ��(�����꣺�����ݻ�����,�����꣺����ѹ������)";
                }
                else if(AllDrawKind == drawDPDF)
                {
                    lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbDrawCWave(bDrawCWave);
                    lldc->SetfMMValue(-10,10);
                    lldc->SetbShowInfo(true);
                    lldc->SetbFromBegining(false);
                    lldc->SetfYVDataMax(lDS.fDPDFMax);
                    lldc->SetXYLabel("����ת��(CA)","ѹ��������(MPa//CA)");
              //-----------------------------------------------------------
              //���������ʾ��Χ�̶���
                    lldc->SetXYPrecision(1 , 3);
                    lldc->SetbSolidYMax(true);
                    lldc->SetfYMinMax(maxBuffer.fDPDFMin, maxBuffer.fDPDFMax);
              //===========================================================
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetData(lDS.fFFFai,lDS.farDPDF,iChoiceStart,iChoiceEnd,true);
                    strStatusInfo = "ѹ��������ͼ��(�����꣺����ת��,�����꣺����ѹ���仯��)";
                }
                else if(AllDrawKind == drawHRR)
                {
                    if(bCombustion)
                    {
                        lldc->SetXYPrecision(1 , 2);
                        lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                        lldc->SetbDrawCWave(bDrawCWave);
                        lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                        lldc->SetfMMValue(-200,1300);
                        lldc->SetbShowInfo(true);
/*
                        lldc->SetbSolidYMax(true);   //��Y�����ֵ�̶�
                        if(lDS.fHRRMax < 500.0)
                        {
                            lldc->SetfYMinMax(lDS.fHRRMin,500.0);
                        }
                        else
                        {
                            lldc->SetfYMinMax(lDS.fHRRMin,lDS.fHRRMax);
                        }
*/
              //-----------------------------------------------------------
              //���������ʾ��Χ�̶���
                        lldc->SetbSolidYMax(true);
                        lldc->SetfYMinMax(maxBuffer.fDHDFMin, maxBuffer.fDHDFMax);
              //===========================================================
                        lldc->SetfYVDataMax(lDS.fHRRMax);
                        lldc->SetbFromBegining(false);
//                        lldc->SetData(lDS.fFFFai,lDS.fDHDF,lDS.iStartFai,lDS.iEndFai,true);
                        lldc->SetData(lDS.fFFFai,lDS.fDHDF, iChoiceStart, iChoiceEnd,true);
                        strStatusInfo = "������ͼ��(�����꣺����ת��,�����꣺������)";
                    }
                    else
                    {
                        strStatusInfo = "ѹ�����̣��޷���ʾ������ͼ";
                        continue;
                    }
                }
                else if(AllDrawKind == drawCHRR)
                {
                    if(bCombustion)
                    {
                        lldc->SetXYPrecision(1 , 2);
                        lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                        lldc->SetbDrawCWave(bDrawCWave);
                        lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                        lldc->SetbShowInfo(true);
                        lldc->SetfYVDataMax(lDS.fCHRRMax);
//                        lldc->SetfMMValue(-200,25000);
                        lldc->SetbFromBegining(false);
                  //-----------------------------------------------------------
                  //���������ʾ��Χ�̶���
                        lldc->SetbSolidYMax(true);
                        lldc->SetfYMinMax(maxBuffer.fCDHDFMin, maxBuffer.fCDHDFMax);
                  //===========================================================
                        lldc->SetData(lDS.fFFFai,lDS.fCDHDF,lDS.iStartFai,lDS.iEndFai,true);
                        strStatusInfo = "�ۻ�������ͼ��(�����꣺����ת��,�����꣺�ۻ�������)";
                    }
                    else
                    {
                        strStatusInfo = "ѹ�����̣��޷���ʾ�ۻ�����ͼ";
                        continue;
                    }
                }
                else if(AllDrawKind == drawFFTP)
                {
                    lldc->SetbDrawPWave(false);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetbShowInfo(true);
                    lldc->SetfYVDataMax(lDS.fMaxFFTP);
                    lldc->SetiDataLength(lDS.iFFTLength);
                    lldc->SetbFromBegining(false);
                    lldc->SetXYPrecision(1 , 2);
                  //-----------------------------------------------------------
                  //���������ʾ��Χ�̶���
                        lldc->SetbSolidYMax(true);
                        lldc->SetfYMinMax(maxBuffer.fFFTMin, maxBuffer.fFFTMax);
                  //===========================================================
                    lldc->SetData(lDS.farFFTPr,lDS.farFFTPi,0,lDS.iFFTLength/2,true);
                    strStatusInfo = "ѹ��Ƶ��ͼ��(�����꣺Ƶ��,�����꣺�������ܶ�)";
                }
                else if(AllDrawKind == drawTT)
                {
                    lldc->SetXYPrecision(1 , 2);
                    lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbDrawCWave(bDrawCWave);
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetfMMValue(0,2500);
                    lldc->SetbShowInfo(true);
                    lldc->SetfYVDataMax(lDS.fTMax);
                    lldc->SetbFromBegining(false);
                  //-----------------------------------------------------------
                  //���������ʾ��Χ�̶���
                    lldc->SetbSolidYMax(true);
                    lldc->SetfYMinMax(maxBuffer.fTMin, maxBuffer.fTMax + 200);
                  //===========================================================
                    lldc->SetData(lDS.fFFFai,lDS.fTT,lDS.iStartFai,lDS.iEndFai,true);
                    strStatusInfo = "�����¶�ͼ��(�����꣺����ת��,�����꣺�����¶�)";
                }
                else if(AllDrawKind == drawDW)
                {
                    lldc->SetXYPrecision(1 , 2);
                    lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbDrawCWave(bDrawCWave);
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetbShowInfo(true);
                    lldc->SetfYVDataMax(lDS.fDWMax);
                    lldc->SetfMMValue(-200,2000);
                  //-----------------------------------------------------------
                  //���������ʾ��Χ�̶���
                        lldc->SetbSolidYMax(true);
                        lldc->SetfYMinMax(maxBuffer.fDWMin, maxBuffer.fDWMax);
                  //===========================================================
                    lldc->SetbFromBegining(false);
                    lldc->SetData(lDS.fFFFai,lDS.farDW,0,lDS.iPLength,true);
                    strStatusInfo = "˲ʱ����(�����꣺����ת��,�����꣺˲ʱ��)";
                }
                else if(AllDrawKind == drawW)
                {
                    lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbDrawCWave(bDrawCWave);
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetbShowInfo(true);
                    lldc->SetfYVDataMax(lDS.fWMax);
                    lldc->SetfMMValue(-200,25000);
                    lldc->SetbFromBegining(false);
                  //-----------------------------------------------------------
                  //���������ʾ��Χ�̶���
                    lldc->SetbSolidYMax(true);
                    lldc->SetfYMinMax(maxBuffer.fWMin, maxBuffer.fWMax);
                  //===========================================================
                    lldc->SetData(lDS.fFFFai,lDS.farW,0,lDS.iPLength,true);
                    strStatusInfo = "�ۻ�����(�����꣺����ת��,�����꣺�ۻ���)";
                }
                else if(AllDrawKind == drawPWave)  //�����͹�ѹ��ͼ��
                {
                    lldc->SetbDrawPWave(false);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbDrawPWave(bDrawPWave);
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetbShowInfo(true);
                    lldc->SetfYVDataMax(lDS.fOilPMax);  //Ӧ��ʾ�͹�ѹ��������ֵ�����ֵ
                    lldc->SetfMMValue(-200,25000);
                    lldc->SetbFromBegining(false);

                    lldc->SetOilParametes(true, lDS.LFHS.fOilPeak, lDS.LFHS.fWavePeriod, lDS.LFHS.fWaveCoef);
                  //-----------------------------------------------------------
                  //���������ʾ��Χ�̶���
                    lldc->SetbSolidYMax(true);
                    lldc->SetfYMinMax(maxBuffer.fOilPMin, maxBuffer.fOilPMax);
                  //===========================================================
//                    lldc->SetData(lDS.fFFFai,lDS.fpOilP,0,lDS.iPLength,true);

                    lldc->SetData(lDS.fFFFai,lDS.fpOilP,lDS.iPLength*2/5,lDS.iPLength*3/5,true);
                    strStatusInfo = "�ۻ�����(�����꣺����ת��,�����꣺�ۻ���)";
                }

                lldc->SetiAllDrawKind( (int)AllDrawKind );
                lldc->LDraw(Sender);
                float ftemp;
                lldc->GetXYStep(fXPSTStep,ftemp);
                ipart ++;
            }
            if(bOut)
            {
                break;
            }
        }
        bPSTChanged = false;
      }

      if(AllDrawKind == drawPWave)
      {
          ImageAll->PopupMenu = PopupMenu10;
      }
      else
      {
          ImageAll->PopupMenu = PopupMenuAllDraw;
      }
    }
    else     //��ʾ��ͼ�Ա�
    {
        lldc->SetbDifferentColor(false);  //���ٻ�����ɫ

        for(icol = 0; icol < iListDrawKind; icol ++)
        {
            strPSTName = strSelectListName[icol];
            ListBoxPST->Items->LoadFromFile(strPSTName);
            ListBoxPST->Refresh();
            iChoicedNum = ListBoxPST->Items->Count;
//            LoadListFile(false);
            ipart = iListPageNum*iListPage;
            lldc->SetColor(clListCurveColor[icol],
                                   clBackground,
                           clListCurveColor[icol]);
            lldc->SetbMerge(bListTogether, icol);

            //���ϲ���ʱ��
            int isposi,islen;
            AnsiString strTTitle;

            strTTitle = ::ExtractFileName(strPSTName);
            isposi = strTTitle.Pos(" ") + 1;
            islen  = strTTitle.Length();
            strTTitle = strTTitle.SubString(isposi, islen - isposi);
            isposi = strTTitle.Pos(" ") + 1;
            islen  = strTTitle.Length();
            strTTitle = strTTitle.SubString(isposi, islen - isposi);
            strTTitle = strTTitle.SubString(1,10);
//            strTTitle = AnsiString("����ʱ�䣺") + strTTitle;
            strTTitle = AnsiString("ʱ��:") + strTTitle;
            if(!bListTogether)  //��������ص�״̬��Ҳ��������ʾʱ��
            {
                TCanvas* ptcanvas;
                if(emDDevice == ddPaintBox)
                {
                    ptcanvas = ((TPaintBox*)(Sender))->Canvas;

                }
                else if(emDDevice == ddImage)
                {
                    ptcanvas = ((TImage*)(Sender))->Canvas;
                }
                else if(emDDevice == ddPrinter)
                {
                    ptcanvas = ((TPrinter*)(Sender))->Canvas;
                }
                SetBkMode(ptcanvas->Handle,TRANSPARENT);
                ptcanvas->TextOut(icol*xstep + 15,6,strTTitle);
            }
            lldc->SetstrTime(strTTitle);

            for(irow = 0; irow < ynum; irow ++)
            {
//                nowrect.Left   = rect.Left+icol*xstep;
//                nowrect.Top    = rect.Top+irow*ystep;
//                nowrect.Bottom = nowrect.Top+ystep;
//                nowrect.Right  = nowrect.Left+xstep;
                if(bPSTDrawOne)
                {
                        nowrect.Left   = rect.Left;
                        nowrect.Top    = rect.Top;
                        nowrect.Bottom = rect.Bottom;
                        nowrect.Right  = rect.Right;
                }
                else
                {
                    if(bListTogether)
                    {
                        nowrect.Left   = rect.Left;
                        nowrect.Top    = rect.Top + irow*ystep;
                        nowrect.Bottom = nowrect.Top  + ystep;
                        nowrect.Right  = nowrect.Left + xstep;
                    }
                    else
                    {
                        nowrect.Left   = rect.Left    + icol*xstep;
                        nowrect.Top    = rect.Top + 20  + irow*ystep;
                        nowrect.Bottom = nowrect.Top  + ystep;
                        nowrect.Right  = nowrect.Left + xstep;
                    }
                }
                lldc->SetbShowTitle(true);
                lldc->SetDrawRect(nowrect);
                lldc->SetfMMValue(0.0,130);
                lldc->SetbSolidYMax(true);   //��Y�����ֵ�̶�
                if( (ipart < 0)|| (ipart >= iChoicedNum) )
                {
                    bOut = true;
                    break;
                }
                strListName = ListBoxPST->Items->Strings[ipart];
                strTempName = ExtractFileName(strListName);
                strListName = strValidPath + strTempName;
                if(!FileExists(strListName))
                {
                    ipart ++;
                    if(ipart >= iChoicedNum)
                    {
                        ipart --;
                        bOut = true;
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }

                if(!lDS.LoadPCPFile(strListName.c_str()))   //�������ɹ�
                {
                    continue;
                }
                bCombustion = lDS.GetbCombustion();
                if(bCombustion)
                {
                    faverSpeed += lDS.getSpeed();
                    faverPower += lDS.getPower();;
                    faverOil   += lDS.getOil();
                    iaverNumber ++;
                }
                iChoiceStart = fstartrt * lDS.iPLength;
                iChoiceEnd   = fendrt   * lDS.iPLength;
                lldc->SetfEngineSpeed(lDS.getSpeed());
                lldc->SetiDataLength(lDS.iPLength);
                lldc->SetTrainInfo(lDS.LFHS.strTrainNum, lDS.LFHS.siDieselNum);
                if(!bPMinMax)
                {
                    lldc->SetfPMinMax(lDS.fOilPMin,lDS.fOilPMax);
                    bPMinMax = true;
                }
                lldc->SetfPWaveData( lDS.fpOilP );  //�����͹�ѹ������
                lldc->SetbDrawCWave( false );       //��ȡ������ѹ���Ļ���
                lldc->SetfCPData( lDS.fNewP);                  //����ѹ������
                lldc->SetfCPMinMax( lDS.fNormalPMin,lDS.fNormalPMax );

                if( AllDrawKind == drawOriginal)
                {
                    if(!bMinMax)
                    {
                        lldc->SetfMMValue(lDS.fMinOldP,lDS.fMaxOldP*fDrawConst);
                        lldc->SetfYMinMax(lDS.fMinOldP,lDS.fMaxOldP*fDrawConst);
                        bMinMax = true;
                    }
                    lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetbShowInfo(true);
                    lldc->SetfYVDataMax(lDS.fMaxOldP);
                    lldc->SetbFromBegining(false);
                    lldc->SetData(lDS.fFFFai,lDS.fOldP,iChoiceStart,iChoiceEnd,true);
                    strStatusInfo = "ԭʼ���� ʱ��ѹ��ͼ��(�����꣺����ת��,�����꣺����ѹ��)";
                }
                else if((AllDrawKind == drawPT)||(AllDrawKind == drawPPWave))
                {
                    if(!bMinMax)
                    {
                        lldc->SetfMMValue(lDS.fNormalPMin,lDS.fNormalPMax*fDrawConst );
                        lldc->SetfYMinMax(lDS.fNormalPMin,lDS.fNormalPMax*fDrawConst );
                        bMinMax = true;
                    }
                    lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetbShowInfo(true);
                    lldc->SetfYVDataMax(lDS.fNormalPMax);
                    lldc->SetbFromBegining(false);
                    lldc->SetData(lDS.fFFFai,lDS.fNewP,iChoiceStart,iChoiceEnd,true);
                    strStatusInfo = "����������� ʱ��ѹ��ͼ��(�����꣺����ת��,�����꣺����ѹ��)";
                }
                else if(AllDrawKind == drawPV)
                {
                    if(!bMinMax)
                    {
                        lldc->SetfMMValue(lDS.fNormalPMin,lDS.fNormalPMax*fDrawConst );
                        lldc->SetfYMinMax(lDS.fNormalPMin,lDS.fNormalPMax*fDrawConst );
                        bMinMax = true;
                    }

                    lldc->SetbDrawPWave(false);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbXVolumn(true);    //���ú�����Ϊ�ݻ�
                    lDS.CalculateLogVLogP(); //���������ݻ���LogV��LogP
                    lldc->SetbFromBegining(true);
                    lldc->SetData(lDS.farV,lDS.fNewP,iChoiceStart,iChoiceEnd,true);
                    strStatusInfo = "�ݻ�ѹ��ͼ��(�����꣺�����ݻ�,�����꣺����ѹ��)";
                }
                else if(AllDrawKind == drawLogVLogP)
                {
                    if(!bMinMax)
                    {
                        lldc->SetfMMValue(20.0*log10(lDS.fNormalPMin),20.0*log10(lDS.fNormalPMax)*fDrawConst );
                        lldc->SetfYMinMax(20.0*log10(lDS.fNormalPMin),20.0*log10(lDS.fNormalPMax)*fDrawConst );
                        bMinMax = true;
                    }
                    lldc->SetbDrawPWave(false);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbXVolumn(true);    //���ú�����Ϊ�ݻ�
                    lDS.CalculateLogVLogP(); //���������ݻ���LogV��LogP
                    lldc->SetbFromBegining(true);
                    lldc->SetData(lDS.farLogV,lDS.farLogP,iChoiceStart,iChoiceEnd,true);
                    strStatusInfo = "�ݻ�ѹ������ͼ��(�����꣺�����ݻ�����,�����꣺����ѹ������)";
                }
                else if(AllDrawKind == drawDPDF)
                {
                    if(!bMinMax)
                    {
                        lldc->SetfMMValue(lDS.fDPDFMin,lDS.fDPDFMax*fDrawConst);
                        lldc->SetfYMinMax(lDS.fDPDFMin,lDS.fDPDFMax*fDrawConst);
                        bMinMax = true;
                    }
                    lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbDrawCWave(bDrawCWave);
                    lldc->SetfMMValue(-10,10);
                    lldc->SetbShowInfo(true);
                    lldc->SetbFromBegining(false);
                    lldc->SetfYVDataMax(lDS.fDPDFMax);
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetData(lDS.fFFFai,lDS.farDPDF,iChoiceStart,iChoiceEnd,true);
                    strStatusInfo = "ѹ��������ͼ��(�����꣺����ת��,�����꣺����ѹ���仯��)";
                }
                else if(AllDrawKind == drawHRR)
                {
                    if(bCombustion)
                    {

                        lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                        lldc->SetbDrawCWave(bDrawCWave);
                        lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                        lldc->SetfMMValue(-200,1300);
                        lldc->SetbShowInfo(true);
                        lldc->SetbSolidYMax(true);   //��Y�����ֵ�̶�
                        if(lDS.fHRRMax < 500.0)
                        {
                            lldc->SetfYMinMax(lDS.fHRRMin,500.0*fDrawConst);
                        }
                        else
                        {
                            lldc->SetfYMinMax(lDS.fHRRMin,lDS.fHRRMax*fDrawConst);
                        }
                        lldc->SetfYVDataMax(lDS.fHRRMax);
                        lldc->SetbFromBegining(false);
                        lldc->SetData(lDS.fFFFai,lDS.fDHDF,lDS.iStartFai,lDS.iEndFai,true);
                        strStatusInfo = "������ͼ��(�����꣺����ת��,�����꣺������)";
                    }
                }
                else if(AllDrawKind == drawCHRR)
                {
                    if(bCombustion)
                    {
                        lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                        lldc->SetbDrawCWave(bDrawCWave);
                        lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                        lldc->SetbShowInfo(true);
                        lldc->SetfYVDataMax(lDS.fCHRRMax);
                        lldc->SetfMMValue(maxBuffer.fCDHDFMin,maxBuffer.fCDHDFMax*fDrawConst);
                        lldc->SetfYMinMax(maxBuffer.fCDHDFMin, maxBuffer.fCDHDFMax*fDrawConst);
                        lldc->SetbFromBegining(false);
                        lldc->SetData(lDS.fFFFai,lDS.fCDHDF,lDS.iStartFai,lDS.iEndFai,true);
                        strStatusInfo = "�ۻ�������ͼ��(�����꣺����ת��,�����꣺�ۻ�������)";
                    }
                }
                else if(AllDrawKind == drawFFTP)
                {
                    lldc->SetbDrawPWave(false);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetbShowInfo(true);
                    lldc->SetfYVDataMax(lDS.fMaxFFTP);
                    lldc->SetfMMValue(maxBuffer.fFFTMin,maxBuffer.fFFTMax*fDrawConst);
                    lldc->SetfYMinMax(maxBuffer.fFFTMin,maxBuffer.fFFTMax*fDrawConst);
                    lldc->SetiDataLength(lDS.iFFTLength);
                    lldc->SetbFromBegining(false);
                    lldc->SetData(lDS.farFFTPr,lDS.farFFTPi,0,lDS.iFFTLength/2,true);
                    strStatusInfo = "ѹ��Ƶ��ͼ��(�����꣺Ƶ��,�����꣺�������ܶ�)";
                }
                else if(AllDrawKind == drawTT)
                {
                    lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbDrawCWave(bDrawCWave);
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetfMMValue(0,2000);
                    lldc->SetbShowInfo(true);
                    lldc->SetfYVDataMax(lDS.fTMax);
                    lldc->SetbFromBegining(false);
                    lldc->SetData(lDS.fFFFai,lDS.fTT,lDS.iStartFai,lDS.iEndFai,true);
                    strStatusInfo = "�����¶�ͼ��(�����꣺����ת��,�����꣺�����¶�)";
                }
                else if(AllDrawKind == drawDW)
                {
                    lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbDrawCWave(bDrawCWave);
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetbShowInfo(true);
                    lldc->SetfYVDataMax(lDS.fDWMax);
                    lldc->SetfMMValue(maxBuffer.fDWMin,maxBuffer.fDWMax*fDrawConst);
                    lldc->SetfYMinMax(maxBuffer.fDWMin,maxBuffer.fDWMax*fDrawConst);
                    lldc->SetbFromBegining(false);
                    lldc->SetData(lDS.fFFFai,lDS.farDW,0,lDS.iPLength,true);
                    strStatusInfo = "˲ʱ����(�����꣺����ת��,�����꣺˲ʱ��)";
                }
                else if(AllDrawKind == drawW)
                {
                    lldc->SetbDrawPWave(bDrawPWave);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbDrawCWave(bDrawCWave);
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetbShowInfo(true);
                    lldc->SetfYVDataMax(lDS.fWMax);
                    lldc->SetfMMValue(maxBuffer.fWMin,maxBuffer.fWMax*fDrawConst);
                    lldc->SetfYMinMax(maxBuffer.fWMin,maxBuffer.fWMax*fDrawConst);
                    lldc->SetbFromBegining(false);
                    lldc->SetData(lDS.fFFFai,lDS.farW,0,lDS.iPLength,true);
                    strStatusInfo = "�ۻ�����(�����꣺����ת��,�����꣺�ۻ���)";
                }
                else if(AllDrawKind == drawPWave)  //�����͹�ѹ��ͼ��
                {
                    lldc->SetbDrawPWave(false);    //�����Ƿ�����͹�ѹ��
                    lldc->SetbDrawPWave(bDrawPWave);
                    lldc->SetbXVolumn(false);    //���ú�����Ϊ�ݻ�
                    lldc->SetbShowInfo(true);
                    lldc->SetfYVDataMax(lDS.fWMax);
//                    lldc->SetfMMValue(-2000.0,15000.0*fDrawConst);
//                    lldc->SetfYMinMax(-2000.0,15000.0*fDrawConst);
                    lldc->SetfMMValue(maxBuffer.fOilPMin,maxBuffer.fOilPMax*fDrawConst);
                    lldc->SetfYMinMax(maxBuffer.fOilPMin,maxBuffer.fOilPMax*fDrawConst);

                    lldc->SetbFromBegining(false);
                    lldc->SetData(lDS.fFFFai,lDS.fpOilP,lDS.iPLength*2/5,lDS.iPLength*3/5,true);
                    strStatusInfo = "�ۻ�����(�����꣺����ת��,�����꣺�ۻ���)";
                }
                lldc->SetiAllDrawKind( (int)AllDrawKind );
                lldc->LDraw(Sender);
                ipart ++;
            }
/*
            if(bOut)
            {
                break;
            }
*/
        }
        ImageAll->PopupMenu = PopupMenuCompare;

    }
    delete lldc;
//    bPSTChanged = false;

/*  //2005_05_23 ��ʱ�رմ���ܣ����ڵ������󲻶�
    if( iListDrawKind == 1)
    {
        if(iaverNumber > 0)
        {
            float ftemp;
            AnsiString strTemp;
            faverSpeed /= (float)iaverNumber;
            ftemp = faverSpeed/120.0;
            strTemp.printf("%.1f", faverSpeed);
            FormEngine->EditDieselSpeed->Text = strTemp;
            strTemp.printf("%.1f", faverPower*ftemp);
            FormEngine->EditPower->Text = strTemp;

            strTemp.printf("%.1f", faverOil*3600.0/faverPower);  //faverOil*ftemp Ϊ��/��

            FormEngine->EditOilConsume->Text  = strTemp;

            FormEngine->EditTrainType->Text   = strTrainType;
            FormEngine->EditTrainNumber->Text = strTrainNumber;
            FormEngine->Show();
        }
    }
*/
}

//2004_01_11 ����
void __fastcall TFormMain::DrawPSTAll3D(TObject* Sender,TRect rect)
{
    //TODO: Add your source code here
    int   icol,irow;
    int   iDieselNum;
    float fPeriod;
//    bool  bOut;
    float fmax,fmin;
    int   xnum,ynum,ipart = 0,xstep,ystep;
    bool bPMinMax;
    AnsiString strListName;
    AnsiString strTempName;

    if(!bPSTOpened)
    {
        RefreshStatus(strStatusInfo);
        return;
    }
    if(!InitParameters())
    {
        return;
    }

    TRect nowrect;

    LDrawCurve *lldc;
    lldc = new LDrawCurve;
    if(lldc == NULL)
    {
        Close();
    }
    lldc->SetCurveColor(mainCurveColor, 16);  //���û�ͼ��ɫ

    lldc->SetDrawCoef(false,false);
    lldc->SetbDrawGrid(bPSTGridLine);
    lldc->SetiDeviceType(2);   //lldc->SetiDeviceType(2);
/*
    if(emDDevice == ddPaintBox)
    {
        lldc->SetiDeviceType(1);  //PaintBox (bMainPBorPrn);
    }
    else if(emDDevice == ddImage)
    {
        lldc->SetiDeviceType(2);
    }
    else if(emDDevice == ddPrinter)
    {
        lldc->SetiDeviceType(0);
    }
*/
    lldc->SetfPFactor(fXFactor,fYFactor);
    lldc->SetbDrawText(false);
    lldc->SetbMinZero(true);
    lldc->SetColor(clLineColor,clBackground,clTextColor);
//    lldc->SetColor(clBlack,clWhite,clBlack);
    lldc->SetbShowInfo(false);
    lldc->SetiDrawLineWidth(iDrawLineWidth);    //���û�ͼ�߿�
    lldc->SetbDifferentColor( true,               //�ֶ���ʾ
                              clInletColor,       //��������ɫ
                              clCompressColor,    //ѹ������ɫ
                              clCombustionColor,  //ȼ������ɫ
                              clOutletColor       //��������ɫ
                              );
    ipart = 0;

    lldc->SetbShowTitle(true);
    lldc->SetDrawRect(rect);

// 2005_05_26 ��Ҫ�ǻ�ͼʱ��һ��ͳһ�ķŴ�����С����
    int iChoiceNum;
    int iDN = 0;
    iChoiceNum = ListBoxPST->Items->Count;
    for(irow = 0; irow<iChoicedNum; irow++ )
    {
        strListName = ListBoxPST->Items->Strings[irow];
        strTempName = ExtractFileName(strListName);
        strListName = strValidPath + strTempName;
        if(FileExists(strListName))
        {
            bAdjustHRR = false;
            iDN ++;                           //��¼�б��ļ��еĸ���
            if(lDS.LoadPCPFile(strListName.c_str()))   //�������ɹ�
            {
                maxInfo[irow].iDieselNum  = irow + 1;
                maxInfo[irow].fFaiPMax    = lDS.fPMaxFai;          //�������ѹ������Ӧ������ת��
                maxInfo[irow].fFaiDPDFMax = lDS.fDPDFMaxFai;       //���ѹ�������ʶ�Ӧ������ת��
                maxInfo[irow].fFaiDHDFMax = lDS.fDHDFMaxFai;       //�������ʶ�Ӧ������ת��
                maxInfo[irow].fFaiTMax    = lDS.fTMaxFai;          //����¶ȶ�Ӧ������ת��
                maxInfo[irow].fFDHDFMax   = lDS.fDHDFMax;     //�����ʵ�һ��ֵ
                maxInfo[irow].fFDHDFMaxFai= lDS.fFaiDHDFMax;  //�����ʵ�һ��ֵ��Ӧ������ת��
  //20050526
                maxInfo[irow].fPMax       = lDS.fNormalPMax;       //�������ѹ��
                maxInfo[irow].fWMax       = lDS.fCPower;             //���������Ĺ�
                maxInfo[irow].fDHDFMax    = lDS.fHRRMax;           //��������
                maxInfo[irow].fDPDFMax    = lDS.fDPDFMax;          //���ѹ��������
                maxInfo[irow].fTMax       = lDS.fTMax;             //����¶�
//-----------------------------------------------------------------------------
//����ͳ�����ֵ����Сֵ����Ϣ
                int inumm = maxBuffer.iCyNums;
                if(inumm >= 16)  //��������˷�Χ�����˳�
                {
                    break;
                }
                maxBuffer.siarDieselNum[inumm] = lDS.LFHS.siDieselNum;
                maxBuffer.farEngineSpeed[inumm] = lDS.LFHS.fEngineSpeed;
                maxBuffer.farPMax[inumm]     = lDS.fNormalPMax;         //�������ѹ��
                maxBuffer.farPMin[inumm]     = lDS.fNormalPMin;         //��С����ѹ��
                maxBuffer.farDPDFMax[inumm]  = lDS.fDPDFMax;      //���ѹ��������
                maxBuffer.farDPDFMin[inumm]  = lDS.fDPDFMin;      //��Сѹ��������
                maxBuffer.farDHDFMax[inumm]  = lDS.fHRRMax;      //��������
                maxBuffer.farDHDFMin[inumm]  = lDS.fHRRMin;      //��С�����ʡ�

                maxBuffer.farCDHDFMax[inumm] = lDS.fCHRRMax;      //��������
                maxBuffer.farCDHDFMin[inumm] = lDS.fCHRRMin;      //��С������

                maxBuffer.farTMax[inumm]     = lDS.fTMax;         //����¶�
                maxBuffer.farTMin[inumm]     = lDS.fTMin;         //����¶�
                maxBuffer.farWMax[inumm]     = lDS.fWMax;         //���������Ĺ�
                maxBuffer.farWMin[inumm]     = lDS.fWMin;         //������������Сֵ

                maxBuffer.farDWMax[inumm]    = lDS.fDWMax;
                maxBuffer.farDWMin[inumm]    = lDS.fDWMin;
                maxBuffer.farOilPMax[inumm]  = lDS.fOilPMax;       //�͹�ѹ���������ֵ
                maxBuffer.farOilPMin[inumm]  = lDS.fOilPMin;       //�͹�ѹ��������Сֵ
                maxBuffer.farOPMax[inumm]    = lDS.fMaxOldP;        //ԭʼ�������ֵ
                maxBuffer.farOPMin[inumm]    = lDS.fMinOldP;        //ԭʼ������Сֵ
                maxBuffer.farLogVMax[inumm]  = lDS.fLogVMax;
                maxBuffer.farLogVMin[inumm]  = lDS.fLogVMin;
                maxBuffer.farLogPMax[inumm]  = lDS.fLogPMax;
                maxBuffer.farLogPMin[inumm]  = lDS.fLogPMin;
                maxBuffer.farFFTMax[inumm]   = lDS.fMaxFFTP;   //20.0*log10(lDS.fMaxFFTP);
                maxBuffer.farFFTMin[inumm]   = lDS.fMinFFTP;   //20.0*log10(lDS.fMinFFTP);
                
                maxBuffer.farCombustionAngle[inumm]  = lDS.LFHS.fCombustionAngle;
                maxBuffer.farOilAngle[inumm]         = lDS.LFHS.fOilAngle;
                maxBuffer.farWavePeriod[inumm]       = lDS.LFHS.fWavePeriod;
                maxBuffer.farCPower[inumm]           = lDS.LFHS.fCPower;       //�����
//                maxBuffer.farVMax[iCyNums]    = ;         //�ݻ����ֵ
//                maxBuffer.farVMin[iCyNums]    = ;         //�ݻ���Сֵ
                maxBuffer.iCyNums ++;
//-----------------------------------------------------------------------------
            }
        }
    }
//---------------------------------------
// �������ֵ�����ֵ�Լ���Сֵ����Сֵ
    maxBuffer.GetMinMax();
//==========================================================================
    if(AllDrawKind == drawMAX)
    {
    }
    else
    {   //�����ݶ���

        lDS.SetDrawKind((int)AllDrawKind);
        if(bFileChanged)
        {
            if(!lDS.Allocate3DMemory())
            {
                return;
            }
            int icount = ListBoxPST->Items->Count;
            for(int i = 0; i < icount; i++)
            {
                strListName = ListBoxPST->Items->Strings[ipart];
                strTempName = ExtractFileName(strListName);
                strListName = strValidPath + strTempName;
                if(!FileExists(strListName))
                {
                    ipart ++;
                    if(ipart >= iChoicedNum)
                    {
                        ipart --;
              //          bOut = true;
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                if(!lDS.LoadPCPFile(strListName.c_str()))   //�������ɹ�
                {
                    continue;
                }
                lDS.Set3DData(ipart,(int)AllDrawKind);
                ipart ++;
            }
            lDS.NormalData();
            iPSTFileNum   = ipart;
            iSelectedItem = iPSTFileNum - 1;

            iX0Position = rectMaxValue.Right/2;
            iY0Position = rect.Height()*7/8;
            iXTimeInterval = rectTime.Width()/5;
            iYTimeInterval = rectTime.Height()*5/6;
            iY3DInterval   = 300;
            
            bFileChanged = false;
            iChoiceStart = lDS.iPLength*fstartrt;
            iChoiceEnd   = lDS.iPLength*fendrt;
            iCutPosi     = iChoiceStart; //lDS.iMaxPosi;
            fMaxMax = 0.0;
            for( int i = 1; i < iPSTFileNum; i++)
            {
                if(fMaxMax < lDS.my3DInfo[i].fMaxValue)
                {
                    fMaxMax = lDS.my3DInfo[i].fMaxValue;
                }
            }
            fMaxMax *= 1.1;
        }
        //ipart Ϊ��Ч���ݵĸ�������಻����16��

//        iChoiceStart = 0;
//        iChoiceEnd   = lDS.iPLength;
        if((iCutPosi < iChoiceStart))
        {
            iCutPosi = iChoiceStart;
        }
        else if(iCutPosi > iChoiceEnd)
        {
            iCutPosi = iChoiceEnd;
        }
        lldc->SetiDataLength(lDS.iPLength);
        lldc->SetbTwoInfo(false);
        lldc->SetbDrawCWave( false );       //��ȡ������ѹ���Ļ���
        lldc->SetfMMValue(0,10);
        lldc->SetDrawRect(rect);
        lldc->SetiDeviceType(2);

        lldc->SetiAngle(fXAngle,fYAngle);
        lldc->SetiXYInterval(iXTimeInterval, iYTimeInterval);
        lldc->SetiY3DInterval(iY3DInterval);
        lldc->SetiX0Y0Posi(iX0Position, iY0Position);
        lldc->SetiCutPosi(iCutPosi);
        lldc->SetbShowCut(bShowCut);
        lldc->SetrectTime(rectTime);
        lldc->SetrectValue(rectValue);
        lldc->SetrectMaxValue(rectMaxValue);
        lldc->SetrectCutValue(rectCutValue);
        lldc->SetrectSingle(rectSingle);
        lldc->SetfStartAngle(lDS.fStartAngle);

        lldc->SetiSelectedItem(iSelectedItem);
//        lldc->SetDrawCoef(false,false);
        lldc->SetbDrawGrid(true);
        lldc->SetfPFactor(1.0,1.0);
        lldc->SetbDrawText(false);
        lldc->SetbMinZero(true);
        lldc->SetiAxisLength(iXALength,iYALength);
        lldc->SetColor(clBlack,clWhite,clBlack);
        lldc->SetbShowInfo(false);
        lldc->SetiXY0Step(iX0Step,iY0Step);
        lldc->SetiChoiceRange(iChoiceStart,iChoiceEnd);
        if( AllDrawKind == drawOriginal)
        {
            lldc->SetbYAxisHalf(false);
            lldc->SetfZDataMinMax(0,fMaxMax);
            lldc->Draw3D(Sender,NULL,NULL,(lDS.f3DData),iPSTFileNum,lDS.iPLength,(l3DInfo*)(lDS.my3DInfo));
            lldc->GetiAxisLength(iXALength,iYALength);
            strStatusInfo = "ԭʼ���� ʱ��ѹ��ͼ��(�����꣺����ת��,�����꣺����ѹ��)";
        }
        else if((AllDrawKind == drawPT)||(AllDrawKind == drawPPWave))
        {
            lldc->SetbYAxisHalf(false);
            lldc->SetfZDataMinMax(0,fMaxMax);
            lldc->Draw3D(Sender,NULL,NULL,(lDS.f3DData),iPSTFileNum,lDS.iPLength,(l3DInfo*)(lDS.my3DInfo));
            lldc->GetiAxisLength(iXALength,iYALength);
            strStatusInfo = "����������� ʱ��ѹ��ͼ��(�����꣺����ת��,�����꣺����ѹ��)";
        }
        else if(AllDrawKind == drawDPDF)
        {
            lldc->SetbYAxisHalf(true);
            lldc->SetfZDataMinMax(0,fMaxMax);
            lldc->Draw3D(Sender,NULL,NULL,(lDS.f3DData),iPSTFileNum,lDS.iPLength, (l3DInfo*)(lDS.my3DInfo));
            lldc->GetiAxisLength(iXALength,iYALength);
            strStatusInfo = "ѹ��������ͼ��(�����꣺����ת��,�����꣺����ѹ���仯��)";
        }
        else if(AllDrawKind == drawHRR)
        {
            lldc->SetbYAxisHalf(false);
            lldc->SetfZDataMinMax(maxBuffer.fDHDFMin,maxBuffer.fDHDFMax);
            lldc->Draw3D(Sender,NULL,NULL,(lDS.f3DData),iPSTFileNum,lDS.iPLength, (l3DInfo*)(lDS.my3DInfo));
            lldc->GetiAxisLength(iXALength,iYALength);
            strStatusInfo = "������ͼ��(�����꣺����ת��,�����꣺������)";
        }
        else if(AllDrawKind == drawCHRR)
        {
            lldc->SetbYAxisHalf(false);
            lldc->SetfZDataMinMax(maxBuffer.fCDHDFMin,maxBuffer.fCDHDFMax);
            lldc->Draw3D(Sender,NULL,NULL,(lDS.f3DData),iPSTFileNum,lDS.iPLength, (l3DInfo*)(lDS.my3DInfo));
            lldc->GetiAxisLength(iXALength,iYALength);
            strStatusInfo = "�ۻ�������ͼ��(�����꣺����ת��,�����꣺�ۻ�������)";
        }
        else if(AllDrawKind == drawPWave)  //�����͹�ѹ��ͼ��
        {
            lldc->SetbYAxisHalf(false);
            lldc->SetfZDataMinMax(0,fMaxMax);
            lldc->Draw3D(Sender,NULL,NULL,(lDS.f3DData),iPSTFileNum,lDS.iPLength, (l3DInfo*)(lDS.my3DInfo));
            lldc->GetiAxisLength(iXALength,iYALength);
            strStatusInfo = "�͹�ѹ����(�����꣺����ת��,�����꣺��ѹ�͹�ѹ������)";
        }
        else if(AllDrawKind == drawTT)  //�����͹�ѹ��ͼ��
        {
            lldc->SetbYAxisHalf(false);
            lldc->SetfZDataMinMax(0,maxBuffer.fTMax);
            lldc->Draw3D(Sender,NULL,NULL,(lDS.f3DData),iPSTFileNum,lDS.iPLength, (l3DInfo*)(lDS.my3DInfo));
            lldc->GetiAxisLength(iXALength,iYALength);
            strStatusInfo = "�͹�ѹ����(�����꣺����ת��,�����꣺��ѹ�͹�ѹ������)";
        }
        else if(AllDrawKind == drawDW)
        {
            lldc->SetbYAxisHalf(true);
            lldc->SetfZDataMinMax(maxBuffer.fDWMin,maxBuffer.fDWMax);
            lldc->Draw3D(Sender,NULL,NULL,(lDS.f3DData),iPSTFileNum,lDS.iPLength, (l3DInfo*)(lDS.my3DInfo));
            lldc->GetiAxisLength(iXALength,iYALength);
        }
        else if(AllDrawKind == drawW)
        {
            lldc->SetbYAxisHalf(true);
            lldc->SetfZDataMinMax(maxBuffer.fWMin,maxBuffer.fWMax);
            lldc->Draw3D(Sender,NULL,NULL,(lDS.f3DData),iPSTFileNum,lDS.iPLength, (l3DInfo*)(lDS.my3DInfo));
            lldc->GetiAxisLength(iXALength,iYALength);
        }
        lldc->SetiAllDrawKind( (int)AllDrawKind );
    }

    delete lldc;
}


void __fastcall TFormMain::ListView1Click(TObject *Sender)
{
    AnsiString FileStr,strTemp;
    if(ListView1->Selected == NULL)
    {
        return;
    }
    FileStr = CurrentDir + "\\" + ListView1->Selected->Caption;
    strTemp = ExtractFileExt(FileStr);
    strTemp = strTemp.UpperCase();
    if(strTemp == ".PST")
    {
        ListBoxPST->Items->LoadFromFile(FileStr);
        iChoicedNum = ListBoxPST->Items->Count;
    }
    else
    {
        ListBoxPST->Items->Clear();
    }

}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ToolButtonPVClick(TObject *Sender)
{
    if(AllDrawed(Sender,drawPV))
    {
        return;
    }
    TCursor Save_Cursor = Screen->Cursor ;
    try
    {
        Screen->Cursor = crHourGlass;
        if(!DetectPSTStatus())
        {
            return;
        }
//        PaintBoxAll->Refresh();
        DrawImageAll();
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButtonModifiedDataClick(TObject *Sender)
{
//    Application->ProcessMessages();
    if(AllDrawed(Sender,drawPT))
    {
        return;
    }
    TCursor Save_Cursor = Screen->Cursor ;    // Show hourglass cursor
    try
    {
        Screen->Cursor = crHourGlass;
        if(!DetectPSTStatus())
        {
            return;
        }
        bFileChanged = true;
//        PaintBoxAll->Refresh();
        DrawImageAll();
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButtonDPDFClick(TObject *Sender)
{
//    Application->ProcessMessages();
    if(AllDrawed(Sender,drawDPDF))
    {
        return;
    }
    TCursor Save_Cursor = Screen->Cursor ;
    try
    {

        Screen->Cursor = crHourGlass;

        if(!DetectPSTStatus())
        {
            return;
        }
        bFileChanged = true;
//        PaintBoxAll->Refresh();
        DrawImageAll();
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }


}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButtonHRRClick(TObject *Sender)
{
    if(AllDrawed(Sender,drawHRR))
    {
        return;
    }
    TCursor Save_Cursor = Screen->Cursor ;
    try
    {

        Screen->Cursor = crHourGlass;
        if(!DetectPSTStatus())
        {
            return;
        }
        bFileChanged = true;
//        PaintBoxAll->Refresh();
        DrawImageAll();
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
}
//---------------------------------------------------------------------------










/*?????????????????????????????????????????

*/


void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    DeleteMemory();
//    RegSaveInfo();       //������ֵ���浽ע�����
    DatabaseClose();
    DeleteRamFile();
    if(bUSBOpened)
    {
        CloseUsb();
        bUSBOpened = false;
    }
    
    FormDelete();
/*
    AddCode(strMSecret);
    AddCode(strNSecret);
    AddCode(strLibSecret);
*/
    bAddMInfo = CheckBoxAddMInfo->Checked;
    RegSaveInfo();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::DeleteMemory(void)
{
    //TODO: Add your source code here
    for(int i=0;i<iPASSNUM;i++)
    {
        if(fPassData[i] != 0)
        {
            delete[] fPassData[i];
            fPassData[i] = NULL;
        }
    }
    delete ListStringList;
    delete ListPath;
    delete ListList;
    if(strData != NULL)
    {
        delete []strData;
    }
    if(strTempData!=NULL)
    {
        delete [] strTempData;
        strTempData=NULL;
    }
}

void __fastcall TFormMain::FFTAnalysis(void)
{
    //TODO: Add your source code here
/*
    if(!bFileOpened)
    {
      	 return;
    }
    lDataProcess* ldp;
    ldp=new lDataProcess;
    if(ldp == NULL)
    {
        Application->MessageBox("���ݴ����ڴ�������!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
//ע�⣺�˴��ļ����������Ҫ�޸ĵ�
    float *ftemp = NULL;
    ftemp = new float[iDataLength];
    if(ftemp == NULL)
    {
        Application->MessageBox("��ʱ�����ڴ�������!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    for(int i=0;i<iDataLength;i++)
    {
        ftemp[i] = fMP[i];
    }
    ldp->LBeginFFT(ftemp,fFFTPr,fFFTPi,256,false);  //FFT�任

    delete ldp;
    bPLog = true;
    fMinFFTP = fFFTPi[0];
    fMaxFFTP = fFFTPi[0];
    for(int j=0;j<128;j++)
    {
        fFFTPr[j] = j;
        if(fMinFFTP > fFFTPi[j])
        {
            fMinFFTP = fFFTPi[j];
        }
        else if(fMaxFFTP < fFFTPi[j])
        {
            fMaxFFTP = fFFTPi[j];
        }
    }
    if(bPLog)
    {
        for(int j=0;j<128;j++)
        {
            fFFTPi[j] = 20.0*log10(fFFTPi[j]);
        }
    }
*/
}
void __fastcall TFormMain::ppmAdjustPTClick(TObject *Sender)
{
    bBackMousePoint = false;
    RadioGroupDrawMode->ItemIndex = -1;
    PPMAdjustSingleMode(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmAdjustPVClick(TObject *Sender)
{
    bBackMousePoint = false;
    RadioGroupDrawMode->ItemIndex = -1;    
    PPMAdjustSingleMode(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmAdjustFFTPClick(TObject *Sender)
{
    bBackMousePoint = false;
    RadioGroupDrawMode->ItemIndex = -1;    
    PPMAdjustSingleMode(7);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmAdjustLogPVClick(TObject *Sender)
{
    bBackMousePoint = false;
    RadioGroupDrawMode->ItemIndex = -1;    
    PPMAdjustSingleMode(3);
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::RefreshStatus(AnsiString str)
{
    //TODO: Add your source code here
    StatusBarMain->Panels->Items[0]->Text = str;
    StatusBarMain->Refresh();
}

void __fastcall TFormMain::ToolButtonDomainClick(TObject *Sender)
{
    if(AllDrawed(Sender,drawFFTP))
    {
        return;
    }

    TCursor Save_Cursor = Screen->Cursor ;
    try
    {

        Screen->Cursor = crHourGlass;
        if(!DetectPSTStatus())
        {
            return;
        }
        bFileChanged = true;
        AllDrawKind = drawFFTP;
//        PaintBoxAll->Refresh();
        DrawImageAll();
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }


}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmAdjustCPTClick(TObject *Sender)
{
    bBackMousePoint = false;
    RadioGroupDrawMode->ItemIndex = -1;    
    PPMAdjustSingleMode(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmAdjustDPDFClick(TObject *Sender)
{
    bBackMousePoint = false;
    RadioGroupDrawMode->ItemIndex = -1;    
    PPMAdjustSingleMode(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmAdjustHRRClick(TObject *Sender)
{
    bBackMousePoint = false;
    RadioGroupDrawMode->ItemIndex = -1;
    PPMAdjustSingleMode(5);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmAdjustCHRRClick(TObject *Sender)
{
    bBackMousePoint = false;
    RadioGroupDrawMode->ItemIndex = -1;    
    PPMAdjustSingleMode(6);

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmAdjustTemperatureClick(TObject *Sender)
{
    bBackMousePoint = false;
    RadioGroupDrawMode->ItemIndex = -1;    
    PPMAdjustSingleMode(8);
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::RefreshDrawMode(void)
{
    //TODO: Add your source code here
    iAdjustDrawNum = 0;
    int iindex = 0;
    if( !bCurveModi )
    {
        iChoiceStart = 0;
        iChoiceEnd = lDS.iPLength;
    }
    if(CheckBoxAdjustPT->Checked)
    {
        iAdjustDrawNum ++;
        bRectSelected[iindex] = true;
    }
    else
    {
        bRectSelected[iindex] = false;
    }
    iindex ++;

    if(CheckBoxAdjustCPT->Checked)
    {
        iAdjustDrawNum ++;
        bRectSelected[iindex] = true;
    }
    else
    {
        bRectSelected[iindex] = false;
    }
    iindex ++;

    if(CheckBoxAdjustPV->Checked)
    {
        iAdjustDrawNum ++;
        bRectSelected[iindex] = true;
    }
    else
    {
        bRectSelected[iindex] = false;
    }
    iindex ++;

    if(CheckBoxAdjustLogVLogP->Checked)
    {
        iAdjustDrawNum ++;
        bRectSelected[iindex] = true;
    }
    else
    {
        bRectSelected[iindex] = false;
    }
    iindex++;

    if(CheckBoxAdjustDPDF->Checked)
    {
        iAdjustDrawNum ++;
        bRectSelected[iindex] = true;
    }
    else
    {
        bRectSelected[iindex] = false;
    }
    iindex ++;

    if(CheckBoxAdjustHRR->Checked)
    {
        iAdjustDrawNum ++;
        bRectSelected[iindex] = true;
    }
    else
    {
        bRectSelected[iindex] = false;
    }
    iindex ++;
    if(CheckBoxAdjustCHRR->Checked)
    {
        iAdjustDrawNum ++;
        bRectSelected[iindex] = true;
    }
    else
    {
        bRectSelected[iindex] = false;
    }
    iindex ++;
    if(CheckBoxAdjustFFTP->Checked)
    {
        iAdjustDrawNum ++;
        bRectSelected[iindex] = true;
    }
    else
    {
        bRectSelected[iindex] = false;
    }
    iindex ++;
    bRectSelected[8] = false;
    bRectSelected[9] = false;
    PaintBoxAdjustTDC->Refresh();
}

void __fastcall TFormMain::PPMAdjustSingleMode(int icindex)
{
    //TODO: Add your source code here
    RadioGroupSingleCurve->ItemIndex = icindex;
    bDrawModePressed = true;
    iChoiceStart = 0;
    iChoiceEnd = lDS.iPLength;
    CheckBoxAdjustPT->Checked = false;
    CheckBoxAdjustCPT->Checked = false;
    CheckBoxAdjustPV->Checked = false;
    CheckBoxAdjustDPDF->Checked = false;
    CheckBoxAdjustFFTP->Checked = false;
    CheckBoxAdjustLogVLogP->Checked = false;
    CheckBoxAdjustHRR->Checked = false;
    CheckBoxAdjustCHRR->Checked = false;
    for(int i=0;i<10;i++)
    {
        bRectSelected[i] = false;
    }
    bRectSelected[icindex] = true;
    switch(icindex)
    {
        case 0:
            CheckBoxAdjustPT->Checked = true;
            PCPDrawKind = drawPT;
            break;
        case 1:
             CheckBoxAdjustCPT->Checked = true;
             PCPDrawKind = drawCPT;
            break;
        case 2:
            CheckBoxAdjustPV->Checked = true;
            PCPDrawKind = drawPV;
            break;
        case 3:
            CheckBoxAdjustLogVLogP->Checked = true;
            PCPDrawKind = drawLogVLogP;
            break;
        case 4:
            CheckBoxAdjustDPDF->Checked = true;
            PCPDrawKind = drawDPDF;
            break;
        case 5:
            CheckBoxAdjustHRR->Checked = true;
            PCPDrawKind = drawHRR;
            break;
        case 6:
            CheckBoxAdjustCHRR->Checked = true;
            PCPDrawKind = drawCHRR;
            break;
        case 7:
            CheckBoxAdjustFFTP->Checked = true;
            PCPDrawKind = drawFFTP;
            break;
/*
    ,          //ԭʼ����
    ,         //�����������
    ,          //�ݻ�ѹ��ͼ
    drawFFTP,        //ѹ��Ƶ��ͼ
    ,    //�ݻ�ѹ������ͼ
    ,        //ѹ��������
    drawHRR,         //����������
    drawCHRR,        //�ۻ�������
    drawTT,          //�������¶�
    drawOriginal,    //ԭʼ����
    drawW,           //�ۻ���
    drawDW,          //˲ʱ��
    drawMAX,         //���׸�������ֵ
    drawPWave,       //���Ƹ�ѹ�͹�ѹ��
    drawPPWave,      //��������ѹ������ѹ�͹�ѹ��
        case 0:
            CheckBoxAdjustPT->Checked = true;
            break;
        case 1:
             CheckBoxAdjustCPT->Checked = true;
            break;
        case 2:
            CheckBoxAdjustPV->Checked = true;
            break;
        case 3:
            CheckBoxAdjustDPDF->Checked = true;
            break;
        case 4:
            CheckBoxAdjustFFTP->Checked = true;
            break;
        case 5:
            CheckBoxAdjustLogVLogP->Checked = true;
            break;
        case 6:
            CheckBoxAdjustHRR->Checked = true;
            break;
        case 7:
            CheckBoxAdjustCHRR->Checked = true;
            break;
        case 8:
            break;
*/
    }


    iAdjustDrawNum = 1;
    PaintBoxAdjustTDC->Refresh();
//    if(icindex == 8) //�����¶ȣ�
//    {
//        bRectSelected[8] = false;
//    }
    bDrawModePressed = false;
}

void __fastcall TFormMain::RadioGroupDrawModeClick(TObject *Sender)
{
    bDrawModePressed = true;
    if(RadioGroupDrawMode->ItemIndex == 1)
    {
        CheckBoxAdjustPT->Checked = true;
        CheckBoxAdjustCPT->Checked = true;
        CheckBoxAdjustPV->Checked = true;
        CheckBoxAdjustDPDF->Checked = true;
        CheckBoxAdjustFFTP->Checked = true;
        CheckBoxAdjustLogVLogP->Checked = true;
        CheckBoxAdjustHRR->Checked = true;
        CheckBoxAdjustCHRR->Checked = true;
        for(int i=0;i<10;i++)
        {
            bRectSelected[i] = true;
            iAdjustDrawNum = 8;
        }
    }
    else
    {
        CheckBoxAdjustPT->Checked = false;
        CheckBoxAdjustCPT->Checked = true;
        CheckBoxAdjustPV->Checked = false;
        CheckBoxAdjustDPDF->Checked = true;
        CheckBoxAdjustFFTP->Checked = false;
        CheckBoxAdjustLogVLogP->Checked = false;
        CheckBoxAdjustHRR->Checked = true;
        CheckBoxAdjustCHRR->Checked = true;
/*        for(int i=0;i<10;i++)
        {
            bRectSelected[i] = false;
            iAdjustDrawNum = 8;
        }
        bRectSelected[1] = true;
*/
    }
    RefreshDrawMode();
    bDrawModePressed = false;
    GroupBox9->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CheckBoxAdjustPTClick(TObject *Sender)
{
    if(!bDrawModePressed)
    {
        RefreshDrawMode();
    }
}
//---------------------------------------------------------------------------

bool __fastcall TFormMain::AllDrawed(TObject * Sender, drawKind drawkind)
{
    //TODO: Add your source code here
    TToolButton *ptTT = (TToolButton*)Sender;
    ptTT->Down = true;
    ptTT->Refresh();
    if( AllDrawKind == drawkind)
    {
        return true;
    }
    else
    {
        AllDrawKind = drawkind;   //���ڻ���ʾ��ͼ
    }
    return false;
}

void __fastcall TFormMain::ToolButtonOriginalDataClick(TObject *Sender)
{
// AllDrawed()  �����ж��Ƿ��Ѿ������˻�ͼ�����ѻ�ͼ���˳������ٻ���
    if(AllDrawed(Sender,drawDetect))
    {
        return;
    }
    TCursor Save_Cursor = Screen->Cursor ;
    try
    {

        Screen->Cursor = crHourGlass;
// DetectPSTStatus()�����ж��Ƿ���PST�б��ļ����򿪣���û������ʾ���б��ļ�
        if(!DetectPSTStatus())
        {
            return;
        }
        if(iListDrawKind != 1)
        {
            Application->MessageBox("�˹���ֻ�����ڵ�̨�����ݷ���!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
            return;
        }
        bFileChanged = true;
//        PaintBoxAll->Refresh();
        DrawImageAll();
        if(bExcel)
        {
            SaveToExcels();
        }
/*
    PageControlCurve->ActivePageIndex = 5;

    if(!DetectPSTStatus())
    {
        return;
    }
    if(iListDrawKind != 1)
    {
        Application->MessageBox("�˹���ֻ�����ڵ�̨�����ݷ���!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    bFileChanged = true;
    DataAnalysis();
    SaveToExcels();
*/
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
/*
// �����ǵ�����ʾ������ʾԭʼ���ݼ��������
// ���˺�����Ϊ���ݷ������̣�ԭʼ����û��ʲô��
    if(iListDrawKind != 1)
    {
        return;
    }

    if(!DetectPSTStatus())
    {
        return;
    }
    AnsiString strTemp;
    FormAnalysisResult = new TFormAnalysisResult(FormMain);
    FormAnalysisResult->PanelTrainType->Caption   = lDS.strTrainType;
    FormAnalysisResult->PanelTrainNumber->Caption = lDS.strTrainNumber;
    FormAnalysisResult->PanelMTime->Caption   = strMTime;

    strTemp.printf("%.1f r//min", maxBuffer.fESMean);
    FormAnalysisResult->PanelSpeed->Caption       = strTemp;
    strTemp.printf("%.1f kW",faverPower* (faverSpeed/iaverNumber/120.0));
    FormAnalysisResult->PanelPower->Caption       = strTemp;
//-----------------------------------------------------------
// ��������������õ��Ĳ���
//  �������ı���
    FormAnalysisResult->StringGridParams->Cells[0][0] = "�׺�";
    FormAnalysisResult->StringGridParams->Cells[1][0] = "����ѹ";
    FormAnalysisResult->StringGridParams->Cells[2][0] = "��ѹ��λ";
    FormAnalysisResult->StringGridParams->Cells[3][0] = "ѹ��ѹ��";
    FormAnalysisResult->StringGridParams->Cells[4][0] = "ѹ����";
    FormAnalysisResult->StringGridParams->Cells[5][0] = "ѹ������λ";
    FormAnalysisResult->StringGridParams->Cells[6][0] = "ƽ��ѹ��";
    FormAnalysisResult->StringGridParams->Cells[7][0] = "��ѹ����";
    FormAnalysisResult->StringGridParams->Cells[8][0] = "������λ";
    FormAnalysisResult->StringGridParams->Cells[9][0] = "ȼ����λ";
    FormAnalysisResult->StringGridParams->Cells[10][0] = "ȼ�սǶ�";
    FormAnalysisResult->StringGridParams->Cells[11][0] = "���������ֵ";
    FormAnalysisResult->StringGridParams->Cells[12][0] = "�ۻ�����";
    FormAnalysisResult->StringGridParams->Cells[13][0] = "��������";
//ȷ���������Ŀ���
    FormAnalysisResult->StringGridParams->ColWidths[0] = 28;
    FormAnalysisResult->StringGridParams->ColWidths[4] = 40;
    FormAnalysisResult->StringGridParams->ColWidths[5] = 64;
    FormAnalysisResult->StringGridParams->ColWidths[11] = 76;
//�������������������
    for( int i = 0 ; i < maxBuffer.iCyNums; i++)
    {
        //0 �׺�
        FormAnalysisResult->StringGridParams->Cells[0][i+1] = maxBuffer.siarDieselNum[i];
        if(maxBuffer.bCombustion)
        {
            //1 ����ѹ
            strTemp.printf("%.2f",maxBuffer.farPMax[i]);
            FormAnalysisResult->StringGridParams->Cells[1][i+1] = strTemp;
            FormAnalysisResult->StringGridParams->Cells[3][i+1] = "";
        }
        else
        {
            //3��ѹ��ѹ��
            strTemp.printf("%.2f", maxBuffer.farPMax[i]);
            FormAnalysisResult->StringGridParams->Cells[3][i+1] = strTemp;
            FormAnalysisResult->StringGridParams->Cells[1][i+1] = "";
        }
        //2��ѹ��λ
        strTemp.printf("%.1f", maxBuffer.farMaxPAngle[i]);
        FormAnalysisResult->StringGridParams->Cells[2][i+1] = strTemp;
        //4ѹ����
        strTemp.printf("%.3f", maxBuffer.farDPDFMax[i]);
        FormAnalysisResult->StringGridParams->Cells[4][i+1] = strTemp;
        //5ѹ������λ
        strTemp.printf("%.1f", maxBuffer.farMaxDPAngle[i]);
        FormAnalysisResult->StringGridParams->Cells[5][i+1] = strTemp;
        //6ƽ��ѹ��
        strTemp.printf("%.2f", maxBuffer.farAverageP[i]);
        FormAnalysisResult->StringGridParams->Cells[6][i+1] = strTemp;
        //7��ѹ����
        strTemp.printf("%.2f%%", maxBuffer.farFlexP[i]);
        FormAnalysisResult->StringGridParams->Cells[7][i+1] = strTemp;
        //8������λ
        strTemp.printf("%.1f", maxBuffer.farOilAngle[i]);
        FormAnalysisResult->StringGridParams->Cells[8][i+1] = strTemp;
        //9ȼ����λ
        strTemp.printf("%.1f", maxBuffer.farCombustionAngle[i]);
        FormAnalysisResult->StringGridParams->Cells[9][i+1] = strTemp;
        //10ȼ�սǶȣ�����ʱ�䣩
        strTemp.printf("%.1f", maxBuffer.farCombustionPeriod[i]);
        FormAnalysisResult->StringGridParams->Cells[10][i+1] = strTemp;
        //11���������ֵ
        strTemp.printf("%.2f", maxBuffer.farDHDFMax[i]/1000.0);
        FormAnalysisResult->StringGridParams->Cells[11][i+1] = strTemp;
        //12�ۻ�������
        strTemp.printf("%.1f", maxBuffer.farCDHDFMax[i]);
        FormAnalysisResult->StringGridParams->Cells[12][i+1] = strTemp;
        //13��������
        strTemp.printf("%.1f", maxBuffer.farCPower[i]);
        FormAnalysisResult->StringGridParams->Cells[13][i+1] = strTemp;
        FormAnalysisResult->StringGridParams->RowCount ++;
    }
    if(maxBuffer.iCyNums > 0)
    {
        FormAnalysisResult->StringGridParams->RowCount--;
    }
//===========================================================
    FormAnalysisResult->ShowModal();
    delete FormAnalysisResult;
//    Application->ProcessMessages();
*/
/*
// ��һ�γ�������ʾ������ԭʼ���ݳ���
    if(AllDrawed(Sender,drawOriginal))
    {
        return;
    }
    TCursor Save_Cursor = Screen->Cursor ;
    try
    {

        Screen->Cursor = crHourGlass;
        if(!DetectPSTStatus())
        {
            return;
        }
        bFileChanged = true;
//        PaintBoxAll->Refresh();
        DrawImageAll();
//
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButtonCHRRClick(TObject *Sender)
{
    if(AllDrawed(Sender,drawCHRR))
    {
        return;
    }
    TCursor Save_Cursor = Screen->Cursor ;
    try
    {
        Screen->Cursor = crHourGlass;
        if(!DetectPSTStatus())
        {
            return;
        }
        bFileChanged = true;
        DrawImageAll();
//        PaintBoxAll->Refresh();
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }

}
//---------------------------------------------------------------------------




void __fastcall TFormMain::LoadCurrentPCP(void)
{
}


void __fastcall TFormMain::LoadCurrentPST(void)
{
    //TODO: Add your source code here
    if( strPSTName == "")
    {
        return;
    }

    ToolButton36->Down = true;
    ToolButton36->Refresh();
    AnsiString strTemp;
    strTemp = ExtractFileExt(strPSTName);
    strTemp = strTemp.UpperCase();
    if(strTemp != ".PST")
    {
        return;
    }
//--------------------------------------------------------------
// ���´����������û�������
    strTemp      = ExtractFileName(strPSTName);
    strTrainType = ExtractFileName(strTemp);
    iTrainType   = GetiTrainType(strTrainType);
    SetDieselInfo(iTrainType, false);
//=================================================================
    FormMain->Caption = strSoftTitle
                        + AnsiString(" - ")
                        + strPSTName;

    PageControlMain->ActivePageIndex  = 4;
    PageControlCurve->ActivePageIndex = 2;

    bLoadList = true;
    bPSTOpened = true;
    ToolButtonModifiedData->Down = true;
    ToolButtonModifiedData->Refresh();
    DrawImageAll();
//    PaintBoxAll->Refresh();
}
void __fastcall TFormMain::BitBtnOneAllClick(TObject *Sender)
{
    if(iKeyPressPosi == 1)
    {
        if(bOneOfAll) //��ǰ��ͼ��ȷ�Ǵ�PST�ļ���ѡ���
        {
//            LoadCurrentPST();
            PageControlCurve->ActivePageIndex = 2;
            iKeyPressPosi = 0;
        }
    }
    else if(iKeyPressPosi == 2)
    {
        if(bOneOfAll)
        {
            iKeyPressPosi = 1;
        }
        else
        {
            iKeyPressPosi = 0;
        }
    }
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::DetectPSTStatus(void)
{
    //TODO: Add your source code here
    if(!bPSTOpened)  //�����û�д�ѹ���ļ����˳�
    {
        ToolButtonPSTClick(ToolButtonPST);
        Application->MessageBox("�������������ҳ˫����Ӧ���б��ļ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return false;
    }
    return true;
}

void __fastcall TFormMain::PaintBoxOriginalPaint(TObject *Sender)
{
    int iDieselNum;
    int ystep;
    float fPeriod,ftemp;
    if(!bLoadOriginal)
    {
        return;
    }
    TRect rect,rect01,rect02,rect03;
    ystep       = PaintBoxOriginal->Height/2;
    rect.Left   = PaintBoxOriginal->Left;
    rect.Top    = PaintBoxOriginal->Top;
    rect.Bottom = PaintBoxOriginal->Top + (PaintBoxOriginal->Height);
    rect.Right  = PaintBoxOriginal->Left + PaintBoxOriginal->Width;
    if(bOriginalDrawAll)
    {
        rect01 = rect;
        rect02 = rect;
        rect03 = rect;
    }
    else
    {
        rect01.Left   = rect.Left;
        rect02.Left   = rect.Left;
        rect03.Left   = rect.Left;
        rect01.Right  = rect.Right;
        rect02.Right  = rect.Right;
        rect03.Right  = rect.Right;
        rect01.Top    = rect.Top;
        rect02.Top    = rect.Top + ystep;
        rect01.Bottom = rect02.Top;
        rect03.Top    = rect02.Top + ystep;
        rect02.Bottom = rect03.Top;
        rect03.Bottom = rect03.Top + ystep;
    }

    LDrawCurve *lldc;
    lldc = new LDrawCurve;
    if(lldc == NULL)
    {
       Close();
    }
//        lldc->SetsbC(bCPeak,bCPeriod,bCCoef);
    lldc->SetOriginalMode(-1500,32768,true);
    lldc->SetDrawCoef(false,false);
    lldc->SetiDeviceType(1);  //PaintBox (bMainPBorPrn);
    lldc->SetbDrawText(false);
    lldc->SetbMinZero(true);
    lldc->SetColor(clBlack,clWhite,clBlack);
    lldc->SetDrawRect(rect01);
    TColor cl = clBlack;
    lldc->iDrawOriginal(Sender,lDS.ipOData[0],iOriginalStart,iOriginalEnd,cl);
    lldc->SetDrawRect(rect02);

    cl = clPurple;
    lldc->iDrawOriginal(Sender,lDS.ipOData[1],iOriginalStart,iOriginalEnd,cl);
    lldc->SetDrawRect(rect03);

    if(!CheckBoxHideThree->Checked)
    {
/*
        for(int i = 0; i<iOriginalEnd - 1; i++)
        {
        // ����ѹ��������
            lDS.ipOData[2][i] = (lDS.ipOData[0][i+1] - lDS.ipOData[0][i]);
        }
*/
/*
    lDS.fPP2[0] = 0.0;
    lDS.fPP2[1] = 0.0;
    lDS.fPP2[iOriginalEnd - 2] = 0.0;
    lDS.fPP2[iOriginalEnd - 1] = 0.0;
    for( int i = 2; i<iOriginalEnd - 2; i++)
    {
        lDS.fPP2[i] = (lDS.fOBuffer[i-2] -8.0*lDS.fOBuffer[i-1] + 8.0*lDS.fOBuffer[i+1] - lDS.fOBuffer[i+2])/(12.0*lDS.fAngleStep);
    }
*/
/*
    if(bFilter)
    {
        lDataProcess *ldp = NULL;
        ldp = new lDataProcess();
        if( ldp == NULL )
        {
            return;
        }
        ldp->lFilterData(lDS.fPP2,lDS.fOBuffer,iOriginalEnd,fFilter,iFilterLen);
        for(int i = 0; i< lDS.iOriginalLength; i++)
        {
            lDS.fPP2[i] = lDS.fOBuffer[i];
        }
        delete ldp;
    }
*/
        TColor cll = clBlue;
        lldc->iDrawOriginal(Sender,lDS.ipOData[2],iOriginalStart,iOriginalEnd-1,cll);
    }
    lldc->GetXYStep(fXOriginalStep,ftemp);
    delete lldc;
    lldc = NULL;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::PaintBoxOriginalMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

    if(Button == mbLeft)
    {
        TPoint pointMouse;
        pointMouse.x = X;
        pointMouse.y = Y;
        TRect rect;
        rect.Left    = PaintBoxOriginal->Left;
        rect.Top     = PaintBoxOriginal->Top;
        rect.Right   = rect.Left + PaintBoxOriginal->Width;
        rect.Bottom  = rect.Top  + PaintBoxOriginal->Height;
       if(PtInRect(&rect,pointMouse))
       {
            pointMouseOrigin.x  = X;
            pointMouseOrigin.y  = Y;
            pointMouseOld.x     = X;
            pointMouseOld.y     = Y;
            bPBMouseDragging    = true;
       }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxOriginalMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    TPaintBox* ptPB = (TPaintBox*)Sender;
    int posX,posY;
    TPoint pointMouse;
    pointMouse.x    = X;
    pointMouse.y    = Y;
    if(bPBMouseDragging)
    {
         TRect rect;
         rect.Left   = PaintBoxOriginal->Left;
         rect.Top    = PaintBoxOriginal->Top;
         rect.Right  = rect.Left + PaintBoxOriginal->Width;
         rect.Bottom = rect.Top + PaintBoxOriginal->Height;
         if(PtInRect(&rect,pointMouse))
         {
             ptPB->Canvas->Pen->Color  = clGridLineColor;
             ptPB->Canvas->Pen->Width  = 1;
//             ptPB->Canvas->Pen->Style  = (TPenStyle)LineType[iGridLineTypeIndex];
             ptPB->Canvas->Pen->Mode   = pmNot;
             PBDrawRect(Sender,pointMouseOrigin,pointMouseOld);
             PBDrawRect(Sender,pointMouseOrigin,pointMouse);
             pointMouseOld.x           = X;
             pointMouseOld.y           = Y;
         }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxOriginalMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

    TPaintBox* ptPB = (TPaintBox*)Sender;
    if((Button == mbLeft)&&(bPBMouseDragging))
    {
        int iStart,iEnd;
        TPoint pointMouse;
        pointMouse.x  = X;
        pointMouse.y  = Y;
        bPBMouseDragging=false;
        ptPB->Canvas->Pen->Color = clGridLineColor;
        ptPB->Canvas->Pen->Width = 1;
 //       ptPB->Canvas->Pen->Style = (TPenStyle)LineType[iGridLineTypeIndex];
         TRect rect;
         rect.Left   = PaintBoxOriginal->Left;
         rect.Top    = PaintBoxOriginal->Top;
         rect.Right  = rect.Left + PaintBoxOriginal->Width;
         rect.Bottom = rect.Top  + PaintBoxOriginal->Height;
        if(PtInRect(&rect,pointMouse))
        {
            ptPB->Canvas->Pen->Mode = pmNot;
            PBDrawRect(Sender,pointMouseOrigin,pointMouseOld);
            ptPB->Canvas->Pen->Mode = pmCopy;
            PBDrawRect(Sender,pointMouseOrigin,pointMouse);
            if(fXOriginalStep < 0.00001)
            {
                return;
            }
            iStart = iOriginalStart + (int)((float)(pointMouseOrigin.x-rect.left)/fXOriginalStep+0.5);
            iEnd   = iOriginalStart + (int)((float)(X-rect.left)/fXOriginalStep+0.5);
            if((iEnd-iStart)<0)
            {
                int ittemp;
                ittemp = iStart;
                iStart = iEnd;
                iEnd   = ittemp;

            }
            int bNoDraw = false;
            if((iEnd-iStart)>64)
            {
                    iOriginalStart  = iStart;
                    iOriginalEnd    = iEnd;
                    bBackMousePoint = false;
            }
            else
            {
                if((iOriginalStart == 0)&&( iOriginalEnd == lDS.iOriginalLength))
                {
                    bNoDraw = true;
                }
                else
                {
                    iOriginalStart = 0;
                    iOriginalEnd   = iOriginalEnd_Backup;
                }
            }
            bPBMouseDragging = false;
            int max,itemp;
            max   = iOriginalEnd_Backup - (iOriginalEnd-iOriginalStart);
            itemp = max/10;
            if(itemp == 0)
            {
                itemp = 1;
            }
        	if(max > 10)
            {
    	        ScrollBarOriginal->Max      = max;
            	ScrollBarOriginal->Position = iOriginalStart;
            }
            else
            {
                if(!bNoDraw)
                {
                    ptPB->Refresh();
                }
            }
        }
        Application->ProcessMessages();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ScrollBarOriginalChange(TObject *Sender)
{
  	int iLen,ipage;
    iLen  = iOriginalEnd - iOriginalStart;
    if(ScrollBarOriginal->Max == 0)
    {
        return;
    }
    if(iLen == iOriginalEnd_Backup)
    {
        return;
    }
    iOriginalStart = ScrollBarOriginal->Position;
    if(iOriginalStart == 0)
    {
      	iOriginalStart = 1;
    }
    iOriginalEnd    = iOriginalStart + iLen;
    if(iOriginalEnd > iOriginalEnd_Backup)
    {
    	iOriginalEnd = iOriginalEnd_Backup;
    }
 //   bDrawed=false;
    PaintBoxOriginal->Repaint();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ppmOriginalDrawModeClick(TObject *Sender)
{
    ppmOriginalDrawMode->Checked = !ppmOriginalDrawMode->Checked ;
    bOriginalDrawAll = ppmOriginalDrawMode->Checked;
    PaintBoxOriginal->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxAbstractPaint(TObject *Sender)
{
//    return;
    int iDieselNum;
    int ystep;
    AnsiString strTemp;
    float fmax1,fmax2;
    float fPeriod,ftemp;
    if(!bLoadOriginal)
    {
        return;
    }
    TRect rect,rect01,rect02,rect03;
    ystep       = PaintBoxAbstract->Height/2;
    rect.Left   = PaintBoxAbstract->Left;
    rect.Top    = PaintBoxAbstract->Top;
    rect.Bottom = PaintBoxAbstract->Top+(PaintBoxAbstract->Height);
    rect.Right  = PaintBoxAbstract->Left+PaintBoxAbstract->Width;

    if(bAbstractDrawAll)
    {
        rect01 = rect;
        rect02 = rect;
        rect03 = rect;
    }
    else
    {
        rect01.Left   = rect.Left;
        rect02.Left   = rect.Left;
        rect03.Left   = rect.Left;
        rect01.Right  = rect.Right;
        rect02.Right  = rect.Right;
        rect03.Right  = rect.Right;
        rect01.Top    = rect.Top;
        rect02.Top    = rect.Top + ystep;
        rect01.Bottom = rect02.Top;
        rect03.Top    = rect02.Top + ystep;
        rect02.Bottom = rect03.Top;
        rect03.Bottom = rect03.Top + ystep;
    }

    LDrawCurve *lldc;
    lldc = new LDrawCurve;
    if(lldc == NULL)
    {
       Close();
    }
    lldc->SetDrawCoef(false,false);
    lldc->SetiDeviceType(1);
    lldc->SetbDrawText(false);
    lldc->SetbMinZero(true);
    lldc->SetColor(clBlack,clWhite,clBlack);
    lldc->SetOriginalMode(-1500,32768,true);
    if( lDS.iSampleNum > 0)
    {
        lldc->SetTopDead(true,lDS.iTDPosi);
        lldc->SetiCombustionPosi(lDS.iCombustionPosi);
        lldc->SetiOilPosi(lDS.iOilPosi);
        lldc->SetDrawRect(rect01);
//        lldc->DrawOriginal(Sender,lDS.fpFData,iAbstractStart,iAbstractEnd,clBlack);
        TColor cll = clBlack;
        lldc->iDrawOriginal(Sender,lDS.ipACData[0],iAbstractStart,iAbstractEnd,cll);
        lldc->SetDrawRect(rect02);
//        lldc->DrawOriginal(Sender,lDS.fpSMDPDF,iAbstractStart,iAbstractEnd,clPurple);
        cll = clPurple;
        lldc->iDrawOriginal(Sender,lDS.ipACData[1],iAbstractStart,iAbstractEnd,cll);
        lldc->SetDrawRect(rect03);
        lldc->SetOriginalMode(-1.0,1.0,false);

        cll = clBlue;
//        lldc->DrawOriginal(Sender,lDS.fpDPDF,iAbstractStart,iAbstractEnd,cll);
        lldc->GetXYStep(fXAbstractStep,ftemp);

    }
    delete lldc;
    lldc = NULL;
    RefreshStatus(strTemp);
    AnsiString strTTT;
    strTTT.printf("%d",lDS.iShiftNumber);

    RefreshPChart();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxAbstractMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(bAbstractDrawFF)
    {
        return;
    }
    TPaintBox* ptPB = (TPaintBox*)Sender;
    if(Button == mbLeft)
    {
        TPoint pointMouse;
        pointMouse.x = X;
        pointMouse.y = Y;
        TRect rect;
        rect.Left = ptPB->Left;
        rect.Top = ptPB->Top;
        rect.Right = rect.Left + ptPB->Width;
        rect.Bottom = rect.Top + ptPB->Height;
       if(PtInRect(&rect,pointMouse))
       {
            pointMouseOrigin.x = X;
            pointMouseOrigin.y = Y;
            pointMouseOld.x = X;
            pointMouseOld.y = Y;
            bPBMouseDragging = true;
       }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxAbstractMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    if(bAbstractDrawFF)
    {
        return;
    }
    TPaintBox* ptPB = (TPaintBox*)Sender;
    int posX,posY;
    TPoint pointMouse;
    pointMouse.x = X;
    pointMouse.y = Y;
    if(bPBMouseDragging)
    {
         TRect rect;
         rect.Left   = ptPB->Left;
         rect.Top    = ptPB->Top;
         rect.Right  = rect.Left + ptPB->Width;
         rect.Bottom = rect.Top + ptPB->Height;
         if(PtInRect(&rect,pointMouse))
         {
             ptPB->Canvas->Pen->Color = clGridLineColor;
             ptPB->Canvas->Pen->Width = 1;
//             ptPB->Canvas->Pen->Style = (TPenStyle)LineType[iGridLineTypeIndex];
             ptPB->Canvas->Pen->Mode  = pmNot;
             PBDrawRect(Sender,pointMouseOrigin,pointMouseOld);
             PBDrawRect(Sender,pointMouseOrigin,pointMouse);
             pointMouseOld.x          = X;
             pointMouseOld.y          = Y;
         }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxAbstractMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(bAbstractDrawFF)
    {
        return;
    }

    TPaintBox* ptPB = (TPaintBox*)Sender;
    if((Button == mbLeft)&&(bPBMouseDragging))
    {
        int iStart,iEnd;
        TPoint pointMouse;
        pointMouse.x = X;
        pointMouse.y = Y;
        bPBMouseDragging = false;
        ptPB->Canvas->Pen->Color = clGridLineColor;
        ptPB->Canvas->Pen->Width = 1;
//        ptPB->Canvas->Pen->Style = (TPenStyle)LineType[iGridLineTypeIndex];
        TRect rect;
        rect.Left = ptPB->Left;
        rect.Top  = ptPB->Top;
        rect.Right = rect.Left + ptPB->Width;
        rect.Bottom = rect.Top + ptPB->Height;
        if(PtInRect(&rect,pointMouse))
        {
            ptPB->Canvas->Pen->Mode = pmNot;
            PBDrawRect(Sender,pointMouseOrigin,pointMouseOld);
            ptPB->Canvas->Pen->Mode = pmCopy;
            PBDrawRect(Sender,pointMouseOrigin,pointMouse);
            if(fXAbstractStep<0.00001) return;
            iStart = iAbstractStart + (int)((float)(pointMouseOrigin.x-rect.left)/fXAbstractStep+0.5);
            iEnd   = iAbstractStart + (int)((float)(X-rect.left)/fXAbstractStep+0.5);
            if((iEnd-iStart)<0)
            {
                int ittemp;
                ittemp = iStart;
                iStart = iEnd;
                iEnd   = ittemp;

            }
            int bNoDraw = false;
            if((iEnd-iStart) > 64)
            {
                    iAbstractStart  = iStart;
                    iAbstractEnd    = iEnd;
                    bBackMousePoint = false;
            }
            else
            {
                if((iAbstractStart == 0)&&( iAbstractEnd == lDS.iAbstractLength))
                {
                    bNoDraw = true;
                }
                else
                {
                    iAbstractStart = 0;
                    iAbstractEnd   = lDS.iAbstractLength;
                }
            }
            bPBMouseDragging = false;
            int max,itemp;
            max   = lDS.iAbstractLength -(iAbstractEnd-iAbstractStart);
            itemp = max/10;
            if(itemp == 0) itemp = 1;
        	if(max > 10)
            {
    	        ScrollBarAbstract->Max      = max;
            	ScrollBarAbstract->Position = iAbstractStart;
            }
            else
            {

                if(!bNoDraw)
                {
                    ptPB->Refresh();
                }
            }
        }
        Application->ProcessMessages();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ScrollBarAbstractChange(TObject *Sender)
{
  	int iLen,ipage;
    iLen = iAbstractEnd - iAbstractStart;
    if(ScrollBarAbstract->Max == 0) return;
    if(iLen == lDS.iAbstractLength) return;
    iAbstractStart = ScrollBarAbstract->Position;
    if(iAbstractStart == 0)
    {
      	iAbstractStart=1;
    }
    iAbstractEnd = iAbstractStart + iLen;
    if(iAbstractEnd > lDS.iAbstractLength)
    {
    	iAbstractEnd = lDS.iAbstractLength;
    }
 //   bDrawed=false;
    PaintBoxAbstract->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmAbstractMergeClick(TObject *Sender)
{
    ppmAbstractMerge->Checked = !ppmAbstractMerge->Checked ;
    bAbstractDrawAll = ppmAbstractMerge->Checked;
    PaintBoxAbstract->Refresh();
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::LoadCurrentOriginal(void)
{
    //TODO: Add your source code here
    AnsiString strTemp;
    bLoadOriginal = lDS.ReadOriginal(strOriginalName.c_str(),true);
    
    if(bLoadOriginal) //�����ȡ���ݳɹ������ͼ
    {   //�������ݺ�û�д���
        bLoadOriginal = true;
        ///////////////////////FX�������ӣ���ʾ�ڶ�ͨ��ԭʼ������ʾ���вɼ����
        bDataShowAll = true;   //ԭʼ������ʾ����

        if( bDataShowAll)
        {
            iOriginalStart = 0;
            iOriginalEnd = lDS.iOriginalLength;
        }
        else
        {
            iOriginalStart = 0;
            iOriginalEnd   = lDS.iAbstractLength;
        }
        iOriginalEnd_Backup = iOriginalEnd;
        iChoiceStart = 0;
        iChoiceEnd = lDS.iPLength;
        bFiltered = false;
        PaintBoxOriginal->Refresh();
        iAbstractStart = 0;
        iAbstractEnd = lDS.iAbstractLength;
         ////////////////////////FX: �����ӱ�ʾ��ʾ�ڶ�ͨ���ɼ�����
        if ( iAbstractEnd  > 0 )        //�ڶ�ͨ���򿪣��˴���ʾ��ʾ�ڶ�ͨ������
            {
              if(lDS.iSampleNum == 0)
                  lDS.iSampleNum = 1 ;   //lDS.iSampleNum>0 ��ʾ�ɼ������� ����Ҫ��ʾ�ڶ�ͨ��
               }
        Application->ProcessMessages();
        strTemp.printf("%.5f",lDS.fDetect);
        LabelDetect->Caption = strTemp;
        PaintBoxAbstract->Refresh();

    }
    else
    {
        bLoadOriginal  = true;
        iOriginalStart = 0;
        iOriginalEnd   = lDS.iOriginalLength;
        iChoiceStart   = 0;
        iChoiceEnd     = lDS.iPLength;
        bFiltered      = false;
        PaintBoxOriginal->Refresh();
        Application->MessageBox("�޷��ҵ���Ч����!!!","��ʾ",MB_OK);
    }
    return true;
}


void __fastcall TFormMain::GetMinMax(float * fdata, int ilen, float& fmin, float & fmax)
{
    //TODO: Add your source code here
    fmin = fdata[0];
    fmax = fdata[0];
    for(int i = 1; i < ilen; i ++)
    {
        if(fdata[i]<fmin)
        {
            fmin = fdata[i];
        }
        else if(fdata[i]>fmax)
        {
            fmax = fdata[i];
        }
    }
}


void __fastcall TFormMain::FileListBoxFileDblClick(TObject *Sender)
{
    AnsiString strTemp,strNameTemp;
    int iindex,icount;
    icount = FileListBoxFile->Items->Count;
    if(icount <= 0)
    {
        return;
    }
    strPSTFileDirectory = FileListBoxFile->Directory;
    iindex = FileListBoxFile->ItemIndex;
    strTemp = FileListBoxFile->Directory + "\\";
    strNameTemp = strTemp + FileListBoxFile->Items->Strings[iindex];
    TCursor Save_Cursor = Screen->Cursor ;    // Show hourglass cursor
    try
    {
/*
        if(!bParameterInited)
        {
            InitParameters();
            bParameterInited = true;
        }
*/
  // Do some lengthy operation

        Screen->Cursor = crHourGlass;
        strTemp = ExtractFileExt(strNameTemp);
        strTemp = strTemp.UpperCase();
        if(strTemp == ".ORG")
        {
            strOriginalName = strNameTemp;
            LoadCurrentBinORG();
        }
        else if(strTemp == ".PCP")
        {
            bCurveModi = false;
            strPCPName = strNameTemp;
            LoadCurrentBinPCP(false);
        }
        else if(strTemp == ".PST")
        {
            bCurveModi   = false;
            bFileChanged = true;
            bPCPEntered  = false;
            strPSTName = strNameTemp;
            bool bHave = false;
            int iCount;
            iListDrawKind = 1;
            strValidPath = ExtractFilePath(strPSTName);
            iCount = ListBoxPSTFile->Items->Count;
            for(int i = 0; i< iCount; i++)
            {
                strTemp = ListBoxPSTFile->Items->Strings[i];
                if(strTemp == strPSTName)
                {
                    ListBoxPSTFile->ItemIndex = i;
                    bHave = true;
                    break;
                }
            }
            if(!bHave)
            {
                ListBoxPSTFile->Items->Add(strPSTName);
                ListBoxPSTFile->ItemIndex = ListBoxPSTFile->Items->Count - 1 ;
            }

            ListBoxPCPFile->Items->LoadFromFile(strPSTName);
            bLoadList = true;
            LoadCurrentBinPST();
            iChoiceStart = 0;
            iChoiceEnd   = lDS.iPLength;
            for( int i = 0; i < 16; i ++)
            {
                strPipeDetectResult[i] = "";
            }
        }
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
    strTemp.printf("���ݴ�����������%d����ɾ������:%d��",lDS.iSampleNum,lDS.iDeleteNum);
    StatusBarMain->Panels->Items[0]->Text = strTemp;

}

//---------------------------------------------------------------------------


void __fastcall TFormMain::BitBtnSavePositionClick(TObject *Sender)
{
/*
    AnsiString Dir;
    int ilen;
    BrowseForFolder("�����ļ�����·��","����",Dir);
    ilen = Dir.Length();

    if((Dir != "")&&(ilen > 3))
    {
        strFileDirectory = Dir + "\\";
        EditPathName->Text = strFileDirectory;
    }
*/
    AnsiString strTemp;
    int iret;
    strTemp = strPresetPath;
    FormPath = new TFormPath(FormMain);
    FormPath->EditFilePath->Text = strPresetPath;
    iret = FormPath->ShowModal();
    if(iret == mrOk)
    {
        strPresetPath=FormPath->EditFilePath->Text;
    }
    delete FormPath;
    if(iret == mrOk)
    {
        char ch='\\',chtemp;
        int ilen=strPresetPath.Length();
        int iNum=0,iposi=0;
        for(int i=0;i<ilen-1;i++)
        {
            chtemp=strPresetPath.c_str()[i];
            if(ch==chtemp)
            {
                iNum++;
            }
        }
        if(iNum>1)
        {
            strPresetPath=strTemp;
            Application->MessageBox("�벻Ҫ���õڶ���Ŀ¼!","��ʾ",MB_OK|MB_ICONINFORMATION);
        }

        if( ExtractFileDrive(strPresetPath)=="")
        {
            strPresetPath=strTemp;
            Application->MessageBox("����ȷָ��������!","��ʾ",MB_OK|MB_ICONINFORMATION);
        }
        ilen = strPresetPath.Length();
        ch = strPresetPath.c_str()[ilen-1];
        if( ch == '\\')
        {
            strPresetPath = strPresetPath.SubString(1,ilen-1);
        }
        EditPathName->Text = strPresetPath;
    }
}
//---------------------------------------------------------------------------




void __fastcall TFormMain::ScrollBarAdjustChange(TObject *Sender)
{
    if( iAdjustDrawNum != 1)
    {
        return;
    }
   	int iLen,ipage;
    if(!bDrawPart)
    {
        iLen = iChoiceEnd - iChoiceStart;
        if(ScrollBarAdjust->Max == 0)
        {
             return;
        }
        if(iLen == lDS.iPLength)
        {
            return;
        }
        iChoiceStart = ScrollBarAdjust->Position;
        if(iChoiceStart*2 > ScrollBarAdjust->Max)
        {
            iChoiceStart = iChoiceStart;
        }
        if(iChoiceStart == 0)
        {
          	iChoiceStart = 1;
        }
        iChoiceEnd = iChoiceStart + iLen;
        if(iChoiceEnd > lDS.iPLength)
        {
        	iChoiceEnd = lDS.iPLength;
        }

//        if(fXData[iChoiceStart]>0.0)
//        {
//            return;
//        }
 //   bDrawed=false;
    }
    else
    {
        iLen = iCCEnd-iCCStart;
        if(ScrollBarAdjust->Max == 0)
        {
             return;
        }
        iCCStart = ScrollBarAdjust->Position;
        if(iCCStart < lDS.iStartFai)
        {
          	iCCStart = lDS.iStartFai;
            return;
        }
        iCCEnd = iCCStart + iLen;
        if(iCCEnd > lDS.iEndFai)
        {
        	iCCEnd = lDS.iEndFai;
        }

//        if(fXData[iCCStart]>0.0)
//        {
//            return;
//        }
    }

    PaintBoxAdjustTDC->Repaint();
}
//---------------------------------------------------------------------------





void __fastcall TFormMain::ActionOperationAbstractDataExecute(
      TObject *Sender)
{
    PageControlMain->ActivePageIndex  = 4; //������ȡ��Ч����ҳ
    PageControlCurve->ActivePageIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionOperationAdjustTDCExecute(TObject *Sender)
{
//    bPSTOpened = false;
//    PanelSingleBack->Visible = true;
    PageControlMain->ActivePageIndex  = 4;
    PageControlCurve->ActivePageIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButtonLogPLogVClick(TObject *Sender)
{
    if(AllDrawed(Sender,drawLogVLogP))
    {
        return;
    }
    TCursor Save_Cursor = Screen->Cursor ;
    try
    {
        Screen->Cursor = crHourGlass;
        if(!DetectPSTStatus())
        {
            return;
        }
//        PaintBoxAll->Refresh();
        DrawImageAll();
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionFilePrintExecute(TObject *Sender)
{
    if(PageControlMain->ActivePageIndex != 4)
    {
        Application->MessageBox("û�п��Դ�ӡ������","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    else
    {
        if(  (PageControlCurve->ActivePageIndex == 1)
           ||(PageControlCurve->ActivePageIndex == 3)
        )
        {
            Application->MessageBox("û�п��Դ�ӡ������","��ʾ",MB_OK|MB_ICONINFORMATION);
            return;
        }
    }
    bControlFromPrinter = true;
    FormPrint = new TFormPrint(FormMain);
    if( FormPrint == NULL)
    {
        Application->MessageBox("������ӡ����ʱ���ִ���","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }

    int iCount,iItem;
    iCount    = ListBoxPSTFile->Items->Count;
    iItem     = ListBoxPSTFile->ItemIndex;
    FormPrint->ComboBoxList->Items->Clear();
    AnsiString strTemp;
    for(int i = 0; i < iCount; i++)
    {
        strTemp = ListBoxPSTFile->Items->Strings[i];
        FormPrint->ComboBoxList->Items->Add(strTemp);
    }
    FormPrint->ComboBoxList->ItemIndex = iItem;
    iCount = ListBoxPCPFile->Items->Count;
    FormPrint->iLibPosi = ListBoxPSTFile->ItemIndex;
    iItem  = ListBoxPCPFile->ItemIndex;
    FormPrint->ComboBoxFile->Items->Clear();
    for(int i=0;i<iCount;i++)
    {
/*        strTemp = strStringListPath->Strings[i];
        strTemp += ListBoxFile->Items->Strings[i];
        strTemp = strStringListDriver->Strings[i]+strTemp;
*/
        strTemp = ListBoxPCPFile->Items->Strings[i];
        FormPrint->ComboBoxFile->Items->Add(strTemp);
    }

    if(iCurrentDraw == 1)  //�����ǰ�����ǵ���ͼ�εĶ��ͼ�ε���ϴ���
    {
        FormPrint->ComboBoxFile->ItemIndex = iItem;
    }
    if(PageControlCurve->ActivePageIndex == 0)
    {
        FormPrint->SetbPrintDrawGrid(bPCPGridLine);
    }
    else
    {
        FormPrint->SetbPrintDrawGrid(bPSTGridLine);
    }
    FormPrint->SetbDrawBasicCurve( bDrawBasicCurve );
    FormPrint->SetiDrawType(iCurrentDraw);

    FormPrint->ShowModal();                 //TMemo

    bControlFromPrinter = false;
    delete FormPrint;
    FormPrint  = NULL;
    fXFactor   = 1.0;
    fYFactor   = 1.0;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionFileReadPSTFileExecute(TObject *Sender)
{
    OpenDialog1->InitialDir = strPSTFileDirectory;
    OpenDialog1->Title = "�������б��ļ� *.PST";
    OpenDialog1->Filter = "�����б��ļ� *.PST|*.PST";
    if(OpenDialog1->Execute())
    {
        strPSTFileDirectory = ExtractFilePath(OpenDialog1->FileName);
        strValidPath = strPSTFileDirectory;
        FileListBoxFile->Directory = strPSTFileDirectory;
        AnsiString strt = ExtractFileExt(OpenDialog1->FileName);
        strt = strt.UpperCase();
        if( strt == ".PST" )
        {
            strPSTName = OpenDialog1->FileName;
            ListBoxPSTFile->Items->Add(strPSTName);
            ListBoxPSTFile->Refresh();
            ListBoxPCPFile->Items->LoadFromFile(strPSTName);
            ListBoxPCPFile->Refresh();
            ListBoxPST->Items->LoadFromFile(strPSTName);
            ListBoxPST->Refresh();
            iChoicedNum = ListBoxPST->Items->Count;
            int icount = ListBoxPSTFile->Items->Count;
            ListBoxPSTFile->ItemIndex = icount - 1;
            LoadCurrentPST();
//            LoadPSTFile();
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionFileReadPCPFileExecute(TObject *Sender)
{
    if(!bParameterInited)
    {
        InitParameters();
        bParameterInited = true;
    }
    OpenDialog1->InitialDir = strPSTFileDirectory;
    OpenDialog1->Title = "����ȡ�������ļ� *.PCP";
    OpenDialog1->Filter = "��ȡ�������ļ� *.PCP|*.PCP";

    if(OpenDialog1->Execute())
    {
        strPSTFileDirectory = ExtractFilePath(OpenDialog1->FileName);
        FileListBoxFile->Directory = strPSTFileDirectory;
        AnsiString strt = ExtractFileExt(OpenDialog1->FileName);
        strt = strt.UpperCase();
        if( strt == ".PCP" )
        {
            strPCPName = OpenDialog1->FileName;
            LoadCurrentBinPCP(false);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionFileReadOriginalFileExecute(
      TObject *Sender)
{
    if(!bParameterInited)
    {
        InitParameters();
        bParameterInited = true;
    }
    OpenDialog1->InitialDir = strPSTFileDirectory;
    OpenDialog1->Title = "��ԭʼ�����ļ� *.ORG";
    OpenDialog1->Filter = "ԭʼ�����ļ� *.ORG|*.ORG";

    if(OpenDialog1->Execute())
    {
        AnsiString strt = ExtractFileExt(OpenDialog1->FileName);
        strt = strt.UpperCase();
        if( strt == ".ORG" )
        {
            strOriginalName = OpenDialog1->FileName;
            LoadCurrentBinORG();
        }
    }
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::ListBoxPSTFileDblClick(TObject *Sender)
{
    int iIndex,iCount;
    iCount = ListBoxPSTFile->Items->Count;
    iIndex = ListBoxPSTFile->ItemIndex;
    if((iIndex <0)||(iCount <= 0))
    {
        return;
    }
    strPSTName = ListBoxPSTFile->Items->Strings[iIndex];
    ListBoxPCPFile->Items->LoadFromFile(strPSTName);
    ListBoxPCPFile->Refresh();
    bLoadList = true;
    bAllHRR = false;
    iListDrawKind = 1;
    LoadCurrentPST();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ListBoxPCPFileDblClick(TObject *Sender)
{
    int iCount,iIndex;
    iIndex = ListBoxPCPFile->ItemIndex;
    iCount = ListBoxPCPFile->Items->Count;
    if((iIndex <0) ||(iCount<=0))
    {
        return;
    }
    strPCPName = ListBoxPCPFile->Items->Strings[iIndex];
    LoadCurrentBinPCP(false);
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::DisplayHint(TObject* Sender)
{
    //TODO: Add your source code here
    StatusBarMain->Panels->Items[0]->Text = GetLongHint(Application->Hint);
}
void __fastcall TFormMain::ToolButtonPowerClick(TObject *Sender)
{
    if(AllDrawed(Sender,drawDW))
    {
        return;
    }
    AllDrawKind = drawDW;
    TCursor Save_Cursor = Screen->Cursor ;
    try
    {

        Screen->Cursor = crHourGlass;
        if(!DetectPSTStatus())
        {
            return;
        }
        bFileChanged = true;
//        PaintBoxAll->Refresh();
        DrawImageAll();
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ppmConGridClick(TObject *Sender)
{
//    ppmConGrid->Checked = !ppmConGrid->Checked;
    bPSTGridLine = !bPSTGridLine;
    if(bPSTGridLine)
    {
        ppmConGrid->Caption = "ȡ��������";
    }
    else
    {
        ppmConGrid->Caption = "����������";
    }
    if(PageControlCurve->ActivePageIndex == 2)
    {
        DrawImageAll();
//        PaintBoxAll->Refresh();
    }
/*
    else if(PageControlCurve->ActivePageIndex == 0)
    {
        PaintBoxAdjustTDC->Refresh();
    }
*/
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::FileListBox2DblClick(TObject *Sender)
{
    strPCPName = FileListBox2->FileName;
    FunHeatRelease();
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::ppMainMenuClick(TObject *Sender)
{
    bool bChecked;
    bChecked = ToolBarMainMenu->Visible;
    if(bChecked)
    {
        ToolBarMainMenu->Visible = false;
        ppMainMenu->Caption = "��ʾ���˵�";
        if(!ToolBarMainToolBar->Visible)
        {
            FormGoBack->Visible = true;
            bShowAll = true;
        }
    }
    else
    {
        ToolBarMainMenu->Visible = true;
        ppMainMenu->Caption = "�������˵�";
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppMainToolbarClick(TObject *Sender)
{
    bool bChecked;
    bChecked = ToolBarMainToolBar->Visible;
    if(bChecked)
    {
        ToolBarMainToolBar->Visible = false;
        ppMainToolbar->Caption = "��ʾ������";
        if(!ToolBarMainMenu->Visible)
        {
            bShowAll = true;
            FormGoBack->Visible = true;
        }
    }
    else
    {
        ToolBarMainToolBar->Visible = true;
        ppMainToolbar->Caption = "���ع�����";
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::mnuOtherHideToolbarClick(TObject *Sender)
{
    bool bChecked;
    bChecked = ToolBarMainToolBar->Visible;
    if(bChecked)
    {
        ToolBarMainToolBar->Visible = false;
        mnuOtherHideToolbar->Caption = "��ʾ������";
        ppMainToolbar->Caption = "��ʾ������";
    }
    else
    {
        ToolBarMainToolBar->Visible = true;
        mnuOtherHideToolbar->Caption = "���ع�����";
        ppMainToolbar->Caption = "���ع�����";
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::N1Click(TObject *Sender)
{
    CoolBar1->Visible = false;
    FormMain->BorderStyle = bsNone;
//    FormGoBack->Left = FormMain->Width - FormGoBack->Width;
//    FormGoBack->Top = FormMain->Height - FormGoBack->Height;
    FormGoBack->Visible = true;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ToolBarMainMenuExit(TObject *Sender)
{
    if(ToolBarMainToolBar->Visible == false)
    {
        FormGoBack->Visible = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolBarMainToolBarExit(TObject *Sender)
{
    if(ToolBarMainMenu->Visible == false)
    {
        FormGoBack->Visible = true;
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::mnuHelpAboutClick(TObject *Sender)
{
    AboutBox = new TAboutBox(FormMain);
    AboutBox->ShowModal();
    delete AboutBox;
    AboutBox = NULL;    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton22Click(TObject *Sender)
{
    ((TToolButton*)Sender)->Down = true;
    ActionOperationReadDataExecute(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton23Click(TObject *Sender)
{
    ((TToolButton*)Sender)->Down = true;
    FilterComboBoxFile->ItemIndex = 0;          //ָ��ORG������
    FilterComboBoxFileChange(NULL);
    PageControlMain->ActivePageIndex  = 4;    //ָ���ͼ��ʾҳ
    PageControlCurve->ActivePageIndex = 1;   //ָ��ORGԭʼ����ҳ
    FilterComboBoxFile->ItemIndex     = iFileExt;
    ToolButtonORG->Down                  = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton24Click(TObject *Sender)
{
    ((TToolButton*)Sender)->Down = true;
    PageControlMain->ActivePageIndex = 3;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ToolButton26Click(TObject *Sender)
{
    ((TToolButton*)Sender)->Down = true;
    FilterComboBoxFile->ItemIndex = 1;          //ָ��PCP������
    FilterComboBoxFileChange(NULL);
    PageControlMain->ActivePageIndex = 4;    //ָ���ͼ��ʾҳ
    PageControlMain->Refresh();
    PageControlCurve->ActivePageIndex = 0;   //ָ��PCP��ʾҳ
    PageControlCurve->Refresh();
    ToolButtonPCP->Down = true;
//    PaintBoxAdjustTDC->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton36Click(TObject *Sender)
{
    ((TToolButton*)Sender)->Down = true;
    FilterComboBoxFile->ItemIndex = 2;          //ָ��PST������
    FilterComboBoxFileChange(NULL);
    PageControlMain->ActivePageIndex = 4;
    PageControlMain->Refresh();
    PageControlCurve->ActivePageIndex = 2;   //
    PageControlCurve->Refresh();
    ToolBar1->Refresh();
    ToolButtonPST->Down = true;
//    PaintBoxAll->Refresh();
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::BrowseForFolder(AnsiString Title, WideString Root, AnsiString & Directory)
{
    //TODO: Add your source code here

    BROWSEINFO Brinfo;
    char *Buffer;
    LPITEMIDLIST RootItemIDList,ItemIDList;
    LPMALLOC ShellMalloc;
    LPSHELLFOLDER DesktopFolder;
    DWORD Eaten,Flags;
    ZeroMemory(&Brinfo,sizeof(BROWSEINFO));

    if(SHGetMalloc(&ShellMalloc) == S_OK &&ShellMalloc != NULL )
    {
        Buffer = (char*) ShellMalloc->Alloc(MAX_PATH);
        strcpy(Buffer,Directory.c_str());
        SHGetDesktopFolder(&DesktopFolder);
        DesktopFolder->ParseDisplayName( BitBtn1->Handle, // Handle,
                                        NULL,
                                        Variant(Root),
                                        &Eaten,
                                        &RootItemIDList,
                                        &Flags
                                        );
        Brinfo.hwndOwner = BitBtn1->Handle;//,Handle;
        Brinfo.pidlRoot = RootItemIDList;
        Brinfo.pszDisplayName = Buffer;
        Brinfo.lpszTitle = Title.c_str();
        Brinfo.ulFlags = BIF_RETURNONLYFSDIRS|
                         BIF_EDITBOX;
/*

    BROWSEINFO bi;
  	char GDir[MAX_PATH];
  	char FolderName[MAX_PATH];
  	LPITEMIDLIST ItemID;
  	memset(&bi, 0, sizeof(BROWSEINFO));
  	memset(GDir, 0, MAX_PATH);
  	bi.hwndOwner      = Handle;
  	bi.pszDisplayName = FolderName;
  	bi.lpszTitle      = "Select Directory to Search In!";
  	ItemID = SHBrowseForFolder(&bi);
  	SHGetPathFromIDList(ItemID, GDir);
*/

        ItemIDList = SHBrowseForFolder(&Brinfo);
        if(ItemIDList != NULL)
        {
            SHGetPathFromIDList(ItemIDList,Buffer);
            ShellMalloc->Free(ItemIDList);
            Directory = Buffer;
            return true;
        }
        else
        {
            ShellMalloc->Free(Buffer);
            return false;
        }
    }
    return true;
}

void __fastcall TFormMain::ToolButtonMMaxClick(TObject *Sender)
{
    if(AllDrawed(Sender,drawMAX))
    {
        return;
    }
/*
    if(CheckBox3D->Checked)
    {
        return;
    }
*/
    TCursor Save_Cursor = Screen->Cursor ;
    try
    {

        Screen->Cursor = crHourGlass;
        if(!DetectPSTStatus())
        {
            return;
        }
        if(iListDrawKind != 1)
        {
            return;
        }
//        PaintBoxAll->Refresh();
        DrawImageAll();
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button1Click(TObject *Sender)
{
    float *fVVV = NULL;
    FILE* fp = NULL;
    Chart2->Title->Text->Strings[0]    = "����������";
    Chart2->BottomAxis->Title->Caption = "����ת��(CA)";
    Chart2->LeftAxis->Title->Caption   = "������";

    if(!InitParameters())
    {
        return;
    }

    for( int i=0;i<8;i++)
    {
        Chart2->Series[i]->Clear();
    }
    fVVV = new float[901];
    if( fVVV == NULL )
    {
        return;
    }
    for(int i=0;i<901;i++)
    {
        fVVV[i] = (float)i/5.0 - 180.0;
    }
    float ftemp0,ftemp1,fval1;
    float fDV1 = 0.0,fDV2 = 0.0,fDV3 = 0.0;
    float ftt0,ftt1;
    float ftemp2;
    float ftemp3;
    float fDV1Max = 0.0,fDV2Max = 0.0,fDV3Max = 0.0,fvmax = 0.0;
    AnsiString strTTemp;

    lDS.GetVolumn(fVVV[0],ftemp0);
    strTTemp = strFileDirectory + "modalsin240.dat";
    if((fp = fopen(strTTemp.c_str(),"w"))!=NULL)
    {
        for(int i=1 ;i<901; i++)
        {
            ftemp3 = lDS.GetVolumn(fVVV[i],ftemp1);
            if(ftemp3 > fvmax)
            {
                fvmax = ftemp3;
            }
            fval1 = pow(ftemp1,-1.3) - pow(ftemp0,-1.3);
            fDV1 = fval1/100.0;
            fval1 = pow(ftemp1,-1.4) - pow(ftemp0,-1.4);
            fDV2 = fval1/100.0;
            fval1 = pow(ftemp1,-1.5) - pow(ftemp0,-1.5);
            fDV3 = fval1/100.0;
            ftemp0 = ftemp1;
            if(fDV1 > fDV1Max)
            {
                fDV1Max = fDV1;
            }
            if(fDV2 > fDV2Max)
            {
                fDV2Max = fDV2;
            }
            if(fDV3 > fDV3Max)
            {
                fDV3Max = fDV3;
            }
        }
        lDS.GetVolumn(fVVV[0],ftemp0);
        for(int i=1; i<901; i++)
        {
            lDS.GetVolumn(fVVV[i],ftemp1);
            fval1 = pow(ftemp1,-1.3) - pow(ftemp0,-1.3);
            fDV1 = fval1/100.0;
            Chart2->Series[1]->AddXY(fVVV[i],fDV1,"",clGreen);
            fval1 = pow(ftemp1,-1.4) - pow(ftemp0,-1.4);
            fDV2 = fval1/100.0;
            Chart2->Series[2]->AddXY(fVVV[i],fDV2,"",clBlue);
            fval1 = pow(ftemp1,-1.5) - pow(ftemp0,-1.5);
            fDV3 = fval1/100.0;
            Chart2->Series[3]->AddXY(fVVV[i],fDV3,"",clYellow);
            ftemp0 = ftemp1;
            ftemp2 = sin(3.1415926*fVVV[i]/180.0);
            fprintf(fp,"%.3f %.6f %.6f %.6f %.6f %.6f\n",
                       fVVV[i],
                       ftemp1/fvmax,
                       ftemp2,
                       fDV1/fDV1Max,
                       fDV2/fDV2Max,
                       fDV3/fDV3Max
                    );
        }
        fclose(fp);
    }
    delete []fVVV;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button2Click(TObject *Sender)
{
    float fPPP0[40];
    float fTTT0[40];
    float fMMM02[40];
    float fMMM04[40];
    float fMMM06[40];
    float fMMM08[40];
    float fMMM10[40];
    float fvr[901];
    float fTOO, fTNN;
    float flmax02,flmax04,flmax06,flmax08,flmax10;
    float fPOO02, fPOO04, fPOO06, fPOO08, fPOO10;
    float fVOO02, fVOO04, fVOO06, fVOO08, fVOO10;
    float fPNN02, fPNN04, fPNN06, fPNN08, fPNN10;
    float fVNN02, fVNN04, fVNN06, fVNN08, fVNN10;
    float *fVVV = NULL;
    FILE* fp = NULL;
    AnsiString strTemp;
    Chart2->Title->Text->Strings[0]    = "����������";
    Chart2->BottomAxis->Title->Caption = "����ת��(CA)";
    Chart2->LeftAxis->Title->Caption   = "������";

    if(!InitParameters())
    {
        return;
    }
    //�Խ���ѹ���ͽ����¶Ƚ��г�ʼ��
    for(int i = 0; i < 40; i++)
    {
        fPPP0[i] = 50000.0  +  (float)i * 5000.0 ;
        fTTT0[i] = 283.0    +  (float)i * 2.0  ;

    }
    for( int i=0;i<4;i++)
    {
        Chart2->Series[i]->Clear();
    }
    fVVV = new float[901];
    if( fVVV == NULL )
    {
        return;
    }
    for(int i=0;i<901;i++)
    {
        fVVV[i] = (float)i/5.0 - 180.0;
        fvr[i]  = 1.4          - (float)i*1.11111e-4;
    }
    float ftemp0,ftemp1,fval1;
    float fDV1,fDV2,fDV3;
    float ftt0,ftt1;
    float ftemp2;
    float ftemp3;
    AnsiString strTTemp;
    strTTemp = strFileDirectory + AnsiString("modalDPDF.dat");
    for( int j = 0; j < 8 ; j++)
    {
        Chart1->Series[j]->Clear();
        Chart2->Series[j]->Clear();
    }
    MemoMaxValue->Lines->Clear();
    if((fp = fopen(strTTemp.c_str(),"w"))!=NULL)
    {
//        for(int m = 0; m < 40; m++)       //�����¶�    MPa
//        {

            for(int n = 0; n < 40; n += 5)   //����ѹ��    K
            {
                lDS.GetVolumn(fVVV[0],ftemp0);
                fPOO02 = fPPP0[n];          //��ʼѹ��
                fVOO02 = ftemp0;           //��ʼ�ݻ�
                fPOO04 = fPPP0[n];          //��ʼѹ��
                fVOO04 = ftemp0;           //��ʼ�ݻ�
                fPOO06 = fPPP0[n];          //��ʼѹ��
                fVOO06 = ftemp0;           //��ʼ�ݻ�
                fPOO08 = fPPP0[n];          //��ʼѹ��
                fVOO08 = ftemp0;           //��ʼ�ݻ�
                fPOO10 = fPPP0[n];          //��ʼѹ��
                fVOO10 = ftemp0;           //��ʼ�ݻ�
                flmax02 = 0.0;
                flmax04 = 0.0;
                flmax06 = 0.0;
                flmax08 = 0.0;
                flmax10 = 0.0;
                for(int i=1 ;i<901; i++)  //�ݻ��仯
                {
                    ftemp3 = lDS.GetVolumn(fVVV[i],ftemp1);
                    fPNN02   = fPOO02 * pow(fVOO02/ftemp3, fvr[i]);
                    if(flmax02 < fPNN02 - fPOO02)
                    {
                        flmax02 = fPNN02 - fPOO02;
                    }

                    Chart1->Series[n/5]->AddXY(fVVV[i],fPNN02,"",clColor[n/5]);
                    Chart2->Series[n/5]->AddXY(fVVV[i],(fPNN02 - fPOO02)*5.0 ,"",clColor[n/5]);
                    fPOO02 = fPNN02;
                    fVOO02 = ftemp3;
                    if( i%2 == 0)
                    {
                        fPNN04 = fPOO04 * pow(fVOO04/ftemp3, fvr[i]);

                        if(flmax04 < fPNN04 - fPOO04)
                        {
                            flmax04 = fPNN04 - fPOO04;
                        }
                        fPOO04 = fPNN04;
                        fVOO04 = ftemp3;
                    }
                    if( i%3 == 0)
                    {
                        fPNN06 = fPOO06 * pow(fVOO06/ftemp3, fvr[i]);
                        if(flmax06 < fPNN06 - fPOO06)
                        {
                            flmax06 = fPNN06 - fPOO06;
                        }
                        fPOO06 = fPNN06;
                        fVOO06 = ftemp3;
                    }
                    if( i%4 == 0)
                    {
                        fPNN08 = fPOO08 * pow(fVOO08/ftemp3, fvr[i]);
                        if(flmax08 < fPNN08 - fPOO08)
                        {
                            flmax08 = fPNN08 - fPOO08;
                        }
                        fPOO08 = fPNN08;
                        fVOO08 = ftemp3;
                    }
                    if( i%5 == 0)
                    {
                        fPNN10 = fPOO10 * pow(fVOO10/ftemp3, fvr[i]);
                        if(flmax10 < fPNN10 - fPOO10)
                        {
                            flmax10 = fPNN10 - fPOO10;
                        }
                        fPOO10 = fPNN10;
                        fVOO10 = ftemp3;
                    }

                }
                strTemp.printf("��ѹ:%.3f, ���ѹ����%.2f",fPPP0[n]/1000000.0,fPNN02/1000000.0);
                MemoMaxValue->Lines->Add(strTemp);
                strTemp.printf("��ѹ:%.3f, ���ѹ���ʣ�%.4f",fPPP0[n]/1000000.0,flmax02/1000000.0*5);
                MemoDPMaxValue->Lines->Add(strTemp);
                fprintf(fp, "%.6f %.6f %.6f %.6f %.6f\n",
                                       flmax02/1000000.0*5,
                                       flmax04/1000000.0*5,
                                       flmax06/1000000.0*5,
                                       flmax08/1000000.0*5,
                                       flmax10/1000000.0*5
                                       );
                fMMM02[n] = flmax02 / 1000000.0*5;
                fMMM04[n] = flmax04 / 1000000.0*2.5;
                fMMM06[n] = flmax06 / 1000000.0*1.666667;
                fMMM08[n] = flmax08 / 1000000.0*1.25;
                fMMM10[n] = flmax10 / 1000000.0;
            }

        fclose(fp);
    }
    delete []fVVV;
}

/*
void __fastcall TFormMain::BitBtnReadListClick(TObject *Sender)
{

    long lRet;
    unsigned char *ptch;
    if(bUSBWorking)
    {
        Application->MessageBox("���ڷ���USB!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    bUSBWorking = true;
    if(!bUSBOpened)
    {
        lRet = OpenUsb();
        ::Sleep(50);
        if( lRet == 1 )
        {
            bUSBOpened = true;
        }
        else
        {
            Application->MessageBox("�޷���USB�ӿ�!","��ʾ",MB_OK|MB_ICONINFORMATION);
            bUSBWorking = false;
            bUSBOpened  = false;
            return;
        }
    }

    TCursor Save_Cursor = Screen->Cursor ;    // Show hourglass cursor
    try
    {
        strComPart[0] = 0xAA;
        strComPart[1] = 0x55;
        lRet = WriteUsb( 64, strComPart );
        ::Sleep(50);
        if( lRet != 64 )
        {
            Application->MessageBox("�޷�������д������!","��ʾ",MB_OK|MB_ICONINFORMATION);
            if( bUSBOpened )
            {
                CloseUsb();
                bUSBOpened  = false;
                bUSBWorking = false;
            }
            return;
        }
        strComPart[0] = 'B';
        strComPart[1] = 'A';
        while(true)
        {
            lRet = ReadUsb(64,strComPart);
            ::Sleep(50);
            if( lRet == 64 )
            {
                if( (strComPart[0] == 0xAA)&&(strComPart[2] == 0x55) )
                {
                    break;
                }
            }
            else
            {
                Application->MessageBox("�������ж�ȡ����ʱ���ִ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
                if( bUSBOpened )
                {
                    CloseUsb();
                    bUSBOpened  = false;
                    bUSBWorking = false;
                }
                return;
            }
        }
        int inow;
        for(int j=0;j<496;j++)
        {
            strData[j*64] = 0;
        }
        for(int i=0;i< 128; i++)
        {
            ptch = &strData[i*128];
            lRet = ReadUsb( 128, ptch);
            ::Sleep(50);
            inow = strData[i*128];
            if( lRet != 128)
            {
                break;
            }
        }
    //���ϲ������ļ�Ŀ¼��������
        strComPart[0] = 0XBB;
        strComPart[1] = 0XAA;
        lRet = WriteUsb(64,strComPart );
        ::Sleep(50);
        iFileNum = 0;
        SGDirectory->RowCount = 2;
        FillGrid();
        bUSBWorking = false;
     __finally
    {
        Screen->Cursor = Save_Cursor;
    }

}
*/
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnReadCurrentClick(TObject *Sender)
{

    TCursor Save_Cursor = Screen->Cursor ;    // Show hourglass cursor
//    Screen->Cursor      = crGlass;
    try
    {
        Screen->Cursor = crHourGlass;
        ReadCurrentList();                   //��ȡ��ǰѡ�е��ļ�
    }
    __finally
    {
        Screen->Cursor = Save_Cursor ;    // Show hourglass cursor
    }

}
//---------------------------------------------------------------------------


void __fastcall TFormMain::BitBtnCloseUSBClick(TObject *Sender)
{
    if(bUSBWorking)
    {
        Application->MessageBox("���ڷ���USB!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    if(bUSBOpened)
    {
        CloseUsb();
        bUSBOpened = false;
        Application->MessageBox("USB�ӿ��ѱ��ر�!","��ʾ",MB_OK|MB_ICONINFORMATION);
    }
    else
    {
        Application->MessageBox("USB�ӿ���ͱ��ر���!","��ʾ",MB_OK|MB_ICONINFORMATION);
    }

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FillGrid(void)
{
    //TODO: Add your source code here
    int i;
    int itemp,itemp2;
//    AnsiString strPassage;
    unsigned char *chTemp;
    unsigned char chTarget = 0xFF;
    AnsiString strTT,strTN,strTemp;
    float fspeed;
    bool  bcombustion;
    bool  bFileValid = false;

//-----------------------------------------------------------
// ��ʼ������
    iTrainTimes = 0;
    ListPath->Clear();
    ListList->Clear();
    ListBoxTrainList->Items->Clear();
    ListBoxPCPFile->Items->Clear();

    for( i = 0; i < 800; i ++)
    {
        iListNum[i] = 1000;   //�Ƚ�����Ϊ1000����ʾ������
    }
//============================================================

    int iNum=0;
    for( i=0; i<ALL_FILE_NUM; i++)
    {
        chTemp = &strData[i*128];
        if( (chTemp[0] == chTarget ) )
        {
            iNum++;
        }
    }
    AnsiString strTempInfo = "";

//--------------------------------------------------
// ��ʼ���ļ���Ϣ
    for( int i = 0; i < MAXTRAININFONUMS; i ++)
    {
        myMTrainInfo[i].bSelected    = false;
        myMTrainInfo[i].iStartPosi   = 0;
        myMTrainInfo[i].iEndPosi     = 0;
        myMTrainInfo[i].strTrainInfo = "";
    }

//======================================================

    for( i=0; i<iNum; i++)
    {
//        if( i == 39)
//        {
//            i = i;
//        }
        if(GetFileHead(i))
        {
            bFileValid = true;
            if( i == 0 )   //���������ǵ�һ���ļ�
            {
                myMTrainInfo[0].iStartPosi   = 0;
                myMTrainInfo[0].iEndPosi     = 0;
                myMTrainInfo[0].bSelected    = true;
                myMTrainInfo[0].strTrainInfo =   strTrainType
                                               + strTrainNumber
                                               + "_"
                                               + strTimeShort;
            }
            else
            {
                strTempInfo =   strTrainType
                              + strTrainNumber
                              + "_"
                              + strTimeShort;
                if( strTempInfo == myMTrainInfo[iMTrainInfoNums].strTrainInfo )
                {
                    myMTrainInfo[iMTrainInfoNums].iEndPosi ++;
                }
                else
                {
                    iMTrainInfoNums ++;
                    myMTrainInfo[iMTrainInfoNums].bSelected    = true;
                    myMTrainInfo[iMTrainInfoNums].iStartPosi   = i;
                    myMTrainInfo[iMTrainInfoNums].iEndPosi     = i;
                    myMTrainInfo[iMTrainInfoNums].strTrainInfo = strTempInfo;
                }
            }
            SGDirectory->Cells[0][iFileNum+1] = iFileNum+1;
            SGDirectory->Cells[1][iFileNum+1] = strTrainType;
            SGDirectory->Cells[2][iFileNum+1] = strTrainNumber;
            SGDirectory->Cells[3][iFileNum+1] = strDieselNumber;
            SGDirectory->Cells[4][iFileNum+1] = strPPSignalType[0];
            SGDirectory->Cells[5][iFileNum+1] = strPPSignalType[1];
            SGDirectory->Cells[6][iFileNum+1] = strPPSignalType[2];
            SGDirectory->Cells[7][iFileNum+1] = strFrequency;
            SGDirectory->Cells[8][iFileNum+1] = strSampleLength;
            SGDirectory->Cells[9][iFileNum+1] = strMTime;

            SGParams->Cells[0][iFileNum+1] = iFileNum+1;
            SGParams->Cells[1][iFileNum + 1] = strTrainType;
            SGParams->Cells[2][iFileNum+1] = strTrainNumber;
            SGParams->Cells[3][iFileNum+1] = strDieselNumber;
            strTemp.printf("%.1f", LFHS.fEngineSpeed);
            SGParams->Cells[4][iFileNum+1] = strTemp;

            if(LFHS.chOil == 0)
            {
                strTemp = "��";
            }
            else
            {
                strTemp = "����";
            }
            SGParams->Cells[5][iFileNum+1] = strTemp;

            if(LFHS.chCombustion == 0)
            {
                strTemp = "ѹ��";
            }
            else
            {
                strTemp = "ȼ��";
            }
            SGParams->Cells[6][iFileNum+1] = strTemp;

            strTemp.printf("%.2f", LFHS.fCPMax);
            SGParams->Cells[7][iFileNum+1] = strTemp;
            strTemp.printf("%.3f", LFHS.fDPDFMax);
            SGParams->Cells[8][iFileNum+1] = strTemp;
            if(LFHS.chOil == 0)
            {
                strTemp = "";
            }
            else
            {
                strTemp.printf("%.1f", LFHS.fOilAngle);
            }
            SGParams->Cells[9][iFileNum+1] = strTemp;
            if(LFHS.chCombustion == 0)
            {
                strTemp = "";
            }
            else
            {
                strTemp.printf("%.1f", LFHS.fCombustionAngle);
            }
            SGParams->Cells[10][iFileNum+1] = strTemp;

            iFileNum++;
            SGDirectory->RowCount++;
            SGParams->RowCount ++;
        }
        else
        {
            break;
        }
// ��TrainList�����Ӳ�����Ϣ
        if(i > 0)
        {
            if(   (strTT != strTrainType)
                ||(strTN != strTrainNumber)
                ||(fabs(fspeed - LFHS.fEn) > f_DELTASPEED)    //����60r/min
                ||(bcombustion != (bool)(LFHS.chOil))
                )
            {
                strTemp.printf("%d.0rpm %04d-%02d-%02d",(int)((LFHS.fEn + 5.0)/10.0)*10,
                                                        LFHS.siYear,
                                                        LFHS.siMonth,
                                                        LFHS.siDate
                                                        );
                if(!HaveTrainList())
                {
                    if((bool)(LFHS.chOil))
                    {
                        strTemp = strTrainType + strTrainNumber + "_ȼ�չ��� " + strTemp;
                    }
                    else
                    {
                        strTemp = strTrainType + strTrainNumber + "_ѹ������ " + strTemp;
                    }
                    ListBoxTrainList->Items->Add(strTemp);
                    strTemp = strPresetPath + "\\" + strTrainType + "\\" + strTrainNumber + AnsiString("\\");
                    ListPath->Add(strTemp);
                }
            }
        }
        else
        {
            strTemp.printf("%d.0rpm %04d-%02d-%02d",(int)((LFHS.fEn + 5.0)/10.0)*10,
                                                    LFHS.siYear,
                                                    LFHS.siMonth,
                                                    LFHS.siDate
                                                    );
            if(!HaveTrainList())
            {
               if((bool)(LFHS.chOil))
               {
                    strTemp = strTrainType + strTrainNumber + "_ȼ�չ��� " + strTemp;
               }
               else
               {
                    strTemp = strTrainType + strTrainNumber + "_ѹ������ " + strTemp;
               }
               ListBoxTrainList->Items->Add(strTemp);
               strTemp = strPresetPath + "\\" + strTrainType + "\\" + strTrainNumber + AnsiString("\\");
               ListPath->Add(strTemp);
           }
           
        }
        strTT        = strTrainType;             //����
        strTN        = strTrainNumber;           //����
        fspeed       = LFHS.fEn;                 //ת��
        bcombustion  = (bool)LFHS.chOil;  //�Ƿ���ȼ�չ���
        int ii = iTrainTimes*16 + LFHS.siDieselNum - 1;
        if(ii < 800)
        {
            if(iListNum[ii] == 1000)
            {
                iListNum[ii] = i;
            }
        }
    }
    if(   (iMTrainInfoNums > 0)
        ||(bFileValid == true)
        )
    {
        iMTrainInfoNums ++;
    }
    iTrainTimes = ListBoxTrainList->Items->Count;
    if(iFileNum > 0)    //������ļ��������һ�п���ɾ��
    {
        SGDirectory->RowCount--;
    }
}

bool __fastcall TFormMain::HaveTrainList(void)
{
    //TODO: Add your source code here
    int iindex,icount;
    bool bhave = false;
    AnsiString strOld,strNew,strN,strComNew;
    AnsiString strComOld;
    int iposi,ipoint;
    float fspeedold,fspeednew;
    icount = ListBoxTrainList->Items->Count;
    if(icount > 0)
    {
        if((bool)LFHS.chOil)
        {
            strComNew = "ȼ��";
        }
        else
        {
            strComNew = "ѹ��";
        }
        strNew = strTrainType + strTrainNumber;
        fspeednew = LFHS.fEn;
        for(int i = 0; i < icount; i ++)
        {
            strOld = ListBoxTrainList->Items->Strings[i];
        //    ilen   = strOld.Length();
            iposi  = strOld.Pos("_");
            strComOld = strOld.SubString(iposi + 1,4);

            iposi  = strOld.Pos(" ");
            ipoint = strOld.Pos(".");
            strN   = strOld.SubString(iposi + 1, ipoint - iposi + 1);
            fspeedold  = StrToFloat(strN);
            iposi  = strOld.Pos("_");
            strOld = strOld.SubString(1,iposi-1);
            
            if(    (strOld == strNew)
                 &&(fabs(fspeednew - fspeedold) < f_DELTASPEED)
                 &&(strComOld == strComNew)
                )
            {
                iTrainTimes = i;
                bhave = true;
                break;
            }
        }

        if(!bhave)       //����б���û�����б�����1
        {
            iTrainTimes = icount;
        }
    }
    return bhave;
}

bool __fastcall TFormMain::GetFileHead(int iIndex)
{
    //TODO: Add your source code here
    //���ڵõ������ļ�ͷ������Ϣ�����������LFHS�ṹ�С�
    //����Ϊ���������� iIndex
    //�����

    int i;
    int iPosi;
    int itemp,itemp2;
//    AnsiString strPassage;
    unsigned char *chtemp;
    unsigned char ch,chH,chL;
    float fTemp1,fTemp2;
    unsigned char chTarget = 0xFF;
    AnsiString strTemp;
//    i = sizeof(lFileHeadStruct);
    if( ( iIndex >= ALL_FILE_NUM )||( iIndex<0 ) )
    {
        Application->MessageBox("�������ݲ�����!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return false;
    }
    iPosi=0;

    for( i = 0; i<ALL_FILE_NUM; i++)
    {
        chtemp = &strData[i*128];
        if( (chtemp[0] == chTarget ) )
        {
            if(iPosi == iIndex)
            {
                break;
            }
            else
            {
                iPosi++;
            }
        }
    }
    LFHS.iHeadLength = 512;                       // 1  �ļ�ͷ�ĳ���
    sprintf(LFHS.str902Mark,"902");             // 2����Ϊ�ļ���������
    lShort mySh;
/*                                                //  ��ȡ������ʱ����
    if(CheckBoxComputerTime->Checked)
    {
        TDateTime Time;
        unsigned short ssiYear,ssiMonth,ssiDate,ssiHour,ssiMinute,ssiSecond,ssiMSecond;
        Time = Now();
        Time.DecodeDate(&ssiYear,&ssiMonth,&ssiDate);
        Time.DecodeTime(&ssiHour,&ssiMinute,&ssiSecond,&ssiMSecond);
        LFHS.siYear   = ssiYear;
        LFHS.siMonth  = ssiMonth;
        LFHS.siDate   = ssiDate;
        LFHS.siHour   = ssiHour;
        LFHS.siMinute = ssiMinute;
        LFHS.siSecond = ssiSecond;
    }
    else
    {
*/
        LFHS.siYear = chtemp[67]*256 + chtemp[66];
        LFHS.siMonth =  chtemp[68];
        LFHS.siDate =   chtemp[70];
        LFHS.siHour =   chtemp[72];
        LFHS.siMinute = chtemp[74];
        LFHS.siSecond = chtemp[76];
//    }
    LFHS.iHeadLength = 512;                     //�ļ�ͷ����
                                                // ����ͨ����ʶ
                                                // �ź�����
    strTimeShort.printf("%04d-%02d-%02d",  LFHS.siYear,
                                               LFHS.siMonth,
                                               LFHS.siDate);
    for(int i=0; i< iPASSNUM; i++)                      //�ź�Դ
    {
        LFHS.siSignalSource[i] = chtemp[(i+1)*2];
    }
//??????????????????????????????????????????????????????????????????????????????????
    int iitemm;
    iitemm = iIndex + 1;
    if(iitemm == 75)
    {
        LFHS.siDieselNum = 6;
    }
    else
    {
        LFHS.siDieselNum       = chtemp[8];     //���ͻ��׺�
    }
//===================================================================================

    LFHS.siTrainType       = chtemp[10];    //��������
    
//��������������������������Щ����
//    LFHS.siTrainType       = 7;             //????????????�����Դ���
//--------------------------------------------------------------------
//    LFHS.siTrainType       = 3;
//====================================================================
    LFHS.iTrainNum         = chtemp[13]*256 + chtemp[12];
    LFHS.siPassOpenNum = 0;
    for(int i=0; i< iPASSNUM; i++)                    //ͨ��״̬
    {
        LFHS.siPassageStatus[i] = chtemp[2*i+32];
        LFHS.siPassOpenNum += LFHS.siPassageStatus[i];
    }
    for(int i=0; i< iPASSNUM; i++)                    //ͨ���ź�����
    {
        LFHS.siSignalType[i] = chtemp[2*i+38];
    }

    for(int i=0; i< iPASSNUM; i++)                    //ͨ������
    {
        LFHS.siSignalGain[i] = chtemp[2*i+44];
    }
    for(int i=0; i< iPASSNUM; i++)                    //ͨ��������
    {
        LFHS.fSensitivity[i] = chtemp[2*i+51]*256
                             + chtemp[2*i+50]
                             + (chtemp[2*i+56]+ chtemp[2*i+57])/100.0;
    }
                                              //ÿͨ�����ݳ���
    LFHS.iLength = (chtemp[127]*256 + chtemp[126])*32768;

    iFileLength = LFHS.iLength;
    LFHS.siFrequency = chtemp[88];
// 2005_02_07�����ӵ�����
//-------------------------------------------------------------
    LFHS.siSetSampleNum   = chtemp[98];
    LFHS.siSampleNum      = chtemp[104];     //���ε�ʵ�ʲ�������
    LFHS.siDataValid      = chtemp[120];     //�Ƿ����������Ч����
    lFloat   myOwn;
    for(int i = 0; i < 4; i++)
    {
        myOwn.ch[i]      = chtemp[108 + i];
    }

    LFHS.fEn              = myOwn.fData;   // ConvertToIEEE(myOwn);            //���ͻ�ת��
    for(int i = 0; i < 4; i++)
    {
        myOwn.ch[i]      = chtemp[116 + i];
    }
    LFHS.fP1RMS           = myOwn.fData;   // ConvertToIEEE(myOwn);         //��һͨ���ľ�����ֵ
    for(int i = 0; i < 4; i++)
    {
        myOwn.ch[i]      = chtemp[112 + i];
    }
    LFHS.fCPMax          = myOwn.fData;   // ConvertToIEEE(myOwn);         //��һͨ����ѹ�����ֵ


    LFHS.iAbstractLength = chtemp[103]*256 + chtemp[102];   //���������Ч������������ݳ���
    LFHS.chVersion       = chtemp[122];                     //�˴�������ǰ汾��

//?????????????????????????????????????????????????????????????????????????????????????
    LFHS.chVersion       = 3;
//?????????????????????????????????????????????????????????????????????????????????????
    if(LFHS.chVersion > 3)
    {
        LFHS.chVersion       = 3;      // �汾��-     1���汾��Ϊ2
    }

    LFHS.fEngineSpeed    = LFHS.fEn;
    LFHS.chCombustion    = chtemp[18];  //�Ƿ���ȼ�չ���   //9
    LFHS.chOil           = chtemp[26];
//========================================================================
    for(int i = 0; i < 4; i++)
    {
        myOwn.ch[i]      = chtemp[14 + i];
    }
    LFHS.fDPDFMax        = myOwn.fData;
    lShort  myShort;

    myShort.ch[0]        = chtemp[20];
    myShort.ch[1]        = chtemp[21];
    LFHS.iCombustionPosi = myShort.iData;

    myShort.ch[0]        = chtemp[22];
    myShort.ch[1]        = chtemp[23];
    LFHS.iOilPosi        = myShort.iData;

    myShort.ch[0]        = chtemp[24];
    myShort.ch[1]        = chtemp[25];
    LFHS.iTDPosi         = myShort.iData;

//    LFHS.iCombustionPosi = chtemp[20];     //(10)
//    LFHS.iOilPosi        = chtemp[22];            //(11)
//    LFHS.iTDPosi         = chtemp[24];              //(12)

//��ȡ����ͨ���ĵ�ͨ�˲�������Ƿ�ͬ����ͬ����ָ��ͨ�˲�Ƶ�������Ƶ��һ��
    LFHS.chTogether[0]   = chtemp[82];     //ͨ�� 1 �Ƿ�ͬ��
    LFHS.chTogether[1]   = chtemp[84];     //ͨ�� 1 �Ƿ�ͬ��
    if(LFHS.chTogether[0] == 0)
    {
        LFHS.siCutoffFreq[0]      = siFrequencyArray[chtemp[78] + 2];     //ͨ�� 1 �ĵ�ͨƵ��
    }
    else
    {
        LFHS.siCutoffFreq[0]      = siFrequencyArray[LFHS.siFrequency];
    }

    if(LFHS.chTogether[1] == 0)
    {
        LFHS.siCutoffFreq[1]      = siFrequencyArray[chtemp[80] + 2];     //ͨ�� 1 �ĵ�ͨƵ��
    }
    else
    {
        LFHS.siCutoffFreq[1]      = siFrequencyArray[LFHS.siFrequency];
    }

    
//    siRFrequency         = ,
//  fTimeInterval,fFrequInterval,siPassOpenNun

//======================================================
//==================================================================================
    SetTrainType(LFHS.siTrainType);
    if(strTrainType != strTrainType_Backup)
    {
        if( !SetDieselInfo(LFHS.siTrainType, true))
        {
            strTemp = "���ݿ��� "  + strTrainType +  "���ͻ�����! �������������" ;
            Application->MessageBox(strTemp.c_str(),"��ʾ",MB_OK|MB_ICONINFORMATION);
            return false;
        }
        strTrainType_Backup = strTrainType;
    }
//------------------------------------------------------------------
    int iposit = strTrainType.Length();
    sprintf(LFHS.strTrainType,"%s",strTrainType.c_str());   //�ַ�����ʽ�Ļ�������      16���ֽ�
    LFHS.strTrainType[iposit] = '\0';
    for(int i=0; i<iPASSNUM-1; i++)
    {
        switch(LFHS.siSignalSource[i])
        {
            case 0:
            {
                strPPSignalType[i] = "ȼ��ѹ��";
                break;
            }
            case 1:
            {
                strPPSignalType[i] = "ѹ��ѹ��";
                break;
            }
            case 2:
            {
                strPPSignalType[i] = "�͹�ѹ��";
                break;
            }
            case 3:
            {
                strPPSignalType[i] = "��ѹѹ��";
                break;
            }
            case 4:
            {
                strPPSignalType[i] = "��������";
                break;
            }
            case 5:
            {
                strPPSignalType[i] = "������ѹ";
                break;
            }
            case 6:
            {
                strPPSignalType[i] = "����";
                break;
            }
        }

//----------------------------------------------------
//        strPPSignalType[1]   = "�͹�ѹ��";
//        LFHS.siSignalSource[1] = 2;
//====================================================

        sprintf(LFHS.strSignalType[i],"%s",strPPSignalType[i].c_str());
        int iil = strPPSignalType[i].Length();
        LFHS.strSignalType[i][iil] = '\0';
    }
    strPPSignalType[2] = "��ѹ�ź�";
    for(int i=0;i<iPASSNUM;i++)
    {
        if(LFHS.siPassageStatus[i] == 0)
        {
            strPPSignalType[i] = "�ر�";
        }
    }
    for(int i=0;i<iPASSNUM;i++)
    {
        LFHS.siGain[i] = (1<<LFHS.siSignalGain[i]);
    }
    LFHS.iRFrequency  = siFrequencyArray[LFHS.siFrequency];
    if(LFHS.iRFrequency != 0)
    {
        LFHS.fTimeInterval = 1000.0/((LFHS.iRFrequency)*2.56);
    }
    LFHS.fAngleStep    = 6.0*(LFHS.fEn)*(LFHS.fTimeInterval)/1000.0;
    strFrequency = IntToStr(siFrequencyArray[LFHS.siFrequency]);

    LFHS.fCombustionAngle = (float)(LFHS.iCombustionPosi - LFHS.iTDPosi)*LFHS.fAngleStep;   // ConvertToIEEE(myOwn);         //��һͨ���ľ�����ֵ
    LFHS.fOilAngle        = (float)(LFHS.iOilPosi - LFHS.iTDPosi)*LFHS.fAngleStep;   // ConvertToIEEE(myOwn);         //��һͨ����ѹ�����ֵ

//    LFHS.fTimeInterval = 1.0/(float)(LFHS.siRFrequency);
//    LFHS.fFreqInterval = ((float)(LFHS.siRFrequency)*1000.0)/(float)(LFHS.iLength);

    strTemp = ComboBoxPersonnel->Text;
    sprintf(LFHS.strPerson,"%s",strTemp.c_str());

//-----------------------------------------------------------------------
    LFHS.fDiameter      = lDS.GetfDiameter();
    LFHS.fPodLength     = lDS.GetfPodLength();
    LFHS.fStroke        = lDS.GetfStroke();
    LFHS.fCRatio        = lDS.GetfCRatio();
    LFHS.fLMDa          = lDS.GetfLMDa();
    LFHS.fOutOpenAngle  = lDS.GetfOutOpenAngle();
    LFHS.fOutCloseAngle = lDS.GetfOutCloseAngle();
    LFHS.fInOpenAngle   = lDS.GetfInOpenAngle();
    LFHS.fInCloseAngle  = lDS.GetfInCloseAngle();
    LFHS.siInfoValid    = 1;
//-------------------------------------------------------------------

//   �������ڻ�þ�����ֵ
    strMTime =  AnsiString(LFHS.siYear) + "-";
    strTemp.printf("%02d-",LFHS.siMonth);
    strMTime += strTemp;
    strTemp.printf("%02d",LFHS.siDate);
    strMTime +=  strTemp;
    sprintf(LFHS.strMTime,"%s",strMTime.c_str());

    strTemp.printf(" %02d-",LFHS.siHour);
    strMTime += strTemp;
    strTemp.printf("%02d-",LFHS.siMinute);
    strMTime += strTemp;
    strTemp.printf("%02d",LFHS.siSecond);
    strMTime += strTemp;
    strTrainNumber.printf("%04d",LFHS.iTrainNum);
    strDieselNumber.printf("%02d",LFHS.siDieselNum);
    strSampleLength = IntToStr(LFHS.iLength) + AnsiString("��");
    iFileLength     = LFHS.iLength;
    return true;
//     LFHS.fCoef[0] =  2000.0/(LFHS.fSensitivity[0]*LFHS.fGain[0])/32768.0;
//     LFHS.fCoef[1] =  2000.0/(LFHS.fSensitivity[1]*LFHS.fGain[1])/32768.0;

}
float __fastcall TFormMain::ConvertToIEEE(lFloat& lf)
{
    //TODO: Add your source code here
    unsigned char chtemp0,chtemp1;
    unsigned char ch0,ch1,ch00,ch11;

    chtemp0 = lf.ch[2];
    ch0 = chtemp0;

    chtemp1 = lf.ch[3];
    ch1     = chtemp1;

    ch11 = ch0&0x80;  //�õ�ָ��λ

    ch00 = ch1&0x01;  //

    ch00 = (chtemp1>>1)|ch11;
    ch11 = (chtemp0 & 0x7f)|(ch00<<7);

    lf.ch[2] = ch11;
    lf.ch[3] = ch00;
    return lf.fData;
}
//-------------------------------------------------------------
void __fastcall TFormMain::BitBtnWriteInstrumentClick(TObject *Sender)
{
    unsigned char *ptDI;
    long lRet;
    int iNum;
    iNum = sizeof(StructDieselInfo);

    int iret = 0;
    if(!bUSBOpened)
    {
        int itrynum = 0;
        do
        {
            if(itrynum == 5)
            {
                break;
            }
            itrynum ++;
            lRet = OpenUsb();
            ::Sleep(400);
        }while(lRet != 1);

        if( lRet == 1 )
        {
            bUSBOpened = true;
        }
        else
        {
            Application->MessageBox("�޷���USB�ӿ�!","��ʾ",MB_OK|MB_ICONINFORMATION);
            bUSBWorking = false;
            bUSBOpened  = false;
            return;
        }
    }
    iNum = sizeof(myDieselInfo)/sizeof(StructDieselInfo);
    FormIndicator->Show();
    FormIndicator->PBIN->Max  = iNum;
    FormIndicator->PBAll->Max = iNum;
    FormIndicator->PBIN->Step  = 0;
    FormIndicator->PBAll->Step = 0;
    FormIndicator->LabelCurrent->Caption = "����״̬��";
    FormIndicator->LabelAll->Caption     = "����״̬��";
    for( int i = 0; i < iNum; i++)
    {
        ptDI = (unsigned char*)(&myDieselInfo[i]);
        iret = WriteUsb(64, ptDI);
        ::Sleep(300);
        FormIndicator->PBIN->Position  = i + 1;
        FormIndicator->PBAll->Position = i + 1;
        Application->ProcessMessages();
    }
    FormIndicator->Hide();
    ::Sleep(400);
    strComPart[0] = 0XBB;
    strComPart[1] = 0XAA;
    WriteUsb(64,strComPart );
    ::Sleep(50);
    if(iret == 64)
    {
        Application->MessageBox("�������ͻ�����д����̽���!","��ʾ",MB_OK|MB_ICONINFORMATION);
    }
}
//---------------------------------------------------------------------------
int __fastcall TFormMain::ReadSingleFile(int iIndex)
{
    //TODO: Add your source code here
        int iret;
        unsigned char *ptch;

//        char chTTT[2048];
        if(!GetFileHead(iIndex))
        {
            return  0;
        }
        iReadTime = (iFileLength / 1024)*LFHS.siPassOpenNum;
        if(strTempData != NULL)
        {
            delete []strTempData;
            strTempData = NULL;
        }
        strTempData = new unsigned char[2048*iReadTime];
        if( strTempData == NULL)
        {
            Application->MessageBox("���ݷ������!","��ʾ",MB_OK|MB_ICONINFORMATION);
            return 0;
        }
        strComPart[0]=0XCC;
        strComPart[1]=0XAA;
        strComPart[2]=iIndex;
        iret = WriteUsb(64,strComPart );
        ::Sleep(50);
        if( iret != 64 )
        {
            Application->MessageBox("��������ʱ���ִ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
            return 0;
        }
        while(true)
        {
            iret = ReadUsb(64,strComPart);
            ::Sleep(50);
            if( iret == 64 )
            {
                if( (strComPart[0] == 0xCC)&&(strComPart[2] == 0xAA) )
                {
                    break;
                }
            }
            else
            {
                Application->MessageBox("�������ж�ȡ����ʱ���ִ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
                if( bUSBOpened )
                {
                    CloseUsb();
                    bUSBOpened  = false;
                    bUSBWorking = false;
                }
                return 0;
            }
        }
        FormIndicator->PBIN->Min = 0;
        FormIndicator->PBIN->Max = iReadTime;
        FormIndicator->PBIN->Step = 1;
        FormIndicator->PBIN->Position = 0;
// TStringGrid
        for(int j=0;j<iReadTime;j++)
        {
            ptch = &strTempData[j*2048];
            iret = ReadUsb( 2048, ptch);
            if(iret == 2048)
            {
                FormIndicator->PBIN->StepIt();
                Application->ProcessMessages();
            }
            else
            {
                delete [] strTempData;
                strTempData = NULL;
                return 0;
            }
        }
/*
        strComPart[0] = 0XBB;
        strComPart[1] = 0XAA;
        iret = WriteUsb(64,strComPart );
        ::Sleep(50);
        if( iret != 64 )
        {
            Application->MessageBox("��������ʱ���ִ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
            return 0;
        }
*/
//        iret = ReadUsb(64,strComPart);

//        if(!bReadAll)
//        {
//            FormIndicator->Hide();
//        }
        bDrawed = false;
        return iFileLength;
}


bool __fastcall TFormMain::SaveSingleFile(int iIndex)
{
    //TODO: Add your source code here
    FILE *fp;
    void * ptrLFHS;
    void * ptrData;
    short * pdata;
    lShort mS;
    int iPNum = 0;
    AnsiString strTemp,strTemp1,strTemp2;
    AnsiString strTempPath;
    ptrLFHS = (void*)&LFHS;
    ptrData = (void*)strTempData;   //ָ��ɼ����ݻ�����
    strTemp2.printf(" %03d.org",iIndex+1);
    strTemp1 = " ";
//    gstrFileName = AnsiString(LFHS.strFileName) + AnsiString(LFHS.strSignalKind) + strMTime + strTemp2;

//���ж�Ԥ���Ŀ¼�Ƿ����
    if(!DirectoryExists(strPresetPath))
    {
        if (!CreateDir(strPresetPath))
        {
            AnsiString sttt;
            sttt = "Ŀ¼��������!!\n\n��������·���Ƿ���ȷ��\n" + strPresetPath;
            sttt += "\n\n �������ȷ�����ڲ˵�|�ļ�|�����ļ�·����ȷ����";
            MessageDlg(sttt, mtConfirmation, TMsgDlgButtons() << mbOK, 0);
            return false;
        }
    }

    strTempPath = strPresetPath + "\\" + strTrainType;
    if( !DirectoryExists(strTempPath) )
    {
        if ( !CreateDir(strTempPath) )
        {
            MessageDlg("Ŀ¼�������� Ԥ��Ŀ¼\\����!", mtConfirmation, TMsgDlgButtons() << mbOK, 0);
            return false;
        }
    }

    strTempPath = strTempPath + "\\" + strTrainNumber;
    if(!DirectoryExists(strTempPath))
    {
        if (!CreateDir(strTempPath))
        {
            MessageDlg("Ŀ¼�������� Ԥ��Ŀ¼\\����\\����!", mtConfirmation, TMsgDlgButtons() << mbOK, 0);
            return false;
        }
    }
/*
    gstrFileName = strFileDirector
                   + strTrainType
                   + strTrainNumber
                   + strDieselNumber
                   + strTemp1
                   + strMTime
                   + strTemp1
                   + strTemp2;
*/
    strPSTFileDirectory =  strPresetPath  + AnsiString("\\")
                         + strTrainType   + AnsiString("\\")
                         + strTrainNumber;
    gstrFileName = strPSTFileDirectory + AnsiString("\\")
                   + strTrainType
                   + strTrainNumber
                   + strDieselNumber
                   + strTemp1
                   + strMTime
                   + strTemp1
                   + strTemp2;

    if((fp = fopen(gstrFileName.c_str(),"wb")) != NULL)
    {
        int iDataWrite = 0;
        iPNum = sizeof(lFileHeadStruct);
        iDataWrite = fwrite(ptrLFHS,sizeof(lFileHeadStruct),1,fp);
        if( iDataWrite != 1)
        {
            fclose(fp);
            Application->MessageBox("д�ļ�ʱ���ִ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
            return false;
        }
        iPNum = 0;
        for(int i=0; i< iPASSNUM; i++)
        {
            if(LFHS.siPassageStatus[i] == 1)
            {
                pdata = (short*)ptrData;
                for( int j = 0; j < LFHS.iAbstractLength + 50; j ++)
                {
                    *(pdata + iPNum*LFHS.iLength + j) = *(pdata + iPNum*LFHS.iLength + j + 64);
                }
                iPNum ++;
            }
        }
        iPNum *= 2*LFHS.iLength;
//        iPNum *= 2;
        iDataWrite = fwrite(ptrData,iPNum,1,fp);
        if( iDataWrite != 1)
        {
            fclose(fp);
            Application->MessageBox("д�ļ�ʱ���ִ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
            return false;
        }
/*
        for(int j=0;j<iPNum;j++)
        {
             mS.ch[0]=strTempData[j*2];
             mS.ch[1]=strTempData[j*2+1];
             fwrite(mS.ch,2,1,fp);
        }
*/
        fclose(fp);
    }
    else
    {
        Application->MessageBox("д�ļ�ʱ���ִ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return false;
    }
    return true;
}



bool __fastcall TFormMain::LIntToFloat(void)
{

    unsigned char *ptch;
    unsigned int   uiTemp;
    lShort mS;
    GetData mGD;
    int ilen;
    float fT[3];
    ptch = &strTempData[0];

    float fT1,fT2,fT3;
    fT[0] = fGuiYiCoef1/(LFHS.fSensitivity[0]*LFHS.siGain[0]);
    fT[1] = fGuiYiCoef2/(LFHS.fSensitivity[1]*LFHS.siGain[1]);
    fT[2] = fGuiYiCoef3/(LFHS.siGain[2]);
    fT10 = fT[0];
    fT20 = fT[1];
    fT30 = fT[2];
    ptch=&strTempData[0];
    for( int i=0; i<3; i++)
    {
        fT[i] /= 32767.0;
    }
//    fT[1] /= 32767.0;
    for(int i=0; i< iPASSNUM; i++)
    {
        if(LFHS.siPassageStatus[i] == 1)
        {
            if( fPassData[i] != NULL)
            {
                delete[] fPassData[i];
                fPassData[i] = NULL;
            }
            fPassData[i] = new float[iFileLength];
            if(fPassData[i] == NULL)
            {
                for(int j=0;j<i;j++)
                {
                    if(fPassData[j] != NULL)
                    {
                        delete[] fPassData[j];
                        fPassData[j] = NULL;
                    }
                }
                Application->MessageBox("�м�����ڴ�������!","��ʾ",MB_OK);
                return false;
            }
        }
    }
    int iNow = 0,iNewPosi,iint;
    for(int i=0; i < iPASSNUM ; i++)
    {
        if(i <iPASSNUM-1)
        {
            if(LFHS.siPassageStatus[i] == 0)
            {
                continue;
            }
            else
            {
                if(LFHS.siSignalType[i] == 0)
                {
                    for(int j=0;j<iFileLength;j++)
                    {
                        iNewPosi = 2*(j + iNow*iFileLength);
                        mS.ch[0] = ptch[ iNewPosi ];
                        mS.ch[1] = ptch[ iNewPosi + 1];
                        fPassData[i][j] = ((float)mS.iData)*fT[i];
                    }
                }
                else
                {
                    for(int j=0;j<iFileLength;j++)
                    {
                        iNewPosi = 2*(j + iNow*iFileLength);
                        mGD.cdata[0] = ptch[ iNewPosi ];
                        mGD.cdata[1] = ptch[ iNewPosi + 1];
                        uiTemp = mGD.idata;
                        if(uiTemp >=32768)
                        {
                            uiTemp = uiTemp - 32768;
                        }
                        else
                        {
                            uiTemp = 32767 + uiTemp;
                        }
                        iint = uiTemp;
                        fPassData[i][j] = ((float)iint)*fT[i];
                    }
                }
                iNow ++;
            }
       }
       else
       {
            if(LFHS.siPassageStatus[i] == 0)
            {
                continue;
            }
            else
            {
                for(int j=0;j<iFileLength;j++)
                {
                    iNewPosi = 2*(j + iNow*iFileLength);
                    mGD.cdata[0] = ptch[ iNewPosi ];
                    mGD.cdata[1] = ptch[ iNewPosi + 1];
                    uiTemp = mGD.idata;
                    if(uiTemp >=32768)
                    {
                        uiTemp = uiTemp - 32768;
                    }
                    else
                    {
                        uiTemp = 32767 + uiTemp;
                    }
                    iint = uiTemp;
                    fPassData[i][j] = ((float)iint)*fT[i];
                }
                iNow ++;
            }

       }
    }
    delete []strTempData;
    strTempData = NULL;
    bITF = true;
    return true;
}

bool __fastcall TFormMain::FillChart(void)
{
    //TODO: Add your source code here
/*
    int ilen,ishowwidth;
    int iint;
    TRect rect;
    ilen = LFHS.iLength;
    ChartP1->Series[0]->Clear();
    ChartP2->Series[0]->Clear();
    ChartP3->Series[0]->Clear();
    ChartP1->Title->Text->Clear();
    ChartP2->Title->Text->Clear();
    ChartP3->Title->Text->Clear();
    ChartP1->LeftAxis->Automatic = false;
    ChartP2->LeftAxis->Automatic = false;
    ChartP3->LeftAxis->Automatic = false;

    if(LFHS.siPassageStatus[0] == 1)
    {
        if(LFHS.siSignalType[0] == 1)
        {
            ChartP1->LeftAxis->Maximum   = 2.0*fT10;
            ChartP1->LeftAxis->Minimum   = 0;
        }
        else
        {
            ChartP1->LeftAxis->Maximum   = fT10;
            ChartP1->LeftAxis->Minimum   = -fT10;
        }
        ChartP1->Title->Text->Add("ͨ��һ");
        ChartP1->Title->Text->Add(AnsiString(LFHS.strSignalType[0]));
        for(int i=0;i<ilen;i++)
        {
            ChartP1->Series[0]->AddXY(i,fPassData[0][i],"",clBlack);
        }
    }
    else
    {
        ChartP1->Title->Text->Add("ͨ��һ");
        ChartP1->Title->Text->Add("�ѱ��ر�");
    }
    if(LFHS.siPassageStatus[1] == 1)
    {
        if(LFHS.siSignalType[1] == 1)
        {
            ChartP2->LeftAxis->Maximum   = 2.0*fT20;
            ChartP2->LeftAxis->Minimum   = 0;
        }
        else
        {
            ChartP2->LeftAxis->Maximum   = fT20;
            ChartP2->LeftAxis->Minimum   = -fT20;
        }

        ChartP2->Title->Text->Add("ͨ����");
        ChartP2->Title->Text->Add(AnsiString(LFHS.strSignalType[1]));
        for(int i=0;i<ilen;i++)
        {
            ChartP2->Series[0]->AddXY(i,fPassData[1][i],"",clBlack);
        }
    }
    else
    {
        ChartP2->Title->Text->Add("ͨ����");
        ChartP2->Title->Text->Add("�ѱ��ر�");
    }
    if(LFHS.siPassageStatus[2] == 1)
    {
        ChartP3->LeftAxis->Maximum   = 2.0*fT30;
        ChartP3->LeftAxis->Minimum   = 0;
        ChartP3->Title->Text->Add("ͨ����");
        ChartP3->Title->Text->Add(AnsiString(LFHS.strSignalType[2]));

        for(int i=0;i<ilen;i++)
        {
            ChartP3->Series[0]->AddXY(i,fPassData[2][i],"",clBlack);
            iint = fPassData[2][i];
        }
    }
    else
    {
        ChartP3->Title->Text->Add("ͨ����");
        ChartP3->Title->Text->Add("�ѱ��ر�");
    }

    ChartP1->Refresh();
    ChartP2->Refresh();
    ChartP3->Refresh();
    Application->ProcessMessages();
*/
    return true;
}


int __fastcall TFormMain::AutoInsertDatabase(void)
{
// ListPath :�б��ļ���¼
// ListBoxPCPFile���б�����ǰ�˳�����е�PCP�ļ���
// iTrainTimes;    �б��ļ�����
// iListNum[800]   �ļ���

    int iCount,iIndex;
    int iret = 0;
    FILE* fp;
    AnsiString strTemp;
    int iposi;
    AnsiString strTempp;

    if(!bParameterInited)  //���û�жԲ��ͻ��������г�ʼ�����ʼ��
    {
        InitParameters();
        bParameterInited = true;
    }

    for( int i = 3 ; i < iTrainTimes; i ++)
    {
        strTemp = ListList->Strings[i];  //�õ��б�����
        ListBoxPCPFile->Items->LoadFromFile(strTemp);
        iCount = ListBoxPCPFile->Items->Count;
        for(int j = 0; j < iCount; j++)  //��һ�������ļ�ͬʱ�������ϴ�
        {
            strPCPName = ListBoxPCPFile->Items->Strings[j];
            LoadCurrentBinPCP(true);

            bAddToDatabaseResult = true;
            AddToDatabase(true);
            bAddToDatabaseResult = false;
        }
    }
    return iret;
}

void __fastcall TFormMain::BitBtnReadListClick(TObject *Sender)
{
    if(!bParameterInited)
    {
        InitParameters();
        bParameterInited = true;
    }
    long lRet;
    unsigned char *ptch;
    bGridFilled = false;
    if(bUSBWorking)
    {
        Application->MessageBox("���ڷ���USB!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    bInstrumentRead = false; //���ڱ�����û�ж�����������
    bUSBWorking = true;
    if(!bUSBOpened)
    {
        int itrynum = 0;
        do
        {
            if(itrynum == 5)
            {
                break;
            }
            itrynum ++;
            lRet = OpenUsb();
            ::Sleep(400);
        }while(lRet != 1);

        if( lRet == 1 )
        {
            bUSBOpened = true;
        }
        else
        {
            Application->MessageBox("�޷���USB�ӿ�!","��ʾ",MB_OK|MB_ICONINFORMATION);
            bUSBWorking = false;
            bUSBOpened  = false;
            return;
        }
    }

    TCursor Save_Cursor = Screen->Cursor ;    // Show hourglass cursor
//    Screen->Cursor      = crGlass;
    try
    {
        Screen->Cursor = crHourGlass;
        ReadFileList();  //��ȡ�ļ��б�
    }
     __finally
    {
        Screen->Cursor = Save_Cursor;
    }
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::ReadFileList(void)
{
    //TODO: Add your source code here
    long lRet;
    unsigned char * ptch = NULL;
//    ::Sleep(400);
    strComPart[0] = 0xAA;
    strComPart[1] = 0x55;
    lRet = WriteUsb( 64, strComPart );
    ::Sleep(50);
    if( lRet != 64 )
    {
        Application->MessageBox("�޷�������д������!","��ʾ",MB_OK|MB_ICONINFORMATION);
        if( bUSBOpened )
        {
            CloseUsb();
            bUSBOpened  = false;
            bUSBWorking = false;
        }
        return false;
    }
    strComPart[0] = 'B';
    strComPart[1] = 'A';
    while(true)
    {
        lRet = ReadUsb(64,strComPart);
        ::Sleep(50);
        if( lRet == 64 )
        {
            if( (strComPart[0] == 0xAA)&&(strComPart[2] == 0x55) )
            {
                break;
            }
        }
        else
        {
            Application->MessageBox("�������ж�ȡ����ʱ���ִ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
            if( bUSBOpened )
            {
                CloseUsb();
                bUSBOpened  = false;
                bUSBWorking = false;
            }
            return false;
        }
    }
//    int inow;
    for(int j=0;j<ALL_FILE_NUM;j++)
    {
        strData[j*64] = 0;
    }
    for(int i=0;i< ALL_FILE_NUM; i++)
    {
        ptch = &strData[i*128];
        lRet = ReadUsb( 128, ptch);
        ::Sleep(50);
//        inow = strData[i*128];
        if( lRet != 128)
        {
            break;
        }
    }
    //���ϲ������ļ�Ŀ¼��������
    strComPart[0] = 0XBB;
    strComPart[1] = 0XAA;
    lRet = WriteUsb(64,strComPart );
    ::Sleep(50);
    iFileNum = 0;
    SGDirectory->RowCount = 2;
    ListPath->Clear();
    ListBoxTrainList->Items->Clear();
    FillGrid();
    bUSBWorking = false;
    return true;
}

void __fastcall TFormMain::ReadCurrentList(void)
{
    //TODO: Add your source code here
    TGridRect choiceRect;
    int ichoiceposi;
    int ilength,ipassage;
    int ireadTime;
    unsigned char *ptch;
    int ireadNum = 0;
    AnsiString strTemp;
    bReadAll = false;

    if(bUSBWorking)
    {
        Application->MessageBox("���ڷ���USB!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    if(!bUSBOpened)
    {
        Application->MessageBox("���ȶ�ȡ�����б�","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }

    strTemp = ComboBoxPersonnel->Text;
    if(strTemp == "")
    {
        Application->MessageBox("�����ü����Ա����!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    bUSBWorking = true;
    choiceRect  = SGDirectory->Selection;

    int istart,iend;
    istart = choiceRect.Top - 1;
    iend   = choiceRect.Bottom;
    if(FormIndicator->Visible == false)
    {
        FormIndicator->PBAll->Min      = 0;
        FormIndicator->PBAll->Max      = iend - istart ;
        FormIndicator->PBAll->Position = 0;
        FormIndicator->PBAll->Step     = 1;
        FormIndicator->Caption         = AnsiString("���ݶ�ȡ����ָʾ");
        FormIndicator->Show();
    }
    for(int i= istart; i< iend; i++)
    {
//        Application->MessageBox("","Before ReadSingleFile",MB_OK);
        if(ReadSingleFile(i))
        {
//            Application->MessageBox("","Before SaveSingleFile",MB_OK);

            if( SaveSingleFile(i))
            {
//                Application->MessageBox("","after SaveSingleFile",MB_OK);
                FormIndicator->PBAll->StepIt();
                ireadNum ++;
            }
            else
            {
                continue;
            }

            if( i == iend - 1)  //ֻ�������һ��ͼ��
            {
                if(FormIndicator->Visible)
                {
                    FormIndicator->Hide();
                }
                if(LIntToFloat())
                {
                    bFileOpened = true;
//                    PageControlMain->ActivePageIndex = 7;
//                    Application->ProcessMessages();
//                    FillChart();
                }
            }
            if(strTempData!=NULL)
            {
                delete [] strTempData;
                strTempData=NULL;
            }
            bUSBWorking=false;
            ListBoxPCPFile->Items->Add(gstrFileName);
            
        }
        else
        {
            break;
        }

//    Application->MessageBox("��ǰ���ݶ�������!","��ʾ",MB_OK|MB_ICONINFORMATION);
//        BitBtnPowerClick(NULL);
    }
//    long lRet = 0;
    strComPart[0] = 0XBB;
    strComPart[1] = 0XAA;
    WriteUsb(64,strComPart );
    ::Sleep(50);
    if(FileExists(gstrFileName))
    {
        strTemp         = strOriginalName;
        strOriginalName = gstrFileName;
        LoadCurrentBinORG();
        strOriginalName = strTemp;
    }
}

void __fastcall TFormMain::ReadAllList(void)
{
    //TODO: Add your source code here
    int  iret;
    int  iPosi, iPosi0 = -1;
    int  iselected = 0;
    int  ireadnum  = 1;
    int  iAllnums = 0, iCurnums = 1;
    AnsiString strTemp;
    iListCount = 0;             //���б���������Ϊ0
    if(bUSBWorking)
    {
        Application->MessageBox("���ڷ���USB!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    if(!bUSBOpened)
    {
        BitBtnReadListClick(NULL);
//        Application->MessageBox("���ȶ�ȡ�����б�","��ʾ",MB_OK|MB_ICONINFORMATION);
//        return;
    }
    ListBoxPCPFile->Items->Clear();
    strTemp = ComboBoxPersonnel->Text;
    if(strTemp == "")
    {
        Application->MessageBox("�����ü����Ա����!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    bReadAll    = true;
    bUSBWorking = true;
    TGridRect choiceRect;
//    PBAll->Min=0;
//    PBAll->Max=iFileNum;
//    PBAll->Position=0;
//    PBAll->Step=1;

    for( int i = 0; i < iMTrainInfoNums; i++)
    {
        if( myMTrainInfo[i].bSelected)
        {
            iselected ++;
            iAllnums += (myMTrainInfo[i].iEndPosi - myMTrainInfo[i].iStartPosi + 1);
        }

    }


    strTemp.printf("��ѡ���� %d ̨����",iselected);
    LabelFileSelected->Caption = strTemp;
    LabelFileIndex->Caption     = "";
    LabelTrainInfo->Caption     = "";
    LabelNums->Caption          = "";
    GroupBoxTrainInfo->Left     = GroupBox28->Left + GroupBox28->Width + 10;
    GroupBoxTrainInfo->Visible = true;

    if(FormIndicator->Visible==false)
    {
        FormIndicator->Show();
    }
    FormIndicator->PBAll->Min = 0;
    FormIndicator->PBAll->Max = iFileNum;
    FormIndicator->PBAll->Position = 0;
    FormIndicator->PBAll->Step = 1;

//    PBAll->Min=0;
//    PBAll->Max=iFileNum;
//    PBAll->Position=0;
//    PBAll->Step=1;

    choiceRect.Left  = 1;
    choiceRect.Right = 1;
    choiceRect.Top   = 1;

    for(int i = 0; i < iFileNum; i ++)
    {
//---------------------------------------------------------------------
// ѡ���Զ�ȡ�ļ�

        if( !PleaseReadIt(i, iPosi) )    //�����趨��Ҫ������ݽ��ж�ȡ
        {
            ListBoxPCPFile->Items->Add("");
            FormIndicator->PBAll->StepIt();
            continue;
        }
        else
        {
            if(iPosi >= 0)
            {
                strTemp.printf("�Ѷ��ļ�:%d,����:%d.", iCurnums, iAllnums );
                LabelNums->Caption          = strTemp;
                iCurnums ++;
                if( iPosi != iPosi0 )
                {
                    strTemp.printf("���ڶ�ȡ�� %d ̨��������!", ireadnum);
                    ireadnum ++;
                    LabelFileIndex->Caption     = strTemp;
                    LabelTrainInfo->Caption     = myMTrainInfo[iPosi].strTrainInfo;
                    iPosi0 = iPosi;
                }
            }
        }

//----------------------------------------------------------------
        choiceRect.Bottom = i+1;
        SGDirectory->Row  = i+1;
        SGDirectory->Selection = choiceRect;
        iret = ReadSingleFile(i);
        if( iret == 0)
        {
            RefreshStatusBar("������ʱ���ִ���!",1);
            FormIndicator->PBAll->StepIt();
            continue;
        }
        else if( iret == 1)
        {

            continue;
        }

        //TStringList
        if(!SaveSingleFile(i))
        {
//            Application->MessageBox("","��ʾ",MB_OK|MB_ICONINFORMATION);
            RefreshStatusBar("��������ʱ���ִ���!!!!",1);
            FormIndicator->PBAll->StepIt();
            continue;
        }
        AnsiString strTemp = "���ڶ��룺";
        strTemp += gstrFileName;
        RefreshStatusBar(gstrFileName,0);
        if(CheckBoxComputerTime->Checked)
        {
            if(  (LFHS.siSignalSource[0] == 0)
               &&(LFHS.siSignalSource[1] == 2)
              )
            {
                if(!lDS.ConverToPCP(gstrFileName))
                {
                    RefreshStatusBar("����ת��ʱ���ִ���!!!!",1);
       //     continue;
                }
            }
        }
        if(!bSaveOrig)
        {
            DeleteFile(gstrFileName);
        }

//        bFileRead = true;
        strTemp = gstrFileName.SubString(1,gstrFileName.Length() - 3);
        strTemp += "PCP";
        ListBoxPCPFile->Items->Add(strTemp);
        FormIndicator->PBAll->StepIt();
    }
    strComPart[0] = 0XBB;
    strComPart[1] = 0XAA;
    WriteUsb(64,strComPart );
    ::Sleep(50);
/*
        if( lRet != 64 )
        {
            Application->MessageBox("��������ʱ���ִ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
            return;
        }
        lRet = ReadUsb(64,strComPart);
*/
    bReadAll    = false;
    bUSBWorking = false;

    FormPSTList_New();  //������ȡ���ݵ��б�

    if(CheckBoxAddToDatabase->Checked)
    {
        if(AutoInsertDatabase() == 0)
        {   //�������������ݿ���
            Application->MessageBox("������������ȫ�����������������ݿ�!","��ʾ",MB_OK|MB_ICONINFORMATION);
        }
        else
        {
            Application->MessageBox("������ȫ������,���������ݿ�ʱ�����˴���!","��ʾ",MB_OK|MB_ICONINFORMATION);
        }
    }
    else
    {
         Application->MessageBox("������������ȫ������!","��ʾ",MB_OK|MB_ICONINFORMATION);
    }

    FormIndicator->PBIN->Position  = 0;
    FormIndicator->PBAll->Position = 0;
    FormIndicator->Hide();

    GroupBoxTrainInfo->Visible = false;

    choiceRect.Bottom = 1;
    SGDirectory->Selection = choiceRect;
    FileListBox2->Update();
    FileListBox2->Refresh();

    ListBoxPSTFile->Items->Clear();
    ListBoxPSTFile->Items->Assign(ListBoxTrainList->Items);
//    DirectoryListBox1->Refresh();
//    FileListBox1->Refresh();
    Application->ProcessMessages();
}

void __fastcall TFormMain::BitBtnReadAllClick(TObject *Sender)
{

    TCursor Save_Cursor = Screen->Cursor ;    // Show hourglass cursor
//    Screen->Cursor      = crGlass;
    try
    {
        Screen->Cursor = crHourGlass;
        bInstrumentRead = true;
        
        ReadAllList();                   //��ȡ��ǰѡ�е��ļ�
        bGridFilled = true;
    }
    __finally
    {
        Screen->Cursor = Save_Cursor ;    // Show hourglass cursor
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmOriginalNegtiveClick(TObject *Sender)
{
    if(!bLoadOriginal)
    {
        return;
    }
    for(int i=0; i< lDS.iOriginalLength; i++)
    {
        lDS.ipOData[0][i] = -lDS.ipOData[0][i];
        lDS.ipOData[1][i] = -lDS.ipOData[1][i];
        lDS.ipOData[2][i] = -lDS.ipOData[2][i];
    }
    PaintBoxOriginal->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmSaveOriginalDataClick(TObject *Sender)
{

    if(!bLoadOriginal)
    {
        //���û�д�ԭʼ�ļ������˳�
        Application->MessageBox("���ȴ�ԭʼ�ļ�!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }

    FILE * fp = NULL;
    AnsiString sstrFileName;
    if( SaveDialog1->Execute() )
    {
        sstrFileName = SaveDialog1->FileName;
        if((fp = fopen(sstrFileName.c_str(),"w")) != NULL)
        {
            for(int i=0; i< lDS.iOriginalLength; i++)
            {
                fprintf(fp,"%d %.4f %d %d %d\n",i,
                    (i*lDS.LFHS.fTimeInterval*6.0*lDS.fEngineSpeed/1000.0),
                                                        lDS.ipOData[0][i],
                                                        lDS.ipOData[1][i],
                                                        lDS.ipOData[2][i]) ;
            }
            fclose(fp);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::mnuPreProAntiFilterSetupClick(TObject *Sender)
{
    if(!bLoadOriginal)
    {
        Application->MessageBox("���ȴ��ļ���Ȼ������˲�����!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
   iPreProMethod = 2;

   AnsiString strTemp;
   FormFilterSetup=new TFormFilterSetup(FormMain);

   float fAnaFre;
   fAnaFre = lDS.LFHS.iRFrequency;
   FormFilterSetup->fmax = fAnaFre;

   strTemp.printf("%.2f",fPl);
   FormFilterSetup->EditFPL->Text = strTemp;

   strTemp.printf("%.2f",fPh);
   FormFilterSetup->EditFPH->Text = strTemp;

   strTemp.printf("%.2f",fSl);
   FormFilterSetup->EditFSL->Text = strTemp;

   strTemp.printf("%.2f",fSh);
   FormFilterSetup->EditFSH->Text = strTemp;

   FormFilterSetup->RadioGroupMethod->ItemIndex     = iDetail;
   FormFilterSetup->ComboBoxFilterKind->ItemIndex   = iFilterKind;
   FormFilterSetup->ComboBoxWindowChoice->ItemIndex = iWindowChoice;
   FormFilterSetup->EditFilterOrder->Text           = IntToStr(iNum);
   FormFilterSetup->CheckBoxSelf->Checked           = bAutolen;
   int Result =  FormFilterSetup->ShowModal();
   if(Result == mrOk)
   {
	    fPl = StrToFloat(FormFilterSetup->EditFPL->Text);
    	fPh = StrToFloat(FormFilterSetup->EditFPH->Text);
	    fSl = StrToFloat(FormFilterSetup->EditFSL->Text);
    	fSh = StrToFloat(FormFilterSetup->EditFSH->Text);
	    iDetail = FormFilterSetup->RadioGroupMethod->ItemIndex;
    	iFilterKind = FormFilterSetup->ComboBoxFilterKind->ItemIndex;
	    iWindowChoice = FormFilterSetup->ComboBoxWindowChoice->ItemIndex;
    	iNum = StrToInt(FormFilterSetup->EditFilterOrder->Text);
	    bAutolen = FormFilterSetup->CheckBoxSelf->Checked;
	    if(bAutolen)
    	{
    		lfir.SetbAutoLength(true,iNum);
	    }
    	else
        {
            lfir.SetbAutoLength(false,iNum);
        }
	   	lfir.SetiWindowChoice(iWindowChoice);
    	lfir.SetiFilterKind(iFilterKind);
	    lfir.SetfSampleRate(fAnaFre*2.56);
    	lfir.SetFrequency(fPl,fPh,fSl,fSh);
	    lfir.GenerateFilter(fFilter, iFilterLen);
        bFilter = true;
   }
   else
   {
   		bFilter = false;
   }

/*
    AnsiString strTemp;
    float fAnaFre;
    fAnaFre = lDS.LFHS.iRFrequency;
    FormFilterSetup = new TFormFilterSetup(FormMain);
    if( FormFilterSetup == NULL )
    {
        Application->MessageBox("��ʾ","��������ʱ���ִ���!",MB_OK|MB_ICONINFORMATION);
        bFilter = false;
    }
    else
    {
        FormFilterSetup->CbBMaxAnalysisFre->ItemIndex = iAnalysisFreIndex;
        FormFilterSetup->ComboBoxFreBand->ItemIndex = iBandChoice;
        FormFilterSetup->fmax = fAnaFre;
        strTemp.printf("%.2f",fpl);
        FormFilterSetup->EditFPL->Text=strTemp;
        strTemp.printf("%.2f",fph);
        FormFilterSetup->EditFPH->Text=strTemp;
        strTemp.printf("%.2f",fsl);
        FormFilterSetup->EditFSL->Text=strTemp;
        strTemp.printf("%.2f",fsh);
        FormFilterSetup->EditFSH->Text=strTemp;
        FormFilterSetup->RadioGroupMethod->ItemIndex=idetail;
        FormFilterSetup->ComboBoxFilterKind->ItemIndex=ifilterkind;
        FormFilterSetup->ComboBoxWindowChoice->ItemIndex=iwindowchoice;
        FormFilterSetup->EditFilterOrder->Text=IntToStr(iNum);
        FormFilterSetup->CheckBoxSelf->Checked=bautolen;
        FormFilterSetup->RGFilterType->ItemIndex = iFilterType ;
        int Result= FormFilterSetup->ShowModal();
        if(Result==mrOk)
        {
            iFilterType = FormFilterSetup->RGFilterType->ItemIndex ;
            iBandChoice = FormFilterSetup->ComboBoxFreBand->ItemIndex;
            if( iFilterType == 0) //ѡ�����FIR�˲���
            {
    	        fpl=StrToFloat(FormFilterSetup->EditFPL->Text);
            	fph=StrToFloat(FormFilterSetup->EditFPH->Text);
	            fsl=StrToFloat(FormFilterSetup->EditFSL->Text);
            	fsh=StrToFloat(FormFilterSetup->EditFSH->Text);
	            idetail=FormFilterSetup->RadioGroupMethod->ItemIndex;
            	ifilterkind=FormFilterSetup->ComboBoxFilterKind->ItemIndex;
	            iwindowchoice=FormFilterSetup->ComboBoxWindowChoice->ItemIndex;
            	iNum=StrToInt(FormFilterSetup->EditFilterOrder->Text);
	            bautolen=FormFilterSetup->CheckBoxSelf->Checked;
                iAnalysisFreIndex = FormFilterSetup->CbBMaxAnalysisFre->ItemIndex;
                fAnaFre = FormFilterSetup->fmax;
	            if(bautolen)
            	{
            		lfir.SetbAutoLength(true,iNum);
	            }
            	else
                {
                	lfir.SetbAutoLength(false,iNum);
                }
        	   	lfir.SetiWindowChoice(iwindowchoice);
            	lfir.SetiFilterKind(ifilterkind);
	            lfir.SetfSampleRate(2.56*fAnaFre);
            	lfir.SetFrequency(fpl,fph,fsl,fsh);
                if(FormFilterSetup->RGPassage->ItemIndex == 0) //��һͨ��
                {
	                lfir.GenerateFilter(fFilter, iFilterLen);
                    bFilter = true;
                }
            }       //ѡ�����IIR�˲���
            else
            {
                FILE *fp;
                if(iBandChoice ==0)
                {
                     iIIRNum = 7;
                }
                else
                {
                    iIIRNum = 13;
                }
                int iIter = 0;
                char chtemp[200];
                AnsiString strr="";
    	        String strFName = ::ParamStr(0);
                strFName = ::ExtractFilePath(strFName) ;
                strr.printf("eiir%d.dat",(int)fAnaFre);
                strFName += strr;
                if((fp = fopen(strFName.c_str(),"r")) == NULL)
                {
                    bFilter = false;
                    Application->MessageBox("�޷����˲������ļ�!","��ʾ",MB_OK);
                    return;
                }
                int iRtime;
                iRtime = iBandChoice;
                while(iRtime>0)
                {
                    fgets(chtemp,200,fp);
                    fgets(chtemp,200,fp);
                    iRtime--;
                }
                for(int i=0;i<iIIRNum;i++)
                {
                    if(i != iIIRNum)
                    {
                        fscanf(fp,"%f",&fIIRB[i]);
                    }
                    else
                    {
                        fscanf(fp,"%f\n",&fIIRB[i]);
                    }
                }
                for(int i=0;i<iIIRNum;i++)
                {
                    if(i != iIIRNum)
                    {
                        fscanf(fp,"%f",&fIIRA[i]);
                    }
                    else
                    {
                        fscanf(fp,"%f\n",&fIIRA[i]);
                    }
                }
                fclose(fp);
            }
        }
//        bFilter=true;
    }
    else
    {
        bFilter = false;
        bBFilter = false;
    }
*/
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ppmAnalysisAccelerateClick(TObject *Sender)
{
    if( bLoadOriginal )
    {
        for(int i=0; i < lDS.iOriginalLength - 1; i++)
        {
            lDS.fOBuffer[i] = lDS.ipOData[2][i];
        }
        PaintBoxOriginal->Refresh();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmSaveAbstractDataClick(TObject *Sender)
{
    if(!bLoadOriginal)
    {
        //���û�д�ԭʼ�ļ������˳�
        Application->MessageBox("���ȴ�ԭʼ�ļ�!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    if( lDS.iSampleNum == 0 )
    {
        //
        Application->MessageBox("û����ȡ����Ч���ݣ������ݿɱ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    FILE * fp = NULL;
    AnsiString sstrFileName;
    if( SaveDialog1->Execute() )
    {
        sstrFileName = SaveDialog1->FileName;
        if((fp = fopen(sstrFileName.c_str(),"w")) != NULL)
        {

            for(int i=0; i< lDS.iNormalLength; i++)
            {
                 fprintf( fp,"%d %.3f %.5f %.2f %.5f %.5f\n",
                          i,
                          (float)i*0.2 - 360.0,
                          lDS.fNewP[i]*10.0,              //�ȼ���ƽ����Ȼ���˲�������
                          lDS.fpOilP[i],
                          lDS.farDPDF[i],            //��һ�����ѹ��������
                          lDS.farDPDF[i]/lDS.fDPDFLocalMax
                         );
            }
/*

            for(int i=0; i< lDS.iAbstractLength; i++)
            {
                 fprintf( fp,"%d %.5f %.5f %d %.5f %.5f\n",
                          i,
                          (float)i*lDS.fAngleStep,
                          lDS.fpFData[i],        //�ȼ���ƽ����Ȼ���˲�������
                          lDS.ipACData[1][i],    //�ڶ�ͨ��������ƽ������
                          lDS.fpDPDF[i],         //��һ�����ѹ��������
                          lDS.fpDPDF[i]/lDS.fDPDFLocalMax
                         );
            }
*/
            fclose(fp);
        }
    }
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::EditDiameterChange(TObject *Sender)
{
    bParameterInited = false;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::RefreshPChart(void)
{
    //TODO: Add your source code here

/*         int i = 0;
    ChartPF->Series[0]->Clear();
    ChartDPDF->Series[0]->Clear();
    ChartPV->Series[0]->Clear();
    ChartLogPLogV->Series[0]->Clear();
    for( i=0; i< lDS.iNormalLength; i++)
    {
        ChartPF->Series[0]->      AddXY(lDS.fFFFai[i], lDS.fNewP[i],"",clBlack);
        ChartDPDF->Series[0]->    AddXY(lDS.fFFFai[i], lDS.farDPDF[i],"",clBlack);
        ChartPV->Series[0]->      AddXY(lDS.farV[i],   lDS.fNewP[i],"",clBlack);
        ChartLogPLogV->Series[0]->AddXY(lDS.farLogV[i],lDS.farLogP[i],"",clBlack);
    }
*/
}

void __fastcall TFormMain::EditShiftKeyPress(TObject *Sender, char &Key)
{
    if((Key <'0')||(Key > '9'))
    {
        Key = 0;
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ToolButton39Click(TObject *Sender)
{
    if((!bFileOpened)&&(!bAdjustHRR)&&(!bLoadOriginal))
    {
       return;
    }
    iCurveShift = StrToInt(EditShift->Text);
    lDS.iTDPosi  += iCurveShift;
    lDS.iTDCStep  = iCurveShift;
    ShiftCurve(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton40Click(TObject *Sender)
{
    if((!bFileOpened)&&(!bAdjustHRR)&&(!bLoadOriginal))
    {
       return;
    }
    iCurveShift = StrToInt(EditShift->Text);
    lDS.iTDPosi -= iCurveShift;
    lDS.iTDCStep = -iCurveShift;
    ShiftCurve(-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton41Click(TObject *Sender)
{
    if((!bFileOpened)&&(!bAdjustHRR)&&(!bLoadOriginal))
    {
       return;
    }
    lDS.iTDCStep = lDS.iTDPosiBackup - lDS.iTDPosi;
    lDS.iTDPosi = lDS.iTDPosiBackup;
    ShiftCurve(-1);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ppmPP1Click(TObject *Sender)
{
    iDrawLineWidth = 1;
    PaintBoxAdjustTDC->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmPP2Click(TObject *Sender)
{
    iDrawLineWidth = 2;
    PaintBoxAdjustTDC->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmPP5Click(TObject *Sender)
{
    iDrawLineWidth = 5;
    PaintBoxAdjustTDC->Repaint();
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::FormPSTList(void)
{
//�˺������������ļ��б�
    AnsiString strStr;
    AnsiString strTT,strTN;
    ppr = new cPPWFDRead();
    if( NULL == ppr )
    {
        Application->MessageBox("�ڴ�������!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return false;
    }
//    FormMain->ListBoxList->Items->Clear();
    strStr = ".PCP";
    ppr->SetstrFileExtend(strStr);
    ppr->SetptListBox(ListBoxPCPFile);
    ppr->SetiAllNum(iListCount);
    for(int m = 0; m < iListCount; m ++)
    {
        strTN  = ExtractFileName(ListStringList->Strings[m]);  //��ȡ����
        strStr = ExtractFilePath(ListStringList->Strings[m]);
        strTT  = strStr.SubString(1,strStr.Length() - 1);
        strTT  = ExtractFileName(strTT);                      //��ȡ����
        strStr = ListStringList->Strings[m] + "\\";
        ppr->SetTrainInfo(strTT,strTN);
        ppr->SetFilePath(strStr.c_str());
        int istart = 0;
        if(m != 0)
        {
            for(int j = 0; j < m; j ++)
            {
                  istart += iMINum[j];
            }
        }
        ppr->SetPosiAndNum(istart , istart + iMINum[m]);
        ppr->GenerateFileList();
    }
//    FormMain->ListBoxList->Items->LoadFromFile(FormMain->ppr->strListBuffer);
    delete ppr;
    return true;
}

void __fastcall TFormMain::FillPSTInfo(void)
{
    if(iListCount == 0)
    {
        ListStringList->Add(strPSTFileDirectory);
        iMINum[iListCount] = 1;
        iListCount ++;
    }
    else
    {
        if(ListStringList->Strings[iListCount-1] == strPSTFileDirectory)
        {
            iMINum[iListCount-1] ++;
        }
        else
        {
            ListStringList->Add(strPSTFileDirectory);
            iMINum[iListCount] = 1;
            iListCount ++;
        }
    }
}

bool __fastcall TFormMain::ConvertOrgToPCP(int i)
{
    //TODO: Add your source code here
//��*.orgԭʼ����ת��Ϊ*.pcp��ȡ���ݸ�ʽ
    return true;
    
}
void __fastcall TFormMain::FilterComboBoxFileChange(TObject *Sender)
{
    iFileExt = FilterComboBoxFile->ItemIndex;
    FileListBoxFile->Mask = FilterComboBoxFile->Mask;
    FileListBoxFile->Update();
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::LoadCurrentBinPCP(bool bdatabase)
{
    AnsiString strTemp;
    bool bret = false;
    FormMain->Caption = strSoftTitle
                       + AnsiString(" - ")
                       + strPCPName;
    if(bdatabase)
    {
        strTemp = "���ڼ��㲢�������ݿ⣺" + strPCPName;
    }
    else
    {
        strTemp = "���ڼ��㣺" + strPCPName;
    }
    RefreshStatusBar(strTemp,0);

    if(lDS.LoadPCPFile(strPCPName.c_str()))   //�������ɹ�
    {
//-------------------------------------------
//�����������������������
// ����һֱ���ڷŴ�״̬
        if( !bCurveModi )
        {
            iChoiceStart = lDS.iPLength/4;
            iChoiceEnd   = lDS.iPLength*3/4;

            iCCStart     = lDS.iStartFai;
            iCCEnd       = lDS.iEndFai;
        }
/*
        else
        {
            iChoiceStart = ;
            iChoiceEnd = ;

            iCCStart = lDS.iStartFai;
            iCCEnd = lDS.iEndFai;
        }
*/
        iCurveShift = 1;

        EditShift->Text = iCurveShift;
        bAdjustHRR = true;
        bFileOpened = true;
        if(!bdatabase)
        {
            ToolButton26->Down = true;
            ToolButton26->Refresh();
            PageControlMain->ActivePageIndex  = 4; //����ͼ�ε���ҳ
            PageControlCurve->ActivePageIndex = 0;
            PaintBoxAdjustTDC->Repaint();          //ʹ��ͼ�ؼ�ˢ��
            RefreshResultControl();  //���¼�����
        }
        FillPcpInfo();
        bret = true;
    }
    return bret;
}

void __fastcall TFormMain::FillPcpInfo(void)
{
    //TODO: Add your source code here
    AnsiString strTemp;
    strTemp.printf("%.1f", lDS.LFHS.fEngineSpeed);
    FormPcpInfo->PanelSpeed->Caption           = strTemp;

    strTemp.printf("%.1f", lDS.LFHS.fOilAngle);
    FormPcpInfo->PanelOilAngle->Caption        = strTemp;
    FormPcpInfo->EditOilAngle->Text        = strTemp;

    strTemp.printf("%.1f", lDS.LFHS.fCombustionAngle);
    FormPcpInfo->PanelCombustionAngle->Caption = strTemp;
    FormPcpInfo->EditCombustionAngle->Text = strTemp;

}

void __fastcall TFormMain::LoadCurrentBinPST(void)
{
    //TODO: Add your source code here
    AnsiString strTemp;
    if( strPSTName == "")
    {
        return;
    }
    FormMain->Caption = strSoftTitle
                       + AnsiString(" - ")
                       + strPSTName;
//--------------------------------------------------------------
// ���´����������û�������
    strTemp        = ExtractFilePath(strPSTName);
    strTemp        = strTemp.SubString(1,strTemp.Length() - 1);
    strTemp        = ExtractFilePath(strTemp);
    strTemp        = strTemp.SubString(1,strTemp.Length() - 1);
    strTrainType   = ExtractFileName(strTemp);
    strTemp        = ExtractFileName(strPSTName);
    int iposi      = strTemp.Pos("_");
    strTrainNumber = strTemp.SubString(iposi - 4, 4);
    iTrainType     = GetiTrainType(strTrainType);
    SetDieselInfo(iTrainType, false);
//=================================================================
    PageControlMain->ActivePageIndex  = 4;
    PageControlCurve->ActivePageIndex = 2;
    bLoadList = true;
    bPSTOpened = true;
    bPSTChanged = true;
//    AllDrawKind = drawPT;
//    ToolButtonModifiedData->Down = true;
//    ToolButtonModifiedData->Refresh();
// 2005��8��25��
//    PaintBoxAll->Refresh();
    DrawImageAll();
    if(AllDrawKind == drawDetect)   //����Ǵ������ݷ�����ʾ����ʱ
    {
        if(bExcel)
        {
            SaveToExcels();
        }
    }
}

void __fastcall TFormMain::LoadCurrentBinORG(void)
{
    //TODO: Add your source code here
    FormMain->Caption = strSoftTitle
                       + AnsiString(" - ")
                       + strOriginalName;
    LoadCurrentOriginal();
    RefreshResultControl();  //���¼�����

    ToolButton23->Down = true;
    ToolButton23->Refresh();
    ScrollBarOriginal->Min = iOriginalStart;
    ScrollBarOriginal->Max = iOriginalEnd_Backup;
    ScrollBarOriginal->Position = iOriginalStart;
    PageControlMain->ActivePageIndex  = 4; //����ͼ�ε���ҳ
    PageControlCurve->ActivePageIndex = 1;
}


void __fastcall TFormMain::RegSaveInfo(void)
{
	//TODO: Add your source code here
    AnsiString strTemp;
    TRegistry *Reg = new TRegistry();
    Reg->RootKey   = HKEY_LOCAL_MACHINE;
    if(!Reg->OpenKey(AnsiString("\\Software\\CARS\\LDCEPAS"),true))
    {
    	delete Reg;
    	return;
    }
//----------------------------------------------------------------
//����ע����Ĵ���
    Reg->WriteInteger("iFileExt",iFileExt);
    Reg->WriteString("strPresetPath",       strPresetPath);
    Reg->WriteString("strPSTFileDirectory", strPSTFileDirectory);
    Reg->WriteInteger("iDrawLineWidth",      iDrawLineWidth);
    Reg->WriteInteger("clInletColor",        clInletColor);
    Reg->WriteInteger("clCompressColor",     clCompressColor);
    Reg->WriteInteger("clCombustionColor",   clCombustionColor);
    Reg->WriteInteger("clOutletColor",       clOutletColor);
    Reg->WriteInteger("clBackground",         clBackground);
    Reg->WriteInteger("clLineColor",       clLineColor);
    Reg->WriteInteger("clTextColor",       clTextColor);
    Reg->WriteString( "strUserTitle",      strUserTitle);
    Reg->WriteString( "strPerson",         strPerson);
    Reg->WriteBool(  "bSaveOrig",          bSaveOrig);
    Reg->WriteBool( "bProcessing" ,        bProcessing);
    Reg->WriteBool( "bExcel",              bExcel);
    Reg->WriteBool( "bAnalysisMethod",     bAnalysisMethod );
//---------------------------------------------------------------
    TDateTime dtTemp = Now();
    if( dtLLastTime < dtTemp)      //���������һ��ʹ������
    {
        dtLLastTime = dtTemp;
    }
    Reg->WriteInteger("iListNumber",       iListNumber);
    Reg->WriteInteger("iBufferSize",       iSSoftNumbers);
    Reg->WriteInteger("clGridLineColor",   iiGridLineColor);
    Reg->WriteInteger("clBorderColor",     clBorderColor);
    Reg->WriteDate(   "iBorderMax",        dtSStartTime);
    Reg->WriteDate(   "iLLBorderMax",      dtLLastTime);
    Reg->WriteBool(   "bAddMInfo",         bAddMInfo);
//===============================================================

    for( int i = 0; i < 16; i++)
    {
        strTemp.printf("mainCurveColor%02d",i);
        Reg->WriteInteger(strTemp, mainCurveColor[i]);
    }
//================================================================
    Reg->CloseKey();
    delete Reg;
    return;
}

bool __fastcall TFormMain::RegSoftwareValid(void)
{
    TRegistry *Reg = new TRegistry();
    AnsiString strTemp;
    Reg->RootKey   = HKEY_LOCAL_MACHINE;

//-------------------------------------------------------------------------------------
//�ж��Ƿ��ǰ�װ�ĳ�������������Ϊע���������
    try
    {
        if(!Reg->OpenKey(AnsiString("\\Software\\CARS\\JL-902�������ͻ�ȼ��ϵͳ������΢������1.1"),false))
        {   // "LDCEPAS":Locomotive Diesel Engine Cylinder Pressure Analysis Software
            bRegHave = false;
        }
        else
        {
            bRegHave = true;
        }
    }
    catch(...)
    {
     	Reg->CloseKey();
        delete Reg;
        return false;
    }
//=======================================================================================
  	Reg->CloseKey();
    delete Reg;
    return bRegHave;
}

bool __fastcall TFormMain::RegReadInfo(void)
{
	//TODO: Add your source code here
    bool bret = false;
    TRegistry *Reg = new TRegistry();
    AnsiString strTemp;
    Reg->RootKey   = HKEY_LOCAL_MACHINE;
    bret = Reg->OpenKey(AnsiString("\\Software\\CARS\\LDCEPAS"), false);
    if(bret)
    {
      	try
        {
//-----------------------------------------------------------
//����ע����Ĵ����
            iFileExt            = Reg->ReadInteger("iFileExt");
            strPresetPath       = Reg->ReadString("strPresetPath");
            strPSTFileDirectory = Reg->ReadString("strPSTFileDirectory");
            iDrawLineWidth      = Reg->ReadInteger("iDrawLineWidth");
            clInletColor        = (TColor)Reg->ReadInteger("clInletColor");
            clCompressColor     = (TColor)Reg->ReadInteger("clCompressColor");
            clCombustionColor   = (TColor)Reg->ReadInteger("clCombustionColor");
            clOutletColor       = (TColor)Reg->ReadInteger("clOutletColor");
            clBackground        = (TColor)Reg->ReadInteger("clBackground");
            clLineColor         = (TColor)Reg->ReadInteger("clLineColor");
            clTextColor         = (TColor)Reg->ReadInteger("clTextColor");
            strUserTitle        = Reg->ReadString("strUserTitle");
            strPerson           = Reg->ReadString("strPerson");
            bSaveOrig           = Reg->ReadBool(  "bSaveOrig");
            bAddMInfo           = Reg->ReadBool( "bAddMInfo");
            bProcessing         = Reg->ReadBool( "bProcessing" );
            bExcel              = Reg->ReadBool( "bExcel");
            bAnalysisMethod     = Reg->ReadBool( "bAnalysisMethod" );
//--------------------------------------------------------------
//�˶����ڼ���
            iSSoftNumbers       = Reg->ReadInteger("iBufferSize");
            iListNumber         = Reg->ReadInteger("iListNumber");
            //iBorderMax �˱������������װ����
            dtSStartTime        = Reg->ReadDate("iBorderMax");
            dtLLastTime         = Reg->ReadDate("iLLBorderMax");
            clBorderColor       = Reg->ReadInteger("clBorderColor");
//==============================================================
            
            for( int i = 0; i < 16; i++)
            {
                strTemp.printf("mainCurveColor%02d",i);
                mainCurveColor[i] = (TColor)Reg->ReadInteger(strTemp);
            }
//===========================================================
        }
        catch(...)
        {
         	Reg->CloseKey();
            delete Reg;
            return false;
        }
      	Reg->CloseKey();
    }
    else
    {
        bFirstTime = true;    //��װ�����Ժ�ĵ�һʹ������
    }
    delete Reg;
    return bret;
}

void __fastcall TFormMain::mnuSetupColorClick(TObject *Sender)
{
    FormSetupColor = new TFormSetupColor(FormMain);
    if(FormSetupColor != NULL)
    {
        FormSetupColor->PanelCC00->Color         = mainCurveColor[0];
        FormSetupColor->PanelCC01->Color         = mainCurveColor[1];
        FormSetupColor->PanelCC02->Color         = mainCurveColor[2];
        FormSetupColor->PanelCC03->Color         = mainCurveColor[3];
        FormSetupColor->PanelCC04->Color         = mainCurveColor[4];
        FormSetupColor->PanelCC05->Color         = mainCurveColor[5];
        FormSetupColor->PanelCC06->Color         = mainCurveColor[6];
        FormSetupColor->PanelCC07->Color         = mainCurveColor[7];
        FormSetupColor->PanelCC08->Color         = mainCurveColor[8];
        FormSetupColor->PanelCC09->Color         = mainCurveColor[9];
        FormSetupColor->PanelCC10->Color         = mainCurveColor[10];
        FormSetupColor->PanelCC11->Color         = mainCurveColor[11];
        FormSetupColor->PanelCC12->Color         = mainCurveColor[12];
        FormSetupColor->PanelCC13->Color         = mainCurveColor[13];
        FormSetupColor->PanelCC14->Color         = mainCurveColor[14];
        FormSetupColor->PanelCC15->Color         = mainCurveColor[15];

        FormSetupColor->Panel1->Color           = clInletColor;
        FormSetupColor->Panel2->Color           = clCompressColor;
        FormSetupColor->Panel3->Color           = clCombustionColor;
        FormSetupColor->Panel4->Color           = clOutletColor;
        FormSetupColor->PanelBackground->Color  = clBackground;
        FormSetupColor->clSetupColor[0]         = clLineColor;
        FormSetupColor->clSetupColor[1]         = clTextColor;
        FormSetupColor->clSetupColor[2]         = clBackground;
        if(FormSetupColor->ShowModal() == mrOk)
        {
            clInletColor      = FormSetupColor->Panel1->Color;
            clCompressColor   = FormSetupColor->Panel2->Color;
            clCombustionColor = FormSetupColor->Panel3->Color;
            clOutletColor     = FormSetupColor->Panel4->Color;
            clBackground      = FormSetupColor->PanelBackground->Color;
            clLineColor       = FormSetupColor->clSetupColor[0];
            clTextColor       = FormSetupColor->clSetupColor[1];
            clBackground      = FormSetupColor->clSetupColor[2];

            mainCurveColor[0]         = FormSetupColor->PanelCC00->Color;
            mainCurveColor[1]         = FormSetupColor->PanelCC01->Color;
            mainCurveColor[2]         = FormSetupColor->PanelCC02->Color;
            mainCurveColor[3]         = FormSetupColor->PanelCC03->Color;
            mainCurveColor[4]         = FormSetupColor->PanelCC04->Color;
            mainCurveColor[5]         = FormSetupColor->PanelCC05->Color;
            mainCurveColor[6]         = FormSetupColor->PanelCC06->Color;
            mainCurveColor[7]         = FormSetupColor->PanelCC07->Color;
            mainCurveColor[8]         = FormSetupColor->PanelCC08->Color;
            mainCurveColor[9]         = FormSetupColor->PanelCC09->Color;
            mainCurveColor[10]        = FormSetupColor->PanelCC10->Color;
            mainCurveColor[11]        = FormSetupColor->PanelCC11->Color;
            mainCurveColor[12]        = FormSetupColor->PanelCC12->Color;
            mainCurveColor[13]        = FormSetupColor->PanelCC13->Color;
            mainCurveColor[14]        = FormSetupColor->PanelCC14->Color;
            mainCurveColor[15]        = FormSetupColor->PanelCC15->Color;

            PanelAll->Color   = clBackground;
            PanelSingle->Color = clBackground;

            if(PageControlMain->ActivePageIndex == 4)
            {
                if(PageControlCurve->ActivePageIndex == 0)
                {
                    PaintBoxAdjustTDC->Refresh();
                }
                else if(PageControlCurve->ActivePageIndex == 2)
                {
                    DrawImageAll();
                }
            }
        }
        delete FormSetupColor;
        FormSetupColor = NULL;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmDPI1Click(TObject *Sender)
{
    iDrawLineWidth = 1;
    DrawImageAll();
//    PaintBoxAll->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppnDPI2Click(TObject *Sender)
{
    iDrawLineWidth = 2;
    DrawImageAll();
//    PaintBoxAll->Repaint();
//    PaintBoxAll->Canvas->CopyRect();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmDPI5Click(TObject *Sender)
{
    iDrawLineWidth = 5;
    DrawImageAll();    
//    PaintBoxAll->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmSaveasTextFileClick(TObject *Sender)
{
    if((!bFileOpened)&&(!bAdjustHRR)&&(!bLoadOriginal))
    {
       return;
    }
    AnsiString strTemp;
    strTemp = strPCPName.SubString(1,strPCPName.Length() - 3);
    strTemp += AnsiString("TXT");
    FILE * fp = NULL;
    if((fp = fopen(strTemp.c_str(),"w"))!= NULL)
    {
        for(int i = 0; i< lDS.iNormalLength; i++)
        {
            fprintf(fp,"%.2f %.2f %.4f\n",lDS.fFFFai[i],lDS.fNewP[i],lDS.farDPDF[i]);
        }
        fclose(fp);
        Application->MessageBox("�ѱ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
    }

}
//---------------------------------------------------------------------------


void __fastcall TFormMain::RefreshResultControl(void)
{
    //TODO: Add your source code here

//������ͻ�ת��
    AnsiString strTemp;
    strTemp.printf("%.1f",lDS.fEngineSpeed);
    EditEngineSpeed->Text           = strTemp;
    FormResult->PanelSpeed->Caption = strTemp;

    //��������ѹ�����ֵ
//    strTemp.printf("%.2f",lDS.fNormalPMax);
    strTemp.printf("%.2f",   lDS.fCPMax);
    FormResult->PanelPMax->Caption     = strTemp;
    EditCPMax->Text                    = strTemp;

    strTemp.printf("%.2f",lDS.fNormalPMaxPosi);
    FormResult->PanelPMaxPosi->Caption     = strTemp;

    //��������ѹ��ѹ����
    strTemp.printf("%.3f",    lDS.fDPDFMax);
    FormResult->PanelDPMax->Caption     = strTemp;
    EditDPDFMax->Text                   = strTemp;
    //�����������ѹ�������ʶ�Ӧ�ĽǶ�
    strTemp.printf("%.2f", lDS.fDPDFMaxPosi);  //(lDS.fDPDFMaxPosi - lDS.iTDPosi)*lDS.fAngleStep);
    FormResult->PanelDPMaxPosi->Caption    = strTemp;

    //���������
    if(lDS.bCombustion)
    {
        //������ȼ��
        strTemp.printf("%.1f",lDS.fCombustionAngle - lDS.fOilAngle);
        FormResult->PanelDelayAngle->Caption = strTemp;

        //������������ֵ
        strTemp.printf("%.1f",lDS.fHRRMax);
        FormResult->PanelHRMax->Caption = strTemp;

        //������������ֵ��Ӧ�ĽǶ�
        strTemp.printf("%.1f",lDS.fDHDFMaxFai );
        FormResult->PanelHRMAngle->Caption = strTemp;

        //����ȼ��ʼ��
        strTemp.printf("%.1f",lDS.fCombustionAngle);
        EditCombustionAngle->Text          = strTemp;
        FormResult->PanelCStartA->Caption  = strTemp;

        strTemp.printf("%.1f",lDS.fCombustionPeriod);
        EditCombustionPeriod->Text         = strTemp;
        FormResult->PanelCPeriod->Caption  = strTemp;

        strTemp.printf("%.1f",lDS.fCombustionPeriod + lDS.fCombustionAngle);
        FormResult->PanelCEndA->Caption    = strTemp;

//        strTemp.printf("%.2f",lDS.LFHS.fCompressFactor);
//        EditCompressFactor->Text           = strTemp;
    }
    else
    {
        //����ȼ��ʼ��
        EditCombustionAngle->Text          = "��ȼ��";
        FormResult->PanelCStartA->Caption  = "��ȼ��";
        EditCombustionPeriod->Text         = "��ȼ��";
        FormResult->PanelCPeriod->Caption  = "��ȼ��";
        EditPipeLast->Text                 = "��ȼ��";

        //������ȼ��
        FormResult->PanelDelayAngle->Caption = "��ȼ��";

        //������������ֵ
        FormResult->PanelHRMax->Caption      = "��ȼ��";

        //������������ֵ��Ӧ�ĽǶ�
        FormResult->PanelHRMAngle->Caption   = "��ȼ��";

        //������ȼ��
        FormResult->PanelCEndA->Caption      = "��ȼ��";


    }
    strTemp.printf("%.2f",lDS.LFHS.fCompressFactor);
    EditCompressFactor->Text           = strTemp;
    if(lDS.bOilSupply)
    {
            //���빩��ʼ��
        strTemp.printf("%.1f",lDS.LFHS.fOilAngle);
        EditOilAngle->Text                 = strTemp;
        FormResult->PanelOilAngle->Caption = strTemp;


        //���빩�ͳ����Ƕ�
        strTemp.printf("%.1f",lDS.LFHS.fWavePeriod);
        EditPipeLast->Text                 = strTemp;
        FormResult->PanelPipeLast->Caption = strTemp;

        //���빩�Ͳ���ϵ��
        strTemp.printf("%.3f",lDS.LFHS.fWaveCoef);
        EditWaveCoef->Text                 = strTemp;
        FormResult->PanelWaveCoef->Caption = strTemp;
    }
    else
    {
        //���빩��ʼ��
        EditOilAngle->Text                 = "�޹���";
        FormResult->PanelOilAngle->Caption = "�޹���";

        //���빩�ͳ����Ƕ�
        EditPipeLast->Text                 = "�޹���";
        FormResult->PanelPipeLast->Caption = "�޹���";

        //���빩�Ͳ���ϵ��
        EditWaveCoef->Text                 = "�޹���";
        FormResult->PanelWaveCoef->Caption = "�޹���";
    }

    SetTrainType(lDS.LFHS.siTrainType);
    EditTrainType->Text    = strTrainType;

    strTemp.printf("%04d",lDS.LFHS.iTrainNum);
    strTrainNumber         = strTemp;
    EditTrainNumber->Text  = strTemp;

    strTemp.printf("��%02d��",lDS.LFHS.siDieselNum );
    strDieselNumber        = strTemp;
    EditDieselNumber->Text = strTemp;

    if(ppmShowResult->Checked)
    {
        FormResult->Show();
    }
}

void __fastcall TFormMain::ppmPopupResultClick(TObject *Sender)
{
    FormResult->Show();    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmShowResultClick(TObject *Sender)
{
    ppmShowResult->Checked = !ppmShowResult->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButtonOilPClick(TObject *Sender)
{
//    Application->ProcessMessages();
    if(AllDrawed(Sender,drawPWave))     //�����͹�ѹ��
    {
        return;
    }
    TCursor Save_Cursor = Screen->Cursor ;
    try
    {
        Screen->Cursor = crHourGlass;
        if(!DetectPSTStatus())
        {
            return;
        }
        if(!InitParameters())
        {
            return;
        }
        bFileChanged = true;
//        PaintBoxAll->Refresh();
        DrawImageAll();
        ImageAll->PopupMenu = PopupMenu10;
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::CheckBoxDrawPipeWaveClick(TObject *Sender)
{
    bDrawPWave = CheckBoxDrawPipeWave->Checked;
    bDrawCWave = CheckBoxDrawCylinderWave->Checked;
    if(PageControlMain->ActivePageIndex == 4)
    {
        if(PageControlCurve->ActivePageIndex == 0)
        {
            if( CheckBoxDrawPipeWave->Checked )
            {
                PaintBoxAdjustTDC->PopupMenu = PopupMenuInsert;
            }
            PaintBoxAdjustTDC->Refresh();
        }
        else if(PageControlCurve->ActivePageIndex == 2)
        {
//            PaintBoxAll->Refresh();
            DrawImageAll();
        }
    }
    bDrawCWave = bDrawCWave;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmDeleteLineClick(TObject *Sender)
{
    ppmDeleteLine->Checked = !ppmDeleteLine->Checked;
    bCursorLine = ppmDeleteLine->Checked;
    bCursorLineChanged = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmSetCursorColorClick(TObject *Sender)
{
    if(ColorDialog1->Execute())
    {
        clGridLineColor = ColorDialog1->Color;
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ppmClearAllListClick(TObject *Sender)
{
    ListBoxPSTFile->Items->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmClearCurrentListClick(TObject *Sender)
{
    int iIndex,iCount;
    iIndex = ListBoxPSTFile->ItemIndex;
    iCount = ListBoxPSTFile->Items->Count;
    if((iIndex >= 0)&&(iIndex < iCount))
    {
        ListBoxPSTFile->Items->Delete(iIndex);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmChangeParametersClick(TObject *Sender)
{
    int iItem;
    AnsiString strFilename;
    int iCount = FileListBoxFile->Items->Count;
    if( iCount > 0)
    {
        for( iItem = 0; iItem < iCount; iItem ++)
        {
            FileListBoxFile->ItemIndex = iItem;
            strFilename = FileListBoxFile->FileName;
            lDS.ChangeSensitivity(strFilename.c_str());
        }
        Application->MessageBox("������ϣ�","��ʾ",MB_OK|MB_ICONINFORMATION);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::RadioGroupSingleCurveClick(TObject *Sender)
{
    int iItem;
    iItem = RadioGroupSingleCurve->ItemIndex;
    switch(iItem)
    {
        case 0:
        {
            ppmAdjustPTClick(NULL);
            break;
        }
        case 1:
        {
            ppmAdjustCPTClick(NULL);
            break;
        }
        case 2:
        {
            ppmAdjustPVClick(NULL);
            break;
        }
        case 3:
        {
            ppmAdjustLogPVClick(NULL);
            break;
        }
        case 4:
        {
            ppmAdjustDPDFClick(NULL);
            break;
        }
        case 5:
        {

            ppmAdjustHRRClick(NULL);
            break;
        }
        case 6:
        {
            ppmAdjustCHRRClick(NULL);
            break;
        }
        case 7:
        {
            ppmAdjustFFTPClick(NULL);
            break;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CheckBoxFirstClick(TObject *Sender)
{
    if(!bLoadOriginal)
    {
        return;
    }
    for(int i=0; i< lDS.iOriginalLength; i++)
    {
        lDS.ipOData[0][i] = -lDS.ipOData[0][i];
    }
    PaintBoxOriginal->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CheckBoxSecondClick(TObject *Sender)
{
    if(!bLoadOriginal)
    {
        return;
    }
    for(int i=0; i< lDS.iOriginalLength; i++)
    {
        lDS.ipOData[1][i] = -lDS.ipOData[1][i];
    }
    PaintBoxOriginal->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CheckBoxHideThreeClick(TObject *Sender)
{
    if(!bLoadOriginal)
    {
        return;
    }
    PaintBoxOriginal->Refresh();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


void __fastcall TFormMain::ppmFormPCPClick(TObject *Sender)
{
    int iCount;
    AnsiString strTemp;
    iCount = FileListBoxFile->Items->Count;
    strTemp = gstrFileName;

    if(iCount > 0)
    {
        if(!InitParameters())
        {
            return;
        }
        for(int i = 0; i < iCount; i ++)
        {
            FileListBoxFile->ItemIndex = i;
            gstrFileName = FileListBoxFile->FileName;
            if(!lDS.ConverToPCP(gstrFileName))
            {
                Application->MessageBox("����ת��ʱ���ִ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
                continue;
            }
            StatusBarMain->Panels->Items[0]->Text = AnsiString("����ת���ļ��� ") + gstrFileName;
            StatusBarMain->Refresh();
        }
    }
    gstrFileName = strTemp;
    Application->MessageBox("����ת������!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
    StatusBarMain->Panels->Items[0]->Text = "";
    StatusBarMain->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ppmSinglePCPClick(TObject *Sender)
{
    int iIndex;
    bool  bConvert = false;
    AnsiString strTemp;
    iIndex = FileListBoxFile->ItemIndex;
    strTemp = gstrFileName;
    if(iIndex >= 0)
    {
        if(!InitParameters())
        {
            return;
        }
        gstrFileName = FileListBoxFile->FileName;
        strTemp = ExtractFileExt(gstrFileName);
        strTemp = strTemp.UpperCase();
        if(strTemp == ".ORG")
        {
            if(!lDS.ConverToPCP(gstrFileName))
            {
                 Application->MessageBox("����ת��ʱ���ִ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
            }
            else
            {
                bConvert = true;
            }
        }
        else
        {
            Application->MessageBox("����ȷѡ����Ҫ����ת��������!","��ʾ",MB_OK|MB_ICONINFORMATION);
        }
        strTemp = gstrFileName;
    }
    gstrFileName = strTemp;
    if(bConvert)
    {
        Application->MessageBox("����ת������!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
    }
    StatusBarMain->Panels->Items[0]->Text = "����ת������!!!";
    StatusBarMain->Refresh();
}
/*
void __fastcall TFormMain::CheckBox3DClick(TObject *Sender)
{

    if(CheckBox3D->Checked)
    {
        ToolButtonOriginalData->Visible = false;
        ToolButtonBasicInfo->Visible    = false;
        ToolButtonMMax->Visible         = false;
        ToolButtonPV->Visible           = false;
        ToolButtonLogPLogV->Visible     = false;
        ToolButtonDomain->Visible       = false;
//        ToolButtonTT->Visible        = false;
//        ToolButtonPower->Visible     = false;
//        ToolButtonCPower->Visible    = false;
        ToolBar1->Refresh();

//        PaintBoxAll->Visible    = true;
        ImageAll->Visible       = true;
        fstartrt     = 3.0/8.0;
        fendrt       = 5.0/8.0;

    }
    else
    {
        ToolButtonOriginalData->Visible = true;
        ToolButtonBasicInfo->Visible = true;
        ToolButtonMMax->Visible      = true;
        ToolButtonPV->Visible        = true;
        ToolButtonLogPLogV->Visible         = true;
        ToolButtonDomain->Visible    = true;
//        ToolButtonTT->Visible        = true;
//        ToolButtonPower->Visible     = true;
//        ToolButtonCPower->Visible    = true;
        ToolBar1->Refresh();

//        PaintBoxAll->Visible    = true;
        ImageAll->Visible       = true;
    }
//    Application->ProcessMessages();
//    PaintBoxAll->Refresh();
    DrawImageAll();

}
//---------------------------------------------------------------------------
*/



void __fastcall TFormMain::ImageAllDblClick(TObject *Sender)
{
    fXAngle     = XANGLE;
    fYAngle     = YANGLE;
    DrawImageAll();
//    fX0Position =
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ppmShowCutClick(TObject *Sender)
{
    bShowCut = !bShowCut;
    ppmShowCut->Checked = bShowCut;
    DrawImageAll();
}
//---------------------------------------------------------------------------







void __fastcall TFormMain::ppmSetColorClick(TObject *Sender)
{
    if(ColorDialog1->Execute())
    {
    }    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CheckBoxValueClick(TObject *Sender)
{
    GroupBoxCalibration->Visible = CheckBoxValue->Checked;
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------


void __fastcall TFormMain::SetTrainType(short int itype)
{
    //TODO: Add your source code here
//-------------------------------------------
//    itype = 4;
//============================================
    switch(itype)
    {
        case 0:
        {
            strTrainType = "����4A";
            break;
        }
        case 1:
        {
            strTrainType = "����4B";
            break;
        }
        case 2:
        {
            strTrainType = "����4C";
            break;
        }
        case 3:
        {
            strTrainType = "����4D";
            break;
        }
        case 4:
        {
            strTrainType = "����8B";
            break;
        }
        case 5:
        {
            strTrainType = "����11";
            break;
        }
        case 6:
        {
            strTrainType = "����5";
            break;
        }
        case 7:
        {
            strTrainType = "����7";
            break;
        }
        case 8:
        {
            strTrainType = "����7C";
            break;
        }
        case 9:
        {
            strTrainType = "����11G";
            break;
        }
        case 10:
        {
            strTrainType = "����A";
            break;
        }
        default:
        {
            strTrainType = "����";
            break;
        }
    }
}



void __fastcall TFormMain::mnuOperationDatabaseClick(TObject *Sender)
{
    PageControlMain->ActivePageIndex = 5;
    UpdateTreeViewDatabase();  
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::DatabaseOpen(void)
{
    //TODO: Add your source code here
    String strSource,strTarget,strDatabase;

    strSource = "Database\\DieselInfo.mdb";
    strSource = strExeFilePath + strSource;
    bool bDbExists = FileExists(strSource);
    if(bDbExists)
    {
        char strTemppp[1024];
        int iFileSize=0,iNowNum;
        int iFileHandle = FileOpen(strSource,fmOpenRead);
        do
        {
            iNowNum = FileRead(iFileHandle,strTemppp,1024);
            if(iNowNum >= 1024)
            iFileSize++;
        }while(iNowNum >= 1024);
        iFileSize /= 1024;  //��ȡ��ǰ�����ļ��Ĵ�С
        if(iFileSize > 400)
        {
            Application->MessageBox("���ݿ�̫����!\n\n������ɾ�����ݿ����ѹ�ʱ������","��ʾ",MB_OK|MB_ICONINFORMATION);
        }
        FileClose(iFileHandle);
    }

//�˶δ����������ñ������ݿ�
    strDatabase = "Provider = Microsoft.Jet.OLEDB.4.0; Data Source = ";
    //��̬�������ݿ�
    strSource   = strExeFilePath + AnsiString("Database\\DieselInfo.mdb; Persist Security Info = False");
    strDatabase = strDatabase    + strSource;

    if(DM->ADOConnectionDataInfo->Connected)
    {
        DM->ADOConnectionDataInfo->Close();
    }
    DM->ADOConnectionDataInfo->ConnectionString = strDatabase;

//  ����������
    if(DM->ADOConnectionDataInfo->Connected == false)
    {
        DM->ADOConnectionDataInfo->Open();
    }

// �����ݱ�
/*  //�ȹر����ݱ�
    if(ADODataSetDataInfo->Active == false)
    {
        ADODataSetDataInfo->Open();
    }
    DataSourceDataInfo->Enabled = true;
*/
}

void __fastcall TFormMain::DatabaseClose(void)
{
    //TODO: Add your source code here
// �ر����ݱ�
    if(DM->DataSourceDataInfo->Enabled)
    {
        DM->DataSourceDataInfo->Enabled = false;
    }
// �ر����ݱ�
    if(DM->ADODataSetDataInfo->Active)
    {
        DM->ADODataSetDataInfo->Close();
    }
//  �ر���������
    if(DM->ADOConnectionDataInfo->Connected)
    {
        DM->ADOConnectionDataInfo->Close();
    }
}

void __fastcall TFormMain::UpdateTreeViewDatabase(void)
{
    //TODO: Add your source code here
}

bool __fastcall TFormMain::AddToDatabase(bool bFromFile)
{
    //TODO: Add your source code here
    bool bret = true;
    if(bAddToDatabaseResult)
    {
        if(!DM->ADODataSetDataInfo->Active)
        {
            DM->ADODataSetDataInfo->Open();
        }
        bool bcanchange = DM->ADODataSetDataInfo->CanModify;
        if(bcanchange)
        {
            try
            {
                DM->ADODataSetDataInfo->Append();
            }
            catch(...)
            {
                throw("Insert �Ƿ�����");
            }
            InsertDatabaseResult(bFromFile);
            DM->ADODataSetDataInfo->Post();
        }
        else
        {
            bret = false;
      	    Application->MessageBox("�޷������ݴ������ݿ�","����",MB_OK);
        }
    }
    return bret;
}

bool __fastcall TFormMain::InsertDatabaseResult(bool bFromFile)
{
    AnsiString strTemp;

    if(bFromFile)  //�����������ж�ȡ��Ϣ
    {
        //1  ��������
        SetTrainType(lDS.LFHS.siTrainType);
        DM->ADODataSetDataInfo->FieldByName("TrainType")->AsString     = strTrainType;

        //2  ��������
        strTemp.printf("%04d",lDS.LFHS.iTrainNum);
        DM->ADODataSetDataInfo->FieldByName("TrainNumber")->AsString   = strTemp;

        //3  ���ͻ��׺�
        strTemp.printf("%d",lDS.LFHS.siDieselNum );
        DM->ADODataSetDataInfo->FieldByName("DieselNumber")->AsString  = strTemp;

        //4  �����Ա
        strTemp = lDS.LFHS.strPerson;
        DM->ADODataSetDataInfo->FieldByName("MPerson")->AsString       =  strTemp;

        // 5 ��������
        strTemp.printf("%04d-%d-%d", lDS.LFHS.siYear,
                                     lDS.LFHS.siMonth,
                                     lDS.LFHS.siDate
                                       );
        DM->ADODataSetDataInfo->FieldByName("MDate")->AsString          = strTemp;

        //6 �������
        strTemp = "����";
        DM->ADODataSetDataInfo->FieldByName("AnalysisResult")->AsString = strTemp;

        //7 ����״̬
        if(lDS.LFHS.chOil == 1)
        {
            strTemp = "ȼ��";
        }
        else
        {
            strTemp = "ѹ��";
        }
        DM->ADODataSetDataInfo->FieldByName("bCombustion")->AsString  = strTemp;

        //8 ���ͻ�ת��
        int ispeed ;
        ispeed = NormalizeSpeed(lDS.LFHS.siTrainType, lDS.LFHS.fEn);
        strTemp.printf("%d", ispeed);
        DM->ADODataSetDataInfo->FieldByName("DieselSpeed")->AsString   = strTemp;

        //9 ѹ�����ֵ
        strTemp.printf("%.2f",lDS.LFHS.fCPMax);
        DM->ADODataSetDataInfo->FieldByName("PressureMax")->AsString   = strTemp;

        //10 ѹ�����������ֵ
        strTemp.printf("%.4f",lDS.fDPDFMax);
        DM->ADODataSetDataInfo->FieldByName("DPDFMax")->AsString       = strTemp;

        //11 ȼ��ʼ��
        strTemp.printf("%.1f", fabs(lDS.fCombustionAngle));
        DM->ADODataSetDataInfo->FieldByName("CombustionPosi")->AsString       = strTemp;

        //12 ȼ�ճ���ʱ��
        strTemp.printf(" %.1f",lDS.fCombustionPeriod);
        DM->ADODataSetDataInfo->FieldByName("CombustionPeriod")->AsString   = strTemp;

        //13 ���������ֵ
        strTemp.printf("%.1f",lDS.fHRRMax);
        DM->ADODataSetDataInfo->FieldByName("DHDFMax")->AsString       = strTemp;


        //14 ѭ��������
        strTemp.printf("%.1f", lDS.LFHS.fHeatAll);
        DM->ADODataSetDataInfo->FieldByName("HeatAll")->AsString       = strTemp;

        //15 ѭ��ָʾ��
        strTemp.printf("%.1f ", lDS.LFHS.fCPower);
        DM->ADODataSetDataInfo->FieldByName("CPower")->AsString       = strTemp;

        //16 ����ʼ��
        strTemp.printf("%.1f",fabs(lDS.fOilAngle));
        DM->ADODataSetDataInfo->FieldByName("OilPosi")->AsString       = strTemp;

        //17 ���ͳ���ʱ��
        strTemp.printf("%.1f",lDS.LFHS.fWavePeriod);
        DM->ADODataSetDataInfo->FieldByName("WavePeriod")->AsString    = strTemp;

        //18 ����ϵ��
        strTemp.printf("%.2f",lDS.LFHS.fWaveCoef);
        DM->ADODataSetDataInfo->FieldByName("WaveCoef")->AsString      = strTemp;

        //19 �ļ���
        DM->ADODataSetDataInfo->FieldByName("FileName")->AsString      = strPCPName;
        //20 �͹����ֵ
        strTemp.printf("%.2f",lDS.LFHS.fOilPeak);
        DM->ADODataSetDataInfo->FieldByName("fOilMax")->AsString      = strTemp;
        //21 ��������ѹ��
        strTemp.printf("%.2f",lDS.LFHS.fBarP0);
        DM->ADODataSetDataInfo->FieldByName("fP0")->AsString          = strTemp;
   }
   return true;
}


void __fastcall TFormMain::ToolButtonPropertyClick(TObject *Sender)
{
    AnsiString strTemp;
    if( (bFileOpened) //����Ѵ�PCP�ļ�������ʾ�ļ�������
       ||(bLoadOriginal)
       )
    {
        FormFileInfo = new TFormFileInfo(FormMain);
        if(bFileOpened)
        {
            FormFileInfo->PanelFileName->Caption  = strPCPName;
        }
        else
        {
            FormFileInfo->PanelFileName->Caption  = strOriginalName;
        }

        SetTrainType(lDS.LFHS.siTrainType);
        FormFileInfo->PanelTrainType->Caption = strTrainType;
                //
        strTemp.printf("%04d",lDS.LFHS.iTrainNum);
        FormFileInfo->PanelTrainNum->Caption  = strTemp;

        strTemp.printf("�� %02d ��",lDS.LFHS.siDieselNum );
        FormFileInfo->PanelDieselNum->Caption = strTemp;

        strTemp.printf("%d Hz",lDS.LFHS.iRFrequency);
        FormFileInfo->PanelSampleFre->Caption = strTemp;

        strTemp.printf("%d Hz",lDS.LFHS.siCutoffFreq[0]);
        FormFileInfo->PanelCutoffFreq->Caption = strTemp;

        strTemp.printf("%d Hz",lDS.LFHS.siCutoffFreq[1]);
        FormFileInfo->PanelCutoffFreq1->Caption = strTemp;

        strTemp.printf("%d ��",lDS.LFHS.siSetSampleNum);
        FormFileInfo->PanelSampleNum->Caption = strTemp;

        strTemp.printf("%d ��",lDS.LFHS.iAbstractLength);
        FormFileInfo->PanelSampleLen->Caption = strTemp;

        strTemp.printf("%.1f r/min",lDS.LFHS.fEn);
        FormFileInfo->PanelfEn->Caption = strTemp;

        strTemp.printf("%.2f MPa",lDS.LFHS.fCPMax);
        FormFileInfo->PanelfCPMax->Caption = strTemp;

        strTemp.printf("%.3f MPa",lDS.LFHS.fP1RMS);
        FormFileInfo->PanelfCPMin->Caption = strTemp;

        if(lDS.LFHS.fCPMax > 0.001)
        {
            strTemp.printf("%.2f %%",(lDS.LFHS.fP1RMS/(lDS.LFHS.fCPMax))*100.0);
            FormFileInfo->PanelPercentage->Caption = strTemp;
        }
        

        strTemp.printf("%d",lDS.LFHS.siGain[0]);
        FormFileInfo->PanelGain->Caption = strTemp;

        strTemp.printf("%.1f PC/MPa",lDS.LFHS.fSensitivity[0]);
        FormFileInfo->PanelSensitivity->Caption = strTemp;
        FormFileInfo->ShowModal();

        delete FormFileInfo;
        FormFileInfo = NULL;    
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ppmAddToDatabaseClick(TObject *Sender)
{
    AnsiString strTemp,strNameTemp,strPCPBackup;
    bool bdatabasestatus;
    int iindex,icount;
    icount = FileListBoxFile->Items->Count;
    if(icount <= 0)
    {
        return;
    }
    strPSTFileDirectory = FileListBoxFile->Directory;
    iindex = FileListBoxFile->ItemIndex;
    if(iindex < 0) return;
    strTemp = FileListBoxFile->Directory + "\\";
    strNameTemp = strTemp + FileListBoxFile->Items->Strings[iindex];
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;    // Show hourglass cursor
    try
    {
        Screen->Cursor = crHourGlass;
        MainInsertToDatabase(strNameTemp);
        Application->MessageBox("�������ݿ��������!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }

}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MainInsertToDatabase(AnsiString strNameTemp)
{
    AnsiString strTemp;
    AnsiString strPCPBackup;
    int icount;
    TCursor Save_Cursor = Screen->Cursor ;    // Show hourglass cursor
    try
    {
        if(!bParameterInited)
        {
            InitParameters();
            bParameterInited = true;
        }
  // Do some lengthy operation
        Screen->Cursor = crHourGlass;
        strTemp = ExtractFileExt(strNameTemp);
        strTemp = strTemp.UpperCase();

        if(strTemp == ".PCP")
        {
            strPCPBackup = strPCPName;
            strPCPName = strNameTemp;
            LoadCurrentBinPCP(false);   //Ҫ��ͼ��false
            bAddToDatabaseResult = true;
            AddToDatabase(true);
            bAddToDatabaseResult = false;
            strPCPName = strPCPBackup;

        }
        else if(strTemp == ".PST")
        {
            bFileChanged = true;
            strPSTName = strNameTemp;
            ListBoxPCPFile->Items->LoadFromFile(strPSTName);
            icount = ListBoxPCPFile->Items->Count;

            if(icount > 0)
            {
                strPCPBackup = strPCPName;
                for( int i = 0; i < icount; i ++)
                {
                    strPCPName = ::ExtractFilePath(strPSTName) + ::ExtractFileName(ListBoxPCPFile->Items->Strings[i]);
                    LoadCurrentBinPCP(true);   //Ҫ��ͼ��false
                    bAddToDatabaseResult = true;
                    AddToDatabase(true);
                    bAddToDatabaseResult = false;
                }
                strPCPName = strPCPBackup;
            }
        }
        if(strPCPName != "")
        {
            LoadCurrentBinPCP(false);
        }
        bTVLoaded   = false;   //��ʾҪ��TV��ͼ����ˢ��
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
    if(DM->ADODataSetDataInfo->Active)
    {
        DM->ADODataSetDataInfo->Close();
    }
}

void __fastcall TFormMain::mnuDatabaseOutputClick(TObject *Sender)
{
    if(DM->ADOConnectionDataInfo->Connected)
    {
        FormReportOut = new TFormReportOut(FormMain);
        if(NULL != FormReportOut)
        {
            FormReportOut->QuickRep1->ReportTitle = "JL-902�������ͻ�ȼ�շ��������ݱ���";
            FormReportOut->QuickRep1->Preview();
            delete FormReportOut;
            FormReportOut  = NULL;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::mnuDatabaseViewClick(TObject *Sender)
{
    PageControlMain->ActivePageIndex = 5;
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;    // Show hourglass cursor
    try
    {
        Screen->Cursor = crHourGlass;
        FormTVAddTrainType();
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::TVAddTrainType(void)
{
/*
    //TODO: Add your source code here
    AnsiString strTempTT,     strTempTN;
    AnsiString strChild,      strChildName;
    AnsiString strsCombustion, strsDate;
    AnsiString strEngineSpeed;
    AnsiString strGrandChild, strGrandChildName;
    TTreeNode* gNode, *lastNode;
    TTreeNode *rNode,*mNode,*sNode,*gsNode,*cmNode;
    AnsiString strTrainID;
    AnsiString strTemp;
    int iImageIndex;

    TreeViewDatabase->Items->Clear();
    rNode = TreeViewDatabase->Items->Add(TreeViewDatabase->Selected,"���ݿ�");  //������ڵ�
    rNode->ImageIndex    = 8;    //û��չ��ǰ��ͼ��
    rNode->SelectedIndex = 8;    //չ���Ժ��ͼ��

    strTempTT = "select distinct TrainType from DataTable order by TrainType";
    DM->ADOQueryTrainType->Close();
    DM->ADOQueryTrainType->SQL->Clear();
    DM->ADOQueryTrainType->SQL->Add (strTempTT);
    DM->ADOQueryTrainType->Open();
    int dataLength = DM->ADOQueryTrainType->RecordCount;
    if(dataLength == 0)
    {
        DM->ADOQueryTrainType->Close();
        return true;
    }
    DM->ADOQueryTrainType->First();
    while(!DM->ADOQueryTrainType->Eof)   //�����������
    {
        strTempTN = DM->ADOQueryTrainType->Fields->Fields[0]->Value;
        mNode   = TreeViewDatabase->Items->AddChild(rNode,strTempTN);
        if(strTempTN == "����4A")
        {
            mNode->ImageIndex = 0;
            mNode->SelectedIndex = 0;
            iImageIndex = 0;
        }
        else if(strTempTN == "����4B")
        {
            mNode->ImageIndex = 1;
            mNode->SelectedIndex = 1;
            iImageIndex = 1;
        }
        else if(strTempTN == "����4C")
        {
            mNode->ImageIndex = 2;
            mNode->SelectedIndex = 2;
            iImageIndex = 2;
        }
        else if(strTempTN == "����4D")
        {
            mNode->ImageIndex = 3;
            mNode->SelectedIndex = 3;
            iImageIndex = 3;
        }
        else if(strTempTN == "����8B")
        {
            mNode->ImageIndex = 5;
            mNode->SelectedIndex = 5;
            iImageIndex = 5;
        }
        else if(strTempTN == "����11")
        {
            mNode->ImageIndex = 6;
            mNode->SelectedIndex = 6;
            iImageIndex = 6;
        }
        else
        {
            mNode->ImageIndex = 7;
            mNode->SelectedIndex = 7;
            iImageIndex = 7;
        }
        strTempTT = "select distinct TrainNumber from DataTable where TrainType = '";
        strTempTT += strTempTN;
        strTempTT += "' order by TrainNumber";
        DM->ADOQueryTrainNumber->Close();
        DM->ADOQueryTrainNumber->SQL->Clear();
        DM->ADOQueryTrainNumber->SQL->Add(strTempTT);
        DM->ADOQueryTrainNumber->Open();
        DM->ADOQueryTrainNumber->First();
        while(!DM->ADOQueryTrainNumber->Eof) //�����������
        {
            strChild = DM->ADOQueryTrainNumber->Fields->Fields[0]->Value;
            gNode = TreeViewDatabase->Items->AddChild(mNode,strChild);
            gNode->ImageIndex    = iImageIndex;
            gNode->SelectedIndex = iImageIndex;

            DM->ADOQueryDate->Close();
            strTempTT = "select distinct MDate from DataTable where TrainType = '";
            strTempTT += strTempTN;
            strTempTT += "' and TrainNumber = '";
            strTempTT += strChild;
            strTempTT += "' order by MDate";
            DM->ADOQueryDate->SQL->Clear();
            DM->ADOQueryDate->SQL->Add(strTempTT);
            DM->ADOQueryDate->Open();
            DM->ADOQueryDate->First();

            while(!DM->ADOQueryDate->Eof)     //�����������
            {
                TDateTime dtTemp;
                dtTemp = TDateTime(DM->ADOQueryDate->Fields->Fields[0]->Value);
                unsigned short usyear,usmonth,usdate;
                dtTemp.DecodeDate(&usyear,&usmonth,&usdate);
                if(usyear<2000)
                {
                     usyear += 2000;
                }
                strChildName.printf("%04d-%02d-%02d",usyear,usmonth,usdate) ;

                gsNode = TreeViewDatabase->Items->AddChild(gNode,strChildName);
                gsNode->ImageIndex    = 9;
                gsNode->SelectedIndex = 9;
                strTempTT = "select distinct DieselSpeed from DataTable where TrainType = '";
                strTempTT += strTempTN;
                strTempTT += "' and TrainNumber = '";
                strTempTT += strChild;
                strTempTT += "' and MDate between :TimeStart and :TimeEnd order by DieselSpeed";
                DM->ADOQuerySpeed->Close();
                DM->ADOQuerySpeed->SQL->Clear();

                DM->ADOQuerySpeed->Parameters->Clear();
                DM->ADOQuerySpeed->Parameters->AddParameter();
                DM->ADOQuerySpeed->Parameters->Items[0]->Name      = "TimeStart";
                DM->ADOQuerySpeed->Parameters->Items[0]->DataType  = ftDate;
                DM->ADOQuerySpeed->Parameters->Items[0]->Direction = pdInput;
                DM->ADOQuerySpeed->Parameters->AddParameter();
                DM->ADOQuerySpeed->Parameters->Items[1]->Name      = "TimeEnd";
                DM->ADOQuerySpeed->Parameters->Items[1]->DataType  = ftDate;
                DM->ADOQuerySpeed->Parameters->Items[1]->Direction = pdInput;
                DM->ADOQuerySpeed->Parameters->Items[0]->Value     = dtTemp - 1;
                DM->ADOQuerySpeed->Parameters->Items[1]->Value     = dtTemp;

                DM->ADOQuerySpeed->SQL->Add(strTempTT);
                DM->ADOQuerySpeed->Open();
                DM->ADOQuerySpeed->First();
                while(!DM->ADOQuerySpeed->Eof)   //������ͻ�ת��
                {
                    strDieselSpeed  = DM->ADOQuerySpeed->Fields->Fields[0]->Value;
                    strTemp  = strDieselSpeed + AnsiString(" r/min");
                    lastNode = TreeViewDatabase->Items->AddChild(gsNode,strTemp);
                    lastNode->ImageIndex    = 10;
                    lastNode->SelectedIndex = 10;

                    strTempTT = "select distinct bCombustion from DataTable where TrainType = '";
                    strTempTT += strTempTN;
                    strTempTT += "' and TrainNumber = '";
                    strTempTT += strChild;
                    strTempTT += "' and MDate between :TimeStart and :TimeEnd ";
                    strTempTT += "  and DieselSpeed = '";
                    strTempTT += strDieselSpeed + "' order by bCombustion";
                    DM->ADOQueryTemp->Close();
                    DM->ADOQueryTemp->SQL->Clear();

                    DM->ADOQueryTemp->Parameters->Clear();
                    DM->ADOQueryTemp->Parameters->AddParameter();
                    DM->ADOQueryTemp->Parameters->Items[0]->Name      = "TimeStart";
                    DM->ADOQueryTemp->Parameters->Items[0]->DataType  = ftDate;
                    DM->ADOQueryTemp->Parameters->Items[0]->Direction = pdInput;
                    DM->ADOQueryTemp->Parameters->AddParameter();
                    DM->ADOQueryTemp->Parameters->Items[1]->Name      = "TimeEnd";
                    DM->ADOQueryTemp->Parameters->Items[1]->DataType  = ftDate;
                    DM->ADOQueryTemp->Parameters->Items[1]->Direction = pdInput;
                    DM->ADOQueryTemp->Parameters->Items[0]->Value     = dtTemp - 1;
                    DM->ADOQueryTemp->Parameters->Items[1]->Value     = dtTemp;

                    DM->ADOQueryTemp->SQL->Add(strTempTT);
                    DM->ADOQueryTemp->Open();
                    DM->ADOQueryTemp->First();
                    while(!DM->ADOQueryTemp->Eof)
                    {
                        strCombustion  = DM->ADOQueryTemp->Fields->Fields[0]->Value;
                        cmNode = TreeViewDatabase->Items->AddChild(lastNode,strCombustion);
                        cmNode->ImageIndex    = 10;
                        cmNode->SelectedIndex = 10;
                        DM->ADOQueryTemp->Next();
                    }
                    DM->ADOQuerySpeed->Next();
                }
                DM->ADOQueryDate->Next();
            }
            DM->ADOQueryTrainNumber->Next();
        }
        DM->ADOQueryTrainType->Next();
    }
    if(DM->ADOQueryTrainType->Active)
    {
        DM->ADOQueryTrainType->Close();
    }
    if(DM->ADOQueryTrainNumber->Active)
    {
        DM->ADOQueryTrainNumber->Close();
    }
    if(DM->ADOQuerySpeed->Active)
    {
        DM->ADOQuerySpeed->Close();
    }
    if(DM->ADOQueryDate->Active)
    {
        DM->ADOQueryDate->Close();
    }
    if(DM->ADOQueryTemp->Active)
    {
        DM->ADOQueryTemp->Close();
    }
    strTempTT = strExeFilePath + "TV.buf";
    TreeViewDatabase->SaveToFile(strTempTT);
    bTVLoaded = true;    //TTreeView
*/
    return true;
}
//=====================================================
void __fastcall TFormMain::RefreshStatusBar(AnsiString strInfo, int iPosi)
{
    //TODO: Add your source code here
    StatusBarMain->Panels->Items[iPosi]->Text = strInfo;
    StatusBarMain->Refresh();
    Application->ProcessMessages();
}

void __fastcall TFormMain::QueryDatabasebyTV(void)
{
    //TODO: Add your source code here
	AnsiString strOperation = "", strForePart = "";
    AnsiString strTemp,strTTemp;
    int iItem;
    int iRecordCount = 0;
    RefreshStatusBar("�Ժ�,���ڲ�ѯ....",0);
    AnsiString strTempTT,strTempTN;

    if(iQueryStyle == 2)  //
    {
        strOperation = "select * from DataTable where TrainType = '" +
                        strTrainType +
                        "' and TrainNumber = '" +
                        strTrainNumber +
                        "'";
    }
    else if(iQueryStyle == 3)   // query by date
    {
        strOperation = "select * from DataTable where TrainType = '" +
                        strTrainType +
                        "' and TrainNumber = '" +
                        strTrainNumber +
                        "' ";
        DM->ADODataSetDataInfo->Parameters->Clear();
        DM->ADODataSetDataInfo->Parameters->AddParameter();
        DM->ADODataSetDataInfo->Parameters->Items[0]->Name = "TimeStart";
        DM->ADODataSetDataInfo->Parameters->Items[0]->DataType = ftDate;
        DM->ADODataSetDataInfo->Parameters->Items[0]->Direction = pdInput;
        unsigned short usYear, usMonth, usDate;
        GetDateFromString(strMDate,usYear,usMonth,usDate);
        TDate dtTemp(usYear,usMonth,usDate);
        strTemp = " and MDate = :TimeStart ";
        DM->ADODataSetDataInfo->Parameters->Items[0]->Value = dtTemp;
        strOperation += strTemp;
    }
    else if(iQueryStyle == 4)  //�� ʱ��Ͳ��ͻ�ת������ѯ
    {
        strOperation = "select * from DataTable where TrainType = '" +
                        strTrainType +
                        "' and TrainNumber = '" +
                        strTrainNumber +
                        "' and DieselSpeed = '" +
                        strDieselSpeed  + "'";
        DM->ADODataSetDataInfo->Parameters->Clear();
        DM->ADODataSetDataInfo->Parameters->AddParameter();
        DM->ADODataSetDataInfo->Parameters->Items[0]->Name = "TimeStart";
        DM->ADODataSetDataInfo->Parameters->Items[0]->DataType = ftDate;
        DM->ADODataSetDataInfo->Parameters->Items[0]->Direction = pdInput;
        unsigned short usYear, usMonth, usDate;
        GetDateFromString(strMDate,usYear,usMonth,usDate);
        TDate dtTemp(usYear,usMonth,usDate);
        strTemp = " and MDate = :TimeStart ";
        DM->ADODataSetDataInfo->Parameters->Items[0]->Value = dtTemp;
        strOperation += strTemp;
    }
    else if(iQueryStyle == 5)  //�� ʱ��Ͳ��ͻ�ת������ѯ
    {
        strOperation = "select * from DataTable where TrainType = '" +
                        strTrainType +
                        "' and TrainNumber = '" +
                        strTrainNumber +
                        "' and DieselSpeed = '" +
                        strDieselSpeed  + "'";
        DM->ADODataSetDataInfo->Parameters->Clear();
        DM->ADODataSetDataInfo->Parameters->AddParameter();
        DM->ADODataSetDataInfo->Parameters->Items[0]->Name = "TimeStart";
        DM->ADODataSetDataInfo->Parameters->Items[0]->DataType = ftDate;
        DM->ADODataSetDataInfo->Parameters->Items[0]->Direction = pdInput;
        unsigned short usYear, usMonth, usDate;
        GetDateFromString(strMDate,usYear,usMonth,usDate);
        TDate dtTemp(usYear,usMonth,usDate);
        DM->ADODataSetDataInfo->Parameters->Items[0]->Value = dtTemp;

        strTemp       = " and MDate = :TimeStart and bCombustion = '";
        strOperation += strTemp;
        strTemp       = strCombustion + "'";
        strOperation += strTemp;
    }


    if(strOperation != "")
    {
    	int iValnum;
    	strOperation += " Order by DieselNumber,MDate,bCombustion";
//        strOperation = strForePart + strOperation;
    	DM->ADODataSetDataInfo->Close();
		DM->ADODataSetDataInfo->CommandType = cmdText;
    	DM->ADODataSetDataInfo->CommandText = strOperation;
    	DM->ADODataSetDataInfo->Open();
        iRecordCount = DM->DataSourceDataInfo->DataSet->RecordCount ;
        strTemp.printf("�˴ι���ѯ�� %d ����¼",iRecordCount);
        PanelRecordNum->Caption = strTemp;
        if(iRecordCount > 0)
        {
            bTVQuery = true;
        }
        else
        {
            DM->ADODataSetDataInfo->Close();
            bTVQuery = false;
        }
    }

    if( iRecordCount >0)   //�����ѯ�������ݣ�������ݽ��з���
    {

    }
    RefreshStatusBar("",0);
}

void __fastcall TFormMain::TVDrawCurve(void)
{
/*
    //TODO: Add your source code here
    int inowposi;
    float ftemp;
    TDateTime dttemp;
    int ioldposi = 0;
    if(bTVQuery)
    {
        if(iQueryStyle == 2)  //�����ͳ��Ž��в�ѯ
        {
            if(bPosiCompare)  //����λ����ʾͼ��
            {
                int iSerial = 0;
                TDateTime dtOld = 0;
                RefreshChartTV(false);
                if(ADODataSetDataInfo->Active)
                {
//                    ADODataSetDataInfo->DisableControls();
                    ADODataSetDataInfo->First();
                    while(!ADODataSetDataInfo->Eof)
                    {
                        inowposi = ADODataSetDataInfo->FieldByName("PosiNumber")->AsInteger;
                        dttemp = ADODataSetDataInfo->FieldByName("MDate")->AsDateTime;
                        if(inowposi != ioldposi)
                        {
                            iSerial = 0;
                            ioldposi = inowposi;
                        }
                        if((dttemp != dtOld)&&(inowposi == ioldposi))
                        {
                            if(bMaxValue)
                            {
                                ftemp  = ADODataSetDataInfo->FieldByName("VMax")->AsFloat;
                            }
                            else
                            {
                                ftemp  = ADODataSetDataInfo->FieldByName("EHigh")->AsFloat;
                            }
                            ChartTV->Series[iSerial]->AddXY(inowposi,ftemp,"",clDrawColor[iSerial]);
                            dtOld = dttemp;
                            iSerial++;
                            if(iSerial>6)
                            {
                                continue;
                            }
                        }
                        ADODataSetDataInfo->Next();
                    }
 //                   ADODataSetDataInfo->EnableControls();
 //                   ADODataSetDataInfo->First();
                }
            }
            else   //������������ʾͼ��
            {
                RefreshChartTV(true);
                if(ADODataSetDataInfo->Active)
                {
 //                   ADODataSetDataInfo->DisableControls();
                    ADODataSetDataInfo->First();
                    while(!ADODataSetDataInfo->Eof)
                    {
                        inowposi = ADODataSetDataInfo->FieldByName("PosiNumber")->AsInteger;
                        if(inowposi == iiNowPosi)
                        {
                            dttemp = ADODataSetDataInfo->FieldByName("MDate")->AsDateTime;
                //  AddXY
                            if(ChBMax->Checked)
                            {
                                ftemp  = ADODataSetDataInfo->FieldByName("VMax")->AsFloat;
                                ChartTV->Series[0]->AddXY(dttemp,ftemp,"",clRed);
                            }
                            if(ChBAverage->Checked)
                            {
                                ftemp  = ADODataSetDataInfo->FieldByName("VAver")->AsFloat;
                                ChartTV->Series[1]->AddXY(dttemp,ftemp,"",clGreen);
                            }
                            if(ChBFre1->Checked)
                            {
                                ftemp  = ADODataSetDataInfo->FieldByName("ELow")->AsFloat;
                                ChartTV->Series[2]->AddXY(dttemp,ftemp,"",clYellow);
                            }
                            if(ChBFre2->Checked)
                            {
                                ftemp  = ADODataSetDataInfo->FieldByName("EMedium")->AsFloat;
                                ChartTV->Series[3]->AddXY(dttemp,ftemp,"",clBlue);
                            }
                            if(ChBFre3->Checked)
                            {
                                ftemp  = ADODataSetDataInfo->FieldByName("EHigh")->AsFloat;
                                ChartTV->Series[4]->AddXY(dttemp,ftemp,"",clWhite);
                            }
                            if(ChBFre4->Checked)
                            {
                                ftemp  = ADODataSetDataInfo->FieldByName("EADD0")->AsFloat;
                                ChartTV->Series[5]->AddXY(dttemp,ftemp,"",clGray);
                            }
                            if(ChBFre5->Checked)
                            {
                                ftemp  = ADODataSetDataInfo->FieldByName("EADD1")->AsFloat;
                                ChartTV->Series[6]->AddXY(dttemp,ftemp,"",clFuchsia);
                            }
                        }
                        else if(inowposi > iiNowPosi)
                        {
                            break;
                        }
                        ADODataSetDataInfo->Next();
                    }
//                    ADODataSetDataInfo->EnableControls();
//                    ADODataSetDataInfo->First();
                }
            }
        }
        else if(iQueryStyle == 3)  //���������ڽ��в�ѯ
        {
            RefreshChartTV(false);
            if(ADODataSetDataInfo->Active)
            {
//                ADODataSetDataInfo->DisableControls();
                ADODataSetDataInfo->First();
                while(!ADODataSetDataInfo->Eof)
                {
                    inowposi = ADODataSetDataInfo->FieldByName("PosiNumber")->AsInteger;
                    if(ChBMax->Checked)
                    {
                        ftemp  = ADODataSetDataInfo->FieldByName("VMax")->AsFloat;
                        ChartTV->Series[0]->AddXY(inowposi,ftemp,"",clRed);
                    }
                    if(ChBAverage->Checked)
                    {
                        ftemp  = ADODataSetDataInfo->FieldByName("VAver")->AsFloat;
                        ChartTV->Series[1]->AddXY(inowposi,ftemp,"",clGreen);
                    }
                    if(ChBFre1->Checked)
                    {
                        ftemp  = ADODataSetDataInfo->FieldByName("ELow")->AsFloat;
                        ChartTV->Series[2]->AddXY(inowposi,ftemp,"",clYellow);
                    }
                    if(ChBFre2->Checked)
                    {
                        ftemp  = ADODataSetDataInfo->FieldByName("EMedium")->AsFloat;
                        ChartTV->Series[3]->AddXY(inowposi,ftemp,"",clBlue);
                    }
                    if(ChBFre3->Checked)
                    {
                        ftemp  = ADODataSetDataInfo->FieldByName("EHigh")->AsFloat;
                        ChartTV->Series[4]->AddXY(inowposi,ftemp,"",clWhite);
                    }
                    if(ChBFre4->Checked)
                    {
                        ftemp  = ADODataSetDataInfo->FieldByName("EADD0")->AsFloat;
                        ChartTV->Series[5]->AddXY(inowposi,ftemp,"",clGray);
                    }
                    if(ChBFre5->Checked)
                    {
                        ftemp  = ADODataSetDataInfo->FieldByName("EADD1")->AsFloat;
                        ChartTV->Series[6]->AddXY(inowposi,ftemp,"",clFuchsia);
                    }
                    ADODataSetDataInfo->Next();
                }
//                ADODataSetDataInfo->EnableControls();
//                ADODataSetDataInfo->First();
            }
        }
    }
*/    
}

void __fastcall TFormMain::RefreshTV(void)
{
    //TODO: Add your source code here
    TCursor Save_Cursor = Screen->Cursor;    // Show hourglass cursor
    if(!bTVLoaded)
    {
        RefreshStatusBar("������ȡ���ݿ���Ϣ!!!",0);
        try
        {
            Screen->Cursor = crHourGlass;
            TVAddTrainType();   //�������б����и���
        }
        __finally
        {
            Screen->Cursor = Save_Cursor;
        }
        RefreshStatusBar("",0);
    }
}




void __fastcall TFormMain::TreeViewDatabaseDblClick(TObject *Sender)
{
/*
    AnsiString strSelected[10];
    TTreeNode* mNode = TreeViewDatabase->Selected;
    int i = 0;
    if(mNode == NULL)
    {
        return;
    }
    if(mNode->Text != "���ݿ�")
    {
        if(mNode != NULL)
        {
            do
            {
                strSelected[i] = mNode->Text;
                mNode = mNode->Parent;
                i++;
            }while( mNode->Text != "���ݿ�");
        }
    }
    strMDate        = "";
    strDieselSpeed  = "";
    strCombustion   = "";

    if(i == 2)
    {
//        Application->MessageBox("ѡ���˳��͡�����!","��ʾ",MB_OK|MB_ICONINFORMATION);
        iQueryStyle     = 2;                //ѡ���˳��ͳ��Ž��в�ѯ
        strTrainType    = strSelected[1];
        strTrainNumber  = strSelected[0];
        
    }
    else if(i == 3)
    {
//        Application->MessageBox("ѡ���˳��͡����źͲ�������!","��ʾ",MB_OK|MB_ICONINFORMATION);
        iQueryStyle     = 3;               //ѡ���˳��ͳ��ż�����ʱ����в�ѯ
        strTrainType    = strSelected[2];
        strTrainNumber  = strSelected[1];
        strMDate        = strSelected[0];
    }
    else if( i == 4)
    {
//        Application->MessageBox("ѡ���˳��͡����š��������ں�ת��!","��ʾ",MB_OK|MB_ICONINFORMATION);

        iQueryStyle      = 4;
        strTrainType     = strSelected[3];
        strTrainNumber   = strSelected[2];
        strMDate         = strSelected[1];
        strDieselSpeed   = strSelected[0];
        strDieselSpeed   = strDieselSpeed.SubString(1,strDieselSpeed.Pos(" ")-1);
    }
    else if( i == 5)
    {
        iQueryStyle     = 5;
        strTrainType    = strSelected[4];
        strTrainNumber  = strSelected[3];
        strMDate        = strSelected[2];
        strDieselSpeed  = strSelected[1];
        strDieselSpeed  = strDieselSpeed.SubString(1,strDieselSpeed.Pos(" ")-1);
        strCombustion   = strSelected[0];
    }
    else
    {
        iQueryStyle = 1;     //��������Ч�Ĳ���
    }

    if(iQueryStyle > 3)    //����ֻ�г��źͲ������ڵĲ��ٽ��в�ѯ
    {
        iTVLastPosi = 0;
        iTVCurrPosi = 0;
        QueryDatabasebyTV();
        
        iTrainType = GetiTrainType(strTrainType);
        SetDieselInfo(iTrainType,false);                //�����ϵͳ���ò��ͻ�����

        TCursor Save_Cursor = Screen->Cursor;
        Screen->Cursor      = crHourGlass;    // Show hourglass cursor

        try
        {
            Screen->Cursor = crHourGlass;
            if(!bAnalysising)
            {
                bAnalysising   = true;

                FillmyDetectResult(true);          //����������װ����Ϣ�ṹ��

                AnalysisResultAll(true);           //������ʾ���

                bAnalysising   = false;
            }
        }
        __finally
        {
            Screen->Cursor = Save_Cursor;
        }
    }
*/    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::GetDateFromString(AnsiString& strdate, unsigned short & uy, unsigned short & um, unsigned short& ud)
{
    //TODO: Add your source code here
    AnsiString strAll,strMore;
    int iposi,ilen;
    strAll  = strdate;
    ilen    = strAll.Length();
    iposi   = strAll.Pos("-");
    strMore = strAll.SubString(1,iposi - 1);
    uy      = StrToInt(strMore);
    strAll  = strAll.SubString(iposi + 1,ilen - iposi);
    ilen    = strAll.Length();
    iposi   = strAll.Pos("-");
    strMore = strAll.SubString(1,iposi-1);
    um      = StrToInt(strMore);
    strAll  = strAll.SubString(iposi+1, ilen - iposi);
    ud      = StrToInt(strAll);
}


int __fastcall TFormMain::GetiTrainType(AnsiString & strTrain)
{
    //TODO: Add your source code here
    int iret;
    if( strTrain == "����4A")
    {
        iret = 0;
    }
    else if(strTrain == "����4B")
    {
        iret = 1;
    }
    else if(strTrain == "����4C")
    {
        iret = 2;
    }
    else if(strTrain == "����4D")
    {
        iret = 3;
    }
    else if(strTrain == "����8B")
    {
        iret = 4;
    }
    else if(strTrain == "����11")
    {
        iret = 5;
    }
    else if(strTrain == "OTHE")
    {
        iret = 6;
    }
    return iret;
}

bool __fastcall TFormMain::FormPSTList_New(void)
{
// ListPath :�б��ļ���¼
// ListBoxPCPFile���б�����ǰ�˳�����е�PCP�ļ���
// iTrainTimes;    �б��ļ�����
// iListNum[800]   �ļ���
    FILE* fp;
    AnsiString strTemp;
    int iposi;
    AnsiString strTempp;
    bool bret = true;
    int icount;
    for( int i = 0 ; i < iTrainTimes; i ++)
    {
        if(bGridFilled)
        {
            strTempp = ListBoxTrainList->Items->Strings[i];
        }
        else
        {
            strTempp = ListPath->Strings[i] +
                   ListBoxTrainList->Items->Strings[i] +
                   AnsiString(".pst");
       //���ȴ��б������ļ�
            ListBoxTrainList->Items->Strings[i] = strTempp;
            ListList->Add(strTempp);
        }
//-----------------------------------------------------------------
//ѡ���Զ�ȡ�ļ�

        if( !ListFileValid( strTempp ) )
        {
            continue;
        }

//=================================================================
        if((fp = fopen(strTempp.c_str(),"w")) != NULL)
        {
            for(int j = 0; j < 16; j++)
            {
                iposi = i * 16 + j;
                if(iListNum[iposi] != 1000)
                {
                    strTemp = ListBoxPCPFile->Items->Strings[iListNum[iposi]];
                    fprintf(fp,"%s\n",strTemp.c_str());
                }
            }
            fclose(fp);
            bret = true;
        }
        else
        {
            bret = false;
        }
    }
    if(CheckBoxAddMInfo->Checked)  //���ѡ�񽫼����Ϣ�������ݿ�Ļ�
    {
        icount = ListBoxTrainList->Items->Count;
        for(int i = 0; i < icount; i ++)
        {
            strTemp = ListBoxTrainList->Items->Strings[i];     //strTemp��Ϊ�б��ļ���
            AddToMeasureInfo( strTemp );
        }
    }
    return bret;
}

void __fastcall TFormMain::ListBoxTrainListDblClick(TObject *Sender)
{
    int icount;
    int iindex;
    AnsiString strTempD, strTempS;
    if(!bInstrumentRead)
    {
        Application->MessageBox("����<��ȫ�������ļ�>��ť��\n����ȫ�������ļ�,\n\nȻ����ִ�д˲���!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    icount = ListBoxTrainList->Items->Count;
    if(icount > 0)
    {
        ToolButton22->Down = false;
        ToolButton36->Down = true;
        iindex = ListBoxTrainList->ItemIndex;
        bLoadList = true;
        bAllHRR = false;
        strPSTName = ListList->Strings[iindex];
        ListBoxPCPFile->Items->LoadFromFile(strPSTName);
        ListBoxPCPFile->Refresh();
        strValidPath = ExtractFilePath(strPSTName);
        strTempS = ::ExtractFileName(strPSTName);
        int icount = ListBoxPSTFile->Items->Count;
        for( int i = 0; i < icount; i++)
        {
            strTempD = ListBoxPSTFile->Items->Strings[i];
            strTempD = ::ExtractFileName(strTempD);
            if( strTempS == strTempD )
            {
//                strValidPath = ::ExtractFilePath(strTempD);
                ListBoxPSTFile->ItemIndex = i;
                break;
            }
        }
        iListDrawKind = 1;
        LoadCurrentBinPST();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Color1Click(TObject *Sender)
{
    if(ColorDialog1->Execute())
    {
       clDatabaseColor = ColorDialog1->Color;
       DBGridDataInfo->Refresh();
//       RefreshPBBaseCurve();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::DBGridDataInfoDrawColumnCell(TObject *Sender,
      const TRect &Rect, int DataCol, TColumn *Column,
      TGridDrawState State)
{
    if( !DM->ADODataSetDataInfo->Active )
    {
        return;
    }
    if(State == TGridDrawState()<<gdSelected)
    {
        return;
    }
    if(DM->ADODataSetDataInfo->RecNo%2 == 0)
    {
        DBGridDataInfo->Canvas->Brush->Color = clWhite;
    }
    else
    {
        DBGridDataInfo->Canvas->Brush->Color = clDatabaseColor;
        DBGridDataInfo->DefaultDrawColumnCell(Rect,DataCol,Column,State);
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::N19Click(TObject *Sender)
{
    FormBackup    = new TFormBackup(FormMain);
    DM->ADOQueryTemp->Close();
    DM->ADOQueryTemp->SQL->Clear();
    DM->ADOQueryTemp->SQL->Add ("select distinct trainNumber from DataTable order by TrainNumber");
    DM->ADOQueryTemp->Open();
    int dataLength = DM->ADOQueryTemp->RecordCount ;
    AnsiString strTTTT;
    FormBackup->ComboBoxTrainNum->Items->Clear();
    DM->ADOQueryTemp->First();
    for(int i=0; i<dataLength; i++)
    {
        strTTTT = DM->ADOQueryTemp->Fields->Fields[0]->Value;
        FormBackup->ComboBoxTrainNum->Items->Add(strTTTT);
        DM->ADOQueryTemp->Next();
    }
    if(dataLength>0)
    {
        FormBackup->ComboBoxTrainNum->ItemIndex = 0;
    }
    FormBackup->RadioGroupBackMode->ItemIndex   = iDeletePosi;

//    FormBackup->
    DM->ADOQueryTemp->First();
    DM->ADOQueryTemp->Close();
    dtDeleteDate = Now();
    FormBackup->DTPDate->DateTime = dtDeleteDate;

    if(FormBackup->ShowModal() == mrOk)
    {
        iBackupMode = FormBackup->RadioGroupBackMode->ItemIndex;
        if(iBackupMode == 0)
        {
            strBackupTrainNum = FormBackup->ComboBoxTrainNum->Text;
        }
        else if(iBackupMode == 1) //��ʱ�䡡
        {
            dtDeleteDate = FormBackup->DTPDate->DateTime;
        }
        iDeletePosi = FormBackup->RadioGroupBackMode->ItemIndex;

        int iresult = MessageDlg("������!!\n\n���Ҫ������ɾ����?", mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0);

        if(iresult==mrYes)
        {
            DeleteFromDatabase();
            Application->MessageBox("����ɾ������!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
        }
    }
    delete FormBackup;
    FormBackup  = NULL;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::DeleteFromDatabase(void)
{
    //TODO: Add your source code here
    int iCount,i;
    int iItem;
    AnsiString strOperation = "", strForePart = "";
    AnsiString strTemp,strTTemp;
    TStringList *  strstrlist;
//    PageControl1->ActivePageIndex=0;

    if(DM->ADOConnectionDataInfo->Connected == false)
    {
        DM->ADOConnectionDataInfo->Open();
    }
    if(DM->ADODataSetDataInfo->Active)
    {
        DM->ADODataSetDataInfo->Close();
    }
    if(DM->ADOQueryTemp->Active)
    {
        DM->ADOQueryTemp->Close();

    }
    DM->ADOQueryTemp->SQL->Clear();
    strOperation  = "delete  from DataTable";

    if(iDeletePosi == 0)//ɾ��ĳ̨����ȫ������
    {
        DM->ADOQueryTemp->SQL->Clear();
        DM->ADOQueryTemp->SQL->Add(strOperation);
        strOperation  = " where TrainNumber = '" + strBackupTrainNum + "'";
        DM->ADOQueryTemp->SQL->Add(strOperation);
        DM->ADOQueryTemp->Prepared  = true;
        DM->ADOQueryTemp->ExecSQL();
        DM->ADOQueryTemp->Prepared  = false;
    }
    else if(iDeletePosi == 1) //���й��ϵ�����ȫ��ɾ��
    {
        DM->ADOQueryTemp->SQL->Clear();
        DM->ADOQueryTemp->Parameters->Clear();
        DM->ADOQueryTemp->Parameters->AddParameter();
        DM->ADOQueryTemp->Parameters->Items[0]->Name      = "DeleteDate";
        DM->ADOQueryTemp->Parameters->Items[0]->DataType  = ftDate;
        DM->ADOQueryTemp->Parameters->Items[0]->Direction = pdInput;
        DM->ADOQueryTemp->Parameters->Items[0]->Value     = dtDeleteDate;
        DM->ADOQueryTemp->SQL->Add(strOperation);
        strOperation  = " where MDate <= :DeleteTime" ;
        DM->ADOQueryTemp->SQL->Add(strOperation);
        DM->ADOQueryTemp->Prepared  = true;
        DM->ADOQueryTemp->ExecSQL();
        DM->ADOQueryTemp->Prepared  = false;
    }
    else if(iDeletePosi == 2)
    {
        DM->ADOQueryTemp->SQL->Clear();
        DM->ADOQueryTemp->SQL->Add(strOperation);
        DM->ADOQueryTemp->Prepared = true;
        DM->ADOQueryTemp->ExecSQL();
        DM->ADOQueryTemp->Prepared = false;
    }
    DM->ADOQueryTemp->Close();

//����������ؼ�����ˢ��
    bTVLoaded   = false;
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;    // Show hourglass cursor
    try
    {
        Screen->Cursor = crHourGlass;
        TVAddTrainType();
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
}
void __fastcall TFormMain::ppmDataDeleteClick(TObject *Sender)
{
    AnsiString strSelected[10];
    AnsiString strOperation;
    int iDeleteStyle;
    TTreeNode* mNode = FormTV->TreeViewDatabase->Selected,*pNode;
    if(DM->ADODataSetDataInfo->Active)
    {
        DM->ADODataSetDataInfo->Close();
    }
    int i = 0;
    if(mNode->Text != "���ݿ�")
    {
        if(mNode != NULL)
        {
            do
            {
                strSelected[i] = mNode->Text;
                mNode = mNode->Parent;
                i++;
            }while( mNode->Text != "���ݿ�");
        }
    }

    if(i == 2)
    {
        iDeleteStyle     = 2;                //ѡ���˳��ͳ��Ž��в�ѯ
        strTrainType    = strSelected[1];
        strTrainNumber  = strSelected[0];
    }
    else if(i == 3)
    {
        iDeleteStyle     = 3;               //ѡ���˳��ͳ��ż�����ʱ����в�ѯ
        strTrainType    = strSelected[2];
        strTrainNumber  = strSelected[1];
        strMDate        = strSelected[0];
    }
    else if( i == 4)
    {
        iDeleteStyle    = 4;
        strTrainType    = strSelected[3];
        strTrainNumber  = strSelected[2];
        strMDate        = strSelected[1];
        strDieselSpeed   = strSelected[0];
        strDieselSpeed   = strDieselSpeed.SubString(1,strDieselSpeed.Pos(" ") - 1 );
    }
    else if(i == 5)
    {
        iDeleteStyle     = 5;
        strTrainType     = strSelected[4];
        strTrainNumber   = strSelected[3];
        strMDate         = strSelected[2];
        strDieselSpeed   = strSelected[1];
        strDieselSpeed   = strDieselSpeed.SubString(1,strDieselSpeed.Pos(" ") - 1 );
        strCombustion    = strSelected[0];
    }
    else
    {
        iDeleteStyle = 1;     //��������Ч�Ĳ���
    }

    int iresult = MessageDlg("������!!\n\n���Ҫ������ɾ����?", mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0);

    if(iresult==mrYes)
    {
        strOperation = "DELETE FROM DataTable ";
        DM->ADOQueryTemp->Close();
        if(iDeleteStyle      == 1)
        {
            DM->ADOQueryTemp->SQL->Clear();
            DM->ADOQueryTemp->SQL->Add(strOperation);
            DM->ADOQueryTemp->Prepared  = true;
            DM->ADOQueryTemp->ExecSQL();
            DM->ADOQueryTemp->Prepared  = false;

        }
        else if(iDeleteStyle == 2)
        {
            DM->ADOQueryTemp->SQL->Clear();
            DM->ADOQueryTemp->SQL->Add(strOperation);
            strOperation  = " where TrainNumber = '" + strTrainNumber + "' and TrainType = '";
            strOperation += strTrainType + "'";
            DM->ADOQueryTemp->SQL->Add(strOperation);
            DM->ADOQueryTemp->Prepared  = true;
            DM->ADOQueryTemp->ExecSQL();
            DM->ADOQueryTemp->Prepared  = false;

        }
        else if(iDeleteStyle == 3) //��ĳһ����ʱ�������ȫ��ɾ��
        {
            DM->ADOQueryTemp->SQL->Clear();
            DM->ADOQueryTemp->SQL->Add(strOperation);
            strOperation  = " where TrainNumber = '" +
                            strTrainNumber +
                            "' and TrainType = '";
            strOperation += strTrainType +
                            "' and MDate = :DeleteDate";

            DM->ADOQueryTemp->SQL->Add(strOperation);
            DM->ADOQueryTemp->Parameters->Clear();
            DM->ADOQueryTemp->Parameters->AddParameter();
            DM->ADOQueryTemp->Parameters->Items[0]->Name      = "DeleteDate";
            DM->ADOQueryTemp->Parameters->Items[0]->DataType  = ftDate;
            DM->ADOQueryTemp->Parameters->Items[0]->Direction = pdInput;

            unsigned short usYear, usMonth, usDate;
            GetDateFromString(strMDate,usYear,usMonth,usDate);
            TDate dtTemp(usYear,usMonth,usDate);
            DM->ADOQueryTemp->Parameters->Items[0]->Value = dtTemp;

            DM->ADOQueryTemp->Prepared  = true;
            DM->ADOQueryTemp->ExecSQL();
            DM->ADOQueryTemp->Prepared  = false;
        }
        else if(iDeleteStyle == 4)
        {
            DM->ADOQueryTemp->SQL->Clear();
            DM->ADOQueryTemp->SQL->Add(strOperation);
            strOperation  = " where TrainNumber = '" +
                            strTrainNumber +
                            "' and TrainType = '";
            strOperation += strTrainType +
                            "' and DieselSpeed = '" +
                            strDieselSpeed + "' and MDate = :DeleteDate";

            DM->ADOQueryTemp->SQL->Add(strOperation);
            DM->ADOQueryTemp->Parameters->Clear();
            DM->ADOQueryTemp->Parameters->AddParameter();
            DM->ADOQueryTemp->Parameters->Items[0]->Name      = "DeleteDate";
            DM->ADOQueryTemp->Parameters->Items[0]->DataType  = ftDate;
            DM->ADOQueryTemp->Parameters->Items[0]->Direction = pdInput;

            unsigned short usYear, usMonth, usDate;
            GetDateFromString(strMDate,usYear,usMonth,usDate);
            TDate dtTemp(usYear,usMonth,usDate);
            DM->ADOQueryTemp->Parameters->Items[0]->Value = dtTemp;

            DM->ADOQueryTemp->Prepared  = true;
            DM->ADOQueryTemp->ExecSQL();
            DM->ADOQueryTemp->Prepared  = false;

        }
        else if(iDeleteStyle == 5)
        {
            DM->ADOQueryTemp->SQL->Clear();
            DM->ADOQueryTemp->SQL->Add(strOperation);
            strOperation  = " where TrainNumber = '" +
                            strTrainNumber +
                            "' and TrainType = '";
            strOperation += strTrainType +
                            "' and DieselSpeed = '" +
                            strDieselSpeed + "' and MDate = :DeleteDate and bCombustion = '";
            strOperation += strCombustion + "'";
            DM->ADOQueryTemp->SQL->Add(strOperation);
            DM->ADOQueryTemp->Parameters->Clear();
            DM->ADOQueryTemp->Parameters->AddParameter();
            DM->ADOQueryTemp->Parameters->Items[0]->Name      = "DeleteDate";
            DM->ADOQueryTemp->Parameters->Items[0]->DataType  = ftDate;
            DM->ADOQueryTemp->Parameters->Items[0]->Direction = pdInput;
            unsigned short usYear, usMonth, usDate;
            GetDateFromString(strMDate,usYear,usMonth,usDate);
            TDate dtTemp(usYear,usMonth,usDate);
            DM->ADOQueryTemp->Parameters->Items[0]->Value = dtTemp;

            DM->ADOQueryTemp->Prepared  = true;
            DM->ADOQueryTemp->ExecSQL();
            DM->ADOQueryTemp->Prepared  = false;

        }

        DM->ADOQueryTemp->Close();

        if(iDeleteStyle > 1)
        {
            AnsiString strTempTT;
            mNode = FormTV->TreeViewDatabase->Selected;
            do
            {
                pNode = mNode->Parent;
                FormTV->TreeViewDatabase->Items->Delete(mNode);
                strTempTT = pNode->Text;
                if(strTempTT == "���ݿ�")
                {
                    break;
                }
                mNode = pNode;
                pNode = pNode->getFirstChild();
            }while(pNode == NULL);
            strTempTT = strExeFilePath + "TV.buf";
            FormTV->TreeViewDatabase->SaveToFile(strTempTT);
            bTVLoaded = true;
        }
        else
        {
            FormTV->TreeViewDatabase->Items->Clear();
            bTVLoaded = false;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmTVReloadClick(TObject *Sender)
{
    mnuDatabaseViewClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::DBGridDataInfoTitleClick(TColumn *Column)
{
    if( !DM->ADODataSetDataInfo->Active )
    {
        return;
    }
    AnsiString strTemp;
    AnsiString strTitle,strY;
    strTitle = Column->Title->Caption;
    strY     = Column->FieldName;
    float fX, fY;
    int i ;
    AnsiString strDesc = "���򣺽���";
    if(
          (strTitle == "����")
//        ||(strTitle == "����")
//        ||(strTitle == "�׺�")
//        ||(strTitle == "����ʱ��")
        ||(strTitle == "�����Ա")
//        ||(strTitle == "����")
        ||(strTitle == "����")
        ||(strTitle == "")
        )
    {
        return;
    }
    if(strTitle == strTitle_Backup)
    {
        DM->ADODataSetDataInfo->Sort = strY + " DESC";  //"LastName ASC, DateDue DESC"
        strTitle_Backup          = "";
    }
    else
    {
        DM->ADODataSetDataInfo->Sort = strY + " ASC";  //"LastName ASC, DateDue DESC"
        strTitle_Backup = strTitle;
        strDesc   = "��������";
    }

    if(CheckBoxShowOrder->Checked)
    {
//        int irecordnum;
        FormOrderInfo->ChartOrder->Series[0]->Clear();
//        irecordnum = ADODataSetDataInfo->RecordCount;  //���м�¼������
        FormOrderInfo->LabelOrder->Caption      =  strDesc;
        FormOrderInfo->LabelParameter->Caption  = AnsiString("������") + strTitle;

//        FormOrderInfo->SGOrder->RowCount    = irecordnum + 1;
        FormOrderInfo->SGOrder->Cells[0][0] = "���к�";    //strMTime;
        FormOrderInfo->SGOrder->Cells[0][1] = strTitle;  //strMTime;
        FormOrderInfo->SGOrder->Cells[0][2] = "�׺�";    //strMTime;
        DM->ADODataSetDataInfo->DisableControls();
        DM->ADODataSetDataInfo->First();
        for( i = 1; i <= 16; i ++)
        {
            for( int j = 0; j < 3; j++)
            {
                FormOrderInfo->SGOrder->Cells[i][j] = "";
            }
        }
        i = 1;
        while(!DM->ADODataSetDataInfo->Eof)
        {
            FormOrderInfo->SGOrder->Cells[i][0] = i;
            strTemp = DM->ADODataSetDataInfo->FieldByName(strY)->AsString;
            fY = StrToFloat(strTemp);
            FormOrderInfo->SGOrder->Cells[i][1] = strTemp;
            strTemp = DM->ADODataSetDataInfo->FieldByName("DieselNumber")->AsString;
            fX = (float)i ;
            FormOrderInfo->SGOrder->Cells[i][2] = strTemp;
//        strTemp = i;
            FormOrderInfo->ChartOrder->Series[0]->AddXY(fX,fY,strTemp,clTeeColor);
            i ++;
            DM->ADODataSetDataInfo->Next();
        }
        DM->ADODataSetDataInfo->First();
        DM->ADODataSetDataInfo->EnableControls();
        if(!FormOrderInfo->Visible)
        {
            FormOrderInfo->Show();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::DBGridDataInfoCellClick(TColumn *Column)
{
//   DBChart1->Series[ip]->XValues->ValueSource = "MDate";
    if( !DM->ADODataSetDataInfo->Active )
    {
        return;
    }
    AnsiString strTitle,strY;
    strTitle = Column->Title->Caption;
    strY     = Column->FieldName;
    if(   (strTitle == "����")
        ||(strTitle == "����")
        ||(strTitle == "�׺�")
        ||(strTitle == "����ʱ��")
        ||(strTitle == "�����Ա")
        ||(strTitle == "����")
        ||(strTitle == "����")
        ||(strTitle == "")
        )
    {
        FillSingleCompareChart(strY);
    }
    else
    {
    //TDBGrid   
        DBChart1->Title->Text->Strings[0]         = strTitle;
        DBChart1->Series[0]->YValues->ValueSource = strY;
        if(strTitle != "ת��")
        {
            if(CheckBoxCylinderCompare->Checked)
            {
                iTVCurrPosi = GetiCurrPosi(strY);
                if(iTVLastPosi != iTVCurrPosi)
                {
                    FillCompareChart(strY);
                }
                iTVLastPosi = iTVCurrPosi;
            }
        }
    }
}
//---------------------------------------------------------------------------


int __fastcall TFormMain::GetiCurrPosi(AnsiString stry)
{
    //TODO: Add your source code here
    int iret = 0;
    if( stry == "PressureMax")
    {
        iret = 1;
    }
    else if(   (stry == "DPDFMax")
             ||(stry == "CombustionPosi")
           )
    {
        iret = 2;
    }
    else if(   (stry == "DHDFMax")
             ||(stry == "CombustionPeriod")
           )
    {
        iret = 3;
    }
    else if(stry == "HeatAll")
    {
        iret = 4;
    }
    else if(stry == "CPower")
    {
        iret = 5;
    }
    else if(  (stry == "OilPosi")
            ||(stry == "WavePeriod")
            ||(stry == "WaveCoef")
           )
    {
        iret = 6;
    }
    return iret;
}

void __fastcall TFormMain::FillSingleCompareChart(AnsiString stry)
{
    //TODO: ����ͬ�׵���������Ա�ͼ��
    AnsiString strfileName;
    AnsiString strTitleTemp = "";
//    static int iCNBefore = 0;
    int dataNum = DM->ADODataSetDataInfo->RecordCount;
    int iistart, iiend;
    int iNumber = 0;
    int iDNNow,iDNBefore;
    // ������ͼ���
    for(int i = 0; i < 16; i ++)
    {
        ChartCurve->Series[i]->Clear();
    }
    if(FormEnlargeView->Visible)
    {
        FormEnlargeView->Visible = false;
    }

    if(dataNum > 0)
    {
        iDNBefore = DM->ADODataSetDataInfo->FieldByName("DieselNumber")->AsInteger;
        while(!DM->ADODataSetDataInfo->Bof)
        {
            DM->ADODataSetDataInfo->Prior();
            iDNNow = DM->ADODataSetDataInfo->FieldByName("DieselNumber")->AsInteger;
            if( iDNBefore != iDNNow)
            {
                DM->ADODataSetDataInfo->Next();
                break;
            }
        }

        while(!DM->ADODataSetDataInfo->Eof)
        {
            iDNNow  = DM->ADODataSetDataInfo->FieldByName("DieselNumber")->AsInteger;
            if(iDNBefore != iDNNow)
            {
                DM->ADODataSetDataInfo->Prior();
                break;
            }
            strPCPName = DM->ADODataSetDataInfo->FieldByName("FileName")->AsString;
            if(LoadCurrentBinPCP(true))  //����ͼ��ʱ��false
            {
                fdatax = lDS.fFFFai;
                fdatay = lDS.fNewP;
                for(int i = 10; i < lDS.iNormalLength - 10; i ++)
                {
                     ChartCurve->Series[iNumber]->AddXY(fdatax[i],fdatay[i],"",clTeeColor);
                }
                iNumber ++;
                fdatay = lDS.fpOilP;
                for(int i = 10; i < lDS.iNormalLength - 10; i ++)
                {
                     ChartCurve->Series[iNumber]->AddXY(fdatax[i],fdatay[i]*50.0/32768.0,"",clTeeColor);
                }
                iNumber ++;
                if(iNumber >= 16)
                {
                    break;
                }
            }
            DM->ADODataSetDataInfo->Next();
        }
        DM->ADODataSetDataInfo->EnableControls();
        iistart = lDS.iNormalLength * fTVStart ;
        iiend   = lDS.iNormalLength * fTVEnd ;
        if( fdatax != NULL )
        {
            if(iistart >= 0)
            {
                ChartCurve->BottomAxis->Minimum = fdatax[iistart];
            }
            if(iiend >= 1)
            {
                ChartCurve->BottomAxis->Maximum = fdatax[iiend - 1];
            }
        }
        strTitleTemp.printf("�� %d ������ѹ������ѹ�͹�ѹ������",iDNBefore);
        ChartCurve->Title->Text->Strings[0] = strTitleTemp;
    }
}

void __fastcall TFormMain::FillCompareChart(AnsiString stry)
{
    //TODO: Add your source code here
    AnsiString strfileName;
    AnsiString strTitleTemp = "�Ա�ͼ��";
    int iCNBefore = 0,iCNNow;
    int dataNum = DM->ADODataSetDataInfo->RecordCount;
    int iistart, iiend;

    // ������ͼ���
    for(int i = 0; i < 16; i ++)
    {
        ChartCurve->Series[i]->Clear();
    }
    for(int i = 0 ; i < 16; i ++)
    {
        FormEnlargeView->ChartEnlarge->Series[i]->Clear();
    }

    if(dataNum > 0)
    {
        DM->ADODataSetDataInfo->DisableControls();
        DM->ADODataSetDataInfo->First();
        while( !DM->ADODataSetDataInfo->Eof)
        {
            strPCPName = DM->ADODataSetDataInfo->FieldByName("FileName")->AsString;
/*
            if(!bParameterInited)
            {
                InitParameters();
                bParameterInited = true;
            }
*/
            if(LoadCurrentBinPCP(true))  //����ͼ��ʱ��false
            {
                iCNNow    = lDS.LFHS.siDieselNum;
                if(iCNNow != iCNBefore)     //�����ȣ���ø��ѻ��ƹ�
                {
                    fdatax = lDS.fFFFai;
                    if( stry == "PressureMax")
                    {
                        fdatay = lDS.fNewP;
                        strTitleTemp = "����ѹ�� /MPa";
                    }
                    else if(   (stry == "DPDFMax")
                             ||(stry == "CombustionPosi")
                           )
                    {
                        fdatay = lDS.farDPDF;
                        strTitleTemp = "ѹ�������� /MPa/CA";
                    }
                    else if(   (stry == "DHDFMax")
                             ||(stry == "CombustionPeriod")
                           )
                    {
                        fdatay = lDS.fDHDF;
                        strTitleTemp = "������ /kJ/CA";
                    }
                    else if(stry == "HeatAll")
                    {
                        fdatay = lDS.fCDHDF;
                        strTitleTemp = "ѭ������ /kJ";
                    }
                    else if(stry == "CPower")
                    {
                        fdatay = lDS.farW;
                        strTitleTemp = "ѭ���� /kJ";
                    }
                    else if(  (stry == "OilPosi")
                            ||(stry == "WavePeriod")
                            ||(stry == "WaveCoef")
                            )
                    {
                        fdatay = lDS.fpOilP;
                        strTitleTemp = "�͹�ѹ������";
                    }

                    for(int i = 0; i < lDS.iNormalLength; i += 5)
                    {
                        ChartCurve->Series[iCNNow - 1]->AddXY(fdatax[i],fdatay[i],"",clTeeColor);
                    }
                    if(FormEnlargeView->Visible)
                    {
                        for(int i = 0; i < lDS.iNormalLength; i += 5)
                        {
                             FormEnlargeView->ChartEnlarge->Series[iCNNow - 1]->AddXY(fdatax[i],fdatay[i],"",clTeeColor);
                        }
                    }
                    bChartFilled = true;
                    ChartCurve->Title->Text->Strings[0] = strTitleTemp + " �Ա�ͼ";

                }
            }
            iCNBefore = lDS.LFHS.siDieselNum;
            DM->ADODataSetDataInfo->Next();
        }
        DM->ADODataSetDataInfo->EnableControls();
        iistart = lDS.iNormalLength * fTVStart ;
        iiend   = lDS.iNormalLength * fTVEnd ;
        ChartCurve->BottomAxis->Minimum = fdatax[iistart];
        ChartCurve->BottomAxis->Maximum = fdatax[iiend - 1];
        if(FormEnlargeView->Visible)
        {
            FormEnlargeView->ChartEnlarge->BottomAxis->Minimum = fdatax[iistart];
            FormEnlargeView->ChartEnlarge->BottomAxis->Maximum = fdatax[iiend - 1];
        }
    }
}

void __fastcall TFormMain::ppmBrowseInsertToDatabaseClick(TObject *Sender)
{
    AnsiString FileStr;
    if(ListView1->Selected == NULL)
    {
        return;
    }
    FileStr = CurrentDir + "\\" + ListView1->Selected->Caption;
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;    // Show hourglass cursor
    try
    {
        Screen->Cursor = crHourGlass;
        MainInsertToDatabase( FileStr);
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton47Click(TObject *Sender)
{
    ((TToolButton*)Sender)->Down = true;
    mnuDatabaseViewClick(NULL);
}
//---------------------------------------------------------------------------




void __fastcall TFormMain::CheckBoxCylinderCompareClick(TObject *Sender)
{
    bool bChecked = CheckBoxCylinderCompare->Checked;
    ComboBoxEnlarge->Enabled = bChecked;
    if(bChecked)
    {
        ComboBoxEnlarge->ItemIndex = iEnlargeSize;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxEnlargeChange(TObject *Sender)
{
    iEnlargeSize = ComboBoxEnlarge->ItemIndex;
    switch(iEnlargeSize)
    {
        case 0:
        {
            fTVStart = 0.0;
            fTVEnd   = 1.0;
            break;
        }
        case 1:
        {
            fTVStart = 0.25;
            fTVEnd   = 0.75;
            break;
        }
        case 2:
        {
            fTVStart = 0.375;
            fTVEnd   = 0.625;
            break;
        }
        case 3:
        {
            fTVStart = 0.4375;
            fTVEnd   = 0.5625;
            break;
        }
    }

    if(bChartFilled)
    {
        int iistart,iiend;
        iistart = lDS.iNormalLength * fTVStart ;
        iiend   = lDS.iNormalLength * fTVEnd ;
        ChartCurve->BottomAxis->Minimum = fdatax[iistart];
        ChartCurve->BottomAxis->Maximum = fdatax[iiend - 1];
        if(FormEnlargeView->Visible)
        {
            FormEnlargeView->ChartEnlarge->BottomAxis->Minimum = fdatax[iistart];
            FormEnlargeView->ChartEnlarge->BottomAxis->Maximum = fdatax[iiend - 1];
            FormEnlargeView->ChartEnlarge->Refresh();
        }
    }
}
//---------------------------------------------------------------------------



bool __fastcall TFormMain::SetDieselInfo(int itype,bool bFromDatabase)
{
    //TODO: Add your source code here
    bool bDatabase = false;
    if(!bFromDatabase)
    {
        if(lDS.LFHS.siInfoValid == 1)
        {
            SetTrainType(itype);
            lDS.SetfDiameter      (lDS.LFHS.fDiameter     );  //1 ����ֱ��
            lDS.SetfStroke        (lDS.LFHS.fStroke       );  //2 �������
            lDS.SetfPodLength     (lDS.LFHS.fPodLength    );  //3 ���˳���
            lDS.SetfCRatio        (lDS.LFHS.fCRatio       );  //4 ѹ����
            lDS.SetfLMDa          (lDS.LFHS.fLMDa         );  //5 ���/����֮��
            lDS.SetfInOpenAngle   (lDS.LFHS.fInOpenAngle  );  //6 �����ſ����Ƕ�
            lDS.SetfInCloseAngle  (lDS.LFHS.fInCloseAngle );  //7�������ŹرսǶ�
            lDS.SetfOutOpenAngle  (lDS.LFHS.fOutOpenAngle );  //8�������ſ����Ƕ�
            lDS.SetfOutCloseAngle (lDS.LFHS.fOutCloseAngle);  //9�������ŹرսǶ�
        }
        else
        {
            bDatabase = true;
        }
    }

    if( bFromDatabase || bDatabase )
    {
        SetTrainType(itype);
        AnsiString strTemp;
        strTemp =  "select * from TypeInfo where DieselType = '";
        strTemp += strTrainType + "'";
        DM->ADOQueryTemp->Close();
        DM->ADOQueryTemp->SQL->Clear();
        DM->ADOQueryTemp->SQL->Add(strTemp);
        DM->ADOQueryTemp->Open();
        if( DM->ADOQueryTemp->RecordCount == 0)
        {
            return false;
        }
        else
        {
            float ftemp,sl,pl;

            ftemp = DM->ADOQueryTemp->FieldByName("DCylinderDiameter")->AsFloat/1000.0;
            lDS.SetfDiameter(ftemp);        //1 ����ֱ��

            ftemp = DM->ADOQueryTemp->FieldByName("DPistonStrock")->AsFloat/1000.0;
            lDS.SetfStroke(ftemp);          //2 �������
            sl    = ftemp;

            ftemp = DM->ADOQueryTemp->FieldByName("DPodLength")->AsFloat/1000.0;
            lDS.SetfPodLength(ftemp);       //3 ���˳���
            pl    = ftemp;

            ftemp = DM->ADOQueryTemp->FieldByName("DCompressRatio")->AsFloat;
            lDS.SetfCRatio(ftemp);          //4 ѹ����

            ftemp = sl / pl;
            lDS.SetfLMDa(ftemp);            //5 ���/����֮��

            ftemp = DM->ADOQueryTemp->FieldByName("DInletOpen")->AsFloat;
            ftemp = -ftemp;                 //6 �����ſ����Ƕ�
            lDS.SetfInOpenAngle(ftemp);

            ftemp = DM->ADOQueryTemp->FieldByName("DInletClose")->AsFloat;
            ftemp -= 180.0;                 //7�������ŹرսǶ�
            lDS.SetfInCloseAngle(ftemp);

            ftemp = DM->ADOQueryTemp->FieldByName("DOutletOpen")->AsFloat;
            ftemp = 180.0 - ftemp;          //8�������ſ����Ƕ�
            lDS.SetfOutOpenAngle(ftemp);

            ftemp = DM->ADOQueryTemp->FieldByName("DOutletClose")->AsFloat;
            lDS.SetfOutCloseAngle(ftemp);   //9�������ŹرսǶ�
        }
        DM->ADOQueryTemp->First();
        DM->ADOQueryTemp->Close();
    }
    return true;
}


void __fastcall TFormMain::InitDieselInfo(void)
{
    //TODO: Add your source code here
// �����������õĲ��ͻ��ṹ�����ø�����ϵͳ
    float ftemp;
    float fSLength;
    float fPodLength;
// �����ǲ��ͻ��ṹ���������ڴ˴�����
//  1 ����ֱ��
    if(EditDiameter->Text == "")
    {
        ftemp = 0.24;
    }
    else
    {
        ftemp = StrToFloat(EditDiameter->Text)/1000.0;
    }
    lDS.SetfDiameter(ftemp);

//  2 �������
    if(EditSLength->Text == "")
    {
        ftemp = 0.275;
    }
    else
    {
        ftemp = StrToFloat(EditSLength->Text)/1000.0;
    }
    fSLength = ftemp;
    lDS.SetfStroke(ftemp);

//  3�����˳���
    if(EditPodLength->Text == "")
    {
        ftemp = 0.58;
    }
    else
    {
        ftemp = StrToFloat(EditPodLength->Text)/1000.0;
    }
    fPodLength = ftemp;
    lDS.SetfPodLength(ftemp);

//  4 ѹ����
    if(EditCRatio->Text == "")
    {
        ftemp = 12.4;
    }
    else
    {
        ftemp = StrToFloat(EditCRatio->Text);
    }
    lDS.SetfCRatio(ftemp);

//  5  ��������˳���֮��
    ftemp = fSLength / fPodLength;
    lDS.SetfLMDa(ftemp);

//  �����ŹرյĽǶ�,���������ֹ���ĽǶ�,��һ������������ʱΪ��ֹ��ǰ������Ϊ��ֵ
//  6�������ſ����Ƕ�
    if(EditInOpenAngle->Text == "")
    {
        ftemp = 50.0;
    }
    else
    {
        ftemp = StrToFloat(EditInOpenAngle->Text);
    }
    ftemp = -ftemp;
    lDS.SetfInOpenAngle(ftemp);

//  7 �����ŹرսǶ�
    if(EditInCloseAngle->Text == "")
    {
        ftemp = 42;
    }
    else
    {
        ftemp = StrToFloat(EditInCloseAngle->Text);
    }
    ftemp -= 180.0;
    lDS.SetfInCloseAngle(ftemp);

//  8 �����Ŵ򿪽Ƕ�
    if(EditOutOpenAngle->Text == "")
    {
        ftemp = 50.0;
    }
    else
    {
        ftemp = StrToFloat(EditOutOpenAngle->Text);
    }
    ftemp = 180.0 - ftemp;
    lDS.SetfOutOpenAngle(ftemp);

//  9�������ŹرսǶ�
    if(EditOutCloseAngle->Text == "")
    {
        ftemp = 50.0;
    }
    else
    {
        ftemp = StrToFloat(EditOutCloseAngle->Text);
    }
    lDS.SetfOutCloseAngle(ftemp);
}

void __fastcall TFormMain::InitEngineSpeed(float fen)
{
    //TODO: Add your source code here
    //�ӽ��������ͻ�ת�٣������õ�����ϵͳ
}



void __fastcall TFormMain::Panel6DblClick(TObject *Sender)
{
    if(bLoadOriginal)   //����Ѵ�ԭʼ����
    {
        ppmShowOriginalAll->Checked = !ppmShowOriginalAll->Checked;
        bDataShowAll                =  ppmShowOriginalAll->Checked;
        if( bDataShowAll)
        {
            iOriginalStart = 0;
            iOriginalEnd   = lDS.iOriginalLength;
        }
        else
        {
            iOriginalStart = 0;
            iOriginalEnd   = lDS.iAbstractLength;
        }

        iOriginalEnd_Backup = iOriginalEnd;
        iChoiceStart = 0;
        iChoiceEnd   = lDS.iPLength;
        bFiltered    = false;
        PaintBoxOriginal->Refresh();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmChangeClick(TObject *Sender)
{
    int iCount;
    AnsiString strTemp;
    if(   (PageControlMain->ActivePageIndex == 4)
        &&(PageControlCurve->ActivePageIndex == 0)
      )
    {
        strTemp    = FileListBoxFile->FileName;
        strTemp    = ExtractFileName(strTemp);
        strTemp    = strTemp.SubString(1,6);
        iTrainType = GetiTrainType(strTemp);
        SetDieselInfo(iTrainType, false);
        
        iCount = FileListBoxFile->Items->Count;
        strPCPName = FileListBoxFile->FileName;
        if(( iCount > 0 )&&(strPCPName != ""))
        ChangeToTxt();
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ChangeToTxt(void)
{
    //TODO: Add your source code here
    FILE* fptxt;
    int iposi;
    AnsiString strTemp,strFore;
    LoadCurrentBinPCP(false);    //��������
    strTemp = strPCPName;
    iposi   = strTemp.Pos(".");
    strFore = strTemp.SubString(1,iposi);
    strFore += ".TXT";
    if((fptxt = fopen(strFore.c_str(),"w")) != NULL)
    {
        fprintf(fptxt,"ת��   ��ѹ   ѹ����   ��ѹ  ������   ������   �ۻ���  ɢ����\n");
        for(int i = 0; i < lDS.iNormalLength; i ++)
        {
            fprintf(fptxt,"%.1f %.2f %.4f %d %.2f %.2f %.2f %.2f\n",
                                  lDS.fFFFai[i],          //����ת��
                                  lDS.fNewP[i],           //����ѹ��
                                  lDS.farDPDF[i],         //ѹ��������
                                  (int)lDS.fpOilP[i],     //�͹�ѹ��

                                  lDS.fDHDF[i],           //������
                                  lDS.fCDHDF[i],          //�ۻ�������
                                  lDS.farW[i],            //�ۻ���
                                  lDS.farQW[i]            //ɢ����
                    );
        }
        fclose(fptxt);
    }
}

void __fastcall TFormMain::Button3Click(TObject *Sender)
{
    if(Button3->Caption == "��������")
    {
        Button3->Caption = "����";
        ButtonPerson->Caption = "�����Ա";
        if(DM->ADOTable2->Active)
        {
            DM->ADOTable2->Close();
        }
        PageControl2->ActivePageIndex = 1;

        if(!DM->ADOTable1->Active)
        {
            DM->ADOTable1->Open();
        }
    }
    else
    {
        Button3->Caption = "��������";
        if(DM->ADOTable1->Active)
        {
            DM->ADOTable1->Close();
        }
        PageControl2->ActivePageIndex = 0;
    }
}
//---------------------------------------------------------------------------




void __fastcall TFormMain::ppmOutOrderClick(TObject *Sender)
{
    DM->ADODataSetDataInfo->Sort = "";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CheckBoxShowOrderClick(TObject *Sender)
{
    if(CheckBoxShowOrder->Checked)
    {
        if(!FormOrderInfo->Visible)
        {
            FormOrderInfo->Show();
        }
    }
    else
    {
        if(FormOrderInfo->Visible)
        {
            FormOrderInfo->Hide();
        }
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::FillmyDetectResult(bool bFromDatabase)
{
    //TODO: Add your source code here
//bool bFromDatabase ;  true: �����ݿ��ж����ݣ�
//                      false:���ļ��ж�����

    AnsiString strfileName;
    AnsiString strTitleTemp = "�Ա�ͼ��";
    AnsiString strCombus;
//    int iCNBefore = 0;
    int iCNNow;
    int iItem;
    int dataNum = DM->ADODataSetDataInfo->RecordCount;
    int iistart, iiend;
    for(int i = 0 ; i < 16; i ++)
    {
        myDetect[i].bDataValid = false;
        myDetect[i].bComValid  = false;
    }
    if(dataNum > 0)
    {
        DM->ADODataSetDataInfo->DisableControls();
        DM->ADODataSetDataInfo->First();
        while( !DM->ADODataSetDataInfo->Eof)
        {
            if(bFromDatabase)   //��ʾ���Ǵ����ݿ��ж�ȡ����
            {
                iCNNow    = DM->ADODataSetDataInfo->FieldByName("DieselNumber")->AsInteger;
                iItem     = iCNNow - 1;
                strCombus = DM->ADODataSetDataInfo->FieldByName("bCombustion")->AsString;

                if(
                      (!myDetect[iItem].bComValid )  //����˸׵�ѹ�����ݻ�û������
                    &&( strCombus == "ѹ��" )        //����˹�����ѹ������
                  )
                {
                    myDetect[iItem].iDieselNum   = iCNNow;        //��Ϊ���ͻ��׺�
                    myDetect[iItem].bComValid    = true;
                    myDetect[iItem].fComPressure = DM->ADODataSetDataInfo->FieldByName("PressureMax")->AsFloat;
                }

                if(
                      ( !myDetect[iItem].bDataValid)  //����˸׵�ȼ�����ݻ�û������
                    &&( strCombus == "ȼ��" )         //����˹�����ȼ�չ���
                  )
                {
                        myDetect[iItem].bDataValid         = true;     //��һ�����Ƿ�

                        myDetect[iItem].iDieselNum         = iCNNow;   //���ͻ��׺�
                        myDetect[iItem].fDieselSpeed
                                       = DM->ADODataSetDataInfo->FieldByName("DieselSpeed")->AsFloat;         //���ͻ�ת��

                        myDetect[iItem].fCPMax
                                       = DM->ADODataSetDataInfo->FieldByName("PressureMax")->AsFloat;         //���ѹ��
                        myDetect[iItem].fDPDFMax
                                       = DM->ADODataSetDataInfo->FieldByName("DPDFMax")->AsFloat; //ѹ�����������ֵ

                        myDetect[iItem].fInletPressure
                                       = DM->ADODataSetDataInfo->FieldByName("fP0")->AsFloat;         //��������ѹ��

                        myDetect[iItem].fOilAngle
                                       = DM->ADODataSetDataInfo->FieldByName("OilPosi")->AsFloat;         //������ǰ��
                        myDetect[iItem].fOilPeak
                                       = DM->ADODataSetDataInfo->FieldByName("fOilMax")->AsFloat;         //������ֵ
                        myDetect[iItem].fOilPeriod
                                       = DM->ADODataSetDataInfo->FieldByName("WavePeriod")->AsFloat;         //���ͳ�����
                        myDetect[iItem].fOilCoef
                                       = DM->ADODataSetDataInfo->FieldByName("WaveCoef")->AsFloat;         //����ϵ��

                        myDetect[iItem].fCombustionAngle
                                       = DM->ADODataSetDataInfo->FieldByName("CombustionPosi")->AsFloat;         //ȼ��ʼ��
                        myDetect[iItem].fCombustionPeriod
                                       = DM->ADODataSetDataInfo->FieldByName("CombustionPeriod")->AsFloat;         //ȼ�ճ���ʱ��
                        myDetect[iItem].fDHDFMax
                                       = DM->ADODataSetDataInfo->FieldByName("DHDFMax")->AsFloat;         //���������ֵ
                        myDetect[iItem].fHAll
                                       = DM->ADODataSetDataInfo->FieldByName("HeatAll")->AsFloat;         //�ۻ�������
                        myDetect[iItem].fCPower
                                       = DM->ADODataSetDataInfo->FieldByName("CPower")->AsFloat;         //ѭ����
                        myDetect[iItem].strResult          = "";       //��������
                    }
            }
            else                //ֻ�Ǵ����ݿ��л�ȡ�ļ�����Ȼ������ļ�������
            {
                strPCPName = DM->ADODataSetDataInfo->FieldByName("FileName")->AsString;
                if(LoadCurrentBinPCP(true))  //����ͼ��ʱ��false
                {
                    iCNNow    = lDS.LFHS.siDieselNum;
                    iItem     = iCNNow - 1;
                    if( !myDetect[iItem].bDataValid)  //����˸׵������������
                    {
                        myDetect[iItem].bDataValid         = true;     //��һ�����Ƿ�

                        myDetect[iItem].iDieselNum         = iCNNow;   //���ͻ��׺�
                        myDetect[iItem].fDieselSpeed       = lDS.fEngineSpeed;         //���ͻ�ת��

                        myDetect[iItem].fCPMax             = lDS.fCPMax;         //���ѹ��
                        myDetect[iItem].fDPDFMax           = lDS.fDPDFMax;         //ѹ�����������ֵ
                        myDetect[iItem].fInletPressure     = lDS.fBarP0;         //��������ѹ��

                        myDetect[iItem].fOilAngle          = lDS.fOilAngle;         //������ǰ��
                        myDetect[iItem].fOilPeak           = lDS.LFHS.fOilPeak;         //������ֵ
                        myDetect[iItem].fOilPeriod         = lDS.LFHS.fWavePeriod;         //���ͳ�����
                        myDetect[iItem].fOilCoef           = lDS.LFHS.fWaveCoef;         //����ϵ��

                        myDetect[iItem].fCombustionAngle   = lDS.fCombustionAngle;         //ȼ��ʼ��
                        myDetect[iItem].fCombustionPeriod  = lDS.fCombustionPeriod;         //ȼ�ճ���ʱ��
                        myDetect[iItem].fDHDFMax           = lDS.fHRRMax;         //���������ֵ
                        myDetect[iItem].fHAll              = lDS.fCHRRMax;         //�ۻ�������
                        myDetect[iItem].fCPower            = lDS.fCPower;         //ѭ����
                        myDetect[iItem].strResult          = "";       //��������
                    }
                }
          //      iCNBefore = lDS.LFHS.siDieselNum;
            }
            DM->ADODataSetDataInfo->Next();
        }
        DM->ADODataSetDataInfo->First();
        DM->ADODataSetDataInfo->EnableControls();
    }
}

void __fastcall TFormMain::AnalysisOilPeriod(bool bShow)
{
    //TODO: Add your source code here
// �������ͽǶ�
   AnsiString strTemp;

    int i;
    float fmin, fmax, faver, fdev, fsum, ftemp;
    int   imin, imax;
    int   iLNum[16];
    int   iHNum[16];
    int   iallNum = 0;
    int   iLowNum = 0;
    int   iHighNum = 0;
    float fLow, fHigh;
    if(bShow)
    {
        MemoResult->Lines->Add("");
        MemoResult->Lines->Add("===================================");
        MemoResult->Lines->Add("=    ****���ͳ����Ƕȷ���***       =");
        MemoResult->Lines->Add("===================================");
        MemoResult->Lines->Add("  �׺� ���ͳ����Ƕ�(CA)  ��С?������?");
    }
    for(i = 0; i < 16; i ++)
    {
        if(myDetect[i].bDataValid)
        {
            fmin = myDetect[i].fOilPeriod;
            fmax = myDetect[i].fOilPeriod;
            imin = i;
            imax = i;
            break;
        }
    }
    for(i++ ; i < 16; i ++)
    {
        if(myDetect[i].bDataValid)
        {
            if(fmin > myDetect[i].fOilPeriod)
            {
               fmin = myDetect[i].fOilPeriod;
               imin = i;
            }
            else if(fmax < myDetect[i].fOilPeriod)
            {
               fmax = myDetect[i].fOilPeriod;
               imax = i;
            }
        }

    }

    faver = 0.0;
    for(i = 0 ; i < 16; i ++)
    {
        if(myDetect[i].bDataValid)
        {
            iallNum ++;
            faver += myDetect[i].fOilPeriod;
        }
    }

    if(iallNum > 0)
    {
        faver /= (float)iallNum;
        fsum   = 0.0;
        for(i = 0 ; i < 16; i ++)
        {
            if(myDetect[i].bDataValid)
            {
                ftemp =  myDetect[i].fOilPeriod - faver;
                fsum  += ftemp*ftemp;
            }
        }
        fsum = sqrt(fsum/(float)iallNum);

        for(i = 0 ; i < 16; i ++)
        {

            if(myDetect[i].bDataValid)
            {
                if(myDetect[i].fDieselSpeed < 500.0)
                {
                    fLow =  (faver - 1.5*fsum) > 6.0? 6.0: (faver - 1.5*fsum);
                    fHigh = (faver + 1.5*fsum);
                }
                else
                {
                    fLow  = faver - 1.5*fsum;
                    fHigh = faver + 1.5*fsum;
                }

                if(  (myDetect[i].fOilPeriod < fLow) //���ѹ��С��2.96MPa����Ϊѹ��̫��
                  )
                {
                    myDetect[i].siOilPeriod = -1;    //1������0����������1����С
                    iLNum[iLowNum] = i;
                    iLowNum ++;
                    strTemp.printf("   %-2d   %8.2f         ��",
                        myDetect[i].iDieselNum,
                        myDetect[i].fOilPeriod
                          );
                }
                else if(  (myDetect[i].fOilPeriod > fHigh) //���ѹ��С��2.96MPa����Ϊѹ��̫��
                  )
                {
                    myDetect[i].siOilPeriod = 1;    //1������0����������1����С
                    iHNum[iHighNum] = i;
                    iHighNum ++;
                    strTemp.printf("   %-2d   %8.2f               ��",
                        myDetect[i].iDieselNum,
                        myDetect[i].fOilPeriod
                          );
                }
                else
                {
                    myDetect[i].siOilPeriod = 0;    //1������0����������1����С

                    strTemp.printf("   %-2d   %8.2f",
                        myDetect[i].iDieselNum,
                        myDetect[i].fOilPeriod
                          );
                }
                if(bShow)
                {
                    MemoResult->Lines->Add(strTemp);
                }
            }
        }
    }

    if(iLowNum > 0)
    {
        if(bShow)
        {
            MemoResult->Lines->Add("");
            strTemp = "  ���У����ͳ����ǶȽ�С���У�";
            MemoResult->Lines->Add(strTemp);
            strTemp = "    ";
            for( i = 0; i < iLowNum; i ++)
            {
                strTemp += AnsiString(myDetect[iLNum[i]].iDieselNum) + ", ";
            }
            MemoResult->Lines->Add(strTemp);
        }
    }
    if( iHighNum > 0)
    {
        if(bShow)
        {
            MemoResult->Lines->Add("");
            strTemp = "  ���ͳ����ǶȽϴ���У�";
            MemoResult->Lines->Add(strTemp);
            strTemp = "    ";
            for( i = 0; i < iHighNum; i ++)
            {
                strTemp += AnsiString(myDetect[iHNum[i]].iDieselNum) + ", ";
            }
            MemoResult->Lines->Add(strTemp);
        }
    }
    if(bShow)
    {
            MemoResult->Lines->Add("");
            strTemp.printf(" ���׹��ͳ����ǶȾ�ֵΪ��%.2f",faver);
            MemoResult->Lines->Add( strTemp );
            strTemp.printf(" ���׹��ͳ����ǶȲ�ֵΪ:%.2f (��� - ��С)", fmax - fmin);
            MemoResult->Lines->Add( strTemp );
            strTemp.printf("  ���ֵ��%.2f CA���� %d ��",fmax, myDetect[imax].iDieselNum);
            MemoResult->Lines->Add(strTemp);
            strTemp.printf("  ��Сֵ��%.2f CA���� %d ��",fmin, myDetect[imin].iDieselNum);
            MemoResult->Lines->Add(strTemp);
            MemoResult->Lines->Add("=====================================");
    }
}

void __fastcall TFormMain::AnalysisOilAngle(bool bShow)
{
// �������ͽǶ�
   AnsiString strTemp;
   int iLowNum  = 0;
   int iLNum[16];
   int iHighNum = 0;
   int iHNum[16];

   bool  bHave = false;
    if(bShow)
    {
        MemoResult->Lines->Add("");
        MemoResult->Lines->Add("===================================");
        MemoResult->Lines->Add("=      ****���Ϳ�ʼ�Ƕȷ���***     =");
        MemoResult->Lines->Add("===================================");
        MemoResult->Lines->Add("  �׺� ���Ϳ�ʼ�Ƕ�(CA)  ����?����С?");
    }
//--------------------------------------------------------------------------------
    for( int i = 0 ; i < 16; i ++)
    {
        if(myDetect[i].bDataValid)    //���ѹ��ѹ����־��Ч
        {
            bHave = true;
            strTemp = "";
            if( myDetect[i].fOilAngle > 24.0 )
            {
                myDetect[i].siOilAngle = 1;     //1������0����������1����С
                iLNum[iLowNum] = i;
                iLowNum ++;
                strTemp.printf("   %-2d   %7.2f         �� ",
                                        myDetect[i].iDieselNum,
                                        -myDetect[i].fOilAngle);
            }
            else if(myDetect[i].fOilAngle < 20.0)    //��������Ϊ��������
            {
                myDetect[i].siOilAngle = -1;     //1������0����������1����С
                iHNum[iHighNum] = i;
                iHighNum ++;
                strTemp.printf("   %-2d   %7.2f               �� ",
                                        myDetect[i].iDieselNum,
                                        -myDetect[i].fOilAngle);
            }
            else
            {
                myDetect[i].siOilAngle = 0;     //1������0����������1����С
                strTemp.printf("   %-2d   %7.2f",
                                        myDetect[i].iDieselNum,
                                        -myDetect[i].fOilAngle);
            }
            if(bShow)
            {
                MemoResult->Lines->Add(strTemp);
            }
        }
    }
    if(!bHave)
    {
        MemoResult->Lines->Add(" ���������!");
    }
    else
    {
        if( iLowNum > 0)
        {
            MemoResult->Lines->Add("");
            MemoResult->Lines->Add("  ���У�������ǰ�Ǵ���У�");
            strTemp = "   ";
            for( int i =0; i < iLowNum; i ++)
            {
                strTemp += AnsiString( myDetect[iLNum[i]].iDieselNum ) + ", ";
            }
            MemoResult->Lines->Add(strTemp);
        }
        if(iHighNum > 0)
        {
            MemoResult->Lines->Add("");
            MemoResult->Lines->Add("  ������ǰ��С���У�");
            strTemp = "   ";
            for( int i =0; i < iHighNum; i ++)
            {
                strTemp += AnsiString( myDetect[iHNum[i]].iDieselNum ) + ", ";
            }
            MemoResult->Lines->Add(strTemp);
        }
    }
    MemoResult->Lines->Add("=========================================");
}

void __fastcall TFormMain::AnalysisMiFengXing(bool bShow)
{
    //TODO: Add your source code here
// ���������ܷ���
   AnsiString strTemp;
   int iLowNum  = 0;
   int iLNum[16];
   int iHighNum = 0;
   int iHNum[16];

   bool  bHave = false;
    if(bShow)
    {
        MemoResult->Lines->Add("");
        MemoResult->Lines->Add("===================================");
        MemoResult->Lines->Add("=       ***�����ܷ��Է���***       =");
        MemoResult->Lines->Add("===================================");
        MemoResult->Lines->Add("  �׺� ѹ��ѹ��(MPa)  ����?������?");
    }
//--------------------------------------------------------------------------------
    for( int i = 0 ; i < 16; i ++)
    {
        if(myDetect[i].bComValid)    //���ѹ��ѹ����־��Ч
        {
            bHave = true;
            strTemp = "";
            if(myDetect[i].fComPressure < 2.6)  //��һ��ѹ��ѹ��̫����
            {
                iLNum[iLowNum] = i;
                iLowNum ++;
                myDetect[i].siComPressure = -1;  //ѹ��ѹ����1������0����������1����С
                strTemp.printf("  %-2d   %7.2f      �� ",
                                        myDetect[i].iDieselNum,
                                        myDetect[i].fComPressure);
            }
            else if(myDetect[i].fComPressure > 2.91)    //��������Ϊ��������
            {
                myDetect[i].siComPressure = 1;  //ѹ��ѹ����1������0����������1����С
                iHNum[iHighNum] = i;
                iHighNum ++;
                strTemp.printf("  %-2d   %7.2f            �� ",
                                        myDetect[i].iDieselNum,
                                        myDetect[i].fComPressure);
            }
            else
            {
                myDetect[i].siComPressure = 0;  //ѹ��ѹ����1������0����������1����С
                strTemp.printf("  %-2d   %7.2f",
                                        myDetect[i].iDieselNum,
                                        myDetect[i].fComPressure);
            }
            if(bShow)
            {
                MemoResult->Lines->Add(strTemp);
            }
        }
    }
    if(!bHave)
    {
        MemoResult->Lines->Add(" ���������!");
    }
    else
    {
        if( iLowNum > 0)
        {
            MemoResult->Lines->Add("");
            MemoResult->Lines->Add("  ���У�ѹ��ѹ���͵��У�");
            strTemp = "   ";
            for( int i =0; i < iLowNum; i ++)
            {
                strTemp += AnsiString( myDetect[iLNum[i]].iDieselNum ) + ", ";
            }
            MemoResult->Lines->Add(strTemp);
        }
        if(iHighNum > 0)
        {
            MemoResult->Lines->Add("");
            MemoResult->Lines->Add("  ѹ��ѹ���ߵ��У�");
            strTemp = "   ";
            for( int i =0; i < iHighNum; i ++)
            {
                strTemp += AnsiString( myDetect[iHNum[i]].iDieselNum ) + ", ";
            }
            MemoResult->Lines->Add(strTemp);
        }
    }
    MemoResult->Lines->Add("=========================================");
}


void __fastcall TFormMain::AnalysisCombustionStatus(bool bShow)
{
    //TODO: Add your source code here
    AnsiString strTemp;
//ȼ��ѹ�������Է���
    int i;
    float fmin, fmax, faver, fdev, fsum, ftemp;
    int   imin, imax;
    int   iLNum[16];
    int   iHNum[16];
    int   iallNum = 0;
    int   iLowNum = 0;
    int   iHighNum = 0;
    float fLow, fHigh;
    if(bShow)
    {
        MemoResult->Lines->Add("================================");
        MemoResult->Lines->Add("=     ****���׾����Է���***     =");
        MemoResult->Lines->Add("================================");
        MemoResult->Lines->Add("  �׺�   ����ѹ��    ����?��������?");
    }
    for(i = 0; i < 16; i ++)
    {
        if(myDetect[i].bDataValid)
        {
            fmin = myDetect[i].fCPMax;
            fmax = myDetect[i].fCPMax;
            imin = i;
            imax = i;
            break;
        }
    }
    for(i++ ; i < 16; i ++)
    {
        if(myDetect[i].bDataValid)
        {
            if(fmin > myDetect[i].fCPMax)
            {
               fmin = myDetect[i].fCPMax;
               imin = i;
            }
            else if(fmax < myDetect[i].fCPMax)
            {
               fmax = myDetect[i].fCPMax;
               imax = i;
            }
        }

    }

    faver = 0.0;
    for(i = 0 ; i < 16; i ++)
    {
        if(myDetect[i].bDataValid)
        {
            iallNum ++;
            faver += myDetect[i].fCPMax;
        }
    }

    if(iallNum > 0)
    {
        faver /= (float)iallNum;
        fsum   = 0.0;
        for(i = 0 ; i < 16; i ++)
        {
            if(myDetect[i].bDataValid)
            {
                ftemp =  myDetect[i].fCPMax - faver;
                fsum  += ftemp*ftemp;
            }
        }
        fsum = sqrt(fsum/(float)iallNum);

        for(i = 0 ; i < 16; i ++)
        {

            if(myDetect[i].bDataValid)
            {
                if(myDetect[i].fDieselSpeed < 500.0)
                {
                    fLow =  (faver - 1.5*fsum) > 2.96? 2.96: (faver - 1.5*fsum);
                    fHigh = (faver + 1.5*fsum);
                }
                else
                {
                    fLow  = faver - 0.3;
                    fHigh = faver + 0.3;
                }

                if(  (myDetect[i].fCPMax < fLow) //���ѹ��С��2.96MPa����Ϊѹ��̫��
                  )
                {
                    myDetect[i].siPMax = -1;         //����ѹ����1������0����������1����С
                    iLNum[iLowNum] = i;
                    iLowNum ++;
                    strTemp.printf("   %-2d   %8.2f     ��",
                        myDetect[i].iDieselNum,
                        myDetect[i].fCPMax
                          );
                }
                else if(  (myDetect[i].fCPMax > fHigh) //���ѹ��С��2.96MPa����Ϊѹ��̫��
                  )
                {
                    myDetect[i].siPMax = 1;         //����ѹ����1������0����������1����С
                    iHNum[iHighNum] = i;
                    iHighNum ++;
                    strTemp.printf("   %-2d   %8.2f             ��",
                                             myDetect[i].iDieselNum,
                                             myDetect[i].fCPMax
                          );
                }
                else
                {
                    myDetect[i].siPMax = 0;         //����ѹ����1������0����������1����С
                    strTemp.printf("   %-2d   %8.2f",
                        myDetect[i].iDieselNum,
                        myDetect[i].fCPMax
                          );
                }
                if(bShow)
                {
                    MemoResult->Lines->Add(strTemp);
                }
            }
        }
    }

    if(iLowNum > 0)
    {
        if(bShow)
        {
            MemoResult->Lines->Add("");
            strTemp = "  ���У�ȼ��ѹ�����͵��У�";
            MemoResult->Lines->Add(strTemp);
            strTemp = "    ";
            for( i = 0; i < iLowNum; i ++)
            {
                strTemp += AnsiString(myDetect[iLNum[i]].iDieselNum) + ", ";
            }
            MemoResult->Lines->Add(strTemp);
        }
    }
    if( iHighNum > 0)
    {
        if(bShow)
        {
            MemoResult->Lines->Add("");
            strTemp = "  ȼ��ѹ�����ߵ��У�";
            MemoResult->Lines->Add(strTemp);
            strTemp = "    ";
            for( i = 0; i < iHighNum; i ++)
            {
                strTemp += AnsiString(myDetect[iHNum[i]].iDieselNum) + ", ";
            }
            MemoResult->Lines->Add(strTemp);
        }
    }
    if(bShow)
    {
            MemoResult->Lines->Add("");
            strTemp.printf(" ����ѹ����ֵΪ��%.2f",faver);
            MemoResult->Lines->Add( strTemp );
            strTemp.printf(" ����ѹ��Ϊ:%.2f (����ѹ - ��С��ѹ)", fmax - fmin);
            MemoResult->Lines->Add( strTemp );
            strTemp.printf("  ���ֵ��%.2fMPa���� %d ��",fmax, myDetect[imax].iDieselNum);
            MemoResult->Lines->Add(strTemp);
            strTemp.printf("  ��Сֵ��%.2fMPa���� %d ��",fmin, myDetect[imin].iDieselNum);
            MemoResult->Lines->Add(strTemp);
        MemoResult->Lines->Add("=====================================");
    }
}



void __fastcall TFormMain::AnalysisCPower(bool bShow)
{
    //TODO: ����������������
    AnsiString strTemp;
//ȼ��ѹ�������Է���
    int i;
    float fmin, fmax, faver, fdev, fsum, ftemp;
    int   imin, imax;
    int   iLNum[16];
    int   iHNum[16];
    int   iallNum = 0;
    int   iLowNum = 0;
    int   iHighNum = 0;
    float fLow, fHigh;
    if(bShow)
    {
        MemoResult->Lines->Add("================================");
        MemoResult->Lines->Add("=   ***���׶�����������***     =");
        MemoResult->Lines->Add("================================");
        MemoResult->Lines->Add("  �׺�   ��������(kW) ����?��������?");
    }
    for(i = 0; i < 16; i ++)
    {
        if(myDetect[i].bDataValid)
        {
            fmin = myDetect[i].fCPower;
            fmax = myDetect[i].fCPower;
            imin = i;
            imax = i;
            break;
        }
    }
    for(i++ ; i < 16; i ++)
    {
        if(myDetect[i].bDataValid)
        {
            if(fmin > myDetect[i].fCPower)
            {
               fmin = myDetect[i].fCPower;
               imin = i;
            }
            else if(fmax < myDetect[i].fCPower)
            {
               fmax = myDetect[i].fCPower;
               imax = i;
            }
        }

    }

    faver = 0.0;
    for(i = 0 ; i < 16; i ++)
    {
        if(myDetect[i].bDataValid)
        {
            iallNum ++;
            faver += myDetect[i].fCPower;
        }
    }

    if(iallNum > 0)
    {
        faver /= (float)iallNum;
        fsum   = 0.0;
        for(i = 0 ; i < 16; i ++)
        {
            if(myDetect[i].bDataValid)
            {
                ftemp =  myDetect[i].fCPower - faver;
                fsum  += ftemp*ftemp;
            }
        }
        fsum = sqrt(fsum/(float)iallNum);

        for(i = 0 ; i < 16; i ++)
        {

            if(myDetect[i].bDataValid)
            {
                if(myDetect[i].fDieselSpeed < 500.0)
                {
                    fLow =  (faver - 1.5*fsum) > 2.96? 2.96: (faver - 1.5*fsum);
                    fHigh = (faver + 1.5*fsum);
                }
                else
                {
                    fLow  = faver - 1.5*fsum;
                    fHigh = faver + 1.5*fsum;
                }

                if(  (myDetect[i].fCPower < fLow) //���ѹ��С��2.96MPa����Ϊѹ��̫��
                  )
                {
                    myDetect[i].siPower = -1;        //����������1������0����������1����С

                    iLNum[iLowNum] = i;
                    iLowNum ++;
                    strTemp.printf("   %-2d   %8.2f       ��",
                                       myDetect[i].iDieselNum,
                                       myDetect[i].fCPower
                                   );
                }
                else if(  (myDetect[i].fCPower > fHigh) //���ѹ��С��2.96MPa����Ϊѹ��̫��
                  )
                {
                    myDetect[i].siPower = 1;        //����������1������0����������1����С
                    iHNum[iHighNum] = i;
                    iHighNum ++;
                     strTemp.printf("   %-2d   %8.2f              ��",
                                        myDetect[i].iDieselNum,
                                        myDetect[i].fCPower
                          );
                }
                else
                {
                    myDetect[i].siPower = 0;        //����������1������0����������1����С
                    strTemp.printf("   %-2d   %8.2f",
                        myDetect[i].iDieselNum,
                        myDetect[i].fCPower
                          );
                }
                if(bShow)
                {
                    MemoResult->Lines->Add(strTemp);
                }
            }
        }
    }

    if(iLowNum > 0)
    {
        if(bShow)
        {
            MemoResult->Lines->Add("");
            strTemp = "  ���У������������ٵ��У�";
            MemoResult->Lines->Add(strTemp);
            strTemp = "    ";
            for( i = 0; i < iLowNum; i ++)
            {
                strTemp += AnsiString(myDetect[iLNum[i]].iDieselNum) + ", ";
            }
            MemoResult->Lines->Add(strTemp);
        }
    }
    if( iHighNum > 0)
    {
        if(bShow)
        {
            MemoResult->Lines->Add("");
            strTemp = "  ���������϶���У�";
            MemoResult->Lines->Add(strTemp);
            strTemp = "    ";
            for( i = 0; i < iHighNum; i ++)
            {
                strTemp += AnsiString(myDetect[iHNum[i]].iDieselNum) + ", ";
            }
            MemoResult->Lines->Add(strTemp);
        }
    }
    if(bShow)
    {
            MemoResult->Lines->Add("");
            strTemp.printf("  ���׶���������ֵΪ��%.2f",faver);
            MemoResult->Lines->Add( strTemp );
            strTemp.printf("  ���׶���������ֵΪ:%.2f (������� - ��С����)", fmax - fmin);
            MemoResult->Lines->Add( strTemp );
            strTemp.printf("  ���ֵ��%.2fkW���� %d ��",fmax, myDetect[imax].iDieselNum);
            MemoResult->Lines->Add(strTemp);
            strTemp.printf("  ��Сֵ��%.2fkW���� %d ��",fmin, myDetect[imin].iDieselNum);
            MemoResult->Lines->Add(strTemp);
        MemoResult->Lines->Add("=====================================");
    }
}

void __fastcall TFormMain::AnalysismyDetectResult(bool bShow, int iItem)
{
   AnsiString strTemp;
   strTemp.printf("�� %02d ��",myDetect[iItem].iDieselNum );
   MemoResult->Lines->Add(strTemp);

//--------------------------------------------------------------------------------
   if(myDetect[iItem].bComValid)    //���ѹ��ѹ����־��Ч
   {
//(1)   �ж�ѹ��ѹ���Ƿ�����
        strTemp = "";
        if(myDetect[iItem].fComPressure < 2.6)  //��һ��ѹ��ѹ��̫����
        {
            strTemp.printf("  * ѹ��ѹ����%.2fMPa, ���ͣ�����(1)����λ��! (2)������!* ",
                                        myDetect[iItem].fComPressure);
        }
        else                                    //��������Ϊ��������
        {
            strTemp.printf("  * ѹ��ѹ����%.2fMPa, ����!*",
                                        myDetect[iItem].fComPressure);
        }
        if(bShow)
        {
            MemoResult->Lines->Add(strTemp);
        }
        myDetect[iItem].strResult += strTemp;
    }
//======================================================================================

//---------------------------------------------------------------------------------------
//(2) �жϹ�����ǰ��������
    if(myDetect[iItem].bDataValid)
    {
//--------------------------------------------------------------------------------------
        strTemp = "";
        if( myDetect[iItem].fOilAngle < 20.0 )
        {
            strTemp.printf("  * ������λ��-%.1f CA, �ͺ���%.1f CA * \n",
                                                  myDetect[iItem].fOilAngle,
                                                  (22.0 - myDetect[iItem].fOilAngle)
                          );
        }
        else if( myDetect[iItem].fOilAngle > 24.0 )
        {
            strTemp.printf("  * ������λ��-%.1f CA, ��ǰ��%.1f CA * \n",
                                                  myDetect[iItem].fOilAngle,
                                                  myDetect[iItem].fOilAngle - 22.0
                          );
        }
        else
        {
            strTemp.printf("  * ������λ��-%.1f CA, ���� * ",   myDetect[iItem].fOilAngle);
        }
        myDetect[iItem].strResult = strTemp;
        if(bShow)
        {
            MemoResult->Lines->Add(strTemp);
        }
        myDetect[iItem].strResult += strTemp;
//========================================================================================

//---------------------------------------------------------------------------------------
//(3) �жϹ���ϵͳ�Ƿ�����
        strTemp = "";
        if(myDetect[iItem].fDieselSpeed < 500.0)
        {
            if( myDetect[iItem].fOilPeriod > 10.0 )
            {
                strTemp.printf("  * ���ͳ����Ƕȣ� %.1f CA, ̫�� * \n",
                                                  myDetect[iItem].fOilPeriod
                          );
            }
            else if( myDetect[iItem].fOilAngle < 4.0 )
            {
                strTemp.printf("  * ���ͳ����Ƕȣ� %.1f CA, ̫�� * \n",
                                                  myDetect[iItem].fOilPeriod
                          );
            }
            else
            {
                strTemp.printf("  * ���ͳ����Ƕȣ� %.1f CA, ���� * ",   myDetect[iItem].fOilPeriod);
            }
            myDetect[iItem].strResult = strTemp;
            if(bShow)
            {
                MemoResult->Lines->Add(strTemp);
            }
            myDetect[iItem].strResult += strTemp;
        }

    }
//========================================================================================
/*
    if(myDetect[iItem].bDataValid)
    {
//--------------------------------------------------------------------------------------
//(2) �͸���ȼ��ʱ�ĸ��׾�����
        strTemp = "";
        if(myDetect[iItem].fDieselSpeed < 500.0)
        {

            if( myDetect[iItem].fCPMax < 3.0 )
            {
                strTemp.printf("  * ȼ��ѹ����%.2f MPa, ��С * \n",
                                                  myDetect[iItem].fCPMax
                          );
            }
            else if( myDetect[iItem].fCPMax > 4.0 )
            {
                strTemp.printf("  * ȼ��ѹ����%.2f MPa, ���� * \n",
                                                  myDetect[iItem].fCPMax
                              );
            }
            else
            {
                strTemp.printf("  * ȼ��ѹ����%.2f MPa, ���� * ",
                          myDetect[iItem].fCPMax);
            }
        }
        myDetect[iItem].strResult = strTemp;
        if(bShow)
        {
            MemoResult->Lines->Add(strTemp);
        }
        myDetect[iItem].strResult += strTemp;
    }
//========================================================================================
*/

/*
        myDetect[iItem].iDieselNum         = iCNNow;   //���ͻ��׺�
        myDetect[iItem].fDieselSpeed       = lDS.fEngineSpeed;         //���ͻ�ת��

        myDetect[iItem].fCPMax             = lDS.fCPMax;         //���ѹ��
        myDetect[iItem].fDPDFMax           = lDS.fDPDFMax;         //ѹ�����������ֵ
        myDetect[iItem].fInletPressure     = lDS.fBarP0;         //��������ѹ��

        myDetect[iItem].fOilAngle          = lDS.fOilAngle;         //������ǰ��
        myDetect[iItem].fOilPeak           = lDS.LFHS.fOilPeak;         //������ֵ
        myDetect[iItem].fOilPeriod         = lDS.LFHS.fWavePeriod;         //���ͳ�����
        myDetect[iItem].fOilCoef           = lDS.LFHS.fWaveCoef;         //����ϵ��

        myDetect[iItem].fCombustionAngle   = lDS.fCombustionAngle;         //ȼ��ʼ��
        myDetect[iItem].fCombustionPeriod  = lDS.fCombustionPeriod;         //ȼ�ճ���ʱ��
        myDetect[iItem].fDHDFMax           = lDS.fHRRMax;         //���������ֵ
        myDetect[iItem].fHAll              = lDS.fCHRRMax;         //�ۻ�������
        myDetect[iItem].fCPower            = lDS.fCPower;         //ѭ����
        myDetect[iItem].strResult          = "";       //��������
*/
}
void __fastcall TFormMain::AnalysisAll(bool bShow)
{
    //TODO: �Ը��׵ķ��������ܽ�

    AnsiString strTemp = "���ͣ�";
    if(bShow)
    {
        MemoResult->Lines->Add("");
        MemoResult->Lines->Add("============================================");
        strTemp += strTrainType;
        strTemp += "�� ���ţ�";
        strTemp += strTrainNumber;
        strTemp += "��";
        MemoResult->Lines->Add(strTemp);
        MemoResult->Lines->Add(" ���׽���������£�");
        MemoResult->Lines->Add(" �׺� ѹ��ѹ�� ����ѹ�� �������� ���ͽǶ� �����Ƕ� ");
    }
    for(int i = 0; i < 16; i++)
    {
        strTemp = "";
        if(  (myDetect[i].bDataValid)
           &&(myDetect[i].bComValid)
          )
        {
            strTemp.printf("  %-2d  %6.2f  %6.2f  %6.1f  %6.1f  %6.1f",
                              myDetect[i].iDieselNum,
                              myDetect[i].fComPressure,
                              myDetect[i].fCPMax,
                              myDetect[i].fCPower,
                              myDetect[i].fOilAngle,
                              myDetect[i].fOilPeriod
                              );
        }
        else if( myDetect[i].bDataValid)
        {
            strTemp.printf("  %-2d          %6.2f  %6.1f  %6.1f  %6.1f",
                              myDetect[i].iDieselNum,
                          //    myDetect[i].fComPressure,
                              myDetect[i].fCPMax,
                              myDetect[i].fCPower,
                              myDetect[i].fOilAngle,
                              myDetect[i].fOilPeriod
                              );
        }
        else if( myDetect[i].bComValid)
        {
            strTemp.printf("  %-2d  %6.2f",
                              myDetect[i].iDieselNum,
                              myDetect[i].fComPressure
                          );
        }
        if(bShow)
        {
            MemoResult->Lines->Add(strTemp);
        }
    }
    if(bShow)
    {
        MemoResult->Lines->Add("--------------------------------------------");
        MemoResult->Lines->Add("");
        MemoResult->Lines->Add(" ���׽���������£�");
        MemoResult->Lines->Add(" �׺� ѹ��ѹ�� ����ѹ�� �������� ���ͽǶ� �����Ƕ� ");
    }
    for(int i = 0; i < 16; i++)
    {
        strTemp = "";
        if(  (myDetect[i].bDataValid)
           &&(myDetect[i].bComValid)
          )
        {
            strTemp.printf("  %-2d  %6d  %6d  %6d  %6d  %6d",
                              myDetect[i].iDieselNum,
                              myDetect[i].siComPressure,
                              myDetect[i].siPMax,
                              myDetect[i].siPower,
                              myDetect[i].siOilAngle,
                              myDetect[i].siOilPeriod
                              );
        }
        else if( myDetect[i].bDataValid)
        {
            strTemp.printf("  %-2d          %6d  %6d  %6d  %6d",
                              myDetect[i].iDieselNum,
                      //        myDetect[i].siComPressure,
                              myDetect[i].siPMax,
                              myDetect[i].siPower,
                              myDetect[i].siOilAngle,
                              myDetect[i].siOilPeriod
                              );
        }
        else if( myDetect[i].bComValid)
        {
            strTemp.printf("  %-2d  %6d",
                              myDetect[i].iDieselNum,
                              myDetect[i].siComPressure
                          );
        }
        if(bShow)
        {
            MemoResult->Lines->Add(strTemp);
        }
    }
    if(bShow)
    {
        MemoResult->Lines->Add("============================================");
    }
}

void __fastcall TFormMain::AnalysisResultAll(bool bShow)
{
    //TODO: Add your source code here
    MemoResult->Lines->Add("");
    AnsiString strTemp = "���ͣ�";
    MemoResult->Lines->Clear();     //��������ʾ����
    strTemp += strTrainType;
    strTemp += "; ���ţ�";
    strTemp += strTrainNumber;
    MemoResult->Lines->Add(strTemp);

    AnalysisMiFengXing( bShow );           //1:���������ܷ���
    if( strCombustion != "ѹ��")
    {
        AnalysisCombustionStatus(bShow);   //2����������ȼ�վ�����
        AnalysisCPower(bShow);             //3���������׶����������
        AnalysisOilAngle(   bShow );       //4���������׹��Ϳ�ʼ�Ƕ�
        AnalysisOilPeriod(  bShow );       //5���������׹��ͳ���ʱ��
    }

    AnalysisAll(bShow);                    //6���ۺϷ���
}

int __fastcall TFormMain::NormalizeSpeed(int itraintype, float fee)
{
    //TODO: Add your source code here
//����8B������11����400,440,480,520,560,600,640,680,720,760,800,840,880,920,960,1000
//����4A��4B��4C��4D��7C�Ȼ������±�����
//
    int ispeed;
    if(
          (itraintype == 4)
        ||(itraintype == 5)
      )
    {
        if     ( fee < 420.0) { ispeed = 400; }
        else if( fee < 460.0) { ispeed = 440; }
        else if( fee < 500.0) { ispeed = 480; }
        else if( fee < 540.0) { ispeed = 520; }
        else if( fee < 580.0) { ispeed = 560; }
        else if( fee < 620.0) { ispeed = 600; }
        else if( fee < 660.0) { ispeed = 640; }
        else if( fee < 700.0) { ispeed = 680; }
        else if( fee < 740.0) { ispeed = 720; }
        else if( fee < 780.0) { ispeed = 760; }
        else if( fee < 820.0) { ispeed = 800; }
        else if( fee < 860.0) { ispeed = 840; }
        else if( fee < 900.0) { ispeed = 880; }
        else if( fee < 940.0) { ispeed = 920; }
        else if( fee < 980.0) { ispeed = 960; }
        else                  { ispeed = 1000;}
    }
    else
    {
        if     ( fee < 480.0) { ispeed = 430; }
        else if( fee < 530.0) { ispeed = 480; }
        else if( fee < 580.0) { ispeed = 530; }
        else if( fee < 630.0) { ispeed = 580; }
        else if( fee < 680.0) { ispeed = 630; }
        else if( fee < 730.0) { ispeed = 680; }
        else if( fee < 780.0) { ispeed = 730; }
        else if( fee < 830.0) { ispeed = 780; }
        else if( fee < 880.0) { ispeed = 830; }
        else if( fee < 930.0) { ispeed = 880; }
        else if( fee < 980.0) { ispeed = 930; }
        else                  { ispeed = 1000; }

    }
    return ispeed;
}




void __fastcall TFormMain::ppmViewListClick(TObject *Sender)
{
    AnsiString strTemp,strFName;
    strFName = FileListBoxFile->FileName;
    strTemp = ::ExtractFileExt(strFName);
    strTemp = strTemp.UpperCase();
    if( strTemp == ".PST")
    {
        try
        {
            ::ShellExecute(Handle,"open","c:\\windows\\Notepad.exe",strFName.c_str(),NULL,SW_SHOWNORMAL);
        }
        catch(...)
        {
        }
    }
}
//---------------------------------------------------------------------------







void __fastcall TFormMain::ToolBar1Click(TObject *Sender)
{
    FormEngine->Show();    
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ppmMemoPrintClick(TObject *Sender)
{
//  TMemo
//    MemoResult->Lines->
}
//---------------------------------------------------------------------------







void __fastcall TFormMain::ppmEditPSTClick(TObject *Sender)
{
     AnsiString FileStr,strTemp;
    if(ListView1->Selected == NULL)
    {
        return;
    }
    FileStr = CurrentDir + "\\" + ListView1->Selected->Caption;
    strTemp = ExtractFileExt(FileStr);
    strTemp = strTemp.UpperCase();
    if(  (strTemp == ".PST")
       ||(strTemp == ".TXT")
       ||(strTemp == ".DAT"))
    {
        try
        {
            ::ShellExecute(Handle,"open","c:\\windows\\Notepad.exe",FileStr.c_str(),NULL,SW_SHOWNORMAL);
        }
        catch(...)
        {
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::mnuHelpInstrumentClick(TObject *Sender)
{
    AnsiString strTemp;
    bool bnotepad;
    String strExeFileName = ::ParamStr(0);
    String strNowPath     = ::ExtractFilePath(strExeFileName);
    strNowPath           += "jl902_Help.chm";       //�����ļ�����
    Application->HelpFile = strNowPath;
//    Application->HelpCommand(HELP_CONTENTS,0);
    HtmlHelpA(
                 GetDesktopWindow(),
                 strNowPath.c_str(),
                 HH_DISPLAY_TOPIC,
                 NULL
                 ) ;
/*
    strTemp = "c:\\windows\\hh.exe";
    bnotepad = FileExists(strTemp);
    if(bnotepad)
    {
        try
        {
            ::ShellExecute(Handle,"open",strTemp.c_str(),strNowPath.c_str(),NULL,SW_SHOWNORMAL);
        }
        catch(...)
        {
        }
    }
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmEditListClick(TObject *Sender)
{
    int iret;
    int iIndex,iCount;
    iIndex =  FileListBoxFile->ItemIndex;
    iCount = FileListBoxFile->Items->Count;
    if( (iIndex<0) || (iIndex>iCount) )
    {
        Application->MessageBox("����ѡ���б��ļ�!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    AnsiString strTemp,strFName;
    strFName = FileListBoxFile->FileName;
    strTemp = ::ExtractFileExt(strFName);
    strTemp = strTemp.UpperCase();
    if( strTemp == ".PST")
    {
        try
        {
            ::ShellExecute(Handle,"open","c:\\windows\\Notepad.exe",strFName.c_str(),NULL,SW_SHOWNORMAL);
        }
        catch(...)
        {
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::GetListPageNum(void)
{
    //TODO: Add your source code here
    AnsiString strTemp;
    int iposi;
    
    if( mnuTraceNum02->Checked )
    {
        iListPageNum = 2;
    }
    else if(mnuTraceNum04->Checked )
    {
        iListPageNum = 4;
    }
    else if(mnuTraceNum08->Checked)
    {
        iListPageNum = 8;
    }
    else if(mnuTraceNum16->Checked)
    {
        iListPageNum = 16;
    }
    else
    {
        iListPageNum = 8;
//        mnuTraceNum08->Checked = true;
//        ppmTraceNum08->Checked = true;
    }
}

void __fastcall TFormMain::DrawListDirect(void)
{
    //TODO: Add your source code here
    bLoadList                         = true;
    PageControlMain->ActivePageIndex  = 4;
    PageControlCurve->ActivePageIndex = 2;
    FormMain->Caption = strSoftTitle
                       + AnsiString(" - ")
                       + strPSTName;//    PaintBoxAll->Refresh();
    DrawImageAll();
}





void __fastcall TFormMain::SetMenuChecked(int iposi)
{
    //TODO: Add your source code here
    if(iposi == 2)
    {
        mnuTraceNum02->Checked = true;
        ppmTraceNum02->Checked = true;
        mnuTraceNum04->Checked = false;
        ppmTraceNum04->Checked = false;
        mnuTraceNum08->Checked = false;
        ppmTraceNum08->Checked = false;
        mnuTraceNum16->Checked = false;
        ppmTraceNum16->Checked = false;
    }
    else if(iposi == 4)
    {
        mnuTraceNum02->Checked = false;
        ppmTraceNum02->Checked = false;
        mnuTraceNum04->Checked = true;
        ppmTraceNum04->Checked = true;
        mnuTraceNum08->Checked = false;
        ppmTraceNum08->Checked = false;
        mnuTraceNum16->Checked = false;
        ppmTraceNum16->Checked = false;
    }
    else if(iposi == 8)
    {
        mnuTraceNum02->Checked = false;
        ppmTraceNum02->Checked = false;
        mnuTraceNum04->Checked = false;
        ppmTraceNum04->Checked = false;
        mnuTraceNum08->Checked = true;
        ppmTraceNum08->Checked = true;
        mnuTraceNum16->Checked = false;
        ppmTraceNum16->Checked = false;
    }
    else if(iposi == 16)
    {
        mnuTraceNum02->Checked = false;
        ppmTraceNum02->Checked = false;
        mnuTraceNum04->Checked = false;
        ppmTraceNum04->Checked = false;
        mnuTraceNum08->Checked = false;
        ppmTraceNum08->Checked = false;
        mnuTraceNum16->Checked = true;
        ppmTraceNum16->Checked = true;
    }
}





void __fastcall TFormMain::Splitter9Moved(TObject *Sender)
{
    DriveComboBox1->Width = GroupBox20->Width - 16;
    FilterComboBoxFile->Width = DriveComboBox1->Width;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ShowCompareList(int inum, bool bAlone)
{
    // bAlone = false; ȼ��ѹ����ѹ��ѹ��һ��Ա�
    // bAlong = true;  ����ȼ��ѹ����ѹ��ѹ��   inum = 0ʱ�Ա�ѹ��ѹ��
    //                                          inum = 1ʱ�Ա�ȼ��ѹ��
    //TODO: Add your source code here
    AnsiString strtemp;
    int iselected;
    int iposi;
    int iCount;
    if( FilterComboBoxFile->ItemIndex != 2)
    {
        FilterComboBoxFile->ItemIndex = 2;
        FileListBoxFile->Mask = FilterComboBoxFile->Mask;
        FileListBoxFile->Update();
//        Application->MessageBox("����ѡ���б��ļ�����","��ʾ",MB_OK|MB_ICONINFORMATION);
//        return;
    }
    if(bAlone)
    {
        if(inum == 1)   //��ȼ��ѹ�����жԱ�
        {
            iCount = FileListBoxFile->Items->Count;
            iselected = 0;
            for( int i = 0; i < iCount; i++)
            {
                strtemp = FileListBoxFile->Items->Strings[i];
                iposi = strtemp.Pos("_ȼ��");
                if( iposi > 0)
                {
                   strSelectListName[iselected] = FileListBoxFile->Directory
                                                + AnsiString("\\")
                                                + FileListBoxFile->Items->Strings[i];
                    strValidPath = FileListBoxFile->Directory + AnsiString("\\");
                    iselected ++;
                }
            }
            if(iselected == 0)
            {
                strtemp.printf("�б��ļ���û�пɹ��Աȵ�ȼ��ѹ������!!!");
                Application->MessageBox(strtemp.c_str(),"��ʾ",MB_OK|MB_ICONINFORMATION);
                return;
            }
        }
        else            //��ѹ��ѹ�����жԱ�
        {
            iCount = FileListBoxFile->Items->Count;
            iselected = 0;
            for( int i = 0; i < iCount; i++)
            {
                strtemp = FileListBoxFile->Items->Strings[i];
                iposi = strtemp.Pos("_ѹ��");
                if( iposi > 0)
                {
                   strSelectListName[iselected] = FileListBoxFile->Directory
                                                + AnsiString("\\")
                                                + FileListBoxFile->Items->Strings[i];
                    strValidPath = FileListBoxFile->Directory + AnsiString("\\");
                    iselected ++;
                }
            }
            if(iselected == 0)
            {
                strtemp.printf("�б��ļ���û�пɹ��Աȵ�ѹ��ѹ������!!!");
                Application->MessageBox(strtemp.c_str(),"��ʾ",MB_OK|MB_ICONINFORMATION);
                return;
            }
        }
        inum = iselected;
        if(iselected == 1) //�������һ���ɹ��Ա�
        {
            bPSTChanged = true;
            strPSTName = strSelectListName[0];
            ListBoxPCPFile->Items->LoadFromFile(strPSTName);
            bLoadList = true;
        }
    }
    else
    {
        iCount = FileListBoxFile->Items->Count;
        if(iCount < inum)
        {
            strtemp.printf("�б��ļ������� %d ��\n\n�޷���ʾ!!!",inum);
            Application->MessageBox(strtemp.c_str(),"��ʾ",MB_OK|MB_ICONINFORMATION);
            return;
        }

        for(int i = 0; i < inum; i ++)
        {
            strSelectListName[i] = FileListBoxFile->Directory
                             + AnsiString("\\")
                             + FileListBoxFile->Items->Strings[iCount - inum + i];
            strValidPath = FileListBoxFile->Directory + AnsiString("\\");
        }

    }

//    strValidPath = FileListBoxFile->Directory + AnsiString("\\");
    ListBoxPST->Items->LoadFromFile(strSelectListName[0]);
    iChoicedNum = ListBoxPST->Items->Count;
/*
    strtemp = strSelectListName[0];
    strSelectListName[1] = FileListBoxFile->Directory
                     + AnsiString("\\")
                    + FileListBoxFile->Items->Strings[iCount - 1];
    strtemp = strSelectListName[1];
*/
    iListDrawKind = inum;   //����������εĲ������
    iListPage = 0;
    GetListPageNum();
    bLoadList  = true;
    bPSTOpened = true;
    if(iChoiceEnd == 0) //��ʾ���ݻ�û�г�ʼ��
    {
        iChoiceEnd = 3600;
    }
    DrawListDirect();
}










void __fastcall TFormMain::ActionTraceTwoExecute(TObject *Sender)
{
    ShowCompareList(2 , false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionTraceThreeExecute(TObject *Sender)
{
    ShowCompareList(3, false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionTraceFourExecute(TObject *Sender)
{
    ShowCompareList(4, false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionTraceFiveExecute(TObject *Sender)
{
    ShowCompareList(5, false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionTraceSixExecute(TObject *Sender)
{
    ShowCompareList(6, false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionTraceSevenExecute(TObject *Sender)
{
    ShowCompareList(7, false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionTraceEightExecute(TObject *Sender)
{
    ShowCompareList(8, false);
}
//---------------------------------------------------------------------------








void __fastcall TFormMain::ToolButton37Click(TObject *Sender)
{

    if(iListDrawKind > 1)
    {
        fDrawConst -= 0.1;
        if(fDrawConst < 0.5)
        {
            fDrawConst = 0.5;
        }
        DrawListDirect();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButton29Click(TObject *Sender)
{
    if(iListDrawKind > 1)
    {
       fDrawConst += 0.1;
        if(fDrawConst > 2.0)
        {
            fDrawConst = 2.0;
        }
        DrawListDirect();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionEnlargeTwoExecute(TObject *Sender)
{
    if(bLoadList)
    {
        iChoiceStart = lDS.iPLength/4;
        iChoiceEnd   = lDS.iPLength*3/4;
        fstartrt     = 1.0/4.0;
        fendrt       = 3.0/4.0;
        DrawImageAll();
//        PaintBoxAll->Refresh();
    }    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionEnlargeFourExecute(TObject *Sender)
{
    if(bLoadList)
    {
        iChoiceStart = lDS.iPLength*3/8;
        iChoiceEnd   = lDS.iPLength*5/8;
        fstartrt     = 3.0/8.0;
        fendrt       = 5.0/8.0;
        DrawImageAll();
//        PaintBoxAll->Refresh();
    }    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionEnlargeEightExecute(TObject *Sender)
{
    if(bLoadList)
    {
        iChoiceStart = lDS.iPLength*7/16;
        iChoiceEnd   = lDS.iPLength*9/16;
        fstartrt     = 7.0/16.0;
        fendrt       = 9.0/16.0;
        DrawImageAll();
//        PaintBoxAll->Refresh();
    }    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionEnlargeNormalExecute(TObject *Sender)
{
    if(bLoadList)
    {
        iChoiceStart = 0;
        iChoiceEnd   = lDS.iPLength;
        fstartrt     = 0;
        fendrt       = 1.0;
        DrawImageAll();
//        PaintBoxAll->Refresh();
    }
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::mnuOtherOptionClick(TObject *Sender)
{
    AnsiString strTemp;
    FormOption = new TFormOption(this);
    if( NULL != FormOption)
    {
        FormOption->CheckBoxExcel->Checked      = bExcel;
        FormOption->RadioGroupAnalysisMethod->ItemIndex = bAnalysisMethod;

        FormOption->EditUserName->Text          = strUserTitle;
        FormOption->CheckBoxbSaveOrig->Checked  = bSaveOrig;
        FormOption->SetDieselInfo(myDieselInfo,      11);
        FormOption->SetDieselName(strArrayTrainType, 5);
        FormOption->RefreshControl(iNowSelectTrainType, true);               //ˢ�¿ؼ���ʾ
        FormOption->RefreshDetectInfo(iNowDetectInfo,   true);

        if(FormOption->ShowModal() == mrOk)
        {
            strUserTitle = FormOption->EditUserName->Text;
            bExcel       = FormOption->CheckBoxExcel->Checked;
            bAnalysisMethod  =        FormOption->RadioGroupAnalysisMethod->ItemIndex;
            bSaveOrig    = FormOption->CheckBoxbSaveOrig->Checked;
            CheckBoxbSaveOrig->Checked = bSaveOrig;
            FormOption->GetDieselInfo(myDieselInfo,      11);
            FormOption->GetDieselName(strArrayTrainType, 5);
            strTemp = FormOption->EditDetectLevel->Text;
            Params_Save();
        }
        delete FormOption;
    }
    else
    {
        Application->MessageBox("�޷�����!","��ʾ",MB_OK|MB_ICONINFORMATION);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButtonBasicInfoClick(TObject *Sender)
{
// AllDrawed()  �����ж��Ƿ��Ѿ������˻�ͼ�����ѻ�ͼ���˳������ٻ���

    if(AllDrawed(Sender,drawInfo))
    {
        return;
    }
    TCursor Save_Cursor = Screen->Cursor ;
    try
    {

        Screen->Cursor = crHourGlass;
// DetectPSTStatus()�����ж��Ƿ���PST�б��ļ����򿪣���û������ʾ���б��ļ�
        if(!DetectPSTStatus())
        {
            return;
        }
        if(iListDrawKind != 1)
        {
            return;
        }
        bFileChanged = true;
//        PaintBoxAll->Refresh();
        DrawImageAll();
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ppmSetupFontClick(TObject *Sender)
{
    TFontStyles TFS;

    FontDialog1->Font->Name = strBasicFontName;
    FontDialog1->Font->Size = iBasicFontSize;
    FontDialog1->Font->Color = clBasicFSColor;
    if(bBasicFSBold)
    {
        TFS<<fsBold;
    }
    if(bBasicFSItalic)
    {
        TFS<<fsItalic;
    }
    if(bBasicFSUnderline)
    {
        TFS<<fsUnderline;
    }
    if(bBasicFSStrikeOut)
    {
        TFS<<fsStrikeOut;
    }
    FontDialog1->Font->Style = TFS;
    if(FontDialog1->Execute())
    {
        strBasicFontName = FontDialog1->Font->Name;
        iBasicFontSize   = FontDialog1->Font->Size;
        TFS              = FontDialog1->Font->Style;

        if(TFS.Contains(fsBold))    {  bBasicFSBold = true; }
        else                        {  bBasicFSBold = false;}

        if(TFS.Contains(fsItalic))  {  bBasicFSItalic = true; }
        else                        {  bBasicFSItalic = false;}

        if(TFS.Contains(fsUnderline))    {  bBasicFSUnderline = true; }
        else                             {  bBasicFSUnderline = false;}

        if(TFS.Contains(fsStrikeOut))    {  bBasicFSStrikeOut = true; }
        else                             {  bBasicFSStrikeOut = false;}
        clBasicFSColor = FontDialog1->Font->Color;
        if(AllDrawKind == drawInfo)
        {
            DrawImageAll();
        }
    }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


void __fastcall TFormMain::Timer1Timer(TObject *Sender)
{
    StatusBarMain->Panels->Items[2]->Text="����ʱ���ǣ�" + TimeToStr(Time());
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonPersonClick(TObject *Sender)
{
    if(ButtonPerson->Caption == "�����Ա")
    {
        ButtonPerson->Caption = "����";
        Button3->Caption      = "��������";
        if(DM->ADOTable1->Active)
        {
            DM->ADOTable1->Close();
        }
        PageControl2->ActivePageIndex = 2;
        if(!DM->ADOTable2->Active)
        {
            DM->ADOTable2->Open();
        }
    }
    else
    {
        ButtonPerson->Caption = "�����Ա";
        if(DM->ADOTable2->Active)
        {
            DM->ADOTable2->Close();
        }
        PageControl2->ActivePageIndex = 0;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmLoadPicClick(TObject *Sender)
{
    if(DM->ADOTable2->Active)
    {
        AnsiString strpicname;
        OpenPictureDialog1->InitialDir = strPresetPath;
        OpenPictureDialog1->Title      = "��BMPͼƬ�ļ� *.QAD";
        OpenPictureDialog1->Filter     = "BMPͼƬ�ļ� *.BMP|*.BMP";
        if(OpenPictureDialog1->Execute())
        {
            strpicname = OpenPictureDialog1->FileName;
        }
        if(FileExists(strpicname))
        {
            try
            {
                DM->ADOTable2->Edit();
                DM->ADOTablePersonPic->LoadFromFile(strpicname);
                DM->ADOTable2->Post();
            }
            catch(...)
            {
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ppmDeletePicClick(TObject *Sender)
{
    if(DM->ADOTable2->Active)
    {
        try
        {
             DM->ADOTable2->Edit();
             DM->ADOTablePersonPic->Clear();// LoadFromFile(strpicname);
             DM->ADOTable2->Post();
        }
        catch(...)
        {
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmStrechPicClick(TObject *Sender)
{
    ppmStrechPic->Checked = !ppmStrechPic->Checked;
    DBImage1->Stretch      = ppmStrechPic->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxPersonnelEnter(TObject *Sender)
{
    DM->ADOQueryTemp->Close();
    DM->ADOQueryTemp->SQL->Clear();
    DM->ADOQueryTemp->SQL->Add("select distinct ���� from ��Ա��Ϣ order by ����");
    DM->ADOQueryTemp->Open();
    int dataLength = DM->ADOQueryTemp->RecordCount ;
    AnsiString strTTTT;
    ComboBoxPersonnel->Items->Clear();
    DM->ADOQueryTemp->First();
    for(int i=0;i<dataLength;i++)
    {
        strTTTT = DM->ADOQueryTemp->Fields->Fields[0]->Value;
        ComboBoxPersonnel->Items->Add(strTTTT);
        DM->ADOQueryTemp->Next();
    }
    DM->ADOQueryTemp->First();
    DM->ADOQueryTemp->Close();
//    FormMain->KeyPreview = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxPersonnelChange(TObject *Sender)
{
    strPerson = ComboBoxPersonnel->Text;    
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ButtonInfoClick(TObject *Sender)
{
/*
    if(FormPcpInfo->Visible)
    {
        FormPcpInfo->Hide();
    }
    else
    {
        FormPcpInfo->Show();
    }
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N46Click(TObject *Sender)
{
    int iCount;
    AnsiString strTemp;
    AnsiString strOld, strNew;
    int irecordcount = 0;
    float foilangle;
    float ftemp;
    float fspeed;
    int   inum, iold;
    bool  bcombustion;
    bool bGone = false;
    int  icolMax = 1;
    iCount = FileListBoxFile->Items->Count;

    PageControlCurve->ActivePageIndex = 3;
    if(FilterComboBoxFile->ItemIndex != 1)
    {
        FilterComboBoxFile->ItemIndex = 1;          //ָ��PCP ������
        FilterComboBoxFileChange(NULL);
        Application->ProcessMessages();
    }
    StringGridOilAngle->RowCount      = 2;

    if(iCount > 0)        
    {
        for(int i = 0; i < iCount; i ++)
        {
            FileListBoxFile->ItemIndex = i;
            gstrFileName = FileListBoxFile->FileName;
            strTemp = ExtractFileExt(gstrFileName);
            strTemp = strTemp.UpperCase();
                if(!lDS.GetPCPHead(gstrFileName))
                {
                    Application->MessageBox("����ת��ʱ���ִ���!","��ʾ",MB_OK|MB_ICONINFORMATION);
                    continue;
                }
                else
                {
                    inum      = lDS.LFHS.siDieselNum;
                    if(inum == 3)
                    {
                        inum =3;
                    }                //��Ҫ��¼���Ǹ׺ţ�ת�٣�������ǰ�Ƕ�
                    bcombustion = (bool)lDS.LFHS.chOil;
                    if(!bcombustion)
                    {
                        continue;
                    }
                    inum      = lDS.LFHS.siDieselNum;
                    fspeed    = lDS.LFHS.fEngineSpeed;
                    foilangle = -lDS.LFHS.fOilAngle;

                    if(foilangle > 45)
                    {
                        foilangle = foilangle;
                    }
                    if(inum == 2)
                    {
                        inum = inum;
                    }
                    if(irecordcount == 0)
                    {
                        StringGridOilAngle->Cells[0][1] = inum;
                        strTemp.printf("%.1f",foilangle);
                        StringGridOilAngle->Cells[1][1] = strTemp;
                        strTemp.printf("%.1f",fspeed);
                        StringGridOilAngle->Cells[1][0] = strTemp;
                        irecordcount ++;
                        StringGridOilAngle->RowCount ++;
                        continue;
                    }
                    bGone = false;
                    bool  bFilled = false;
                    for( int i = 0; i < irecordcount+1; i++)
                    {
                        strOld = StringGridOilAngle->Cells[0][i + 1];
                        strNew = inum;
                    if(strOld == "")  //������ѵ��˱������һ����
                    {
                        StringGridOilAngle->RowCount ++;
                        irecordcount              = StringGridOilAngle->RowCount;
                        StringGridOilAngle->Cells[0][i + 1] = inum;
                        for( int j = 1; j < 12; j++)
                        {
                            strTemp = StringGridOilAngle->Cells[j][0];
                            if(strTemp == "")
                            {
                                strTemp.printf("%.1f",foilangle);
                                StringGridOilAngle->Cells[j][i+1] = strTemp;
                                if(StringGridOilAngle->Cells[j][0] == "")
                                {
                                    strTemp.printf("%.1f",fspeed);
                                    StringGridOilAngle->Cells[j][0] = strTemp;
                                }
                                if(StringGridOilAngle->Cells[0][i+1] == "")
                                {
                                    StringGridOilAngle->Cells[0][i+1] = inum;
                                }
                                bGone = true;
                                break;
                            }
                            else
                            {
                                ftemp = StrToFloat(strTemp);
                                if( fabs(ftemp - fspeed) < 30)
                                {
                                    strTemp = StringGridOilAngle->Cells[j][i+1];
                                    if(strTemp =="")
                                    {
                                        strTemp.printf("%.1f",foilangle);
                                        StringGridOilAngle->Cells[j][i+1] = strTemp;

                                    }
                                    else
                                    {
                                        int iii = 0;
                                        do
                                        {
                                            strTemp = StringGridOilAngle->Cells[j][i+1 + iii];
                                            if(strTemp == "")
                                            {
                                                break;
                                            }
                                            iii ++;
                                        }while(true);
                                        strTemp.printf("%.1f",foilangle);
                                        StringGridOilAngle->Cells[j][i+1+iii] = strTemp;
                                        StringGridOilAngle->Cells[0][i+ 1 + iii] = inum;
                                        irecordcount = i + iii + 2;
                                        StringGridOilAngle->RowCount = i+1+iii + 1;
                                    }
                                    bGone = true;
                                    break;
                                }
                                else
                                {
                                    continue;
                                }
                            }
                        }
                    }
                    else
                    {
                        iold   = StrToInt(strOld);
                        if(strOld == strNew)
                        {
                            for( int j = 1; j < 12; j++)
                            {
                                if(icolMax < j)
                                {
                                    icolMax = j;
                                }
                                strTemp = StringGridOilAngle->Cells[j][0];
                                if(strTemp == "")
                                {
                                    strTemp.printf("%.1f",foilangle);
                                    StringGridOilAngle->Cells[j][i+1] = strTemp;
                                    if(StringGridOilAngle->Cells[j][0] == "")
                                    {
                                        strTemp.printf("%.1f",fspeed);
                                        StringGridOilAngle->Cells[j][0] = strTemp;
                                    }
                                    if(StringGridOilAngle->Cells[0][i+1] == "")
                                    {
                                        StringGridOilAngle->Cells[0][i+1] = inum;
                                    }
                                    bGone = true;
                                    break;
                                }
                                else
                                {
                                    ftemp = StrToFloat(strTemp);
                                    if( fabs(ftemp - fspeed) < 30)
                                    {
                                        strTemp = StringGridOilAngle->Cells[j][i+1];
                                        if(strTemp =="")
                                        {
                                            strTemp.printf("%.1f",foilangle);
                                            StringGridOilAngle->Cells[j][i+1] = strTemp;

                                        }
                                        else
                                        {
                                            int iii = 0;
                                            do
                                            {
                                                strTemp = StringGridOilAngle->Cells[j][i+1 + iii];
                                                if(strTemp == "")
                                                {
                                                    break;
                                                }
                                                iii ++;
                                            }while(true);
                                            strTemp.printf("%.1f",foilangle);
                                            StringGridOilAngle->Cells[j][i+1+iii] = strTemp;
                                            StringGridOilAngle->Cells[0][i+ 1 + iii] = inum;
                                            irecordcount = i + iii + 2;
                                            StringGridOilAngle->RowCount = i+1+iii + 1;
                                        }
                                        bGone = true;
                                        break;
                                    }
                                    else
                                    {
                                        continue;
                                    }
                                }
                            }
                        }
                        else  if(inum > iold)
                        {
                            continue;
                        }
                        else
                        {
                            for( int j = 1; j < 12; j++)
                            {
                                if(icolMax < j)
                                {
                                    icolMax = j;
                                }
                                strTemp = StringGridOilAngle->Cells[j][0];
                                if(strTemp == "")
                                {
                                    strTemp.printf("%.1f",foilangle);
                                    StringGridOilAngle->Cells[j][i+1] = strTemp;
                                    if(StringGridOilAngle->Cells[j][0] == "")
                                    {
                                        strTemp.printf("%.1f",fspeed);
                                        StringGridOilAngle->Cells[j][0] = strTemp;
                                    }
                                    if(StringGridOilAngle->Cells[0][i+1] == "")
                                    {
                                        StringGridOilAngle->Cells[0][i+1] = inum;
                                    }
                                    bGone = true;
                                    break;
                                }
                                else
                                {
                                    ftemp = StrToFloat(strTemp);
                                    if( fabs(ftemp - fspeed) < 30)
                                    {
                                        strTemp = StringGridOilAngle->Cells[j][i+1];
                                        if(strTemp =="")
                                        {
                                            strTemp.printf("%.1f",foilangle);
                                            StringGridOilAngle->Cells[j][i+1] = strTemp;

                                        }
                                        else
                                        {
                                            int iii = 0;
                                            do
                                            {
                                                strTemp = StringGridOilAngle->Cells[j][i+1 + iii];
                                                if(strTemp == "")
                                                {
                                                    break;
                                                }
                                                iii ++;
                                            }while(true);
                                            strTemp.printf("%.1f",foilangle);
                                            StringGridOilAngle->Cells[j][i+1+iii] = strTemp;
                                            StringGridOilAngle->Cells[0][i+ 1 + iii] = inum;
                                            irecordcount = i+1+iii;
                                            StringGridOilAngle->RowCount ++;
                                        }
                                        bGone = true;
                                        break;
                                    }
                                    else
                                    {
                                        continue;
                                    }
                                }
                            }
                        }
                    }

                    if(bGone == true)
                    {
                        break;
                    }
                }
            }
        }

        FILE* fp;
        AnsiString strTemp1,strTemp2;
        TDate dt = Now();
        strTemp1 = dt.DateString();
        strTemp2.printf(" ϵ��_%.2f_%.2f", lDS.fLevelStart, lDS.fLevelEnd);
        strTemp = ExtractFileName(gstrFileName);
        int iposi = strTemp.Pos(" ");
        strTemp = strTemp.SubString(1,iposi - 3);

        StringGridOilAngle->Cells[0][0] = "0";
        strTemp =  strTemp + " " + strTemp1 + strTemp2 + AnsiString(".dat");
        strTemp = strExeFilePath + "Debug\\" + strTemp;
        if((fp = fopen(strTemp.c_str(),"w")) != NULL)
        {
            for(int i = 0; i < irecordcount; i++)
            {
                strTemp = "";
                for( int j = 0; j <= icolMax; j++)
                {
                    strTemp2 = StringGridOilAngle->Cells[j][i];
                    if( strTemp2 == "")
                    {
                        strTemp2 = "0";
                    }
                    strTemp += (" " + strTemp2);
                }
                fprintf(fp, "%s\n", strTemp.c_str());
            }
            fclose(fp);
        }
    }
    Application->MessageBox("������ǰ�Ƿ�������!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmAddToTypicalPipeDatabaseClick(
      TObject *Sender)
{
    AnsiString strTemp,strNameTemp,strPCPBackup;
    bool bdatabasestatus;
    int iindex,icount;
    icount = FileListBoxFile->Items->Count;
    if(icount <= 0)
    {
        return;
    }
    iindex = FileListBoxFile->ItemIndex;
    if(iindex < 0)
    {
        return;
    }
    strTemp = FileListBoxFile->Directory + "\\";
    strNameTemp = strTemp + FileListBoxFile->Items->Strings[iindex];

    InsertToPipeBase(strNameTemp);
}

void __fastcall TFormMain::InsertToPipeBase(AnsiString& strNameTemp)
{
    AnsiString strTemp;
    strTemp = ExtractFileExt(strNameTemp);
    strTemp = strTemp.UpperCase();
    if( strTemp == ".PCP")
    {
        TCursor Save_Cursor = Screen->Cursor;
        Screen->Cursor = crHourGlass;    // Show hourglass cursor
        try
        {
            Screen->Cursor = crHourGlass;
            InsertTypicalPipeToDatabase(strNameTemp);
        }
        __finally
        {
            Screen->Cursor = Save_Cursor;
        }
    }
    else
    {
        Application->MessageBox("��ѡ������������ļ�(*.PCP�ļ�)!!!\n\nȻ��ִ���������!!","��ʾ",MB_OK|MB_ICONINFORMATION);
    }
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::InsertTypicalPipeToDatabase(AnsiString& strfileName)
{
    //TODO: Add your source code here
// ����ֹ��ǰ40������ֹ���60�ȵ������������ݿ�
    bool bret = false;
    bool bDBActive = false;
    float fPipeData[500];
    AnsiString strTemp, strFaultType = "����";
    if(FileExists(strfileName))
    {
        if(lDS.LoadPCPFile(strfileName.c_str()))   //�������ɹ�
        {
            float ftgain = 1.0;
            short int siTemp = 0;
            TDateTime dtTemp,dtTemp0;
            dtTemp = TDateTime( lDS.LFHS.siYear,
                                lDS.LFHS.siMonth,
                                lDS.LFHS.siDate);
            dtTemp0 = TDateTime(lDS.LFHS.siHour,
                                lDS.LFHS.siMinute,
                                lDS.LFHS.siSecond,
                                siTemp);
            dtTemp = dtTemp + dtTemp0;
            lDS.GetValidPipe(fPipeData, -40, 500);
            FormPipeBase = new TFormPipeBase(FormMain);
            FormPipeBase->EditFaultType->Text        = strFaultType;
            FormPipeBase->PanelTrainType->Caption    = lDS.strTrainType;
            FormPipeBase->PanelTrainNumber->Caption  = lDS.strTrainNumber;
            FormPipeBase->PanelDieselNumber->Caption = lDS.strDieselNumber;
            strTemp.printf("%.1f",lDS.fEngineSpeed);
            FormPipeBase->PanelDieselSpeed->Caption  = strTemp;
            strTemp.printf("%.1f",lDS.fOilAngle);
            FormPipeBase->PanelOilAngle->Caption     = strTemp;

            strTemp.printf("%.1f",lDS.LFHS.fOilPeak/ftgain );
            FormPipeBase->PanelWavePeak->Caption     = strTemp;
            strTemp.printf("%.1f",lDS.LFHS.fWavePeriod);
            FormPipeBase->PanelWavePeriod->Caption     = strTemp;
            strTemp.printf("%.3f",lDS.LFHS.fWaveCoef);
            FormPipeBase->PanelWaveCoef->Caption     = strTemp;
            FormPipeBase->ChartPipe->Series[0]->Clear();
            for(int i = 0; i < 500; i ++)
            {
                FormPipeBase->ChartPipe->Series[0]->AddXY(i, fPipeData[i],"",clRed);
            }
            if( FormPipeBase->ShowModal() == mrOk)
            {
                strFaultType = FormPipeBase->EditFaultType->Text;
            }
            delete FormPipeBase;

            bret = DM->ADODataSetPipe->Active;
            if( !bDBActive )                      //������ݿ⴦�ڹر�״̬
            {
                DM->ADODataSetPipe->Open();
            }
            bool bcanchange = DM->ADODataSetPipe->CanModify;
            if(bcanchange)
            {
                try
                {
                    DM->ADODataSetPipe->Append();
                }
                catch(...)
                {
                    throw("ѹ�����������ݿ� ���ӷǷ�����!!");

                }

                DM->ADODataSetPipe->FieldByName("TrainType")->AsString   = lDS.strTrainType;
                strTemp.printf("%d",(int)(lDS.fEngineSpeed));
                DM->ADODataSetPipe->FieldByName("TrainNumber")->AsString   = lDS.strTrainNumber;
                DM->ADODataSetPipe->FieldByName("DieselNumber")->AsString  = lDS.strDieselNumber;
                DM->ADODataSetPipe->FieldByName("MDate")->AsDateTime       = dtTemp;
                DM->ADODataSetPipe->FieldByName("DieselSpeed")->AsString = strTemp;

                strTemp.printf("%.1f",lDS.LFHS.fOilPeak/ftgain );
                DM->ADODataSetPipe->FieldByName("WavePeak")->AsString  = strTemp;
                strTemp.printf("%.1f",lDS.LFHS.fWavePeriod);
                DM->ADODataSetPipe->FieldByName("WavePeriod")->AsString    = strTemp;
                strTemp.printf("%.1f",lDS.LFHS.fWaveCoef);
                DM->ADODataSetPipe->FieldByName("WaveCoef")->AsString  = strTemp;

                DM->ADODataSetPipe->FieldByName("FaultType")->AsString   = strFaultType ;
                TMemoryStream *pms=new TMemoryStream();

                pms->Position=0;
                pms->Write((void*)(&fPipeData[0]), sizeof(float)*500);
                DM->ADODataSetPipe_PipeData->LoadFromStream(pms);
                DM->ADODataSetPipe->Post();
                delete pms;
            }
            if( !bDBActive )
            {
                DM->ADODataSetPipe->Close();
            }
            bret = true;
        }
    }
    return bret;
}

void __fastcall TFormMain::ComboBoxPipeTrainTypeEnter(TObject *Sender)
{
    AnsiString strTemp;
    if(DM->ADOQueryTemp->Active)
    {
        DM->ADOQueryTemp->Close();
    }
    strTemp = "select distinct TrainType from PipeInfo order by TrainType";
    DM->ADOQueryTemp->SQL->Clear();
    DM->ADOQueryTemp->SQL->Add(strTemp);
    DM->ADOQueryTemp->Open();
    int dataLength = DM->ADOQueryTemp->RecordCount ;
    AnsiString strTTTT;
    ComboBoxPipeTrainType->Items->Clear();
    DM->ADOQueryTemp->First();
    for(int i=0; i<dataLength; i++)
    {
        strTTTT = DM->ADOQueryTemp->Fields->Fields[0]->Value;
        ComboBoxPipeTrainType->Items->Add(strTTTT);
        DM->ADOQueryTemp->Next();
    }
    DM->ADOQueryTemp->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxPipeTrainTypeChange(TObject *Sender)
{
	AnsiString strOperation = "";
    AnsiString strTemp, strOrderInfo;
    bool bQueryPart = true;      //
    int iItem;
    int iRecordCount = 0;
    //��������Ž��в�ѯ
    if(ComboBoxPipeTrainType->Text != "")   //�и׸�����
    {
        strTemp = "TrainType = '" + ComboBoxPipeTrainType->Text + "'";
        strOperation  = "select * from PipeInfo where ";
        strOperation += strTemp;

    	DM->ADODataSetPipe->Close();
		DM->ADODataSetPipe->CommandType = cmdText;
    	DM->ADODataSetPipe->CommandText = strOperation;
    	DM->ADODataSetPipe->Open();
/*
        iRecordCount = DM->ADODataSetPipe->RecordCount ;
        strTemp.printf("�� %d ��",iRecordCount);
        PanelATRecordNum->Caption = strTemp;

        iATTableIndex = 1;
        strTemp.printf("�� %d ��", iATTableIndex);
        PanelCurrentRec->Caption = strTemp;
*/
    }    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::DBGrid3CellClick(TColumn *Column)
{
    FillPipeData();    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::DBNavigator4Click(TObject *Sender,
      TNavigateBtn Button)
{
    FillPipeData();    
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::FillPipeData(void)
{
    //TODO: Add your source code here
    if(DM->ADODataSetPipe->Active)
    {
        float fPipeData[500];
        AnsiString strTemp = "", strAll;
        strAll = "���ͣ�" ;
        strTemp = DM->ADODataSetPipe->FieldByName("TrainType")->AsString;
        strAll += strTemp;
        strAll += "; ���ţ�";
        strTemp = DM->ADODataSetPipe->FieldByName("TrainNumber")->AsString;
        strAll += strTemp;
        strAll += "; �׺ţ�";
        strTemp = DM->ADODataSetPipe->FieldByName("DieselNumber")->AsString;
        strAll += strTemp;
        ChartPipe->Series[0]->Clear();
        ChartPipe->Title->Text->Strings[1] = strAll;
        TMemoryStream *pms = new TMemoryStream();
        DM->ADODataSetPipe_PipeData->SaveToStream(pms);
        pms->Position=0;
        pms->Read((void*)(&fPipeData[0]), sizeof(float)*500);
        delete pms;
        for(int i = 0; i < 500; i++)
        {
            ChartPipe->Series[0]->AddXY(-40.0 + i*0.2, fPipeData[i], "", clBlue);
        }
    }
}
void __fastcall TFormMain::ToolButtonPSTClick(TObject *Sender)
{
    TToolButton *ptTT = (TToolButton*)Sender;
    ptTT->Down = true;
    ptTT->Refresh();
    FilterComboBoxFile->ItemIndex = ptTT->Tag;
    FileListBoxFile->Mask = FilterComboBoxFile->Mask;
    FileListBoxFile->Update();
    if( ToolButtonPST->Down )
    {
        ToolButton36->Down = true;
    }
    else if( ToolButtonPCP->Down )
    {
        ToolButton26->Down = true;
    }
    else if( ToolButtonORG->Down )
    {
        ToolButton23->Down = true;
    }
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::ToolButtonPipeClick(TObject *Sender)
{
    AnsiString strTemp;
    if( strPSTName != "")
    {
 //       ImagePipe->Width = PanelCompareBack->Width - ScrollBarPipe->Width;
        AnalysisPipe();
    }
    else
    {
        Application->MessageBox("���ȴ��б��ļ�!","��ʾ",MB_OK|MB_ICONINFORMATION);
    }

    if(PageControlMain->ActivePageIndex != 4)
    {
        PageControlMain->ActivePageIndex  = 4;
    }
    if(PageControlCurve->ActivePageIndex != 4)
    {
        PageControlCurve->ActivePageIndex = 4;
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ToolButton5Click(TObject *Sender)
{
    PageControlMain->ActivePageIndex  = 4;
    PageControlCurve->ActivePageIndex = 3;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::AnalysisPipe(void)
{
    //TODO: Add your source code here
    int iCount = ListBoxPCPFile->Items->Count;
    if(iCount == 0)
    {
        Application->MessageBox("����װ���б��ļ�\n\nȻ���ٷ���!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    TCursor Save_Cursor = Screen->Cursor ;    // Show hourglass cursor
    try
    {
        Screen->Cursor = crHourGlass;
        if(DetectPipeByBase())
        {
            bPipeCompared = true;
//            PageControlMain->ActivePageIndex = 2;
//            Application->ProcessMessages();
/*
            if(IntImage())
            {
                PageControlMain->ActivePageIndex = iPageAnalysisData;
//        GroupBoxInfo->Height=260;
                ScrollBarCompare->Visible = true;
                ScrollBarCompare->Max     = 1600-ScrollBarCompare->Height;
                bLibCompare               = true;
                bAnalysisResult           = false;
                DrawCompare();
            }
*/
        }
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
}

bool __fastcall TFormMain::DetectPipeByBase(void)
{
    float *fModalResult = NULL;
    AnsiString strModalResult,strFileTemp,strFilePathTemp;
    float fmax,fmin;
    float fOriginal[500];
    float fModal[500];
    float fMatchLevel[20];
    float fMatchCoef[] = {10,10,10,10,10,10,10,10,10,10};
    int iDNum[16];
    int istartposi;
    int iCountNum,iCNum;
    bool bDWrong[16];
    int iDieselCount;
    AnsiString strTTT,strOperation,strSpeed, strTemp;
    int iindex;
    int iCount;
    iDieselCount    = ListBoxPCPFile->Items->Count;
    iCount          = iDieselCount;
    strFilePathTemp = strExeFilePath;
    strFilePathTemp += "Temp";
    iindex = ListBoxPSTFile->ItemIndex;
    if(iindex < 0)
    {
        return false;
    }
    strTTT = ::ExtractFileName(ListBoxPSTFile->Items->Strings[iindex]);
    strTemp = strTTT;
//--------------------------------------------------------
// �������ڻ�ȡ��������
    char ch;
    ch = strTTT.c_str()[7];
    if((ch < '0')||(ch > '9'))
    {
        strTTT = strTTT.SubString(1,7);
    }
    else
    {
        strTTT = strTTT.SubString(1,6);
    }
//=========================================================

//---------------------------------------------------------
// �������ڻ�ȡ���ͻ�ת��
   int iposispace,iposir;
   float fspeed;
   iposispace = strTemp.Pos(" ");
   iposir     = strTemp.Pos("r");
   strSpeed   = strTemp.SubString(iposispace , iposir - iposispace);
   fspeed     = StrToFloat(strSpeed);

    DM->ADODataSetPipe->Parameters->Clear();
    DM->ADODataSetPipe->Parameters->AddParameter();
    DM->ADODataSetPipe->Parameters->Items[0]->Name      = "LowValue";
    DM->ADODataSetPipe->Parameters->Items[0]->DataType  = ftFloat;
    DM->ADODataSetPipe->Parameters->Items[0]->Direction = pdInput;
    DM->ADODataSetPipe->Parameters->Items[0]->Value     = fspeed - 30.0;

    DM->ADODataSetPipe->Parameters->AddParameter();
    DM->ADODataSetPipe->Parameters->Items[1]->Name      = "HighValue";
    DM->ADODataSetPipe->Parameters->Items[1]->DataType  = ftFloat;
    DM->ADODataSetPipe->Parameters->Items[1]->Direction = pdInput;
    DM->ADODataSetPipe->Parameters->Items[1]->Value     = fspeed + 30.0;
    strTemp.printf(" and DieselSpeed > :LowValue and DieselSpeed < :HighValue ");
//=========================================================

    strTTT = "TrainType = '" + strTTT + "'" ;
    strTTT += strTemp;
    strOperation = "select * from PipeInfo where " + strTTT ;

    DM->ADODataSetPipe->Close();
    DM->ADODataSetPipe->CommandType = cmdText;
    DM->ADODataSetPipe->CommandText = strOperation;
    DM->ADODataSetPipe->Open();
    iCountNum = DM->ADODataSetPipe->RecordCount;

    if(iCountNum == 0)
    {       //�ָ�ԭ�������ݿ��״̬
        Application->MessageBox("���ݿ���Ͳ���û����Ӧ������!!!\n\n�޷����з���","��ʾ",MB_OK|MB_ICONINFORMATION);
        DM->ADODataSetPipe->Close();
        return false;
    }

    fModalResult = new float[iCountNum];
    if( NULL == fModalResult )
    {       //�ָ�ԭ�������ݿ��״̬
        Application->MessageBox("�޷������ڴ�!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
        DM->ADODataSetPipe->Close();
        return false;
    }

    DM->ADODataSetPipe->DisableControls();

    strLibResult->Clear();
    MemoPipeResult->Lines->Clear();
    MemoPipeResult->Lines->Add("             * ���ζԱ�˵�� *");
    MemoResult->Lines->Add("====================================================");
    MemoPipeResult->Lines->Add(" ����ֵ�����ڱ�ʾ�������ε����Ƴ̶ȣ�");
    MemoPipeResult->Lines->Add(" (1)��ֵԽС��������������Խ���ƣ�");
    MemoPipeResult->Lines->Add(" (2)����ֵ����ĳһ����ֵʱ������Ϊ�������β������ƣ�");
    MemoPipeResult->Lines->Add("====================================================");
    MemoPipeResult->Lines->Add("");
    MemoPipeResult->Lines->Add(" �׺�    ����ֵ    �������");

    for(int i=0; i<16; i++)
    {
        bDWrong[i] = false;
    }
    int iDieselCountBackup ;
    iDieselCountBackup = iDieselCount;
    iDieselCount = 16;
    for(int i=0; i<iCount; i++)
    {
        strTemp =  ::ExtractFilePath(strPSTName) + ::ExtractFileName(ListBoxPCPFile->Items->Strings[i]);
        if(!lDS.LoadPCPFile(strTemp.c_str()))   //�������ɹ�
        {
            iDNum[i]=0;
            strLibResult->Add("��������Ч");
            MemoPipeResult->Lines->Add("");
            continue;
        }

        iDNum[i]   = lDS.LFHS.siDieselNum;
        iDieselNum = lDS.LFHS.siDieselNum;
        iCNum = 200;
        lDS.GetValidPipe(fOriginal, -40, 500);  //����ֹ��ǰ40�ȿ�ʼ�����500�����ݵ�
        FillAndCopyChart(fOriginal, 500, i, 0, strTemp);
        NormalizeData(fOriginal, 500);

        DM->ADODataSetPipe->First();
        for(int j = 0 ; j < iCountNum; j ++)
        {
            ReadPipeDataFromDatabase(fModal, 500);
            NormalizeData(fModal,500);
            fModalResult[j] = DetectModal(fModal, fOriginal, 50,400);
            DM->ADODataSetPipe->Next();
        }

        float ftempp=1000.0;
        int itempp=0;

//        strModalResult = DM->ADODataSetPipe->FieldByName("FaultType")->AsString;
        strFileTemp  = strFilePathTemp + AnsiString(i);
        strFileTemp += ".pip";
        for(int j = 0; j < iCountNum; j++ )
        {
            if(ftempp > fModalResult[j])
            {
                itempp = j;
                ftempp = fModalResult[j];
            }
        }
        DM->ADODataSetPipe->First();
        DM->ADODataSetPipe->MoveBy(itempp);
        strModalResult = DM->ADODataSetPipe->FieldByName("FaultType")->AsString;
        ReadPipeDataFromDatabase(fModal, 500);
        FillAndCopyChart(fModal, 500, i, 1, strModalResult);
        strModalResult = DM->ADODataSetPipe->FieldByName("FaultType")->AsString;

        if( ftempp > myDieselInfo[iNowSelectTrainType].fDetectLevel ) //�����������������Դ����趨������ֵ
        {
            strModalResult = "�������̫���޷��ж�!!!";
        }
        if( i < 16 )
        {
            strPipeDetectResult[i] = strModalResult;
        }
        AnsiString strt1,strt2;
        strModalResult = strModalResult.TrimLeft();
        int ill = strModalResult.Length();
        ill = ill < 8 ? ill : 8;
        strt1 = strModalResult.SubString(1,ill);
        if(   (strModalResult != "�ϸ�")
           && (strModalResult != "����")
           && (strt1 != "��������")
           )
        {
            bDWrong[i] = true;
        }
        strTTT.printf("  %02d    %.3f     ",iDNum[i],ftempp);
        strTTT += strModalResult;
//        strLibResult->Add(strModalResult);
        MemoPipeResult->Lines->Add(strTTT);

    }

    delete [] fModalResult;
    DM->ADODataSetPipe->First();
    FillPipeData();
    DM->ADODataSetPipe->EnableControls();

    return true;
}

void __fastcall TFormMain::NormalizeData(float* fp, int ilen)
{
    //TODO: Add your source code here
    float fmax = 0.0 , fmin;
    for(int i = 0; i < 20; i++)
    {
        fmax += fp[i];
    }
    fmax /= 20.0;

    fmin = fmax;
    for(int i = 0; i < ilen; i++ )
    {
        if(fp[i] > fmax)
        {
            fmax = fp[i];
        }
    }
    for(int i = 0; i < ilen; i++)
    {
        fp[i] = (fp[i] - fmin) / (fmax - fmin);
    }
}

void __fastcall TFormMain::ReadPipeDataFromDatabase(float* fdata,int ilen)
{
    //TODO: Add your source code here
    int illen;
    TMemoryStream *pms;
    pms = new TMemoryStream();
    DM->ADODataSetPipe_PipeData->SaveToStream(pms);
    pms->Position=0;
    pms->Read((void*)fdata,sizeof(float)*ilen);
    delete pms;
}

float __fastcall TFormMain::DetectModal(float* fdata, float * ftarget, int startnum, int len)
{
    //TODO: Add your source code here
    float fresult[60];
    float fftemp = 0.0;
    for(int i=0; i<30; i++)
    {
        fresult[i]=0.0;
        for(int j = startnum; j<startnum + len; j++)
        {
            fftemp = fdata[j]-ftarget[j+i];
            fresult[i] += fftemp*fftemp;
        }
    }
    for(int i = 30; i<60; i++)
    {
        fresult[i] = 0.0;
        for(int j = startnum; j < startnum + len; j++)
        {
            fftemp = ftarget[j] - fdata[j+i-30];
            fresult[i] += fftemp*fftemp;
        }
    }
    fftemp = fresult[0];
    for(int i = 1; i<60; i++)
    {
        if(fresult[i] < fftemp)
        {
            fftemp = fresult[i];
        }
    }
    return fftemp;
}

void __fastcall TFormMain::FillAndCopyChart(float* fdata, int ilen, int ix, int iy, AnsiString& strinfo)
{
    //TODO: Add your source code here
    TColor clTemp;
    TRect rectTarget;
    int iwidth;
    float iheight;
    ChartPipe->Series[0]->Clear();
/*
    if( iy == 0)
    {
        clTemp = clRed;
    }
    else
    {
        clTemp = clBlue;
    }
*/
//    iwidth = ImagePipe->Width   / 2;
    iwidth = (PanelCompareBack->Width - ScrollBarPipe->Width - 6)/2;
    iheight = (float)ImagePipe->Height /16.0;
    rectTarget.Left   = iwidth*iy - 3;
    rectTarget.Right  = rectTarget.Left + iwidth - 1;
    rectTarget.Top    = iheight*ix + 1;
    rectTarget.Bottom = rectTarget.Top + iheight - 3;
    for( int i = 0; i < ilen; i++)
    {
        ChartPipe->Series[0]->AddXY(i*0.2 - 40.0, fdata[i],"",clTeeColor);
    }
    ChartPipe->Title->Text->Clear();
    AnsiString strTemp;
    if(iy == 0)
    {
        strTemp.printf("��%d��",iDieselNum);
        ChartPipe->LeftAxis->Title->Caption = strTemp;
    }
    else
    {
        ChartPipe->LeftAxis->Title->Caption = "";
    }
//    ChartPipe->LeftAxis->Labels         = false;
//    ChartPipe->LeftAxis->Title->
    ChartPipe->BottomAxis->Title->Caption = "";
    ChartPipe->Draw(ImagePipe->Canvas, rectTarget );

    if( iy != 0)
    {
        int iwidth = ImagePipe->Canvas->TextWidth( strinfo );
        SetBkMode(ImagePipe->Canvas->Handle,TRANSPARENT);
        ImagePipe->Canvas->TextOut( rectTarget.Right - iwidth- 20,rectTarget.Top + 6, strinfo);
    }

    ChartPipe->Title->Text->Clear();
    ChartPipe->Title->Text->Add("��ѹ�͹�ѹ�������ϲ���");
    ChartPipe->Title->Text->Add("");
    ChartPipe->LeftAxis->Title->Caption = "��һ���������";
    ChartPipe->BottomAxis->Title->Caption = "����ת�� /CA";

}

void __fastcall TFormMain::PanelCompareBackResize(TObject *Sender)
{
    ScrollBarPipe->Min = 0;
    ScrollBarPipe->Max = 1600 - PanelCompareBack->Height;
//    ImagePipe->Width   = PanelCompareBack->Width - ScrollBarPipe->Width - 6;
    ImagePipe->Top =  2;
    ScrollBarPipe->Position = 0;
    if( bPipeCompared )
    {
        AnalysisPipe();
    }
    Application->ProcessMessages();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ScrollBarPipeChange(TObject *Sender)
{
    ImagePipe->Top = 2 - ScrollBarPipe->Position;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToolButtonDrawOneClick(TObject *Sender)
{
    if( iListDrawKind > 1)
    {
        Application->MessageBox("�˹��ܽ�������ͬһ̨�����ݶԱ�!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    ToolButtonDrawOne->Down = !bPSTDrawOne;
    bPSTDrawOne = ToolButtonDrawOne->Down;
    if(bPSTDrawOne)
    {
        ScrollBarPST->Visible = true;
    }
    else
    {
        ScrollBarPST->Visible = false;
    }
//    Application->ProcessMessages();
    if(PageControlCurve->ActivePageIndex == 2)
    {
        if(bLoadList)
        {
            //PaintBoxAll->Refresh();
            if( iListDrawKind == 1)
            DrawImageAll();
        }
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::GetTrainInfo(AnsiString& str)
{
    //TODO: Add your source code here
    AnsiString strTemp;
    char       chTemp;
    int        iTemp;
    strTemp        = ExtractFileName(str);
    chTemp         = strTemp.c_str()[6];
    if( ( chTemp < '0') || (chTemp > '9') )
    {
        strTrainType   = strTemp.SubString(1, 7);
        strTrainNumber = strTemp.SubString(8, 4);
    }
    else
    {
        strTrainType   = strTemp.SubString(1, 6);
        strTrainNumber = strTemp.SubString(7, 4);
    }
    iTemp          = strTemp.Pos( "rpm " );
    strMTime       = strTemp.SubString( iTemp + 4, 10) ;
}

void __fastcall TFormMain::FillPSTSingle(int inum, float* ffai, float* fvalue, int istart, int iend)
{
    //TODO: Add your source code here
/*
    TRect rect;
    TColor clTemp;

    if(bPSTDrawOne)   //������ͼ�λ��Ƶ�ͬһ��ͼ����
    {
        rect = ImagePST->ClientRect;
        ChartPST->Series[inum - 1]->Clear();
    }
    else
    {
        int iheight = ImagePST->Height/8.0;
        int iwidth  = ImagePST->Width/2.0;
        rect.Top    = iheight *( (inum - 1) / 2);
        rect.Bottom = rect.Top + iheight;
        if( ((inum - 1) % 2) == 0)
        {
            rect.Left   = 0;
        }
        else
        {
            rect.Left = iwidth;
        }
        rect.Right = rect.Left + iwidth;
        for( int i = 0; i < 16; i++)
        {
            ChartPST->Series[i]->Clear();
        }
    }

    for( int i = istart; i < iend; i++)
    {
        ChartPST->Series[inum - 1]->AddXY(ffai[i], fvalue[i], "",  clTeeColor);  //TChart
    }
    ChartPST->Draw(ImagePST->Canvas, rect);
*/
}
void __fastcall TFormMain::ScrollBarPSTChange(TObject *Sender)
{
  	int iLen,ipage;
    iLen  = iChoiceEnd - iChoiceStart;
    if(ScrollBarPST->Max == 0)
    {
        return;
    }
    if(iLen == lDS.iPLength)
    {
        return;
    }
    iChoiceStart = ScrollBarPST->Position;
    if(iChoiceStart == 0)
    {
      	iChoiceStart = 1;
    }
    iChoiceEnd    = iChoiceStart + iLen;
    if(iChoiceEnd > lDS.iPLength)
    {
    	iChoiceEnd = lDS.iPLength;
    }
 //   bDrawed=false;
    fstartrt  = (float)iChoiceStart/(float)lDS.iPLength;
    fendrt    = (float)iChoiceEnd/(float)lDS.iPLength;
//    PaintBoxAll->Repaint();
    DrawImageAll();
    Application->ProcessMessages();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::DeleteRamFile(void)
{
    //TODO: Add your source code here
    for(int i = 0; i < 32; i++)
    {
        if( pArrayMS[i] != NULL)
        {
            delete pArrayMS[i];
        }
    }
}


void __fastcall TFormMain::ppmCombustionCompareClick(TObject *Sender)
{
    ShowCompareList(1, true);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmCompressCompareClick(TObject *Sender)
{
    ShowCompareList(0, true);    
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::mnuCompareToolbarClick(TObject *Sender)
{
    bool bChecked;
//    bChecked = ToolBarCompareList->Visible;
    if(bChecked)
    {
//        ToolBarCompareList->Visible = false;
//        mnuCompareToolbar->Caption = "��ʾ�Աȹ�����";
//        ppMainToolbar->Caption = "��ʾ������";
    }
    else
    {
//        ToolBarCompareList->Visible = true;
//        mnuCompareToolbar->Caption = "���ضԱȹ�����";
//        ppMainToolbar->Caption = "���ع�����";
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageAllMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int iNN;
    AnsiString strTempName;
    if(!bPSTOpened)
    {
        return;
    }
    if(Button == mbRight)
    {
        ptOriginal.x = X;
        ptOriginal.y = Y;
        bMouseDown = true;
        bRightButton = true;
        return;
    }
    else
    {
        bRightButton = false;
    }
    if(bPSTDrawOne)
    {
        TPoint pointMouse;
        pointMouse.x = X;
        pointMouse.y = Y;
        TRect rect;
        rect.Left    = ImageAll->Left;
        rect.Top     = ImageAll->Top;
        rect.Right   = rect.Left + ImageAll->Width;
        rect.Bottom  = rect.Top  + ImageAll->Height;
        TImage* ptPB = (TImage*)Sender;
        ptPB->Canvas->Pen->Mode  = pmNot;
        if(PtInRect(&rect,pointMouse))
        {
            pointMouseOrigin.x  = X;
            pointMouseOrigin.y  = Y;
            pointMouseOld.x     = X;
            pointMouseOld.y     = Y;
            bPBMouseDragging    = true;
        }
        return;
    }
    if(iListDrawKind != 1)
    {
        iListPage ++;
        iNN = 16 / iListPageNum;
        if( iListPage >= iNN)
        {
            iListPage = 0;
        }
        DrawListDirect();
        return;
    }
/*
    if(CheckBox3D->Checked)
    {
        ptOriginal.x = X;
        ptOriginal.y = Y;
        bMouseDown = true;
        bRightButton = false;
        return;
    }
*/
    if(  (AllDrawKind == drawMAX)            //�������ֵ
//       ||(AllDrawKind == drawPWave)          //�����͹�ѹ��ͼ
       ||(AllDrawKind == drawPPWave)         //�����͹ܼ�����ѹ��ͼ
//       ||(AllDrawKind == drawInfo)
      )
    {
        return;
    }

//    if( AllDrawKind == drawPPWave )

    if( AllDrawKind == drawInfo)
    {
         bDrawBasicCurve = !bDrawBasicCurve;
//         PaintBoxAll->Refresh();
        DrawImageAll();
         return;
    }
    if( AllDrawKind == drawDetect )
    {
        return;
    }
    TPoint pointMouse;
    int icol,irow,ichoice;
    int xnum,ynum,ipart,xstep,ystep;
    bool bOut;
    bOut           = false;
    pointMouse.x   = X;
    pointMouse.y   = Y;
    bool bSelected = false;
    if(bDrawStyle)
    {
          ynum   = iChoicedNum / 2;
          ipart  = iChoicedNum % 2;
          ynum   = ynum + ipart;
          xnum   = 2;
    }
    else
    {
          xnum   = 1;
          ynum   = iChoicedNum;
    }
    TRect rect,nowrect;
    rect.Left    = ImageAll->Left;
    rect.Top     = ImageAll->Top;
    rect.Bottom  = ImageAll->Top  + ImageAll->Height;
    rect.Right   = ImageAll->Left + ImageAll->Width;
    xstep        = (rect.Right-rect.Left)/xnum;
    ystep        = (rect.Bottom-rect.Top)/ynum;
    ipart        = 0;
    for(irow = 0; irow < ynum; irow++)
    {
        for(icol = 0; icol < xnum; icol++)
        {
            nowrect.Left    = rect.Left + icol*xstep;
            nowrect.Top     = rect.Top  + irow*ystep;
            nowrect.Bottom  = nowrect.Top+ystep;
            nowrect.Right   = nowrect.Left+xstep;
            if(PtInRect(&nowrect,pointMouse))
            {
                bSelected   = true;
                ichoice     = ipart;
            }
            if(ipart == iChoicedNum)
            {
                bOut = true;
                break;
            }
            ipart ++;
         }
         if(bOut) break;
         if(bSelected)
         {
            break;
         }
    }

    if(bSelected)
    {
        strOneOfAll       = ListBoxPST->Items->Strings[ichoice];
        strTempName = ExtractFileName(strOneOfAll);
        strOneOfAll = strValidPath + strTempName;
        strPCPName        = strOneOfAll;
        if(  (AllDrawKind == drawPWave)          //�����͹�ѹ��ͼ,�������ڻ���ԭʼ����һ��
          )
        {
            PaintBoxAdjustTDC->PopupMenu = PopupMenuInsert;
            PCPDrawKind = drawOriginal;
        }
        else if( CheckBoxDrawPipeWave->Checked )
        {
            PaintBoxAdjustTDC->PopupMenu = PopupMenuInsert;
            PCPDrawKind = AllDrawKind;
//            PCPDrawKind = drawOriginal;

        }
        else
        {
            PCPDrawKind = AllDrawKind;
            PaintBoxAdjustTDC->PopupMenu = PopupMenuAllDraw;
        }

        bOneOfAll         = true;
        bFileOpened       = true;
        bAdjustHRR        = true;
        iKeyPressPosi     = 1;
        CheckBoxAdjustPT->Checked       = false;
        CheckBoxAdjustCPT->Checked      = false;
        CheckBoxAdjustPV->Checked       = false;
        CheckBoxAdjustLogVLogP->Checked = false;
        CheckBoxAdjustDPDF->Checked     = false;
        CheckBoxAdjustHRR->Checked      = false;
        CheckBoxAdjustCHRR->Checked     = false;
        CheckBoxAdjustFFTP->Checked     = false;

        if((!bPCPEntered) || ( AllDrawKind == drawOriginal))
        {
            if( AllDrawKind != drawPWave )
            {
                PCPDrawKind = AllDrawKind;
            }
            bPCPEntered = true;
        }
//        PCPDrawKind = AllDrawKind;
        switch( PCPDrawKind )
        {
            case drawOriginal:         //ԭʼ����
            {
                CheckBoxAdjustPT->Checked = true;
                break;
            }
            case drawPT:        //�����������
            {
                CheckBoxAdjustCPT->Checked = true;
                break;
            }
            case drawPV:         //�ݻ�ѹ��ͼ
            {
                CheckBoxAdjustPV->Checked  = true;
                break;
            }
            case drawFFTP:       //ѹ��Ƶ��ͼ
            {
                CheckBoxAdjustFFTP->Checked = true;
                break;
            }
            case drawLogVLogP: //�ݻ�ѹ������ͼ
            {
                CheckBoxAdjustLogVLogP->Checked = true;
                break;
            }
            case drawDPDF:       //ѹ��������
            {
                CheckBoxAdjustDPDF->Checked     = true;
                break;
            }
            case drawHRR:        //����������
            {
                CheckBoxAdjustHRR->Checked      = true;
                break;
            }
            case drawCHRR:       //�ۻ�������
            {
                CheckBoxAdjustCHRR->Checked     = false;
                break;
            }
        }
        LoadCurrentBinPCP(false);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageAllMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   if(!bPSTOpened)
    {
        int i = 1;
        return;
    }
    int xB,yB;
    TPoint pointMouse;
    pointMouse.x = X;
    pointMouse.y = Y;
    bMouseMoving = true;
    if(bRightButton) return;
    if(bPSTDrawOne)
    {
        if(bPBMouseDragging)
        {
            TRect rect, rtTemp;
            TImage* ptPB = (TImage*)Sender;
            rect.Left    = ImageAll->Left;
            rect.Top     = ImageAll->Top;
            rect.Right   = rect.Left + ImageAll->Width;
            rect.Bottom  = rect.Top  + ImageAll->Height;
            if(PtInRect(&rect,pointMouse))
            {
//                ptPB->Canvas->Pen->Color = clRed;
                 ptPB->Enabled = true;
                 ptPB->Canvas->Pen->Mode  = pmNot;
//                ptPB->Canvas->Pen->Width = 1;
//                PBDrawRect(Sender,pointMouseOrigin,pointMouseOld);
                 ptPB->Canvas->MoveTo(pointMouseOrigin.x,  pointMouseOrigin.y);
                 ptPB->Canvas->LineTo(pointMouseOrigin.x,  pointMouseOld.y);
                 ptPB->Canvas->LineTo(pointMouseOld.x,     pointMouseOld.y);
                 ptPB->Canvas->LineTo(pointMouseOld.x,     pointMouseOrigin.y);
                 ptPB->Canvas->LineTo(pointMouseOrigin.x,  pointMouseOrigin.y);
//                ptPB->Canvas->Pen->Mode = pmCopy;
//                PBDrawRect(Sender,pointMouseOrigin,pointMouse);
                 ptPB->Canvas->MoveTo(pointMouseOrigin.x,  pointMouseOrigin.y);
                 ptPB->Canvas->LineTo(pointMouseOrigin.x,  pointMouse.y);
                 ptPB->Canvas->LineTo(pointMouse.x,        pointMouse.y);
                 ptPB->Canvas->LineTo(pointMouse.x,        pointMouseOrigin.y);
                 ptPB->Canvas->LineTo(pointMouseOrigin.x,  pointMouseOrigin.y);
                 pointMouseOld = pointMouse;

/*                 ptPB->Canvas->Pen->Mode = pmCopy;
                 if(pointMouseOrigin.x > pointMouse.x)
                 {
                    rtTemp.Left = pointMouse.x;
                    rtTemp.Right = pointMouseOrigin.x;
                 }
                 else
                 {
                    rtTemp.Left = pointMouseOrigin.x;
                    rtTemp.Right = pointMouse.x;
                 }
                 if(pointMouseOrigin.y > pointMouse.y)
                 {
                    rtTemp.Top = pointMouse.y;
                    rtTemp.Bottom = pointMouseOrigin.y;
                 }
                 else
                 {
                    rtTemp.Top = pointMouseOrigin.y;
                    rtTemp.Bottom = pointMouse.y;
                 }
                 ptPB->Canvas->FillRect(rtTemp);
*/                 
            }
        }
        return;
    }
    if(bMouseDown)
    {
//        
        if(PtInRect(&rect3D,pointMouse))
        {
            if(   PtInRect(&rectMaxValue,pointMouse)
                ||PtInRect(&rectCutValue,pointMouse)
//                ||PtInRect(&rectSingle,pointMouse)
               )
            {
                return;
            }

            if(Shift.Contains(ssCtrl))
         //   if(bRightButton)
            {
                xB = abs(X - ptOriginal.x);
                yB = abs(Y - ptOriginal.y);
                if(xB > yB)
                {
                    if(X < ptOriginal.x)
                    {
                        fXAngle += 2;
                    }
                    else
                    {
                        fXAngle -= 2;
                    }
                }
                else
                {
                    if(Y < ptOriginal.y)
                    {
                        fYAngle += 2;
                    }
                    else
                    {
                        fYAngle -= 2;
                    }
                }
            }
            else if(Shift.Contains(ssShift))
            {
                if(X < ptOriginal.x)
                {
                    iX0Position -= abs(X - ptOriginal.x);
                }
                else
                {
                    iX0Position += abs(X - ptOriginal.x);
                }

                if(Y < ptOriginal.y)
                {
                    iY0Position -= abs(Y - ptOriginal.y);
                }
                else
                {
                    iY0Position += abs(Y - ptOriginal.y);
                }
            }
            else if(PtInRect(&rectSingle,pointMouse))
            {
                if(Y < ptOriginal.y)
                {
                    iYTimeInterval += abs(Y - ptOriginal.y);
                }
                else
                {
                    iYTimeInterval -= abs(Y - ptOriginal.y);
                    if(iYTimeInterval < 10)
                    {
                        iYTimeInterval = 10;
                    }
                }
            }
            else
            {
                TColor clTemp;
                clTemp = ImageAll->Canvas->Pixels[X][Y];
                if(clTemp == clWhite)
                {
                    if(Y < ptOriginal.y)
                    {
                        iY3DInterval += abs(Y - ptOriginal.y);
                    }
                    else
                    {
                        iY3DInterval -= abs(Y - ptOriginal.y);
                        if(iY3DInterval < 10)
                        {
                            iY3DInterval = 10;
                        }
                    }
                }
                else
                {
                    return;
                }
            }
        }
        else if(PtInRect(&rectTime,pointMouse))
        {
            if(X < ptOriginal.x)
            {
                iXTimeInterval -= abs(X - ptOriginal.x);
                if(iXTimeInterval < 1)
                {
                    iXTimeInterval = 1;
                }
            }
            else
            {
                iXTimeInterval   += abs(X - ptOriginal.x);
                if(iXTimeInterval > rectTime.Width() - 1)
                {
                    iXTimeInterval = rectTime.Width() - 1;
                }
            }

            if(Y < ptOriginal.y)
            {
                iYTimeInterval += abs(Y - ptOriginal.y);
//                if(iYTimeInterval < 5)
//                {
//                    iYTimeInterval = 5;
//                }
            }
            else
            {
                iYTimeInterval -= abs(Y - ptOriginal.y);
                if(iYTimeInterval < 10)
                {
                    iYTimeInterval = 10;
                }
            }
        }
        else if(PtInRect(&rectValue,pointMouse))
        {
            if(X < ptOriginal.x)
            {
                iCutPosi -= abs(X - ptOriginal.x);
                if(iCutPosi < iChoiceStart)
                {
                    iCutPosi = iChoiceStart;
                }
            }
            else
            {
                iCutPosi   += abs(X - ptOriginal.x);
                if(iCutPosi > iChoiceEnd - 1)
                {
                    iCutPosi = iChoiceEnd - 1;
                }
            }

            if(Y < ptOriginal.y)
            {
                iYTimeInterval += abs(Y - ptOriginal.y);
//                if(iYTimeInterval < 5)
//                {
//                    iYTimeInterval = 5;
//                }
            }
            else
            {
                iYTimeInterval -= abs(Y - ptOriginal.y);
                if(iYTimeInterval < 10)
                {
                    iYTimeInterval = 10;
                }
            }
        }

        ptOriginal.x = X;
        ptOriginal.y = Y;
        DrawImageAll();
    }
}

void __fastcall TFormMain::ImageAllMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(!bPSTOpened)
    {
        return;
    }
    if(bPSTDrawOne)
    {                     //TImage

        TImage* ptPB = (TImage*)Sender;
        if((Button == mbLeft)&&(bPBMouseDragging))
        {
            int iStart,iEnd;
            int iPenWidth;
            TPenStyle tpsBackup;
            TPoint pointMouse;
            pointMouse.x  = X;
            pointMouse.y  = Y;
            bPBMouseDragging=false;

//            iPenWidth = ptPB->Canvas->Pen->Width;
//            tpsBackup = ptPB->Canvas->Pen->Style;
//            ptPB->Canvas->Pen->Color = clGridLineColor;
//            ptPB->Canvas->Pen->Color = clRed;
            ptPB->Canvas->Pen->Width = 1;
//            ptPB->Canvas->Pen->Style = (TPenStyle)LineType[iGridLineTypeIndex];
             TRect rect, rtTemp;
             rect.Left   = ImageAll->Left;
             rect.Top    = ImageAll->Top;
             rect.Right  = rect.Left + ImageAll->Width;
             rect.Bottom = rect.Top  + ImageAll->Height;
            if(PtInRect(&rect,pointMouse))
            {
                ptPB->Canvas->Pen->Mode = pmNot;
//                ptPB->Canvas->Brush->Color = clGray;
//                ptPB->Canvas->Pen->Color = clGray;
//                PBDrawRect(Sender,pointMouseOrigin,pointMouseOld);
                 TColor clTemp;
                // clTemp =  FillRect
/*
                 ptPB->Canvas->MoveTo(pointMouseOrigin.x,  pointMouseOrigin.y);
                 ptPB->Canvas->LineTo(pointMouseOrigin.x,  pointMouseOld.y);
                 ptPB->Canvas->LineTo(pointMouseOld.x,     pointMouseOld.y);
                 ptPB->Canvas->LineTo(pointMouseOld.x,     pointMouseOrigin.y);
                 ptPB->Canvas->LineTo(pointMouseOrigin.x,  pointMouseOrigin.y);
                 ptPB->Canvas->Pen->Mode = pmCopy;
*/
//                PBDrawRect(Sender,pointMouseOrigin,pointMouse);
                 ptPB->Canvas->MoveTo(pointMouseOrigin.x,  pointMouseOrigin.y);
                 ptPB->Canvas->LineTo(pointMouseOrigin.x,  pointMouse.y);
                 ptPB->Canvas->LineTo(pointMouse.x,        pointMouse.y);
                 ptPB->Canvas->LineTo(pointMouse.x,        pointMouseOrigin.y);
                 ptPB->Canvas->LineTo(pointMouseOrigin.x,  pointMouseOrigin.y);
                 ptPB->Canvas->Pen->Mode = pmCopy;
//                 ptPB->Repaint();
//                 Application->ProcessMessages();
/*
                 if(pointMouseOrigin.x > pointMouse.x)
                 {
                    rtTemp.Left = pointMouse.x;
                    rtTemp.Right = pointMouseOrigin.x;
                 }
                 else
                 {
                    rtTemp.Left = pointMouseOrigin.x;
                    rtTemp.Right = pointMouse.x;
                 }
                 if(pointMouseOrigin.y > pointMouse.y)
                 {
                    rtTemp.Top = pointMouse.y;
                    rtTemp.Bottom = pointMouseOrigin.y;
                 }
                 else
                 {
                    rtTemp.Top = pointMouseOrigin.y;
                    rtTemp.Bottom = pointMouse.y;
                 }
                 ptPB->Canvas->FillRect(rtTemp);
*/                 
                if(fXPSTStep < 0.00001)
                {
                    return;
                }
                int iedge = 40;
                iStart = iChoiceStart + (int)((float)(pointMouseOrigin.x - rect.left - 70)/fXPSTStep+0.5);
                iEnd   = iChoiceStart + (int)((float)(X - rect.left - 70)/fXPSTStep+0.5);
                if((iEnd-iStart)<0)
                {
                    int ittemp;
                    ittemp = iStart;
                    iStart = iEnd;
                    iEnd   = ittemp;

                }
                int bNoDraw = false;
                if((iEnd-iStart)>30)
                {
                    iChoiceStart  = iStart;
                    iChoiceEnd    = iEnd;
                    bBackMousePoint = false;
                }
                else
                {
                    if((iChoiceStart == 0)&&( iChoiceEnd == lDS.iPLength))
                    {
                        bNoDraw = true;
                    }
                    else
                    {
                        fstartrt = 0.0;
                        fendrt   = 1.0;
                        iChoiceStart = 0;
                        iChoiceEnd   = lDS.iPLength;
                    }
                }
                bPBMouseDragging = false;
                int max,itemp;
                max   = lDS.iPLength - (iChoiceEnd-iChoiceStart);
                itemp = max/10;
                if(itemp == 0)
                {
                    itemp = 1;
                }
            	if(max > 10)
                {
    	            ScrollBarPST->Max      = max;
            	    ScrollBarPST->Position = iChoiceStart;
                }
                else
                {
                    if(!bNoDraw)
                    {
                        fstartrt = 0.0;
                        fendrt   = 1.0;
                       // ptPB->Refresh();
                        DrawImageAll();
                    }
                }
            }
            ptPB->Canvas->Pen->Width = iPenWidth;
            ptPB->Canvas->Pen->Style = tpsBackup;
            Application->ProcessMessages();
        }
        return;
    }
    bMouseDown = false;
    TPoint pointMouse;
    pointMouse.x = X;
    pointMouse.y = Y;
/*
    if(CheckBox3D->Checked)
    {
        TRect rect3d;
        if(PtInRect(&rect3D,pointMouse))
        {
            if((ptOriginal.x == X)&&(ptOriginal.y == Y))
            {
                clSelectedColor = ImageAll->Canvas->Pixels[X][Y];    //TCanvas
                if(  (clSelectedColor == clWhite)
                   ||(clSelectedColor == RGB(220,220,220))
                  )
                {
                    return;
                }
                else
                {
                    for(int i = 0; i < iPSTFileNum; i++)
                    {
                        if(clSelectedColor == mainCurveColor[i])    //CURVECOLOR[]��drawcoor�ж���
                        {
                            iSelectedItem = i;
                            break;
                        }
                    }
                    DrawImageAll();
                }
            }
        }
    }
*/    
    bMouseMoving = false;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::DrawImageAll(void)
{
    //TODO: Add your source code here

    if(!bPSTOpened)    //�����û��װ���б��ļ������˳�
    {
        return;
    }
    TCursor saveCursor;
    saveCursor = Screen->Cursor;

    try
    {         //TBitmap
        Screen->Cursor = crHourGlass;

        TImage* imageCompare;
        imageCompare    = new TImage(NULL);
        if(imageCompare == NULL)
        {
            Application->MessageBox("��ͼʱ�ڴ�������!","��ʾ",MB_OK|MB_ICONINFORMATION);
            return;
        }
        imageCompare->Visible = false;
        imageCompare->Height  = ImageAll->Height;
        imageCompare->Width   = ImageAll->Width;
        imageCompare->Top     = 0;
        imageCompare->Left    = 0;

        iCurrentDraw   = 2;
        TRect rect;
        rect.Left      = ImageAll->Left;
        rect.Top       = ImageAll->Top;
        rect.Bottom    = ImageAll->Height;
        rect.Right     = ImageAll->Width;
        imageCompare->Canvas->Brush->Color = clBackground ;
        imageCompare->Canvas->FillRect(rect);

        ImageAll->Canvas->Brush->Color = clBackground ;
        ImageAll->Canvas->FillRect(rect);
        emDDevice = ddImage;                //����ȷ������TImage�ؼ��ϻ���ͼ�Ρ�

        rectTime.Left   = rect.Left + 4;
        rectTime.Top    = rect.Top + 4;
        rectTime.Right  = (rect.Right - rect.Left)/2 - 5;
        rectTime.Bottom = rect.Top + (rect.Height())/4;

        rectValue.Left   = rectTime.Right + 10;
        rectValue.Top    = rect.Top + 4;
        rectValue.Right  = rect.Right - 8;
        rectValue.Bottom = rectTime.Bottom;

        rectMaxValue.Left   = rect.Left + 4;
        rectMaxValue.Top    = rectTime.Bottom + 8;
        rectMaxValue.Right  = (rect.Right - rect.Left)*1.1/5 - 8;
        rectMaxValue.Bottom = rectTime.Bottom + 10 + (rect.Height())/2;


        rectCutValue.Top    = rectValue.Bottom + 8;
        rectCutValue.Right  = rect.Right - 8;
        rectCutValue.Left   = rect.Right - (rect.Right - rect.Left)*1.1/5;
        rectCutValue.Bottom = rectMaxValue.Bottom;

// ���ڻ��Ƶ���ͼ�ε�����
        rectSingle.Left     = rectMaxValue.Right + 10;
        rectSingle.Right    = rectCutValue.Left  - 10;
        rectSingle.Top      = rectMaxValue.Top;
        rectSingle.Bottom   = rectMaxValue.Top + rectValue.Height();

        rect3D.Left   = rect.Left;
        rect3D.Top    = rectTime.Bottom;
        rect3D.Right  = rect.Right;
        rect3D.Bottom = rect.Bottom;
        strStatusInfo  = "";

        if( iListDrawKind == 1)
        {
            ListBoxPST->Items->LoadFromFile(strPSTName);
            ListBoxPST->Refresh();
            iChoicedNum = ListBoxPST->Items->Count;
/*
            if(CheckBox3D->Checked)   //����3Dͼ��
            {
                DrawPSTAll3D(imageCompare,rect);
            }
            else
            {

            }
*/            
            DrawPSTAll(imageCompare,rect);
        }
        else
        {
            DrawPSTAll(imageCompare, rect);
        }
/*
        if( AllDrawKind == drawDetect )
        {
            ImageAll->Canvas->CopyRect(rect, ImagePipe->Canvas, rect);
        }
        else
        {

        }
*/
        ImageAll->Canvas->CopyRect(rect, imageCompare->Canvas, rect);
        RefreshStatus(strStatusInfo);
        bAdjustHRR     = false;
        bFileOpened    = false;

        bPSTDrawed      = true;
        delete imageCompare;
    }
    __finally
    {
        Screen->Cursor = saveCursor;

    }
}
void __fastcall TFormMain::ppmGridClick(TObject *Sender)
{
//    ppmConGrid->Checked = !ppmConGrid->Checked;
    bPCPGridLine = !bPCPGridLine;
    if(bPCPGridLine)
    {
        ppmGrid->Caption = "ȡ��������";
    }
    else
    {
        ppmGrid->Caption = "����������";
    }
    if(PageControlCurve->ActivePageIndex == 0)
    {
        PaintBoxAdjustTDC->Refresh();
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


void __fastcall TFormMain::PanelAllResize(TObject *Sender)
{
    if(!bStarted)
    {
        return;
    }
    if(!bPSTOpened)  //�����û�д�ѹ���ļ����˳�
    {
        return;
    }
    
    TCursor Save_Cursor = Screen->Cursor ;
    try
    {
        Screen->Cursor = crHourGlass;

        if(iListDrawKind != 1)
        {
            return;
        }
        bFileChanged = true;
        DrawImageAll();
    }
    __finally
    {
        Screen->Cursor = Save_Cursor;
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ppmAddToListClick(TObject *Sender)
{
    AnsiString strTemp,strTempOld;
    int idret;
    int iNo_New, iNo_Old,iCount;
    bool bInserted = false;
    idret = Application->MessageBox("�滻�б��е���Ӧ��������\n\n'��'��ʾ���滻; '��'��ʾ���ӵ����滻\n\n\n�˴����滻��ӱ�ʾ���ǽ���ǰѡ���PCP�ļ����ӵ���ǰ���ڴ�״̬���б��ļ���!!!","��ʾ",MB_ICONQUESTION|MB_YESNOCANCEL);
    if( idret == IDYES)
    {
        strTemp = FileListBoxFile->FileName;
        iNo_New = GetiDieselNum(strTemp);         //��ȡ��ǰ�ļ��ĸ׺�
        iCount = ListBoxPCPFile->Items->Count;
        for( int i = 0; i < iCount; i ++)
        {
            strTempOld = ListBoxPCPFile->Items->Strings[i];
            iNo_Old    = GetiDieselNum(strTempOld);
            if(iNo_Old == iNo_New)
            {
                ListBoxPCPFile->Items->Delete(i);
                ListBoxPCPFile->Items->Insert(i,strTemp);
                bInserted = true;
                break;
            }
        }
        if(!bInserted)
        {
            ListBoxPCPFile->Items->Add(strTemp);
        }
        ListBoxPCPFile->Items->SaveToFile(strPSTName);
    }
    else if(idret == IDNO)
    {
        strTemp = FileListBoxFile->FileName;
        ListBoxPCPFile->Items->Add(strTemp);
        ListBoxPCPFile->Items->SaveToFile(strPSTName);
    }
    if( idret != IDCANCEL)
    {
        bFileChanged = true;
        bPCPEntered  = false;
        bool bHave = false;
        int iCount;
        iListDrawKind = 1;
        strValidPath = ExtractFilePath(strPSTName);
        iCount = ListBoxPSTFile->Items->Count;
        for(int i = 0; i< iCount; i++)
        {
            strTemp = ListBoxPSTFile->Items->Strings[i];
            if(strTemp == strPSTName)
            {
                ListBoxPSTFile->ItemIndex = i;
                bHave = true;
                break;
            }
        }
        if(!bHave)
        {
            ListBoxPSTFile->Items->Add(strPSTName);
            ListBoxPSTFile->ItemIndex = ListBoxPSTFile->Items->Count - 1 ;
        }
        ListBoxPCPFile->Items->LoadFromFile(strPSTName);
        bLoadList = true;
        LoadCurrentBinPST();
        iChoiceStart = 0;
        iChoiceEnd   = lDS.iPLength;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmDeleteFromListClick(TObject *Sender)
{
    int iIndex;
    iIndex = ListBoxPCPFile->ItemIndex;
    if(iIndex < 0)
    {
        return;
    }
    ListBoxPCPFile->Items->Delete(iIndex);
    ListBoxPCPFile->Items->SaveToFile(strPSTName);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::mnuFileSaveClick(TObject *Sender)
{
    if(AllDrawKind != drawDetect)
    {
        return;
    }
    if(!bPSTOpened)    //�����û��װ���б��ļ������˳�
    {
        return;
    }
    AnsiString strTemp;
    int index;
    FILE* fp;
    strTemp = strPSTName;
    index = strTemp.Pos(".");
    strTemp = strTemp.SubString(1,index);
    strTemp += "txt";
    if((fp = fopen(strTemp.c_str(), "w")) != NULL)
    {
        fprintf(fp,"�� ��ѹ    ��ѹ ƽ��ָ ѹ���� ѹ���� ����   ����   ȼ��   ȼ��   ������ ������ �ۻ�   ���� ���ͻ�\n");
        fprintf(fp,"�� ���ֵ  ��λ ʾѹ�� ���ֵ ��λ   ��ǰ�� ������ ��ʼ�� ������ ���ֵ ��λ   ������ ���� ת��\n");
        fprintf(fp,"n  MPa     CA   MPa    MPa/CA  CA     CA     CA     CA     CA    kJ/CA  CA     kJ      kJ  r/min\n");
        for( int i = 0; i < maxBuffer.iCyNums; i++)
        {
            fprintf(fp,"%02d %7.2f %-5.1f %-7.2f %-7.3f %-7.1f %-7.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n",
                        maxBuffer.siarDieselNum[i],          //1  �׺�
                        maxBuffer.farPMax[i],                //2  ����ѹ
                        maxBuffer.farMaxPAngle[i],           //3  ����ѹ�ĽǶ�
                        maxBuffer.farAverageP[i],            //4  ƽ��ָʾѹ��
                        maxBuffer.farDPDFMax[i],             //5  ѹ�������ֵ
                        maxBuffer.farMaxDPAngle[i],          //6  ѹ�������ֵ�Ƕ�
                        maxBuffer.farOilAngle[i],            //7  ������ǰ��
                        maxBuffer.farWavePeriod[i],          //8  ���ͳ�����
                        maxBuffer.farCombustionAngle[i],     //9  ȼ�տ�ʼ��
                        maxBuffer.farCombustionPeriod[i],    //10 ȼ�ճ�����
                        maxBuffer.farDHDFMax[i],             //11 ���������ֵ
                        maxBuffer.farMaxDHAngle[i],          //12 ���������ֵ�ĽǶ�
                        maxBuffer.farCDHDFMax[i],            //13 �ۻ�������
                        maxBuffer.farCPower[i],              //14 ��������
                        maxBuffer.farEngineSpeed[i]);        //15 ���ͻ�ת��
        }
        fclose(fp);

        try
        {
            ::ShellExecute(Handle,"open","c:\\windows\\Notepad.exe",strTemp.c_str(),NULL,SW_SHOWNORMAL);
        }
        catch(...)
        {
        }

//        strTemp += "\n\n�ѱ���!!";
//        Application->MessageBox(strTemp.c_str(),"��ʾ",MB_OK|MB_ICONINFORMATION);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmPSTSortClick(TObject *Sender)
{
    int iIndex,iCount,iCountFile;
    iCount      = ListBoxPSTFile->Items->Count;
    iCountFile  = ListBoxPCPFile->Items->Count;
    iIndex      = ListBoxPSTFile->ItemIndex;
    if((iCount == 0)||(iIndex<0)||(iCountFile==0))
    {
        return;
    }
    AnsiString strFN;
    strFN  = ListBoxPSTFile->Items->Strings[iIndex];
    ListBoxPCPFile->Sorted = true;
    ListBoxPCPFile->Refresh();
    ListBoxPCPFile->Items->SaveToFile(strFN);
    ListBoxPSTFileDblClick(NULL);
    ListBoxPCPFile->Sorted = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmPipeToBaseClick(TObject *Sender)
{
    AnsiString strTemp,strNameTemp,strPCPBackup;
    bool bdatabasestatus;
    int iindex,icount;
    icount = ListBoxPCPFile->Items->Count;
    if(icount <= 0)
    {
        return;
    }
    iindex = ListBoxPCPFile->ItemIndex;
    if(iindex < 0)
    {
        return;
    }
    strTemp = ListBoxPCPFile->Items->Strings[iindex];
    InsertToPipeBase(strTemp);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::mnuHelpSoftwareClick(TObject *Sender)
{
    AnsiString strTemp;
    bool bnotepad;
    String strExeFileName = ::ParamStr(0);
    String strNowPath     = ::ExtractFilePath(strExeFileName);
    strNowPath           += "SoftMainHelp.chm";       //�����ļ�����
    Application->HelpFile = strNowPath;
//    Application->HelpCommand(HELP_CONTENTS,0);
    HtmlHelpA(
                 GetDesktopWindow(),
                 strNowPath.c_str(),
                 HH_DISPLAY_TOPIC,
                 NULL
                 ) ;
/*
    strTemp = "c:\\windows\\hh.exe";
    bnotepad = FileExists(strTemp);
    if(bnotepad)
    {
        try
        {
            ::ShellExecute(Handle,"open",strTemp.c_str(),strNowPath.c_str(),NULL,SW_SHOWNORMAL);
        }
        catch(...)
        {
        }
    }
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    if(bExitOut)
    {
        CanClose = true;
        return;
    }
    int ExitCode = Application->MessageBox("������˳���",
                                         "��ʾ", MB_YESNO|MB_ICONWARNING);
    switch(ExitCode)
    {
        case IDYES:
        {
            CanClose =  true;  //add code for saving
            break;
        }
        case IDNO:
        {
            CanClose = false;  // skip the saving and just close
            break;
        }
    }
}
//---------------------------------------------------------------------------




void __fastcall TFormMain::Params_Save(void)
{
//TODO: Add your source code here
//������������

    FILE* fp;
    void* ptVoid;
    AnsiString strTemp;
    strTemp = strExeFilePath + "Params.ini";
    if((fp = fopen(strTemp.c_str(), "wb")) != NULL)
    {
        for(int i = 0; i < 11; i++)
        {
            ptVoid = (void*)(&myDieselInfo[i]);
            fwrite( ptVoid,
                    sizeof(StructDieselInfo),
                    1,
                    fp
                    );
        }
        fclose(fp);
    }
    IniFileWrite();
}

void __fastcall TFormMain::Params_Load(void)
{
//TODO: Add your source code here
//�����������
        FILE* fp;
    void* ptVoid;
    AnsiString strTemp;
    strTemp = strExeFilePath + "Params.ini";
    if(FileExists(strTemp))
    {
        if((fp = fopen(strTemp.c_str(), "rb")) != NULL)
        {
            for(int i = 0; i < 11; i++)
            {
                ptVoid = (void*)(&myDieselInfo[i]);
                fread( ptVoid,
                    sizeof(StructDieselInfo),
                    1,
                    fp
                    );
            }
            fclose(fp);
        }
    }
}



void __fastcall TFormMain::BitBtnArrayMDateClick(TObject *Sender)
{
    TCursor cur = Screen->Cursor;
    Screen->Cursor = crHourGlass;
    RefreshTViewByDate();
    Screen->Cursor = cur;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnArrayTrainTypeClick(TObject *Sender)
{
    TCursor cur = Screen->Cursor;
    Screen->Cursor = crHourGlass;
    RefreshTViewByTrainType();
    Screen->Cursor = cur;    
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::RefreshTViewByDate(void)
{
    //TODO: Add your source code here
    AnsiString strTempTT,     strTempTN, strTempTM, strTempCM,strTemp;
    AnsiString strChild,      strChildName;
    AnsiString strsCombustion, strsDate;
    AnsiString strEngineSpeed;
    AnsiString strGrandChild, strGrandChildName;
    TTreeNode* gNode, *lastNode;
    TTreeNode *rNode,*mNode,*sNode,*gsNode,*cmNode;
    AnsiString strTrainID;
    int iImageIndex;

    if(DM->ADOTableMInfo->Active)
    {
        DM->ADOTableMInfo->Close();
    }
    TreeViewTrain->Items->Clear();
    rNode = TreeViewTrain->Items->Add(TreeViewTrain->Selected,"��ѹ�������");  //������ڵ�
    rNode->ImageIndex    = 15;    //û��չ��ǰ��ͼ��
    rNode->SelectedIndex = 15;    //չ���Ժ��ͼ��
    
//(1)�Ȱ�ʱ����м���
    strTempTT = "select distinct MDate from MeasureInfo order by MDate";           
    DM->ADOQueryDate->Close();
    DM->ADOQueryDate->SQL->Clear();
    DM->ADOQueryDate->SQL->Add (strTempTT);
    DM->ADOQueryDate->Open();
    int dataLength = DM->ADOQueryDate->RecordCount;
    if(dataLength == 0)
    {
        DM->ADOQueryDate->Close();
        return;
    }
    DM->ADOQueryDate->First();
    while(!DM->ADOQueryDate->Eof)   //�����������
    {
        strTempTM = DM->ADOQueryDate->Fields->Fields[0]->Value;
        ReformDate(strTempTM);
        mNode     = TreeViewTrain->Items->AddChild(rNode,strTempTM);
        FormDate(strTempTM);
        mNode->ImageIndex    = 18;
        mNode->SelectedIndex = 18;
        strTemp = "select distinct TrainType from MeasureInfo where MDate = '";
        strTemp += strTempTM;
        strTemp += "' order by TrainType";
        DM->ADOQueryTrainType->Close();
        DM->ADOQueryTrainType->SQL->Clear();
        DM->ADOQueryTrainType->SQL->Add(strTemp);
        DM->ADOQueryTrainType->Open();
        DM->ADOQueryTrainType->First();

//(2)�����������
        while(!DM->ADOQueryTrainType->Eof)
        {
            strTempTT = DM->ADOQueryTrainType->Fields->Fields[0]->Value;
            gNode = TreeViewTrain->Items->AddChild(mNode,strTempTT);

            if(strTempTT == "����4A")
            {
                gNode->ImageIndex    = 0;
                gNode->SelectedIndex = 0;
                iImageIndex          = 0;
            }
            else if(strTempTT == "����4B")
            {
                gNode->ImageIndex    = 1;
                gNode->SelectedIndex = 1;
                iImageIndex          = 1;
            }
            else if(strTempTT == "����4C")
            {
                gNode->ImageIndex    = 2;
                gNode->SelectedIndex = 2;
                iImageIndex          = 2;
            }
            else if(strTempTT == "����4D")
            {
                gNode->ImageIndex    = 3;
                gNode->SelectedIndex = 3;
                iImageIndex          = 3;
            }
            else if(strTempTT == "����8B")
            {
                gNode->ImageIndex    = 5;
                gNode->SelectedIndex = 5;
                iImageIndex          = 5;
            }
            else if(strTempTT == "����11")
            {
                gNode->ImageIndex    = 6;
                gNode->SelectedIndex = 6;
                iImageIndex          = 6;
            }
            else
            {
                gNode->ImageIndex    = 4;
                gNode->SelectedIndex = 4;
                iImageIndex          = 4;
            }
            DM->ADOQueryTrainNumber->Close();
            strTemp = "select distinct trainNumber from MeasureInfo where TrainType = '";
            strTemp += strTempTT;
            strTemp += "' and MDate = '";
            strTemp += strTempTM;
            strTemp += "' order by trainNumber";
            DM->ADOQueryTrainNumber->SQL->Clear();
            DM->ADOQueryTrainNumber->SQL->Add(strTemp);
            DM->ADOQueryTrainNumber->Open();
            DM->ADOQueryTrainNumber->First();

//(3)�����������
            while(!DM->ADOQueryTrainNumber->Eof)
            {
                strTempTN = DM->ADOQueryTrainNumber->Fields->Fields[0]->Value;
                gsNode = TreeViewTrain->Items->AddChild(gNode,strTempTN);
                gsNode->ImageIndex    = 20;
                gsNode->SelectedIndex = 20;

//--------------------------------------------------------------------------------
//(4) �Բ��ͻ�ȼ��״̬���в�ѯ
                DM->ADOQueryTemp->Close();    //��ʼ��ȼ��״̬���в�ѯ
                strTemp = "select distinct CombustionMode from MeasureInfo where TrainType = '";
                strTemp += strTempTT;
                strTemp += "' and trainNumber = '";
                strTemp += strTempTN;
                strTemp += "' and MDate = '";
                strTemp += strTempTM;
                strTemp += "' order by CombustionMode";
                DM->ADOQueryTemp->SQL->Clear();
                DM->ADOQueryTemp->SQL->Add(strTemp);
                DM->ADOQueryTemp->Open();
                DM->ADOQueryTemp->First();
                while(!DM->ADOQueryTemp->Eof)
                {
                    strTempCM = DM->ADOQueryTemp->Fields->Fields[0]->Value;
                    sNode = TreeViewTrain->Items->AddChild(gsNode,strTempCM);
                    if(strTempCM == "ȼ�չ���")
                    {
                        sNode->ImageIndex    = 17;
                        sNode->SelectedIndex = 17;
                    }
                    else
                    {
                        sNode->ImageIndex    = 19;
                        sNode->SelectedIndex = 19;
                    }
                    DM->ADOQueryTemp->Next();
                }
//================================================================================

                DM->ADOQueryTrainNumber->Next();
            }
            DM->ADOQueryTrainType->Next();
        }
        DM->ADOQueryDate->Next();
    }
    if(DM->ADOQueryTrainType->Active)
    {
        DM->ADOQueryTrainType->Close();
    }
    if(DM->ADOQueryDate->Active)
    {
        DM->ADOQueryDate->Close();
    }
    if(DM->ADOQueryTrainNumber->Active)
    {
        DM->ADOQueryTrainNumber->Close();
    }
    if(DM->ADOQueryTemp->Active)
    {
        DM->ADOQueryTemp->Close();
    }
/*
    if(DM->ADOQueryFName->Active)
    {
        DM->ADOQueryFName->Close();
    }
*/
    bDateBrowseStyle = true;
    GroupBoxBrowse->Caption = " ��Ϣ(��������ʾ) ";
    rNode->Expand(false);
}

void __fastcall TFormMain::RefreshTViewByTrainType(void)
{
    //TODO: Add your source code here
    AnsiString strTempTT,     strTempTN, strTempTM,strTemp, strTempCM;
    AnsiString strChild,      strChildName;
    AnsiString strsCombustion, strsDate;
    AnsiString strEngineSpeed;
    AnsiString strGrandChild, strGrandChildName;
    TTreeNode* gNode, *lastNode;
    TTreeNode *rNode,*mNode,*sNode,*gsNode,*cmNode;
    unsigned short usYear, usMonth, usDate;
    AnsiString strTrainID;
    int iImageIndex;

    if(DM->ADOTableMInfo->Active)
    {
        DM->ADOTableMInfo->Close();
    }
    TreeViewTrain->Items->Clear();
    rNode = TreeViewTrain->Items->Add(TreeViewTrain->Selected,"��ѹ�������");  //������ڵ�
    rNode->ImageIndex    = 15;    //û��չ��ǰ��ͼ��
    rNode->SelectedIndex = 15;    //չ���Ժ��ͼ��
    
//(1)�Ȱ��������ͽ��м���
    strTemp = "select distinct TrainType from MeasureInfo order by TrainType";
    DM->ADOQueryTrainType->Close();
    DM->ADOQueryTrainType->SQL->Clear();
    DM->ADOQueryTrainType->SQL->Add (strTemp);
    DM->ADOQueryTrainType->Open();
    int dataLength = DM->ADOQueryTrainType->RecordCount;
    if(dataLength == 0)
    {
        DM->ADOQueryTrainType->Close();
        return;
    }
    DM->ADOQueryTrainType->First();
    while(!DM->ADOQueryTrainType->Eof)   //�����������
    {
        strTempTT = DM->ADOQueryTrainType->Fields->Fields[0]->Value;
        mNode     = TreeViewTrain->Items->AddChild(rNode,strTempTT);

            if(strTempTT == "����4A")
            {
                mNode->ImageIndex    = 0;
                mNode->SelectedIndex = 0;
                iImageIndex          = 0;
            }
            else if(strTempTT == "����4B")
            {
                mNode->ImageIndex    = 1;
                mNode->SelectedIndex = 1;
                iImageIndex          = 1;
            }
            else if(strTempTT == "����4C")
            {
                mNode->ImageIndex    = 2;
                mNode->SelectedIndex = 2;
                iImageIndex          = 2;
            }
            else if(strTempTT == "����4D")
            {
                mNode->ImageIndex    = 3;
                mNode->SelectedIndex = 3;
                iImageIndex          = 3;
            }
            else if(strTempTT == "����8B")
            {
                mNode->ImageIndex    = 5;
                mNode->SelectedIndex = 5;
                iImageIndex          = 5;
            }
            else if(strTempTT == "����11")
            {
                mNode->ImageIndex    = 6;
                mNode->SelectedIndex = 6;
                iImageIndex          = 6;
            }
            else
            {
                mNode->ImageIndex    = 4;
                mNode->SelectedIndex = 4;
                iImageIndex          = 4;
            }

        strTemp = "select distinct TrainNumber from MeasureInfo where TrainType = '";
        strTemp += strTempTT;
        strTemp += "' order by TrainNumber";
        DM->ADOQueryTrainNumber->Close();
        DM->ADOQueryTrainNumber->SQL->Clear();
        DM->ADOQueryTrainNumber->SQL->Add(strTemp);
        DM->ADOQueryTrainNumber->Open();
        DM->ADOQueryTrainNumber->First();

//(2)�����������
        while(!DM->ADOQueryTrainNumber->Eof)
        {
            strTempTN = DM->ADOQueryTrainNumber->Fields->Fields[0]->Value;
            gNode = TreeViewTrain->Items->AddChild(mNode,strTempTN);
            gNode->ImageIndex    = 20;
            gNode->SelectedIndex = 20;

            DM->ADOQueryDate->Close();
            strTemp = "select distinct MDate from MeasureInfo where TrainType = '";
            strTemp += strTempTT;
            strTemp += "' and TrainNumber = '";
            strTemp += strTempTN;
            strTemp += "' order by MDate";
            DM->ADOQueryDate->SQL->Clear();
            DM->ADOQueryDate->SQL->Add(strTemp);
            DM->ADOQueryDate->Open();
            DM->ADOQueryDate->First();

//(3)�����������

            while(!DM->ADOQueryDate->Eof)
            {
                strTempTM = DM->ADOQueryDate->Fields->Fields[0]->Value;
                ReformDate(strTempTM);
                gsNode = TreeViewTrain->Items->AddChild(gNode,strTempTM);

                gsNode->ImageIndex    = 18;
                gsNode->SelectedIndex = 18;
/*
        strTempTM = DM->ADOQueryDate->Fields->Fields[0]->Value;
        ReformDate(strTempTM);
        mNode     = TreeViewTrain->Items->AddChild(rNode,strTempTM);
*/
                FormDate(strTempTM);
//--------------------------------------------------------------------------------
//(4) �Բ��ͻ�ȼ��״̬���в�ѯ
                DM->ADOQueryTemp->Close();    //��ʼ��ȼ��״̬���в�ѯ
                strTemp = "select distinct CombustionMode from MeasureInfo where TrainType = '";
                strTemp += strTempTT;
                strTemp += "' and trainNumber = '";
                strTemp += strTempTN;
                strTemp += "' and MDate = '";
                strTemp += strTempTM;
                strTemp += "' order by CombustionMode";
                DM->ADOQueryTemp->SQL->Clear();
                DM->ADOQueryTemp->SQL->Add(strTemp);
                DM->ADOQueryTemp->Open();
                DM->ADOQueryTemp->First();
                while(!DM->ADOQueryTemp->Eof)
                {
                    strTempCM = DM->ADOQueryTemp->Fields->Fields[0]->Value;
                    sNode = TreeViewTrain->Items->AddChild(gsNode,strTempCM);
                    if(strTempCM == "ȼ�չ���")
                    {
                        sNode->ImageIndex    = 17;
                        sNode->SelectedIndex = 17;
                    }
                    else
                    {
                        sNode->ImageIndex    = 19;
                        sNode->SelectedIndex = 19;
                    }
                    DM->ADOQueryTemp->Next();
                }
//================================================================================
                DM->ADOQueryDate->Next();
            }
            DM->ADOQueryTrainNumber->Next();
        }
        DM->ADOQueryTrainType->Next();
    }
    if(DM->ADOQueryTrainType->Active)
    {
        DM->ADOQueryTrainType->Close();
    }
    if(DM->ADOQueryDate->Active)
    {
        DM->ADOQueryDate->Close();
    }
    if(DM->ADOQueryTrainNumber->Active)
    {
        DM->ADOQueryTrainNumber->Close();
    }
    if(DM->ADOQueryTemp->Active)
    {
        DM->ADOQueryTemp->Close();
    }
    bDateBrowseStyle = false;
    GroupBoxBrowse->Caption = " ��Ϣ(��������ʾ) ";
    rNode->Expand(false);
}
void __fastcall TFormMain::ppmAddToMeassureInfoDatabaseClick(
      TObject *Sender)
{
    TSearchRec srD,srF;
    String strFileFore,strLIST, strTemp,strDName,strFPath,strFName;
    String strFileExtend    = "*";
    String strfilepath      = DirectoryListBoxFile->Directory;
    String strfilename      = ::ExtractFileName(strfilepath);

    TCursor  backCursor1;
    int iTrainTypeBackup;
    backCursor1             = Screen->Cursor;

    Screen->Cursor          = crHourGlass;
    Application->ProcessMessages();

    strfilename = strfilename.UpperCase();
    if(strfilename == "����4A")
    {
        iMainTrainType = 0;
    }
    else if(strfilename == "����4B")
    {
       iMainTrainType = 1;
    }
    else if(strfilename == "����4C")
    {
       iMainTrainType = 2;
    }
    else if(strfilename == "����4D")
    {
       iMainTrainType = 3;
    }
    else if(strfilename == "����4D")
    {
       iMainTrainType = 4;
    }
    else if(strfilename == "����7C")
    {
       iMainTrainType = 5;
    }
    else if(strfilename == "����8B")
    {
       iMainTrainType = 6;
    }
    else if(strfilename == "����11")
    {
       iMainTrainType = 7;
    }
   else if(strfilename == "����11G")   iMainTrainType = 8;
   else if(strfilename == "����11Z")   iMainTrainType = 9;
/*
   else if(strfilename == strOtherA) iMainTrainType = 10;
   else if(strfilename == strOtherB) iMainTrainType = 11;
   else if(strfilename == strOtherC) iMainTrainType = 12;
   else if(strfilename == strOtherD) iMainTrainType = 13;
   else if(strfilename == strOtherE) iMainTrainType = 14;
   else                              iMainTrainType = -1;
*/
    strfilepath += "\\";
    strDName = strfilepath + strFileExtend;
    int iDirectoryNum      = 0;
    int iFileNum           = 0;

    if(iMainTrainType == -1)
    {
        Application->MessageBox("·�����ú��񲻶ԣ�û�пɴ������ļ�!\n\n��ѡ��������!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
        Screen->Cursor = backCursor1;
        return;
    }
    else
    {
        iTrainTypeBackup = iMainTrainType;
        strTemp = "ȷʵҪ������·����ѹ�����������������Ϣ���ݿ���?\n\n";
        strTemp += DirectoryListBoxFile->Directory;

        int iresult = MessageDlg(strTemp.c_str(), mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0);
        if(iresult == mrNo)
        {
            Screen->Cursor = backCursor1;
            return;
        }
    }

    if(FindFirst(strDName,faDirectory,srD) == 0)
    {
       do
       {
            strFileFore = srD.Name;
            if(strFileFore.Length()<3)
            {
                continue;
            }
/*
            strFPath=strfilepath + strFileFore+"\\";
            strFName=strfilepath + strFileFore+"\\*.pst";
*/
            strFPath = strfilepath + strFileFore + "\\";
            strFName = strfilepath + strFileFore + "\\*.pst";
            if(FindFirst(strFName,faArchive,srF)==0)
            {
                do
                {
                    strLIST = srF.Name;       //strLIST�б�������б��ļ���
                    if(strLIST.Length()<3)
                    {
                        continue;
                    }
                    strLIST = strFPath + strLIST;
                    AddToMeasureInfo( strLIST );
                }while(FindNext(srF)==0);
                FindClose(srF);
            }

       }while (FindNext(srD) == 0);

       FindClose(srD);
       Application->MessageBox("�����������!","��ʾ",MB_OK|MB_ICONINFORMATION);
    }

    else
    {
        Application->MessageBox("·�����ú��񲻶ԣ�û�пɴ������ļ�!","��ʾ",MB_OK|MB_ICONINFORMATION);
    }

    Screen->Cursor = backCursor1;
    Application->ProcessMessages();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::AddToMeasureInfo(AnsiString& strlist)
{
    //TODO: Add your source code here
    int iposi, ilen;
    AnsiString strTType, strMM, strTNum,strTemp,strDSpeed,strCMode;
    strTemp  = ::ExtractFilePath(strlist);

// 1 ��������
    strTemp  = strTemp.SubString(1, strTemp.Length() - 1);
    strTNum  = ::ExtractFileName(strTemp);

// 2 ��������
    strTemp  = ::ExtractFilePath(strTemp);
    strTemp  = strTemp.SubString(1, strTemp.Length() - 1);
    strTType = ::ExtractFileName(strTemp);

// 3 ȼ��״̬
    strTemp = ::ExtractFileName(strlist);
    iposi   = strTemp.Pos("_");
    strCMode = strTemp.SubString( iposi + 1, 8);

// 4 ���ͻ�ת��
    iposi   = strTemp.Pos(" ");
    ilen    = strTemp.Length();
    strTemp = strTemp.SubString(iposi, ilen - iposi);
    iposi    = strTemp.Pos("rpm ");
    strDSpeed = strTemp.SubString(2, iposi - 2);

//  5 ����ʱ��
    strMM   = strTemp.SubString(iposi + 4, 10);

    strTemp = "select filename from MeasureInfo where TrainType = '";
    strTemp += strTType + "' and TrainNumber = '";
    strTemp += strTNum + "' and MDate = '";
    strTemp += strMM + "' and DieselSpeed = '";
    strTemp += strDSpeed + "'";

    DM->ADOQueryTemp->Close();
    DM->ADOQueryTemp->SQL->Clear();
    DM->ADOQueryTemp->SQL->Add(strTemp);
    DM->ADOQueryTemp->Open();
    int ilength = DM->ADOQueryTemp->RecordCount;
    DM->ADOQueryTemp->Close();
    if( ilength != 0)   //���ԭ����������ɾ�����������µ����ݡ� ��
    {
        strTemp = "delete from MeasureInfo where TrainType = '";
        strTemp += strTType + "' and TrainNumber = '";
        strTemp += strTNum  + "' and MDate = '";
        strTemp += strMM    + "' and DieselSpeed = '";
        strTemp += strDSpeed + "'";
        try
        {
            DM->ADOQueryTemp->SQL->Clear();
            DM->ADOQueryTemp->SQL->Add(strTemp);
            DM->ADOQueryTemp->ExecSQL();
            DM->ADOQueryTemp->Close();
        }
        catch(...)
        {

        }
    }

    if(!DM->ADOTableMInfo->Active)
    {
        DM->ADOTableMInfo->Open();
    }
    if(DM->ADOTableMInfo->CanModify)
    {
        try
        {
            DM->ADOTableMInfo->Append();
            DM->ADOTableMInfo->FieldByName("MDate")->AsString          = strMM;
            DM->ADOTableMInfo->FieldByName("TrainType")->AsString      = strTType;
            DM->ADOTableMInfo->FieldByName("TrainNumber")->AsString    = strTNum;
            DM->ADOTableMInfo->FieldByName("DieselSpeed")->AsString    = strDSpeed;
            DM->ADOTableMInfo->FieldByName("CombustionMode")->AsString = strCMode;
            DM->ADOTableMInfo->FieldByName("FileName")->AsString       = strlist;
            DM->ADOTableMInfo->Post();
        }
        catch(...)
        {
        }
    }
}
void __fastcall TFormMain::N21Click(TObject *Sender)
{
    AnsiString strSelected[6];
    AnsiString strTemp, strInfo;
    TTreeNode* mNode = TreeViewTrain->Selected;
    int i = 0;
    if(mNode == NULL)
    {
        return;
    }
    if(mNode->Text != "��ѹ�������")
    {
        if(mNode != NULL)
        {
            do
            {
                strSelected[i] = mNode->Text;
                mNode = mNode->Parent;
                i++;
            }while( mNode->Text != "��ѹ�������");
        }
    }
    if( i == 1)
    {
        if(bDateBrowseStyle)
        {
            strQMDate       = strSelected[0];
            strQTrainType   = "";
            strQTrainNumber = "";
        }
        else
        {
            strQTrainType   = strSelected[0];
            strQMDate       = "";
            strQTrainNumber = "";
        }
    }
    else if(i == 2)
    {
        if(bDateBrowseStyle)
        {
            strQTrainType = strSelected[0];
            strQMDate     = strSelected[1];
            strQTrainNumber = "";
        }
        else
        {
            strQTrainNumber = strSelected[0];
            strQTrainType   = strSelected[1];
            strQMDate       = "";
        }
    }
    else if(i == 3)
    {
        if(bDateBrowseStyle)
        {
            strQTrainNumber = strSelected[0];
            strQTrainType   = strSelected[1];
            strQMDate       = strSelected[2];
        }
        else
        {
            strQMDate       = strSelected[0];
            strQTrainNumber = strSelected[1];
            strQTrainType   = strSelected[2];
        }
    }

    strTemp = "delete from MeasureInfo";
    if( i == 0)  //ɾ��ȫ��������
    {
        strInfo = "ȷʵҪ����Ϣ���е�����ȫ��ɾ���𣿣�";
    }
    else if( i == 1)
    {
        if(bDateBrowseStyle)
        {

//            strTempTM = DM->ADOQueryDate->Fields->Fields[0]->Value;
//            ReformDate(strTempTM);
//        mNode     = TreeViewTrain->Items->AddChild(rNode,strTempTM);
//            FormDate(strTempTM);
            FormDate( strQMDate );
            strTemp += " where MDate = '" + strQMDate + "'";
            strInfo = "ȷʵҪ�� " + strQMDate + " ����������ȫ������Ϣ����ɾ����";
        }
        else
        {
            strTemp += " where TrainType = '" + strQTrainType + "'";
            strInfo = "ȷʵҪ�� " + strQTrainType + " ���͵����ݴ���Ϣ����ɾ����";
        }
    }
    else if(i == 2)
    {
        if(bDateBrowseStyle)
        {
            FormDate( strQMDate );
            strTemp += " where MDate = '" + strQMDate + "'";
            strTemp += " and TrainType = '" + strQTrainType + "'";
            strInfo = "ȷʵҪ�� " + strQMDate + "��һ�������" + strQMDate + " ���ݴ���Ϣ����ɾ����";
        }
        else
        {
            strTemp += " where TrainType = '" + strQTrainType + "'";
            strTemp += " and TrainNumber = '" + strQTrainNumber + "'";
            strInfo = "ȷʵҪ�� " + strQTrainType + strQTrainNumber + " �����ݴ���Ϣ����ɾ����";
        }
    }
    else if( i == 3)
    {
        FormDate( strQMDate );
        strTemp += " where TrainType = '" + strQTrainType + "'";
        strTemp += " and TrainNumber = '" + strQTrainNumber + "'";
        strTemp += " and MDate = '" + strQMDate + "'";
        strInfo = "ȷʵҪ�� " + strQMDate + "������" + strQTrainType + strQTrainNumber + " �����ݴ���Ϣ����ɾ����";
    }
    if( Application->MessageBox(strInfo.c_str(),"��ʾ", MB_ICONQUESTION|MB_YESNO)
        == IDYES)
    {
        DM->ADOQueryTemp->Close();
        DM->ADOQueryTemp->SQL->Clear();
        DM->ADOQueryTemp->SQL->Add(strTemp);
        DM->ADOQueryTemp->Prepared = true;
        DM->ADOQueryTemp->ExecSQL();
        DM->ADOQueryTemp->Prepared = false;
        DM->ADOQueryTemp->Close();
        mNode = TreeViewTrain->Selected;
        TreeViewTrain->Items->Delete(mNode);
        Application->MessageBox("����ɾ������!","��ʾ",MB_OK|MB_ICONINFORMATION);
    }

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TreeViewTrainDblClick(TObject *Sender)
{
    AnsiString strSelected[10];
    TTreeNode* mNode = TreeViewTrain->Selected;
    int i = 0;
    if(mNode == NULL)
    {
        return;
    }
    if(mNode->Text != "��ѹ�������")
    {
        if(mNode != NULL)
        {
            do
            {
                strSelected[i] = mNode->Text;
                mNode = mNode->Parent;
                i++;
            }while( mNode->Text != "��ѹ�������");
        }
    }

    if(i == 2)
    {
//        Application->MessageBox("ѡ���˳��͡�����!","��ʾ",MB_OK|MB_ICONINFORMATION);
        iQueryStyle     = 2;                //ѡ���˳��ͳ��Ž��в�ѯ
        if(bDateBrowseStyle)
        {
            strQTrainType      = strSelected[0];
            strQMDate          = strSelected[1];
            strQTrainNumber    = "";
            strQCombustionMode = "";
        }
        else
        {
            strQTrainNumber    = strSelected[0];
            strQTrainType      = strSelected[1];
            strQMDate          = "";
            strQCombustionMode = "";
        }
    }
    else if(i == 3)
    {
        iQueryStyle     = 3;               //ѡ���˳��ͳ��ż�����ʱ����в�ѯ
        if(bDateBrowseStyle)
        {
            strQTrainNumber    = strSelected[0];
            strQTrainType      = strSelected[1];
            strQMDate          = strSelected[2];
            strQCombustionMode = "";

        }
        else
        {
            strQMDate          = strSelected[0];
            strQTrainNumber    = strSelected[1];
            strQTrainType      = strSelected[2];
            strQCombustionMode = "";
        }
        FormDate(strQMDate);
    }
    else if(i == 4)
    {
        iQueryStyle     = 4;               //ѡ���˳��ͳ��ż�����ʱ����в�ѯ
        if(bDateBrowseStyle)
        {
            strQCombustionMode = strSelected[0];
            strQTrainNumber    = strSelected[1];
            strQTrainType      = strSelected[2];
            strQMDate          = strSelected[3];
        }
        else
        {
            strQCombustionMode = strSelected[0];
            strQMDate          = strSelected[1];
            strQTrainNumber    = strSelected[2];
            strQTrainType      = strSelected[3];
        }
        FormDate(strQMDate);
    }

    if( i >= 2)
    {
        bBrowseOperated = true;
        LoadBrowseList();
    }
    else
    {
        bBrowseOperated = false;
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::LoadBrowseList(void)
{
    //TODO: Add your source code here
    AnsiString strTemp;
    bool       bTNAdded = false;
    if(strQTrainType == "")
    {
        return;
    }

    strTemp = "select filename,MDate from MeasureInfo where TrainType = '";
    strTemp += strQTrainType + "' and ";

    if(strQTrainNumber != "")
    {
        strTemp += "TrainNumber = '" + strQTrainNumber + "'";
        bTNAdded = true;
    }
    if(strQMDate != "")
    {
        if(bTNAdded)
        {
            strTemp += " and ";
        }
        strTemp += " MDate = '" + strQMDate + "'";
    }

    if( strQCombustionMode != "")
    {
        strTemp += " and CombustionMode = '" + strQCombustionMode + "'";
    }

    strTemp += " Order by MDate";


//���¿�ʼ��ѯ��������������

    DM->ADOQueryTemp->Close();
    DM->ADOQueryTemp->SQL->Clear();
    DM->ADOQueryTemp->SQL->Add(strTemp);
    DM->ADOQueryTemp->Open();
    DM->ADOQueryTemp->First();
    iListDrawKind = 0;
    while(!DM->ADOQueryTemp->Eof)   //������ͻ�ת��
    {
        strSelectListName[iListDrawKind]  = DM->ADOQueryTemp->Fields->Fields[0]->Value;
        iListDrawKind ++;
        if(iListDrawKind > 20)
        {
            break;
        }
        DM->ADOQueryTemp->Next();
    }
    DM->ADOQueryTemp->Close();
    
    if(iListDrawKind > 1)
    {
        strValidPath = ExtractFilePath(strSelectListName[0]);
        if(FileExists(strSelectListName[0]))
        {
            ListBoxPST->Items->LoadFromFile(strSelectListName[0]);
            strPSTName  =  strSelectListName[0];
            iChoicedNum = ListBoxPST->Items->Count;
//        iListDrawKind = inum;   //����������εĲ������
            iListPage = 0;
            GetListPageNum();
            bLoadList  = true;
            bPSTOpened = true;
            if(iChoiceEnd == 0) //��ʾ���ݻ�û�г�ʼ��
            {
                iChoiceEnd = 3600;
            }
            DrawListDirect();//    iListDrawKind --;
        }
        else
        {
            if( Application->MessageBox("���ļ������ڣ������ѱ�����!!!\n\n��������Ҽ�Ȼ��ѡ�����Ϣ����ɾ��!!!","��ʾ",MB_OK|MB_ICONINFORMATION) == IDYES)
            {

            }

        }
    }
    else if(iListDrawKind == 1)
    {
        strPSTName       = strSelectListName[0];             //Ҫװ����б��ļ�����
        iListDrawKind    = 1;
        strFileDirectory = ::ExtractFilePath(strPSTName);
        if( FileExists(strPSTName))
        {
            ListBoxPCPFile->Items->LoadFromFile(strPSTName);
            ListBoxPST->Items->LoadFromFile(strPSTName);
            strValidPath     = strFileDirectory;
            iChoicedNum = ListBoxPST->Items->Count;
            bLoadList = true;
            LoadCurrentBinPST();
            iChoiceStart = 0;
            iChoiceEnd   = lDS.iPLength;
        }
        else
        {
            if( Application->MessageBox("���ļ������ڣ������ѱ�����!!!\n\n�Ƿ��������Ϣ?","��ʾ",MB_YESNO|MB_ICONINFORMATION) == IDYES)
            {

            }
        }
    }
}
void __fastcall TFormMain::PageControlBrowseChange(TObject *Sender)
{
    Application->ProcessMessages();
    if(PageControlBrowse->ActivePageIndex == 2)
    {
        TCursor cur = Screen->Cursor;
        Screen->Cursor = crHourGlass;
        RefreshTViewByDate();
        Screen->Cursor = cur;
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ppmAddToMInfoDatabaseClick(TObject *Sender)
{
    AnsiString strTemp;
    strTemp = ExtractFileExt( FileListBoxFile->FileName );
    strTemp = strTemp.UpperCase();
    if(strTemp == ".PST")
    {
        strTemp = FileListBoxFile->FileName;
        AddToMeasureInfo( strTemp );
        strTemp += "\n\n";
        strTemp += "�б���������������Ϣ���ݿ�!";
        Application->MessageBox(strTemp.c_str(),"��ʾ",MB_OK|MB_ICONINFORMATION);
    }
    else
    {
        Application->MessageBox("�޷�����!!!\n\n��ѡ��PST��׺���б��ļ�!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ReformDate(AnsiString& strdate)
{
    //TODO: Add your source code here
    unsigned short usYear, usMonth, usDate;
    AnsiString strTemp;
    strTemp = strdate.SubString( 1, 4);
    usYear  = StrToInt(strTemp);
    strTemp = strdate.SubString( 6, 2);
    usMonth = StrToInt(strTemp);
    strTemp = strdate.SubString( 9, 2);
    usDate  = StrToInt(strTemp);
    strdate.printf("%04d��%02d��%02d��", usYear, usMonth, usDate);
}

void __fastcall TFormMain::FormDate(AnsiString& strdate)
{
    //TODO: Add your source code here
    unsigned short usYear, usMonth, usDate;
    AnsiString strTemp;
    strTemp = strdate.SubString( 1, 4);
    usYear  = StrToInt(strTemp);
    strTemp = strdate.SubString( 7, 2);
    usMonth = StrToInt(strTemp);
    strTemp = strdate.SubString( 11, 2);
    usDate  = StrToInt(strTemp);
    strdate.printf("%04d-%02d-%02d", usYear, usMonth, usDate);
}


int __fastcall TFormMain::GetiDieselNum(AnsiString& str)
{
    //TODO: Add your source code here
    int iNum;
    int iIndex;
    AnsiString strOp;
    AnsiString strTemp = " ";
    strOp = str;
    iIndex = strOp.Pos(strTemp);
    strOp  = strOp.SubString(1,iIndex);
    iIndex  = strOp.Length();
    strOp = strOp.SubString(iIndex - 2, 2);
    iNum  = StrToInt(strOp);
    return iNum;
}

void __fastcall TFormMain::ppmTrainSelectionClick(TObject *Sender)
{
    FormTrainSelection = new TFormTrainSelection(this);
    if( NULL != FormTrainSelection )
    {
        FormTrainSelection->SetiTrainNums( iMTrainInfoNums );   //���û�����
        FormTrainSelection->ShowModal();
        delete FormTrainSelection;
    }
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::PleaseReadIt(int iIndex, int& iposi)
{
    //TODO: Add your source code here
    bool bret = false;
    iposi = -1;
    for( int i = 0; i < iMTrainInfoNums; i ++ )
    {
        if(   ( iIndex >= myMTrainInfo[i].iStartPosi )
            &&( iIndex <= myMTrainInfo[i].iEndPosi )
          )
        {
            iposi = i;
            if( myMTrainInfo[i].bSelected )
            {
                bret = true;
            }
        }
    }
    return bret;
}

bool __fastcall TFormMain::ListFileValid(AnsiString& strTemp)
{
    //TODO: Add your source code here
    bool bret = false;
    AnsiString strInfo, strTemp0;
    int iposi;
    strTemp0 = ExtractFileName( strTemp );
    iposi = strTemp0.Pos("_");
    strInfo = strTemp0.SubString(1, iposi);
    iposi = strTemp0.Pos("rpm");
    strInfo += strTemp0.SubString(iposi + 4, 10);
    for( int i = 0; i < iMTrainInfoNums; i ++ )
    {
        if( strInfo == myMTrainInfo[i].strTrainInfo )
        {
            if(myMTrainInfo[i].bSelected)
            {
                bret = true;
            }
        }
    }
    return bret;
}

void __fastcall TFormMain::DataAnalysis(void)
{
/*
//TODO: �����ݽ�����ʾ����������������
    float fTTemp[20];
    int iLeft;
    int iTop;
    int iWidth;
    int iHeight;
    int iColNums = 15;
    int iDias    = 5;
    int iystep;
    int ixstep;
    AnsiString strTemp, strTemp0;
    if( lDS.bOilSupply != false)
    {
        strTemp = "*** ȼ�չ��̸������б� ***";
    }
    else
    {
        strTemp = "*** ѹ�����̸������б� ***";
    }
    MemoDetect->Lines->Clear();

    MemoDetect->Lines->Add(strTemp);

    strTemp0.printf("ƽ��ת��:%.1fr/min; ָʾ����:%.1fkW", maxBuffer.fESMean, maxBuffer.fCCPower);

    strTemp  = "����:" + strTrainType    + "; " ;
    strTemp += "����:" + strTrainNumber  + "; " ;
    strTemp += "����ʱ��:" + strMDate        + "; ";
    strTemp += strTemp0;

    MemoDetect->Lines->Add(strTemp);
// Ӧ�ü��ϻ������͡��������źͲ���ʱ��

    for(int i =0; i < maxBuffer.iCyNums; i ++)
    {
//0 ��ʾ�׺�
        strTemp.printf("%02d", maxBuffer.siarDieselNum[i]);
        StringGridResult->Cells[0][i + 3] = strTemp;
//1 ��ʾ������ѹ�����ֵ
        strTemp.printf("%.2f", maxBuffer.farPMax[i]);
        StringGridResult->Cells[1][i + 3] = strTemp;
//2 ��ʾ����ѹ����λ
        strTemp.printf("%.1f", maxBuffer.farMaxPAngle[i]);
        StringGridResult->Cells[2][i + 3] = strTemp;
//3 ��ʾ����ƽ��ָʾѹ��
        strTemp.printf("%.3f", maxBuffer.farAverageP[i]);
        StringGridResult->Cells[3][i + 3] = strTemp;
//4 ��ʾ����ѹ�������ֵ
        strTemp.printf("%.3f", maxBuffer.farDPDFMax[i]);
        StringGridResult->Cells[4][i + 3] = strTemp;
//5 ��ʾ����ѹ�������ֵ��λ
        strTemp.printf("%.1f", maxBuffer.farMaxDPAngle[i]);
        StringGridResult->Cells[5][i + 3] = strTemp;
//6 ��ʾ���׹�����ǰ��
        strTemp.printf("%.1f",  maxBuffer.farOilAngle[i]);
        StringGridResult->Cells[6][i + 3] = strTemp;
//7 ��ʾ���׹��ͳ���ʱ��
        strTemp.printf("%.1f", maxBuffer.farWavePeriod[i]);
        StringGridResult->Cells[7][i + 3] = strTemp;
//8 ��ʾ����ȼ�ս�
        strTemp.printf("%.1f", maxBuffer.farCombustionAngle[i]);
        StringGridResult->Cells[8][i + 3] = strTemp;
//9 ��ʾ����ȼ�ճ�����
        strTemp.printf("%.1f", maxBuffer.farCombustionPeriod[i]);
        StringGridResult->Cells[9][i + 3] = strTemp;
//10 ��ʾ���׷��������ֵ
        strTemp.printf("%.3f", maxBuffer.farDHDFMax[i]/1000.0);
        StringGridResult->Cells[10][i + 3] = strTemp;
//11 ��ʾ���׷��������ֵ��λ
        strTemp.printf("%.1f", maxBuffer.farMaxDHAngle[i]);
        StringGridResult->Cells[11][i + 3] = strTemp;
//12 ��ʾ�����ۻ�������
        strTemp.printf("%.2f", maxBuffer.farCDHDFMax[i]);
        StringGridResult->Cells[12][i + 3] = strTemp;
//13 ��ʾ�����ۻ�������
        strTemp.printf("%.2f", maxBuffer.farCPower[i]);
        StringGridResult->Cells[13][i + 3] = strTemp;
//14��ʾ����ת��
        strTemp.printf("%.1f", maxBuffer.farEngineSpeed[i] );
        StringGridResult->Cells[14][i + 3] = strTemp;
    }

    MemoDetect->Lines->Add("");
    strTemp = "�����������";
    MemoDetect->Lines->Add(strTemp);

    lCombustionDetect myDetect;
//
//------------------------------------------------------------------
//  11111111111111111111111111     ������ǰ�Ƿ������
    if( lDS.bOilSupply )
    {
        myDetect.AnalysisAll(maxBuffer);
        strTemp.printf(" (1)���׹�����ǰ�Ƿ���:   ���׾�ֵΪ��%5.1fCA��", myDetect.OilAngleInfo.fAverage);
        MemoDetect->Lines->Add(strTemp);

        if(myDetect.OilAngleInfo.iErrorNum > 0)  //����г�����Χ�ģ�����ʾ
        {
            for( int i = 0; i < maxBuffer.iCyNums; i++)
            {
                if( myDetect.OilAngleInfo.bError[i])
                {
                    if(myDetect.OilAngleInfo.fChar[i] > 0.0)
                    {
                        strTemp.printf("    �׺ţ�%02d��������ǰ�ǣ�%5.1fCA��", //Ȩ�أ�%5.2f��",
                                            maxBuffer.siarDieselNum[i],
                                            maxBuffer.farOilAngle[i] //,

                                            //myDetect.OilAngleInfo.fChar[i]
                                            );
                    }
                    else
                    {
                        strTemp.printf("    �׺ţ�%02d��������ǰ�ǣ�%5.1fCA��", //Ȩ�أ�%5.2f��",
                                            maxBuffer.siarDieselNum[i],
                                            maxBuffer.farOilAngle[i] //,
                                          //  -myDetect.OilAngleInfo.fWeight[i],
                                          //  myDetect.OilAngleInfo.fChar[i]
                                            );

                    }
                    MemoDetect->Lines->Add(strTemp);;
                }
            }
        }
        else
        {
            strTemp = "      ���׹��ͽǶȲ�𲻴�!" ;
            MemoDetect->Lines->Add(strTemp);
        }
//------------------------------------------------------------------
//  22222222222222222     ����ѹ������
        myDetect.AnalysisCombustion(maxBuffer);

        MemoDetect->Lines->Add("");
        strTemp.printf(" (2)�������ѹ������:     ���׾�ֵΪ��%5.2fMPa��", myDetect.CombustionInfo.fAverage);
        MemoDetect->Lines->Add(strTemp);

        if(myDetect.CombustionInfo.iErrorNum > 0)  //����г�����Χ�ģ�����ʾ
        {
            for( int i = 0; i < maxBuffer.iCyNums; i++)
            {
                if( myDetect.CombustionInfo.bError[i])
                {
                    if(myDetect.CombustionInfo.fChar[i] > 999.0)
                    {
                        strTemp.printf("    �׺ţ�%02d������ѹ����%5.2fMPa����λ��%5.1fCA��������ǰ�ǣ�%5.1fCA��", //Ȩ�أ�%5.2f�����Ż�",
                                            maxBuffer.siarDieselNum[i],
                                            maxBuffer.farPMax[i],
                                            maxBuffer.farMaxPAngle[i],
                                            maxBuffer.farOilAngle[i] //,
                                          //  mb.farCombustionLevel[i]
                                            );
                    }
                    else if( myDetect.CombustionInfo.fChar[i] > 0.0 )
                    {
                        strTemp.printf("    �׺ţ�%02d������ѹ����%5.2fMPa����λ��%5.1fCA��������ǰ�ǣ�%5.1fCA��", //Ȩ�أ�%5.2f��",
                                            maxBuffer.siarDieselNum[i],
                                            maxBuffer.farPMax[i],
                                            maxBuffer.farMaxPAngle[i],
                                            maxBuffer.farOilAngle[i] //,
                                            //myDetect.CombustionInfo.fChar[i]
                                            );
                    }
                    else
                    {
                        strTemp.printf("    �׺ţ�%02d������ѹ����%5.2fMPa����λ��%5.1fCA��������ǰ�ǣ�%5.1fCA��", //Ȩ�أ�%5.2f��",
                                            maxBuffer.siarDieselNum[i],
                                            maxBuffer.farPMax[i],
                                            maxBuffer.farMaxPAngle[i],
                                            maxBuffer.farOilAngle[i]
                                        //    ,
                                        //    myDetect.CombustionInfo.fChar[i]
                                            );

                    }
                    MemoDetect->Lines->Add(strTemp);

                }
            }
        }
        else
        {
            strTemp = "      �������ѹ����𲻴�!" ;
            MemoDetect->Lines->Add(strTemp);
        }
//========================================================================================

//  3333333333333333333333     ƽ��ָʾѹ������
        myDetect.AnalysisAveragePressure(maxBuffer);
        MemoDetect->Lines->Add("");
        strTemp.printf(" (3)����ƽ��ָʾѹ������:  ���׾�ֵΪ��%5.2fMPa��", myDetect.AveragePressureInfo.fAverage);
        MemoDetect->Lines->Add(strTemp);

        if(myDetect.AveragePressureInfo.iErrorNum > 0)  //����г�����Χ�ģ�����ʾ
        {
            for( int i = 0; i < maxBuffer.iCyNums; i++)
            {
                if( myDetect.AveragePressureInfo.bError[i])
                {
                    if(myDetect.AveragePressureInfo.fChar[i] > 0.0)
                    {
                    }
                    else
                    {
                        strTemp.printf("    �׺ţ�%02d��ƽ��ָʾѹ����%5.2fMPa��ѭ������%5.2fkW�����������٣�", //Ȩ�أ�%5.2f��",
                                            maxBuffer.siarDieselNum[i],
                                            maxBuffer.farAverageP[i],
                                            maxBuffer.farCPower[i]
                                            //,
                                            //myDetect.AveragePressureInfo.fChar[i]
                                            );
                        MemoDetect->Lines->Add(strTemp);
                    }


                }
            }
        }
        else
        {
            strTemp = "      ����ƽ��ָʾѹ����𲻴�!" ;
            MemoDetect->Lines->Add(strTemp);;
        }

//========================================================================================
    }
    else     //��ѹ�����̽��з���
    {
        myDetect.AnalysisCompress(maxBuffer);
        strTemp.printf(" (1)����ѹ��ѹ��������   ���׾�ֵΪ��%5.2f MPa��", myDetect.CompressInfo.fAverage);
        MemoDetect->Lines->Add(strTemp);

        if(myDetect.CompressInfo.iErrorNum > 0)  //����г�����Χ�ģ�����ʾ
        {
            for( int i = 0; i < maxBuffer.iCyNums; i++)
            {
                if( myDetect.CompressInfo.bError[i])
                {
                    if(myDetect.CompressInfo.fChar[i] > 0.0)
                    {
                        strTemp.printf("    �׺ţ�%02d��ѹ��ѹ����%5.2f MPa��", //Ȩ�أ�%5.2f��",
                                            maxBuffer.siarDieselNum[i],
                                            maxBuffer.farPMax[i]
                                            //,
                                            //myDetect.CompressInfo.fChar[i]
                                       );
                    }
                    else
                    {

                        strTemp.printf("    �׺ţ�%02d��ѹ��ѹ����%5.2f MPa��", //Ȩ�أ�%5.2f��",
                                            maxBuffer.siarDieselNum[i],
                                            maxBuffer.farPMax[i]
                                            //,
                                           // myDetect.CompressInfo.fChar[i]
                                       );
                    }
                    MemoDetect->Lines->Add(strTemp);
                }
            }
        }
        else
        {
            strTemp = "      ����ѹ��ѹ����������!" ;
            MemoDetect->Lines->Add(strTemp);
        }
    }
*/
}



void __fastcall TFormMain::SaveToExcels(void)
{
   //TODO: Add your source code here
    AnsiString strTT, strTTemp;
    AnsiString strHTTT, strTemp;
    int iposi, ilen;
    int irow, icol;
    bool bHaveOne = false;
//�ڱ������ڵ�·������һ�����������ݵ�Excel����
    AnsiString strSource, strTarget;
    strSource = strExeFilePath + "EXCELS\\Model_Result.xls";
    strHTTT = strPSTName.UpperCase();
    iposi  = strHTTT.Pos(".P");
    strHTTT = strHTTT.SubString(1,iposi);
    strTarget =  strHTTT + "xls";
    if(FileExists(strSource))
    {
    //�������ͬ�����ļ��򸲸�
        if(!FileExists(strTarget))
        {
             ::CopyFile(strSource.c_str(), strTarget.c_str(),false);
             bHaveOne = false;
        }
        else
        {
            if( Application->MessageBox("EXCEL���ݷ��������Ѵ���!!!\n\nҪ���������𣿣�","��ʾ",MB_YESNO|MB_ICONINFORMATION) == IDYES)
            {
                ::CopyFile(strSource.c_str(), strTarget.c_str(),false);
                bHaveOne = false;
            }
            else
            {
                bHaveOne = true;
            }
        }
    }
    else
    {
        Application->MessageBox("ģ�����ݲ�����!!!","��ʾ",MB_OK|MB_ICONINFORMATION);
        return;
    }
    if(bHaveOne)
    {
            if( Application->MessageBox("Ҫ���Ѵ��ڵ�Excel����","��ʾ",MB_YESNO|MB_ICONINFORMATION) == IDNO)
            {
                return;
            }
    }

    try
    {
        vExcelApp = Variant::CreateObject("Excel.Application");
    }
    catch(...)
    {
        Application->MessageBox("�޷�����Excel","����",MB_ICONSTOP|MB_OK);
        return;
    }

// ����ExcelΪ�ɼ�
    vExcelApp.OlePropertySet("Visible",true);
// ��ָ����Excel�����ļ��������ļ�������趨ֻ��һ��Sheet��
//    vExcelApp.OlePropertyGet("WorkBooks").OleProcedure("New",strExcelFileName.c_str());

// ����һ���µĵ���

//    vWorkBook.OleProcedure("Save");//�������
    vExcelApp.OlePropertyGet("WorkBooks").OleFunction("Open",strTarget.c_str());
//��������޷���Excel2000�д����±�
//    vExcelApp.OlePropertyGet("WorkBooks").OleFunction("New");
    vWorkBook = vExcelApp.OlePropertyGet("ActiveWorkBook");
    vSheet    = vWorkBook.OlePropertyGet("ActiveSheet");//��õ�ǰĬ�ϵ�Sheet
    if(bHaveOne)
    {
        return;
    }
//    vSheet.OlePropertyGet("Cells",irow,icol).OlePropertySet("Value",strTemp.c_str());
    strTemp = strUserTitle + "      �������ͻ�ȼ��ϵͳ�����";
    vSheet.OlePropertyGet("Cells",1, 1).OlePropertySet("Value",strTemp.c_str());

    if( lDS.bOilSupply != false)
    {
        strTemp = "*** ȼ�չ��̸������б� ***";
    }
    else
    {
        strTemp = "*** ѹ�����̸������б� ***";
    }
    vSheet.OlePropertyGet("Cells",2, 1).OlePropertySet("Value",strTemp.c_str());


    strTTemp.printf("ƽ��ת��:%.1fr/min; ָʾ����:%.1fkW", maxBuffer.fESMean, maxBuffer.fCCPower);

    strTemp  = "����:" + strTrainType    + "; " ;
    strTemp += "����:" + strTrainNumber  + "; " ;
    strTemp += "����ʱ��:" + strMTime    + "; ";
    strTemp += strTTemp;

    vSheet.OlePropertyGet("Cells", 3, 1).OlePropertySet("Value",strTemp.c_str());

// Ӧ�ü��ϻ������͡��������źͲ���ʱ��

//----------------------------------------------------------------------
//�ӵ�5�п�ʼ��������
    for(int i =0; i < maxBuffer.iCyNums; i ++)
    {
//0 ��ʾ�׺�
        irow = i + 7;
        strTemp.printf("%02d", maxBuffer.siarDieselNum[i]);
        vSheet.OlePropertyGet("Cells",irow, 1).OlePropertySet("Value",strTemp.c_str());
//1 ��ʾ������ѹ�����ֵ
        strTemp.printf("%.2f", maxBuffer.farPMax[i]);
        vSheet.OlePropertyGet("Cells",irow, 2).OlePropertySet("Value",strTemp.c_str());
//2 ��ʾ����ѹ����λ
        strTemp.printf("%.1f", maxBuffer.farMaxPAngle[i]);
        vSheet.OlePropertyGet("Cells",irow, 3).OlePropertySet("Value",strTemp.c_str());
//3 ��ʾ����ƽ��ָʾѹ��
        strTemp.printf("%.3f", maxBuffer.farAverageP[i]);
        vSheet.OlePropertyGet("Cells",irow, 4).OlePropertySet("Value",strTemp.c_str());
//4 ��ʾ����ѹ�������ֵ
        strTemp.printf("%.3f", maxBuffer.farDPDFMax[i]);
        vSheet.OlePropertyGet("Cells",irow, 5).OlePropertySet("Value",strTemp.c_str());
//5 ��ʾ����ѹ�������ֵ��λ
        strTemp.printf("%.1f", maxBuffer.farMaxDPAngle[i]);
        vSheet.OlePropertyGet("Cells",irow, 6).OlePropertySet("Value",strTemp.c_str());
//6 ��ʾ���׹�����ǰ��
        strTemp.printf("%.1f",  maxBuffer.farOilAngle[i]);
        vSheet.OlePropertyGet("Cells",irow, 7).OlePropertySet("Value",strTemp.c_str());
//7 ��ʾ���׹��ͳ���ʱ��
        strTemp.printf("%.1f", maxBuffer.farWavePeriod[i]);
        vSheet.OlePropertyGet("Cells",irow, 8).OlePropertySet("Value",strTemp.c_str());
//8 ��ʾ����ȼ�ս�
        strTemp.printf("%.1f", maxBuffer.farCombustionAngle[i]);
        vSheet.OlePropertyGet("Cells",irow, 9).OlePropertySet("Value",strTemp.c_str());
//9 ��ʾ����ȼ�ճ�����
        strTemp.printf("%.1f", maxBuffer.farCombustionPeriod[i]);
        vSheet.OlePropertyGet("Cells",irow, 10).OlePropertySet("Value",strTemp.c_str());
//10 ��ʾ���׷��������ֵ
        strTemp.printf("%.3f", maxBuffer.farDHDFMax[i]/1000.0);
        vSheet.OlePropertyGet("Cells",irow, 11).OlePropertySet("Value",strTemp.c_str());
//11 ��ʾ���׷��������ֵ��λ
        strTemp.printf("%.1f", maxBuffer.farMaxDHAngle[i]);
        vSheet.OlePropertyGet("Cells",irow, 12).OlePropertySet("Value",strTemp.c_str());
//12 ��ʾ�����ۻ�������
        strTemp.printf("%.2f", maxBuffer.farCDHDFMax[i]);
        vSheet.OlePropertyGet("Cells",irow, 13).OlePropertySet("Value",strTemp.c_str());
//13 ��ʾ�����ۻ�������
        strTemp.printf("%.2f", maxBuffer.farCPower[i]);
        vSheet.OlePropertyGet("Cells",irow, 14).OlePropertySet("Value",strTemp.c_str());
//14��ʾ����ת��
        strTemp.printf("%.1f", maxBuffer.farEngineSpeed[i] );
        vSheet.OlePropertyGet("Cells",irow, 15).OlePropertySet("Value",strTemp.c_str());
    }
//==================================================================

    irow = 25;

    strTemp = "***�����������***";
    vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value",strTemp.c_str());
    int iStartP = 2;

    lCombustionDetect myDetect;
//
//------------------------------------------------------------------
//  11111111111111111111111111     ������ǰ�Ƿ������
    if( lDS.bOilSupply )
    {
        if(bAnalysisMethod)   //���ö��Է�������
        {
            myDetect.AnalysisAll(maxBuffer);
            strTemp.printf("(1)���׹�����ǰ�Ƿ���:   ���׾�ֵΪ��%5.1fCA��", myDetect.OilAngleInfo.fAverage);
            vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value",strTemp.c_str());
            if(myDetect.OilAngleInfo.iErrorNum > 0)  //����г�����Χ�ģ�����ʾ
            {
                for( int i = 0; i < maxBuffer.iCyNums; i++)
                {
                    if( myDetect.OilAngleInfo.bError[i])
                    {
                        if(myDetect.OilAngleInfo.fChar[i] > 0.0)
                        {
                            strTemp.printf("  �׺ţ�%02d��������ǰ�ǣ�%5.1fCA��������ǰ��ƫС", //Ȩ�أ�%5.2f��",
                                                maxBuffer.siarDieselNum[i],
                                            maxBuffer.farOilAngle[i] //,

                                            //myDetect.OilAngleInfo.fChar[i]
                                            );
                        }
                        else
                        {
                            strTemp.printf("  �׺ţ�%02d��������ǰ�ǣ�%5.1fCA��������ǰ��ƫ��", //Ȩ�أ�%5.2f��",
                                                maxBuffer.siarDieselNum[i],
                                            maxBuffer.farOilAngle[i] //,
                                          //  -myDetect.OilAngleInfo.fWeight[i],
                                          //  myDetect.OilAngleInfo.fChar[i]
                                            );

                        }
                        vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value",strTemp.c_str());
                    }
                }
            }
            else
            {
                strTemp = "  ���׹��ͽǶȲ�𲻴�!" ;
                vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value",strTemp.c_str());
            }
        }
        else  //��������
        {
            int itrainType;
            itrainType = lDS.LFHS.siTrainType;
            myDetect.SetAngleInfo( myDieselInfo[itrainType].fOilAngleMin,
                                  myDieselInfo[itrainType].fOilAngleMax);
            myDetect.AnalysisMOilAngle(maxBuffer);
            irow ++; //�ճ�һ��
            strTemp.printf(" **��������**  ���׹�����ǰ�ǣ�������Χ��(%.1f �� %.1f CA)", myDieselInfo[itrainType].fOilAngleMax,
                                                                            myDieselInfo[itrainType].fOilAngleMin
                                                                            );
            vSheet.OlePropertyGet("Cells",irow++, iStartP).OlePropertySet("Value",strTemp.c_str());
            if( myDetect.MAngleInfo.iNFNums > 0)
            {
                strTemp.printf("  ���� %d ���׵Ĺ�����ǰ�ǲ��ڷ�Χ�� ", myDetect.MAngleInfo.iNFNums  );
                vSheet.OlePropertyGet("Cells",irow++, iStartP).OlePropertySet("Value",strTemp.c_str());

                if( myDetect.MAngleInfo.iNFLeftNums > 0)
                {
                    strTemp.printf("��������ǰ�ǽϴ�������� %d ���ף�", myDetect.MAngleInfo.iNFLeftNums);
                    vSheet.OlePropertyGet("Cells",irow++, iStartP).OlePropertySet("Value", strTemp.c_str());
                    for( int i = 0; i < myDetect.MAngleInfo.iNFLeftNums; i ++)
                    {
                        strTT.printf("�׺ţ�%02d;  ����ѹ����%.2f; ������ǰ�ǣ�%.2f" ,
                                                            myDetect.MAngleInfo.iNFLIndex[i],
                                                        myDetect.MAngleInfo.fNFLPressure[i],
                                                        myDetect.MAngleInfo.fNFLAngle[i]);
                        vSheet.OlePropertyGet("Cells",irow++, iStartP + 1).OlePropertySet("Value",strTT.c_str());
                    }
                }
                if( myDetect.MAngleInfo.iNFRightNums > 0)
                {
                    strTemp.printf("��������ǰ�ǽ�С�������� %d ���ף�", myDetect.MAngleInfo.iNFRightNums);
                    vSheet.OlePropertyGet("Cells",irow++, iStartP).OlePropertySet("Value", strTemp.c_str());
                    for( int i = 0; i < myDetect.MAngleInfo.iNFRightNums; i ++)
                    {
                        strTT.printf("�׺ţ�%02d;  ����ѹ����%.2f;  ������ǰ�ǣ�%.2f;",
                                                        myDetect.MAngleInfo.iNFRIndex[i],
                                                        myDetect.MAngleInfo.fNFRPressure[i],
                                                        myDetect.MAngleInfo.fNFRAngle[i]);
                    vSheet.OlePropertyGet("Cells",irow++, iStartP + 1).OlePropertySet("Value",strTT.c_str());
                    }
                }
            }
            else
            {
                strTemp = "      ���׹�����ǰ�Ǿ��ڹ涨��Χ��!" ;
                vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value",strTemp.c_str());
            }
        }
//------------------------------------------------------------------
//  22222222222222222     ����ѹ������
        myDetect.AnalysisCombustion(maxBuffer);

        vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value","");
        strTemp.printf(" (2)�������ѹ������:     ���׾�ֵΪ��%5.2fMPa��", myDetect.CombustionInfo.fAverage);
        vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value",strTemp.c_str());

        if(myDetect.CombustionInfo.iErrorNum > 0)  //����г�����Χ�ģ�����ʾ
        {
            for( int i = 0; i < maxBuffer.iCyNums; i++)
            {
                if( myDetect.CombustionInfo.bError[i])
                {
                    if(myDetect.CombustionInfo.fChar[i] > 999.0)
                    {
                        strTemp.printf("  �׺ţ�%02d������ѹ����%5.2fMPa����λ��%5.1fCA��������ǰ�ǣ�%5.1fCA����ѹƫС", //Ȩ�أ�%5.2f�����Ż�",
                                            maxBuffer.siarDieselNum[i],
                                            maxBuffer.farPMax[i],
                                            maxBuffer.farMaxPAngle[i],
                                            maxBuffer.farOilAngle[i] //,
                                          //  mb.farCombustionLevel[i]
                                            );
                    }
                    else if( myDetect.CombustionInfo.fChar[i] > 0.0 )
                    {
                        strTemp.printf("  �׺ţ�%02d������ѹ����%5.2fMPa����λ��%5.1fCA��������ǰ�ǣ�%5.1fCA����ѹƫ��", //Ȩ�أ�%5.2f��",
                                            maxBuffer.siarDieselNum[i],
                                            maxBuffer.farPMax[i],
                                            maxBuffer.farMaxPAngle[i],
                                            maxBuffer.farOilAngle[i] //,
                                            //myDetect.CombustionInfo.fChar[i]
                                            );
                    }
                    else
                    {
                        strTemp.printf("  �׺ţ�%02d������ѹ����%5.2fMPa����λ��%5.1fCA��������ǰ�ǣ�%5.1fCA����ѹƫС", //Ȩ�أ�%5.2f��",
                                            maxBuffer.siarDieselNum[i],
                                            maxBuffer.farPMax[i],
                                            maxBuffer.farMaxPAngle[i],
                                            maxBuffer.farOilAngle[i]
                                        //    ,
                                        //    myDetect.CombustionInfo.fChar[i]
                                            );

                    }
                    vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value",strTemp.c_str());

                }
            }
        }
        else
        {
            strTemp = "      �������ѹ����𲻴�!" ;
            vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value",strTemp.c_str());
        }
/*


//========================================================================================
*/
//  3333333333333333333333     ������������
        myDetect.AnalysisPower(maxBuffer);
        vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value", "");
        strTemp.printf(" (3)���׶�����������:  ����������ֵΪ��%5.2fMPa��", myDetect.PowerInfo.fAverage);
        vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value",strTemp.c_str());

        if(myDetect.PowerInfo.iErrorNum > 0)  //����г�����Χ�ģ�����ʾ
        {
            for( int i = 0; i < maxBuffer.iCyNums; i++)
            {
                if( myDetect.PowerInfo.bError[i])
                {
                    if(myDetect.PowerInfo.bHigh[i] > 0.0)
                    {
                        strTemp.printf("  �׺ţ�%02d������������%5.2fkW��������", //Ȩ�أ�%5.2f��",
                                            maxBuffer.siarDieselNum[i],
                                       //     maxBuffer.farAverageP[i],
                                            maxBuffer.farCPower[i]
                                            //,
                                            //myDetect.AveragePressureInfo.fChar[i]
                                            );
                        vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value",strTemp.c_str());

                    }
                    else
                    {
                        strTemp.printf("  �׺ţ�%02d������������%5.2fkW�������ٻ�Ϊ������", //Ȩ�أ�%5.2f��",
                                            maxBuffer.siarDieselNum[i],
                                       //     maxBuffer.farAverageP[i],
                                            maxBuffer.farCPower[i]
                                            //,
                                            //myDetect.AveragePressureInfo.fChar[i]
                                            );
                        vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value",strTemp.c_str());
                    }
                }
            }
        }
        else
        {
            strTemp = "      ���׶���������𲻴�!" ;
            vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value",strTemp.c_str());
        }

//        myDetect.SetfNoFireLimit(myDieselInfo[lDS.LFHS.siTrainType].fCompressMax);
        myDetect.AnalysisMean( maxBuffer );   //��ʧ��׽����ж�
        if( myDetect.MeanInfo.iNFNums > 0)    //��ʾ��ʧ������
        {
            strTemp.printf(" **�����Է���**  ���� %d ���״�����ȼ��״̬����������",myDetect.MeanInfo.iNFNums );
            irow ++;   //�ճ�һ��
            vSheet.OlePropertyGet("Cells",irow++, iStartP).OlePropertySet("Value",strTemp.c_str());
            if( myDetect.MeanInfo.iNFLeftNums > 0 )
            {
                strTemp.printf("��1 �� 8��һ�������� %d ���ף�", myDetect.MeanInfo.iNFLeftNums);
                vSheet.OlePropertyGet("Cells",irow++, iStartP).OlePropertySet("Value", strTemp.c_str());

                for( int i = 0; i < myDetect.MeanInfo.iNFLeftNums; i ++)
                {
                    strTT.printf("�׺ţ�%02d; ������ǰ�ǣ�%.1f CA;����������%.2f kW;",
                                                        myDetect.MeanInfo.iNFLIndex[i],
                                           //             myDetect.MeanInfo.fNFLPressure[i],
                                                        myDetect.MeanInfo.fNFLAngle[i],
                                                        myDetect.MeanInfo.fNFLPower[i]);
                    vSheet.OlePropertyGet("Cells",irow++, iStartP + 1).OlePropertySet("Value",strTT.c_str());
                }
            }

            if( myDetect.MeanInfo.iNFRightNums > 0)
            {
                strTemp.printf("��9 �� 16��һ�������� %d ���ף�", myDetect.MeanInfo.iNFRightNums);
                vSheet.OlePropertyGet("Cells",irow++, iStartP).OlePropertySet("Value", strTemp.c_str());

                for( int i = 0; i < myDetect.MeanInfo.iNFRightNums; i ++)
                {
                    strTT.printf("�׺ţ�%02d; ������ǰ�ǣ�%.1f CA;������������%.2f kW;",
                                                        myDetect.MeanInfo.iNFRIndex[i],
                                           //             myDetect.MeanInfo.fNFRPressure[i],
                                                        myDetect.MeanInfo.fNFRAngle[i],
                                                        myDetect.MeanInfo.fNFRPower[i]);
                    vSheet.OlePropertyGet("Cells",irow++, iStartP + 1).OlePropertySet("Value",strTT.c_str());
                }
            }
//--------------------------------------------------------------------------------------------
// �жϱ�׼1��2���������񶯽ϴ�ʱ���жϷ���
            if(   ((myDetect.MeanInfo.iNFLeftNums >= 3)&&(abs(myDetect.MeanInfo.iNFRightNums - myDetect.MeanInfo.iNFLeftNums) >= 3))
                ||((myDetect.MeanInfo.iNFRightNums >= 3)&&(abs(myDetect.MeanInfo.iNFRightNums - myDetect.MeanInfo.iNFLeftNums) >= 3))
            )
            {
                strTemp = "���ۣ��û����ܴ��ڽϴ��񶯣������С�������;�����!";
                vSheet.OlePropertyGet("Cells",irow++, iStartP).OlePropertySet("Value",strTemp.c_str());
            }
            else if( myDetect.MeanInfo.iNFNums >= 4)
            {
                strTemp.printf("���ۣ��û��ڵ������� %d ���ײ�����!",  myDetect.MeanInfo.iNFNums);
                vSheet.OlePropertyGet("Cells",irow++, iStartP).OlePropertySet("Value",strTemp.c_str());
            }
//=============================================================================================
        }
        else
        {
            strTemp = "����������𲻴�";
            vSheet.OlePropertyGet("Cells",irow++, iStartP).OlePropertySet("Value",strTemp.c_str());
        }
//========================================================================================
    }
    else     //��ѹ�����̽��з���
    {
        if( bAnalysisMethod ) //���Է���
        {
            myDetect.AnalysisCompress(maxBuffer);
            strTemp.printf(" (1)����ѹ��ѹ����������   ���׾�ֵΪ��%5.2f MPa��", myDetect.CompressInfo.fAverage);
            vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value",strTemp.c_str());

            if(myDetect.CompressInfo.iErrorNum > 0)  //����г�����Χ�ģ�����ʾ
            {
                for( int i = 0; i < maxBuffer.iCyNums; i++)
                {
                    if( myDetect.CompressInfo.bError[i])
                    {
                        if(myDetect.CompressInfo.fChar[i] > 0.0)
                        {
                            strTemp.printf("  �׺ţ�%02d��ѹ��ѹ����%5.2f MPa��", //Ȩ�أ�%5.2f��",
                                            maxBuffer.siarDieselNum[i],
                                            maxBuffer.farPMax[i]
                                            //,
                                            //myDetect.CompressInfo.fChar[i]
                                       );
                        }
                        else
                        {

                            strTemp.printf("  �׺ţ�%02d��ѹ��ѹ����%5.2f MPa��", //Ȩ�أ�%5.2f��",
                                            maxBuffer.siarDieselNum[i],
                                            maxBuffer.farPMax[i]
                                            //,
                                           // myDetect.CompressInfo.fChar[i]
                                       );
                        }
                        vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value",strTemp.c_str());
                    }
                }
            }
            else
            {
                strTemp = "      ����ѹ��ѹ����𲻴�!" ;
                vSheet.OlePropertyGet("Cells",irow++, 1).OlePropertySet("Value",strTemp.c_str());
            }
        }
        else
        {
            int itrainType;
            itrainType = lDS.LFHS.siTrainType;
            myDetect.SetCompressInfo( myDieselInfo[itrainType].fCompressMin,
                                  myDieselInfo[itrainType].fCompressMax);
            myDetect.AnalysisMCompress(maxBuffer);
            irow ++; //�ճ�һ��
            strTemp.printf(" **��������** ����ѹ��ѹ����������Χ��(%.2fMPa ~ %.2fMPa)",
                                                        myDieselInfo[itrainType].fCompressMin,
                                                        myDieselInfo[itrainType].fCompressMax);

            vSheet.OlePropertyGet("Cells",irow++, iStartP).OlePropertySet("Value",strTemp.c_str());
            if( myDetect.MCompInfo.iNFNums > 0)
            {
                if( myDetect.MCompInfo.iNFLeftNums > 0)
                {
                    strTemp.printf("��ѹ��ѹ����С�������� %d ���ף�", myDetect.MCompInfo.iNFLeftNums);
                    vSheet.OlePropertyGet("Cells",irow++, iStartP).OlePropertySet("Value", strTemp.c_str());
                    for( int i = 0; i < myDetect.MCompInfo.iNFLeftNums; i ++)
                    {
                        strTT.printf("�׺�:  %02d;  ����ѹ����%.3f;",
                                                        myDetect.MCompInfo.iNFLIndex[i],
                                                        myDetect.MCompInfo.fNFLPressure[i]);
                        vSheet.OlePropertyGet("Cells",irow++, iStartP + 1).OlePropertySet("Value",strTT.c_str());
                    }
                }
                if( myDetect.MCompInfo.iNFRightNums > 0)
                {
                    strTemp.printf("��ѹ��ѹ���ϴ�������� %d ���ף�", myDetect.MCompInfo.iNFRightNums);

                    vSheet.OlePropertyGet("Cells",irow++, iStartP).OlePropertySet("Value", strTemp.c_str());
                    for( int i = 0; i < myDetect.MCompInfo.iNFRightNums; i ++)
                    {
                        strTT.printf("�׺�:  %02d;  ����ѹ����%.3fMPa; ",
                                                        myDetect.MCompInfo.iNFRIndex[i],
                                                        myDetect.MCompInfo.fNFRPressure[i]);
                        vSheet.OlePropertyGet("Cells",irow++, iStartP + 1).OlePropertySet("Value",strTT.c_str());
                    }
                }
            }
            else
            {
                strTemp = "      ����ѹ��ѹ�����ڹ涨��Χ��!" ;
                vSheet.OlePropertyGet("Cells",irow++, iStartP).OlePropertySet("Value",strTemp.c_str());
            }
        }
    }
/*

*/
//=====================================================================
    vWorkBook.OleProcedure("Save");//�������
}

bool __fastcall TFormMain::IniFileRead(void)
{
//TODO: Add your source code here
    AnsiString strIniName;
    bool bret = false;
    strIniName = strExeFilePath + "SystemSetup.ini";
    if(FileExists(strIniName))   //���INI�ļ�����
    {
        bret = true;
    }

    TIniFile *pIniFile = new TIniFile(strIniName);

//----------------------------------------------------------------------------------------------
//  tMP.bInQiDaoCoef     = pIniFile->ReadBool("InletCoef", "bInQiDaoCoef",    false);

//    fDetectLevel = pIniFile->ReadFloat( "Option", "fDetectLevel", 6.0);
    delete pIniFile;
    return bret;
}

void __fastcall TFormMain::IniFileWrite(void)
{
    AnsiString strIniName;

    strIniName = strExeFilePath + "SystemSetup.ini";
    TIniFile *pIniFile = new TIniFile(strIniName);
//----------------------------------------------------------------------------------------------
//  pIniFile->WriteBool("InletCoef",  "bInQiDaoCoef",       tMP.bInQiDaoCoef );
//    pIniFile->WriteFloat("Option", "fDetectLevel", fDetectLevel );
    delete pIniFile;
}

void __fastcall TFormMain::ppmAddTypeClick(TObject *Sender)
{
    InsertToPipeBase(strPCPName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmTraceNum02Click(TObject *Sender)
{
    SetMenuChecked(2);
    bLoadList = true;
    GetListPageNum();
    DrawListDirect();    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmTraceNum04Click(TObject *Sender)
{
    SetMenuChecked(4);
    bLoadList = true;
    GetListPageNum();
    DrawListDirect();    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmTraceNum08Click(TObject *Sender)
{
    SetMenuChecked(8);
    bLoadList = true;
    GetListPageNum();
    DrawListDirect();    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmTraceNum16Click(TObject *Sender)
{
    SetMenuChecked(16);
    bLoadList = true;
    GetListPageNum();
    DrawListDirect();    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmTraceMergeClick(TObject *Sender)
{
    if(iListDrawKind == 1)
    {
        return;
    }
    ppmTraceMerge->Checked = !ppmTraceMerge->Checked;
    mnuTraceMerge->Checked = ppmTraceMerge->Checked;
//    CheckBoxMerge->Checked = ppmTraceMerge->Checked;
    bListTogether = ppmTraceMerge->Checked;
    DrawListDirect();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FileListBoxFileChange(TObject *Sender)
{
    if( FilterComboBoxFile->ItemIndex == 2)
    {
        FileListBoxFile->PopupMenu = PopupMenuPST;
    }
    else  if( FilterComboBoxFile->ItemIndex == 1)
    {
        FileListBoxFile->PopupMenu = PopupMenuPCP;
    }
    else  if( FilterComboBoxFile->ItemIndex == 0)
    {
        FileListBoxFile->PopupMenu = PopupMenuOrg;
    }

}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::FormTVAddTrainType(void)
{
    //TODO: Add your source code here
    AnsiString strTempTT,     strTempTN;
    AnsiString strChild,      strChildName;
    AnsiString strsCombustion, strsDate;
    AnsiString strEngineSpeed;
    AnsiString strGrandChild, strGrandChildName;
    TTreeNode* gNode, *lastNode;
    TTreeNode *rNode,*mNode,*sNode,*gsNode,*cmNode;
    AnsiString strTrainID;
    AnsiString strTemp;
    int iImageIndex;
/*
    if(bTVLoaded)
    {
        strTempTT = strExeFilePath + "TV.buf";
        if(FileExists(strTempTT))
        {
            TreeViewDatabase->LoadFromFile(strTempTT);
            return true;
        }
    }
*/
    FormTV->TreeViewDatabase->Items->Clear();
    rNode = FormTV->TreeViewDatabase->Items->Add(FormTV->TreeViewDatabase->Selected,"���ݿ�");  //������ڵ�
    rNode->ImageIndex    = 8;    //û��չ��ǰ��ͼ��
    rNode->SelectedIndex = 8;    //չ���Ժ��ͼ��
/*
    strTempTT = CboBTrainType->Text;
    strTempTN = CboBTrainNumber->Text;
*/
    strTempTT = "select distinct TrainType from DataTable order by TrainType";
    DM->ADOQueryTrainType->Close();
    DM->ADOQueryTrainType->SQL->Clear();
    DM->ADOQueryTrainType->SQL->Add (strTempTT);
    DM->ADOQueryTrainType->Open();
    int dataLength = DM->ADOQueryTrainType->RecordCount;
    if(dataLength == 0)
    {
        DM->ADOQueryTrainType->Close();
        return true;
    }
    DM->ADOQueryTrainType->First();
    while(!DM->ADOQueryTrainType->Eof)   //�����������
    {
        strTempTN = DM->ADOQueryTrainType->Fields->Fields[0]->Value;
        mNode   = FormTV->TreeViewDatabase->Items->AddChild(rNode,strTempTN);
        if(strTempTN == "����4A")
        {
            mNode->ImageIndex = 0;
            mNode->SelectedIndex = 0;
            iImageIndex = 0;
        }
        else if(strTempTN == "����4B")
        {
            mNode->ImageIndex = 1;
            mNode->SelectedIndex = 1;
            iImageIndex = 1;
        }
        else if(strTempTN == "����4C")
        {
            mNode->ImageIndex = 2;
            mNode->SelectedIndex = 2;
            iImageIndex = 2;
        }
        else if(strTempTN == "����4D")
        {
            mNode->ImageIndex = 3;
            mNode->SelectedIndex = 3;
            iImageIndex = 3;
        }
        else if(strTempTN == "����8B")
        {
            mNode->ImageIndex = 5;
            mNode->SelectedIndex = 5;
            iImageIndex = 5;
        }
        else if(strTempTN == "����11")
        {
            mNode->ImageIndex = 6;
            mNode->SelectedIndex = 6;
            iImageIndex = 6;
        }
        else
        {
            mNode->ImageIndex = 7;
            mNode->SelectedIndex = 7;
            iImageIndex = 7;
        }
        strTempTT = "select distinct TrainNumber from DataTable where TrainType = '";
        strTempTT += strTempTN;
        strTempTT += "' order by TrainNumber";
        DM->ADOQueryTrainNumber->Close();
        DM->ADOQueryTrainNumber->SQL->Clear();
        DM->ADOQueryTrainNumber->SQL->Add(strTempTT);
        DM->ADOQueryTrainNumber->Open();
        DM->ADOQueryTrainNumber->First();
        while(!DM->ADOQueryTrainNumber->Eof) //�����������
        {
            strChild = DM->ADOQueryTrainNumber->Fields->Fields[0]->Value;
            gNode = FormTV->TreeViewDatabase->Items->AddChild(mNode,strChild);
            gNode->ImageIndex    = iImageIndex;
            gNode->SelectedIndex = iImageIndex;

            DM->ADOQueryDate->Close();
            strTempTT = "select distinct MDate from DataTable where TrainType = '";
            strTempTT += strTempTN;
            strTempTT += "' and TrainNumber = '";
            strTempTT += strChild;
            strTempTT += "' order by MDate";
            DM->ADOQueryDate->SQL->Clear();
            DM->ADOQueryDate->SQL->Add(strTempTT);
            DM->ADOQueryDate->Open();
            DM->ADOQueryDate->First();

            while(!DM->ADOQueryDate->Eof)     //�����������
            {
                TDateTime dtTemp;
                dtTemp = TDateTime(DM->ADOQueryDate->Fields->Fields[0]->Value);
                unsigned short usyear,usmonth,usdate;
                dtTemp.DecodeDate(&usyear,&usmonth,&usdate);
                if(usyear<2000)
                {
                     usyear += 2000;
                }
                strChildName.printf("%04d-%02d-%02d",usyear,usmonth,usdate) ;

                gsNode = FormTV->TreeViewDatabase->Items->AddChild(gNode,strChildName);
                gsNode->ImageIndex    = 9;
                gsNode->SelectedIndex = 9;
                strTempTT = "select distinct DieselSpeed from DataTable where TrainType = '";
                strTempTT += strTempTN;
                strTempTT += "' and TrainNumber = '";
                strTempTT += strChild;
                strTempTT += "' and MDate between :TimeStart and :TimeEnd order by DieselSpeed";
                DM->ADOQuerySpeed->Close();
                DM->ADOQuerySpeed->SQL->Clear();

                DM->ADOQuerySpeed->Parameters->Clear();
                DM->ADOQuerySpeed->Parameters->AddParameter();
                DM->ADOQuerySpeed->Parameters->Items[0]->Name      = "TimeStart";
                DM->ADOQuerySpeed->Parameters->Items[0]->DataType  = ftDate;
                DM->ADOQuerySpeed->Parameters->Items[0]->Direction = pdInput;
                DM->ADOQuerySpeed->Parameters->AddParameter();
                DM->ADOQuerySpeed->Parameters->Items[1]->Name      = "TimeEnd";
                DM->ADOQuerySpeed->Parameters->Items[1]->DataType  = ftDate;
                DM->ADOQuerySpeed->Parameters->Items[1]->Direction = pdInput;
                DM->ADOQuerySpeed->Parameters->Items[0]->Value     = dtTemp - 1;
                DM->ADOQuerySpeed->Parameters->Items[1]->Value     = dtTemp;

                DM->ADOQuerySpeed->SQL->Add(strTempTT);
                DM->ADOQuerySpeed->Open();
                DM->ADOQuerySpeed->First();
                while(!DM->ADOQuerySpeed->Eof)   //������ͻ�ת��
                {
                    strDieselSpeed  = DM->ADOQuerySpeed->Fields->Fields[0]->Value;
                    strTemp  = strDieselSpeed + AnsiString(" r/min");
                    lastNode = FormTV->TreeViewDatabase->Items->AddChild(gsNode,strTemp);
                    lastNode->ImageIndex    = 10;
                    lastNode->SelectedIndex = 10;

                    strTempTT = "select distinct bCombustion from DataTable where TrainType = '";
                    strTempTT += strTempTN;
                    strTempTT += "' and TrainNumber = '";
                    strTempTT += strChild;
                    strTempTT += "' and MDate between :TimeStart and :TimeEnd ";
                    strTempTT += "  and DieselSpeed = '";
                    strTempTT += strDieselSpeed + "' order by bCombustion";
                    DM->ADOQueryTemp->Close();
                    DM->ADOQueryTemp->SQL->Clear();

                    DM->ADOQueryTemp->Parameters->Clear();
                    DM->ADOQueryTemp->Parameters->AddParameter();
                    DM->ADOQueryTemp->Parameters->Items[0]->Name      = "TimeStart";
                    DM->ADOQueryTemp->Parameters->Items[0]->DataType  = ftDate;
                    DM->ADOQueryTemp->Parameters->Items[0]->Direction = pdInput;
                    DM->ADOQueryTemp->Parameters->AddParameter();
                    DM->ADOQueryTemp->Parameters->Items[1]->Name      = "TimeEnd";
                    DM->ADOQueryTemp->Parameters->Items[1]->DataType  = ftDate;
                    DM->ADOQueryTemp->Parameters->Items[1]->Direction = pdInput;
                    DM->ADOQueryTemp->Parameters->Items[0]->Value     = dtTemp - 1;
                    DM->ADOQueryTemp->Parameters->Items[1]->Value     = dtTemp;

                    DM->ADOQueryTemp->SQL->Add(strTempTT);
                    DM->ADOQueryTemp->Open();
                    DM->ADOQueryTemp->First();
                    while(!DM->ADOQueryTemp->Eof)
                    {
                        strCombustion  = DM->ADOQueryTemp->Fields->Fields[0]->Value;
                        cmNode = FormTV->TreeViewDatabase->Items->AddChild(lastNode,strCombustion);
                        cmNode->ImageIndex    = 10;
                        cmNode->SelectedIndex = 10;
                        DM->ADOQueryTemp->Next();
                    }
                    DM->ADOQuerySpeed->Next();
                }
                DM->ADOQueryDate->Next();
            }
            DM->ADOQueryTrainNumber->Next();
        }
        DM->ADOQueryTrainType->Next();
    }
    if(DM->ADOQueryTrainType->Active)
    {
        DM->ADOQueryTrainType->Close();
    }
    if(DM->ADOQueryTrainNumber->Active)
    {
        DM->ADOQueryTrainNumber->Close();
    }
    if(DM->ADOQuerySpeed->Active)
    {
        DM->ADOQuerySpeed->Close();
    }
    if(DM->ADOQueryDate->Active)
    {
        DM->ADOQueryDate->Close();
    }
    if(DM->ADOQueryTemp->Active)
    {
        DM->ADOQueryTemp->Close();
    }
    strTempTT = strExeFilePath + "TV.buf";
    FormTV->TreeViewDatabase->SaveToFile(strTempTT);
    bTVLoaded = true;    //TTreeView
    return true;
}
void __fastcall TFormMain::PanelTVUnDock(TObject *Sender, TControl *Client,
      TWinControl *NewTarget, bool &Allow)
{
    PanelTV->Width    = 1;
    SplitterTV->Width = 1;
    bTVOuter          = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PanelTVDockOver(TObject *Sender,
      TDragDockObject *Source, int X, int Y, TDragState State,
      bool &Accept)
{
    PanelTV->Width    = FormTV->Width;
    SplitterTV->Width = 4;
    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonTVClick(TObject *Sender)
{
    if( ButtonTV->Caption == "�رղ�ѯ����")
    {
        ButtonTV->Caption  = "�򿪲�ѯ����";
        ButtonTV->Repaint();
        FormTV->Hide();
        iTVHeight = FormTV->Height;
        iTVWidth  = FormTV->Width;
        PanelTV->Width    = 1;
        SplitterTV->Width = 1;

    }
    else
    {
        ButtonTV->Caption = "�رղ�ѯ����";
        ButtonTV->Repaint();
        if( !bTVOuter)
        {
            PanelTV->Width    = FormTV->Width;
            SplitterTV->Width = 4;
        }
        FormTV->Show();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PanelTVDragDrop(TObject *Sender,
      TObject *Source, int X, int Y)
{
    bTVOuter = false;    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ChartCurveDblClick(TObject *Sender)
{
    FormEnlargeView->Visible = true;    
}
//---------------------------------------------------------------------------



bool __fastcall TFormMain::ProcessData(AnsiString& str)
{
    //TODO: Add your source code here
    AnsiString strTemp, strExt, strTName;
    FILE *fp, *fpNew;
    float *fPress = NULL, ftemp,ftp;
    int iNums;
    strTemp = str;
    strTName = "New" + ExtractFileName(strTemp);
    strTName = ExtractFilePath( strTemp ) + strTName;
    int ilen = strTName.Length();
    strTName.c_str()[ilen - 3] = 'D';
    strTName.c_str()[ilen - 2] = 'A';
    strTName.c_str()[ilen - 1] = 'T';
    strExt = ExtractFileExt(strTemp);
    strExt = strExt.UpperCase();
    if( strExt != ".TXT" )
    {
        return true;
    }
    if( FileExists( strTemp ) )
    {
        iNums = 0;
        if( (fp = fopen( strTemp.c_str(), "r"))!= NULL )
        {
            while(true)
            {
                if( fscanf(fp, "%f %f\n", &ftemp, &ftp) != EOF)
                {
                    iNums ++;
                }
                else
                {
                    break;
                }
            }
            fseek(fp, 0L, SEEK_SET );
            fPress = new float[iNums + 1];
            iNums = 0;
            if( fPress != NULL )
            {
                while(true)
                {
                    if( fscanf( fp, "%f %f",&ftemp, &fPress[iNums] ) != EOF )
                    {
                        iNums ++;
                    }
                    else
                    {
                        break;
                    }
                }
                if( iNums > 0 )
                {
                    float fTTT = fPress[0];
                    if( (fpNew = fopen( strTName.c_str(), "w")) != NULL)
                    {
                        for( int i = 0; i < iNums; i+= 2)
                        {
                            fprintf(fpNew, "%f %f %f\n", 0.1*i - 360.0, fPress[i], 5.0*(fPress[i] - fTTT) );
                            fTTT = fPress[i];
                        }
                        fclose(fpNew);
                    }
                }
                delete[] fPress;
            }
            fclose(fp);
        }
    }
    return true;
}
void __fastcall TFormMain::ButtonProcessClick(TObject *Sender)
{
    int iIndex;
    int iCount;
    AnsiString strTemp;
    iCount = FileListBox1->Items->Count;

    if( iCount > 0)
    {
        for( int i = 0; i < iCount; i ++)
        {
            FileListBox1->ItemIndex = i;
            strTemp = FileListBox1->FileName;
            ProcessData( strTemp);
        }
        Application->MessageBoxA("�������̽���!!!","��ʾ",MB_OK);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ppmInsertToDatabaseClick(TObject *Sender)
{
//    �������ѹ�������ݿ�
    if( strPCPName != "")
    {
        InsertToPipeBase(strPCPName);
    }
}
//---------------------------------------------------------------------------
