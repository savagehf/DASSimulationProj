#pragma once

#include "BusinessDataDefine.h"
#include <map>
using namespace std;

class CMainFrame;

class CBussinessData
{
public:
	static CBussinessData* Instance();
	static void DestroyInstance();
	~CBussinessData();
	
	void SetMainFrm(CMainFrame* pMainFrm);
	BOOL InitBussinessData();
	BOOL InitAllTasks(CString strFile = ALL_TASK_FILE);
	BOOL InitTUT(CString strFile = TUT_FILE_PATH);
	BOOL ImportUSAndTDRSVisibleTimeTable(CString strFile = UP_VIS_TIME_FILEPATH);
	BOOL CalculateUsableTimeTable();

	BOOL SimulateTUT(CTime	timeStartDate, CTime timeEndDate);
	void ReWriteTaskCache2File();
	void ReWriteTUTCache2File();
	void AddDASTaskItem(TaskItem& oneItem);
	void DeleteTaskItem(/*CString& strSSC, */CString& strStart, CString& strEnd);
	void AddOcupiedTime(TimeItem& item);
	void DeleteOcupiedTimeTable(EServiceType etype,CString& strOldStart, CString& strOldEnd);
	void UpdateUsableTime(CString& strItemStart, CString& strItemEnd, CString& strNewStartTime, CString& strNewEndTime);
	void UpdateVisbleTime(CString& strItemStart, CString& strItemEnd, CString& strNewStartTime, CString& strNewEndTime);
	void UpdateTUT(CString& strItemStart, CString& strItemEnd, CString& strNewStartTime, CString& strNewEndTime);
	void UpdateTimeAfterOcupiedTask(EServiceType eType, CString& strOldStart,CString& strOldEnd,TimeItem& newTime);
	//void DeleteOneUsableItemFromCache(ETaskType eType,CString strStartTime, CString strEndTime);
	void SimulateVisableTime(CTime& timeBeing, CTime& timeEnd, map<CString, CString>& mapVisTimeTable);
	BYTE CalVisualTime(float tmp1, float tmp2);
protected:
	CBussinessData();
	void ChangeTimeFormat(CString& strTime);
	void StopCheckThread();

protected:
	void CheckTaskStatus();
	void StartCheckTaskThread();
	static UINT CheckTaskFunc(LPVOID lpPara);
	CWinThread* m_pLoopTaskThread;
	CEvent	m_EventStopCheckTask;

protected:
	CMainFrame* m_pMainFrm;

public:
	ListTimeTable	 m_listTUT;
	ListTimeTable	 m_listVisbleTime;	
	ListTimeTable	 m_listUsableTime;
	ListTimeTable    m_listOcupiedTime;
	ListTaskTable	 m_listAllTasks;
	
	CRITICAL_SECTION m_csVisableTime;
	CRITICAL_SECTION m_csAllTasks;
	CRITICAL_SECTION m_csOcupiedTime;
private:
	static CBussinessData* m_pInstance;
	
};