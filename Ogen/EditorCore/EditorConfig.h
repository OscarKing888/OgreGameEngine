#pragma once
#include "EditorCorePrerequisites.h"

///
/// 相机速度
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
/// 截图大小
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
/// 编辑器全局配置
///
class _EditorCoreExport EditorConfig
	: public SingletonMyers<EditorConfig>
	, public PropertyObject
{
public:

	EditorConfig();

	/// 加载配置文件
	void LoadConfig();

	/// 是否本地化界面文字
	bool GetTranslateUIToLocal() const { return _translateUIToLocal; }
	void SetTranslateUIToLocal(bool val) { _translateUIToLocal = val; }

	///
	/// 视图显示参数-----------------------------------------------------------
	///

	/// 视图背景颜色
	Ogre::ColourValue GetViewportColour() const { return _viewportColour; }
	void SetViewportColour(Ogre::ColourValue val) { _viewportColour = val; }

	///
	/// 相机参数-----------------------------------------------------------
	///

	/// 相机移动速度
	ECameraSpeed GetCameraMoveSpeed() const { return _cameraMoveSpeed; }
	void SetCameraMoveSpeed(ECameraSpeed val) { _cameraMoveSpeed = val; }

	/// 移动相机时离地面的高度
	float GetCameraHeight() const { return _cameraHeight; }
	void SetCameraHeight(float val) { _cameraHeight = val; }

	/// 移动相机时是否贴地
	bool GetClampCameraToTerrain() const { return _clampCameraToTerrain; }
	void SetClampCameraToTerrain(bool val) { _clampCameraToTerrain = val; }

	///
	/// 移动物体-----------------------------------------------------------
	///

	/// Pick时的过滤器
	uint32	PickFilter;
	
	/// 移动物体时是否自动贴地
	bool GetAutoClampTerrainOnMove() const { return _autoClampTerrainOnMove; }
	void SetAutoClampTerrainOnMove(bool val) { _autoClampTerrainOnMove = val; }

	/// 移动物体时是否自动对齐地表法线
	bool GetAutoAlignTerrainNormal() const { return _autoAlignTerrainNormal; }
	void SetAutoAlignTerrainNormal(bool val) { _autoAlignTerrainNormal = val; }

	/// 是否移动物体时对齐到网格
	bool GetSnapToGrid() const { return _snapToGrid; }
	void SetSnapToGrid(bool val) { _snapToGrid = val; }

	/// 是否显示网格线
	bool GetShowSnapGrid() const { return _showSnapGrid; }
	void SetShowSnapGrid(bool val) { _showSnapGrid = val; }

	/// 对齐用的网格大小
	Ogre::Vector2 GetSnapGridSize() const { return _snapGridSize; }
	void SetSnapGridSize(Ogre::Vector2 val) { _snapGridSize = val; }


	///
	/// 创建物体-----------------------------------------------------------
	///

	/// 批量创建物体是朝向是否随机
	bool GetRandomOrientationOnCreate() const { return _randomOrientationOnCreate; }
	void SetRandomOrientationOnCreate(bool val) { _randomOrientationOnCreate = val; }

	/// 批量创建物体时是否随机缩放物体
	bool GetRandomScaleOnCreate() const { return _randomScaleOnCreate; }
	void SetRandomScaleOnCreate(bool val) { _randomScaleOnCreate = val; }

	/// 批量创建物体时的随机缩放范围
	Ogre::Vector2 GetRandomScaleRange() const { return _randomScaleRange; }
	void SetRandomScaleRange(Ogre::Vector2 val) { _randomScaleRange = val; }

	///
	/// 截图参数-----------------------------------------------------------
	///

	/// 截图时的Pitch角度
	const Radian GetSnapshotAngle() const;
	void SetSnapshotAngle(const Radian& ang);

	/// 截图时用的FOV
	const Radian GetSnapshotFov() const;
	void SetSnapshotFov(const Radian& ang);

	/// 截图大小
	ESnapSize GetSnapshotSize() const;
	void SetSnapshotSize(ESnapSize sz);

	/// 截图扩展名类型
	String GetImageType() const;
	void SetImageType(const String& t);

	/// 截图类型扩展名列表
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

