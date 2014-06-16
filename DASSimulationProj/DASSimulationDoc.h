
// DASSimulationDoc.h : interface of the CDASSimulationDoc class
//


#pragma once


class CDASSimulationDoc : public CDocument
{
protected: // create from serialization only
	CDASSimulationDoc();
	DECLARE_DYNCREATE(CDASSimulationDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CDASSimulationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


