#include "stdafx.h"
#include "Grid.h"


Grid::Grid()
: _node(0)
, _cellSizeOnX(100.0f)
, _cellSizeOnZ(100.0f)
, _bigCellCntOnX(10)
, _bigCellCntOnZ(10)
, _xCellCnt(40)
, _zCellCnt(40)
, _mainAxisClr(0.3f, 0.3f, 0.3f)
, _cellAxisClr(0.4f, 0.4f, 0.4f)
, _bigCellAxisClr(0.6f, 0.6f, 0.6f)
, _visible(true)
, _visibilityFlag(Ogre::MovableObject::getDefaultVisibilityFlags())
, _offset(Vector3::ZERO)
{
	_UpdateGrid();
}
//--------------------------------------------------------------------------------
Grid::~Grid()
{
	Destroy();
}
//--------------------------------------------------------------------------------
void Grid::_UpdateGrid()
{
	if(!_lineList.empty() && (_lineList.size() != _xCellCnt * _zCellCnt))
	{
		Destroy();
	}

	if(!_node)
	{
		SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
		static uint32 i = 0;
		String name = formatString("__autogrid_name[%d]", i);
		++i;
		_node = sm->getRootSceneNode()->createChildSceneNode(name);
		_node->setVisible(_visible);
	}

	int halfX = _xCellCnt / 2;
	int halfZ = _zCellCnt / 2;

	float halfXSz = halfX * _cellSizeOnX;
	float halfZSz = halfZ * _cellSizeOnZ;

	if(_lineList.empty())
	{
		for (int x = -halfX; x <= halfX; ++x)
		{
			DynamicLines* l = OGRE_NEW DynamicLines();
			l->setVisibilityFlags(_visibilityFlag);
			//l->setRenderQueueGroup(RENDER_QUEUE_MAX);
			_lineList.push_back(l);
			_node->attachObject(l);
		}

		for (int z = -halfZ; z <= halfZ; ++z)
		{
			DynamicLines* l = OGRE_NEW DynamicLines();
			l->setVisibilityFlags(_visibilityFlag);
			//l->setRenderQueueGroup(RENDER_QUEUE_MAX);
			_lineList.push_back(l);
			_node->attachObject(l);
		}
	}

	int idx = 0;
	for (int x = -halfX; x <= halfX; ++x)
	{
		DynamicLines* l = _lineList[idx];
		l->clear();
		l->addPoint(Vector3(x * _cellSizeOnX, 0.0f, -halfZSz) + _offset);
		l->addPoint(Vector3(x * _cellSizeOnX, 0.0f, halfZSz) + _offset);
		if(x == 0)
		{
			l->setLineColour(_mainAxisClr);
		}
		else if( x % _bigCellCntOnX)
		{
			l->setLineColour(_bigCellAxisClr);
		}
		else
		{
			l->setLineColour(_cellAxisClr);
		}

		++idx;
	}

	for (int z = -halfZ; z <= halfZ; ++z)
	{
		DynamicLines* l = _lineList[idx];
		l->clear();
		l->addPoint(Vector3(-halfXSz, 0.0f, z * _cellSizeOnZ) + _offset);
		l->addPoint(Vector3(halfXSz, 0.0f, z * _cellSizeOnZ) + _offset);
		if(z == 0)
		{
			l->setLineColour(_mainAxisClr);
		}
		else if( z % _bigCellCntOnZ)
		{
			l->setLineColour(_bigCellAxisClr);
		}
		else
		{
			l->setLineColour(_cellAxisClr);
		}

		++idx;
	}
}
//--------------------------------------------------------------------------------
void Grid::Destroy()
{
	if(_node)
	{
		_node->detachAllObjects();
	}


	LinesList::iterator i = _lineList.begin();
	for (; i != _lineList.end(); ++i)
	{
		OGRE_DELETE *i;
	}

	_lineList.clear();

	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	sm->destroySceneNode(_node);
	_node = 0;
}
//--------------------------------------------------------------------------------
void Grid::VisibilityFlag(Ogre::uint32 val)
{
	_visibilityFlag = val;

	LinesList::iterator i = _lineList.begin();
	for (; i != _lineList.end(); ++i)
	{
		DynamicLines* l = *i;
		l->setVisibilityFlags(_visibilityFlag);
	}
}
//--------------------------------------------------------------------------------
void Grid::SetVisible(bool val)
{
	_visible = val;
	if(_node)
	{
		_node->setVisible(val);
	}
}
//--------------------------------------------------------------------------------
void Grid::Offset( const Vector3& pos )
{
	_offset = pos;
	_UpdateGrid();
}
//--------------------------------------------------------------------------------
const Ogre::Vector3& Grid::Offset() const
{
	return _offset;
}