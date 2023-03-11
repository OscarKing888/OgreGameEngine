#include "stdafx.h"
#include "EditorUtil.h"

void EditorUtil::InitCompositorList(CMFCToolBar& toolbar, UINT buttonID)
{
	if(!CompositorManager::getSingletonPtr())
	{
		return;
	}

	CMFCToolBarComboBoxButton comboButton(buttonID, GetCmdMgr()->GetCmdImage(buttonID, FALSE), CBS_DROPDOWNLIST);

	CompositorManager::ResourceMapIterator resourceIterator =
		CompositorManager::getSingleton().getResourceIterator();

	//Viewport* vp = EditorGame::getSingleton().GetActiveViewport();

	comboButton.AddItem("None");
	comboButton.AddItem("Clear");
	// add all compositor resources to the view container
	while (resourceIterator.hasMoreElements())
	{
		ResourcePtr resource = resourceIterator.getNext();
		const String& compositorName = resource->getName();
		// Don't add base Ogre/Scene compositor to view
		if (compositorName == "Ogre/Scene")
			continue;
		// Don't add the deferred shading compositors, thats a different demo.
		if (Ogre::StringUtil::startsWith(compositorName, "DeferredShading", false))
			continue;

		comboButton.AddItem(compositorName.c_str());
	}

	comboButton.SelectItem(0);
	toolbar.ReplaceButton (buttonID, comboButton);
}
//--------------------------------------------------------------------------------
void EditorUtil::InitCompositorList( CMFCRibbonComboBox* combo)
{
	if(!CompositorManager::getSingletonPtr())
	{
		return;
	}

	combo->RemoveAllItems();

	CompositorManager::ResourceMapIterator resourceIterator =
		CompositorManager::getSingleton().getResourceIterator();

	//Viewport* vp = EditorGame::getSingleton().GetActiveViewport();

	combo->AddItem("None");
	combo->AddItem("Clear");
	// add all compositor resources to the view container
	while (resourceIterator.hasMoreElements())
	{
		ResourcePtr resource = resourceIterator.getNext();
		const String& compositorName = resource->getName();
		// Don't add base Ogre/Scene compositor to view
		if (compositorName == "Ogre/Scene")
			continue;
		// Don't add the deferred shading compositors, thats a different demo.
		if (Ogre::StringUtil::startsWith(compositorName, "DeferredShading", false))
			continue;

		combo->AddItem(compositorName.c_str());
	}

	combo->SelectItem(0);	
}
//--------------------------------------------------------------------------------
void EditorUtil::InitCompositorList( CMFCRibbonPanel* panel, UINT startID, CompositorStatusMap& statMap )
{
	CompositorManager::ResourceMapIterator resourceIterator =
		CompositorManager::getSingleton().getResourceIterator();

	int i = 0;
	while (resourceIterator.hasMoreElements())
	{
		ResourcePtr resource = resourceIterator.getNext();
		const String& compositorName = resource->getName();
		// Don't add base Ogre/Scene compositor to view
		if (compositorName == "Ogre/Scene")
			continue;
		// Don't add the deferred shading compositors, thats a different demo.
		if (Ogre::StringUtil::startsWith(compositorName, "DeferredShading", false))
			continue;

		UINT id = startID + i;		

		statMap.insert(std::make_pair(id,
			std::make_pair(compositorName, 0)));

		CMFCRibbonCheckBox* box = new CMFCRibbonCheckBox(id,
			compositorName.c_str());
		panel->Add(box);

		++i;
	}
}
//--------------------------------------------------------------------------------
void EditorUtil::SelectViewportCompsoitor(Viewport* vp, UINT buttonID,
										  CompositorEnabledMap& compositorStatMap)
{
	if(MaterialManager::getSingletonPtr())
	{
		CMFCToolBarComboBoxButton* pSrcCombo = NULL;

		CObList listButtons;
		if (CMFCToolBar::GetCommandButtons(buttonID, listButtons) > 0)
		{
			for (POSITION posCombo = listButtons.GetHeadPosition();
				pSrcCombo == NULL && posCombo != NULL;)
			{
				CMFCToolBarComboBoxButton* pCombo = DYNAMIC_DOWNCAST(CMFCToolBarComboBoxButton, listButtons.GetNext(posCombo));

				if (pCombo != NULL && CMFCToolBar::IsLastCommandFromButton(pCombo))
				{
					pSrcCombo = pCombo;
				}
			}
		}

		if (pSrcCombo != NULL)
		{
			ASSERT_VALID(pSrcCombo);

			LPCTSTR lpszSelItem = pSrcCombo->GetItem();
			CString strSelItem = (lpszSelItem == NULL) ? ("") : lpszSelItem;

			String compName = (LPCTSTR)strSelItem;

			CompositorManager& mgr = CompositorManager::getSingleton();
			if(strSelItem == "Clear")
			{
				mgr.removeCompositorChain(vp);
				compositorStatMap.clear();
			}
			else if(strSelItem == "None")			
			{
				CompositorChain* chain = mgr.getCompositorChain(vp);
				size_t c = chain->getNumCompositors();
				for (size_t i = 0; i < c; ++i)
				{
					chain->setCompositorEnabled(i, false);
				}

				CompositorEnabledMap::iterator i = compositorStatMap.begin();
				for (; i != compositorStatMap.end(); ++i)
				{
					i->second = false;
				}
			}
			else
			{
				CompositorEnabledMap::iterator i = compositorStatMap.find(compName);
				if(i != compositorStatMap.end())
				{
					//mgr.removeCompositorChain(vp);
					mgr.setCompositorEnabled(vp, compName, !i->second);
					i->second = !i->second;
				}
				else
				{
					//CompositorInstance* instance = 
					mgr.addCompositor(vp, compName);
					mgr.setCompositorEnabled(vp, compName, true);
					compositorStatMap.insert(std::make_pair(compName, true));
				}
			}

			//CompositorManager::getSingleton().setCompositorEnabled(vp, (LPCTSTR)strSelItem, true);
			//MaterialManager::getSingleton().setActiveScheme((LPCTSTR)strSelItem);
			//AfxMessageBox(strSelItem);
		}
		else
		{
			AfxMessageBox(("Compositor Manager not inited!"));
		}
	}
}
//--------------------------------------------------------------------------------
void EditorUtil::SelectViewportCompsoitor( Viewport* vp, CMFCRibbonComboBox* combo, CompositorEnabledMap& compositorStatMap )
{
	if(MaterialManager::getSingletonPtr())
	{
		LPCTSTR lpszSelItem = combo->GetItem(combo->GetCurSel());
		CString strSelItem = (lpszSelItem == NULL) ? ("") : lpszSelItem;

		String compName = (LPCTSTR)strSelItem;

		CompositorManager& mgr = CompositorManager::getSingleton();
		if(strSelItem == "Clear")
		{
			mgr.removeCompositorChain(vp);
			compositorStatMap.clear();
		}
		else if(strSelItem == "None")			
		{
			CompositorChain* chain = mgr.getCompositorChain(vp);
			size_t c = chain->getNumCompositors();
			for (size_t i = 0; i < c; ++i)
			{
				chain->setCompositorEnabled(i, false);
			}

			CompositorEnabledMap::iterator i = compositorStatMap.begin();
			for (; i != compositorStatMap.end(); ++i)
			{
				i->second = false;
			}
		}
		else
		{
			CompositorEnabledMap::iterator i = compositorStatMap.find(compName);
			if(i != compositorStatMap.end())
			{
				//mgr.removeCompositorChain(vp);
				mgr.setCompositorEnabled(vp, compName, !i->second);
				i->second = !i->second;
			}
			else
			{
				//CompositorInstance* instance = 
				mgr.addCompositor(vp, compName);
				mgr.setCompositorEnabled(vp, compName, true);
				compositorStatMap.insert(std::make_pair(compName, true));
			}
		}
	}
	else
	{
		AfxMessageBox("Compositor Manager not inited!");
	}
}
//--------------------------------------------------------------------------------
void EditorUtil::SelectViewportCompsoitor( Viewport* vp, CompositorStatusMap& statMap, UINT id )
{
	CompositorStatusMap::iterator i = statMap.find(id);
	if(i != statMap.end())
	{
		CompositorManager& mgr = CompositorManager::getSingleton();
		const String& compName = i->second.first;
		byte stats = i->second.second;
		if(stats == 0)
		{
			mgr.addCompositor(vp, compName);
			mgr.setCompositorEnabled(vp, compName, true);
			i->second.second = 1;
		}
		else if(stats == 1) // 当前打开
		{
			i->second.second = 2;
			mgr.setCompositorEnabled(vp, compName, false);
		}
		else if(stats == 2) // 当前关闭
		{
			i->second.second = 1;
			mgr.setCompositorEnabled(vp, compName, true);
		}
	}
}
//--------------------------------------------------------------------------------
void EditorUtil::UpdateCompositorUI( CCmdUI* pCmdUI, CompositorStatusMap& statMap )
{
	CompositorStatusMap::iterator i = statMap.find(pCmdUI->m_nID);
	if(i != statMap.end())
	{
		pCmdUI->SetCheck(i->second.second == 1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}
//--------------------------------------------------------------------------------
const String& EditorUtil::GetThumbImageDir()
{
	static String thumbPath;
	if(thumbPath.empty())
	{
		thumbPath = FileEx::GetResourceFilePath("__THUMBVIEW_DIR__", false);
	}

	return thumbPath;
}
//--------------------------------------------------------------------------------
String EditorUtil::NormalizeThumbFileName(const String& str)
{
	CString strT = str.c_str();
	strT.Replace("../", "");
	strT.Replace("..\\", "");
	strT.Replace("\\", "_");
	strT.Replace("/", "_");
	strT.Replace(":", "");
	return (LPCTSTR)strT;
}
//--------------------------------------------------------------------------------
const String& EditorUtil::GetThumbFileTag()
{
	static String thumbTag = "_iconthumb.bmp";
	return thumbTag;
}
//--------------------------------------------------------------------------------
String EditorUtil::GetThumbFileName(const String& sourceFileName)
{
	String path = FileEx::GetResourceFilePath(sourceFileName, true);
	path = NormalizeThumbFileName(path);
	String baseName, ext, pth;
	StringUtil::splitFullFilename(path, baseName, ext, pth);
	String thumbFile = EditorUtil::GetThumbImageDir() + pth + baseName + GetThumbFileTag();
	return thumbFile;
}
//--------------------------------------------------------------------------------
String EditorUtil::GetParticleThumbFileName(const String& sourceFileName)
{
	String path = sourceFileName;
	path = NormalizeThumbFileName(path);
	String baseName, ext, pth;
	StringUtil::splitFullFilename(path, baseName, ext, pth);
	String thumbFile = EditorUtil::GetThumbImageDir() + pth + baseName + GetThumbFileTag();
	return thumbFile;
}
//--------------------------------------------------------------------------------
EditorUtil::CreateThumbImageResult EditorUtil::CreateThumbImage(const String& textureResName, int sz, const String resGroup)
{
	CreateThumbImageResult r;
	r.second = 0;

	String icon = NormalizeThumbFileName(textureResName.c_str());
	icon = GetThumbImageDir() + icon;
	r.first = icon;

	if(!File::isFileExist(r.first))
	{
		DataStreamPtr dataStrm = ResourceGroupManager::getSingleton()
			.openResource(textureResName, resGroup);

		//Codec::DecodeResult dr = codec->decode(dataStrm);
		//img.load(dataStrm, tex->getWidth(), tex->getHeight(), PF_A8R8G8B8);

		String base, ext, pth;
		StringUtil::splitFullFilename(textureResName, base, ext, pth);
		StringUtil::toLowerCase(ext);

		Image img;
		img.load(dataStrm, ext);
		PixelFormat fmt = img.getFormat();
		//if(fmt != PF_DXT1)
		{
			if((!PixelUtil::isCompressed(fmt)) && (img.getDepth() == 1))
			{
				img.resize(sz, sz);
				img.save(r.first);
			}
			else
			{
				return r;
			}
		}
	}

	r.second = (HBITMAP)LoadImage(AfxGetInstanceHandle(), r.first.c_str(),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	return r;
}
//--------------------------------------------------------------------------------
void EditorUtil::InitMaterialSchemeList( CMFCRibbonComboBox* cbtn )
{
	ConfigFile cfg;
	cfg.loadFromResourceSystem("Material.cfg", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	String val = cfg.getSetting("Count", "Schemes");
	int cnt = StringConverter::parseInt(val);
	for (int i = 0; i < cnt; ++i)
	{
		String key = formatString("S[%d]", i);
		val = cfg.getSetting(key, "Schemes");
		cbtn->AddItem(val.c_str());
	}

	cbtn->SelectItem(0);
}
//--------------------------------------------------------------------------------
