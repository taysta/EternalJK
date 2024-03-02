/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2005 - 2015, ioquake3 contributors
Copyright (C) 2013 - 2015, OpenJK contributors
Copyright (C) 2015 - 2021, EternalJK contributors
Copyright (C) 2015 - 2023, TaystJK contributors


This file is part of the TaystJK source code.

TaystJK is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <https://www.gnu.org/licenses/>.
===========================================================================
*/

#ifndef ETERNALJK_HUD_STRAFEHELPER_H
#define ETERNALJK_HUD_STRAFEHELPER_H

#include "cg_local.h"
#include "../ui/ui_shared.h"
#include "../game/bg_public.h"

//Pmove functions
int DF_GetMovePhysics();
float DF_GetAccelerate();
float DF_GetAirAccelerate();
float DF_GetAirStrafeAccelerate();
float DF_GetAirStopAccelerate();
float DF_GetAirStrafeWishspeed();
float DF_GetFriction();
float DF_GetDuckScale();
qboolean DF_HasAirControl();
qboolean DF_HasForceJumps();
qboolean DF_HasAutoJump();

/* Defines */
//Strafe Helper Options

//Movement direction keys - Odd is 2 keys
#define KEY_W       0
#define KEY_WA      1
#define KEY_A       2
#define KEY_AS      3
#define KEY_S       4
#define KEY_SD      5
#define KEY_D       6
#define KEY_DW      7
#define KEY_CENTER  8

#define SHELPER_ORIGINAL    	(1<<0)
#define SHELPER_UPDATED		    (1<<1)
#define SHELPER_CGAZ			(1<<2)
#define SHELPER_WSW 			(1<<3)
#define SHELPER_SOUND			(1<<4)
#define SHELPER_W				(1<<5)
#define SHELPER_WA				(1<<6)
#define SHELPER_WD				(1<<7)
#define SHELPER_A				(1<<8)
#define SHELPER_D				(1<<9)
#define SHELPER_REAR			(1<<10)
#define SHELPER_CENTER			(1<<11)
#define SHELPER_ACCELMETER		(1<<12)
#define SHELPER_WEZE			(1<<13)
#define SHELPER_CROSSHAIR		(1<<14)
#define SHELPER_S				(1<<15)
#define SHELPER_SA				(1<<16)
#define SHELPER_SD				(1<<17)
#define SHELPER_TINY		    (1<<18)
#define SHELPER_MAX 			(1<<19)
#define SHELPER_ACCELZONES      (1<<20)

#define SPEEDOMETER_ENABLE			(1<<0)
#define SPEEDOMETER_GROUNDSPEED		(1<<1)
#define SPEEDOMETER_JUMPHEIGHT		(1<<2)
#define SPEEDOMETER_JUMPDISTANCE	(1<<3)
#define SPEEDOMETER_VERTICALSPEED	(1<<4)
#define SPEEDOMETER_YAWSPEED		(1<<5)
#define SPEEDOMETER_ACCELMETER		(1<<6)
#define SPEEDOMETER_SPEEDGRAPH		(1<<7)
#define SPEEDOMETER_KPH				(1<<8)
#define SPEEDOMETER_MPH				(1<<9)
#define SPEEDOMETER_JUMPS			(1<<10)
#define SPEEDOMETER_COLORS          (1<<11)
#define SPEEDOMETER_JUMPSCOLORS1    (1<<12)
#define SPEEDOMETER_JUMPSCOLORS2    (1<<13)
#define SPEEDOMETER_SPEEDGRAPHOLD   (1<<14)
#define SPEEDOMETER_XYZ   			(1<<15)

//Miscellaneous
#define SPEEDOMETER_NUM_SAMPLES 500
#define SPEEDOMETER_MIN_RANGE 900
#define SPEED_MED 1000.f
#define SPEED_FAST 1600.f
#define ACCEL_SAMPLES 16
#define PERCENT_SAMPLES 16

#define VectorLerp( f, s, e, r ) ((r)[0]=(s)[0]+(f)*((e)[0]-(s)[0]),\
  (r)[1]=(s)[1]+(f)*((e)[1]-(s)[1]),\
  (r)[2]=(s)[2]+(f)*((e)[2]-(s)[2]))
#define YAW_FRAMES    16
#define	SPEED_SAMPLES		256

#define GAZ_MIN 0
#define GAZ_OPT 1
#define GAZ_MAX 2
#define GAZ_MAX_COS 3
#define GAZ_CENTER 4

/* Structs */
typedef struct {
	float x;    // horiz position
	float y;    // vert position
	float w;    // width
	float h;    // height;
} rectDef_c;

//Strafehelper states

typedef struct {
	float           stopspeed;
	float           duckscale;
	float           swimscale;
	float           wadescale;
	float           accelerate;
	float           airaccelerate;
	float           wateraccelerate;
	float           flyaccelerate;
	float           friction;
	float           waterfriction;
	float           flightfriction;
	float           airdecelrate;
	float           airstopaccelerate;
	float           airstrafewishspeed;
	float           airstrafeaccelerate;
	float           aircontrol;
	qboolean        hasAirControl;
	qboolean        hasForceJumps;
	qboolean        hasAutoJump;
}dfstyle; //movement pysics constants

typedef struct {
	float           frametime;
	float           wishspeed;
	vec3_t          velocityAngles;
	float           currentSpeed;
	qboolean        wasOnGround;

	float 			v;
	float			vxyz;
	float 			vf;
	float 			a;
	float 			s;

	float 			d_min;
	float 			d_opt;
	float 			d_max_cos;
	float 			d_max;
	float 			d_vel;

	vec2_t 			wishvel;
}dfcgaz; //values used for cgaz and its caluclations

typedef struct {
	vec4_t color;
	qboolean active;
	float angle;
	qboolean onScreen;
	float x;
	float y;
	vec3_t angs;
}dfsline;

typedef struct {
	float speed;
}dfspeedo;

typedef struct {
	float lineWidth;
	int sensitivity;
	int LINE_HEIGHT;
	vec4_t twoKeyColor;
	vec4_t oneKeyColor;
	vec4_t oneKeyColorAlt;
	vec4_t rearColor;
	vec4_t activeColor;
	qboolean rear;
	qboolean center;
	qboolean max;
	qboolean triangles;
	float offset;
}dfshelp;

//snaphud start
#define SNAPHUD_MAXZONES	128

typedef struct {
	float			speed;
	float		zones[SNAPHUD_MAXZONES];
	int			count;
	vec2_t		m;
	qboolean	promode;
	int 		fps;
} dfsnaphud;

typedef struct {
	float 		minLine;
	float 		minLineRear;
	float		optLine;
	float		optLineRear;
	float 		maxCosLine;
	float 		maxCosLineRear;
	float 		maxLine;
	float 		maxLineRear;
} dfcgazhud;

typedef struct {
	qboolean        onGround;
	qboolean 		onSlick;
	vec_t* 			velocity;
	usercmd_t       cmd;
	int             moveStyle;
	dfstyle         physics;
	int             moveDir;
	vec3_t          viewAngles;
	vec3_t          viewOrg;
	dfshelp         strafeHelper;
	dfcgaz          cgaz;
	dfsnaphud		snappinghud;
	dfcgazhud 		cgazhud;
	qboolean		racemode;
	dfspeedo 		speedometer;
	float 			speed;
	int 			m_iVehicleNum;
	centity_t* 		vehCent;
	int 			commandTime;
	int 			pm_type;
	int 			pm_time;
	int 			groundEntityNum;
	float 			vertspeed;
	int 			clientnum;
	int 			duelTime;
} dfstate;

typedef struct {
	int		frameSamples[SPEED_SAMPLES];
	int		frameCount;
} speedgraph_t;

/* Strafe HUD Init */
void DF_DrawStrafeHUD(centity_t* cent);
void DF_DrawShowPos();

/* Playerstate Init */

void DF_StrafeHelper();
int DF_SetPlayerState(centity_t* cent);
void DF_SetClientCmdReal();
void DF_SetClientCmd(centity_t* cent);
void DF_SetPhysics();

/* Cgaz Init */
void DF_SetCGAZ();
void DF_SetFrameTime();
void DF_SetVelocityAngles();

/* Speedometer Init */
void DF_SetSpeedometer();

/* Strafehelper functions */
void DF_SetStrafeHelper();
dfsline DF_GetLine(int moveDir, qboolean rear, int gazLine, qboolean fake);
void DF_SetAngleToX(dfsline* inLine);
void DF_SetLineColor(dfsline* inLine, int moveDir, int gazLine);

/* Cgaz functions */
float CGAZ_Min(qboolean onGround, float v, float vf, float a, float s);
float CGAZ_Opt(qboolean onGround, float v, float vf, float a, float s);
float CGAZ_Max(qboolean onGround, float v, float vf, float a, float s);
float CGAZ_Max_Cos(qboolean onGround, float v, float vf, float a, float s);
float DF_GetWishspeed(usercmd_t inCmd);
float DF_GetCmdScale(usercmd_t cmd);

/* Strafe Helper Functions */
void DF_DrawStrafeLine(dfsline line);
void DF_DrawLine(float x1, float y1, float x2, float y2, float size, vec4_t color, float ycutoff);
void DF_DrawStrafehelperWeze(int moveDir);
void DF_StrafeHelperSound(float difference);
void DF_SetAccelColor();
void DF_DrawTriangle(float start, float end);
void DF_DrawZone(float start, float end);


/* Speedometer Functions */
void DF_GraphAddSpeed();
void DF_DrawSpeedGraph(rectDef_c* rect, const vec4_t foreColor, vec4_t backColor);
void DF_DrawSpeedGraphOld();
void DF_DrawJumpHeight();
void DF_DrawJumpDistance();
void DF_DrawVerticalSpeed();
void DF_DrawAccelMeter();
void DF_DrawSpeedometer();

void DF_RaceTimer();
void DF_DrawYawSpeed();

/* Misc Functions */
float DF_GetGroundDistance();
qboolean DF_IsSlickSurf();
void DF_DrawMovementKeys();

usercmd_t DF_DirToCmd(int moveDir);

void DF_DrawSnapHud();
void DF_DrawPitchHud(float pitch);

float speedometerXPos, jumpsXPos;
float firstSpeed;
float speedSamples[SPEEDOMETER_NUM_SAMPLES];
int oldestSpeedSample;
int maxSpeedSample;
dfstate state;
speedgraph_t speedgraph;

#endif //ETERNALJK_HUD_STRAFEHELPER_H
