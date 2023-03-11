#pragma once


// NewMapDlg dialog

class NewMapDlg : public CDialog
{
	DECLARE_DYNAMIC(NewMapDlg)

public:
	NewMapDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NewMapDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEW_MAP };

	const CString& GetMapName() const{ return _mapName; }
	int GetTileSize() const { return _vertexCountOnEdge; }
	int GetWorldSize() const { return _terrainWorldSize; }
	int GetTerrainXCnt() const { return _terrainXCnt; }
	int GetTerrainZCnt() const { return _terrainZCnt; }
	
	float GetTexWorldSize() const { return _textureWorldSize; }
	uint16 GetBlendMapSize() const { return _blendMapSize; }
	uint16 GetColourMapSize() const { return _colourMapSize; }
	uint16 GetLightMapSize() const { return _lightMapSize; }

	float GetCompositeMapDistance() const { return _compositeMapDistance; }
	float GetSkirtSize() const { return _skirtSize; }
	uint16 GetCompositeMapSize() const { return _compositeMapSize; }
	float GetMaxPixelError() const { return _maxPixelError; }

	bool GetCastDynamicShadows() const { return _castDynShadows == 1; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	CString	_mapName;
	int		_vertexCountOnEdge;
	int		_terrainWorldSize;
	long	_terrainXCnt;
	long	_terrainZCnt;
	
	float	_textureWorldSize;
	int _blendMapSize;
	int _colourMapSize;
	int _lightMapSize;

	float _compositeMapDistance;
	float _skirtSize;
	int _compositeMapSize;
	float _maxPixelError;

	int _castDynShadows;


	CComboBox _vertexCountOnEdgeBox;
	CComboBox _terrainWorldSizeBox;
	CComboBox _terrainXCntBox;
	CComboBox _terrainZCntBox;

	CComboBox _textureWorldSizeBox;
	CComboBox _blendMapSizeBox;
	CComboBox _colourMapSizeBox;
	CComboBox _lightMapSizeBox;

	CComboBox _compositeMapDistanceBox;
	CComboBox _skirtSizeBox;
	CComboBox _compositeMapSizeBox;
	CComboBox _maxPixelErrorBox;

	CComboBox _castDynShadowsBox;

	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
};
