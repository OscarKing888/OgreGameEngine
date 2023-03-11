// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2006-12-30
// *************************************************************************************

#include "StdAfx.h"
#include "FrameAnimation.h"
#include "GameApp.h"

//--------------------------------------------------------------------------------------
template<> Animations::FrameAnimationType* Singleton<Animations::FrameAnimationType>::ms_Singleton = 0;

namespace Animations
{
	
static FrameAnimationType _FrameAnimationTypeInstance;

FrameAnimationType::FrameAnimationType()
{
	M_AddEnumToDescriptorEx(eAnimationNone, None);
	M_AddEnumToDescriptorEx(eAnimationLoop, Loop);
	M_AddEnumToDescriptorEx(eAnimationOnce, Once);
	M_AddEnumToDescriptorEx(eAnimationReverse, Reverse);

	Init();
}

FrameAnimationType::~FrameAnimationType()
{
}

//------------------------------------------------------------------------

const SAnimationInfo& FrameAnimationConfigManager::GetFrameAnimationInfo(const String& strAniName)
{
	FrameAnimationConfigMap::iterator i = _mapFrameAnimationConfig.find(strAniName);
	if(i != _mapFrameAnimationConfig.end())
	{
		return i->second;
	}
	
	OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
		formatString(("[FrameAnimationConfigManager::GetFrameAnimationInfo] [%s] not found!"), strAniName.c_str()),
		"FrameAnimationConfigManager::GetFrameAnimationInfo");
}

bool FrameAnimationConfigManager::AddFrameAnimationInfo(const String& strAniName, const SAnimationInfo& info)
{
	FrameAnimationConfigMap::iterator i = _mapFrameAnimationConfig.find(strAniName);
	if(i != _mapFrameAnimationConfig.end())
	{
		return false;
	}

	_mapFrameAnimationConfig.insert(make_pair(strAniName, info));
	return true;
}

void FrameAnimationConfigManager::RemoveFrameAnimationInfo(const String& strAniName)
{
	FrameAnimationConfigMap::iterator i = _mapFrameAnimationConfig.find(strAniName);
	if(i != _mapFrameAnimationConfig.end())
	{
		_mapFrameAnimationConfig.erase(i);
	}
}

void FrameAnimationConfigManager::LoadConfig(const String& strFile)
{
	XMLDoc doc;
	DataStreamPtr dataStreamPtr =
		ResourceGroupManager::getSingleton().openResource(strFile);

	if(doc.loadFromXML(dataStreamPtr->getAsString()))
	{
		XMLElement c = doc.getRoot();
		c = c.firstChild();
		
		while(!c.isNull())
		{
			SAnimationInfo info;
			String strKey = c.name();
			info.ImageName = c.getAttribute(("ImageName"));

			info.FrameAnimationType = (EAnimationType)FrameAnimationType::getSingleton().FromString(
				(c.getAttribute(("FrameAnimationType"))));

			info.TotalFrames = getIntAttribuate(("TotalFrames"), c);
			
			info.HFrameCount = getIntAttribuate("HFrameCount", c);
			info.VFrameCount = getIntAttribuate("VFrameCount", c);
									
			XMLElement d = c.firstChild();
			while(!d.isNull())
			{
				String nodeName = d.name();
				if(nodeName == ("DelayEach"))
				{
					info.DelayEach = getFloatAttribuate("Time", d);
				}
				else if(nodeName == ("Delay"))
				{
					int key = getIntAttribuate("FrameNumber", d);
					float val = getFloatAttribuate("Time", d);
					info.DelayFPSMap.insert(std::make_pair(key, val));
				}
				d = d.nextSibling();
			}
			_mapFrameAnimationConfig.insert(make_pair(strKey, info));
			c = c.nextSibling();
		}
	}
}

//--------------------------------------------------------------------------------------

FrameAnimation::FrameAnimation(const String& frameAniConfigName)
							   : _configName(frameAniConfigName)
							   , _currentFrame(0)
							   , _timeDelta(0.0f)
							   , _aniDir(eAD_Forward)
							   , _enable(true)
							   , _isOnceDead(false)
							   , _isEndShow(false)
{
	SetAnimationInfo(
		FrameAnimationConfigManager::getSingleton().GetFrameAnimationInfo(frameAniConfigName));
}

FrameAnimation::FrameAnimation(const String& frameAniConfigName,
							   const SAnimationInfo& aniInfo)
: _configName(frameAniConfigName)
, _currentFrame(0)
, _timeDelta(0.0f)
, _aniDir(eAD_Forward)
, _enable(true)
, _isOnceDead(false)
, _isEndShow(false)
{
	FrameAnimationConfigManager::getSingleton().AddFrameAnimationInfo(frameAniConfigName, aniInfo);
	SetAnimationInfo(aniInfo);
}

FrameAnimation::~FrameAnimation(void)
{
	GameApp::getSingleton().RemoveUpdateable(this);
}

void FrameAnimation::Enable(bool on)
{
	_enable = on;
}

//--------------------------------------------------------------------------------------

void FrameAnimation::Update(float deltaTime)
{
	if(!_enable)
	{
		return;
	}

	//UpdateImage();

	float fDelayTime = _aniInfo.GetDelayTime(_currentFrame);
	_timeDelta += deltaTime;

	bool onceEnd = false;

	if(_timeDelta >= fDelayTime)	// advance the current frame
	{
		_timeDelta = 0.0f;
		switch(_aniInfo.FrameAnimationType) 
		{
		case eAnimationNone:
			{
				_currentFrame = 0;
				break;
			}

		case eAnimationLoop:
			{
				++_currentFrame;
				if(_currentFrame >= _aniInfo.TotalFrames)
				{
					_currentFrame = 0;
				}

				break;
			}

		case eAnimationOnce:
			{
				if(_currentFrame < _aniInfo.TotalFrames - 1)
				{
					++_currentFrame;
				}
				else
				{
					onceEnd = true;
				}
				break;
			}

		case eAnimationReverse:
			{
				if(_aniDir == eAD_Forward)
				{
					++_currentFrame;
					if(_currentFrame >= _aniInfo.TotalFrames)
					{
						_currentFrame = _aniInfo.TotalFrames - 1;
						_aniDir = eAD_Backward;
					}
				}
				else if(_aniDir == eAD_Backward)
				{
					--_currentFrame;
					if(_currentFrame < 0)
					{
						_currentFrame = 0;
						_aniDir = eAD_Forward;
					}
				}
				break;
			}
		} // switch

		_isOnceDead = onceEnd;
		UpdateImage();

	} // if
}

//--------------------------------------------------------------------------------------

void FrameAnimation::SetAnimationInfo(const SAnimationInfo& aniInfo)
{
	GameApp::getSingleton().RemoveUpdateable(this);
	GameApp::getSingleton().AddUpdateable(this);

	_aniInfo = aniInfo;

	UpdateImage();
}

void FrameAnimation::SetCurrentFrame(int frameNumber)
{
	GameApp::getSingleton().RemoveUpdateable(this);
	GameApp::getSingleton().AddUpdateable(this);

	if ( _aniInfo.FrameAnimationType == eAnimationOnce )
	{
		_isOnceDead = false;
	}

	_currentFrame = frameNumber;
	UpdateImage();
}

float FrameAnimation::GetFrameAniTime()
{
    float time = 0.0;
    for (int i=0;i< _aniInfo.TotalFrames;i++)
    {
        time +=_aniInfo.GetDelayTime(i);
    }
	return time;
}

//-------------------------------------------------------------------------------------------------

FrameAnimation_Material::FrameAnimation_Material(TextureUnitState* tus, const String& frameAniConfigName)
: FrameAnimation(frameAniConfigName)
, _textureUnitState(tus)
{
	Init();
}

FrameAnimation_Material::FrameAnimation_Material(TextureUnitState* tus,
												 const String& frameAniConfigName,
												 const SAnimationInfo& aniInfo)
: FrameAnimation(frameAniConfigName, aniInfo)
, _textureUnitState(tus)
{
	Init();
}

FrameAnimation_Material::~FrameAnimation_Material()
{
}

void FrameAnimation_Material::Init()
{
	TexturePtr tex;// = TextureManager::getSingleton().getByName(_aniInfo.ImageName);

	if(!TextureManager::getSingleton().resourceExists(_aniInfo.ImageName))
	{
		DataStreamPtr dataStreamPtr = ResourceGroupManager::getSingleton().openResource(_aniInfo.ImageName);
		
		tex = TextureManager::getSingleton().load(_aniInfo.ImageName, ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
	}
	else
	{
		tex = TextureManager::getSingleton().getByName(_aniInfo.ImageName);
	}


// 	float w = (float)tex->getWidth();
// 	float h = (float)tex->getHeight();
// 	float us = 1.0f / (float)_aniInfo.HFrameCount;
// 	float vs = 1.0f / (float)_aniInfo.VFrameCount;

	_uvUnit.x = 1.0f / (float)_aniInfo.HFrameCount;
	_uvUnit.y = 1.0f / (float)_aniInfo.VFrameCount;

	_textureUnitState->setTextureScale((float)_aniInfo.HFrameCount, (float)_aniInfo.VFrameCount);
	_textureUnitState->setTextureName(_aniInfo.ImageName);
}

void FrameAnimation_Material::UpdateImage()
{
	assert(_textureUnitState);

	int x = _currentFrame % _aniInfo.HFrameCount;
	int y = _currentFrame / _aniInfo.HFrameCount;
	
	if (x < 2)
		x += 2;
	else
		x -= 2;

	float uOffset = _uvUnit.x * ((float)x + 0.5f);
	float vOffset = _uvUnit.y * ((float)y + 0.5f);
#if 0
	if(_aniInfo.ImageName.compare("star_botton_green.dds") == 0)
		LogConsoleIDf("Animation", "%f-----%f-----%d-----%d", uOffset, vOffset, _currentFrame, y * 4 + x);
#endif
	_textureUnitState->setTextureScroll(uOffset, vOffset);
	//_imageHandle->setProperty(("Image"), _aniInfo.GetFrameImagemName(_currentFrame).c_str());
}

}