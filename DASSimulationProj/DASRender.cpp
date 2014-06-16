#include "stdafx.h"
#include "DASRender.h"

#include "Math.h"
#include "MyTools.h"
#include "CommDefine.h"
#include "Logging/Logging.h"



#define COLOR_MAF RGB(0,1.0,0)
#define COLOR_MAR RGB(1.0,0,1.0)


float m_fScale;
float m_fUsAngle;			//弧度。非角度。
float m_fUsAngleStep;		//每秒的角度步长,弧度。在[1.0--2.1]之间
const float m_fEarthRadius = 12.0f;
const float m_fUsRadius	   = 14.0f;

CDASRender* CDASRender::m_pInstance = NULL;

GLuint	base;

CDASRender::CDASRender()
{
	m_bIsTdrsLoaded		= false;
	m_bIsUsLoaded		= false;
	m_bIsPOCCLoaded		= false;
	m_bIsPocc22Loaded   = false;
	m_bOpenGL			= true;

	m_RadarRotateAngle	= 0.0f;
	m_fSignalStep		= 0.0f;
	
	int  nFullWidth=GetSystemMetrics(SM_CXSCREEN);  
	int  nFullHeight=GetSystemMetrics(SM_CYSCREEN);  
// 	if (nFullWidth > 1024)
// 	{
// 		m_fScale = 1.0f;
// 	}
// 	else 
	if(nFullWidth >800/* && nFullWidth <= 1024*/)
	{
		m_fScale = 0.8f;
	}
	else if (nFullWidth <= 800)
	{
		m_fScale = 0.6;
	}
	LOG_NOTICE(_T("Initialize nFullWidth = %d, nFullHeight = %d, m_fScale = %f"), nFullWidth, nFullHeight, m_fScale);
	
	m_fUsAngle			= s_constMinAngle;		//用户星 可见范围[1.0, 2.1]
	m_fUsAngleStep		= 0.15f/180.0f * PI;

	m_baseTime			= CTime::GetCurrentTime();
	m_ctimeShown		= CTime::GetCurrentTime();
	m_sysStartTime		= CTime::GetCurrentTime();
	m_sysCurrentTime	= CTime::GetCurrentTime();
	m_nTimeFrequency		= 1;

	//m_bCountDown = FALSE;
	//m_nNum = 0;
	m_bSimulateTask = FALSE;
	m_bMAFTask = TRUE;
	m_bMAFMARTask = FALSE;
	m_ePhaseType = enmPhase_None;

	//maf
	m_bTer2TDRS = FALSE;
	m_bTDRS2US = FALSE;
	//mar
	m_bTDRS2Ter = FALSE;
	m_bTer2UsrCen = FALSE;
	

}

CDASRender* CDASRender::Instance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CDASRender();
	}

	return m_pInstance;
}

void CDASRender::DestroyInstance()
{
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

CDASRender::~CDASRender()
{
	wglDeleteContext(m_hglrc);
	if(m_pDC) 
	{
		delete m_pDC;
	}
}

void CDASRender::InitOpenGL(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;  
	m_pDC = (CClientDC*)pDC;

	// 设置当前的绘图像素格式  
	InitPixelFormat();

	int n=::GetPixelFormat(m_pDC->GetSafeHdc());  
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n,sizeof(pfd),&pfd);  
	// 创建绘图描述表  
	m_hglrc=wglCreateContext(m_pDC->GetSafeHdc());  
	// 使绘图描述表为当前调用现程的当前绘图描述表  
	wglMakeCurrent(m_pDC->GetSafeHdc(),m_hglrc);
	glEnable(GL_DEPTH_TEST);  

	//创建大号字体
	BuildFont();

	//if(!m_font.InitFont())
	//	return ;

	//GLfloat ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	//GLfloat diffuse[] = { 0.3, 0.3, 0.3, 1.0 };
	//GLfloat specular[] = { 0.6, 0.6, 0.6, 1.0 };
	//GLfloat position[] = { 1.0,10.0,-10.0,0.0 };//light0 位置
	//GLfloat lmodel_ambient[] = { 0.2, 0.7, 0.5, 1.0 };

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glEnable( GL_COLOR_MATERIAL); 
	//glShadeModel(GL_SMOOTH);


	//glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	//glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST );
	//glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	//glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	//glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	/////////////////////////////////////////////
	////	glAlphaFunc(GL_GEQUAL,0.1f);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE); //字体显示要求,不能少
	//glDepthFunc(GL_LEQUAL);
	//glReadBuffer(GL_FRONT);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //不能去掉,否则会非法操作,不知为什么


}

void CDASRender::BuildFont()								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(	-200,							// Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		FW_BOLD,						// Font Weight
		FALSE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
		_T("Courier New"));					// Font Name

	oldfont = (HFONT)SelectObject(m_pDC->GetSafeHdc(), font);           // Selects The Font We Want
	wglUseFontBitmaps(m_pDC->GetSafeHdc(), 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(m_pDC->GetSafeHdc(), oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

void CDASRender::KillFont()									// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

// void CDASRender::DrawCountDown()
// {
// 	//设置清屏颜色为黑色  
// 	glClearColor(0.1f,0.1f,0.1f,0.0f);  
// 	//清除颜色缓冲区和深度缓冲区  
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
// 	glLoadIdentity();
// 	glTranslatef(0.0f,0.0f,-10.0f);	
// 
// 	glPushMatrix();  
// 	glColor3d(1.0, 0.0, 0.0); 
// 
// 	glRasterPos3d(-1.0,0,0);
// 	//glPrint(_T("%d"), m_nNum);
// 	glPopMatrix();
// 
// 	glFlush();	glFinish(); 
// 	SwapBuffers(/*wglGetCurrentDC()*/m_pDC->GetSafeHdc());
// }	


void CDASRender::glPrint(const TCHAR *fmt, ...)					// Custom GL "Print" Routine
{
	TCHAR		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	_vstprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(_tcslen(text)+1, GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}

void CDASRender::DisableOpenGL()
{
	m_bOpenGL = false;
	wglDeleteContext(m_hglrc);
	if(m_pDC) 
	{
		//delete m_pDC;
	}
}

void CDASRender::ResizeViewPort(int cx, int cy)
{
	glViewport(0,0,cx,cy);
	//透视投影变换 
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();  
	gluPerspective(45, (double)cx/(double)cy,1,100);  
	//视角变换  
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity();  
	gluLookAt(0,0,10,0,0,0,0,1,0);  
}

void CDASRender::InitPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =   
	{  
		sizeof(PIXELFORMATDESCRIPTOR),  // pfd结构的大小   
		1,                              // 版本号   
		PFD_DRAW_TO_WINDOW |            // 支持在窗口中绘图   
		PFD_SUPPORT_OPENGL |            // 支持 OpenGL   
		PFD_DOUBLEBUFFER,               // 双缓存模式   
		PFD_TYPE_RGBA,                  // RGBA 颜色模式   
		24,                             // 24 位颜色深度   
		0, 0, 0, 0, 0, 0,               // 忽略颜色位   
		0,                              // 没有非透明度缓存   
		0,                              // 忽略移位位   
		0,                              // 无累计缓存   
		0, 0, 0, 0,                     // 忽略累计位   
		32,                             // 32 位深度缓存       
		0,                              // 无模板缓存   
		0,                              // 无辅助缓存   
		PFD_MAIN_PLANE,                 // 主层   
		0,                              // 保留   
		0, 0, 0                         // 忽略层,可见性和损毁掩模   

	};  
	int pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);  
	// 为设备描述表得到最匹配的像素格式   
	if(pixelFormat == false)  
	{  
		AfxMessageBox( _T("ChoosePixelFormat failed") );  
		
		return ;  
	}  
	// 设置最匹配的像素格式为当前的像素格式   
	if(SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd) == false)  
	{  
		AfxMessageBox( _T("SetPixelFormat failed") );  
		return ;  
	}  
}

void CDASRender::InitLight()
{

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable( GL_COLOR_MATERIAL); 
	glShadeModel(GL_SMOOTH);
	

	//设置light0
	GLfloat ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat diffuse[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat specular[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat position[] = { -10.0,10.0,-10.0,0.0 };//light0 位置
	GLfloat lmodel_ambient[] = { 0.2, 0.7, 0.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);

	// 用户星旁边的Light
	static float Light1Ambient[]   = {0.2f, 0.4f, 0.6f, 1.0f};	//环境光
	//static float Light1Diffuse[]   = {0.5f,0.3f,0.7f,1.0f};//{0.7f, 0.4f, 0.2f, 1.0f};	//反射光（漫反射）
	static float Light1Position[]  = {9.0f, 4.0f, 0.0f,1.0f};//{0.0f, 2.0f, 0.0f, 1.0f};
	glLightfv(GL_LIGHT1, GL_AMBIENT, Light1Ambient);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, Light1Diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,Light1Position);
	glEnable(GL_LIGHT1);

	// TDRS附近的Light
	static float Light2Ambient2[]  = {0.4f, 0.4f, 0.4f, 1.0f};
	static float Light2Diffuse2[]  = {0.0f, 0.4f, 0.4f, 1.0f};
	static float Light2Position2[] = {-15.0f, 30.0f, -30.0f,1.0f};//{-8.0f, 0.0f, 10.0f, 1.0f};

	glLightfv(GL_LIGHT2, GL_AMBIENT, Light2Ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Light2Diffuse2);
	glLightfv(GL_LIGHT2, GL_POSITION,Light2Position2);
	glEnable(GL_LIGHT2);



	// The following commands should induce OpenGL to create round points and   
	// antialias points and lines.  (This is implementation dependent unfortunately).  
	//RGBA mode antialias need cooperate with blend function.  	
	//glEnable(GL_LINE_SMOOTH); 
	//glEnable(GL_POLYGON_SMOOTH);

	//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points  
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Antialias the lines  
	//glHint(GL_POLYGON_SMOOTH, GL_NICEST);
	//glEnable(GL_BLEND);  
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

}

void CDASRender::Load3dsModels() 
{
	//Load TDRS模型
	m_3dsTDRS.Init("ModelData\\tdrs_new.3DS");

	//Load US
	m_3dsUS.Init("ModelData\\UsrStallite.3ds");
	//m_3dsUS.Init("ModelData\\deep.3ds");

	m_3dsBuilding_1.Init("ModelData\\1.3DS");
	//m_3dsBuilding_2.Init("ModelData\\building_model_2.3DS");
	m_3dsBuilding_3.Init("ModelData\\3.3DS");

	//没有纹理的tdrs模型
	C3dsReader Loader;
	//POCC Building.
	if (m_listPOCC.getNumObjects() > 0)
	{
		m_listPOCC.removeAllObjects();
	}
	m_bIsPOCCLoaded = Loader.Reader("ModelData\\dc10.3ds", &m_listPOCC);
	m_listPOCC.doAfterMath();

	C3dsReader loader22;
	if (m_listPocc22.getNumObjects() > 0)
	{
		m_listPocc22.removeAllObjects();
	}
	m_bIsPocc22Loaded = loader22.Reader("ModelData\\POCC.3ds", &m_listPocc22);
	m_listPocc22.doAfterMath();

	//m_3dsPOCC.Init("ModelData\\1.3DS");
	

}

void CDASRender::LoadTextures()
{
	m_Texture.LoadTextures(m_arrTextureID, 1/*TOTAL_TEXTURE_NUM*/);
}

void CDASRender::DrawDASScence()
{
	if (m_bOpenGL) 
	{

		glPushMatrix();
		glScalef(m_fScale,m_fScale,m_fScale); 
		LOG_NOTICE(_T("m_fScale = %f"), m_fScale);
		//绘制倒计时。
		//if (m_bCountDown)
		//{
			//DrawCountDown();
		//}
		//else
		//{	
			DrawDas();	
		//}

		glPopMatrix();
		
	}
}


std::string str1("DAF:提交服务请求");
std::string str2("DAF:发送给中继星");
std::string str3("DAF:发送给用户星");
std::string str4("DAR:发送给中继星");
std::string str5("DAR:发送给地面站");
std::string str6("DAR:发送给用户中心");

void CDASRender::DrawDas()
{
	//设置清屏颜色为黑色  
	glClearColor(0.1f,0.1f,0.1f,0.0f);  
	//清除颜色缓冲区和深度缓冲区  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  


	//绘制坐标轴
	Point3D axisOrigin(-7,-3.5,0);
	DrawDasAxis(axisOrigin);

	//绘制当前时间+其他文本
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	//glLoadIdentity();
	CString timeStr = MyTools::GetFormatedDateTime(m_ctimeShown);
	DrawString3f(2,3.8,0,timeStr);
	CString usAngle;	
	usAngle.Format(_T("US1 angle:%f"),m_fUsAngle);
	DrawString3f(2,3.55,0,usAngle);
	DrawString3f(-2.5,2.5,0,_T("TDRS #1"));

	glPopMatrix();

	//绘制地面站
	s_ptGoundTerminalBase = Point3D(-1.0,-2.4,3.5);//Point3D(-3.0,-2,4);
	DrawDasGroundTerminal(s_ptGoundTerminalBase);
	CString strGT("GT");
	glColor3f(1.0,0.0,1.0);
	DrawString3f(-0.6, -2.0, 3.5, strGT);

	//绘制用户中心
	Point3D userCenterPoint(0,0,0);
	DrawDasUserCenter(userCenterPoint);
	CString strPOCC("POCC1");
	glColor3f(1.0, 0.0, 1.0);
	DrawString3f(-1.9, -1.2, 1.0, strPOCC);

	//绘制用户中心2
	Point3D userCenter22(0,0,0);
	DrawDasUserCenter22(userCenter22);
	CString strPocc22("POCC2");
	glColor3f(1.0, 0.0, 1.0);
	DrawString3f(1.9,-1.2,1.0, strPocc22);



	//绘制NCC
	Point3D nccPoint(-3.0,-2,4);//(-1.0,-2.2,3.4);
	DrawNCC(nccPoint);
	CString strNCC(_T("NCC"));
	glColor3f(1.0,0.0,1.0);
	DrawString3f(-2.8,-2.0,3.5, strNCC);

	//绘制FDF
	Point3D fdfPoint(1.5,-2.2,3.4);
	DrawFDF(fdfPoint);
	CString strFDF(_T("FDF"));
	glColor3f(1.0,0.0,1.0);
	DrawString3f(2.2, -2.0, 3.5, strFDF);


	//绘制中继星
	glColor3f(0.2,0.2,0.2);
	s_ptTDRSCenter = Point3D(-15, 12, -30);
	DrawDasTdrs(s_ptTDRSCenter);

	//绘制地球
	glColor3f(1,1,1);
	s_ptEarthCenter = Point3D(0, -14, 0);
	DrawDasEarth(s_ptEarthCenter);

	//绘制卫星
	DrawDasUserSatellite(s_ptEarthCenter);

	switch(m_ePhaseType)
	{
	case enmPhase1_Request:
		{
			glPushMatrix();
			// 1. 用户中心-->地面站
			Point3D ptUserCen;
			ptUserCen.x = userCenterPoint.x;
			ptUserCen.y = userCenterPoint.y-1.5;
			ptUserCen.z = userCenterPoint.z+0.2;
			DrawDasSignal(2, ptUserCen, s_ptGoundTerminalBase);

			Point3D ptFDF;
			ptFDF.x = fdfPoint.x;
			ptFDF.y = fdfPoint.y;
			ptFDF.z = fdfPoint.z;
			DrawDasSignal(2, ptFDF, s_ptGoundTerminalBase);

			Point3D ptNCC;
			if (m_bMAFTask)
			{
				ptNCC.x = nccPoint.x;
				ptNCC.y = nccPoint.y;
				ptNCC.z = nccPoint.z;
				DrawDasSignal(2, ptNCC, s_ptGoundTerminalBase);
			}
			

			//用户中心-->地面站
			selectFont(20, DEFAULT_CHARSET, "华文仿宋");
			glColor3f(1.0f, 0.0f, 0.0f);
			glRasterPos3f(ptUserCen.x-0.5,ptUserCen.y,ptUserCen.z+3);
			drawCNString("  1:发送规划信息请求");

			glRasterPos3f(ptFDF.x-1, ptFDF.y, ptFDF.z+1);
			drawCNString("  1:FDF向DASCON提供TDRS和用户飞行器飞行状态向量");

			if(m_bMAFTask)
			{
				glRasterPos3f(ptNCC.x, ptNCC.y, ptNCC.z+1);
				drawCNString("  1:NCC定时向DASCON提供TUT表");
			}
			

			glPopMatrix();
		}
		break;
	case enmPhase1_Answer:
		{
			//地面站--->pocc
			glPushMatrix();
			Point3D ptUserCen;
			ptUserCen.x = userCenterPoint.x;
			ptUserCen.y = userCenterPoint.y-1.5;
			ptUserCen.z = userCenterPoint.z+0.2;
			DrawDasSignal(/*1*/2, s_ptGoundTerminalBase, ptUserCen);

			selectFont(20, DEFAULT_CHARSET, "华文仿宋");
			glColor3f(1.0f, 0.0f, 0.0f);
			glRasterPos3f(s_ptGoundTerminalBase.x+1,s_ptGoundTerminalBase.y+0.7,s_ptGoundTerminalBase.z-1);
			drawCNString("  2:返回规划信息");

			glPopMatrix();
		}
		break;
	case enmPhase1_UnexpectedMAR_Requset:
		{
			glPushMatrix();
			if (m_fUsAngle > s_constMinAngle && m_fUsAngle < s_constMaxAngle) 
			{
				//1. 用户星-->中继星
				DrawDasSignal(1,s_ptUsrCenter,s_ptTDRSCenter);

				selectFont(20, DEFAULT_CHARSET, "华文仿宋");
				glColor3f( 1.0f, 0.0f,0.0f);
				glRasterPos3f(s_ptUsrCenter.x,s_ptUsrCenter.y+1,s_ptUsrCenter.z-1);
				drawCNString("  1:需要DAR服务的UP直接发送请求给TDRS");


				// 2. 中继星-->地面站
				Point3D ptStart(s_ptTDRSCenter.x, s_ptTDRSCenter.y, s_ptTDRSCenter.z);
				Point3D ptEnd(s_ptGoundTerminalBase.x, s_ptGoundTerminalBase.y, s_ptGoundTerminalBase.z);
				DrawDasSignal(1, ptStart/*s_ptTDRSCenter*/, ptEnd/*s_ptGoundTerminalBase*/);

				selectFont(20, DEFAULT_CHARSET, "华文仿宋");
				glColor3f(1.0f, 0.0f, 0.0f);
				glRasterPos3f(s_ptTDRSCenter.x+1,s_ptTDRSCenter.y-4,s_ptTDRSCenter.z-1);
				drawCNString("  2:TDRS将服务请求转发到地面终端");


				// 3. 地面站-->用户中心
				Point3D ptUserCen;
				ptUserCen.x = userCenterPoint.x;
				ptUserCen.y = userCenterPoint.y-1.5;
				ptUserCen.z = userCenterPoint.z+0.2;
				DrawDasSignal(/*2*/1, s_ptGoundTerminalBase, ptUserCen);

				selectFont(20, DEFAULT_CHARSET, "华文仿宋");
				glColor3f( 1.0f, 0.0f, 0.0f);
				glRasterPos3f(s_ptGoundTerminalBase.x+1,s_ptGoundTerminalBase.y+1,s_ptGoundTerminalBase.z-1);
				drawCNString("  3:DAS通知用户UP请求发起突发返向数据传输模式");

			}
			glPopMatrix();
		}
		break;
	case enmPhase1_UnexpectedMAR_Answer:
		{
			glPushMatrix();
			if (m_fUsAngle > s_constMinAngle && m_fUsAngle < s_constMaxAngle) 
			{
				// 3. 用户中心-->地面站
				Point3D ptUserCen;
				ptUserCen.x = userCenterPoint.x;
				ptUserCen.y = userCenterPoint.y-1.5;
				ptUserCen.z = userCenterPoint.z+0.2;
				DrawDasSignal(0, ptUserCen, s_ptGoundTerminalBase);

				selectFont(20, DEFAULT_CHARSET, "华文仿宋");
				glColor3f( 1.0f, 0.0f, 0.0f);
				glRasterPos3f(s_ptGoundTerminalBase.x+1,s_ptGoundTerminalBase.y+1,s_ptGoundTerminalBase.z-1);
				drawCNString("  1:POCC已接受服务请求，开始准备传输数据");


				// 2. 地面站-->中继星
				Point3D ptStart(s_ptTDRSCenter.x, s_ptTDRSCenter.y, s_ptTDRSCenter.z);
				Point3D ptEnd(s_ptGoundTerminalBase.x, s_ptGoundTerminalBase.y, s_ptGoundTerminalBase.z);
				DrawDasSignal(0, ptEnd, ptStart);

				selectFont(20, DEFAULT_CHARSET, "华文仿宋");
				glColor3f(1.0f, 0.0f, 0.0f);
				glRasterPos3f(s_ptTDRSCenter.x+1,s_ptTDRSCenter.y-4,s_ptTDRSCenter.z-1);
				drawCNString("  2:DAS将服务请求结果返向传输回TDRS");

				//1. 中继星-->用户星
				DrawDasSignal(0,s_ptTDRSCenter,s_ptUsrCenter);

				selectFont(20, DEFAULT_CHARSET, "华文仿宋");
				glColor3f( 1.0f, 0.0f,0.0f);
				glRasterPos3f(s_ptUsrCenter.x,s_ptUsrCenter.y+1,s_ptUsrCenter.z-1);
				drawCNString("  3:TDRS将服务请求结果转发到UP");
			}
			glPopMatrix();
		}
		break;
	case enmPhase2_Answer:
		{
			//地面站--->pocc
			glPushMatrix();
			Point3D ptUserCen;
			ptUserCen.x = userCenterPoint.x;
			ptUserCen.y = userCenterPoint.y-1.5;
			ptUserCen.z = userCenterPoint.z+0.2;
			DrawDasSignal(/*2*/2, s_ptGoundTerminalBase, ptUserCen);

// 			selectFont(20, DEFAULT_CHARSET, "华文仿宋");
// 			glColor3f(1.0f, 0.0f, 0.0f);
// 			glRasterPos3f(s_ptGoundTerminalBase.x+1,s_ptGoundTerminalBase.y+0.7,s_ptGoundTerminalBase.z-1);
// 			drawCNString("  返回规划信息");

			glPopMatrix();
		}
		break;
	case enmPhase1_Occupy:
		{
			//地面站--->pocc2(被抢占者)
			glPushMatrix();
			Point3D ptUserCen;
			ptUserCen.x = userCenter22.x+2.0;
			ptUserCen.y = userCenter22.y-1.5;
			ptUserCen.z = userCenter22.z+0.2;
			DrawDasSignal(2, s_ptGoundTerminalBase, ptUserCen);

			selectFont(20, DEFAULT_CHARSET, "华文仿宋");
			glColor3f( 1.0f, 0.0f,0.0f);
			glRasterPos3f(s_ptGoundTerminalBase.x+1,s_ptGoundTerminalBase.y+0.7,s_ptGoundTerminalBase.z-1);
			drawCNString("  任务被抢占！");
			glPopMatrix();
		}
		break;
	default:
		break;
	}

	//1. 是否需要模拟信号
	if (m_bSimulateTask)
	{
		//双向任务
		if (m_bMAFMARTask)
		{
			//前向
			glPushMatrix();
			if (m_fUsAngle > s_constMinAngle && m_fUsAngle < s_constMaxAngle) 
			{
				glPushMatrix();
				// 1. 用户中心-->地面站
				Point3D ptUserCen;
				ptUserCen.x = userCenterPoint.x;
				ptUserCen.y = userCenterPoint.y-1.5;
				ptUserCen.z = userCenterPoint.z+0.2;
				DrawDasSignal(0, ptUserCen, s_ptGoundTerminalBase);


				selectFont(20, DEFAULT_CHARSET, "华文仿宋");
				glColor3f( 1.0f, 0.0f, 0.0f);
				glRasterPos3f(ptUserCen.x-1,ptUserCen.y,ptUserCen.z+3);
				drawCNString("  1:地面站转发来自POCC和中继星的数据");
				glPopMatrix();

				glPushMatrix();
				if (m_bTer2TDRS)
				{
					// 2. 地面站-->中继星
					DrawDasSignal(0,s_ptGoundTerminalBase,s_ptTDRSCenter);

					selectFont(20, DEFAULT_CHARSET, "华文仿宋");
					glColor3f(1.0f, 0.0f,  0.0f);
					glRasterPos3f(s_ptTDRSCenter.x+3,s_ptTDRSCenter.y-4,s_ptTDRSCenter.z-1);
					drawCNString("  2:中继星转发来自地面站和用户星的数据");
				}
				glPopMatrix();

				glPushMatrix();
				if (m_bTDRS2US)
				{
					//3. 中继星-->用户星
					DrawDasSignal(/*2*/0,s_ptTDRSCenter,s_ptUsrCenter);

					selectFont(20, DEFAULT_CHARSET, "华文仿宋");
					glColor3f( 1.0f, 0.0f,0.0f);
					//glRasterPos3f(s_ptTDRSCenter.x+3,s_ptTDRSCenter.y-4,s_ptTDRSCenter.z-1);
					glRasterPos3f(s_ptUsrCenter.x,s_ptUsrCenter.y+1,s_ptUsrCenter.z-1);
					drawCNString("  3:用户星接收和发送数据");
				}
				glPopMatrix();
			}
			glPopMatrix();

			//反向任务
			glPushMatrix();
			if (m_fUsAngle > s_constMinAngle && m_fUsAngle < s_constMaxAngle) 
			{
				//1. 用户星-->中继星
				DrawDasSignal(1,s_ptUsrCenter,s_ptTDRSCenter);

				/*selectFont(20, DEFAULT_CHARSET, "华文仿宋");
				glColor3f( 0.0f,1.0f,0.0f);
				glRasterPos3f(s_ptUsrCenter.x,s_ptUsrCenter.y,s_ptUsrCenter.z-1);
				drawCNString("  数据从用户星传输给中继星");*/


				// 2. 中继星-->地面站
				if (m_bTDRS2Ter)
				{
					Point3D ptStart(s_ptTDRSCenter.x, s_ptTDRSCenter.y, s_ptTDRSCenter.z);
					Point3D ptEnd(s_ptGoundTerminalBase.x, s_ptGoundTerminalBase.y, s_ptGoundTerminalBase.z);
					DrawDasSignal(1, ptStart/*s_ptTDRSCenter*/, ptEnd/*s_ptGoundTerminalBase*/);

					/*selectFont(20, DEFAULT_CHARSET, "华文仿宋");
					glColor3f(0.0f,1.0f,0.0f);
					glRasterPos3f(s_ptTDRSCenter.x+1.4,s_ptTDRSCenter.y-3,s_ptTDRSCenter.z-1);
					drawCNString("  数据从中继星传输给地面站");*/
				}

				// 3. 地面站-->用户中心
				if (m_bTer2UsrCen)
				{
					Point3D ptUserCen;
					ptUserCen.x = userCenterPoint.x;
					ptUserCen.y = userCenterPoint.y-1.5;
					ptUserCen.z = userCenterPoint.z+0.2;
					DrawDasSignal(/*2*/1, s_ptGoundTerminalBase, ptUserCen);

					/*selectFont(20, DEFAULT_CHARSET, "华文仿宋");
					glColor3f( 0.0f,1.0f,0.0f);
					glRasterPos3f(s_ptGoundTerminalBase.x+1,s_ptGoundTerminalBase.y+1,s_ptGoundTerminalBase.z-1);
					drawCNString("  地面站通过网络将数据传输给POCC");*/
				}
			}
			glPopMatrix();
		}
		else
		{
			// 1.1前向任务
			if (m_bMAFTask)
			{
				glPushMatrix();
				if (m_fUsAngle > s_constMinAngle && m_fUsAngle < s_constMaxAngle) 
				{
					glPushMatrix();
					// 1. 用户中心-->地面站
					Point3D ptUserCen;
					ptUserCen.x = userCenterPoint.x;
					ptUserCen.y = userCenterPoint.y-1.5;
					ptUserCen.z = userCenterPoint.z+0.2;
					DrawDasSignal(0, ptUserCen, s_ptGoundTerminalBase);


					selectFont(20, DEFAULT_CHARSET, "华文仿宋");
					glColor3f( 1.0f, 0.0f, 0.0f);
					glRasterPos3f(ptUserCen.x-2,ptUserCen.y+0.3,ptUserCen.z+3);
					drawCNString("  1:地面站通过网络接收POCC传输过来的数据");
					glPopMatrix();

					glPushMatrix();
					if (m_bTer2TDRS)
					{
						// 2. 地面站-->中继星
						DrawDasSignal(0,s_ptGoundTerminalBase,s_ptTDRSCenter);

						selectFont(20, DEFAULT_CHARSET, "华文仿宋");
						glColor3f(1.0f, 0.0f,  0.0f);
						glRasterPos3f(s_ptTDRSCenter.x+3,s_ptTDRSCenter.y-4,s_ptTDRSCenter.z-1);
						drawCNString("  2:中继星接收地面站传输过来的数据");
					}
					glPopMatrix();

					glPushMatrix();
					if (m_bTDRS2US)
					{
						//3. 中继星-->用户星
						DrawDasSignal(/*2*/0,s_ptTDRSCenter,s_ptUsrCenter);

						selectFont(20, DEFAULT_CHARSET, "华文仿宋");
						glColor3f( 1.0f, 0.0f,0.0f);
						//glRasterPos3f(s_ptTDRSCenter.x+3,s_ptTDRSCenter.y-4,s_ptTDRSCenter.z-1);
						glRasterPos3f(s_ptUsrCenter.x,s_ptUsrCenter.y+1,s_ptUsrCenter.z-1);
						drawCNString("  3:用户星接收中继星传输过来的数据");
					}
					glPopMatrix();
				}
				glPopMatrix();
			}
			// 1.2反向任务
			else
			{
				glPushMatrix();
				if (m_fUsAngle > s_constMinAngle && m_fUsAngle < s_constMaxAngle) 
				{
					//1. 用户星-->中继星
					DrawDasSignal(1,s_ptUsrCenter,s_ptTDRSCenter);

					selectFont(20, DEFAULT_CHARSET, "华文仿宋");
					glColor3f( 1.0f, 0.0f,0.0f);
					glRasterPos3f(s_ptUsrCenter.x,s_ptUsrCenter.y+1,s_ptUsrCenter.z-1);
					drawCNString("  1:数据从用户星传输给中继星");


					// 2. 中继星-->地面站
					if (m_bTDRS2Ter)
					{
						Point3D ptStart(s_ptTDRSCenter.x, s_ptTDRSCenter.y, s_ptTDRSCenter.z);
						Point3D ptEnd(s_ptGoundTerminalBase.x, s_ptGoundTerminalBase.y, s_ptGoundTerminalBase.z);
						DrawDasSignal(1, ptStart/*s_ptTDRSCenter*/, ptEnd/*s_ptGoundTerminalBase*/);

						selectFont(20, DEFAULT_CHARSET, "华文仿宋");
						glColor3f(1.0f, 0.0f, 0.0f);
						glRasterPos3f(s_ptTDRSCenter.x+1,s_ptTDRSCenter.y-4,s_ptTDRSCenter.z-1);
						drawCNString("  2:数据从中继星传输给地面站");
					}

					// 3. 地面站-->用户中心
					if (m_bTer2UsrCen)
					{
						Point3D ptUserCen;
						ptUserCen.x = userCenterPoint.x;
						ptUserCen.y = userCenterPoint.y-1.5;
						ptUserCen.z = userCenterPoint.z+0.2;
						DrawDasSignal(/*2*/1, s_ptGoundTerminalBase, ptUserCen);

						selectFont(20, DEFAULT_CHARSET, "华文仿宋");
						glColor3f( 1.0f, 0.0f, 0.0f);
						glRasterPos3f(s_ptGoundTerminalBase.x+1,s_ptGoundTerminalBase.y+0.6,s_ptGoundTerminalBase.z-1);
						drawCNString("  3:地面站通过网络将数据传输给POCC");
					}
				}
				glPopMatrix();
			}
		}
		
	}//1. 是否需要模拟信号

	glFlush();	
	glFinish(); 
 	SwapBuffers(m_pDC->GetSafeHdc()/*wglGetCurrentDC()*/);

}

void CDASRender::DrawDasAxis(Point3D originPoint)
{
	//矩阵堆栈函数，和glPopMatrix()相对应
	glPushMatrix();  
	//平移到左下方位置
	glTranslatef(originPoint.x, originPoint.y, originPoint.z);
	glScalef(0.6,0.6,0.6);
	glBegin( GL_LINES );  
	glColor3d(1.0, 0.0, 0.0);   // X轴 红色  
	glVertex3d(0.0, 0.0, 0.0);   		glVertex3d(1.0, 0.0, 0.0);	
	glColor3d(0.0, 1.0, 0.0);   // Y轴 绿色  
	glVertex3d(0.0, 0.0, 0.0);  		glVertex3d(0.0, 1.0, 0.0);  
	glColor3d(0.0, 0.0, 1.0);   // Z轴 蓝色  
	glVertex3d(0.0, 0.0, 0.0);  		glVertex3d(0.0, 0.0, 1.0);  
	glEnd();  

	glColor3f(1.0, 1.0, 1.0);  
	DrawString3f(1,0,0,_T("x"));	DrawString3f(0,1,0,_T("y"));	DrawString3f(0,0,1,_T("z"));
	glPopMatrix(); 
}

void CDASRender::DrawDasGroundTerminal(Point3D originPoint)
{

	float x = originPoint.x;
	float y = originPoint.y;
	float z = originPoint.z;
	float scale = 0.3f;

	float baseBoxWidth = 1.0f ;
	glPushMatrix();//平台==============================
	glTranslatef(x,y,z);			//平台的定位
	//glScalef(scale,scale,scale);
	glColor3f(0.7f,0.7f,0.72f);		//绿色
	auxSolidCube(0.4);				//方台(边长)
	glTranslatef(0.0f,-0.4f,0.0f);	//架的位置调整
	glColor3f(0.9f,0.9f,0.9f);		
	auxSolidCylinder(0.04, 0.4f);
	glPopMatrix();

	glPushMatrix();//雷达==============================

	glTranslatef(x,y+baseBoxWidth*0.8,z);			//雷达的定位1
	glScalef(scale,scale,scale);	
	glRotatef(/*m_RadarRotateAngle*/15.0,0.0,1.0,0.0);	//雷达旋转2
	//=======================================
	glColor3f(0.0f,1.0f,1.0f);		
	glRotatef(45, 1.0, 0.0, 0.0);		//盘的角度调整，仰30度
	//auxWireCone(1.5,0.6f);			//线园锥盘(底半径、高)
	auxSolidCone(1.6,0.65f);			//线园锥盘(底半径、高)
	//=======================================
	glRotatef(180, 1.0, 0.0, 0.0);	//杆的角度调整,反方向转
	glTranslatef(0.0f,0.0f,-0.7f);  //杆的位置调整,缩进一点
	auxSolidCone(0.15f,2.0f);			//园锥杆(底半径、高)
	glColor3f(FRAND,0,0);			//随机红色
	//glColor3f(1,0,0);				//随机红色
	glTranslatef(0.0f,0.0f,2.0f);	//杆的位置调整,缩进一点

	//获得当前变化矩阵
	glGetFloatv(GL_MODELVIEW_MATRIX , g_arrModelview_tmp); 

	auxSolidSphere(0.15f);			//园(半径)
	glPopMatrix();
}

void CDASRender::DrawDasUserCenter(Point3D centerPoint)
{
	glPushMatrix();
	glColor3f(0.4,0.4,0.4);
	glTranslatef(0.0f,-2.0f,-5.0f);
	//glScalef(0.00001,0.00001,0.00001);
	//glScalef(0.003,0.003,0.003);
	glScalef(0.05,0.05,0.05);
	glRotatef(90,1.0,0.0,0.0);
	glRotatef(180,0,1,0);

 	if (m_bIsPOCCLoaded)
 	{
 		m_listPOCC.drawGL();
 	}
	//m_3dsPOCC.Draw();
	glPopMatrix();
}

void CDASRender::DrawDasUserCenter22(Point3D centerPoint)
{
	glPushMatrix();
	glColor3f(0.4,0.4,0.4);
	glTranslatef(2.3f,-2.2f,-0.2f);
	glScalef(0.002,0.002,0.002);
	glRotatef(90,1.0,0.0,0.0);
	glRotatef(180,0,1,0);
	glRotatef(-13, 0,1,0);

	if (m_bIsPocc22Loaded)
	{
		m_listPocc22.drawGL();
	}
	
	glPopMatrix();
}

void CDASRender::DrawNCC(Point3D originalPoint)
{
	glPushMatrix();
	glTranslatef(originalPoint.x,originalPoint.y-0.4,originalPoint.z);	
	glColor3f(0.4f,0.4f,0.0f);	
	//auxSolidCube(0.4);
	glScalef(0.0002,0.0002,0.0002);
	m_3dsBuilding_1.Draw();
	glPopMatrix();
}

void CDASRender::DrawFDF(Point3D originalPoint)
{
	glPushMatrix();
	glTranslatef(originalPoint.x,originalPoint.y+0.3,originalPoint.z+0.4);
	glColor3f(0.4f,0.0f,0.42f);	
	glScalef(0.002,0.002,0.002);
	glRotatef(180, 1.0, 0.0, 0.0);
	m_3dsBuilding_3.Draw();
	//auxSolidCube(0.4);
	glPopMatrix();
}

void CDASRender::DrawDasTdrs(Point3D centerPoint)
{
	glPushMatrix();		
	glScalef(m_fScale,m_fScale,m_fScale);

	glTranslatef(centerPoint.x-10,centerPoint.y+9,centerPoint.z);
	glScalef(0.80,0.80,0.80);
	glRotatef(140,1,0,0);
	m_3dsTDRS.Draw();
	glPopMatrix();
}

void CDASRender::DrawDasEarth(Point3D centerPoint)
{
	glEnable(GL_TEXTURE_2D); 
	glPushMatrix();
	glTranslatef(centerPoint.x, centerPoint.y, centerPoint.z);
	//glRotatef(20,0,0,1);
	glRotatef(70,0,1,0);
	glRotatef(80,1,0,0);

	glBindTexture(GL_TEXTURE_2D, m_arrTextureID[0]);
	GLUquadricObj*  q = gluNewQuadric();
	gluQuadricDrawStyle(q, GLU_FILL);
	gluQuadricNormals (q, GLU_SMOOTH);
	gluQuadricTexture (q, true);
	gluSphere(q, m_fEarthRadius, 60, 60);
	gluDeleteQuadric(q);

	//	DrawDasUserSatellite(centerPoint);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void CDASRender::DrawDasUserSatellite(Point3D s_ptEarthCenter)
{
	float x = s_ptEarthCenter.x + m_fUsRadius * cos(m_fUsAngle);
	float y = s_ptEarthCenter.y + m_fUsRadius * sin(m_fUsAngle);
	float z = 0.0f;
	//Point3D s_ptUsrCenter(x,y,z);
	s_ptUsrCenter = Point3D(x,y,z);
	glPushMatrix();
	glTranslatef(x,y,z);
	//Old-tdrs模型
	glColor3f(0.5f,0.4f,0.7f);
	glRotatef(30,0,1,0);
	glRotatef(90,1,0,0);
	glScalef(0.003,0.003,0.003);
	//glScalef(0.2, 0.2, 0.2);
	m_3dsUS.Draw();

	glPopMatrix();
}

void CDASRender::DrawDasSignal(int direction, Point3D start, Point3D end) 
{
	float signalLength = 0.1f;

	float deltX = end.x - start.x;
	float deltY = end.y - start.y;
	float deltZ = end.z - start.z;

	float x1 = start.x + deltX * m_fSignalStep;
	float y1 = start.y + deltY * m_fSignalStep;
	float z1 = start.z + deltZ * m_fSignalStep;

	float x2 = x1 + deltX * signalLength ;
	float y2 = y1 + deltY * signalLength ;
	float z2 = z1 + deltZ * signalLength ;

	//TRACE("Len:%f",deltX * signalLength);

	glLineWidth(6);
	glEnable(GL_LINES) ;
	glLineStipple (1, 0x0F0F); 
	glColor3fv(g_constSignalColor[direction]);

	glPushMatrix();
	glBegin(GL_LINES);
	glVertex3f(x1,y1,z1);
	glVertex3f(x2,y2,z2);
	//glVertex3f(GTPoint2[0],GTPoint2[1],GTPoint2[2]);
	glEnd();
	glPopMatrix();
	glDisable(GL_LINES) ;
	glLineWidth(1);
}

void CDASRender::DrawDasRotateCircle(COLORREF color, int lineType) 
{
	float radius		= 0.5f;
	//float arcLength		= PI/0.50f;
	float currentArc	= s_fStartArc;
	float deltArc		= 0.05;
	glColor3f(1.0,0.0,0.0);
	Point3D currentPoint ;
	glLineWidth(2.0);
	glBegin(GL_LINE_STRIP);
	do{
		currentPoint.x = radius * cos(currentArc);
		currentPoint.y = radius * sin(currentArc);
		glVertex3f(currentPoint.x, currentPoint.y,0);
		//glVertex3f(radius * cos(currentArc),radius * sin(currentArc),0);
		currentArc += deltArc;
	}while(currentArc<1.9*PI + s_fStartArc);
	glEnd();
	glLineWidth(1.0);

	glPointSize(8.0);
	glBegin(GL_POINTS);
	glVertex3f(currentPoint.x,currentPoint.y,0);
	glEnd();
	glPointSize(1.0);

	/*
	float arrowLength = radius / 2.0f;
	float arrowEndX = currentPoint.x + cos(PI/2.0 - (currentArc-deltArc)-PI/6.0f);
	float arrowEndY = currentPoint.y + sin(PI/2.0 - (currentArc-deltArc)-PI/6.0f);

	glBegin(GL_LINES);
	glVertex3f(currentPoint.x,currentPoint.y,0);
	glVertex3f(arrowEndX,arrowEndY,0);
	glEnd();
	*/
}

void CDASRender::DrawString3f(float x,float y,float z,CString content)
{
	glRasterPos3f(x, y, z);			 
	for(int j = 0; j < content.GetLength(); j++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, content[j]);
	}
}

void CDASRender::selectFont(int size, int charset, const char* face) 
{
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_BOLD, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(/*wglGetCurrentDC()*/m_pDC->GetSafeHdc(), hFont);
	DeleteObject(hOldFont);
}
void CDASRender::drawCNString(float x, float y, float z, const char* str)
{
	selectFont(16, GB2312_CHARSET, "楷体_GB2312");

	glRasterPos3f(x, y, z);	

	int len, i;
	wchar_t* wstring;
	HDC hDC = m_pDC->GetSafeHdc();//wglGetCurrentDC(); //获取显示设备
	GLuint list = glGenLists(1); //申请1个显示列表
	
	//计算字符的个数
	//如果是双字节字符的（比如中文字符），两个字节才算一个字符
	//否则一个字节算一个字符
	len = 0;
	for(i=0; str[i]!='\0'; ++i)
	{
		if( IsDBCSLeadByte(str[i]) )
			++i;
		++len;
	}
	// 将混合字符转化为宽字符
	int nLen = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, NULL, 0);
	wstring = (wchar_t*)malloc((len+1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';// 只是转义符,它本身的类型是wchar_t

	// 逐个输出字符
	for(i=0; i<len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}
	// 回收所有临时资源
	free(wstring);
	glDeleteLists(list, 1);
} 

std::wstring CDASRender::ANSIToUnicode(const char* str , CString& strWide  )
{
	int  unicodeLen = ::MultiByteToWideChar( CP_ACP,0,str,-1,NULL,0 ); 
	wchar_t *  pUnicode; 
	pUnicode = new  wchar_t[unicodeLen+1]; 
	memset(pUnicode,0,(unicodeLen+1)*2); 
	::MultiByteToWideChar( CP_ACP, 0, str, -1,(LPWSTR)pUnicode, unicodeLen ); 
	wstring  rt; 
	pUnicode[unicodeLen] = '\0';
	strWide = pUnicode;
	rt = ( wchar_t* )pUnicode;
	delete  pUnicode;

	return  rt; 
}

//-------------------------------------------------------------------------------------
//Description:
// This function maps a character string to a wide-character (Unicode) string
//
//Parameters:
// lpcszStr: [in] Pointer to the character string to be converted 
// lpwszStr: [out] Pointer to a buffer that receives the translated string. 
// dwSize: [in] Size of the buffer
//
//Return Values:
// TRUE: Succeed
// FALSE: Failed
// 
//Example:
// MByteToWChar(szA,szW,sizeof(szW)/sizeof(szW[0]));
//---------------------------------------------------------------------------------------
BOOL CDASRender::MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize)
{
	// Get the required size of the buffer that receives the Unicode // string. 
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, NULL, 0);
	if(dwSize < dwMinSize)
		return FALSE;

	// Convert headers from ASCII to Unicode.
	MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, lpwszStr, dwMinSize);  
	return TRUE;
}

void CDASRender::drawCNString(const char* str) 
{
	int len, i;
	wchar_t* wstring;
	HDC hDC = m_pDC->GetSafeHdc();//wglGetCurrentDC();
	GLuint list = glGenLists(1);

	/*CString asName(str);
	WCHAR* wszName = new WCHAR[asName.GetLength()+1];
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)asName.GetBuffer(0), -1, wszName, asName.GetLength()+1);
	std::wstring wstr(wszName);
	delete []wszName;*/

	CString strWide;
 	std::wstring wstr = ANSIToUnicode(str, strWide);
 	len = strWide.GetLength();//wstr.length();
	 //逐个输出字符
	for(i=0; i<=len; ++i)
	{
		wchar_t wch = strWide.GetAt(i);//wstr[i];
		DWORD dwFirst = (DWORD)wch;
		wglUseFontBitmapsW(hDC, wch/*wstr[i]*/, 2, list);
		glCallList(list);
	}
	glDeleteLists(list, 1);

	

	//// 计算字符的个数
	//// 如果是双字节字符的（比如中文字符），两个字节才算一个字符
	//// 否则一个字节算一个字符
	//len = 0;
	//for(i=0; str[i]!='\0'; ++i)
	//{
	//	if( IsDBCSLeadByte(str[i]) )
	//		++i;
	//	++len;
	//}

	//// 将混合字符转化为宽字符
	//int nLen = ::MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	//wstring = new wchar_t[nLen+1];
	//memset(wstring, 0, (nLen+1)*sizeof(wchar_t));

	////wstring = (wchar_t*)malloc((len+1) * sizeof(wchar_t));
	//::MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)wstring, len);
	//wstring[len] = L'\0';

	//// 逐个输出字符
	//for(i=0; i<len; ++i)
	//{
	//	wglUseFontBitmapsW(hDC, wstring[i], 1, list);
	//	glCallList(list);
	//}

	//// 回收所有临时资源
	//free(wstring);
	//glDeleteLists(list, 1);
}

void CDASRender::CalculateUsAngleStep(CTime& timeBegin, CTime& timeEnd, DWORD dwEllapsTime)
{
	//默认DrawSecen定时器是300毫秒一次，且可见的区域的角度为[1.0--2.1]
	CTimeSpan span = timeEnd - timeBegin;
	UINT nTotalMicroSeconds = (span.GetTotalSeconds())*1000;
	
	//计算新的步长。
	LOG_NOTICE(_T("Angle & Step : Before Changed...m_fUsAngle= %f, m_fUsAngleStep"), m_fUsAngle, m_fUsAngleStep);
	m_fUsAngleStep = (2.1-1.0)/(nTotalMicroSeconds/300);

	//移动卫星一定的步长，因为已经有逝去的时间。
	m_fUsAngle = s_constMinAngle + ((dwEllapsTime*1000)/300)*m_fUsAngleStep;
	LOG_NOTICE(_T("Angle & Step : After  Changed...m_fUsAngle= %f, m_fUsAngleStep"), m_fUsAngle, m_fUsAngleStep);
}


void CDASRender::ReCalculateAngle()
{
	m_RadarRotateAngle += 1.0;
	
	//LOG_NOTICE(_T("m_fUsAngle Before Changed...m_fUsAngle= %f"), m_fUsAngle);
	m_fUsAngle += m_fUsAngleStep*m_nTimeFrequency;//0.15:角度
	//LOG_NOTICE(_T("m_fUsAngle After  Changed...m_fUsAngle= %f"), m_fUsAngle);
	
	if (m_fSignalStep >= 0.9f) 
	{
		m_fSignalStep = 0.0f;
	}else
	{
		m_fSignalStep += 0.05f;
	}
}

void CDASRender::ReFreshTime()
{
	s_fStartArc += 0.75f;

	m_sysCurrentTime = CTime::GetCurrentTime();
	CString strCurret = MyTools::GetFormatedDateTime(m_sysCurrentTime);
	CString strStart  = MyTools::GetFormatedDateTime(m_sysStartTime);
	CString strBase   = MyTools::GetFormatedDateTime(m_baseTime);

	CTimeSpan span = m_sysCurrentTime - m_sysStartTime;
	CSingleLock lock(&m_csShowtime);
	m_ctimeShown = m_baseTime.GetTime() + span.GetTotalSeconds()*m_nTimeFrequency;

}

void CDASRender::SetSysTimeFrequency(int nTimes)
{	
	if (nTimes>0)
	{
		m_nTimeFrequency = nTimes;
	}

	CTime timeCurr = GetShowTime();
	CDASRender::Instance()->SetNewTimeInView(CTime::GetCurrentTime(), timeCurr);

	
}
CTime CDASRender::GetShowTime()
{
	CSingleLock lock(&m_csShowtime);
	return m_ctimeShown;
}

void CDASRender::ScaleView(UINT nChar)
{
	if (nChar == VK_UP) 
	{
		LOG_NOTICE(_T("VK_UP Before...m_fScale=%f"), m_fScale);
		if(m_fScale > 0.0 && m_fScale <= 0.9)
		{
			m_fScale += 0.1;
		}
		LOG_NOTICE(_T("VK_UP After...m_fScale=%f"), m_fScale);
	}
	else if (nChar == VK_DOWN) 
	{
		LOG_NOTICE(_T("VK_DOWN Before...m_fScale=%f"), m_fScale);
		if(m_fScale > 0.0 && m_fScale >=0.7)
		{
			m_fScale -= 0.1;
		}
		LOG_NOTICE(_T("VK_DOWN After...m_fScale=%f"), m_fScale);
	}
}


void CDASRender::SetNewTimeInView(CTime& tStartTime, CTime& tBaseTime)
{
	//for test
	CString strStart = MyTools::GetFormatedDateTime(tStartTime);
	CString strBase  = MyTools::GetFormatedDateTime(tBaseTime);

	m_sysStartTime = tStartTime;
	m_baseTime	   = tBaseTime;
}

void CDASRender::SetPhase12Flag(EPhaseDrawType eType)
{
	m_ePhaseType = eType;
}

// void CDASRender::SetCountDownFlag(BOOL bCountDown)
// {
// 	m_bCountDown = bCountDown;
// }

void CDASRender::SetSimulateFlag(BOOL bSimulate)
{
	m_bSimulateTask = bSimulate;
}
void CDASRender::SetMAFFlag(BOOL bMAF)
{
	m_bMAFTask = bMAF;
}
void CDASRender::SetMAFMARFlag(BOOL bMAFMAR)
{
	m_bMAFMARTask = bMAFMAR;
}

//maf
void CDASRender::SetDrawTDRS2US(BOOL bFlag)
{
	m_bTDRS2US = bFlag;
}
void CDASRender::SetDrawTer2TDRS(BOOL bFlag)
{
	m_bTer2TDRS = bFlag;
}
//mar
void CDASRender::SetDrawTDRS2Ter(BOOL bFlag)
{
	m_bTDRS2Ter = bFlag;
}
void CDASRender::SetDrawTer2UsrCen(BOOL bFlag)
{
	m_bTer2UsrCen = bFlag;
}

void CDASRender::ReSetStatus()
{

	m_bSimulateTask = FALSE;
	m_bMAFTask = TRUE;
	m_bMAFMARTask = FALSE;
	//maf
	m_bTer2TDRS = FALSE;
	m_bTDRS2US = FALSE;
	//mar
	m_bTDRS2Ter = FALSE;
	m_bTer2UsrCen = FALSE;

	m_ePhaseType = enmPhase_None;

}