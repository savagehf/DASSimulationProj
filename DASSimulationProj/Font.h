//========================================================
/**
*  @file      Font.h
*
*  ��Ŀ������ OpenGL��ʾ����
*  �ļ�����:  ������  
*  ����ƽ̨�� Windows98/2000/NT/XP
*  
*  ���ߣ�     WWBOSS
*  �����ʼ�:  wwboss123@gmail.com
*  �������ڣ� 2006-09-13	
*  �޸����ڣ� 2006-11-24
*
*/     
//========================================================
#ifndef	__FONT_H__
#define	__FONT_H__

#pragma warning(disable: 4267)  /**< ��ֹ��������������ת���ľ��� */
#pragma warning(disable: 4244)

/** ���������� */
class GLFont															
{
public:
    
	/** ���캯������������ */
	GLFont();
	~GLFont();
     
	/** ��ʼ������ */
	bool InitFont(); 
	
	/** ��(x,y)�����string���� */
	void PrintText(TCHAR *string, float x, float y, float z); 
	
protected:
	HFONT m_hFont;     /**< ������ */
		
};

#endif	// __GLFONT_H__