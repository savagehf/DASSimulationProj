
// DASSimulationDoc.cpp : implementation of the CDASSimulationDoc class
//

#include "stdafx.h"
#include "DASSimulation.h"

#include "DASSimulationDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDASSimulationDoc

IMPLEMENT_DYNCREATE(CDASSimulationDoc, CDocument)

BEGIN_MESSAGE_MAP(CDASSimulationDoc, CDocument)
END_MESSAGE_MAP()


// CDASSimulationDoc construction/destruction

CDASSimulationDoc::CDASSimulationDoc()
{
	// TODO: add one-time construction code here

}

CDASSimulationDoc::~CDASSimulationDoc()
{
}

BOOL CDASSimulationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CDASSimulationDoc serialization

void CDASSimulationDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CDASSimulationDoc diagnostics

#ifdef _DEBUG
void CDASSimulationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDASSimulationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDASSimulationDoc commands
