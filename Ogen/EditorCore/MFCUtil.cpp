#include "stdafx.h"
#include "MFCUtil.h"

CFrameWnd* MFCCreateNewWindow( CDocTemplate* pTemplate,
							 CDocument* pDocument )
{
	ASSERT_VALID( pTemplate );
	if(! pDocument )
	{
		pDocument = pTemplate->CreateNewDocument();
	}

	// 1 - Create the OGRE_NEW frame window
	// (will in turn create the associated view)
	CFrameWnd* pFrame = pTemplate->CreateNewFrame(
		pDocument, NULL );
	if( pFrame == NULL )
	{
		// Window creation failed
		// TRACE0( "Warning: failed to create OGRE_NEW frame.\n" );
		return NULL;
	}
	ASSERT_KINDOF( CFrameWnd, pFrame );

	// 2 - Tell the frame to update itself
	// (and its child windows)
	pTemplate->InitialUpdateFrame( pFrame, pDocument );

	// 3 - Return a pointer to the OGRE_NEW frame window object
	return pFrame;
}
//--------------------------------------------------------------------------------
CFrameWnd* MFCCreateNewWindow(CDocTemplate* pTemplate)
{
	CFrameWnd* frm = dynamic_cast<CFrameWnd*>(AfxGetApp()->GetMainWnd());
	return MFCCreateNewWindow(pTemplate, frm->GetActiveDocument());
}
//--------------------------------------------------------------------------------
CMFCRibbonPanel* MFCGetRibbonPanel( const CString& name, CMFCRibbonCategory* cat )
{
	int cnt = cat->GetPanelCount();
	for (int i = 0; i < cnt; ++i)
	{
		CMFCRibbonPanel* panel = cat->GetPanel(i);
		if(panel->GetName() == name)
		{
			return panel;
		}
	}

	return 0;
}