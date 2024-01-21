/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2015, OpenJK contributors

This file is part of the OpenJK source code.

OpenJK is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/

// cg_localents.c -- every frame, generate renderer commands for locally
// processed entities, like smoke puffs, gibs, shells, etc.

#include "cg_local.h"

#define	MAX_LOCAL_ENTITIES	2048 // 512
localEntity_t	cg_localEntities[MAX_LOCAL_ENTITIES];
localEntity_t	cg_activeLocalEntities;		// double linked list
localEntity_t	*cg_freeLocalEntities;		// single linked list

#if _NEWTRAILS
#define	_OPTIMIZETRAIL	0
#define	MAX_STRAFE_TRAILS	1024*16
strafeTrail_t	cg_strafeTrails[MAX_STRAFE_TRAILS]; //Seperate array for just strafetrails to save space i guess, so we can have a ton of them
strafeTrail_t	cg_activeStrafeTrails;		// double linked list
strafeTrail_t	*cg_freeStrafeTrails;		// single linked list
strafeTrailRef_t tempRefEntity;
#if _OPTIMIZETRAIL
strafeTrail_t	cg_lastStrafeTrail;			// k
#endif
#endif

/*
===================
CG_InitLocalEntities

This is called at startup and for tournament restarts
===================
*/
void	CG_InitLocalEntities( void ) {
	int		i;

	memset( cg_localEntities, 0, sizeof( cg_localEntities ) );
	cg_activeLocalEntities.next = &cg_activeLocalEntities;
	cg_activeLocalEntities.prev = &cg_activeLocalEntities;
	cg_freeLocalEntities = cg_localEntities;
	for ( i = 0 ; i < MAX_LOCAL_ENTITIES - 1 ; i++ ) {
		cg_localEntities[i].next = &cg_localEntities[i+1];
	}
}


/*
==================
CG_FreeLocalEntity
==================
*/
void CG_FreeLocalEntity( localEntity_t *le ) {
	if ( !le->prev ) {
		trap->Error( ERR_DROP, "CG_FreeLocalEntity: not active" );
		return;
	}

	// remove from the doubly linked active list
	le->prev->next = le->next;
	le->next->prev = le->prev;

	// the free list is only singly linked
	le->next = cg_freeLocalEntities;
	cg_freeLocalEntities = le;
}

/*
===================
CG_AllocLocalEntity

Will allways succeed, even if it requires freeing an old active entity
===================
*/
localEntity_t	*CG_AllocLocalEntity( void ) {
	localEntity_t	*le;

	if ( !cg_freeLocalEntities ) {
		// no free entities, so free the one at the end of the chain
		// remove the oldest active entity
		CG_FreeLocalEntity( cg_activeLocalEntities.prev );
	}

	le = cg_freeLocalEntities;
	cg_freeLocalEntities = cg_freeLocalEntities->next;

	memset( le, 0, sizeof( *le ) );

	// link into the active list
	le->next = cg_activeLocalEntities.next;
	le->prev = &cg_activeLocalEntities;
	cg_activeLocalEntities.next->prev = le;
	cg_activeLocalEntities.next = le;
	return le;
}


/*
====================================================================================

FRAGMENT PROCESSING

A fragment localentity interacts with the environment in some way (hitting walls),
or generates more localentities along a trail.

====================================================================================
*/

/*
================
CG_BloodTrail

Leave expanding blood puffs behind gibs
================
*/
void CG_BloodTrail( localEntity_t *le ) {
	int		t;
	int		t2;
	int		step;
	vec3_t	newOrigin;
	localEntity_t	*blood;

	step = 150;
	t = step * ( (cg.time - cg.frametime + step ) / step );
	t2 = step * ( cg.time / step );

	for ( ; t <= t2; t += step ) {
		BG_EvaluateTrajectory( &le->pos, t, newOrigin );

		blood = CG_SmokePuff( newOrigin, vec3_origin,
					  20,		// radius
					  1, 1, 1, 1,	// color
					  2000,		// trailTime
					  t,		// startTime
					  0,		// fadeInTime
					  0,		// flags
					  cgs.media.bloodTrailShader );
		// use the optimized version
		blood->leType = LE_FALL_SCALE_FADE;
		// drop a total of 40 units over its lifetime
		blood->pos.trDelta[2] = 40;
	}
}


/*
================
CG_FragmentBounceMark
================
*/
void CG_FragmentBounceMark( localEntity_t *le, trace_t *trace ) {
//	int radius;

	if ( le->leMarkType == LEMT_BLOOD ) {
	//	radius = 16 + (rand()&31);
	//	CG_ImpactMark( cgs.media.bloodMarkShader, trace->endpos, trace->plane.normal, Q_flrand(0.0f, 1.0f)*360, 1,1,1,1, qtrue, radius, qfalse );
	} else if ( le->leMarkType == LEMT_BURN ) {
	//	radius = 8 + (rand()&15);
	//	CG_ImpactMark( cgs.media.burnMarkShader, trace->endpos, trace->plane.normal, Q_flrand(0.0f, 1.0f)*360, 1,1,1,1, qtrue, radius, qfalse );
	}

	// don't allow a fragment to make multiple marks, or they pile up while settling
	le->leMarkType = LEMT_NONE;
}

/*
================
CG_FragmentBounceSound
================
*/
void CG_FragmentBounceSound( localEntity_t *le, trace_t *trace ) {
	// half the fragments will make a bounce sounds
	if ( rand() & 1 )
	{
		sfxHandle_t	s = 0;

		switch( le->leBounceSoundType )
		{
		case LEBS_ROCK:
			s = cgs.media.rockBounceSound[Q_irand(0,1)];
			break;
		case LEBS_METAL:
			s = cgs.media.metalBounceSound[Q_irand(0,1)];// FIXME: make sure that this sound is registered properly...might still be rock bounce sound....
			break;
		case LEBS_BLOOD:
			// half the gibs will make splat sounds
			if ( rand() & 1 ) {
				int r = rand()&3;
				sfxHandle_t	s;

				if ( r == 0 ) {
					s = cgs.media.gibBounce1Sound;
				} else if ( r == 1 ) {
					s = cgs.media.gibBounce2Sound;
				} else {
					s = cgs.media.gibBounce3Sound;
				}
				trap->S_StartSound( trace->endpos, ENTITYNUM_WORLD, CHAN_AUTO, s );
			}
		default:
			return;
		}

		if ( s )
		{
			trap->S_StartSound( trace->endpos, ENTITYNUM_WORLD, CHAN_AUTO, s );
		}

		// bouncers only make the sound once...
		// FIXME: arbitrary...change if it bugs you
		le->leBounceSoundType = LEBS_NONE;
	}
	else if ( rand() & 1 )
	{
		// we may end up bouncing again, but each bounce reduces the chance of playing the sound again or they may make a lot of noise when they settle
		// FIXME: maybe just always do this??
		le->leBounceSoundType = LEBS_NONE;
	}
}


/*
================
CG_ReflectVelocity
================
*/
void CG_ReflectVelocity( localEntity_t *le, trace_t *trace ) {
	vec3_t	velocity;
	float	dot;
	int		hitTime;

	// reflect the velocity on the trace plane
	hitTime = cg.time - cg.frametime + cg.frametime * trace->fraction;
	BG_EvaluateTrajectoryDelta( &le->pos, hitTime, velocity );
	dot = DotProduct( velocity, trace->plane.normal );
	VectorMA( velocity, -2*dot, trace->plane.normal, le->pos.trDelta );

	VectorScale( le->pos.trDelta, le->bounceFactor, le->pos.trDelta );

	VectorCopy( trace->endpos, le->pos.trBase );
	le->pos.trTime = cg.time;

	// check for stop, making sure that even on low FPS systems it doesn't bobble
	if ( trace->allsolid ||
		( trace->plane.normal[2] > 0 &&
		( le->pos.trDelta[2] < 40 || le->pos.trDelta[2] < -cg.frametime * le->pos.trDelta[2] ) ) ) {
		le->pos.trType = TR_STATIONARY;
	} else {

	}
}

/*
================
CG_AddFragment
================
*/
void CG_AddFragment( localEntity_t *le ) {
	vec3_t	newOrigin;
	trace_t	trace;

	if (le->forceAlpha)
	{
		le->refEntity.renderfx |= RF_FORCE_ENT_ALPHA;
		le->refEntity.shaderRGBA[3] = le->forceAlpha;
	}

	if ( le->pos.trType == TR_STATIONARY ) {
		// sink into the ground if near the removal time
		int		t;
		float	t_e;

		t = le->endTime - cg.time;
		if ( t < (SINK_TIME*2) ) {
			le->refEntity.renderfx |= RF_FORCE_ENT_ALPHA;
			t_e = (float)((float)(le->endTime - cg.time)/(SINK_TIME*2));
			t_e = (int)((t_e)*255);

			if (t_e > 255)
			{
				t_e = 255;
			}
			if (t_e < 1)
			{
				t_e = 1;
			}

			if (le->refEntity.shaderRGBA[3] && t_e > le->refEntity.shaderRGBA[3])
			{
				t_e = le->refEntity.shaderRGBA[3];
			}

			le->refEntity.shaderRGBA[3] = t_e;

			trap->R_AddRefEntityToScene( &le->refEntity );
		} else {
			trap->R_AddRefEntityToScene( &le->refEntity );
		}

		return;
	}

	// calculate new position
	BG_EvaluateTrajectory( &le->pos, cg.time, newOrigin );

	// trace a line from previous position to new position
	CG_Trace( &trace, le->refEntity.origin, NULL, NULL, newOrigin, -1, CONTENTS_SOLID );
	if ( trace.fraction == 1.0 ) {
		// still in free fall
		VectorCopy( newOrigin, le->refEntity.origin );

		if ( le->leFlags & LEF_TUMBLE ) {
			vec3_t angles;

			BG_EvaluateTrajectory( &le->angles, cg.time, angles );
			AnglesToAxis( angles, le->refEntity.axis );
			ScaleModelAxis(&le->refEntity);
		}

		trap->R_AddRefEntityToScene( &le->refEntity );

		// add a blood trail
		if ( le->leBounceSoundType == LEBS_BLOOD ) {
			CG_BloodTrail( le );
		}

		return;
	}

	// if it is in a nodrop zone, remove it
	// this keeps gibs from waiting at the bottom of pits of death
	// and floating levels
	if ( CG_PointContents( trace.endpos, 0 ) & CONTENTS_NODROP ) {
		CG_FreeLocalEntity( le );
		return;
	}

	if (!trace.startsolid)
	{
		// leave a mark
		CG_FragmentBounceMark( le, &trace );

		// do a bouncy sound
		CG_FragmentBounceSound( le, &trace );

		if (le->bounceSound)
		{ //specified bounce sound (debris)
			trap->S_StartSound(le->pos.trBase, ENTITYNUM_WORLD, CHAN_AUTO, le->bounceSound);
		}

		// reflect the velocity on the trace plane
		CG_ReflectVelocity( le, &trace );

		trap->R_AddRefEntityToScene( &le->refEntity );
	}
}

/*
=====================================================================

TRIVIAL LOCAL ENTITIES

These only do simple scaling or modulation before passing to the renderer
=====================================================================
*/

/*
====================
CG_AddFadeRGB
====================
*/
void CG_AddFadeRGB( localEntity_t *le ) {
	refEntity_t *re;
	float c;

	re = &le->refEntity;

	c = ( le->endTime - cg.time ) * le->lifeRate;
	c *= 0xff;

	re->shaderRGBA[0] = le->color[0] * c;
	re->shaderRGBA[1] = le->color[1] * c;
	re->shaderRGBA[2] = le->color[2] * c;
	re->shaderRGBA[3] = le->color[3] * c;

	trap->R_AddRefEntityToScene( re );
}

static void CG_AddFadeScaleModel( localEntity_t *le )
{
	refEntity_t	*ent = &le->refEntity;

	float frac = ( cg.time - le->startTime )/((float)( le->endTime - le->startTime ));

	frac *= frac * frac; // yes, this is completely ridiculous...but it causes the shell to grow slowly then "explode" at the end

	ent->nonNormalizedAxes = qtrue;

	AxisCopy( axisDefault, ent->axis );

	VectorScale( ent->axis[0], le->radius * frac, ent->axis[0] );
	VectorScale( ent->axis[1], le->radius * frac, ent->axis[1] );
	VectorScale( ent->axis[2], le->radius * 0.5f * frac, ent->axis[2] );

	frac = 1.0f - frac;

	ent->shaderRGBA[0] = le->color[0] * frac;
	ent->shaderRGBA[1] = le->color[1] * frac;
	ent->shaderRGBA[2] = le->color[2] * frac;
	ent->shaderRGBA[3] = le->color[3] * frac;

	// add the entity
	trap->R_AddRefEntityToScene( ent );
}

/*
==================
CG_AddMoveScaleFade
==================
*/
static void CG_AddMoveScaleFade( localEntity_t *le ) {
	refEntity_t	*re;
	float		c;
	vec3_t		delta;
	float		len;

	re = &le->refEntity;

	if ( le->fadeInTime > le->startTime && cg.time < le->fadeInTime ) {
		// fade / grow time
		c = 1.0 - (float) ( le->fadeInTime - cg.time ) / ( le->fadeInTime - le->startTime );
	}
	else {
		// fade / grow time
		c = ( le->endTime - cg.time ) * le->lifeRate;
	}

	re->shaderRGBA[3] = 0xff * c * le->color[3];

	if ( !( le->leFlags & LEF_PUFF_DONT_SCALE ) ) {
		re->radius = le->radius * ( 1.0 - c ) + 8;
	}

	BG_EvaluateTrajectory( &le->pos, cg.time, re->origin );

	// if the view would be "inside" the sprite, kill the sprite
	// so it doesn't add too much overdraw
	VectorSubtract( re->origin, cg.refdef.vieworg, delta );
	len = VectorLength( delta );
	if ( len < le->radius ) {
		CG_FreeLocalEntity( le );
		return;
	}

	trap->R_AddRefEntityToScene( re );
}

/*
==================
CG_AddPuff
==================
*/
static void CG_AddPuff( localEntity_t *le ) {
	refEntity_t	*re;
	float		c;
	vec3_t		delta;
	float		len;

	re = &le->refEntity;

	// fade / grow time
	c = ( le->endTime - cg.time ) / (float)( le->endTime - le->startTime );

	re->shaderRGBA[0] = le->color[0] * c;
	re->shaderRGBA[1] = le->color[1] * c;
	re->shaderRGBA[2] = le->color[2] * c;

	if ( !( le->leFlags & LEF_PUFF_DONT_SCALE ) ) {
		re->radius = le->radius * ( 1.0 - c ) + 8;
	}

	BG_EvaluateTrajectory( &le->pos, cg.time, re->origin );

	// if the view would be "inside" the sprite, kill the sprite
	// so it doesn't add too much overdraw
	VectorSubtract( re->origin, cg.refdef.vieworg, delta );
	len = VectorLength( delta );
	if ( len < le->radius ) {
		CG_FreeLocalEntity( le );
		return;
	}

	trap->R_AddRefEntityToScene( re );
}

/*
===================
CG_AddScaleFade

For rocket smokes that hang in place, fade out, and are
removed if the view passes through them.
There are often many of these, so it needs to be simple.
===================
*/
static void CG_AddScaleFade( localEntity_t *le ) {
	refEntity_t	*re;
	float		c;
	vec3_t		delta;
	float		len;

	re = &le->refEntity;

	// fade / grow time
	c = ( le->endTime - cg.time ) * le->lifeRate;

	re->shaderRGBA[3] = 0xff * c * le->color[3];
	re->radius = le->radius * ( 1.0 - c ) + 8;

	// if the view would be "inside" the sprite, kill the sprite
	// so it doesn't add too much overdraw
	VectorSubtract( re->origin, cg.refdef.vieworg, delta );
	len = VectorLength( delta );
	if ( len < le->radius ) {
		CG_FreeLocalEntity( le );
		return;
	}

	trap->R_AddRefEntityToScene( re );
}


/*
=================
CG_AddFallScaleFade

This is just an optimized CG_AddMoveScaleFade
For blood mists that drift down, fade out, and are
removed if the view passes through them.
There are often 100+ of these, so it needs to be simple.
=================
*/
static void CG_AddFallScaleFade( localEntity_t *le ) {
	refEntity_t	*re;
	float		c;
	vec3_t		delta;
	float		len;

	re = &le->refEntity;

	// fade time
	c = ( le->endTime - cg.time ) * le->lifeRate;

	re->shaderRGBA[3] = 0xff * c * le->color[3];

	re->origin[2] = le->pos.trBase[2] - ( 1.0 - c ) * le->pos.trDelta[2];

	re->radius = le->radius * ( 1.0 - c ) + 16;

	// if the view would be "inside" the sprite, kill the sprite
	// so it doesn't add too much overdraw
	VectorSubtract( re->origin, cg.refdef.vieworg, delta );
	len = VectorLength( delta );
	if ( len < le->radius ) {
		CG_FreeLocalEntity( le );
		return;
	}

	trap->R_AddRefEntityToScene( re );
}



/*
================
CG_AddExplosion
================
*/
static void CG_AddExplosion( localEntity_t *ex ) {
	refEntity_t	*ent;

	ent = &ex->refEntity;

	// add the entity
	trap->R_AddRefEntityToScene(ent);

	// add the dlight
	if ( ex->light ) {
		float		light;

		light = (float)( cg.time - ex->startTime ) / ( ex->endTime - ex->startTime );
		if ( light < 0.5 ) {
			light = 1.0;
		} else {
			light = 1.0 - ( light - 0.5 ) * 2;
		}
		light = ex->light * light;
		trap->R_AddLightToScene(ent->origin, light, ex->lightColor[0], ex->lightColor[1], ex->lightColor[2] );
	}
}

/*
================
CG_AddSpriteExplosion
================
*/
static void CG_AddSpriteExplosion( localEntity_t *le ) {
	refEntity_t	re;
	float c;

	re = le->refEntity;

	c = ( le->endTime - cg.time ) / ( float ) ( le->endTime - le->startTime );
	if ( c > 1 ) {
		c = 1.0;	// can happen during connection problems
	}

	re.shaderRGBA[0] = 0xff;
	re.shaderRGBA[1] = 0xff;
	re.shaderRGBA[2] = 0xff;
	re.shaderRGBA[3] = 0xff * c * 0.33;

	re.reType = RT_SPRITE;
	re.radius = 42 * ( 1.0 - c ) + 30;

	trap->R_AddRefEntityToScene( &re );

	// add the dlight
	if ( le->light ) {
		float		light;

		light = (float)( cg.time - le->startTime ) / ( le->endTime - le->startTime );
		if ( light < 0.5 ) {
			light = 1.0;
		} else {
			light = 1.0 - ( light - 0.5 ) * 2;
		}
		light = le->light * light;
		trap->R_AddLightToScene(re.origin, light, le->lightColor[0], le->lightColor[1], le->lightColor[2] );
	}
}


/*
===================
CG_AddRefEntity
===================
*/
void CG_AddRefEntity( localEntity_t *le ) {
	if (le->endTime < cg.time) {
		CG_FreeLocalEntity( le );
		return;
	}
	trap->R_AddRefEntityToScene( &le->refEntity );
}

/*
===================
CG_AddScorePlum
===================
*/
#define NUMBER_SIZE		8

void CG_AddScorePlum( localEntity_t *le ) {
	refEntity_t	*re;
	vec3_t		origin, delta, dir, vec, up = {0, 0, 1};
	float		c = 0, len;
	int			i, score, digits[10], numdigits, negative;
	qboolean	strafeTrailNum = qfalse;

	if (le->lifeRate == 0) {
		strafeTrailNum = qtrue;
	}

	if (strafeTrailNum) {
		vec3_t diff;
		VectorSubtract(cg.refdef.vieworg, le->pos.trBase, diff);

		if (cg_strafeTrailGhost.integer > 1)
			return;

		if (diff[2] > 2048 || diff[2] < -8192) { //Ditch trails that are 2048 above us or 8192 below us
			return;
		}
		if ((VectorLengthSquared(diff) > 4096*4096) /*|| !trap->R_inPVS(cg.refdef.vieworg, trail->start, cg.snap->areamask)*/) {
			return;
		}
	}

	re = &le->refEntity;

	score = le->radius;
	if (strafeTrailNum) { //its a strafetrail num, why the fuck wont this work
		re->shaderRGBA[0] = 255;
		re->shaderRGBA[1] = 0;
		re->shaderRGBA[2] = 0;
	}
	else if (score < 0) {
		re->shaderRGBA[0] = 0xff;
		re->shaderRGBA[1] = 0x11;
		re->shaderRGBA[2] = 0x11;
	}
	else {
		re->shaderRGBA[0] = 0xff;
		re->shaderRGBA[1] = 0xff;
		re->shaderRGBA[2] = 0xff;
		if (score >= 50) {
			re->shaderRGBA[1] = 0;
		} else if (score >= 20) {
			re->shaderRGBA[0] = re->shaderRGBA[1] = 0;
		} else if (score >= 10) {
			re->shaderRGBA[2] = 0;
		} else if (score >= 2) {
			re->shaderRGBA[0] = re->shaderRGBA[2] = 0;
		}

	}

	if (strafeTrailNum) {
		re->shaderRGBA[3] = 0xff;
	}
	else {
		c = ( le->endTime - cg.time ) * le->lifeRate;
		if (c < 0.25)
			re->shaderRGBA[3] = 0xff * 4 * c;
		else
			re->shaderRGBA[3] = 0xff;
	}

	re->radius = NUMBER_SIZE / 2;

	VectorCopy(le->pos.trBase, origin);

	if (!strafeTrailNum) {
		origin[2] += 110 - c * 100;
	}

	VectorSubtract(cg.refdef.vieworg, origin, dir);
	CrossProduct(dir, up, vec);
	VectorNormalize(vec);

	VectorMA(origin, -10 + 20 * sin(c * 2 * M_PI), vec, origin);

	// if the view would be "inside" the sprite, kill the sprite
	// so it doesn't add too much overdraw
	if (!strafeTrailNum) {
		VectorSubtract( origin, cg.refdef.vieworg, delta );

		len = VectorLength( delta );
		if ( len < 20 ) {
			CG_FreeLocalEntity( le );
			return;
		}
	}

	negative = qfalse;
	if (score < 0) {
		negative = qtrue;
		score = -score;
	}

	for (numdigits = 0; !(numdigits && !score); numdigits++) {
		digits[numdigits] = score % 10;
		score = score / 10;
	}

	if (negative) {
		digits[numdigits] = 10;
		numdigits++;
	}

	for (i = 0; i < numdigits; i++) {
		VectorMA(origin, (float) (((float) numdigits / 2) - i) * NUMBER_SIZE, vec, re->origin);
		re->customShader = cgs.media.numberShaders[digits[numdigits-1-i]];
		trap->R_AddRefEntityToScene( re );
	}
}

void CG_AddSpotIcon( localEntity_t *le ) {
	refEntity_t		ent;
	float distance;
	vec3_t diff;
	int add;

	memset( &ent, 0, sizeof( ent ) );
	VectorCopy( le->pos.trBase, ent.origin );
	ent.origin[2] += 48;
	ent.reType = RT_SPRITE;
	ent.renderfx = RF_NODEPTH;
	ent.shaderRGBA[0] = 255;
	ent.shaderRGBA[1] = 255;
	ent.shaderRGBA[2] = 255;
	ent.shaderRGBA[3] = 255;

	VectorSubtract(cg.predictedPlayerState.origin, le->pos.trBase, diff);
	distance = VectorLength(diff);

	add = distance / 50; //What a gross hack this is
	ent.radius = 5 + add;

	if (cg.predictedPlayerState.persistant[PERS_TEAM] == TEAM_RED)
		ent.customShader = cgs.media.teamBlueShader;
	else
		ent.customShader = cgs.media.teamRedShader;

	trap->R_AddRefEntityToScene( &ent );
}

/*
===================
CG_AddOLine

For forcefields/other rectangular things
===================
*/
void CG_AddOLine( localEntity_t *le )
{
	refEntity_t	*re;
	float		frac, alpha;

	re = &le->refEntity;

	frac = (cg.time - le->startTime) / ( float ) ( le->endTime - le->startTime );
	if ( frac > 1 )
		frac = 1.0;	// can happen during connection problems
	else if (frac < 0)
		frac = 0.0;

	// Use the liferate to set the scale over time.
	re->data.line.width = le->data.line.width + (le->data.line.dwidth * frac);
	if (re->data.line.width <= 0)
	{
		CG_FreeLocalEntity( le );
		return;
	}

	// We will assume here that we want additive transparency effects.
	alpha = le->alpha + (le->dalpha * frac);
	re->shaderRGBA[0] = 0xff * alpha;
	re->shaderRGBA[1] = 0xff * alpha;
	re->shaderRGBA[2] = 0xff * alpha;
	re->shaderRGBA[3] = 0xff * alpha;	// Yes, we could apply c to this too, but fading the color is better for lines.

	re->shaderTexCoord[0] = 1;
	re->shaderTexCoord[1] = 1;

	re->rotation = 90;

	re->reType = RT_ORIENTEDLINE;

	trap->R_AddRefEntityToScene( re );
}

/*
===================
CG_AddLine

for beams and the like.
===================
*/
void CG_AddLine( localEntity_t *le )
{
	refEntity_t	*re;

	re = &le->refEntity;

	re->reType = RT_LINE;

	trap->R_AddRefEntityToScene( re );
}

void CG_AddMissile(localEntity_t *le) {
	vec3_t	currentPos;
	int		weapon;
	qboolean altFire;

	if (le->leFlags % 2) {
		weapon = (le->leFlags - 1) / 2;
		altFire = qtrue;
	}
	else {
		weapon = (le->leFlags / 2);
		altFire = qfalse;
	}

	BG_EvaluateTrajectory(&le->pos, cg.time, currentPos); //Is muzzlepoint accurate?
	//Com_Printf("Weapon is %i and altfire is %i, flags was %i\n", weapon, altFire, le->leFlags);

	switch (weapon) {
		default:
			return;
		case WP_BRYAR_PISTOL:
		case WP_BRYAR_OLD:
			trap->FX_PlayEffectID(cgs.effects.bryarShotEffect, currentPos, le->angles.trBase, -1, -1, qfalse);
			break;
		case WP_BLASTER:
			trap->FX_PlayEffectID(cgs.effects.blasterShotEffect, currentPos, le->angles.trBase, -1, -1, qfalse);
			break;
		case WP_DISRUPTOR:
			if (cgs.jcinfo & JAPRO_CINFO_PROJSNIPER)
				trap->FX_PlayEffectID(cgs.effects.bryarShotEffect, currentPos, le->angles.trBase, -1, -1, qfalse);
			break;
		case WP_BOWCASTER:
			trap->FX_PlayEffectID(cgs.effects.bowcasterShotEffect, currentPos, le->angles.trBase, -1, -1, qfalse);
			break;
		case WP_REPEATER:
			if (altFire) {
				if (cgs.jcinfo2 & JAPRO_CINFO2_WTTRIBES)
					trap->FX_PlayEffectID(cgs.effects.mortarProjectile, currentPos, le->angles.trBase, -1, -1, qfalse);
				else
					trap->FX_PlayEffectID(cgs.effects.repeaterAltProjectileEffect, currentPos, le->angles.trBase, -1, -1, qfalse);
			}
			else
				trap->FX_PlayEffectID(cgs.effects.repeaterProjectileEffect, currentPos, le->angles.trBase, -1, -1, qfalse);
			break;
		case WP_FLECHETTE:
			if (altFire)
				trap->FX_PlayEffectID(cgs.effects.flechetteAltShotEffect, currentPos, le->angles.trBase, -1, -1, qfalse);
			else
				trap->FX_PlayEffectID(cgs.effects.flechetteShotEffect, currentPos, le->angles.trBase, -1, -1, qfalse);
			break;
		case WP_ROCKET_LAUNCHER:
			trap->FX_PlayEffectID(cgs.effects.rocketShotEffect, currentPos, le->angles.trBase, -1, -1, qfalse);
			break;
		case WP_CONCUSSION:
			trap->FX_PlayEffectID(cgs.effects.concussionShotEffect, currentPos, le->angles.trBase, -1, -1, qfalse);
			break;
	}
}

//==============================================================================

/*
===================
CG_AddLocalEntities

===================
*/
void CG_AddLocalEntities( void ) {
	localEntity_t	*le, *next;

	// walk the list backwards, so any new local entities generated
	// (trails, marks, etc) will be present this frame
	le = cg_activeLocalEntities.prev;
	for ( ; le != &cg_activeLocalEntities ; le = next ) {
		// grab next now, so if the local entity is freed we
		// still have it
		next = le->prev;

		if ( cg.time >= le->endTime ) {
			CG_FreeLocalEntity( le );
			continue;
		}
		switch ( le->leType ) {
		default:
			trap->Error( ERR_DROP, "Bad leType: %i", le->leType );
			break;

		case LE_MARK:
			break;

		case LE_SPRITE_EXPLOSION:
			CG_AddSpriteExplosion( le );
			break;

		case LE_EXPLOSION:
			CG_AddExplosion( le );
			break;

		case LE_FADE_SCALE_MODEL:
			CG_AddFadeScaleModel( le );
			break;

		case LE_FRAGMENT:			// gibs and brass
			CG_AddFragment( le );
			break;

		case LE_PUFF:
			CG_AddPuff( le );
			break;

		case LE_MOVE_SCALE_FADE:		// water bubbles
			CG_AddMoveScaleFade( le );
			break;

		case LE_FADE_RGB:				// teleporters, railtrails
			CG_AddFadeRGB( le );
			break;

		case LE_FALL_SCALE_FADE: // gib blood trails
			CG_AddFallScaleFade( le );
			break;

		case LE_SCALE_FADE:		// rocket trails
			CG_AddScaleFade( le );
			break;

		case LE_SCOREPLUM:
			if (le->radius)
				CG_AddScorePlum( le );
			else
				CG_AddSpotIcon( le );
			break;

		case LE_OLINE:
			CG_AddOLine( le );
			break;

		case LE_SHOWREFENTITY:
			CG_AddRefEntity( le );
			break;

		case LE_LINE:					// oriented lines for FX
			CG_AddLine( le );
			break;

		case LE_MISSILE: //cg_simulatedProjectiles
			CG_AddMissile( le );
			break;
		}
	}
}

#if _NEWTRAILS
void	CG_InitStrafeTrails( void ) {
	int		i;

	memset( cg_strafeTrails, 0, sizeof( cg_strafeTrails ) );
	cg_activeStrafeTrails.next = &cg_activeStrafeTrails;
	cg_activeStrafeTrails.prev = &cg_activeStrafeTrails;
	cg_freeStrafeTrails = cg_strafeTrails;
	for ( i = 0 ; i < MAX_STRAFE_TRAILS - 1 ; i++ ) {
		cg_strafeTrails[i].next = &cg_strafeTrails[i+1];
	}
}

/*
==================
CG_FreeLocalEntity
==================
*/
void CG_FreeStrafeTrail( strafeTrail_t *trail ) {
	if ( !trail->prev ) {
		trap->Error( ERR_DROP, "CG_FreeStrafeTrail: not active" );
		return;
	}

	// remove from the doubly linked active list
	trail->prev->next = trail->next;
	trail->next->prev = trail->prev;

	// the free list is only singly linked
	trail->next = cg_freeStrafeTrails;
	cg_freeStrafeTrails = trail;
}

void CG_RemoveStrafeTrail( int clientNum ) {
	int i;

	if (clientNum == -1)
		cg.drawingStrafeTrails = 0;
	else 
		cg.drawingStrafeTrails &= ~(1 << clientNum); 

	for ( i = 0 ; i < MAX_STRAFE_TRAILS - 1 ; i++ ) {
		if ((cg_strafeTrails[i].clientNum == clientNum+1) || clientNum == -1)
			cg_strafeTrails[i].endTime = cg.time + 100;
	}

	for ( i = 0 ; i < MAX_LOCAL_ENTITIES - 1 ; i++ ) {
		if ( (cg_localEntities[i].leType == LE_SCOREPLUM) &&
			(cg_localEntities[i].lifeRate == 0) &&
			 ((cg_localEntities[i].leFlags == clientNum+1) || (cg_localEntities[i].leFlags > 0 && clientNum == -1)) ) {
				 cg_localEntities[i].endTime = cg.time + 100;
				 //CG_FreeLocalEntity(&cg_localEntities[i]); //just set leTime to 0 instead idk
		}
	}
}

void CG_AddSingleStrafeTrail( strafeTrail_t *trail )
{	
	refEntity_t	*re;
	unsigned int color = trail->color;
	float radius;

	radius = cg_strafeTrailRadius.value;
	if (radius < 0.1f)
		radius = 0.1f;
	else if (radius > 100)
		radius = 100;

	re = &tempRefEntity.refEntity;
	VectorCopy( trail->start, re->origin );
	VectorCopy( trail->end, re->oldorigin);

	re->reType = RT_LINE;
	re->radius = 0.5*radius;
	re->customShader = cgs.media.whiteShader;
	re->shaderTexCoord[0] = re->shaderTexCoord[1] = 1.0f;

	re->shaderRGBA[0] = color & 0xff;
	color >>= 8;
	re->shaderRGBA[1] = color & 0xff;
	color >>= 8;
	re->shaderRGBA[2] = color & 0xff;
	re->shaderRGBA[3] = 0xff;

	trap->R_AddRefEntityToScene( re );
}

#if _OPTIMIZETRAIL
void StrafeTrailCopy(strafeTrail_t *in, strafeTrail_t *out) {
	out->clientNum = in->clientNum;
	out->color = in->color;
	VectorCopy(in->end, out->end);
	out->endTime = in->endTime;
	out->next = in->next;
	out->prev = in->prev;
	VectorCopy(in->start, out->start);
}
#endif

void CG_AddStrafeGhost(vec3_t org) {
	refEntity_t	*re;

	re = &tempRefEntity.refEntity;
	VectorCopy(org, re->origin);
	VectorCopy(org, re->oldorigin);

	re->origin[2] -= 12;
	re->oldorigin[2] += 12;
	re->radius = 12;

	re->shaderRGBA[0] = 255;
	re->shaderRGBA[1] = 0;
	re->shaderRGBA[2] = 0;

	re->reType = RT_LINE;
	re->customShader = cgs.media.whiteShader;
	trap->R_AddRefEntityToScene(re);
}

void CG_AddAllStrafeTrails(void) { //Can this be ignored if we know we are not drawing strafetrails?
	strafeTrail_t	*trail, *next;
	vec3_t distance;
#if _OPTIMIZETRAIL
	vec3_t line1, line2, ang1, ang2;
	float dot;
#endif
	int i = 0;
	const int speed = 1000 / cg_strafeTrailFPS.integer;
	int time = 0;
	qboolean drawn = qfalse;

	if (!cg_strafeTrailGhost.integer || !cg.snap) //||!cg.snap
		drawn = qtrue;
	else {
		time = (cg.time - cg.predictedPlayerState.duelTime); //god damn this
		if (!(cg.predictedPlayerState.pm_flags & PMF_FOLLOW) && (cg.predictedPlayerState.persistant[PERS_TEAM] != TEAM_SPECTATOR))//Ignore ping and frametime if we are in spec.
			time += cg.snap->ping + speed*0.5f;
	}

	trail = cg_activeStrafeTrails.prev;
	for (; trail != &cg_activeStrafeTrails; trail = next) {
		next = trail->prev;
		if (cg.time >= trail->endTime) {
			CG_FreeStrafeTrail(trail);
			continue;
		}

		i++;
		//Test conditions
		VectorSubtract(cg.refdef.vieworg, trail->end, distance);

		if (distance[2] > 2048 || distance[2] < -8192) { //Ditch trails that are 2048 above us or 8192 below us
			continue;
		}
		if (VectorLengthSquared(distance) > 16384 * 16384) {
			continue;
		}
		if (!trap->R_InPVS(cg.refdef.vieworg, trail->end, NULL)) {
			continue;
		}

#if _OPTIMIZETRAIL

		if (cg_draw2D.integer > 1) {
			/*
			At node n, get line1 as n, n+1
			get line2 as n, n+2

			get dotproduct of line1, line2
			if small, skip

			if big, keep

			if skip, draw line as n,n+2
			if keep, draw line as n,n+1

			if skip, increment n an extra time?
			*/

			VectorSubtract(trail->start, trail->end, line1);
			VectorSubtract(trail->end, next->start, line2);

			vectoangles(line1, ang1);
			vectoangles(line2, ang2);

			VectorNormalize(ang1);
			VectorNormalize(ang2);

			dot = DotProduct(ang1, ang2);

			if (cg_draw2D.integer > 2)
				Com_Printf("Dotproduct is %.2f\n", dot);

			if (dot > cg_thirdPersonFlagAlpha.value) { //0.9f) 
				strafeTrail_t tempTrail;

				StrafeTrailCopy(trail, &tempTrail);
				VectorCopy(next->start, tempTrail.end);
				trail->color = 0x8B0000; //test..

				CG_AddSingleStrafeTrail(&tempTrail);
				trail = next; //Increment node..
			}
			else {
				CG_AddSingleStrafeTrail(trail);
			}

		}

		/*
		if (cg_draw2D.integer > 1) {
		strafeTrail_t tempTrail;
		StrafeTrailCopy(trail, &tempTrail);
		if (!cg_lastStrafeTrail.endTime) {
		StrafeTrailCopy(trail, &cg_lastStrafeTrail);
		Com_Printf("Initializing last strafetrail\n");
		}
		else {
		VectorSubtract(trail->end, trail->start, line1);
		VectorSubtract(trail->start, cg_lastStrafeTrail.end, line2);

		vectoangles(line1, ang1);
		vectoangles(line2, ang2);

		VectorNormalize(ang1);
		VectorNormalize(ang2);

		dot = DotProduct(ang1, ang2);

		if (cg_draw2D.integer > 2)
		Com_Printf("Dotproduct is %.2f\n", dot);

		if (dot > cg_thirdPersonFlagAlpha.value) { //0.9f) {//angle between trail and last trail is very small, and trail is far away //SKIP

		VectorCopy(cg_lastStrafeTrail.start, tempTrail.start);
		StrafeTrailCopy(trail, &cg_lastStrafeTrail);

		CG_AddSingleStrafeTrail(&tempTrail);
		trail = next;

		continue;
		}
		}

		CG_AddSingleStrafeTrail(&tempTrail);
		}
		*/
		else
#endif
		if (cg_strafeTrailGhost.integer < 2)
			CG_AddSingleStrafeTrail(trail);

		if (!drawn && time < (i*speed)) {
			CG_AddStrafeGhost(trail->start);
			drawn = qtrue;
		}
	}
}

strafeTrail_t	*CG_AllocStrafeTrail( void ) {
	strafeTrail_t	*trail;

	if ( !cg_freeStrafeTrails ) {
		// no free entities, so free the one at the end of the chain
		// remove the oldest active entity
		//Com_Printf("Out of space, freeing trail at %.1f, %.1f\n", cg_activeStrafeTrails.start[0], cg_activeStrafeTrails.start[1]);
		CG_FreeStrafeTrail( cg_activeStrafeTrails.prev );  //why this doesnt seem to work sometimes?
	}

	trail = cg_freeStrafeTrails;
	cg_freeStrafeTrails = cg_freeStrafeTrails->next;

	memset( trail, 0, sizeof( *trail ) );

	// link into the active list
	trail->next = cg_activeStrafeTrails.next;
	trail->prev = &cg_activeStrafeTrails;
	cg_activeStrafeTrails.next->prev = trail;
	cg_activeStrafeTrails.next = trail;
	return trail;
}
#else
void CG_DeleteLocalEntity( int clientNum ) {
	int i;

	for ( i = 0 ; i < MAX_LOCAL_ENTITIES - 1 ; i++ ) {
		if ( (cg_localEntities[i].leType == LE_LINE) &&
			 ((cg_localEntities[i].leFlags == clientNum+1) || (cg_localEntities[i].leFlags > 0 && clientNum == -1)) ) {
				 cg_localEntities[i].endTime = cg.time + 100;
				 //CG_FreeLocalEntity(&cg_localEntities[i]); //just set leTime to 0 instead idk
		}
	}
}
#endif