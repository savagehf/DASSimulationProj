#pragma once

#include <stdarg.h>

class MyTools  
{
public:
	MyTools();
	virtual ~MyTools();
	
	//static method
	//��õ�ǰʱ��ĸ�ʽ���ַ���
	static CString GetFormatedDateTime();
	//��ø���ʱ��ĸ�ʽ���ַ���
	static CString GetFormatedDateTime(CTime time);
	//��õ�ǰʱ��ĸ�ʽ���ַ���
	static CString GetFormatedDate();
	//��ø���ʱ��ĸ�ʽ���ַ���
	static CString GetFormatedDate(CTime time);
	//��õ�ǰʱ��ĸ�ʽ���ַ���
	static CString GetFormatedTime();
	//��ø���ʱ��ĸ�ʽ���ַ���
	static CString GetFormatedTime(CTime time);
	//
	static void GetYMDFromDateStr(int &y,int &m,int d, CString dateStr);

	static CTime GetCTimeFromStr(CString str);
	//
	static void AlertDecimal(int num, ...);

	static void AlertFloat(int num, ...);

	static CTime GetTimeFromString(CString timerStr);

	static CTime GetCTimeFromStringEx(CString strTime);

	static CString GetCurrentPath();

	static CString GetFormatSpanTime(CTimeSpan span);

	static UINT GetMonthDays(UINT nYear, UINT nMonth);

	//"D:\..\Exec\"
	static BOOL GetCurrentAppPath(CString& strAppPath);

};

//#endif // !defined(AFX_MYTOOLS_H__CC05A289_038E_402D_9EB2_B341EDA4D744__INCLUDED_)
