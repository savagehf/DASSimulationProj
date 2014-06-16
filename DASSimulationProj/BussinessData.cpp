#include "stdafx.h"
#include "BussinessData.h"
#include "DASRender.h"
#include "MainFrm.h"
#include "MyTools.h"
#include <vector>
#include <map>
#include <math.h>
using namespace  std;
CBussinessData* CBussinessData::m_pInstance = NULL;

CBussinessData::CBussinessData()
: m_EventStopCheckTask(FALSE, TRUE)
{
	::InitializeCriticalSection(&m_csVisableTime);
	::InitializeCriticalSection(&m_csAllTasks);
	::InitializeCriticalSection(&m_csOcupiedTime);

	StartCheckTaskThread();
	InitBussinessData();
	m_pMainFrm = NULL;
}

void CBussinessData::SetMainFrm(CMainFrame* pMainFrm)
{
	m_pMainFrm = pMainFrm;
}

CBussinessData::~CBussinessData()
{
	StopCheckThread();

	m_listTUT.clear();
	m_listVisbleTime.clear();
	m_listAllTasks.clear();
	m_listUsableTime.clear();

	::DeleteCriticalSection(&m_csVisableTime);
	::DeleteCriticalSection(&m_csAllTasks);
	::DeleteCriticalSection(&m_csOcupiedTime);
}

void CBussinessData::StopCheckThread()
{
	if ( NULL != m_pLoopTaskThread )
	{
		m_EventStopCheckTask.SetEvent();

		DWORD dwResult;
		if (::GetExitCodeThread(m_pLoopTaskThread->m_hThread, &dwResult))
		{
			if (STILL_ACTIVE == dwResult)
			{
				if (::WaitForSingleObject(m_pLoopTaskThread->m_hThread, 2000) != WAIT_OBJECT_0)
				{
					::TerminateThread(m_pLoopTaskThread->m_hThread, 0);
				}
			}
		}
		delete m_pLoopTaskThread;
		m_pLoopTaskThread = NULL;
	}
}

CBussinessData* CBussinessData::Instance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CBussinessData();
	}

	return m_pInstance;
}

void CBussinessData::DestroyInstance()
{
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

BOOL CBussinessData::InitBussinessData()
{
	return InitAllTasks()/* && InitTUT() */
		/*&&ImportUSAndTDRSVisibleTimeTable()*/;
}

BOOL CBussinessData::InitAllTasks(CString strFile/* = ALL_TASK_FILE*/)
{
	CStdioFile file;
	if (!file.Open(strFile, CFile::modeCreate))
	{
		return FALSE;
	}

	

	CString strItem;
	BOOL bNotInEnd = file.ReadString(strItem);
	int nLength = file.GetLength();
	int nPostio = file.GetPosition();

	::EnterCriticalSection(&m_csAllTasks);
	m_listAllTasks.clear();
	while(nPostio <= nLength && bNotInEnd)
	{
		if (!strItem.IsEmpty())
		{	
			TaskItem oneTask;

			int nTDRSIndx = strItem.Find(_T(','),0);
			if (nTDRSIndx != -1)
			{
				// TDRS
				oneTask.strTDRS = strItem.Left(nTDRSIndx);
							
				// SIC(userName)
				int nSICIndx = strItem.Find(_T(','), nTDRSIndx+1);
				oneTask.strSIC = strItem.Mid(nTDRSIndx+1, nSICIndx - nTDRSIndx - 1);

				// SSC
				int nSSCIndx = strItem.Find(_T(','), nSICIndx+1);
				oneTask.strSSC = strItem.Mid(nSICIndx+1, nSSCIndx-nSICIndx-1);

				// Level
				int nLevelIndx = strItem.Find(_T(','), nSSCIndx+1);
				CString strLevel = strItem.Mid(nSSCIndx+1, nLevelIndx-nSSCIndx-1);
				oneTask.uLevel = (ETaskLevel)_tcstoul(strItem.GetBuffer(0), NULL, 10);

				// StartTime
				int nStartTimeIndx = strItem.Find(_T(','), nLevelIndx+1);
				oneTask.strStartTime = strItem.Mid(nLevelIndx+1, nStartTimeIndx-nLevelIndx-1);

				// EndTime
				int nEndTimeIndx = strItem.Find(_T(','), nStartTimeIndx+1);
				oneTask.strEndTime = strItem.Mid(nStartTimeIndx+1, nEndTimeIndx-nStartTimeIndx-1);

				// Last Time
				int nLastTimeIndx = strItem.Find(_T(','), nEndTimeIndx+1);
				oneTask.strLastTime = strItem.Mid(nEndTimeIndx+1, nLastTimeIndx-nEndTimeIndx-1);

				// status
				int nStatuIndx = strItem.Find(_T(','), nLastTimeIndx+1);
				CString strStatus = strItem.Mid(nLastTimeIndx+1, nStatuIndx-nLastTimeIndx-1);
				oneTask.uStatus = (ETaskStatus)_tcstoul(strStatus.GetBuffer(0), NULL, 10);

				// Channel
				int nChannelIndx = strItem.Find(_T(','), nStatuIndx+1);
				CString strChannel = strItem.Mid(nStatuIndx+1, nChannelIndx-nStatuIndx-1);
				oneTask.nChannelID = _tcstoul(strChannel.GetBuffer(0), NULL, 10);

				// ETaskType
				CString strTaskType = strItem.Mid(nChannelIndx+1, strItem.GetLength()-nChannelIndx-1);
				oneTask.eTaskType = (ETaskType)_tcstoul(strTaskType.GetBuffer(0), NULL, 10);	


				m_listAllTasks.push_back(oneTask);
			}

		}

		strItem.Empty();
		bNotInEnd = file.ReadString(strItem);
		nPostio = file.GetPosition();

	}

	::LeaveCriticalSection(&m_csAllTasks);

	file.Close();

	return TRUE;
}

// Read tut.txt and cache.
BOOL CBussinessData::InitTUT(CString strFile/* = TUT_FILE_PATH*/)
{
	CStdioFile file;
	if (!file.Open(strFile, CFile::modeRead))
	{
		return FALSE;
	}
	
	m_listTUT.clear();

	CString strItem;
	BOOL bNotInEnd = file.ReadString(strItem);
	int nLength = file.GetLength();
	int nPostio = file.GetPosition();

	while(nPostio <= nLength && bNotInEnd)
	{
		if (!strItem.IsEmpty())
		{	
			TimeItem oneTime;

			int nTDRSIndx = strItem.Find(_T(','),0);
			if (nTDRSIndx != -1)
			{
				// TDRS
				oneTime.strTDRS = strItem.Left(nTDRSIndx);

				// StartTime
				int nStartTimeIndx = strItem.Find(_T(','), nTDRSIndx+1);
				oneTime.strStartTime = strItem.Mid(nTDRSIndx+1, nStartTimeIndx-nTDRSIndx-1);

				// EndTime
				int nEndTimeIndx = strItem.Find(_T(','), nStartTimeIndx+1);
				oneTime.strEndTime = strItem.Mid(nStartTimeIndx+1, nEndTimeIndx-nStartTimeIndx-1);

				// Last Time
				oneTime.strLastTime = strItem.Mid(nEndTimeIndx+1, strItem.GetLength()-nEndTimeIndx-1);

				m_listTUT.push_back(oneTime);

			}
		}

		strItem.Empty();
		bNotInEnd = file.ReadString(strItem);
		nPostio = file.GetPosition();

	}

	file.Close();
	
	return TRUE;	
}

// Read visibleTime.txt and cache.
// 导入stk的数据。
BOOL CBussinessData::ImportUSAndTDRSVisibleTimeTable(CString strFile/* = UP_VIS_TIME_FILEPATH*/)
{
	CStdioFile file;
	if (!file.Open(strFile, CFile::modeRead))
	{
		return FALSE;
	}

	std::vector<CString> vecItems;

	CString strItem;
	BOOL bNotInEnd = file.ReadString(strItem);
	int nLength = file.GetLength();
	int nPostio = file.GetPosition();

	while(nPostio <= nLength && bNotInEnd)
	{
		if (!strItem.IsEmpty())
		{	
			vecItems.push_back(strItem);
		}

		strItem.Empty();
		bNotInEnd = file.ReadString(strItem);
		nPostio = file.GetPosition();

	}

	file.Close();


	::EnterCriticalSection(&m_csVisableTime);
	m_listVisbleTime.clear();
	int nSize = vecItems.size();
	for (int i=0; i<nSize; i++)
	{
		CString strItem = vecItems[i];

		if (!strItem.IsEmpty())
		{	
			TimeItem oneTime;

			int nStartIndex = strItem.Find(_T(','),0);
			if (nStartIndex != -1)
			{
				// start time
				oneTime.strStartTime = strItem.Left(nStartIndex);

				// end time
				int nEndIndex = strItem.Find(_T(','), nStartIndex+1);
				oneTime.strEndTime = strItem.Mid(nStartIndex + 1, nEndIndex - nStartIndex - 1);

				// Last Time
				oneTime.strLastTime = strItem.Mid(nEndIndex+1, strItem.GetLength()-nEndIndex-1);

				m_listVisbleTime.push_back(oneTime);

			}
		}

	}

	::LeaveCriticalSection(&m_csVisableTime);

	return TRUE;
}

//目前这个计算只对DAF有效，对于MAR没有tut,是直接导入stk m_listVisableTime的
BOOL CBussinessData::CalculateUsableTimeTable()
{
	if (m_listTUT.size() == 0 || m_listVisbleTime.size() == 0)
	{
		return FALSE;
	}

	//临时拷贝一份tut表
	ListTimeTable tutList;
	for (ListTimeTable::iterator itor = m_listTUT.begin();
		itor != m_listTUT.end(); ++itor)
	{
		//如果没有被占用就copy
		if (!itor->bOuccupied)
		{
			TimeItem oneCopy;
			oneCopy.strEndTime = itor->strEndTime;
			oneCopy.strLastTime = itor->strLastTime;
			oneCopy.strSIC = itor->strSIC;
			oneCopy.strStartTime = itor->strStartTime;
			oneCopy.strTDRS = itor->strTDRS;

			tutList.push_back(oneCopy);
		}
		
	}
	
	m_listUsableTime.clear();

	//让可见时间表 和 临时的tut表 进行取交计算
	::EnterCriticalSection(&m_csVisableTime);
	for (ListTimeTable::iterator itVT = m_listVisbleTime.begin(); itVT != m_listVisbleTime.end(); ++itVT)
	{
		//如果没有被占用
		if (!itVT->bOuccupied)
		{
			BOOL bMatched = FALSE;
			TimeItem usableItem;

			CString strVTStart = itVT->strStartTime;
			CString strVTEnd   = itVT->strEndTime;
			CTime timeVTStart  = MyTools::GetCTimeFromStringEx(strVTStart);
			CTime timeVTEnd	   = MyTools::GetCTimeFromStringEx(strVTEnd);

			for (ListTimeTable::iterator itTut = tutList.begin(); itTut != tutList.end(); ++itTut)
			{
				CString strTutStart = itTut->strStartTime;
				CString strTutEnd	= itTut->strEndTime;

				CTime timeTutStart = MyTools::GetCTimeFromStringEx(strTutStart);
				CTime timeTutEnd	= MyTools::GetCTimeFromStringEx(strTutEnd);


				if (timeVTStart <= timeTutStart)
				{
					// case 1:
					//     |<------->|    TUT
					//  |<----->|         VT
					if (timeVTEnd <= timeTutEnd && timeVTEnd > timeTutStart)
					{
						usableItem.strSIC = itTut->strSIC;
						usableItem.strTDRS = itTut->strTDRS;
						usableItem.strStartTime = MyTools::GetFormatedDateTime(timeTutStart);
						usableItem.strEndTime   = MyTools::GetFormatedDateTime(timeVTEnd);
						//计算
						CTimeSpan span = timeVTEnd-timeTutStart;
						usableItem.strLastTime  = MyTools::GetFormatSpanTime(span);

						//删除该项，下次循环不再出现
						itTut = tutList.erase(itTut);

						bMatched = TRUE;
						break;
					}
					// case 2:
					//     |<------->|
					//  |<-------------->|
					else if (timeVTEnd > timeTutEnd)
					{
						usableItem.strSIC = itTut->strSIC;
						usableItem.strTDRS = itTut->strTDRS;
						usableItem.strStartTime = MyTools::GetFormatedDateTime(timeTutStart);
						usableItem.strEndTime   = MyTools::GetFormatedDateTime(timeTutEnd);
						//计算
						CTimeSpan span = timeTutEnd-timeTutStart;
						usableItem.strLastTime  = MyTools::GetFormatSpanTime(span);

						//删除该项，下次循环不再出现
						itTut = tutList.erase(itTut);

						bMatched = TRUE;
						break;
					}
				}

				if (timeVTStart >= timeTutStart && timeVTStart < timeTutEnd)
				{
					// case 3:
					//     |<------->|
					//       |<--->|
					if (timeVTEnd <= timeTutEnd)
					{
						usableItem.strSIC = itTut->strSIC;
						usableItem.strTDRS = itTut->strTDRS;
						usableItem.strStartTime = MyTools::GetFormatedDateTime(timeVTStart);
						usableItem.strEndTime   = MyTools::GetFormatedDateTime(timeVTEnd);
						//计算
						CTimeSpan span = timeVTEnd-timeVTStart;
						usableItem.strLastTime  = MyTools::GetFormatSpanTime(span);

						//删除该项，下次循环不再出现
						itTut = tutList.erase(itTut);

						bMatched = TRUE;
						break;
					}
					// case 4:
					//     |<------->|
					//        |<------->|
					else if (timeVTEnd > timeTutEnd)
					{
						usableItem.strSIC = itTut->strSIC;
						usableItem.strTDRS = itTut->strTDRS;
						usableItem.strStartTime = MyTools::GetFormatedDateTime(timeVTStart);
						usableItem.strEndTime   = MyTools::GetFormatedDateTime(timeTutEnd);
						//计算
						CTimeSpan span = timeTutEnd-timeVTStart;
						usableItem.strLastTime  = MyTools::GetFormatSpanTime(span);

						//删除该项，下次循环不再出现
						itTut = tutList.erase(itTut);

						bMatched = TRUE;
						break;
					}
				}

			}

			//如果找到了匹配项，就添加
			if (bMatched)
			{
				m_listUsableTime.push_back(usableItem);
			}

		}

	}

	::LeaveCriticalSection(&m_csVisableTime);

	int nSize = m_listUsableTime.size();
}

void CBussinessData::AddOcupiedTime(TimeItem& item)
{
	::EnterCriticalSection(&m_csOcupiedTime);
	m_listOcupiedTime.push_back(item);
	::LeaveCriticalSection(&m_csOcupiedTime);
}

//用户申请的时间是被占用的子集，所以需要转换为时间再判断。
void CBussinessData::DeleteOcupiedTimeTable(EServiceType etype, CString& strOldStart, CString& strOldEnd)
{
	CTime timeNewStart = MyTools::GetCTimeFromStringEx(strOldStart);
	CTime timeNewEnd   = MyTools::GetCTimeFromStringEx(strOldEnd);

	::EnterCriticalSection(&m_csOcupiedTime);
	for (ListTimeTable::iterator it = m_listOcupiedTime.begin();
		it != m_listOcupiedTime.end(); ++it)
	{
		if (etype == it->eType)
		{
			CTime timeStart = MyTools::GetCTimeFromStringEx(it->strStartTime);
			CTime timeEnd   = MyTools::GetCTimeFromStringEx(it->strEndTime);
			if (timeNewEnd >= timeStart && timeNewEnd <= timeEnd)
			{
				it = m_listOcupiedTime.erase(it);
				break;
			}
		}
	}
	::LeaveCriticalSection(&m_csOcupiedTime);
}

//多余出来的时间段需要合并到Usabletime，Tut(DAF),VisbaleTime
void CBussinessData::UpdateTimeAfterOcupiedTask(EServiceType eType, CString& strOldStart,CString& strOldEnd, TimeItem& newTime)
{
	CTime timeOldStart = MyTools::GetCTimeFromStringEx(strOldStart);
	CTime timeOldEnd   = MyTools::GetCTimeFromStringEx(strOldEnd);
	CTime timeNewStart = MyTools::GetCTimeFromStringEx(newTime.strStartTime);
	CTime timeNewEnd   = MyTools::GetCTimeFromStringEx(newTime.strEndTime);

	//1.upate usable time table.
	//    |---|    new
	//|---------|
	if (timeOldStart < timeNewStart && timeOldEnd > timeNewEnd )
	{
		TimeItem item1;
		item1.strSIC = newTime.strSIC;
		item1.strTDRS = newTime.strTDRS;
		CTimeSpan span = timeNewStart - timeOldStart;
		item1.strLastTime = MyTools::GetFormatSpanTime(span);
		item1.strStartTime = MyTools::GetFormatedDateTime(timeOldStart);
		item1.strEndTime   = MyTools::GetFormatedDateTime(timeNewStart);

		m_listUsableTime.push_back(item1);

		TimeItem item2;
		item2.strSIC = newTime.strSIC;
		item2.strTDRS = newTime.strTDRS;
		CTimeSpan span2 = timeOldEnd - timeNewEnd;
		item2.strLastTime = MyTools::GetFormatSpanTime(span2);
		item2.strStartTime = MyTools::GetFormatedDateTime(timeNewEnd);
		item2.strEndTime   = MyTools::GetFormatedDateTime(timeOldEnd);

		m_listUsableTime.push_back(item2);

	}
	//   |----| new
	//|-------|
	if (timeNewEnd == timeOldEnd && timeNewStart>timeOldStart)
	{
		TimeItem item1;
		item1.strSIC = newTime.strSIC;
		item1.strTDRS = newTime.strTDRS;
		CTimeSpan span = timeNewStart - timeOldStart;
		item1.strLastTime = MyTools::GetFormatSpanTime(span);
		item1.strStartTime = MyTools::GetFormatedDateTime(timeOldStart);
		item1.strEndTime   = MyTools::GetFormatedDateTime(timeNewStart);

		m_listUsableTime.push_back(item1);
	}

	//   |----|    new
	//   |-------|
	if (timeOldStart == timeNewStart && timeOldEnd>timeNewEnd)
	{
		TimeItem item2;
		item2.strSIC = newTime.strSIC;
		item2.strTDRS = newTime.strTDRS;
		CTimeSpan span2 = timeOldEnd - timeNewEnd;
		item2.strLastTime = MyTools::GetFormatSpanTime(span2);
		item2.strStartTime = MyTools::GetFormatedDateTime(timeNewEnd);
		item2.strEndTime   = MyTools::GetFormatedDateTime(timeOldEnd);

		m_listUsableTime.push_back(item2);
	}
	//是否需要加入时间融合的操作。。。

	//2.update visbale time table
	
	::EnterCriticalSection(&m_csVisableTime);
	//    |---|    new
	//|---------|
	if (timeOldStart < timeNewStart && timeOldEnd > timeNewEnd )
	{
		TimeItem item1;
		item1.strSIC = newTime.strSIC;
		item1.strTDRS = newTime.strTDRS;
		CTimeSpan span = timeNewStart - timeOldStart;
		item1.strLastTime = MyTools::GetFormatSpanTime(span);
		item1.strStartTime = MyTools::GetFormatedDateTime(timeOldStart);
		item1.strEndTime   = MyTools::GetFormatedDateTime(timeNewStart);

		m_listVisbleTime.push_back(item1);

		TimeItem item2;
		item2.strSIC = newTime.strSIC;
		item2.strTDRS = newTime.strTDRS;
		CTimeSpan span2 = timeOldEnd - timeNewEnd;
		item2.strLastTime = MyTools::GetFormatSpanTime(span2);
		item2.strStartTime = MyTools::GetFormatedDateTime(timeNewEnd);
		item2.strEndTime   = MyTools::GetFormatedDateTime(timeOldEnd);

		m_listVisbleTime.push_back(item2);

	}
	//   |----| new
	//|-------|
	if (timeNewEnd == timeOldEnd && timeNewStart>timeOldStart)
	{
		TimeItem item1;
		item1.strSIC = newTime.strSIC;
		item1.strTDRS = newTime.strTDRS;
		CTimeSpan span = timeNewStart - timeOldStart;
		item1.strLastTime = MyTools::GetFormatSpanTime(span);
		item1.strStartTime = MyTools::GetFormatedDateTime(timeOldStart);
		item1.strEndTime   = MyTools::GetFormatedDateTime(timeNewStart);

		m_listVisbleTime.push_back(item1);
	}

	//   |----|    new
	//   |-------|
	if (timeOldStart == timeNewStart && timeOldEnd>timeNewEnd)
	{
		TimeItem item2;
		item2.strSIC = newTime.strSIC;
		item2.strTDRS = newTime.strTDRS;
		CTimeSpan span2 = timeOldEnd - timeNewEnd;
		item2.strLastTime = MyTools::GetFormatSpanTime(span2);
		item2.strStartTime = MyTools::GetFormatedDateTime(timeNewEnd);
		item2.strEndTime   = MyTools::GetFormatedDateTime(timeOldEnd);

		m_listVisbleTime.push_back(item2);
	}

	::LeaveCriticalSection(&m_csVisableTime);


	//3.udpate tut if maf & mafmar.
	if (enmSrvType_MAF == eType || enmSrvType_MAFMAR == eType)
	{
		//    |---|    new
		//|---------|
		if (timeOldStart < timeNewStart && timeOldEnd > timeNewEnd )
		{
			TimeItem item1;
			item1.strSIC = newTime.strSIC;
			item1.strTDRS = newTime.strTDRS;
			CTimeSpan span = timeNewStart - timeOldStart;
			item1.strLastTime = MyTools::GetFormatSpanTime(span);
			item1.strStartTime = MyTools::GetFormatedDateTime(timeOldStart);
			item1.strEndTime   = MyTools::GetFormatedDateTime(timeNewStart);

			m_listTUT.push_back(item1);

			TimeItem item2;
			item2.strSIC = newTime.strSIC;
			item2.strTDRS = newTime.strTDRS;
			CTimeSpan span2 = timeOldEnd - timeNewEnd;
			item2.strLastTime = MyTools::GetFormatSpanTime(span2);
			item2.strStartTime = MyTools::GetFormatedDateTime(timeNewEnd);
			item2.strEndTime   = MyTools::GetFormatedDateTime(timeOldEnd);

			m_listTUT.push_back(item2);

		}
		//   |----| new
		//|-------|
		if (timeNewEnd == timeOldEnd && timeNewStart>timeOldStart)
		{
			TimeItem item1;
			item1.strSIC = newTime.strSIC;
			item1.strTDRS = newTime.strTDRS;
			CTimeSpan span = timeNewStart - timeOldStart;
			item1.strLastTime = MyTools::GetFormatSpanTime(span);
			item1.strStartTime = MyTools::GetFormatedDateTime(timeOldStart);
			item1.strEndTime   = MyTools::GetFormatedDateTime(timeNewStart);

			m_listTUT.push_back(item1);
		}

		//   |----|    new
		//   |-------|
		if (timeOldStart == timeNewStart && timeOldEnd>timeNewEnd)
		{
			TimeItem item2;
			item2.strSIC = newTime.strSIC;
			item2.strTDRS = newTime.strTDRS;
			CTimeSpan span2 = timeOldEnd - timeNewEnd;
			item2.strLastTime = MyTools::GetFormatSpanTime(span2);
			item2.strStartTime = MyTools::GetFormatedDateTime(timeNewEnd);
			item2.strEndTime   = MyTools::GetFormatedDateTime(timeOldEnd);

			m_listTUT.push_back(item2);
		}
	}
}

//从可用时间表中删除掉已经用掉的时间段，如果有剩余时间，就需要添加进来。
void CBussinessData::UpdateUsableTime(CString& strItemStart, CString& strItemEnd, 
									  CString& strNewStartTime, CString& strNewEndTime)
{
	CString strTDRS;
	CString strSIC;

	BOOL bfound = FALSE;
	TimeItem modifiedItem;
	//先删除掉旧的被占用的时间段。
	for (ListTimeTable::iterator it = m_listUsableTime.begin();
		it != m_listUsableTime.end();)
	{
		//1.先找到修改的项
		if (strItemStart == it->strStartTime && strItemEnd == it->strEndTime)
		{
			bfound = TRUE;
			modifiedItem = *it;
			strTDRS = it->strTDRS;
			strSIC  = it->strSIC;
			it = m_listUsableTime.erase(it);
			break;
		}
		else
		{
			++it;
		}
	}
	if (!bfound)
	{
		return;
	}

	//2.如果有剩余的可用时间段，需要加入。
	CTime timeStart = MyTools::GetCTimeFromStringEx(strItemStart);
	CTime timeEnd   = MyTools::GetCTimeFromStringEx(strItemEnd);
	CTime timeNewStart = MyTools::GetCTimeFromStringEx(strNewStartTime);
	CTime timeNewEnd   = MyTools::GetCTimeFromStringEx(strNewEndTime);
	
	
	//    |---|    new
	//|---------|
	if (timeStart < timeNewStart && timeEnd > timeNewEnd )
	{
		TimeItem item1;
		item1.strSIC = strSIC;
		item1.strTDRS = strTDRS;
		CTimeSpan span = timeNewStart - timeStart;
		item1.strLastTime = MyTools::GetFormatSpanTime(span);
		item1.strStartTime = MyTools::GetFormatedDateTime(timeStart);
		item1.strEndTime   = MyTools::GetFormatedDateTime(timeNewStart);

		m_listUsableTime.push_back(item1);

		TimeItem item2;
		item2.strSIC = strSIC;
		item2.strTDRS = strTDRS;
		CTimeSpan span2 = timeEnd - timeNewEnd;
		item2.strLastTime = MyTools::GetFormatSpanTime(span2);
		item2.strStartTime = MyTools::GetFormatedDateTime(timeNewEnd);
		item2.strEndTime   = MyTools::GetFormatedDateTime(timeEnd);

		m_listUsableTime.push_back(item2);
		
	}
	//   |----| new
	//|-------|
	if (timeNewEnd == timeEnd && timeNewStart>timeStart)
	{
		TimeItem item1;
		item1.strSIC = strSIC;
		item1.strTDRS = strTDRS;
		CTimeSpan span = timeNewStart - timeStart;
		item1.strLastTime = MyTools::GetFormatSpanTime(span);
		item1.strStartTime = MyTools::GetFormatedDateTime(timeStart);
		item1.strEndTime   = MyTools::GetFormatedDateTime(timeNewStart);

		m_listUsableTime.push_back(item1);
	}

	//   |----|    new
	//   |-------|
	if (timeStart == timeNewStart && timeEnd>timeNewEnd)
	{
		TimeItem item2;
		item2.strSIC = strSIC;
		item2.strTDRS = strTDRS;
		CTimeSpan span2 = timeEnd - timeNewEnd;
		item2.strLastTime = MyTools::GetFormatSpanTime(span2);
		item2.strStartTime = MyTools::GetFormatedDateTime(timeNewEnd);
		item2.strEndTime   = MyTools::GetFormatedDateTime(timeEnd);

		m_listUsableTime.push_back(item2);
	}
	//   |-----|  new
	//   |-----|
	//说明整个时间段被占用了，没有剩余了。就不用管了。
	if (timeStart == timeNewStart && timeEnd == timeNewEnd)
	{

	}
	

	//

}

void CBussinessData::UpdateVisbleTime(CString& strItemStart, CString& strItemEnd, 
									  CString& strNewStartTime, CString& strNewEndTime)
{
	CString strTDRS;
	CString strSIC;

	BOOL bFound = FALSE;
	TimeItem modifiedItem;
	::EnterCriticalSection(&m_csVisableTime);
	for (ListTimeTable::iterator it = m_listVisbleTime.begin();
		it != m_listVisbleTime.end();)
	{
		//1.先找到修改的项
		if (strItemStart == it->strStartTime || strItemEnd == it->strEndTime)
		{
			bFound = TRUE;
			modifiedItem = *it;
			strTDRS = it->strTDRS;
			strSIC  = it->strSIC;
			it = m_listVisbleTime.erase(it);
			break;
		}
		else
		{
			++it;
		}
	}

	if (!bFound)
	{
		::LeaveCriticalSection(&m_csVisableTime);
		return;
	}

	//2.找到后在重新计算，
	CTime timeStart = MyTools::GetCTimeFromStringEx(modifiedItem.strStartTime);
	CTime timeEnd   = MyTools::GetCTimeFromStringEx(modifiedItem.strEndTime);
	CTime timeNewStart = MyTools::GetCTimeFromStringEx(strNewStartTime);
	CTime timeNewEnd   = MyTools::GetCTimeFromStringEx(strNewEndTime);


	//    |---|    new
	//|---------|
	if (timeStart < timeNewStart && timeEnd > timeNewEnd )
	{
		TimeItem item1;
		item1.strSIC = strSIC;
		item1.strTDRS = strTDRS;
		CTimeSpan span = timeNewStart - timeStart;
		item1.strLastTime = MyTools::GetFormatSpanTime(span);
		item1.strStartTime = MyTools::GetFormatedDateTime(timeStart);
		item1.strEndTime   = MyTools::GetFormatedDateTime(timeNewStart);

		m_listVisbleTime.push_back(item1);

		TimeItem item2;
		item2.strSIC = strSIC;
		item2.strTDRS = strTDRS;
		CTimeSpan span2 = timeEnd - timeNewEnd;
		item2.strLastTime = MyTools::GetFormatSpanTime(span2);
		item2.strStartTime = MyTools::GetFormatedDateTime(timeNewEnd);
		item2.strEndTime   = MyTools::GetFormatedDateTime(timeEnd);

		m_listVisbleTime.push_back(item2);

	}
	//   |----| new
	//|-------|
	if (timeNewEnd == timeEnd && timeNewStart>timeStart)
	{
		TimeItem item1;
		item1.strSIC = strSIC;
		item1.strTDRS = strTDRS;
		CTimeSpan span = timeNewStart - timeStart;
		item1.strLastTime = MyTools::GetFormatSpanTime(span);
		item1.strStartTime = MyTools::GetFormatedDateTime(timeStart);
		item1.strEndTime   = MyTools::GetFormatedDateTime(timeNewStart);

		m_listVisbleTime.push_back(item1);
	}

	//   |----|    new
	//   |-------|
	if (timeStart == timeNewStart && timeEnd>timeNewEnd)
	{
		TimeItem item2;
		item2.strSIC = strSIC;
		item2.strTDRS = strTDRS;
		CTimeSpan span2 = timeEnd - timeNewEnd;
		item2.strLastTime = MyTools::GetFormatSpanTime(span2);
		item2.strStartTime = MyTools::GetFormatedDateTime(timeNewEnd);
		item2.strEndTime   = MyTools::GetFormatedDateTime(timeEnd);

		m_listVisbleTime.push_back(item2);
	}

	::LeaveCriticalSection(&m_csVisableTime);

}
void CBussinessData::UpdateTUT(CString& strItemStart, CString& strItemEnd, 
							   CString& strNewStartTime, CString& strNewEndTime)
{
	CString strTDRS;
	CString strSIC;

	BOOL bFound = FALSE;
	TimeItem modifiedItem;
	for (ListTimeTable::iterator it = m_listTUT.begin();
		it != m_listTUT.end();)
	{
		//1.先找到修改的项
		if (strItemStart == it->strStartTime || strItemEnd == it->strEndTime)
		{
			bFound = TRUE;
			modifiedItem = *it;
			strTDRS = it->strTDRS;
			strSIC  = it->strSIC;
			it = m_listTUT.erase(it);
			break;
		}
		else
		{
			++it;
		}
	}

	if (!bFound)
	{
		return;
	}


	//2.找到后在重新计算，
	CTime timeStart = MyTools::GetCTimeFromStringEx(modifiedItem.strStartTime);
	CTime timeEnd   = MyTools::GetCTimeFromStringEx(modifiedItem.strEndTime);
	CTime timeNewStart = MyTools::GetCTimeFromStringEx(strNewStartTime);
	CTime timeNewEnd   = MyTools::GetCTimeFromStringEx(strNewEndTime);

	//    |---|    new
	//|---------|
	if (timeStart < timeNewStart && timeEnd > timeNewEnd )
	{
		TimeItem item1;
		item1.strSIC = strSIC;
		item1.strTDRS = strTDRS;
		CTimeSpan span = timeNewStart - timeStart;
		item1.strLastTime = MyTools::GetFormatSpanTime(span);
		item1.strStartTime = MyTools::GetFormatedDateTime(timeStart);
		item1.strEndTime   = MyTools::GetFormatedDateTime(timeNewStart);

		m_listTUT.push_back(item1);

		TimeItem item2;
		item2.strSIC = strSIC;
		item2.strTDRS = strTDRS;
		CTimeSpan span2 = timeEnd - timeNewEnd;
		item2.strLastTime = MyTools::GetFormatSpanTime(span2);
		item2.strStartTime = MyTools::GetFormatedDateTime(timeNewEnd);
		item2.strEndTime   = MyTools::GetFormatedDateTime(timeEnd);

		m_listTUT.push_back(item2);

	}
	//   |----| new
	//|-------|
	if (timeNewEnd == timeEnd && timeNewStart>timeStart)
	{
		TimeItem item1;
		item1.strSIC = strSIC;
		item1.strTDRS = strTDRS;
		CTimeSpan span = timeNewStart - timeStart;
		item1.strLastTime = MyTools::GetFormatSpanTime(span);
		item1.strStartTime = MyTools::GetFormatedDateTime(timeStart);
		item1.strEndTime   = MyTools::GetFormatedDateTime(timeNewStart);

		m_listTUT.push_back(item1);
	}

	//   |----|    new
	//   |-------|
	if (timeStart == timeNewStart && timeEnd>timeNewEnd)
	{
		TimeItem item2;
		item2.strSIC = strSIC;
		item2.strTDRS = strTDRS;
		CTimeSpan span2 = timeEnd - timeNewEnd;
		item2.strLastTime = MyTools::GetFormatSpanTime(span2);
		item2.strStartTime = MyTools::GetFormatedDateTime(timeNewEnd);
		item2.strEndTime   = MyTools::GetFormatedDateTime(timeEnd);

		m_listTUT.push_back(item2);
	}
}

//目前对于mar，只是从cache中删除了该项，但csv文件中没有删除
// void CBussinessData::DeleteOneUsableItemFromCache(ETaskType eType, CString strStartTime, CString strEndTime)
// {
// 	if (enmTaskType_MAF == eType)
// 	{
// 		for (ListTimeTable::iterator it = m_listUsableTime.begin();
// 			it != m_listUsableTime.end(); ++it)
// 		{
// 			if (strStartTime == it->strStartTime && strEndTime == it->strEndTime)
// 			{
// 				it = m_listUsableTime.erase(it);
// 				break;
// 			}
// 		}
// 	}
// 	else if (enmTaskType_MAR == eType)
// 	{
// 		::EnterCriticalSection(&m_csVisableTime);
// 		for (ListTimeTable::iterator it = m_listVisbleTime.begin();
// 			it != m_listVisbleTime.end(); ++it)
// 		{
// 			if (strStartTime == it->strStartTime && strEndTime == it->strEndTime)
// 			{
// 				it = m_listVisbleTime.erase(it);
// 				break;
// 			}
// 		}
// 		::LeaveCriticalSection(&m_csVisableTime);
// 	}
// 	//lee：这个时间段改是哪个容器来存储？m_listUsableTime/m_listVisbleTime?
// 	else if (enmTaskType_MAFMAR == eType)
// 	{
// 		::EnterCriticalSection(&m_csVisableTime);
// 		for (ListTimeTable::iterator it = m_listVisbleTime.begin();
// 			it != m_listVisbleTime.end(); ++it)
// 		{
// 			if (strStartTime == it->strStartTime && strEndTime == it->strEndTime)
// 			{
// 				it = m_listVisbleTime.erase(it);
// 				break;
// 			}
// 		}
// 		::LeaveCriticalSection(&m_csVisableTime);
// 	}
// 	
// }


//11 Mar 2013 8:10:20.100 ==> 2013-3-11 8:10:20.100
void CBussinessData::ChangeTimeFormat(CString& strTime)
{
	int nDayIndx = strTime.Find(_T(" "), 0);
	CString strDay = strTime.Left(nDayIndx);

	int nMonIndx = strTime.Find(_T(" "), nDayIndx+1);
	CString strMon = strTime.Mid(nDayIndx+1, nMonIndx-nDayIndx-1);
	strMon.MakeLower();
	int nMonth = 0;
	if (strMon.Find(_T("jan"), 0) != -1)
	{
		nMonth = 1;
	}
	else if (strMon.Find(_T("feb"), 0) != -1)
	{
		nMonth = 2;
	}
	else if(strMon.Find(_T("mar"), 0) != -1)
	{
		nMonth = 3;
	}
	else if (strMon.Find(_T("apr"), 0) != -1)
	{
		nMonth = 4;
	}
	else if (strMon.Find(_T("may"), 0) != -1)
	{
		nMonth = 5;
	}
	else if (strMon.Find(_T("jun"), 0) != -1)
	{
		nMonth = 6;
	}
	else if (strMon.Find(_T("jul"), 0) != -1)
	{
		nMonth = 7;
	}
	else if (strMon.Find(_T("aug"), 0) != -1)
	{
		nMonth = 8;
	}
	else if (strMon.Find(_T("sep"), 0) != -1)
	{
		nMonth = 9;
	}
	else if (strMon.Find(_T("oct"), 0) != -1)
	{
		nMonth = 10;
	}
	else if (strMon.Find(_T("nov"), 0) != -1)
	{
		nMonth = 11;
	}
	else if (strMon.Find(_T("dec"), 0) != -1)
	{
		nMonth = 12;
	}

	int nYearIndx = strTime.Find(_T(" "), nMonIndx+1);
	CString strYear = strTime.Mid(nMonIndx+1, nYearIndx-nMonIndx-1);
	
	CString strLeftTime = strTime.Mid(nYearIndx+1, strTime.GetLength()-nYearIndx-1);

	CString strNewTime;
	strNewTime.Format(_T("%s-%d-%s %s"), strYear, nMonth, strDay,strLeftTime);
	strTime = strNewTime;
}

void CBussinessData::ReWriteTaskCache2File()
{
	CStdioFile file;
	if (!file.Open(ALL_TASK_FILE, CFile::modeCreate|CFile::modeWrite))
	{
		return;
	}

	::EnterCriticalSection(&m_csAllTasks);
	for (ListTaskTable::iterator it = m_listAllTasks.begin(); 
		it != m_listAllTasks.end(); ++it)
	{
		CString strOneItem;
		strOneItem.Format(_T("%s,%s,%s,%d,%s,%s,%s,%d,%d,%d"), it->strTDRS, it->strSIC, it->strSSC,
			it->uLevel, it->strStartTime, it->strEndTime, it->strLastTime, it->uStatus, it->nChannelID,it->eTaskType);
		file.WriteString(strOneItem);
		file.WriteString(_T("\n"));
	}
	::LeaveCriticalSection(&m_csAllTasks);

	file.Close();
}

/*const*/ //float PI = 3.1415926;
/*const*/ float i0 = 14.249*PI/180;
/*const*/ float a0 = 42162.7;
/*const*/ float e0 = 0.00284;
/*const*/ float m0 = 358.1*PI/180;
/*const*/ float w0 = 275.34*PI/180;

/*const*/ float i1 = 55.5*PI/180;
/*const*/ float a1 = 7000.0;
/*const*/ float e1 = 0.0;
/*const*/ float m1 = 0.0;
/*const*/ float w1 = 0.0;

/*const*/ float t0 = 8.6562*PI/180;
/*const*/ float d0 = 41895.418;

/*const*/ float f00 = 259.264;
/*const*/ float f11 = 0.0;

/*const*/ float wf = 0.250685/60.0;
/*const*/ float wf2 = 3.428571/60.0;

//lee注，这个模拟操作巨耗时。是否有优化的空间？！
void CBussinessData::SimulateVisableTime(CTime& timeBeing, CTime& timeEnd, map<CString, CString>& mapVisTimeTable)
{
	CTimeSpan span = timeEnd - timeBeing;
	int TOTAL_SECONDES = span.GetTotalSeconds();

	map<UINT, float> f0;
	map<UINT, float> f1;

	map<UINT, BYTE> result;
	map<UINT, BYTE> resu;

	for (int i = 1; i<=TOTAL_SECONDES; i+=60)
	{
		float a0 = fmod((float)(f00+i*wf), (float)360.0)*PI/180.0;//(((f00+i*wf)%360)*PI/180.0;
		f0.insert(make_pair(i, a0));

		float a1 = fmod((float)(f11+i*wf2), (float)360.0)*PI/180.0;//((f11+i*wf2)%360)*PI/180.0;
		f1.insert(make_pair(i, a1));

		BYTE Y = CalVisualTime(a0, a1);

		//第一个元素是1,61,121...
		resu.insert(make_pair(i, Y));
	}


	int nSize = resu.size();
	for (int i = 1; i<= nSize; i++)
	{
		BYTE a = resu[1+60*(i-1)];
		BYTE b = resu[1+60*i];

		//如果相邻元素不等0.1或者1.0，给result计算60个元素值，0或则1.
		if (a != b)
		{
			BOOL bHaszero = FALSE;
			int nCount = 0;
			//如果resu[2] != resu[3]不等，ii[61,121]共60个元素。
			int ii = 0;
			for ( ii = (60*(i-1)+1); ii<(60*i + 1); ii++)
			{
				float a0 = fmod((float)(f00+ii*wf), (float)360.0)*PI/180.0;
				f0.insert(make_pair(ii, a0));

				float b0 = fmod((float)(f11+ii*wf2), (float)360.0)*PI/180.0;
				f1.insert(make_pair(ii, a1));

				BYTE Y = CalVisualTime(a0, a1);
				//如果有0，就不要人工干预
				if (Y == 0)
				{
					bHaszero = TRUE;
				}
			
				result.insert(make_pair(ii, Y));

				nCount++;
			}

			if (!bHaszero)
			{
				for (int j=60*(i-1)+1; j<(60*i + 1); j++)
				{
					result[j] = 0;
				}
			}
		}
		//如果相邻元素是相等的。即都为1.
		else if(a == 1  && b == 1)
		{
			for (int j = 1+60*(i-1); j < 1+60*i; j++)
			{
				result.insert(make_pair(j, 1));
			}
		}
	}

	//生成可见时间段
	map<CTime, CTime> mapValueTimeIndex;
	BOOL bVadidBegin = FALSE;
	BOOL bValidEnd   = FALSE;
	UINT nBeginIndex = 0;
	for (map<UINT, BYTE>::iterator it = result.begin(); it != result.end(); ++it)
	{
		UINT nSecIndex = it->first;
		BYTE nValue = it->second;
		if (!bVadidBegin && nValue == 1)
		{
			nBeginIndex = nSecIndex;
			bVadidBegin = TRUE;
		}
		if(bVadidBegin && nValue == 0)
		{
			bValidEnd = TRUE;
		}

		if (bVadidBegin && bValidEnd)
		{
			CTime beginTime = timeBeing+nBeginIndex+rand()%60;
			CTime endTime   = timeBeing+(nSecIndex-1)+rand()%60;
			CString strBegin = MyTools::GetFormatedDateTime(beginTime);
			CString strEnd   = MyTools::GetFormatedDateTime(endTime);
			mapVisTimeTable.insert(make_pair(strBegin, strEnd));
			bVadidBegin = FALSE;
			bValidEnd = FALSE;
			nBeginIndex = 0;
		}
	}


	//写入csv文件
	CStdioFile file;
	if (!file.Open(UP_VIS_TIME_FILEPATH, CFile::modeCreate|CFile::modeWrite))
	{
		return;
	}

	for (map<CString, CString>::iterator it = mapVisTimeTable.begin();
		it != mapVisTimeTable.end(); ++it)
	{
		CTime timeBegin = MyTools::GetCTimeFromStringEx(it->first);
		CTime timeEnd   = MyTools::GetCTimeFromStringEx(it->second);
		CTimeSpan span = timeEnd - timeBegin;
		CString strSpanTime = MyTools::GetFormatSpanTime(span);

		CString strOneItem;
		strOneItem.Format(_T("%s,%s,%s"), it->first, it->second, strSpanTime);
		file.WriteString(strOneItem);
		file.WriteString(_T("\n"));
	}

	file.Close();

}

BYTE CBussinessData::CalVisualTime(float tmp1, float tmp2)
{
	BYTE y = 0;

	float x0 = a0*(1-e0*e0)/(1+e0*cos(tmp1))*(cos(m0)*cos(w0+tmp1)-sin(m0)*sin(w0+tmp1)*cos(i0));
	float y0 = a0*(1-e0*e0)/(1+e0*cos(tmp1))*(sin(m0)*cos(w0+tmp1)-cos(m0)*sin(w0+tmp1)*cos(i0));
	float z0 = z0=a0*(1-e0*e0)/(1+e0*cos(tmp1))*(sin(w0+tmp1)*sin(i0));

	float x1 = a1*(1-e1*e1)/(1+e1*cos(tmp2))*(cos(m1)*cos(w1+tmp2)-sin(m1)*sin(w1+tmp2)*cos(i1));
	float y1 = a1*(1-e1*e1)/(1+e1*cos(tmp2))*(sin(m1)*cos(w1+tmp2)-cos(m1)*sin(w1+tmp2)*cos(i1));
	float z1 = a1*(1-e1*e1)/(1+e1*cos(tmp2))*(sin(w1+tmp2)*sin(i1));

	float d = sqrt((x0-x1)*(x0-x1) + (y0-y1)*(y0-y1) + (z0-z1)*(z0-z1));
	float t = acos(((x0-x1)*x0+(y0-y1)*y0+(z0-z1)*z0)/d/sqrt(x0*x0+y0*y0+z0*z0));

	float Rs=sqrt(x0*x0+y0*y0+z0*z0);
	float t0=asin(6371.004/Rs);
	float d0=sqrt(Rs*Rs-6371.004*6371.004);

	if (abs(t) - t0 > 0)
	{
		y = 1;
	}
	else
	{
		if (d < d0)
		{
			y = 1;
		}
		else 
		{
			y = 0;
		}
	}


	return y;
}


BOOL CBussinessData::SimulateTUT(CTime	timeStartDate, CTime timeEndDate)
{
	CString strStart = MyTools::GetFormatedDateTime(timeStartDate);
	CString strEnd   = MyTools::GetFormatedDateTime(timeEndDate);

	int nYearStart  = timeStartDate.GetYear();
	int nMonthStart = timeStartDate.GetMonth();
	int nDayStart   = timeStartDate.GetDay();

	int nYearEnd  = timeEndDate.GetYear();
	int nMonthEnd = timeEndDate.GetMonth();
	int nDayEnd   = timeEndDate.GetDay();

	//当然从12月--->1月，这个模拟就暂时不支持，可以再加代码。
	if (nYearStart == nYearEnd)
	{
		m_listTUT.clear();


		//同一月份的
		if(nMonthEnd == nMonthStart)
		{
			for (int day = nDayStart; day<=nDayEnd; day++)
			{
				for (int hour=0; hour<24; )
				{
					int nMinStart = rand()%10; //0--60
					int nSecStart = rand()%60; //0--60
					int nMinEnd = rand()%10;
					int nSecEnd = rand()%60;

					CTime starttime = CTime(nYearStart, nMonthStart, day, hour, nMinStart, nSecStart);
					CTime endtime = CTime(nYearStart, nMonthStart, day, hour+1, nMinEnd, nSecEnd);
					CTimeSpan span = endtime - starttime;


					CString strStart = MyTools::GetFormatedDateTime(starttime);
					CString strEnd   = MyTools::GetFormatedDateTime(endtime);
					CString strSpan  = MyTools::GetFormatSpanTime(span);

					TimeItem timeItem;
					timeItem.strStartTime = strStart;
					timeItem.strEndTime   = strEnd;
					timeItem.strLastTime  = strSpan;

					m_listTUT.push_back(timeItem);

					hour += 2;

				}
			}
		}
		//不是用一个月份
		else
		{
			//跨度有1个月
			if(nMonthStart+1 == nMonthEnd)
			{
				//1.先生成start月的tut。[nDayStart--nLastDay]
				UINT nLastDay = MyTools::GetMonthDays(nYearStart, nMonthStart);
				for (int day = nDayStart; day<=nLastDay; day++)
				{
					for (int hour=0; hour<24; )
					{
						int nMinStart = rand()%10; //0--60
						int nSecStart = rand()%60; //0--60
						int nMinEnd = rand()%10;
						int nSecEnd = rand()%60;

						CTime starttime = CTime(nYearStart, nMonthStart, day, hour, nMinStart, nSecStart);
						CTime endtime = CTime(nYearStart, nMonthStart, day, hour+1, nMinEnd, nSecEnd);
						CTimeSpan span = endtime - starttime;


						CString strStart = MyTools::GetFormatedDateTime(starttime);
						CString strEnd   = MyTools::GetFormatedDateTime(endtime);
						CString strSpan  = MyTools::GetFormatSpanTime(span);

						TimeItem timeItem;
						timeItem.strStartTime = strStart;
						timeItem.strEndTime   = strEnd;
						timeItem.strLastTime  = strSpan;

						m_listTUT.push_back(timeItem);

						hour += 2;

					}
				}

				//2.再从end月的第一天开始生成tut时间:[1---nDayEnd]
				for (int day = 1; day<=nDayEnd; day++)
				{
					for (int hour=0; hour<24; )
					{
						int nMinStart = rand()%10; //0--60
						int nSecStart = rand()%60; //0--60
						int nMinEnd = rand()%10;
						int nSecEnd = rand()%60;

						CTime starttime = CTime(nYearStart, nMonthEnd, day, hour, nMinStart, nSecStart);
						CTime endtime = CTime(nYearStart, nMonthEnd, day, hour+1, nMinEnd, nSecEnd);
						CTimeSpan span = endtime - starttime;


						CString strStart = MyTools::GetFormatedDateTime(starttime);
						CString strEnd   = MyTools::GetFormatedDateTime(endtime);
						CString strSpan  = MyTools::GetFormatSpanTime(span);

						TimeItem timeItem;
						timeItem.strStartTime = strStart;
						timeItem.strEndTime   = strEnd;
						timeItem.strLastTime  = strSpan;

						m_listTUT.push_back(timeItem);

						hour += 2;
					}
				}
			}
			else
			{
				AfxMessageBox(_T("目前只支持跨度只有一个月的TUT的时间表，请重新起始和终止时间！"));
				return FALSE;
			}
		}

	}
	else
	{
		AfxMessageBox(_T("你填写的起始和终止时间至少应该在同一年！"));
		return FALSE;
	}

	ReWriteTUTCache2File();
	return TRUE;
}

void CBussinessData::ReWriteTUTCache2File()
{
	CStdioFile file;
	if (!file.Open(TUT_FILE_PATH, CFile::modeCreate|CFile::modeWrite))
	{
		return;
	}

	for (ListTimeTable::iterator it = m_listTUT.begin(); 
		it != m_listTUT.end(); ++it)
	{
		CString strOneItem;
		strOneItem.Format(_T("%s,%s,%s,%s,%s"), it->strTDRS,it->strSIC,
			it->strStartTime, it->strEndTime, it->strLastTime);
		file.WriteString(strOneItem);
		file.WriteString(_T("\n"));
	}

	file.Close();
}

void CBussinessData::DeleteTaskItem(/*CString& strSSC, */CString& strStart, CString& strEnd)
{
	::EnterCriticalSection(&m_csAllTasks);
	for (ListTaskTable::iterator it = m_listAllTasks.begin();
		it != m_listAllTasks.end(); ++it)
	{
		/*if (strSSC == it->strSSC)
		{*/
			if (it->strStartTime == strStart || it->strEndTime == strEnd)
			{
				it = m_listAllTasks.erase(it);
				break;
			}
		//}
	}
	
	::LeaveCriticalSection(&m_csAllTasks);

	ReWriteTaskCache2File();
}

void CBussinessData::AddDASTaskItem(TaskItem& oneItem)
{
	::EnterCriticalSection(&m_csAllTasks);
	m_listAllTasks.push_back(oneItem);
	::LeaveCriticalSection(&m_csAllTasks);

	ReWriteTaskCache2File();
}

UINT CBussinessData::CheckTaskFunc(LPVOID lpPara)
{
	CBussinessData* pThis = (CBussinessData*)lpPara;
	pThis->CheckTaskStatus();
	return 0;
}
void CBussinessData::StartCheckTaskThread()
{
	m_EventStopCheckTask.ResetEvent();
	m_pLoopTaskThread = AfxBeginThread(CheckTaskFunc, this, 0, 0, CREATE_SUSPENDED,0);
	m_pLoopTaskThread->ResumeThread();
}

void CBussinessData::CheckTaskStatus()
{
	while(true)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(&m_EventStopCheckTask, 0))
		{
			break;
		}

		CTime& timeCurr = CDASRender::Instance()->GetShowTime();
		CString strCurrentTime = MyTools::GetFormatedDateTime(timeCurr);
		int nTimeFrequency = CDASRender::Instance()->GetTimeFrequency();

		::EnterCriticalSection(&m_csVisableTime);
		//1.遍历可见时间窗。如果有时间到了，就挪卫星，计算步长。
		for (ListTimeTable::iterator itime = m_listVisbleTime.begin();
			itime != m_listVisbleTime.end(); ++itime)
		{
			CTime timeBegin = MyTools::GetCTimeFromStringEx(itime->strStartTime);
			CTime timeEnd   = MyTools::GetCTimeFromStringEx(itime->strEndTime);
			
			//当期时间点刚好是在一个可见时间窗的范围内
			if ( !itime->bPlayed && timeBegin <= timeCurr && timeCurr <= timeEnd)
			{
				CTimeSpan span = timeCurr - timeBegin;
				//计算已近跑过去的时间。
				int dwEllapsetime = span.GetTotalSeconds();
				if (dwEllapsetime < 0 )
				{
					break;
				}
				
				itime->bPlayed = TRUE;//已经执行过了，下次for检测就不会反复走进来。
				TimeItem* pTime = new TimeItem;
				pTime->strEndTime = itime->strEndTime;
				pTime->strLastTime = itime->strLastTime;
				pTime->strSIC = itime->strSIC;
				pTime->strStartTime = itime->strStartTime;
				pTime->strTDRS = itime->strTDRS;
				pTime->bPlayed = itime->bPlayed;
				if ( m_pMainFrm != NULL)
				{
					m_pMainFrm ->PostMessage(WM_VISIBLE_TIME_BEGIN, (WPARAM)pTime, (LPARAM)dwEllapsetime);
				}

				break;
			}
		}
		::LeaveCriticalSection(&m_csVisableTime);

		//当任务被创建后，可见时间段就移动到OcupiedTimeTable中去了，VisableTimeTable中就没有了。所以上面的
		//WM_VISIBLE_BEGIN就不会执行了，所以就需要下面的代码中去发送WM_VISIBLE_BEGIN
		//2014年1月19日2:23:01，如果此处不执行，就会出现奇怪的现象。fuck。
		::EnterCriticalSection(&m_csOcupiedTime);
		//1.遍历可见时间窗。如果有时间到了，就挪卫星，计算步长。
		for (ListTimeTable::iterator itime = m_listOcupiedTime.begin();
			itime != m_listOcupiedTime.end(); ++itime)
		{
			CTime timeBegin = MyTools::GetCTimeFromStringEx(itime->strStartTime);
			CTime timeEnd   = MyTools::GetCTimeFromStringEx(itime->strEndTime);

			//当期时间点刚好是在一个可见时间窗的范围内
			if ( !itime->bPlayed && timeBegin <= timeCurr && timeCurr <= timeEnd)
			{
				CTimeSpan span = timeCurr - timeBegin;
				//计算已近跑过去的时间。
				int dwEllapsetime = span.GetTotalSeconds();
				if (dwEllapsetime < 0 )
				{
					break;
				}

				itime->bPlayed = TRUE;//已经执行过了，下次for检测就不会反复走进来。
				TimeItem* pTime = new TimeItem;
				pTime->strEndTime = itime->strEndTime;
				pTime->strLastTime = itime->strLastTime;
				pTime->strSIC = itime->strSIC;
				pTime->strStartTime = itime->strStartTime;
				pTime->strTDRS = itime->strTDRS;
				pTime->bPlayed = itime->bPlayed;
				if ( m_pMainFrm != NULL)
				{
					m_pMainFrm ->PostMessage(WM_VISIBLE_TIME_BEGIN, (WPARAM)pTime, (LPARAM)dwEllapsetime);
				}

				break;
			}
		}
		::LeaveCriticalSection(&m_csOcupiedTime);


		//2.对于可见时间窗，结束不需要去管了？只需要关注哪个任务结束了，就开始还原时间倍率等。
		//  遍历任务列表，如果有任务开始，就绘制信号线。
		::EnterCriticalSection(&m_csAllTasks);
		for (ListTaskTable::iterator it = m_listAllTasks.begin(); it != m_listAllTasks.end(); ++it)
		{
			TaskItem& item = *it;
			CTime timeBegin = MyTools::GetCTimeFromStringEx(item.strStartTime);
			CTime timeEnd   = MyTools::GetCTimeFromStringEx(item.strEndTime);

			//1.如果任务是没有执行的。
			if (item.uStatus == enmTaskStatus_UnProcess)
			{	
				CTimeSpan spanpre = timeBegin-timeCurr;
				int ngap = spanpre.GetTotalSeconds();
				//当前时间在任务执行前两分钟。
				if ( ngap > 0 && ngap <= TIME_BEFORE_TASK_BEGIN)
				{
					TaskItem* pTaskItem = new TaskItem;
					pTaskItem->eTaskType = item.eTaskType;
					pTaskItem->nChannelID = item.nChannelID;
					pTaskItem->strEndTime = item.strEndTime;
					pTaskItem->strLastTime = item.strLastTime;
					pTaskItem->strSIC = item.strSIC;
					pTaskItem->strSSC = item.strSSC;
					pTaskItem->strStartTime = item.strStartTime;
					pTaskItem->strTDRS = item.strTDRS;
					pTaskItem->uLevel = item.uLevel;
					pTaskItem->uStatus = item.uStatus;

					item.uStatus = enmTaskStatus_Prepare;
					//CMainFrame* pMainFrm = (CMainFrame*)(AfxGetMainWnd());
					if (m_pMainFrm != NULL)
					{
						m_pMainFrm->PostMessage(WM_TASK_PREPARE, (WPARAM)pTaskItem);
					}
					
				}
				//当前时间已经超过任务开始时间了。直接修改任务状态，不进入prepare阶段
				else if(timeCurr >= timeBegin && timeCurr<timeEnd)
				{
					item.uStatus = enmTaskStatus_Prepare;
				}
				else if (timeCurr >= timeEnd)
				{
					item.uStatus = enmTaskStatus_Processed;
				}
			}
			//当前任务是准备阶段的。
			else if(item.uStatus == enmTaskStatus_Prepare)
			{
				//当前那时间在任务开始和结束之间。进入3D传输，并更改任务状态。
				if (timeBegin <= timeCurr && timeCurr<= timeEnd)
				{
					TaskItem* pTaskItem = new TaskItem;
					pTaskItem->eTaskType = item.eTaskType;
					pTaskItem->nChannelID = item.nChannelID;
					pTaskItem->strEndTime = item.strEndTime;
					pTaskItem->strLastTime = item.strLastTime;
					pTaskItem->strSIC = item.strSIC;
					pTaskItem->strSSC = item.strSSC;
					pTaskItem->strStartTime = item.strStartTime;
					pTaskItem->strTDRS = item.strTDRS;
					pTaskItem->uLevel = item.uLevel;
					pTaskItem->uStatus = item.uStatus;

					item.uStatus = enmTaskStatus_Prcessing;

					//CMainFrame* pMainFrm = (CMainFrame*)(AfxGetMainWnd());
					if (m_pMainFrm  != NULL)
					{
						m_pMainFrm->PostMessage(WM_TASK_BEGIN, (WPARAM)pTaskItem);
					}
					
					break;
				}
			}	
			else if (enmTaskStatus_Prcessing == item.uStatus)
			{
				CTime timeEnd = MyTools::GetCTimeFromStringEx(item.strEndTime);
				if (timeEnd >= timeCurr)
				{
					//当前时间点到了任务的结束点了
					CTimeSpan span = timeEnd - timeCurr;
					if (span.GetTotalSeconds() <= 1*nTimeFrequency)
					{
						TaskItem* pTaskItem = new TaskItem;
						pTaskItem->eTaskType = item.eTaskType;
						pTaskItem->nChannelID = item.nChannelID;
						pTaskItem->strEndTime = item.strEndTime;
						pTaskItem->strLastTime = item.strLastTime;
						pTaskItem->strSIC = item.strSIC;
						pTaskItem->strSSC = item.strSSC;
						pTaskItem->strStartTime = item.strStartTime;
						pTaskItem->strTDRS = item.strTDRS;
						pTaskItem->uLevel = item.uLevel;
						pTaskItem->uStatus = item.uStatus;

						item.uStatus = enmTaskStatus_Processed;

						//CMainFrame* pMainFrm = (CMainFrame*)(AfxGetMainWnd());
						if (m_pMainFrm  != NULL)
						{
							m_pMainFrm->PostMessage(WM_TASK_END, (WPARAM)pTaskItem);
						}	

						break;
					}
				}	
			}
			
		}
		::LeaveCriticalSection(&m_csAllTasks);

		//主动sleep
		Sleep(500);
	}

}