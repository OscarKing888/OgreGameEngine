// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-07-18
// *************************************************************************************

#pragma once
#include "VisualObject.h"

class _EngineExport StaticModel
	: public VisualObject
	, public ResourceBackgroundQueue::Listener
{
public:

	StaticModel(void);
	virtual ~StaticModel(void);

	virtual void SetMeshFile(const String& meshFile);
	virtual void SetMeshFileEx(const String& meshFile, bool loadInBackground);
	virtual const String& GetMeshFile() const { return _meshFile; }

	virtual Entity* GetEntity() const { return _entity; }
	virtual Mesh* GetMesh();

	void SetCastShadow(bool on);
	bool GetCastShadow() const { return _castShadow; }

	void SetBatchInstance(bool on);
	bool GetBatchInstance() const { return _isBatchInstance; }

	void SetSubEntityMaterial(size_t subEntityIdx, const String& matName);
	const String& GetSubEntityMaterial(size_t subEntityIdx);

	// composited string value  idx:materialName;idx:materialName
	void SetSubMaterials(const StringMap& idxAndMatName);
	StringMap GetSubMaterials() const;

	StringVector GetSubEntities() const;

	virtual const String& GetTypeName() const;

	virtual void SetVisibilityFlag(uint32 flag);

	void DestroyEntity();
	void CreateEntity();

protected:

	Entity*	_entity;
	String	_meshFile;
	bool	_castShadow;
	bool	_isBatchInstance;

	typedef std::map<size_t, String> SubMaterialMap;
	SubMaterialMap	_materials;
	BackgroundProcessTicket _meshLoadTicket;
	bool _loadInBackground;

	virtual void BuildPropertyMap();

	virtual void DestroyEntityImp();
	virtual void CreateEntityImp();

	virtual void PreCreateEntity();
	virtual void PostCreateEntity();

	virtual void PreDestroyEntity();
	virtual void PostDestroyEntity();

	virtual void OnMeshFileChanged();

	virtual String GetEntityName();

	virtual void PostCreateNode();
	virtual void PreDestroyNode();

	virtual void operationCompleted(BackgroundProcessTicket ticket, const BackgroundProcessResult& result);
};