#include "stdafx.h"
#include "TransformedMesh.h"





	//--------------------------------------------------------------------------------
	// TransformedSubMesh
	//--------------------------------------------------------------------------------
	TransformedSubMesh::TransformedSubMesh(TransformedMesh* parent, SubMesh* subMesh)
		: _subMesh(subMesh)
		, _parent(parent)
	{
		_material = MaterialManager::getSingleton().getByName(_subMesh->getMaterialName());
	}
	//--------------------------------------------------------------------------------
	TransformedSubMesh::~TransformedSubMesh()
	{
	}
	//--------------------------------------------------------------------------------
	const MaterialPtr& TransformedSubMesh::getMaterial(void) const
	{
		return _material;
	}
	//--------------------------------------------------------------------------------
	void TransformedSubMesh::getWorldTransforms( Matrix4* xform ) const
	{
		Matrix4 m;
		m.makeTransform(
			_parent->getPosition(),
			_parent->getScale(),
			_parent->getOrientation());
		*xform = m;
	}
	//--------------------------------------------------------------------------------
	Real TransformedSubMesh::getSquaredViewDepth(const Camera* cam) const
	{
		Vector3 vMin, vMax, vMid, vDist;
		AxisAlignedBox aabb = _parent->getBoundingBox();
		vMin = aabb.getMinimum();
		vMax = aabb.getMaximum();
		vMid = ((vMin - vMax) * 0.5) + vMin;
		vDist = cam->getDerivedPosition() - vMid;

		return vDist.squaredLength();
	}
	//--------------------------------------------------------------------------------
	const LightList& TransformedSubMesh::getLights(void) const
	{
		// Use movable query lights
		return _parent->queryLights();
	}
	//--------------------------------------------------------------------------------
	void TransformedSubMesh::getRenderOperation(RenderOperation& op)
	{
		_subMesh->_getRenderOperation(op);
	}

	//--------------------------------------------------------------------------------
	// TransformedMesh
	//--------------------------------------------------------------------------------
	TransformedMesh::TransformedMesh(const String& name, MeshPtr mesh)
		: MovableObject(name)
		, _mesh(mesh)
		, _position(Vector3::ZERO)
		, _scale(Vector3::UNIT_SCALE)
		, _orientation(Quaternion::IDENTITY)
	{
		Mesh::SubMeshIterator i = mesh->getSubMeshIterator();
		while (i.hasMoreElements())
		{
			SubMesh* subMesh = i.peekNext();
			TransformedSubMesh* tsm = OGRE_NEW TransformedSubMesh(this, subMesh);
			_subMeshList.push_back(tsm);
			i.moveNext();
		}
	}
	//--------------------------------------------------------------------------------
	TransformedMesh::~TransformedMesh()
	{
		for (TransformedSubMeshList::iterator i = _subMeshList.begin();
			i != _subMeshList.end(); ++i)
		{
			TransformedSubMesh* subMesh = *i;
			OGRE_DELETE subMesh;
		}
	}
	//--------------------------------------------------------------------------------
	const String& TransformedMesh::getMovableType(void) const
	{
		static String movType = "TransformedMesh";
		return movType;
	}
	//--------------------------------------------------------------------------------
	void TransformedMesh::_updateRenderQueue(RenderQueue* queue)
	{
		for (TransformedSubMeshList::iterator i = _subMeshList.begin();
			i != _subMeshList.end(); ++i)
		{
			TransformedSubMesh* subMesh = *i;
			queue->addRenderable(subMesh, mRenderQueueID, OGRE_RENDERABLE_DEFAULT_PRIORITY);
		}
	}
	//--------------------------------------------------------------------------------
	const AxisAlignedBox& TransformedMesh::getBoundingBox(void) const
	{
		static AxisAlignedBox aabb;
		aabb = _mesh->getBounds();
		Matrix4 m;
		m.makeTransform(
			getPosition(),
			getScale(),
			getOrientation());
		aabb.transformAffine(m);
		return aabb;
	}
	//--------------------------------------------------------------------------------
	Real TransformedMesh::getBoundingRadius(void) const
	{
		AxisAlignedBox mBox = _mesh->getBounds();
		return Math::Sqrt(std::max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
	}
	//--------------------------------------------------------------------------------
	void TransformedMesh::visitRenderables(Renderable::Visitor* visitor, 
		bool debugRenderables)
	{
		for (TransformedSubMeshList::iterator i = _subMeshList.begin();
			i != _subMeshList.end(); ++i)
		{
			TransformedSubMesh* subMesh = *i;
			visitor->visit(subMesh, 0, false);
		}
	}
	//----------------------------------------------------------------------------------------------
	void TransformedMesh::setYawPitchRoll(const Vector3& ypr)
	{
		_orientation = MathUtil::MakeQuaternion(Degree(ypr.x), Degree(ypr.y), Degree(ypr.z));
	}
	//----------------------------------------------------------------------------------------------
	Vector3 TransformedMesh::getYawPitchRoll() const
	{
		return MathUtil::ToAxisOrientation(_orientation);
	}
	//----------------------------------------------------------------------------------------------

