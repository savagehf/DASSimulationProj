#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDasSimuView construction/destruction

static const float s_constMinAngle = 1.1f;
static const float s_constMaxAngle = 2.2f;
static float s_fStartArc = 0.0f;

//================场景基准点==============================
const static float zValue = -2.0f;
//gluLookAt(0, 0, 3, 0, 0, -3, 0, 1, 0);
//视点位置
const static Point3D eyePos(0,0,3);
//看向lookAt点
const static Point3D lookAtPos(0,0,-3);
//场景中心点位置
const static Point3D sceneCenterPos(0,0,-3);
//==============================================
const int	Signal_Dir_Forward = 1 ;	
const int	Signal_Dir_Rerturn = 2 ;
const float g_constSignalColor[3][3] = {{1,0,1},{0,1,0},{0,0,1}};

static Point3D s_ptTDRSCenter;//(-15, 12, -30);
static Point3D s_ptGoundTerminalBase;//(-3.0,-2,4);
static Point3D s_ptEarthCenter;
static Point3D s_ptUsrCenter;

float g_arrModelview_tmp[16];
float g_arrModelview[16];
CArray<CTime, CTime&>	g_arrMafEmptyWin;
//CArray<CTask, CTask&>	g_taskArray;
