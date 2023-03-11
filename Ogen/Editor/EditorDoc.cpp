
// EditorDoc.cpp : EditorDoc 类的实现
//

#include "stdafx.h"
#include "Editor.h"

#include "EditorDoc.h"
#include "EditorGameApp.h"

// EditorDoc

IMPLEMENT_DYNCREATE(EditorDoc, CDocument)

BEGIN_MESSAGE_MAP(EditorDoc, CDocument)
END_MESSAGE_MAP()

#ifndef DELETE_EXCEPTION
#	define DELETE_EXCEPTION(e) do { if(e) { e->Delete(); } } while (0)
#endif

// EditorDoc 构造/析构

EditorDoc::EditorDoc()
{
	// TODO: 在此添加一次性构造代码
	EditorApp::getSingleton().GetCommandHistory().AddCommandExecuteObserver(this);
}

EditorDoc::~EditorDoc()
{
	if(EditorApp::getSingletonPtr())
	{
		EditorApp::getSingleton().GetCommandHistory().RemoveCommandExecuteObserver(this);
	}
}

BOOL EditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	
	EditorGame::getSingleton().OnNewScene();

	return TRUE;
}

void EditorDoc::DeleteContents()
{
	if(EditorGame::getSingletonPtr())
	{
		
		EditorGame::getSingleton().OnDeleteScene();
	}
}

void EditorDoc::OnCloseDocument()
{
	if(EditorGame::getSingletonPtr())
	{
		EditorGame::getSingleton().OnCloseScene();
	}

	CDocument::OnCloseDocument();
}


// EditorDoc 序列化

void EditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// EditorDoc 诊断

#ifdef _DEBUG
void EditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void EditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// EditorDoc 命令
void EditorDoc::OnCommandExecuted()
{
	SetModifiedFlag();
}

BOOL EditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
#ifdef _DEBUG
	if (IsModified())
		TRACE(traceAppMsg, 0, "Warning: OnOpenDocument replaces an unsaved document.\n");
#endif

	ENSURE(lpszPathName);


	DeleteContents();
	//SetModifiedFlag();  // dirty during de-serialize

	TRY
	{
		CWaitCursor wait;
		EditorGame::getSingleton().OnOpenScene(lpszPathName);
		
		//String serverMap = outBase + ".server";
		//LoadServerMap(serverMap);
	}
	CATCH_ALL(e)
	{
		DeleteContents();   // remove failed contents

		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		END_TRY
		DELETE_EXCEPTION(e);

		return FALSE;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // start off with unmodified

	//PrintSceneNode("Editor.xml", GameApp::getSingleton().GetGame()->GetVisualScene()->getRootSceneNode());

	return TRUE;
}

BOOL EditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	ENSURE(lpszPathName);

	CFileException fe;

	TRY
	{
		CWaitCursor wait;

		//EditorGame::getSingleton().SaveScene(lpszPathName);		
	}
	CATCH_ALL(e)
	{
		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		}
		END_TRY
		DELETE_EXCEPTION(e);

		return FALSE;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // back to unmodified

	return TRUE;        // success
}

//void EditorDoc::LoadServerMap(const String& serverFileName)
//{
//	if(!_serverObjects.empty())
//	{
//		for(GameObjectList::iterator i = _serverObjects.begin(); i != _serverObjects.end(); ++i)
//		{
//			IGameObject* obj = *i;
//			EditorGame::getSingleton().DestroyGameObject(obj);
//		}
//	}
//
//	ClassXMLSerializer ser;
//	_serverObjects = ser.LoadFromResourceSystem(serverFileName);	
//}
