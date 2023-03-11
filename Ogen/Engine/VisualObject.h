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
#include "AbstractGameObject.h"

#define M_DEBUG_MESH_FILE "Debug.Flag.mesh"

///
/// Wrapper class for setUserAny 
///
struct _EngineExport AnyGameObject : public OgeAlloc
{
	IGameObject* GameObject;
	AnyGameObject(IGameObject* obj);

	_EngineExport friend std::ostream& operator<<(std::ostream& o, const AnyGameObject& r)
	{ (void)r; return o; }
};

///
/// Abstract class for manage SceneNode
///
class _EngineExport VisualObject : public AbstractGameObject
{
public:

	VisualObject(void);
	virtual ~VisualObject(void);

	virtual void SetGUID(const XGUID& guid);

	virtual void PostLoad();
	virtual void Destroy();

	virtual const Vector3& GetPosition() const;
	virtual void SetPosition(const Vector3& pos);
	virtual void SetPosition(float x, float y, float z) { SetPosition(Vector3(x, y, z)); }
	virtual void SetWorldPosition(const Vector3& pos);
	virtual const Vector3& GetWorldPosition() const;

	virtual const Vector3& GetScale() const;
	virtual void SetScale(const Vector3& scale);
	virtual void SetScale(float x, float y, float z) { SetScale(Vector3(x, y, z)); }
	virtual const Vector3& GetWorldScale() const;

	virtual const Quaternion& GetOrientation() const;
	virtual void SetOrientation(const Quaternion& orient);
	//virtual void SetOrientation(float yawDegree, float pitchDegree, float rollDegree);
	virtual void SetOrientation(float w, float x, float y, float z){ SetOrientation(Quaternion(w, x, y, z)); }
	virtual void SetWorldOrientation(const Quaternion& q);
	virtual const Quaternion& GetWorldOrientation() const;

	virtual void SetYaw(const Radian& r);
	virtual void SetPitch(const Radian& r);
	virtual void SetRoll(const Radian& r);

	virtual const Radian& GetYaw() const;
	virtual const Radian& GetPitch() const;
	virtual const Radian& GetRoll() const;

	virtual void SetYaw(const Degree& d);
	virtual void SetPitch(const Degree& d);
	virtual void SetRoll(const Degree& d);

	virtual SceneNode* GetNode() { return _node; }

	virtual void SetVisible(bool on);
	virtual bool GetVisible() const;

	virtual void SetShowBoundingBox(bool on);
	virtual bool GetShowBoundingBox() const;

	void SetDisplayEditorObjectInGame(bool on){ _displayEditorObjectInGame = on; SetVisible(_visible); }
	bool GetDisplayEditorObjectInGame() const { return _displayEditorObjectInGame; }

	virtual bool IsEditorDislpayOnly() const { return _isEditorDislpayOnly; }
	bool GetEditorDislpayOnly() const { return IsEditorDislpayOnly(); }
	void SetEditorDislpayOnly(bool){} // just empty for property handling

	virtual void OnEditorSelect(){}
	virtual void OnEditorDeselect(){}

	static uint32 DefaultVisibilityFlag;

	virtual void SetVisibilityFlag(uint32 flag);
	virtual uint32 GetVisibilityFlag() const;

	virtual uint32 GetQueryFlag();

	virtual void Move(const Vector3& deltaPos);
	virtual void Yaw(const Radian& angle);
	virtual void Pitch(const Radian& angle);
	virtual void Roll(const Radian& angle);

	SceneManager* GetSceneManager();

protected:

	SceneNode*	_node;
	bool				_visible;
	bool				_displayEditorObjectInGame;
	bool				_isEditorDislpayOnly;
	bool				_isShowBoundingBox;
	uint32		_visibilityFlag;

	Radian		_yaw;
	Radian		_pitch;
	Radian		_roll;

	void UpdateRawPitchRoll();


	virtual void BuildPropertyMap();

	virtual void DestroyNodeImp();
	virtual void CreateNodeImp();

	virtual void PreCreateNode();
	virtual void PostCreateNode();

	virtual void PreDestroyNode();
	virtual void PostDestroyNode();

	virtual String GetNodeName();

	virtual void OnAddToScene(SceneNode* rootNode);
	virtual void OnRemoveFromScene(SceneNode* rootNode);

private:

	void DestroyNode();
	void CreateNode();
};
