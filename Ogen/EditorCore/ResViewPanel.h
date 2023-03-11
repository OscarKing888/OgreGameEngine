#pragma once
#include "ResViewPanelBase.h"

///
/// Base mesh resource view panel
///
class _EditorCoreExport ResPanel_MeshBase
: public ResPanel_ResGrouped
{
public:

	ResPanel_MeshBase();
	virtual ~ResPanel_MeshBase();

	virtual CString GetPanelTitle() const { return "Mesh"; }

	virtual void OnInitListItem(void);

	virtual void OnClickTreeItem(HTREEITEM item, const CString& resName);

protected:
	///
	/// Request
	///
	struct ThumbRequest
	{
		ThumbRequest()
			: owner(0)
			, itemIdx(0)
		{
		}

		ResViewPanel* owner;
		String sourceFile;
		int itemIdx;

		_EditorCoreExport friend std::ostream& operator<<(std::ostream& o, const ThumbRequest& r)
		{ (void)r; return o; }
	};

	///
	/// Response
	///
	struct ThumbResponse
	{
		ThumbResponse()
			: hbmp(0)
		{
		}

		HBITMAP hbmp;

		_EditorCoreExport friend std::ostream& operator<<(std::ostream& o, const ThumbResponse& r)
		{ (void)r; return o; }
	};

	/// WorkQueue::RequestHandler override
	bool canHandleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
	/// WorkQueue::RequestHandler override
	WorkQueue::Response* handleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
	/// WorkQueue::ResponseHandler override
	bool canHandleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);
	/// WorkQueue::ResponseHandler override
	void handleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);
};

//--------------------------------------------------------------------------------
///
/// Global ResPanel_Mesh
///
class _EditorCoreExport  ResPanel_Mesh
	: public ResPanel_MeshBase
	, public Singleton<ResPanel_Mesh>
{
public:

	virtual void OnDBClickListItem(int item, const CString& resName);
};

//------------------------------------------------------------------------
///
/// Base texture resource view panel
///
class _EditorCoreExport  ResPanel_TextureBase
	: public ResPanel_ResGrouped
{
public:
	
	virtual CString GetPanelTitle() const { return "Texture"; }

	virtual void OnInitListItem(void);

	virtual void OnClickTreeItem(HTREEITEM item, const CString& resName);

protected:
	///
	/// Request
	///
	struct ThumbRequest
	{
		ThumbRequest()
			: owner(0)
			, archive(0)
			, itemIdx(0)
		{
		}

		ResViewPanel* owner;
		Archive* archive;
		String sourceFile;
		int itemIdx;


		_EditorCoreExport friend std::ostream& operator<<(std::ostream& o, const ThumbRequest& r)
		{ (void)r; return o; }
	};

	///
	/// Response
	///
	struct ThumbResponse
	{
		ThumbResponse()
			: hbmp(0)
		{
		}

		HBITMAP hbmp;

		_EditorCoreExport friend std::ostream& operator<<(std::ostream& o, const ThumbResponse& r)
		{ (void)r; return o; }
	};

	/// WorkQueue::RequestHandler override
	bool canHandleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
	/// WorkQueue::RequestHandler override
	WorkQueue::Response* handleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
	/// WorkQueue::ResponseHandler override
	bool canHandleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);
	/// WorkQueue::ResponseHandler override
	void handleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);

	virtual bool canShowInTree(const String& resGroup) const;
};

///
/// Global texture resource view panel
///
class _EditorCoreExport ResPanel_Texture
	: public ResPanel_TextureBase
	, public Singleton<ResPanel_Texture>
{
public:

	virtual void OnDBClickListItem(int item, const CString& resName);
	virtual void OnClickListItem(int item, const CString& resName);		
};


//------------------------------------------------------------------------
///
/// Base material resource view panel
///
class _EditorCoreExport  ResPanel_MaterialBase
	: public ResViewPanel
{
public:

	ResPanel_MaterialBase();
	virtual ~ResPanel_MaterialBase();

	virtual CString GetPanelTitle() const { return "Material"; }
	virtual void OnInitListItem(void);
	virtual void OnRefreshListItem(void);
};

///
/// Global material resource view panel
///
class _EditorCoreExport ResPanel_Material
	: public ResPanel_MaterialBase
	, public Singleton<ResPanel_Material>
{
public:

	virtual void OnDBClickListItem(int item, const CString& resName);
};


//------------------------------------------------------------------------
///
/// Base particle system resource view panel
///
class _EditorCoreExport  ResPanel_ParticleSystemBase
	: public ResViewPanel
{
public:

	ResPanel_ParticleSystemBase();
	virtual ~ResPanel_ParticleSystemBase();


	virtual CString GetPanelTitle() const { return "Particle System"; }
	virtual void OnInitListItem(void);
	virtual void OnRefreshListItem(void);
};

///
/// Global particle system resource view panel
///
class _EditorCoreExport  ResPanel_ParticleSystem
	: public ResPanel_ParticleSystemBase
	, public Singleton<ResPanel_ParticleSystem>
{
public:

	virtual void OnDBClickListItem(int item, const CString& resName);
};