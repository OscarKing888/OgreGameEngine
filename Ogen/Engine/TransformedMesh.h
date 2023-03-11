#pragma once



	class TransformedMesh;

	//--------------------------------------------------------------------------------
	// TransformedMesh
	//--------------------------------------------------------------------------------
	class _EngineExport TransformedSubMesh
		: public Renderable
		, public OgeAlloc
	{
	public:
		
		TransformedSubMesh(TransformedMesh* parent, SubMesh* subMesh);
		virtual ~TransformedSubMesh();

		virtual const MaterialPtr& getMaterial(void) const;
		virtual void getRenderOperation(RenderOperation& op);
		virtual void getWorldTransforms( Matrix4* xform ) const;
		virtual const LightList& getLights(void) const;
		virtual Real getSquaredViewDepth(const Camera* cam) const;

	protected:

		SubMesh* _subMesh;
		TransformedMesh* _parent;
		MaterialPtr _material;
	};

	//--------------------------------------------------------------------------------
	// TransformedMesh
	//--------------------------------------------------------------------------------
	class _EngineExport TransformedMesh : public MovableObject
	{
	public:

		TransformedMesh(const String& name, MeshPtr mesh);
		virtual ~TransformedMesh();

		virtual const String& getMovableType(void) const;

		virtual const AxisAlignedBox& getBoundingBox(void) const;
		virtual Real getBoundingRadius(void) const;

		virtual void _updateRenderQueue(RenderQueue* queue);

		virtual void visitRenderables(Renderable::Visitor* visitor, 
			bool debugRenderables);

		void setPosition(const Vector3& pos) { _position = pos; }
		const Vector3& getPosition() const { return _position; }

		void setScale(const Vector3& s) { _scale = s; }
		const Vector3& getScale() const { return _scale; }

		void setYawPitchRoll(const Vector3& ypr);
		Vector3 getYawPitchRoll() const;

		const Quaternion& getOrientation() const { return _orientation; }

	protected:

		typedef std::vector<TransformedSubMesh*> TransformedSubMeshList;
		TransformedSubMeshList _subMeshList;
		MeshPtr _mesh;
		Vector3 _position;
		Vector3 _scale;
		Quaternion _orientation;
	};


