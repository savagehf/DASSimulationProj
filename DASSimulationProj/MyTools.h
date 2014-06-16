#pragma once

#include <stdarg.h>

class MyTools  
{
public:
	MyTools();
	virtual ~MyTools();
	
	//static method
	//获得当前时间的格式化字符串
	static CString GetFormatedDateTime();
	//获得给定时间的格式化字符串
	static CString GetFormatedDateTime(CTime time);
	//获得当前时间的格式化字符串
	static CString GetFormatedDate();
	//获得给定时间的格式化字符串
	static CString GetFormatedDate(CTime time);
	//获得当前时间的格式化字符串
	static CString GetFormatedTime();
	//获得给定时间的格式化字符串
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
