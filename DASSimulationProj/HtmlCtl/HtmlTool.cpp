// MyFun.cpp: implementation of the MyFun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HtmlTool.h"

#include "urlmon.h"
typedef HRESULT STDAPICALLTYPE SHOWHTMLDIALOGFN
 (HWND hwndParent, IMoniker
*pmk, VARIANT *pvarArgIn, TCHAR* pchOptions,
   VARIANT *pvArgOut);

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHtmlTool::CHtmlTool()
{

}

CHtmlTool::~CHtmlTool()
{

}

////////////////////////////////////////////////////////////////
// ˵��: ����ShowHTMLDialog�Ի������ҳ
// ���ߣ�����(airen3339@163.com)
// ���ڣ�2005.12
////////////////////////////////////////////////////////////////
BOOL CHtmlTool::ShowHtml(LPWSTR url)
{
	//װ�ض�̬��
   HINSTANCE hinstMSHTML = LoadLibrary(_T("MSHTML.DLL"));
   //�˵�ַ���ƿ�ֱ����html�ļ�������
   LPWSTR lpUrl=url; 
   if(hinstMSHTML)//װ�ض�̬���ӿ�ɹ�
   {
      SHOWHTMLDIALOGFN *pfnShowHTMLDialog;
      pfnShowHTMLDialog = (SHOWHTMLDIALOGFN*) GetProcAddress(hinstMSHTML, "ShowHTMLDialog");
      if(pfnShowHTMLDialog)
	  {
         IMoniker *moniker=NULL;
         //
         if( FAILED(CreateURLMoniker( NULL, (LPWSTR)lpUrl, &moniker ) ))
		 {
              FreeLibrary(hinstMSHTML);
              return FALSE;
		 }
         //����ShowHTMLDialog������ʾURL�ϵ�HTML�ļ�
         pfnShowHTMLDialog(NULL, moniker, NULL, NULL, NULL);

         if(moniker != NULL)
         moniker->Release();

         //��ʾ�ɹ�������TRUE
         return TRUE;

	  }
      else //GetProcessAddressʧ��
         return FALSE;

      FreeLibrary(hinstMSHTML);
   }
   else //װ�ض�̬���ӿ�ʧ��
      return FALSE;
}

////////////////////////////////////////////////////////////////
// ˵��: ����Ĭ�����������ҳ
// ���ߣ�����(airen3339@163.com)
// ���ڣ�2005.12
////////////////////////////////////////////////////////////////
void CHtmlTool::GetUrl(CString sURL)
{
	HKEY hkRoot,hSubKey; //����ע�����ؼ��ּ��ӹؼ��� 
	/*char*/TCHAR ValueName[256]; 
	/*unsigned char*/TCHAR DataValue[256]; 
	unsigned long cbValueName=256; 
	unsigned long cbDataValue=256; 
	/*char*/TCHAR ShellChar[256]; //���������� 
	DWORD dwType; 
	
	//��ע�����ؼ��� 
	if(RegOpenKey(HKEY_CLASSES_ROOT,NULL,&hkRoot)==ERROR_SUCCESS) 
	{ 
		//���ӹؼ��� 
		if(RegOpenKeyEx(hkRoot, 
			_T("htmlfile\\shell\\open\\command"), 
			0, 
			KEY_ALL_ACCESS, 
			&hSubKey)==ERROR_SUCCESS) 
		{ 
			//��ȡע�����ȡĬ���������������  
			RegEnumValue(hSubKey,  
				0, 
				ValueName, 
				&cbValueName, 
				NULL, 
				&dwType, 
				(LPBYTE)DataValue, 
				&cbDataValue); 
			// ���ò�������ҳ��ַ����ֵ 
			/*strcpy*/_tcscpy(ShellChar,/*(char *)*/(TCHAR*)DataValue); 
			/*strcat*/_tcscat(ShellChar,sURL.GetBuffer(0)); 
			// ��������� 
			WinExec((LPCSTR)ShellChar,SW_SHOW); 
			
		} 
	else 
	{
	    //�ر�ע��� 
	    RegCloseKey(hSubKey); 
	    RegCloseKey(hkRoot); 
	}
	}
}
