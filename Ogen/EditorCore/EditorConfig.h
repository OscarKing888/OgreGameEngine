#pragma once
#include "EditorCorePrerequisites.h"

///
/// ����ٶ�
///
enum _EditorCoreExport ECameraSpeed
{
	eCS_Slow = 0,
	eCS_Normal = 1,
	eCS_Fast = 2
};

class _EditorCoreExport ECameraSpeedConverter : public AbstractEnumDescriptor
{
public:

	ECameraSpeedConverter();
	virtual ~ECameraSpeedConverter();
};

///
/// ��ͼ��С
///
enum _EditorCoreExport ESnapSize
{
	eSS_256 = 256,
	eSS_512 = 512,
	eSS_1024 = 1024,
	eSS_2048 = 2048,
	eSS_4096 = 4096,
	eSS_8192 = 8192,
};

class _EditorCoreExport ESnapSizeConverter : public AbstractEnumDescriptor
{
public:

	ESnapSizeConverter();
	virtual ~ESnapSizeConverter();
};

//--------------------------------------------------------------------------------
///
/// �༭��ȫ������
///
class _EditorCoreExport EditorConfig
	: public SingletonMyers<EditorConfig>
	, public PropertyObject
{
public:

	EditorConfig();

	/// ���������ļ�
	void LoadConfig();

	/// �Ƿ񱾵ػ���������
	bool GetTranslateUIToLocal() const { return _translateUIToLocal; }
	void SetTranslateUIToLocal(bool val) { _translateUIToLocal = val; }

	///
	/// ��ͼ��ʾ����-----------------------------------------------------------
	///

	/// ��ͼ������ɫ
	Ogre::ColourValue GetViewportColour() const { return _viewportColour; }
	void SetViewportColour(Ogre::ColourValue val) { _viewportColour = val; }

	///
	/// �������-----------------------------------------------------------
	///

	/// ����ƶ��ٶ�
	ECameraSpeed GetCameraMoveSpeed() const { return _cameraMoveSpeed; }
	void SetCameraMoveSpeed(ECameraSpeed val) { _cameraMoveSpeed = val; }

	/// �ƶ����ʱ�����ĸ߶�
	float GetCameraHeight() const { return _cameraHeight; }
	void SetCameraHeight(float val) { _cameraHeight = val; }

	/// �ƶ����ʱ�Ƿ�����
	bool GetClampCameraToTerrain() const { return _clampCameraToTerrain; }
	void SetClampCameraToTerrain(bool val) { _clampCameraToTerrain = val; }

	///
	/// �ƶ�����-----------------------------------------------------------
	///

	/// Pickʱ�Ĺ�����
	uint32	PickFilter;
	
	/// �ƶ�����ʱ�Ƿ��Զ�����
	bool GetAutoClampTerrainOnMove() const { return _autoClampTerrainOnMove; }
	void SetAutoClampTerrainOnMove(bool val) { _autoClampTerrainOnMove = val; }

	/// �ƶ�����ʱ�Ƿ��Զ�����ر���
	bool GetAutoAlignTerrainNormal() const { return _autoAlignTerrainNormal; }
	void SetAutoAlignTerrainNormal(bool val) { _autoAlignTerrainNormal = val; }

	/// �Ƿ��ƶ�����ʱ���뵽����
	bool GetSnapToGrid() const { return _snapToGrid; }
	void SetSnapToGrid(bool val) { _snapToGrid = val; }

	/// �Ƿ���ʾ������
	bool GetShowSnapGrid() const { return _showSnapGrid; }
	void SetShowSnapGrid(bool val) { _showSnapGrid = val; }

	/// �����õ������С
	Ogre::Vector2 GetSnapGridSize() const { return _snapGridSize; }
	void SetSnapGridSize(Ogre::Vector2 val) { _snapGridSize = val; }


	///
	/// ��������-----------------------------------------------------------
	///

	/// �������������ǳ����Ƿ����
	bool GetRandomOrientationOnCreate() const { return _randomOrientationOnCreate; }
	void SetRandomOrientationOnCreate(bool val) { _randomOrientationOnCreate = val; }

	/// ������������ʱ�Ƿ������������
	bool GetRandomScaleOnCreate() const { return _randomScaleOnCreate; }
	void SetRandomScaleOnCreate(bool val) { _randomScaleOnCreate = val; }

	/// ������������ʱ��������ŷ�Χ
	Ogre::Vector2 GetRandomScaleRange() const { return _randomScaleRange; }
	void SetRandomScaleRange(Ogre::Vector2 val) { _randomScaleRange = val; }

	///
	/// ��ͼ����-----------------------------------------------------------
	///

	/// ��ͼʱ��Pitch�Ƕ�
	const Radian GetSnapshotAngle() const;
	void SetSnapshotAngle(const Radian& ang);

	/// ��ͼʱ�õ�FOV
	const Radian GetSnapshotFov() const;
	void SetSnapshotFov(const Radian& ang);

	/// ��ͼ��С
	ESnapSize GetSnapshotSize() const;
	void SetSnapshotSize(ESnapSize sz);

	/// ��ͼ��չ������
	String GetImageType() const;
	void SetImageType(const String& t);

	/// ��ͼ������չ���б�
	StringVector GetImageTypeList() const;
	void SetImageTypeList(const StringVector& lst);

	/// ??
	Vector3 GetCameraOffsetScale() const;
	void SetCameraOffsetScale(const Vector3& o);


protected:

	bool _translateUIToLocal;

	ColourValue	_viewportColour;

	ECameraSpeed	_cameraMoveSpeed;
	Vector3		_camOffsetScale;

	float		_cameraHeight;
	bool		_clampCameraToTerrain;

	bool		_autoAlignTerrainNormal;
	bool		_autoClampTerrainOnMove;
	bool		_snapToGrid;
	bool		_showSnapGrid;
	Vector2		_snapGridSize;

	Vector2		_randomScaleRange;
	bool		_randomScaleOnCreate;
	bool		_randomOrientationOnCreate;

	Radian		_snapshotAngle;
	Radian		_snapshotFov;
	ESnapSize	_snapshotTexSize;
	String		_imageType;

	virtual void BuildPropertyMap();
};

