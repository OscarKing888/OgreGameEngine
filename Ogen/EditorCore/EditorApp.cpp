#include "stdafx.h"
#include "EditorApp.h"
#include "EditorConfig.h"
#include "AbstractEditState.h"
#include "Engine/OgreLib.h"
#include "Engine/IState.h"

const String EditorLogID = "Editor";
const String EditorResourceGroup = "Editor";

template<> EditorApp* Singleton<EditorApp>::ms_Singleton = 0;

UINT GetAutoWMID()
{
	static UINT id = EDITOR_MSGID_START;
	++id;
	return id;
}

int GetAutoEditStateID()
{
	static int id2 = EDITOR_ID_START;
	++id2;
	return id2;
}
//--------------------------------------------------------------------------------
// EditorPlugin
//--------------------------------------------------------------------------------
EditorPlugin::EditorPlugin(const Ogre::String& name)
: _name(name)
{
	EditorApp::getSingleton().AddPlugin(this);
	GameApp::getSingleton().AddAppListener(this);
}
//--------------------------------------------------------------------------------
EditorPlugin::~EditorPlugin()
{
	EditorApp::getSingleton().RemovePlugin(this);
	GameApp::getSingleton().RemoveAppListener(this);
}
//--------------------------------------------------------------------------------
const Ogre::String& EditorPlugin::getName() const
{
	return _name;
}
//--------------------------------------------------------------------------------
void EditorPlugin::install()
{
}
//--------------------------------------------------------------------------------
void EditorPlugin::uninstall()
{
}
//--------------------------------------------------------------------------------
void EditorPlugin::initialise()
{
}
//--------------------------------------------------------------------------------
void EditorPlugin::shutdown()
{
}
//--------------------------------------------------------------------------------
void EditorPlugin::onCreateEditorMainWnd(CFrameWndEx* mainWnd)
{
}
//--------------------------------------------------------------------------------
void EditorPlugin::onCreateDockPanel( CFrameWndEx* mainWnd, CDockablePane* leftRoot,
										 CDockablePane* rightRoot,
										 CDockablePane* bottomRoot )
{

}

//--------------------------------------------------------------------------------
void EditorPlugin::onCreateEditStates(IStateManager* sm)
{
}
//--------------------------------------------------------------------------------
void EditorPlugin::onWindowResized(RenderWindow* rw)
{
}
//--------------------------------------------------------------------------------
const String& EditorPlugin::getDescription() const
{
	return _description;
}
//--------------------------------------------------------------------------------
void EditorPlugin::onCreateEditorMainToolbar( CMFCRibbonBar* mainToolbar )
{

}

//--------------------------------------------------------------------------------
void EditorPlugin::onMapCreate( const String& mapName )
{

}
//--------------------------------------------------------------------------------
void EditorPlugin::onMapLoad( const String& mapName )
{

}
//--------------------------------------------------------------------------------
void EditorPlugin::onMapUnload( const String& mapName )
{

}
//--------------------------------------------------------------------------------
void EditorPlugin::onMapSave( const String& mapName )
{

}
//--------------------------------------------------------------------------------
void EditorPlugin::onCreateGlobalObjects( CMFCRibbonCategory* cat, CMFCRibbonPanel* mainPanel )
{

}
//--------------------------------------------------------------------------------
void EditorPlugin::onCreateToolsCategory( CMFCRibbonCategory* cat, CMFCRibbonPanel* mainPanel )
{

}
//--------------------------------------------------------------------------------
void EditorPlugin::onPostCreateEditStateCategory( CMFCRibbonCategory* cat )
{

}
//--------------------------------------------------------------------------------
void GetMeshInformation(const MeshPtr mesh, size_t &vertex_count, Vector3* &vertices,
						size_t &index_count, unsigned long* &indices, const Vector3 &position, 
						const Quaternion &orient, const Vector3 &scale)
{
	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	vertex_count = index_count = 0;

	// Calculate how many vertices and indices we're going to need
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		SubMesh* submesh = mesh->getSubMesh(i);

		// We only need to add the shared vertices once
		if (submesh->useSharedVertices)
		{
			if (!added_shared)
			{
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else
		{
			vertex_count += submesh->vertexData->vertexCount;
		}
		// Add the indices
		index_count += submesh->indexData->indexCount;
	}

	// Allocate space for the vertices and indices
	vertices = OGE_ALLOC_T(Vector3, vertex_count);
	indices = OGE_ALLOC_T(unsigned long, index_count);

	added_shared = false;

	// Run through the submeshes again, adding the data into the arrays
	for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		SubMesh* submesh = mesh->getSubMesh(i);

		VertexData* vertex_data = submesh->useSharedVertices ?
			mesh->sharedVertexData : submesh->vertexData;

		if ((!submesh->useSharedVertices) ||
			(submesh->useSharedVertices && !added_shared))
		{
			if (submesh->useSharedVertices)
			{
				added_shared = true;
				shared_offset = current_offset;
			}

			const VertexElement* posElem =
				vertex_data->vertexDeclaration->findElementBySemantic(VES_POSITION);

			HardwareVertexBufferSharedPtr vbuf =
				vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* vertex = static_cast<unsigned char*>(
				vbuf->lock(HardwareBuffer::HBL_READ_ONLY));

			// There is _no_ baseVertexPointerToElement() which takes an Real or a double
			//  as second argument. So make it float, to avoid trouble when Real will
			//  be comiled/typedefed as double:
			//      Real* pReal;
			float* pReal;

			for (size_t j = 0; j < vertex_data->vertexCount;
				++j, vertex += vbuf->getVertexSize())
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);
				Vector3 pt(pReal[0], pReal[1], pReal[2]);
				vertices[current_offset + j] = (orient * (pt * scale)) + position;
			}

			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}

		IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

		bool use32bitindexes = (ibuf->getType() == HardwareIndexBuffer::IT_32BIT);

		unsigned long*  pLong = static_cast<unsigned long*>(
			ibuf->lock(HardwareBuffer::HBL_READ_ONLY));

		unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


		size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

		if (use32bitindexes)
		{
			for (size_t k = 0; k < numTris*3; ++k)
			{
				indices[index_offset++] =
					pLong[k] + static_cast<unsigned long>(offset);
			}
		}
		else
		{
			for (size_t k = 0; k < numTris*3; ++k)
			{
				indices[index_offset++] = static_cast<unsigned long>(
					pShort[k]) + static_cast<unsigned long>(offset);
			}
		}

		ibuf->unlock();
		current_offset = next_offset;
	}
}
//--------------------------------------------------------------------------------
void GetMeshInformation(const VertexData* vd, const IndexData* id,
						size_t& vertex_count, Vector3*& vertices,
						size_t& index_count, unsigned long*& indices,
						const Vector3& position, const Quaternion& orient,
						const Vector3& scale)
{
	bool added_shared = false;
	size_t current_offset = 0;
	//	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	vertex_count = index_count = 0;

	const VertexBufferBinding::VertexBufferBindingMap& vbmap =
		vd->vertexBufferBinding->getBindings();

	VertexBufferBinding::VertexBufferBindingMap::const_iterator it = vbmap.begin();
	for(; it != vbmap.end(); ++it)
	{
		HardwareVertexBufferSharedPtr hvb = it->second;
		vertex_count += hvb->getNumVertices();
	}

	vertex_count = vd->vertexCount;

	//index_count = id->indexBuffer->getNumIndexes();
	index_count = id->indexCount;

	// Allocate space for the vertices and indices
	vertices = OGE_ALLOC_T(Vector3, vertex_count);
	indices = OGE_ALLOC_T(unsigned long, index_count);

	added_shared = false;

	const VertexData* vertex_data = vd;
	const VertexElement* posElem =
		vertex_data->vertexDeclaration->findElementBySemantic(VES_POSITION);
	HardwareVertexBufferSharedPtr vbuf =
		vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

	unsigned char* vertex = static_cast<unsigned char*>(
		vbuf->lock(HardwareBuffer::HBL_READ_ONLY));

	// There is _no_ baseVertexPointerToElement() which takes an Real or a double
	//  as second argument. So make it float, to avoid trouble when Real will
	//  be comiled/typedefed as double:
	//      Real* pReal;
	float* pReal;

	for (size_t j = 0; j < vertex_data->vertexCount;
		++j, vertex += vbuf->getVertexSize())
	{
		posElem->baseVertexPointerToElement(vertex, &pReal);
		Vector3 pt(pReal[0], pReal[1], pReal[2]);
		vertices[current_offset + j] = (orient * (pt * scale)) + position;
	}

	vbuf->unlock();
	next_offset += vertex_data->vertexCount;

	const IndexData* index_data = id;
	size_t numTris = index_data->indexCount / 3;
	HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

	bool use32bitindexes = (ibuf->getType() == HardwareIndexBuffer::IT_32BIT);

	unsigned long*  pLong = static_cast<unsigned long*>(
		ibuf->lock(HardwareBuffer::HBL_READ_ONLY));

	unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


	size_t offset = current_offset;

	if (use32bitindexes)
	{
		for (size_t k = 0; k < numTris*3; ++k)
		{
			indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
		}
	}
	else
	{
		for (size_t k = 0; k < numTris*3; ++k)
		{
			indices[index_offset++] = static_cast<unsigned long>(
				pShort[k]) + static_cast<unsigned long>(offset);
		}
	}

	ibuf->unlock();
	current_offset = next_offset;
}
//--------------------------------------------------------------------------------
// EditorApp
//--------------------------------------------------------------------------------
EditorApp::EditorApp()
: _game(0)
, _activeCamera(0)
, _mainToolbar(0)
, _mfcApp(0)
, _terrainRayQuery(0)
, _activeViewport(0)
{
	GameApp::getSingleton().AddAppListener(this);
}
//--------------------------------------------------------------------------------
EditorApp::~EditorApp()
{
}
//--------------------------------------------------------------------------------
void EditorApp::LoadPlugins(const String& cfgFile)
{
	if(cfgFile.empty())
	{
		LogWarningIDf(EditorLogID, "[%s] config file[%s] is empty!", cfgFile.c_str());
		return;
	}

	StringVector pluginList;
	String pluginDir;
	ConfigFile cfg;

	try 
	{
		cfg.load( cfgFile );
	}
	catch (Exception)
	{
		LogErrorIDf(EditorLogID,
			"EditorApp::LoadPlugins[%s] not found, automatic plugin loading disabled.",
			cfgFile.c_str());

		return;
	}

	pluginDir = cfg.getSetting("PluginFolder"); // Ignored on Mac OS X, uses Resources/ directory
	pluginList = cfg.getMultiSetting("Plugin");

	if (pluginDir.empty())
	{
		// User didn't specify plugins folder, try current one
		pluginDir = ".";
	}

	char last_char = pluginDir[pluginDir.length()-1];
	if (last_char != '/' && last_char != '\\')
	{
		pluginDir += "\\";
	}

	for( StringVector::iterator it = pluginList.begin(); it != pluginList.end(); ++it )
	{
		Root::getSingleton().loadPlugin(*it);
	}
}
//--------------------------------------------------------------------------------
const EditorPluginList& EditorApp::GetLoadedPluginList() const
{
	return _loadedPlugins;
}
//--------------------------------------------------------------------------------
const EditorPlugin* EditorApp::GetPlugin(const String& pluginName) const
{
	EditorPluginList::const_iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		EditorPlugin* p = *i;
		if(p->getName() == pluginName)
		{
			p;
		}
	}

	LogErrorIDf(EditorLogID, "[%s]Plugin [%s] not found!",
		__FUNCTION__, pluginName.c_str());

	return 0;
}
//--------------------------------------------------------------------------------
void EditorApp::AddPlugin(EditorPlugin* plugin)
{
	_loadedPlugins.push_back(plugin);
}
//--------------------------------------------------------------------------------
void EditorApp::RemovePlugin(EditorPlugin* plugin)
{
	EditorPluginList::iterator i = std::find(
		_loadedPlugins.begin(), _loadedPlugins.end(), plugin);
	if(i != _loadedPlugins.end())
	{
		_loadedPlugins.erase(i);
	}
	else
	{
		LogWarningIDf(EditorLogID, "[%s]Plugin [%s] not found!",
			__FUNCTION__, plugin->getName().c_str());
	}
}
//--------------------------------------------------------------------------------
void EditorApp::UnloadPlugin(const String& pluginName)
{
	Root::getSingleton().unloadPlugin(pluginName);
}
//--------------------------------------------------------------------------------
void EditorApp::UnloadAllPlugins()
{
	EditorPluginList::iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		UnloadPlugin((*i)->getName());
	}

	_loadedPlugins.clear();
}
//--------------------------------------------------------------------------------
IStateManager& EditorApp::GetStateManager()
{
	return _game->GetStateManager();
}
//--------------------------------------------------------------------------------
void EditorApp::AddState(IStatePtr s)
{
	GetStateManager().AddState(s);

	AbstractEditState* as = dynamic_cast<AbstractEditState*>(s.GetPointer());
	if(!as)
	{
		assert(false && "State is not derived from AbstractEditState");
	}
	else
	{
		_states.insert(std::make_pair(as->GetID(), as));
	}
}
//--------------------------------------------------------------------------------
void EditorApp::ActiveState( const String& name )
{
	GetStateManager().ActiveState(name);
}
//--------------------------------------------------------------------------------
void EditorApp::ActiveState( int id )
{
	GetStateManager().ActiveState(id);
}
//--------------------------------------------------------------------------------
void EditorApp::AddMemento(IMementoPtr m)
{
	_mementoList.push_back(m);
}
//--------------------------------------------------------------------------------
void EditorApp::RemoveMemento(IMementoPtr m)
{
	MementoList::iterator i = std::find(_mementoList.begin(),
		_mementoList.end(), m);
	if(i != _mementoList.end())
	{
		_mementoList.erase(i);
	}
}
//--------------------------------------------------------------------------------
void EditorApp::RemoveAllMemeto()
{
	_mementoList.clear();
}
//--------------------------------------------------------------------------------
Camera* EditorApp::GetActiveCamera()
{
	return _activeCamera;
}
//--------------------------------------------------------------------------------
void EditorApp::SetActiveCamera(Camera* cam)
{
	_activeCamera = cam;
}
//--------------------------------------------------------------------------------
const String& EditorApp::GetDisplayString(const String& keyString)
{
	if(!EditorConfig::getSingleton().GetTranslateUIToLocal())
	{
		return keyString;
	}

	String2StringMap::const_iterator i = _key2DisplayString.find(keyString);
	if(i != _key2DisplayString.end())
	{
		return i->second;
	}

	if(!keyString.empty())
	{
		_key2DisplayString.insert(std::make_pair(keyString, keyString));
	}

	return keyString;
}
//--------------------------------------------------------------------------------
const String& EditorApp::GetKeyString(const String& displayString)
{
	if(!EditorConfig::getSingleton().GetTranslateUIToLocal())
	{
		return displayString;
	}

	String2StringMap::const_iterator i = _display2KeyString.find(displayString);
	if(i != _display2KeyString.end())
	{
		return i->second;
	}

	return displayString;
}
//--------------------------------------------------------------------------------
void EditorApp::LoadDisplayStringCfg(const std::string& file,
									 const std::string& resGroup)
{
	// ¼ÓÔØÏÔÊ¾×Ö·û±í
	ConfigFile cfg;
	cfg.loadFromResourceSystem(file, resGroup);
	ConfigFile::SettingsIterator si = cfg.getSettingsIterator();
	
	//_key2DisplayString.clear();
	//_display2KeyString.clear();

	while (si.hasMoreElements())
	{
		_key2DisplayString.insert(std::make_pair(si.peekNextKey(), si.peekNextValue()));
		_display2KeyString.insert(std::make_pair(si.peekNextValue(), si.peekNextKey()));
		si.moveNext();
	}
}
//--------------------------------------------------------------------------------
void EditorApp::SaveDisplayStringToFile(const String& fileName)
{
	CStdioFile file;
	if(file.Open(fileName.c_str(),
		CFile::typeText | CFile::modeCreate | CFile::modeWrite) == TRUE)
	{
		String2StringMap::iterator i = _key2DisplayString.begin();
		CString str;
		for(; i != _key2DisplayString.end(); ++i)
		{
			if(i->first == i->second)
			{
				str.Format("%s=%s\n", i->first.c_str(), i->second.c_str());
				file.WriteString(str);
			}
		}

		file.Close();
	}
}
//--------------------------------------------------------------------------------
void EditorApp::NotifyCreateEditorMainWnd(CFrameWndEx* mainWnd)
{
	EditorPluginList::iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		EditorPlugin* ep = *i;
		ep->onCreateEditorMainWnd(mainWnd);
	}
}
//--------------------------------------------------------------------------------
void EditorApp::NotifyCreateEditStates(IStateManager* sm)
{
	EditorPluginList::iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		EditorPlugin* ep = *i;
		ep->onCreateEditStates(sm);
	}
}
//--------------------------------------------------------------------------------
void EditorApp::NotifywindowResized(RenderWindow* rw)
{
	EditorPluginList::iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		EditorPlugin* ep = *i;
		ep->onWindowResized(rw);
	}
}
//--------------------------------------------------------------------------------
void EditorApp::NotifyCreateMainToolbar( CMFCRibbonBar* mainToolbar )
{
	_mainToolbar = mainToolbar;
	EditorPluginList::iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		EditorPlugin* ep = *i;
		ep->onCreateEditorMainToolbar(mainToolbar);
	}
}
//--------------------------------------------------------------------------------
CMFCRibbonBar* EditorApp::GetMainToolbar()
{
	return _mainToolbar;
}
//--------------------------------------------------------------------------------
CWinApp* EditorApp::GetMFCApp()
{
	return _mfcApp;
}
//--------------------------------------------------------------------------------
void EditorApp::SetMFCApp( CWinApp* app )
{
	_mfcApp = app;
}
//--------------------------------------------------------------------------------
AbstractEditState* EditorApp::GetState( const String& name ) const
{
	StateNameMap::const_iterator i = _states.find(name);
	if(i != _states.end())
	{
		return i->second;
	}

	return 0;
}
//--------------------------------------------------------------------------------
AbstractEditState* EditorApp::GetState( int id ) const
{
	StateNameMap::const_iterator i = _states.begin();
	for (; i != _states.end(); ++i)
	{
		AbstractEditState* as = i->second;
		if(as->GetCommandID() == id)
		{
			return as;
		}
	}

	return 0;
}
//--------------------------------------------------------------------------------
const StateNameMap& EditorApp::GetStateList() const
{
	return _states;
}
//--------------------------------------------------------------------------------
TerrainGroup::RayResult EditorApp::PickOnTerrain( int x, int y, Camera* cam /*= 0*/ )
{
	if(!cam)
	{
		cam = _activeCamera;
	}

	if(!cam)
	{
		return TerrainGroup::RayResult(false, 0, Vector3::ZERO);
	}

	Viewport* vp = cam->getViewport();
	float tx = (float)x / vp->getActualWidth();
	float ty = (float)y / vp->getActualHeight();

	Ray ray = cam->getCameraToViewportRay(tx, ty);
	
	TerrainGroup::RayResult re =
		GameApp::getSingleton().GetGame()->RayIntersects(ray);
	if(re.hit)
	{
		EventsManager::getSingleton().CreateEvent("PickOnTerrain", Any(re.position));
	}
	return re;
}
//--------------------------------------------------------------------------------
void EditorApp::SetGame( GeneralGame* game )
{
	_game = game;
}
//--------------------------------------------------------------------------------
void EditorApp::Shutdown()
{
	GameApp::getSingleton().RemoveAppListener(this);

	RemoveAllMemeto();
	if(_terrainRayQuery)
	{
		GameApp::getSingleton().GetGame()->GetVisualScene()->destroyQuery(_terrainRayQuery);
	}

	EventsManager::getSingleton().RemoveAll();

	UnloadAllPlugins();
}
//--------------------------------------------------------------------------------
SPickGameObjectResult EditorApp::PickGameObjects( int x, int y,
												 uint32 queryMask /*= MovableObject::getDefaultQueryFlags()*/,
												 uint32 queryTypeMask,
												 Camera* cam /*= 0*/ )
{
	SPickGameObjectResult r;

	if(!cam)
	{
		cam = GetActiveCamera();
	}

	Viewport* vp = cam->getViewport();
	float tx = (float)x / vp->getActualWidth();
	float ty = (float)y / vp->getActualHeight();

	Ray ray = cam->getCameraToViewportRay(tx, ty);
	_terrainRayQuery->setRay(ray);

	_terrainRayQuery->setQueryMask(queryMask);

	// Make light/billboard pickable
	_terrainRayQuery->setQueryTypeMask(queryTypeMask);

	r.RawResult =  _terrainRayQuery->execute();

	if (!r.RawResult.empty())
	{
		// At this point we have raycast to a series of different objects bounding boxes.
		// we need to test these different objects to see which is the first polygon hit.
		// there are some minor optimizations (distance based) that mean we wont have to
		// check all of the objects most of the time, but the worst case scenario is that
		// we need to test every triangle of every object.
		Real closest_distance = -1.0f;
		//RaySceneQueryResult &query_result = _rayQuery->getLastResults();
		for (size_t qr_idx = 0; qr_idx < r.RawResult.size(); qr_idx++)
		{
			// stop checking if we have found a raycast hit that is closer than all remaining entities
			if ((closest_distance >= 0.0f) && (closest_distance < r.RawResult[qr_idx].distance))
				break;

			r.RawObject = r.RawResult[qr_idx].movable;

			if ((r.RawObject != NULL) &&
				(r.RawObject->getMovableType().compare("Entity") == 0))  //Get Movableobject triangle
			{
 				if(!r.RawObject->getUserAny().isEmpty())
 				{
					r.GameObject = any_cast<AnyGameObject>(r.RawObject->getUserAny()).GameObject;
 				}

				if(r.GameObject == 0) // try find by name
				{
					r.GameObject = _game->GetGameObject(XGUID(r.RawObject->getName()));
				}


				// Only check this result if its a hit against an entity 
				// Get the entity to check
				Entity *pentity = dynamic_cast<Entity*>(r.RawObject);           

				// mesh data to retrieve         
				size_t vertex_count;
				size_t index_count;
				Vector3* vertices = 0;
				unsigned long* indices = 0;

				// get the mesh information
				GetMeshInformation(pentity->getMesh(), vertex_count, vertices, index_count, indices,             
					pentity->getParentNode()->_getDerivedPosition(),
					pentity->getParentNode()->_getDerivedOrientation(),
					pentity->getParentNode()->_getDerivedScale());

				// test for hitting individual triangles on the mesh
				bool new_closest_found = false;
				for (int i = 0; i < static_cast<int>(index_count); i += 3)
				{
					// check for a hit against this triangle
					std::pair<bool, Real> hit = Math::intersects(ray, vertices[indices[i]],
						vertices[indices[i+1]], vertices[indices[i+2]], true, false);

					// if it was a hit check if its the closest
					if (hit.first)
					{
						if ((closest_distance < 0.0f) || (hit.second < closest_distance))
						{
							// this is the closest so far, save it off
							closest_distance = hit.second;
							new_closest_found = true;
						}
					}
				}

				// free the verticies and indicies memory
				OGE_FREE(vertices);
				OGE_FREE(indices);

				// if we found a new closest raycast for this object,
				// update the IntersectPoint before moving on to the next object.
				if (new_closest_found)
				{
					r.IntersectPoint = ray.getPoint(closest_distance);           
					goto e_RaycastOver;
				}
			}
			else if((r.RawObject != NULL) &&
				(!r.RawObject->getUserAny().isEmpty()))
			{
				r.RawObject = r.RawObject;
 				if(!r.RawObject->getUserAny().isEmpty())
 				{
					r.GameObject = any_cast<AnyGameObject>(r.RawObject->getUserAny()).GameObject;
 				}

				if(r.GameObject == 0) // try find by name
				{
					r.GameObject = _game->GetGameObject(XGUID(r.RawObject->getName()));
				}
				r.IntersectPoint = ray.getPoint(r.RawResult[qr_idx].distance);
				goto e_RaycastOver;
			}
			else if((r.RawObject != NULL) &&
				(r.RawObject->getMovableType().compare("SimpleRenderable") == 0))
			{
				r.RawObject = r.RawObject;
 				if(!r.RawObject->getUserAny().isEmpty())
 				{
					r.GameObject = any_cast<AnyGameObject>(r.RawObject->getUserAny()).GameObject;
 				}
 
				if(r.GameObject == 0) // try find by name
				{
					r.GameObject = _game->GetGameObject(XGUID(r.RawObject->getName()));
				}


				// Only check this result if its a hit against an entity 
				// Get the entity to check
				SimpleRenderable* psr = dynamic_cast<SimpleRenderable*>(r.RawObject);           

				// mesh data to retrieve         
				size_t vertex_count;
				size_t index_count;
				Vector3* vertices = 0;
				unsigned long* indices = 0;

				RenderOperation op;
				psr->getRenderOperation(op);
				if(op.indexData)
				{
					// get the mesh information
					GetMeshInformation(op.vertexData, op.indexData, vertex_count, vertices, index_count, indices,             
						psr->getParentNode()->_getDerivedPosition(),
						psr->getParentNode()->_getDerivedOrientation(),
						psr->getParentNode()->_getDerivedScale());

					// test for hitting individual triangles on the mesh
					bool new_closest_found = false;
					for (int i = 0; i < static_cast<int>(index_count); i += 3)
					{
						// check for a hit against this triangle
						std::pair<bool, Real> hit = Math::intersects(ray, vertices[indices[i]],
							vertices[indices[i+1]], vertices[indices[i+2]], true, false);

						// if it was a hit check if its the closest
						if (hit.first)
						{
							if ((closest_distance < 0.0f) || (hit.second < closest_distance))
							{
								// this is the closest so far, save it off
								closest_distance = hit.second;
								new_closest_found = true;
							}
						}
					}

					// free the verticies and indicies memory
					OGE_FREE(vertices);
					OGE_FREE(indices);

					// if we found a new closest raycast for this object,
					// update the IntersectPoint before moving on to the next object.
					if (new_closest_found)
					{
						r.IntersectPoint = ray.getPoint(closest_distance);           
						goto e_RaycastOver;
					}
				}
			}			
			else // Intersect with Terrain
			{
				for (RaySceneQueryResult::iterator i = r.RawResult.begin(); i < r.RawResult.end(); i++)
				{
					if (i != r.RawResult.end() && i->worldFragment)
					{
						r.IntersectPoint = i->worldFragment->singleIntersection;
						r.GameObject = 0;
						goto e_RaycastOver;
					}
				}
			}
		}
	}


e_RaycastOver:

	return r;
}
//--------------------------------------------------------------------------------
void EditorApp::SetCursor( HCURSOR cur )
{
	CFrameWndEx* pMainWnd = dynamic_cast<CFrameWndEx*>(AfxGetMainWnd());
	CView* mainView = dynamic_cast<CView*>(pMainWnd->GetActiveView());
	
	SetClassLong(pMainWnd->GetSafeHwnd(), GCL_HCURSOR, (LONG)cur);
	SetClassLong(mainView->GetSafeHwnd(), GCL_HCURSOR, (LONG)cur);
	::SetCursor(cur);

}
//--------------------------------------------------------------------------------
void EditorApp::OnPostGameInit()
{
	if(!_terrainRayQuery)
	{
		Ray ray;
		_terrainRayQuery = GameApp::getSingleton().GetGame()->GetVisualScene()->createRayQuery(ray);
		_terrainRayQuery->setSortByDistance(true);
	}
}
//--------------------------------------------------------------------------------
void EditorApp::NotifyMapCreated( const String& mapName )
{
	EditorPluginList::iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		EditorPlugin* ep = *i;
		ep->onMapCreate(mapName);
	}
}
//--------------------------------------------------------------------------------
void EditorApp::NotifyMapLoad( const String& mapName )
{
	EditorPluginList::iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		EditorPlugin* ep = *i;
		ep->onMapLoad(mapName);
	}
}
//--------------------------------------------------------------------------------
void EditorApp::NotifyMapUnload( const String& mapName )
{
	EditorPluginList::iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		EditorPlugin* ep = *i;
		ep->onMapUnload(mapName);
	}
}
//--------------------------------------------------------------------------------
void EditorApp::NotifyMapSave( const String& mapName )
{
	EditorPluginList::iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		EditorPlugin* ep = *i;
		ep->onMapSave(mapName);
	}
}
//--------------------------------------------------------------------------------
void EditorApp::NotifyCreateDockPanel( CFrameWndEx* mainWnd, CDockablePane* leftRoot,
									  CDockablePane* rightRoot, CDockablePane* bottomRoot )
{
	EditorPluginList::iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		EditorPlugin* ep = *i;
		ep->onCreateDockPanel(mainWnd, leftRoot, rightRoot, bottomRoot);
	}
}
//--------------------------------------------------------------------------------
void EditorApp::NotifyCreateToolsCategory( CMFCRibbonCategory* cat, CMFCRibbonPanel* mainPanel )
{
	EditorPluginList::iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		EditorPlugin* ep = *i;
		ep->onCreateToolsCategory(cat, mainPanel);
	}
}
//--------------------------------------------------------------------------------
void EditorApp::NotifyCreateGlobalObjectsCategory( CMFCRibbonCategory* cat, CMFCRibbonPanel* mainPanel )
{
	EditorPluginList::iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		EditorPlugin* ep = *i;
		ep->onCreateGlobalObjects(cat, mainPanel);
	}
}
//--------------------------------------------------------------------------------
void EditorApp::NotifyPostCreateEditStateCategory( CMFCRibbonCategory* cat )
{
	EditorPluginList::iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		EditorPlugin* ep = *i;
		ep->onPostCreateEditStateCategory(cat);
	}
}
//--------------------------------------------------------------------------------
BOOL EditorApp::NotifyWndMsg( UINT nID, int nCode, void* pExtra,
							 AFX_CMDHANDLERINFO* pHandlerInfo )
{
	CmdTargetList::iterator i  = _handlerLst.begin();
	for (; i != _handlerLst.end(); ++i)
	{
		if((*i)->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) == TRUE)
		{
			return TRUE;
		}
	}

	return FALSE;
}
//--------------------------------------------------------------------------------
void EditorApp::RegisterWMHandler( CCmdTarget* h )
{
	_handlerLst.push_back(h);
}
//--------------------------------------------------------------------------------
void EditorApp::UnRegisterWMHandler( CCmdTarget* h )
{
	CmdTargetList::iterator i = std::find(_handlerLst.begin(),
		_handlerLst.end(), h);

	if(i != _handlerLst.end())
	{
		_handlerLst.erase(i);
	}
}
//--------------------------------------------------------------------------------
int EditorApp::GetActiveStateCommandID() const
{
	return _game->GetStateManager().GetActiveStateCommandID();
}