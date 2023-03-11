#pragma once
#include "EditorCorePrerequisites.h"

typedef map<String, bool>::type CompositorEnabledMap;
typedef std::pair<String, byte> CompositorStatus;
typedef map<UINT, CompositorStatus>::type CompositorStatusMap;

class _EditorCoreExport EditorUtil
{
public:

	static void InitCompositorList(CMFCToolBar& toolbar, UINT buttonID);
	static void SelectViewportCompsoitor(Viewport* vp, UINT buttonID, CompositorEnabledMap& compositorStatMap);

	static void InitCompositorList(CMFCRibbonComboBox* combo);
	static void SelectViewportCompsoitor(Viewport* vp, CMFCRibbonComboBox* combo, CompositorEnabledMap& compositorStatMap);

	static void InitCompositorList(CMFCRibbonPanel* panel, UINT startID, CompositorStatusMap& statMap);
	static void SelectViewportCompsoitor(Viewport* vp, CompositorStatusMap& statMap, UINT id);
	static void UpdateCompositorUI(CCmdUI* pCmdUI, CompositorStatusMap& statMap);


	typedef std::pair<String, HBITMAP> CreateThumbImageResult;

	static const String& GetThumbImageDir();
	static String NormalizeThumbFileName(const String& str);
	static String GetThumbFileName(const String& sourceFileName);
	static String GetParticleThumbFileName(const String& sourceFileName);

	static const String& GetThumbFileTag();

	static CreateThumbImageResult CreateThumbImage(const String& textureResName, int sz = 128,
		const String resGroup = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	static void InitMaterialSchemeList( CMFCRibbonComboBox* cbtn );

};