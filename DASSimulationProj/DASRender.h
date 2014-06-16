#pragma once

#include "Constants.h"
#include "3dsReader.h"	
//#include "Task.h"
#include "TriList.h"
#include "Textures.h"
#include "Point3D.h"
#include "3ds.h"
#include "Font.h"
#include <string>
#include <iostream>
#include "BusinessDataDefine.h"

using namespace std;

#define TIME_BEFORE_TASK_BEGIN	2*60
#define TASK_EXECUTE_GAP 1

enum EPhaseDrawType
{
	enmPhase1_Request = 0,
	enmPhase1_Answer  = 1,
	enmPhase1_UnexpectedMAR_Requset = 2,//突发返向模式
	enmPhase1_UnexpectedMAR_Answer  = 3,
	enmPhase2_Answer = 4,
	enmPhase1_Occupy = 5,
	enmPhase_None = -1,
};

class CDASRender
{
public:
	~CDASRender();
	static CDASRender* Instance();
	static void DestroyInstance();
	
	// OpenGL initialize
	void InitOpenGL(CDC* pDC);
	void DisableOpenGL();
	void InitLight();
	void ResizeViewPort(int cx, int cy);

	// Load Resources
	void Load3dsModels();
	void LoadTextures();

	// Get openGL Mode.
	bool IsOpenGL()		{	return m_bOpenGL;	}

	// Draw scene.
	void DrawDASScence();

	// Timer
	void CalculateUsAngleStep(CTime& timeBegin, CTime& timeEnd, DWORD dwEllapsTime);
	void ReCalculateAngle();
	void ReFreshTime();
	void SetSysTimeFrequency(int nTimes);
	int  GetTimeFrequency()	{	return m_nTimeFrequency;	}
	CTime GetShowTime();

	// Key message process
	void ScaleView(UINT nChar);

	//void SetUsAngle(float fNewAngle);
	void SetNewTimeInView(CTime& tStartTime, CTime& tBaseTime);

	void SetPhase12Flag(EPhaseDrawType eType);
	//void SetCountDownFlag(BOOL bCountDown);
	//void DrawCountDown();
	//int  m_nNum;
	
	void SetSimulateFlag(BOOL bSimulate);
	void SetMAFFlag(BOOL bMAF);
	void SetMAFMARFlag(BOOL bMAFMAR);

	//maf
	void SetDrawTDRS2US(BOOL bFlag);
	void SetDrawTer2TDRS(BOOL bFlag);
	//mar
	void SetDrawTDRS2Ter(BOOL bFlag);
	void SetDrawTer2UsrCen(BOOL bFlag);

	void ReSetStatus();

private:
	CDASRender();
	static CDASRender* m_pInstance;
protected:
	void BuildFont();
	void KillFont();
	void glPrint(const TCHAR *fmt, ...);
	void InitPixelFormat();
	void DrawDas();
	void DrawDasAxis(Point3D originPoint);
	void DrawDasGroundTerminal(Point3D originPoint);
	void DrawDasUserCenter(Point3D centerPoint);
	void DrawDasUserCenter22(Point3D centerPoint);
	void DrawNCC(Point3D originalPoint);
	void DrawFDF(Point3D originalPoint);
	void DrawDasTdrs(Point3D centerPoint);
	void DrawDasEarth(Point3D centerPoint);
	void DrawDasUserSatellite(Point3D centerPoint);
	void DrawDasSignal(int direction, Point3D start, Point3D end);
	void DrawDasRotateCircle(COLORREF color, int lineType);
	void DrawString3f(float x, float y, float z, CString content);
	void drawCNString(const char* str);
	void drawCNString(float x, float y, float z, const char* str);
	void selectFont(int size, int charset, const char* face);
	wstring ANSIToUnicode(const char* str , CString& strWide );
	BOOL MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize);



protected:
	
	C3DS	m_3dsTDRS;
	C3DS	m_3dsUS;
	//C3DS       m_3dsPOCC;
	C3DS	m_3dsBuilding_1;
	C3DS	m_3dsBuilding_2;
	C3DS	m_3dsBuilding_3;

	CClientDC* m_pDC;
	HGLRC	   m_hglrc;	//OpenGL Render描述表（OpenGL绘图环境）
	bool	   m_bOpenGL;

	//GLFont	   m_font;	   
	
	// Model
	//CTriList m_listTdrs;
	//CTriList m_listUS;
	CTriList m_listPOCC;
	CTriList m_listPocc22;

	bool	 m_bIsTdrsLoaded;
	bool     m_bIsUsLoaded;
	bool	 m_bIsPOCCLoaded;
	bool	 m_bIsPocc22Loaded;

	// Texture
	CTextures	m_Texture;
	GLuint		m_arrTextureID[3];

	// Geomatric para.
	//float m_fScale;
	//float m_fEarthRadius;
	//float m_fUsRadius;
	//float m_fUsAngle;			//弧度。非角度。
	//float m_fUsAngleStep;		//每秒的角度步长,弧度。在[1.0--2.1]之间
	float m_RadarRotateAngle;
	float m_fSignalStep;

	// Time
	CTime m_ctimeShown;		//显示在View中的模拟时间
	CTime m_baseTime;		//用户设置的模拟时间
	CTime m_sysStartTime;	//用户设置时间的那一刻的真实时间
	CTime m_sysCurrentTime;	//系统的当前真实时间

	int  m_nTimeFrequency;	//系统时间倍率。

	// 是否在倒计时
	//BOOL m_bCountDown;
	BOOL m_bSimulateTask;
	BOOL m_bMAFTask;//TRUE=MAF,FALSE=MAR
	BOOL m_bMAFMARTask;

	EPhaseDrawType m_ePhaseType;
	

	//maf
	BOOL m_bTer2TDRS;
	BOOL m_bTDRS2US;
	//mar
	BOOL m_bTDRS2Ter;
	BOOL m_bTer2UsrCen;
	
	CCriticalSection m_csShowtime;
};