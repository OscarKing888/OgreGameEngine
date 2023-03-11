#pragma once
#include "CinemaEditorEvents.h"

class Action_NewCinema : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};
//--------------------------------------------------------------------------------
class Action_EditCinema : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};
//--------------------------------------------------------------------------------
class Action_SaveAll : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};
//--------------------------------------------------------------------------------
class Action_AddTrack : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};
//--------------------------------------------------------------------------------
class Action_RemoveTrack : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};
//--------------------------------------------------------------------------------
class Action_CreateKeyFrame : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};
//--------------------------------------------------------------------------------
//class Action_CinemaPlay : public IActionCommandX
//{
//public:
//
//	virtual void Execute(const Any& contex);
//};
////--------------------------------------------------------------------------------
//class Action_CinemaPause : public IActionCommandX
//{
//public:
//
//	virtual void Execute(const Any& contex);
//};
////--------------------------------------------------------------------------------
//class Action_CinemaStop : public IActionCommandX
//{
//public:
//
//	virtual void Execute(const Any& contex);
//};
////--------------------------------------------------------------------------------
//class Action_CinemaToBegin : public IActionCommandX
//{
//public:
//
//	virtual void Execute(const Any& contex);
//};
////--------------------------------------------------------------------------------
//class Action_CinemaToEnd : public IActionCommandX
//{
//public:
//
//	virtual void Execute(const Any& contex);
//};