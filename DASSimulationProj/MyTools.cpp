// MyTools.cpp: implementation of the MyTools class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyTools.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyTools::MyTools()
{

}

MyTools::~MyTools()
{

}

CString MyTools::GetFormatSpanTime(CTimeSpan span)
{
	CString str;
	//str = span.Format("%D:%H:%M:%S");
	str = span.Format(_T("%H:%M:%S"));
	return str;
}

CString MyTools::GetFormatedDateTime(){
	CTime time = CTime::GetCurrentTime();
	//time.get
	CString str = time.Format("%Y-%m-%d %X");
	return str;
}

CString MyTools::GetFormatedDateTime(CTime time){
	CString str = time.Format("%Y-%m-%d %X");
	return str;
}

CString MyTools::GetFormatedDate(){
	CTime time = CTime::GetCurrentTime();
	//time.get
	CString str = time.Format("%Y-%m-%d");
	return str;
}

CString MyTools::GetFormatedDate(CTime time){
	CString str = time.Format("%Y-%m-%d");
	return str;
}
CString MyTools::GetFormatedTime(){
	CTime time = CTime::GetCurrentTime();
	//time.get
	CString str = time.Format("%X");
	return str;
}

CString MyTools::GetFormatedTime(CTime time){
	CString str = time.Format("%X");
	return str;
}

void  MyTools::GetYMDFromDateStr(int &y,int &m,int d, CString dateStr){
	_tcscanf_s/*sscanf*/(dateStr,"%d-%d-%d",&y,&m,&d);
	
}

CTime MyTools::GetCTimeFromStr(CString str)
{
	int y,m,d,h,mi,s;
	/*sscanf*/_tcscanf_s(str,"%d-%d-%d %d:%d:%d",&y,&m,&d,&h,&mi,&s);
	return CTime(y,m,d,h,mi,s);
}

CString MyTools::GetCurrentPath(){
	CString path;
	GetModuleFileName(NULL,path.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	path.ReleaseBuffer(); 
	int pos = path.ReverseFind('\\'); 
	path = path.Left(pos); 
	/*pos = path.ReverseFind('\\'); 
	path = path.Left(pos); */
	return path;
}

//=============== ¿ìËÙMessageBoxÊä³ö======================
void MyTools::AlertDecimal(int num, ...) {
	CString content = _T("Value: ");
	va_list ap; 
	va_start ( ap, num );
	for ( int i= 0; i< num; i++ ) {
		int argValue = va_arg (ap, int);
		CString tmpStr ;
		tmpStr.Format(_T("%d"),argValue);
		content = content + tmpStr +_T(", ");
	}
	va_end (ap);
	AfxMessageBox(content);
}

void MyTools::AlertFloat(int num, ...) {
	CString content = _T("Value: ");
	va_list ap; 
	va_start ( ap, num );
	for ( int i= 0; i< num; i++ ) {
		float argValue = va_arg (ap, float);
		CString tmpStr ;
		tmpStr.Format(_T("%f"),argValue);
		content = content + tmpStr +_T(", ");
	}
	va_end (ap);
	AfxMessageBox(content);
}

CTime  MyTools::GetTimeFromString(CString timerStr){
	//CString   strCString="2011-03-24  14:45:22";     
	COleVariant   vtime(timerStr);   
	vtime.ChangeType(VT_DATE);   
	COleDateTime   oletime = vtime;   
	SYSTEMTIME   systime;   
	VariantTimeToSystemTime(oletime,   &systime);   
	//return CTime(oletime);
	CTime   time(oletime); 
	return time;
}

CTime MyTools::GetCTimeFromStringEx(CString strTime)
{
	//strExample = "2013-3-12 11:20:30"
	
	int nYIndx = strTime.Find(_T("-"), 0);
	CString strYear = strTime.Left(nYIndx);

	int nMIndx = strTime.Find(_T("-"), nYIndx+1);
	CString strMonth = strTime.Mid(nYIndx+1, nMIndx-nYIndx-1);

	int nDIndx = strTime.Find(_T(" "), nMIndx+1);
	CString strDay = strTime.Mid(nMIndx+1, nDIndx-nMIndx-1);

	int nHIndx = strTime.Find(_T(":"), nDIndx+1);
	CString strHour = strTime.Mid(nDIndx+1, nHIndx-nDIndx-1);

	int nMinIndx = strTime.Find(_T(":"), nHIndx+1);
	CString strMin = strTime.Mid(nHIndx+1, nMinIndx-nHIndx-1);

	CString strSec = strTime.Mid(nMinIndx+1, strTime.GetLength()-nMinIndx-1);

	int Y = _tcstoul(strYear.GetBuffer(), NULL, 10);
	int M = _tcstoul(strMonth.GetBuffer(), NULL, 10);
	int D = _tcstoul(strDay.GetBuffer(), NULL, 10);
	int H = _tcstoul(strHour.GetBuffer(), NULL, 10);
	int Mi= _tcstoul(strMin.GetBuffer(), NULL, 10);
	int S = _tcstoul(strSec.GetBuffer(), NULL, 10);
	
	CTime time(Y,M,D,H,Mi,S);
	return time;

}

UINT MyTools::GetMonthDays(UINT nYear, UINT nMonth)
{
	UINT nDays = 0;

	
	switch(nMonth)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		{
			nDays = 31;		
			break;
		}
		
	case 4:
	case 6:
	case 9:
	case 11:
		{
			nDays = 30;
			break;
		}
		
	case 2:
		{
			if ((nYear%4 == 0 && nYear%100 != 0) || nYear%400==0)
			{
				nDays = 29;
			}
			else
			{
				nDays = 28;
			}
		}
		break;
	default:
		break;
	}
	
	return nDays;
}

//"D:\..\Exec\"
BOOL MyTools::GetCurrentAppPath(CString& strAppPath)
{
	TCHAR szFullExeName[MAX_PATH];
	DWORD nResult = ::GetModuleFileName(NULL, szFullExeName, MAX_PATH);
	if (0 == nResult)
	{
		return FALSE;
	}

	CString strFullFileName(szFullExeName);
	UINT nIndex = strFullFileName.ReverseFind('\\');
	strAppPath = strFullFileName.Left(nIndex+1);

	return TRUE;
}



