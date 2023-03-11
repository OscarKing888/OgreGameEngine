// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// CinemaEditorDoc.cpp : implementation of the CCinemaEditorDoc class
//

#include "stdafx.h"
#include "CinemaEditor.h"

#include "CinemaEditorDoc.h"


// CCinemaEditorDoc

IMPLEMENT_DYNCREATE(CinemaEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CinemaEditorDoc, CDocument)
END_MESSAGE_MAP()


// CCinemaEditorDoc construction/destruction

CinemaEditorDoc::CinemaEditorDoc()
{
	// TODO: add one-time construction code here

}

CinemaEditorDoc::~CinemaEditorDoc()
{
}

BOOL CinemaEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CCinemaEditorDoc serialization

void CinemaEditorDoc::Serialize(CArchive& ar)
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


// CCinemaEditorDoc diagnostics

#ifdef _DEBUG
void CinemaEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CinemaEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCinemaEditorDoc commands
