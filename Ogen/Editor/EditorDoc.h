
// EditorDoc.h : EditorDoc 类的接口
//


#pragma once

class EditorDoc
	: public CDocument
	, public ICommandXExecuteObserver
{
protected: // 仅从序列化创建
	EditorDoc();
	DECLARE_DYNCREATE(EditorDoc)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

	//void LoadServerMap(const String& serverFileName);

// 实现
public:
	virtual ~EditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	virtual void OnCommandExecuted();

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};


