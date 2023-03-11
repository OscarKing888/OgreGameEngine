
// EditorDoc.h : EditorDoc ��Ľӿ�
//


#pragma once

class EditorDoc
	: public CDocument
	, public ICommandXExecuteObserver
{
protected: // �������л�����
	EditorDoc();
	DECLARE_DYNCREATE(EditorDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

	//void LoadServerMap(const String& serverFileName);

// ʵ��
public:
	virtual ~EditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	virtual void OnCommandExecuted();

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


