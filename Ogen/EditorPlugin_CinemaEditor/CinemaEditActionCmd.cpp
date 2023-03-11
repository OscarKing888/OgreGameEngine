#include "stdafx.h"
#include "CinemaEditActionCmd.h"
#include "CinemaEditor.h"
#include "Cmd_AddTrack.h"
#include "Cmd_CreateKeyFrame.h"

void Action_NewCinema::Execute( const Any& contex )
{
	CinemaManager::getSingleton().CreateCinema(
		any_cast<String>(contex), 10.0f);
}
//--------------------------------------------------------------------------------
void Action_SaveAll::Execute( const Any& contex )
{
	IGame* game = GameApp::getSingleton().GetGame();
	String sceneDir = game->GetSceneDir();
	sceneDir +=  game->GetLoadMapName() + "/" + CinemaManager::CinemaXMLFileName;
	CinemaManager::getSingleton().SaveCinemas(sceneDir);
}
//--------------------------------------------------------------------------------
void Action_AddTrack::Execute( const Any& contex )
{
	String trackType = any_cast<String>(contex);
	CinemaEditor& editor = CinemaEditor::getSingleton();
	editor.AddCommand(OGRE_NEW Cmd_AddTrack(
		EditorApp::getSingleton().GetSelectedObject(),
		editor.GetCinema(), trackType, NameValuePairList(), CinemaKeyFrameDataList(),
		ICinemaTrack::KeyFrameTipList()));
}
//--------------------------------------------------------------------------------
void Action_EditCinema::Execute( const Any& contex )
{
	CinemaEditor::getSingleton().EditCinema(any_cast<String>(contex));
}
//--------------------------------------------------------------------------------
void Action_CreateKeyFrame::Execute( const Any& contex )
{
	CinemaEditor& editor = CinemaEditor::getSingleton();
	ICinemaTrack* track = editor.GetTrack();

	if(!track->GetTargetObjct())
	{
		track->SetTargetObjct(EditorApp::getSingleton().GetSelectedObject());
	}	

	ITrackEditor* te = editor.GetTrackEditor(track);
	if(te)
	{
		CinemaKeyFrame* kf = track->CreateKeyFrame(editor.GetCurrentTime());

		if(te->NeedShowDlgWhenCreateKeyFrame())
		{
			te->EditKeyFrameWithDlg(track, kf);
		}
		else
		{
			track->AutoUpdateKeyFrame(kf);
		}

		editor.AddCommand(OGRE_NEW Cmd_DeleteKeyFrame(track, kf), false);
	}

	EventsManager::getSingleton().CreateEvent(
		CinemaEditorEvents::UpdateUI_CreateKeyFrame, Any(track));
}
////--------------------------------------------------------------------------------
//void Action_CinemaPlay::Execute( const Any& contex )
//{
//	CinemaEditor::getSingleton().GetCinema()->Play();
//}
////--------------------------------------------------------------------------------
//void Action_CinemaPause::Execute( const Any& contex )
//{
//	bool isPlaying = CinemaEditor::getSingleton().GetCinema()->IsPlaying();
//	CinemaEditor::getSingleton().GetCinema()->Pause(!isPlaying);
//}
////--------------------------------------------------------------------------------
//void Action_CinemaStop::Execute( const Any& contex )
//{
//	CinemaEditor::getSingleton().GetCinema()->Stop();
//}
////--------------------------------------------------------------------------------
//void Action_CinemaToBegin::Execute( const Any& contex )
//{
//	CinemaEditor::getSingleton().GetCinema()->ToBegin();
//}
////--------------------------------------------------------------------------------
//void Action_CinemaToEnd::Execute( const Any& contex )
//{
//	CinemaEditor::getSingleton().GetCinema()->ToEnd();
//}


void Action_RemoveTrack::Execute( const Any& contex )
{
	CinemaEditor& editor = CinemaEditor::getSingleton();
	ICinemaTrack* track = any_cast<ICinemaTrack*>(contex);
	int idx = editor.GetCinema()->GetTrackIndex(track);
	assert(idx >= 0);

	editor.AddCommand(OGRE_NEW Cmd_RemoveTrack(
		track->GetTargetObjct(), editor.GetCinema(), idx));
}