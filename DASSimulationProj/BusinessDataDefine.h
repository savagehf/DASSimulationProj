#pragma once

#include <list>

using namespace std;

enum ETaskLevel
{
	enmTaskLevel_Low  = 1,
	enmTaskLevel_Mid  = 2,
	enmTaskLevel_High = 3,
};

enum ETaskStatus
{
	enmTaskStatus_UnProcess = 1,
	enmTaskStatus_Prepare   = 2,
	enmTaskStatus_Prcessing = 3,
	enmTaskStatus_Processed = 4,
};

enum ETaskType
{
	enmTaskType_MAF = 1,
	enmTaskType_MAR = 2,
	enmTaskType_MAFMAR = 3,
};



enum EServiceType
{
	enmSrvType_None	=	-1,
	enmSrvType_MAF	= 0,
	enmSrvType_MAR	= 1,
	enmSrvType_MAFMAR,		//双向任务
	enmSrvType_UnexpectMAR,	//突发返向任务
};

typedef struct stTaskItem
{
public:
	CString strTDRS;
	CString strSIC;
	CString strSSC;
	CString strStartTime;
	CString strEndTime;
	CString strLastTime;
	ETaskLevel    uLevel;
	ETaskStatus	uStatus ;
	ETaskType  eTaskType;
	UINT    nChannelID;
	stTaskItem()
	{
		strTDRS = _T("");
		strSIC = _T("");
		strSSC = _T("");
		strStartTime = _T("");
		strEndTime = _T("");
		strLastTime = _T("");
		uLevel = enmTaskLevel_Low;
		uStatus = enmTaskStatus_UnProcess;
		nChannelID = 0;
	}

}TaskItem;
typedef list<TaskItem> ListTaskTable;

// typedef struct stMARTask
// {
// public:
// 	CString strTDRS;
// 	CString strSIC;
// 	CString strStartTime;
// 	CString strEndTime;
// 	CString strLastTime;
// 	UINT nChannelID;
// 	ETaskLevel eLevel;
// }MARTask;
// typedef list<MARTask> ListMARTaskTable;

typedef struct stTimeItem
{
public:
	CString strTDRS;		//
	CString strSIC;			//stk.csv中up可见时间窗，需要sic字段，目前模拟阶段只设置1个sic=001
	CString strStartTime;
	CString strEndTime;
	CString strLastTime;
	BOOL	bPlayed;
	BOOL	bOuccupied;
	BOOL	bUnexp;
	int     nLevel;
	EServiceType eType;
	stTimeItem()
	{
		strTDRS = _T("1");
		strSIC  = _T("001");	
		strStartTime = _T("");
		strEndTime = _T("");
		strLastTime = _T("");
		bPlayed = FALSE;
		bOuccupied = FALSE;
		bUnexp = FALSE;
		nLevel = 1;
		eType = enmSrvType_None;
	}

} TimeItem;

typedef list<TimeItem> ListTimeTable;


#define ALL_TASK_FILE	_T("Database\\task.txt")
#define TUT_FILE_PATH _T("Database\\tut.txt")
#define UP_VIS_TIME_FILEPATH  _T("Database\\stk_up_timetable.csv")
#define MAR_USABLE_CHANNEL_RES _T("Database\\MarUsableChannels.txt")