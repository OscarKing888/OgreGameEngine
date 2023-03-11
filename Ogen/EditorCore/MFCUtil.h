#pragma once
#include "EditorCorePrerequisites.h"

_EditorCoreExport CFrameWnd* MFCCreateNewWindow( CDocTemplate* pTemplate,
							 CDocument* pDocument );

_EditorCoreExport CFrameWnd* MFCCreateNewWindow( CDocTemplate* pTemplate);

_EditorCoreExport CMFCRibbonPanel* MFCGetRibbonPanel(const CString& name, CMFCRibbonCategory* cat);