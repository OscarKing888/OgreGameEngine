// ResViewPanel.cpp : implementation file
//

#include "stdafx.h"
#include "ResViewPanel.h"
#include <xutility>
#include "Cmd_SetProperty.h"
#include <OgreCodec.h>
#include "Resource.h"
#include "EditorUtil.h"
#include "PropertyGrid.h"
#include "EditorApp.h"
#include <OgreImageCodec.h>

template<> ResPanel_Material* Singleton<ResPanel_Material>::ms_Singleton = 0;
template<> ResPanel_Texture* Singleton<ResPanel_Texture>::ms_Singleton = 0;
template<> ResPanel_Mesh* Singleton<ResPanel_Mesh>::ms_Singleton = 0;
template<> ResPanel_ParticleSystem* Singleton<ResPanel_ParticleSystem>::ms_Singleton = 0;

//------------------------------------------------------------------------
// ResPanel_Mesh
//------------------------------------------------------------------------
ResPanel_MeshBase::ResPanel_MeshBase()
{
	_dragdropGameObjectTypeName = "AnimationModel";
	_canDragCreate = true;
}

ResPanel_MeshBase::~ResPanel_MeshBase()
{

}

void ResPanel_MeshBase::OnInitListItem(void)
{
	OnViewList();
	ClearListItem();
	OnClickTreeItem(0, _selectedTreeItemName);
}

void ResPanel_MeshBase::OnClickTreeItem(HTREEITEM item, const CString& resName)
{
	OnViewBigIconList();

	ClearListItem();
	Archive* archive = 0;
	ArchiveManager::ArchiveMapIterator it =
		ArchiveManager::getSingleton().getArchiveIterator();
	while(it.hasMoreElements())
	{
		if(it.peekNextKey() == (LPCTSTR)resName)
		{
			archive = it.peekNextValue();
			break;
		}
		it.moveNext();
	}

	CreateImageList();

	if(archive)
	{
		StringVectorPtr files = archive->find("*.mesh");
		_listCtrl.SetItemCount(files->size());
		for (size_t j = 0; j < files->size(); ++j)
		{
			String srcName = (*files)[j];
			String fileName = EditorUtil::GetThumbFileName(srcName);
			int imgIdx = -1;
			if(FileEx::isDirExist(fileName))
			{
				ImageListMap::iterator it = _iconMap.find(fileName);
				if(it != _iconMap.end())
				{
					imgIdx = it->second;
				}
				else
				{
					//imgIdx = AddToImageList(fileName);
					ThumbRequest req;
					req.owner = this;
					req.sourceFile = fileName;
					req.itemIdx = _listCtrl.InsertItem(j, srcName.c_str());
					AddLoadingRequest(Any(req));
				}
			}
			else
			{
				imgIdx = 0;
			}

			if(imgIdx != -1)
			{
				_listCtrl.InsertItem(j, srcName.c_str(), imgIdx);
			}
		} // for
	}

	OnRefreshListItem();
}

bool ResPanel_MeshBase::canHandleRequest( const WorkQueue::Request* req,
											const WorkQueue* srcQ )
{
	ThumbRequest r = any_cast<ThumbRequest>(req->getData());
	// only deal with own requests
	if (r.owner != this)
		return false;
	else
		return RequestHandler::canHandleRequest(req, srcQ);
}

WorkQueue::Response* ResPanel_MeshBase::handleRequest( const WorkQueue::Request* req,
														 const WorkQueue* srcQ )
{
	ThumbRequest r = any_cast<ThumbRequest>(req->getData());

	ThumbResponse resp;
	resp.hbmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), r.sourceFile.c_str(),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	WorkQueue::Response* response = OGRE_NEW WorkQueue::Response(req, true, Any(resp));
	return response;
}

bool ResPanel_MeshBase::canHandleResponse( const WorkQueue::Response* res, const WorkQueue* srcQ )
{
	ThumbRequest r = any_cast<ThumbRequest>(res->getRequest()->getData());
	// only deal with own requests
	if (r.owner != this)
		return false;
	else
		return ResponseHandler::canHandleResponse(res, srcQ);
}

void ResPanel_MeshBase::handleResponse( const WorkQueue::Response* res, const WorkQueue* srcQ )
{
	ThumbResponse r = any_cast<ThumbResponse>(res->getData());
	ThumbRequest req = any_cast<ThumbRequest>(res->getRequest()->getData());

	if(r.hbmp)
	{
		CBitmap* bmp = CBitmap::FromHandle(r.hbmp);
		int idx = _iconImages.Add(bmp, RGB(0, 0, 0));
		//_iconImagesSmall.Add(bmp, RGB(0, 0, 0));

		_iconMap.insert(std::make_pair(req.sourceFile, idx));
		AddToSmallImageList(bmp);

		_listCtrl.SetItem(req.itemIdx, 0, LVIF_IMAGE, NULL, idx, 0, 0, 0);
		//LogDebugIDf("Editor", "List icon load completed:[%d]->[%d]%s", t->GetItemIdx(), idx, t->GetSourceFileName().c_str());
	}
	else
	{
		LogErrorIDf("Editor", "Loading icon failed:%s", req.sourceFile.c_str());
	}

	_isLoading = false;
}

//--------------------------------------------------------------------------------
///
/// ResPanel_Mesh
///

void ResPanel_Mesh::OnDBClickListItem(int item, const CString& resName)
{
	IPropertyObject* obj = ObjectPropertiesWnd::getSingleton().GetObject();
	if(obj && obj->HasProperty("MeshFile"))
	{
		EditorApp::getSingleton().GetCommandHistory().AddCommand(
			OGRE_NEW Cmd_SetProperty(obj, "MeshFile", (LPCTSTR)resName));

		if(obj->HasProperty("EnabledAnimations"))
		{
			if(obj->GetProperty("EnabledAnimations").empty())
			{
				String allAnis = obj->GetProperty("AllAnimations");
				StringVector v = StringUtil::split(allAnis, "|");
				if(!v.empty())
				{
					obj->SetProperty("EnabledAnimations", v.front());
				}
			}
		}
	}
}

//------------------------------------------------------------------------
// ResPanel_Texture
//------------------------------------------------------------------------

void ResPanel_TextureBase::OnInitListItem(void)
{
	ClearListItem();
	OnClickTreeItem(0, _selectedTreeItemName);
}

void ResPanel_TextureBase::OnClickTreeItem(HTREEITEM item, const CString& resName)
{
	ClearListItem();
	Archive* archive = 0;
	ArchiveManager::ArchiveMapIterator it = ArchiveManager::getSingleton().getArchiveIterator();
	while(it.hasMoreElements())
	{
		if(it.peekNextKey() == (LPCTSTR)resName)
		{
			archive = it.peekNextValue();
			break;
		}
		it.moveNext();
	}

	CreateImageList();

	if(archive)
	{
		StringVectorPtr files = archive->list();
		_listCtrl.SetItemCount(files->size());
		int insetIdx = 0;
		for (size_t j = 0; j < files->size(); ++j)
		{
			String fileName = (*files)[j];
			String base, ext, pth;
			StringUtil::splitFullFilename(fileName, base, ext, pth);
			StringUtil::toLowerCase(ext);

			StringVector texFileExt = Codec::getExtensions();
			for (size_t i = 0; i < texFileExt.size(); ++i)
			{
				if(ext == texFileExt[i])
				{
					int idx = -1;

					// 不是thumb文件
					if(String::npos != fileName.find(EditorUtil::GetThumbFileTag(), 0))
					{
						continue;
					}
					else
					{
						ImageListMap::iterator it = _iconMap.find(fileName);
						if(it != _iconMap.end())
						{
							idx = it->second;
						}
						else
						{
							int newIdx = _listCtrl.InsertItem(insetIdx, fileName.c_str());
							//LogDebugIDf("Editor", "[%d] Cnt:%d [%s]", idx, insetIdx, fileName.c_str());
							++insetIdx;
							ThumbRequest req;
							req.archive = archive;
							req.owner = this;
							req.sourceFile = fileName;
							req.itemIdx = newIdx;
							AddLoadingRequest(Any(req));							
						}
					}


					if(idx != -1)
					{
						_listCtrl.InsertItem(insetIdx, fileName.c_str(), idx);
						++insetIdx;
					}
					else
					{
						//_listCtrl.InsertItem(j, fileName.c_str(), 0);
					}

					break;
				} // if ext
			} // for
		} // for
	}

	OnRefreshListItem();
}

bool ResPanel_TextureBase::canHandleRequest( const WorkQueue::Request* req,
									const WorkQueue* srcQ )
{
	ThumbRequest r = any_cast<ThumbRequest>(req->getData());
	// only deal with own requests
	if (r.owner != this)
		return false;
	else
		return RequestHandler::canHandleRequest(req, srcQ);
}

WorkQueue::Response* ResPanel_TextureBase::handleRequest( const WorkQueue::Request* req,
												 const WorkQueue* srcQ )
{
	ThumbRequest r = any_cast<ThumbRequest>(req->getData());

	String icon = EditorUtil::GetThumbFileName(r.sourceFile);

	if(!File::isFileExist(icon))
	{
		DataStreamPtr dataStrm = r.archive->open(r.sourceFile);

		RenderSystemCapabilities* rc =
			(RenderSystemCapabilities*)Root::getSingleton().getRenderSystem()->getCapabilities();

		bool dxtCompress = rc->hasCapability(RSC_TEXTURE_COMPRESSION_DXT);
		rc->unsetCapability(RSC_TEXTURE_COMPRESSION_DXT);

		String base, ext, pth;
		StringUtil::splitFullFilename(r.sourceFile, base, ext, pth);
		StringUtil::toLowerCase(ext);
		Codec* codec = Codec::getCodec(ext);
		Codec::DecodeResult dr = codec->decode(dataStrm);
		Ogre::ImageCodec::ImageData* imgDat = dynamic_cast<ImageCodec::ImageData*>(dr.second.get());

		uchar* dat = OGE_ALLOC_T(uchar, dr.first->size());
		memcpy(dat, dr.first->getPtr(), dr.first->size());
		Ogre::Image img;
		img.loadDynamicImage(dat, imgDat->width, imgDat->height, 1, imgDat->format, true);

		img.resize(128, 128);
		img.save(icon);

		if(dxtCompress)
		{
			//rc->setCapability(RSC_TEXTURE_COMPRESSION_DXT);
		}
	}

	ThumbResponse resp;
	resp.hbmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), icon.c_str(),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	WorkQueue::Response* response = OGRE_NEW WorkQueue::Response(req, true, Any(resp));
	return response;
}

bool ResPanel_TextureBase::canHandleResponse( const WorkQueue::Response* res, const WorkQueue* srcQ )
{
	ThumbRequest r = any_cast<ThumbRequest>(res->getRequest()->getData());
	// only deal with own requests
	if (r.owner != this)
		return false;
	else
		return ResponseHandler::canHandleResponse(res, srcQ);
}

void ResPanel_TextureBase::handleResponse( const WorkQueue::Response* res, const WorkQueue* srcQ )
{
	ThumbResponse r = any_cast<ThumbResponse>(res->getData());
	ThumbRequest req = any_cast<ThumbRequest>(res->getRequest()->getData());

	if(r.hbmp)
	{
		CBitmap* bmp = CBitmap::FromHandle(r.hbmp);
		int idx = _iconImages.Add(bmp, RGB(0, 0, 0));
		//_iconImagesSmall.Add(bmp, RGB(0, 0, 0));

		_iconMap.insert(std::make_pair(req.sourceFile, idx));
		AddToSmallImageList(bmp);

		_listCtrl.SetItem(req.itemIdx, 0, LVIF_IMAGE, NULL, idx, 0, 0, 0);
		//LogDebugIDf("Editor", "List icon load completed:[%d]->[%d]%s", t->GetItemIdx(), idx, t->GetSourceFileName().c_str());
	}
	else
	{
		LogErrorIDf("Editor", "Loading icon failed:%s", req.sourceFile.c_str());
	}

	_isLoading = false;
}

bool ResPanel_TextureBase::canShowInTree( const String& resGroup ) const
{
	if(String::npos != resGroup.find("Thumbview"))
	{
		return false;
	}

	return true;
}
//--------------------------------------------------------------------------------
///
/// ResPanel_Texture 
///

void ResPanel_Texture::OnDBClickListItem(int item, const CString& resName)
{
	
}

void ResPanel_Texture::OnClickListItem(int item, const CString& resName)
{
	String base, ext, pth;
	StringUtil::splitFullFilename((LPCTSTR)resName, base, ext, pth);
	StringUtil::toLowerCase(ext);

	StringVector imgExts = Codec::getExtensions();
	for (size_t i = 0; i < imgExts.size(); ++i)
	{
		String imgExt = imgExts[i];
		StringUtil::toLowerCase(imgExt);
		if(imgExt == ext)
		{
			EventsManager::getSingleton().CreateEvent("SelectTexture", Any(String((LPCTSTR)resName)));
			break;
		}
	}
}

//------------------------------------------------------------------------
// ResPanel_Material
//------------------------------------------------------------------------

ResPanel_MaterialBase::ResPanel_MaterialBase()
: ResViewPanel(false)
{

}

ResPanel_MaterialBase::~ResPanel_MaterialBase()
{

}

void ResPanel_MaterialBase::OnInitListItem(void)
{
	OnViewList();
	OnRefreshListItem();
}

void ResPanel_MaterialBase::OnRefreshListItem(void)
{
	ClearListItem();

	MaterialManager& mgr = MaterialManager::getSingleton();
	MaterialManager::ResourceMapIterator i = mgr.getResourceIterator();
	while (i.hasMoreElements())
	{
		_listCtrl.InsertItem(_listCtrl.GetItemCount(), i.peekNextValue()->getName().c_str());
		i.moveNext();
	}

	ResViewPanel::OnRefreshListItem();
}

void ResPanel_Material::OnDBClickListItem(int item, const CString& resName)
{
	IPropertyObject* obj = ObjectPropertiesWnd::getSingleton().GetObject();
	if(obj && obj->HasProperty("MaterialName"))
	{
		EditorApp::getSingleton().GetCommandHistory().
			AddCommand(OGRE_NEW Cmd_SetProperty(obj, "MaterialName", (LPCTSTR)resName));
	}
}

//------------------------------------------------------------------------
// ResPanel_ParticleSystem
//------------------------------------------------------------------------

ResPanel_ParticleSystemBase::ResPanel_ParticleSystemBase()
: ResViewPanel(false)
{
	_dragdropGameObjectTypeName = "ParticleSys";
	_canDragCreate = true;
}

ResPanel_ParticleSystemBase::~ResPanel_ParticleSystemBase()
{

}

void ResPanel_ParticleSystemBase::OnInitListItem(void)
{
	OnViewList();
	OnRefreshListItem();
}

void ResPanel_ParticleSystemBase::OnRefreshListItem(void)
{
	ClearListItem();

	CreateImageList();
	OnViewBigIconList();

	ParticleSystemManager& mgr = ParticleSystemManager::getSingleton();
	ParticleSystemManager::ParticleSystemTemplateIterator i = mgr.getTemplateIterator();
	while (i.hasMoreElements())
	{
		String particleName = i.peekNextKey();
		String fileName = EditorUtil::GetParticleThumbFileName(particleName);
		int imgIdx = 0;
		if(FileEx::isDirExist(fileName))
		{
			ImageListMap::iterator it = _iconMap.find(fileName);
			if(it != _iconMap.end())
			{
				imgIdx = it->second;
			}
			else
			{
				imgIdx = AddToImageList(fileName);
			}
		}

		_listCtrl.InsertItem(_listCtrl.GetItemCount(), particleName.c_str(), imgIdx);

		i.moveNext();
	}

	ResViewPanel::OnRefreshListItem();
}


void ResPanel_ParticleSystem::OnDBClickListItem(int item, const CString& resName)
{
	IPropertyObject* obj = ObjectPropertiesWnd::getSingleton().GetObject();
	if(obj && obj->HasProperty("ParticleTemplateName"))
	{
		EditorApp::getSingleton().GetCommandHistory()
			.AddCommand(OGRE_NEW Cmd_SetProperty(obj, "ParticleTemplateName", (LPCTSTR)resName));

		//Root::getSingleton().renderOneFrame();
	}
}