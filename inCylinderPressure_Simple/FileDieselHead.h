#ifndef __FileDieselHead_H__
#define __FileDieselHead_H__
struct StructDieselInfo
{
    unsigned char chCommandMark;   //应为0x99
    unsigned char chDieselGroup;   //组号
    char  chTemp[2];               //保留2个字节
    float fDiameter;               //气缸直径
	float fStroke;                 //活塞冲程
	float fLMDa;                   //曲柄连杆比
	float fCRatio;                 //压缩比
	float fPodLength;              //连杆长度
	float fInCloseAngle;           //进气门开启角度
	float fOutOpenAngle;           //排气门关闭角度
	float fCAING;                  //供油角度
    float fCompressMax;            //压缩压力最大值　
    float fCompressMin;            //压缩压力最小值
    float fOilAngleMax;            //供油提前角最大值　
    float fOilAngleMin;            //供油提前角最小值　
    float fCombustionAngleMax;     //燃烧提前角最大值　
    float fCombustionAngleMin;     //燃烧提前角最小值
    float fDetectLevel;                   //保留的1个浮点数
};
#endif
 