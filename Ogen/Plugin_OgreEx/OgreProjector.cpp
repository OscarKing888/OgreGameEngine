#include "stdafx.h"
#include "OgreProjector.h"
#include <OgreTerrainGroup.h>
#include <OgreMaterialManager.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreWireBoundingBox.h>
#include <OgreSubMesh.h>

#ifdef _DEBUG
#	pragma comment(lib, "OgreTerrain_d.lib")
#else
#	pragma comment(lib, "OgreTerrain.lib")
#endif

namespace Ogre
{
	class ProjectorRenderable : public Ogre::Renderable, public Ogre::GeometryAllocatedObject
	{
	protected:
		Projector* mParent;

	public:
		Ogre::Entity* mEntity;
		Ogre::RenderOperation mRenderOp;

	public:
		ProjectorRenderable::ProjectorRenderable(Projector *parent)
			: Renderable()
			, mParent(parent)
			, mEntity(0)
			, mRenderOp()
		{
			mRenderOp.vertexData = OGRE_NEW Ogre::VertexData;
			mRenderOp.useIndexes = false;

			Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
			decl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
		}
		//-----------------------------------------------------------------------
		ProjectorRenderable::~ProjectorRenderable()
		{
			OGRE_DELETE mRenderOp.vertexData;

			if (mRenderOp.indexData)
			{
				OGRE_DELETE mRenderOp.indexData;
			}
		}
		//-----------------------------------------------------------------------
		/** Overridden - see Renderable. */
		const Ogre::MaterialPtr& getMaterial(void) const
		{
			return mParent->mPatchMaterial;
		}
		//-----------------------------------------------------------------------
		/** Overridden - see Renderable. */
		void getRenderOperation(Ogre::RenderOperation& op)
		{
			op = mRenderOp;
		}
		//-----------------------------------------------------------------------
		/** Overridden - see Renderable. */
		void getWorldTransforms(Ogre::Matrix4* xform) const
		{
			if (mEntity)
			{
				*xform = mEntity->_getParentNodeFullTransform();
			}
			else
			{
				*xform = Ogre::Matrix4::IDENTITY;
			}
		}
		//-----------------------------------------------------------------------
		/** Overridden - see Renderable. */
		Ogre::Real getSquaredViewDepth(const Ogre::Camera* camera) const
		{
			Ogre::Node* n = mParent->getParentNode();
			assert(n);
			return n->getSquaredViewDepth(camera);
		}
		//-----------------------------------------------------------------------
		/** Overridden - see Renderable. */
		const Ogre::LightList& getLights(void) const
		{
			return mParent->queryLights();
		}
		//-----------------------------------------------------------------------
	};

	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	Projector::Projector(const String& name)
		: Ogre::Frustum()
		, mTerrain()
		, mTerrainGridLimit(256)
		, mQueryMask(~0)
		, mQueryTypeMask(~0)
		, mVertexLimit(4096)
		, mVertexBuffer()
		, mRenderables()
		, mPatchMaterial()
		, mPatchMaterialName()
		, mPatchMaterialDirty(false)
		, mShowFrustum()
		, mEnableClipPlanes()
		, mBoundingBoxEntity(0)
		, mShowBoundingBox(false)
	{
		// Overriden Frustum default
		mVisible = true;

		// Disable cast shadows by default
		mCastShadows = false;

		// MovableObject name
		mName = name;

		// default type mask to everything except lights & fx (previous behaviour)
		mQueryTypeMask = ~(Ogre::SceneManager::FX_TYPE_MASK  | Ogre::SceneManager::LIGHT_TYPE_MASK);

		// default query mask set to decal target only
		mQueryMask = MovableObject::getDefaultQueryFlags();

		setProjectionType(PT_ORTHOGRAPHIC);
		setFOVy(Radian(Math::PI));
		setNearClipDistance(10);
		setFarClipDistance(1000.0f);
		setOrthoWindowHeight(100);
		setAspectRatio(1.0f);
		setEnableClipPlanes(false);
		setCastShadows(false);
		setPatchMaterial("Core/Projector");
	}
	//-----------------------------------------------------------------------
	Projector::~Projector()
	{
		if (!mPatchMaterial.isNull())
		{
			Ogre::MaterialManager::getSingleton().remove(mPatchMaterial->getHandle());
			mPatchMaterial.setNull();
		}

		if(mBoundingBoxEntity)
		{
			OGRE_DELETE mBoundingBoxEntity;
			mBoundingBoxEntity = 0;
		}

		_clearRenderables();
	}
	//-----------------------------------------------------------------------
	void Projector::_clearRenderables(void)
	{
		_freeRenderables();

		for (ProjectorRenderableList::const_iterator it = mFreeRenderables.begin(); it != mFreeRenderables.end(); ++it)
		{
			OGRE_DELETE *it;
		}
		mFreeRenderables.clear();
	}
	//-----------------------------------------------------------------------
	void Projector::_freeRenderables(void)
	{
		// Unref all hardware buffers
		for (ProjectorRenderableList::const_iterator it = mRenderables.begin(); it != mRenderables.end(); ++it)
		{
			ProjectorRenderable* pr = *it;
			if (pr->mRenderOp.vertexData)
			{
				pr->mRenderOp.vertexData->vertexBufferBinding->unsetAllBindings();
			}

			if (pr->mRenderOp.indexData)
			{
				pr->mRenderOp.indexData->indexBuffer.setNull();
			}
		}

		mFreeRenderables.splice(mFreeRenderables.end(), mRenderables);
	}
	//-----------------------------------------------------------------------
	ProjectorRenderable* Projector::_allocRenderable(Ogre::Entity* entity)
	{
		ProjectorRenderable* result;

		if (mFreeRenderables.empty())
		{
			result = OGRE_NEW ProjectorRenderable(this);
			mRenderables.push_back(result);
		}
		else
		{
			result = mFreeRenderables.front();
			mRenderables.splice(mRenderables.end(), mFreeRenderables, mFreeRenderables.begin());
		}

		result->mEntity = entity;
		return result;
	}
	//-----------------------------------------------------------------------
	void Projector::_addRenderable(Ogre::Entity* entity, size_t vertexStart, size_t vertexCount)
	{
		if (vertexCount > 0)
		{
			ProjectorRenderable* result = _allocRenderable(entity);
			result->mRenderOp.vertexData->vertexStart = vertexStart;
			result->mRenderOp.vertexData->vertexCount = vertexCount;
		}
	}
	//-----------------------------------------------------------------------
	void Projector::setShowFrustum(bool showFrustum)
	{
		mShowFrustum = showFrustum;
	}
	//-----------------------------------------------------------------------
	bool Projector::getShowFrustum(void) const
	{
		return mShowFrustum;
	}
	//-----------------------------------------------------------------------
	void Projector::setEnableClipPlanes(bool enableClipPlanes)
	{
		mEnableClipPlanes = enableClipPlanes;
	}
	//-----------------------------------------------------------------------
	bool Projector::getEnableClipPlanes(void) const
	{
		return mEnableClipPlanes;
	}
	//-----------------------------------------------------------------------
	void Projector::setPatchMaterial(const String& materialName)
	{
		mPatchMaterialName = materialName;
		mPatchMaterialDirty = true;
	}
	//-----------------------------------------------------------------------
	const String& Projector::getPatchMaterialName(void) const
	{
		return mPatchMaterialName;
	}
	//-----------------------------------------------------------------------
	const Ogre::MaterialPtr& Projector::_getPatchMaterial(void)
	{
		_updateMaterial();
		return mPatchMaterial;
	}
	//-----------------------------------------------------------------------
	void Projector::setTerrain(TerrainGroup* terrain)
	{
		mTerrain = terrain;
	}
	//-----------------------------------------------------------------------
	TerrainGroup* Projector::getTerrain(void) const
	{
		return mTerrain;
	}
	//-----------------------------------------------------------------------
	void Projector::setTerrainGridLimit(int limit)
	{
		assert(limit > 0);
		mTerrainGridLimit = limit;
	}
	//-----------------------------------------------------------------------
	int Projector::getTerrainGridLimit(void) const
	{
		return mTerrainGridLimit;
	}
	//-----------------------------------------------------------------------
	void Projector::setVertexLimit(size_t limit)
	{
		mVertexLimit = limit;
	}
	//-----------------------------------------------------------------------
	size_t Projector::getVertexLimit(void) const
	{
		return mVertexLimit;
	}
	//-----------------------------------------------------------------------
	void Projector::setQueryMask(uint32 mask)
	{
		mQueryMask = mask;
	}
	//-----------------------------------------------------------------------
	uint32 Projector::getQueryMask(void) const
	{
		return mQueryMask;
	}
	//-----------------------------------------------------------------------
	void Projector::setQueryTypeMask(uint32 mask)
	{
		mQueryTypeMask = mask;
	}
	//-----------------------------------------------------------------------
	uint32 Projector::getQueryTypeMask(void) const
	{
		return mQueryTypeMask;
	}
	//-----------------------------------------------------------------------
	const String& Projector::getMovableType(void) const
	{
		return ProjectorFactory::FACTORY_TYPE_NAME;
	}
	//-----------------------------------------------------------------------
	bool Projector::_updateMaterial(void)
	{
		if (mPatchMaterialDirty)
		{
			if (!mPatchMaterial.isNull())
			{
				Ogre::MaterialManager::getSingleton().remove(mPatchMaterial->getHandle());
				mPatchMaterial.setNull();
			}

			if (!mPatchMaterialName.empty())
			{
				mPatchMaterial = createProjectiveMaterial(mPatchMaterialName, this);
			}

			mPatchMaterialDirty = false;
		}

		return !mPatchMaterial.isNull();
	}
	//-----------------------------------------------------------------------
	void Projector::_notifyCurrentCamera(Ogre::Camera* camera)
	{
		mCurrentCamera = camera;

		Frustum::_notifyCurrentCamera(camera);
	}
	//-----------------------------------------------------------------------
	void Projector::_updateRenderQueue(Ogre::RenderQueue* queue)
	{
		if (mShowFrustum)
		{
			if (mRenderQueueIDSet)
			{
				queue->addRenderable(this, mRenderQueueID);
			}
			else
			{
				queue->addRenderable(this);
			}
		}

		if(mShowBoundingBox)
		{
			if(!mBoundingBoxEntity)
			{
				mBoundingBoxEntity = OGRE_NEW WireBoundingBox();
			}

			AxisAlignedBox aabb = getAABB();
			mBoundingBoxEntity->setupBoundingBox(aabb);

			if (mRenderQueueIDSet)
			{
				queue->addRenderable(mBoundingBoxEntity, mRenderQueueID);
			}
			else
			{
				queue->addRenderable(mBoundingBoxEntity);
			}
		}

		if (!_updateMaterial())
		{
			return;
		}

		_freeRenderables();
		mTempPositions.clear();

		_populateForTerrain();
		_populateForMovables();

		if (mTempPositions.empty() || mRenderables.empty())
		{
			return;
		}

		//////////////////////////////////////////////////////////////////////////

		_buildVertexBuffer();

		// Add to render queue
		for (ProjectorRenderableList::const_iterator it = mRenderables.begin(); it != mRenderables.end(); ++it)
		{
			ProjectorRenderable* pr = *it;

			// Re-bound vertex buffer
			pr->mRenderOp.vertexData->vertexBufferBinding->setBinding(0, mVertexBuffer);

			Ogre::uint8 groupId;
			if (mRenderQueueIDSet)
			{
				groupId = mRenderQueueID;
			}
			else if (pr->mEntity)
			{
				// Hack to access MovableObject::mRenderQueueIDSet
				if (((Projector*)pr->mEntity)->mRenderQueueIDSet)
				{
					groupId = pr->mEntity->getRenderQueueGroup();
				}
				else
				{
					groupId = queue->getDefaultQueueGroup();
				}
			}
			else if(TerrainGlobalOptions::getSingletonPtr())
			{
				// Yes, from Ogre1.71 on, new terrain is rendering with RENDER_QUEUE_MAIN
				groupId = TerrainGlobalOptions::getSingleton().getRenderQueueGroup();
			}
			else
			{
				// Default to the render queue that the world geometry associated with.
				groupId = getParentSceneNode()->getCreator()->getWorldGeometryRenderQueue();
			}

			// Render later ensure the projective texture would be override
			// the basic geometry even if depth check is enabled.
			queue->addRenderable(pr, groupId, queue->getDefaultRenderablePriority() + 10);
		}
	}
	//-----------------------------------------------------------------------
	void Projector::_buildVertexBuffer(void)
	{
		size_t numVertices = mTempPositions.size();

		// Check vertex buffer fit
		if (mVertexBuffer.isNull() ||
			mVertexBuffer->getNumVertices() < numVertices)
		{
			// Release vertex buffer first
			mVertexBuffer.setNull();
			// Allocate vertex buffer
			mVertexBuffer =
				Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				3 * sizeof(float),
				numVertices,
				Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
		}

		// Fill up vertex buffer
#if !OGRE_DOUBLE_PRECISION
		mVertexBuffer->writeData(0, 3 * sizeof(float) * numVertices, &mTempPositions[0], true);
#else
		float* pFloat = static_cast<float*>(mVertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
		vector<Ogre::Vector3>::type::const_iterator itPosition = mTempPositions.begin();
		for (size_t i = 0; i < numVertices; ++i, ++itPosition)
		{
			*pFloat++ = itPosition->x;
			*pFloat++ = itPosition->y;
			*pFloat++ = itPosition->z;
		}
		mVertexBuffer->unlock();
#endif
	}
	//-----------------------------------------------------------------------
	void Projector::_populateForTerrain(void)
	{
		if (!mTerrain)
			return;

		// Check vertex limit
		if (mTempPositions.size() >= mVertexLimit)
			return;

		TerrainGroup::TerrainList tlst;
		AxisAlignedBox aabb = getAABB();

		mTerrain->boxIntersects(aabb, &tlst);
		if(!tlst.empty())
		{
			Vector3 minPos = aabb.getMinimum();
			Vector3 maxPos = aabb.getMaximum();

			if(this->getProjectionType() == Ogre::PT_ORTHOGRAPHIC)
			{
				Vector3 pos = mParentNode->_getDerivedPosition();
				minPos = pos + Vector3(-mOrthoHeight, 0, -mOrthoHeight);
				maxPos = pos + Vector3(mOrthoHeight, 0, mOrthoHeight);
			}

			float cellSize = 20.0f; // 20 cm

			float minX = minPos.x;
			float minZ = minPos.z;
			float maxX = maxPos.x;
			float maxZ = maxPos.z;
			float y = maxPos.y;

			int cx = (int)Math::Ceil((maxX - minX) / cellSize);
			int cz = (int)Math::Ceil((maxZ - minZ) / cellSize);

			size_t vertexStart = mTempPositions.size();
			size_t vertexCount = 0;

			// X+0,Z+0     X+1,Z+0

			// 0+------+1
			//	|\     |
			//	| \    |
			//	|  \   |
			//	|   \  |
			//	|    \ |
			//	|     \|
			// 3+------+2

			//	X+0,Z+1     X+1,Z+1

			for (int x = -1; x < cx; ++x)
			{
				float xpos = minX + (float)x * cellSize;
				float xpos1 = minX + (float)(x + 1) * cellSize;

				for (int z = -1; z < cz; ++z)
				{
					float zpos = minZ + (float)z * cellSize;
					float zpos1 = minZ + (float)(z + 1) * cellSize;


					float h0 = mTerrain->getHeightAtWorldPosition(xpos, y, zpos);
					float h1 = mTerrain->getHeightAtWorldPosition(xpos1, y, zpos);
					float h2 = mTerrain->getHeightAtWorldPosition(xpos1, y, zpos1);
					float h3 = mTerrain->getHeightAtWorldPosition(xpos, y, zpos1);

					Vector3 p0(xpos, h0, zpos);
					Vector3 p1(xpos1, h1, zpos);
					Vector3 p2(xpos1, h2, zpos1);
					Vector3 p3(xpos, h3, zpos1);

					mTempPositions.push_back(p0);
					mTempPositions.push_back(p3);
					mTempPositions.push_back(p2);

					mTempPositions.push_back(p0);
					mTempPositions.push_back(p2);
					mTempPositions.push_back(p1);

					vertexCount += 6;
				}
			}

			if(vertexCount > 0)
			{
				_addRenderable(NULL, vertexStart, vertexCount);
			}
		}
	}
	//-----------------------------------------------------------------------
	void Projector::_populateForMovables(void)
	{
		// Shortcut to ignore all movables
		if (!mQueryMask || !mQueryTypeMask)
			return;

		// Check vertex limit
		if (mTempPositions.size() >= mVertexLimit)
			return;

		Ogre::SceneNode* sn = this->getParentSceneNode(); assert(sn);
		Ogre::SceneManager* sm = sn->getCreator(); assert(sm);

		AxisAlignedBox aabb = getAABB();

		Ogre::PlaneBoundedVolumeListSceneQuery* query = sm->createPlaneBoundedVolumeQuery(Ogre::PlaneBoundedVolumeList());
		//Ogre::AxisAlignedBoxSceneQuery* query = sm->createAABBQuery(aabb);
		{
			Ogre::PlaneBoundedVolumeList volumes;
			Ogre::PlaneBoundedVolume volume;
			const Ogre::Plane* frustumPlanes;

			// Pre-allocate spaces
			volume.planes.reserve(6 * 2);

			// Inject our clip planes
			frustumPlanes = this->getFrustumPlanes();
			volume.planes.insert(volume.planes.end(), frustumPlanes, frustumPlanes + 6);

			// Setup renderable clip planes if need
			if (mEnableClipPlanes)
			{
				mClipPlanes = volume.planes;
			}

			// Inject camera's clip planes
			frustumPlanes = mCurrentCamera->getFrustumPlanes();
			volume.planes.insert(volume.planes.end(), frustumPlanes, frustumPlanes + 6);

			// Construct volumes and query
			volumes.push_back(volume);
			query->setVolumes(volumes);
		}

		// Cache the view-projection matrix
		Ogre::Matrix4 viewProjMatrix = this->getProjectionMatrix() * this->getViewMatrix();

		// NOTE:
		//query->setQueryMask(mQueryMask);
		//query->setQueryTypeMask(mQueryTypeMask);
		Ogre::SceneQueryResult& result = query->execute();
		for (Ogre::SceneQueryResultMovableList::const_iterator it = result.movables.begin(); it != result.movables.end(); ++it)
		{
			Ogre::MovableObject* movable = *it;
			if (!(movable->_notifyCurrentCamera(mCurrentCamera), movable->isVisible()))
				continue;

			if (movable->getMovableType() == Ogre::EntityFactory::FACTORY_TYPE_NAME)
			{
				Ogre::Entity* entity = static_cast<Ogre::Entity*>(movable);
				if (!entity->hasSkeleton() && !entity->hasVertexAnimation())
				{
					// Static entity

					const Ogre::MeshPtr& mesh = entity->getMesh();
					// Get world matrix
					Ogre::Matrix4 worldMatrix = entity->_getParentNodeFullTransform();
					// Compute world-view-projection matrix
					Ogre::Matrix4 worldViewProjMatrix = viewProjMatrix * worldMatrix;
					// Get the collision mode

					// Get the view vector base on projection type
					Ogre::Vector4 vv;
					Ogre::Matrix4 eyeToModel = (this->getViewMatrix() * worldMatrix).inverse();
					if (this->getProjectionType() == Ogre::PT_PERSPECTIVE)
					{
						// Use view position as view vector
						vv[0] = eyeToModel[0][3];
						vv[1] = eyeToModel[1][3];
						vv[2] = eyeToModel[2][3];
						vv[3] = eyeToModel[3][3];
					}
					else
					{
						// Use view direction as view vector
						vv[0] = eyeToModel[0][2];
						vv[1] = eyeToModel[1][2];
						vv[2] = eyeToModel[2][2];
						vv[3] = eyeToModel[3][2];
					}

					uint numTriangles = 0;
					size_t vertexStart = mTempPositions.size();
					size_t vertexCount = 0;


					for (ushort i = 0; i < mesh->getNumSubMeshes(); ++i)
					{
						SubMesh* sm = mesh->getSubMesh(i);

						const VertexData* vd = sm->vertexData;
						//vertexCount += vd->vertexCount * 2;

						VertexDeclaration* vdec = vd->vertexDeclaration;			
						const VertexElement* vePos = vdec->findElementBySemantic(VES_POSITION);
						const VertexElement* veNm = vdec->findElementBySemantic(VES_NORMAL);

						ushort sidx = vePos->getSource();
						HardwareVertexBufferSharedPtr vbuf = vd->vertexBufferBinding->getBuffer(sidx);
						unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(HardwareBuffer::HBL_READ_ONLY));

						const IndexData* idx = sm->indexData;
						ushort* index = static_cast<ushort*>(idx->indexBuffer->lock(HardwareBuffer::HBL_READ_ONLY));
						size_t startIdx = idx->indexStart;
						numTriangles += idx->indexCount / 3;

						for(size_t i = startIdx; i < idx->indexCount; i += 3)
						{
							Vector3 p1, p2, p3;
							float* pFloat = 0;
							ushort idxVal = index[i + 0];
							vePos->baseVertexPointerToElement(vertex + vbuf->getVertexSize() * idxVal, &pFloat);
							p1.x = (*pFloat++);
							p1.y = (*pFloat++);
							p1.z = (*pFloat++);

							idxVal = index[i + 1];
							vePos->baseVertexPointerToElement(vertex + vbuf->getVertexSize() * idxVal, &pFloat);
							p2.x = (*pFloat++);
							p2.y = (*pFloat++);
							p2.z = (*pFloat++);

							idxVal = index[i + 2];
							vePos->baseVertexPointerToElement(vertex + vbuf->getVertexSize() * idxVal, &pFloat);
							p3.x = (*pFloat++);
							p3.y = (*pFloat++);
							p3.z = (*pFloat++);

							// TODO: Check the triangle is projector facing
							//if (faceNormal.dotProduct(vv) > 0)
							//{
							//}
							mTempPositions.push_back(p1);
							mTempPositions.push_back(p2);
							mTempPositions.push_back(p3);

							vertexCount += 3;
						}

						idx->indexBuffer->unlock();
						vbuf->unlock();
					}

					// Check vertex limit
					assert(vertexStart <= mVertexLimit);
					if (mVertexLimit <= vertexStart)
						break;  // Just for safely

					// Check overrun
					bool overrun = vertexStart + vertexCount > mVertexLimit;
					if (overrun)
					{
						// discard overrun vertices
						vertexCount = (mVertexLimit - vertexStart) / 3 * 3;
						//mTempPositions.resize(vertexStart + vertexCount);
					}

					_addRenderable(entity, vertexStart, vertexCount);

					// No more process because vertices overrun
					if (overrun)
						break;
				}
				else
				{
					// Animation entity
					// TODO:
				}
			}
		}

		sm->destroyQuery(query);
	}

	Ogre::AxisAlignedBox Projector::getAABB() const
	{
		AxisAlignedBox aabb = getBoundingBox();
		if(mParentNode)
		{
			aabb.transformAffine(mParentNode->_getFullTransform());
		}

		return aabb;
	}

	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	const String ProjectorFactory::FACTORY_TYPE_NAME = "Projector";
	//-----------------------------------------------------------------------
	const String& ProjectorFactory::getType(void) const
	{
		return FACTORY_TYPE_NAME;
	}
	//-----------------------------------------------------------------------
	Ogre::MovableObject* ProjectorFactory::createInstanceImpl(const String& name,
		const Ogre::NameValuePairList* params)
	{
		return OGRE_NEW Projector(name);
	}
	//-----------------------------------------------------------------------
	void ProjectorFactory::destroyInstance(Ogre::MovableObject* obj)
	{
		delete obj;
	}
	//--------------------------------------------------------------------------------
	MaterialPtr createProjectiveMaterial(const String& materialTemplateName, const Frustum* frustum)
	{
		if (materialTemplateName.empty())
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"You must give a material template name for create projective material.",
				"createProjectiveMaterial");
		}

		// Build the material from template
		MaterialPtr material = MaterialManager::getSingleton().getByName(materialTemplateName);
		if (material.isNull())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
				"Can't found material with name " + materialTemplateName +
				"Have you forgotten to define it in a  .material script?",
				"createProjectiveMaterial");
		}

		material = material->clone(material->getName() + "/Projectived/" + frustum->getName());
		fixupProjectiveMaterial(material, frustum);

		return material;
	}
	//--------------------------------------------------------------------------------
	void fixupProjectiveMaterial(const MaterialPtr& material, const Frustum* frustum)
	{
		// apply texture projective to each texture unit state
		Material::TechniqueIterator ti = material->getTechniqueIterator();
		while (ti.hasMoreElements())
		{
			Technique* technique = ti.getNext();
			Technique::PassIterator pi = technique->getPassIterator();
			while (pi.hasMoreElements())
			{
				Pass* pass = pi.getNext();
				Pass::TextureUnitStateIterator tusi = pass->getTextureUnitStateIterator();
				while (tusi.hasMoreElements())
				{
					TextureUnitState* tus = tusi.getNext();
					// Set projector only if texture addressing mode is clamp and doesn't
					// exists any texture effect that conflict with projective texturing
#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
					const TextureUnitState::UVWAddressingMode& am = tus->getTextureAddressingMode();
					if (am.u == tus->TAM_CLAMP || am.u == tus->TAM_BORDER &&
						am.v == tus->TAM_CLAMP || am.v == tus->TAM_BORDER &&
						am.w == tus->TAM_CLAMP || am.w == tus->TAM_BORDER)
#else
					if (tus->getTextureAddressingMode() == tus->TAM_CLAMP)
#endif
					{						
						const TextureUnitState::EffectMap& effects = tus->getEffects();
						if (effects.find(tus->ET_ENVIRONMENT_MAP) == effects.end())
						{
							tus->setProjectiveTexturing(true, frustum);
						}
					}
				}
			}
		}
	}
} // namespace
