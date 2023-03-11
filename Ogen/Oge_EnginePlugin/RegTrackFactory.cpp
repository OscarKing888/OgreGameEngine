#include "stdafx.h"
#include "ICinemaTrack.h"

#include "CinemaTrack_Movement.h"
#include "CinemaTrack_Camera.h"
#include "CinemaTrack_FadeScreen.h"
#include "CinemaTrack_Property.h"
#include "CinemaTrack_LightColour.h"
#include "CinemaTrack_MainLightColour.h"
#include "CinemaTrack_AmbientLightColour.h"

static CinemaManager _CinemaManager;

M_REG_CINEMA_TRACK_FACTORY(Movement, CinemaTrack_MovementFactory);
M_REG_CINEMA_TRACK_FACTORY(Camera, CinemaTrack_CameraFactory);
M_REG_CINEMA_TRACK_FACTORY(FadeScreen, CinemaTrack_FadeScreenFactory);
M_REG_CINEMA_TRACK_FACTORY(Property, CinemaTrack_PropertyFactory);
M_REG_CINEMA_TRACK_FACTORY(LightColour, CinemaTrack_LightColourFactory);
M_REG_CINEMA_TRACK_FACTORY(MainLightColour, CinemaTrack_MainLightColourFactory);
M_REG_CINEMA_TRACK_FACTORY(AmbientLightColour, CinemaTrack_AmbientLightColourFactory);
