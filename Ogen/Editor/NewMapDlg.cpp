// NewMapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Editor.h"
#include "NewMapDlg.h"
#include "EditorGameApp.h"

// NewMapDlg dialog

IMPLEMENT_DYNAMIC(NewMapDlg, CDialog)

NewMapDlg::NewMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NewMapDlg::IDD, pParent)
	, _vertexCountOnEdge(512)
	, _terrainWorldSize(10000)
	, _terrainXCnt(10)
	, _terrainZCnt(10)
	, _textureWorldSize(100.0f)
	, _castDynShadows(0)
	, _maxPixelError(8.0f)
	, _lightMapSize(1024)
	, _compositeMapDistance(10000)
	, _skirtSize(30)
	, _compositeMapSize(1024)
	, _blendMapSize(1024)
	, _colourMapSize(1024)
{	
}

NewMapDlg::~NewMapDlg()
{
}

void NewMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MAP_WIDTH, _vertexCountOnEdgeBox);
	DDX_Control(pDX, IDC_EDIT_WORLD_SIZE, _terrainWorldSizeBox);
	DDX_Control(pDX, IDC_EDIT_TERRAIN_CNT_X, _terrainXCntBox);
	DDX_Control(pDX, IDC_EDIT_TERRAIN_CNT_Z, _terrainZCntBox);

	DDX_Control(pDX, IDC_EDIT_TEXTURE_WORLD_SIZE, _textureWorldSizeBox);
	DDX_Control(pDX, IDC_EDIT_BLENDMAP_SIZE, _blendMapSizeBox);
	DDX_Control(pDX, IDC_EDIT_COLOURMAP_SIZE, _colourMapSizeBox);
	DDX_Control(pDX, IDC_EDIT_LIGHTMAP_SIZE, _lightMapSizeBox);

	DDX_Control(pDX, IDC_EDIT_COMPOSITEMAP_DISTANCE, _compositeMapDistanceBox);
	DDX_Control(pDX, IDC_EDIT_SKIRT_SIZE, _skirtSizeBox);
	DDX_Control(pDX, IDC_EDIT_COMPOSITEMAP_SIZE, _compositeMapSizeBox);
	DDX_Control(pDX, IDC_EDIT_MAX_PIXEL_ERROR, _maxPixelErrorBox);

	DDX_Control(pDX, IDC_CHECK_CAST_DYN_SHADOWS, _castDynShadowsBox);





	DDX_Text(pDX, IDC_EDIT_MAP_NAME, _mapName);

	DDX_Text(pDX, IDC_EDIT_MAP_WIDTH, _vertexCountOnEdge);
	DDX_Text(pDX, IDC_EDIT_WORLD_SIZE, _terrainWorldSize);
	DDX_Text(pDX, IDC_EDIT_TERRAIN_CNT_X, _terrainXCnt);
	DDX_Text(pDX, IDC_EDIT_TERRAIN_CNT_Z, _terrainZCnt);
	
	DDX_Text(pDX, IDC_EDIT_TEXTURE_WORLD_SIZE, _textureWorldSize);
	DDX_Text(pDX, IDC_EDIT_BLENDMAP_SIZE, (int&)_blendMapSize);
	DDX_Text(pDX, IDC_EDIT_COLOURMAP_SIZE, (int&)_colourMapSize);
	DDX_Text(pDX, IDC_EDIT_LIGHTMAP_SIZE, (int&)_lightMapSize);

	DDX_Text(pDX, IDC_EDIT_COMPOSITEMAP_DISTANCE, _compositeMapDistance);
	DDX_Text(pDX, IDC_EDIT_SKIRT_SIZE, _skirtSize);
	DDX_Text(pDX, IDC_EDIT_COMPOSITEMAP_SIZE, (int&)_compositeMapSize);
	DDX_Text(pDX, IDC_EDIT_MAX_PIXEL_ERROR, _maxPixelError);

	DDX_Check(pDX, IDC_CHECK_CAST_DYN_SHADOWS, _castDynShadows);	
}


BEGIN_MESSAGE_MAP(NewMapDlg, CDialog)
END_MESSAGE_MAP()


// NewMapDlg message handlers

BOOL NewMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int j = 5;
	while( j < 30)
	{
		CString str;
		str.Format("%d", 2 << j);
		_vertexCountOnEdgeBox.AddString(str);
		++j;
	}

	for(int i = 5000; i <= 100000; i += 5000)
	{
		CString str;
		str.Format("%d", i);
		_terrainWorldSizeBox.AddString(str);
	}

	for(int i = 1; i <= 50; ++i)
	{
		CString str;
		str.Format("%d", i * 10);
		_textureWorldSizeBox.AddString(str);
	}


	for(int i = 10; i <= 100; i += 10)
	{
		CString str;
		str.Format("%d", i);
		_terrainXCntBox.AddString(str);
		_terrainZCntBox.AddString(str);
	}

	_vertexCountOnEdgeBox.SelectString(0, "512");
	_terrainWorldSizeBox.SelectString(0, "10000");

	_textureWorldSizeBox.SelectString(0, "100");
	_terrainXCntBox.SelectString(0, "10");
	_terrainZCntBox.SelectString(0, "10");

	static int idx = 0;
	_mapName.Format("Map%d", idx);
	++idx;
	UpdateData(FALSE);

	return TRUE;
}

void NewMapDlg::OnOK()
{
	UpdateData();
	String sceneDir = EditorGame::getSingleton().GetSceneDir();
	sceneDir += _mapName;
	if(FileEx::isDirExist(sceneDir))
	{
		::AfxMessageBox(_UIDisplayString("Map already exist!").c_str());
		return;
	}

	CDialog::OnOK();
}