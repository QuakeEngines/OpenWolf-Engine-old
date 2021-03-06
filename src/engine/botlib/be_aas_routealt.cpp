////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 1999 - 2010 id Software LLC, a ZeniMax Media company.
// Copyright(C) 2011 - 2018 Dusan Jocic <dusanjocic@msn.com>
//
// This file is part of the OpenWolf GPL Source Code.
// OpenWolf Source Code is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// OpenWolf Source Code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenWolf Source Code.  If not, see <http://www.gnu.org/licenses/>.
//
// In addition, the OpenWolf Source Code is also subject to certain additional terms.
// You should have received a copy of these additional terms immediately following the
// terms and conditions of the GNU General Public License which accompanied the
// OpenWolf Source Code. If not, please request a copy in writing from id Software
// at the address below.
//
// If you have questions concerning this license or the applicable additional terms,
// you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
// Suite 120, Rockville, Maryland 20850 USA.
//
// -------------------------------------------------------------------------------------
// File name:   be_aas_routealt.cpp
// Version:     v1.00
// Created:
// Compilers:   Visual Studio 2015
// Description: AAS
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifdef DEDICATED
#include <null/null_precompiled.h>
#else
#include <OWLib/precompiled.h>
#endif

#define ENABLE_ALTROUTING
//#define ALTROUTE_DEBUG

typedef struct midrangearea_s
{
    S32 valid;
    U16 starttime;
    U16 goaltime;
} midrangearea_t;

midrangearea_t* midrangeareas;
S32* clusterareas;
S32 numclusterareas;

//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
void AAS_AltRoutingFloodCluster_r( S32 areanum )
{
    S32 i, otherareanum;
    aas_area_t* area;
    aas_face_t* face;
    
    //add the current area to the areas of the current cluster
    clusterareas[numclusterareas] = areanum;
    numclusterareas++;
    //remove the area from the mid range areas
    midrangeareas[areanum].valid = false;
    //flood to other areas through the faces of this area
    area = &aasworld.areas[areanum];
    for( i = 0; i < area->numfaces; i++ )
    {
        face = &aasworld.faces[abs( aasworld.faceindex[area->firstface + i] )];
        //get the area at the other side of the face
        if( face->frontarea == areanum ) otherareanum = face->backarea;
        else otherareanum = face->frontarea;
        //if there is an area at the other side of this face
        if( !otherareanum ) continue;
        //if the other area is not a midrange area
        if( !midrangeareas[otherareanum].valid ) continue;
        //
        AAS_AltRoutingFloodCluster_r( otherareanum );
    } //end for
} //end of the function AAS_AltRoutingFloodCluster_r
//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
S32 AAS_AlternativeRouteGoals( vec3_t start, S32 startareanum, vec3_t goal, S32 goalareanum, S32 travelflags,
                               aas_altroutegoal_t* altroutegoals, S32 maxaltroutegoals, S32 type )
{
#ifndef ENABLE_ALTROUTING
    return 0;
#else
    S32 i, j, bestareanum;
    S32 numaltroutegoals, nummidrangeareas;
    S32 starttime, goaltime, goaltraveltime;
    F32 dist, bestdist;
    vec3_t mid, dir;
#ifdef ALTROUTE_DEBUG
    S32 startmillisecs;
    
    startmillisecs = Sys_MilliSeconds();
#endif
    
    if( !startareanum || !goalareanum )
        return 0;
    //travel time towards the goal area
    goaltraveltime = AAS_AreaTravelTimeToGoalArea( startareanum, start, goalareanum, travelflags );
    //clear the midrange areas
    ::memset( midrangeareas, 0, aasworld.numareas * sizeof( midrangearea_t ) );
    numaltroutegoals = 0;
    //
    nummidrangeareas = 0;
    //
    for( i = 1; i < aasworld.numareas; i++ )
    {
        //
        if( !( type & ALTROUTEGOAL_ALL ) )
        {
            if( !( type & ALTROUTEGOAL_CLUSTERPORTALS && ( aasworld.areasettings[i].contents & AREACONTENTS_CLUSTERPORTAL ) ) )
            {
                if( !( type & ALTROUTEGOAL_VIEWPORTALS && ( aasworld.areasettings[i].contents & AREACONTENTS_VIEWPORTAL ) ) )
                {
                    continue;
                } //end if
            } //end if
        } //end if
        //if the area has no reachabilities
        if( !AAS_AreaReachability( i ) ) continue;
        //tavel time from the area to the start area
        starttime = AAS_AreaTravelTimeToGoalArea( startareanum, start, i, travelflags );
        if( !starttime ) continue;
        //if the travel time from the start to the area is greater than the shortest goal travel time
        if( starttime > ( F32 ) 1.1 * goaltraveltime ) continue;
        //travel time from the area to the goal area
        goaltime = AAS_AreaTravelTimeToGoalArea( i, NULL, goalareanum, travelflags );
        if( !goaltime ) continue;
        //if the travel time from the area to the goal is greater than the shortest goal travel time
        if( goaltime > ( F32 ) 0.8 * goaltraveltime ) continue;
        //this is a mid range area
        midrangeareas[i].valid = true;
        midrangeareas[i].starttime = starttime;
        midrangeareas[i].goaltime = goaltime;
        Log_Write( "%d midrange area %d", nummidrangeareas, i );
        nummidrangeareas++;
    } //end for
    //
    for( i = 1; i < aasworld.numareas; i++ )
    {
        if( !midrangeareas[i].valid ) continue;
        //get the areas in one cluster
        numclusterareas = 0;
        AAS_AltRoutingFloodCluster_r( i );
        //now we've got a cluster with areas through which an alternative route could go
        //get the 'center' of the cluster
        VectorClear( mid );
        for( j = 0; j < numclusterareas; j++ )
        {
            VectorAdd( mid, aasworld.areas[clusterareas[j]].center, mid );
        } //end for
        VectorScale( mid, 1.0 / numclusterareas, mid );
        //get the area closest to the center of the cluster
        bestdist = 999999;
        bestareanum = 0;
        for( j = 0; j < numclusterareas; j++ )
        {
            VectorSubtract( mid, aasworld.areas[clusterareas[j]].center, dir );
            dist = VectorLength( dir );
            if( dist < bestdist )
            {
                bestdist = dist;
                bestareanum = clusterareas[j];
            } //end if
        } //end for
        //now we've got an area for an alternative route
        //FIXME: add alternative goal origin
        VectorCopy( aasworld.areas[bestareanum].center, altroutegoals[numaltroutegoals].origin );
        altroutegoals[numaltroutegoals].areanum = bestareanum;
        altroutegoals[numaltroutegoals].starttraveltime = midrangeareas[bestareanum].starttime;
        altroutegoals[numaltroutegoals].goaltraveltime = midrangeareas[bestareanum].goaltime;
        altroutegoals[numaltroutegoals].extratraveltime =
            ( midrangeareas[bestareanum].starttime + midrangeareas[bestareanum].goaltime ) -
            goaltraveltime;
        numaltroutegoals++;
        //
#ifdef ALTROUTE_DEBUG
        AAS_ShowAreaPolygons( bestareanum, 1, true );
#endif
        //don't return more than the maximum alternative route goals
        if( numaltroutegoals >= maxaltroutegoals ) break;
    } //end for
#ifdef ALTROUTE_DEBUG
    botimport.Print( PRT_MESSAGE, "alternative route goals in %d msec\n", Sys_MilliSeconds() - startmillisecs );
#endif
    return numaltroutegoals;
#endif
} //end of the function AAS_AlternativeRouteGoals
//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
void AAS_InitAlternativeRouting( void )
{
#ifdef ENABLE_ALTROUTING
    if( midrangeareas ) FreeMemory( midrangeareas );
    midrangeareas = ( midrangearea_t* ) GetMemory( aasworld.numareas * sizeof( midrangearea_t ) );
    if( clusterareas ) FreeMemory( clusterareas );
    clusterareas = ( S32* ) GetMemory( aasworld.numareas * sizeof( S32 ) );
#endif
} //end of the function AAS_InitAlternativeRouting
//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
void AAS_ShutdownAlternativeRouting( void )
{
#ifdef ENABLE_ALTROUTING
    if( midrangeareas ) FreeMemory( midrangeareas );
    midrangeareas = NULL;
    if( clusterareas ) FreeMemory( clusterareas );
    clusterareas = NULL;
    numclusterareas = 0;
#endif
} //end of the function AAS_ShutdownAlternativeRouting
