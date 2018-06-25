////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 2011 - 2018 Dusan Jocic <dusanjocic@msn.com>
//
// This file is part of OpenWolf.
//
// OpenWolf is free software; you can redistribute it
// and / or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the License,
// or (at your option) any later version.
//
// OpenWolf is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenWolf; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110 - 1301  USA
//
// -------------------------------------------------------------------------------------
// File name:   chars.h
// Version:     v1.00
// Created:
// Compilers:   Visual Studio 2015
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CHARS_H__
#define __CHARS_H__

//========================================================
//========================================================
//name
#define CHARACTERISTIC_NAME							0	//string
//gender of the bot
#define CHARACTERISTIC_GENDER						1	//string ("male", "female", "it")
//attack skill
// >  0.0 && <  0.2 = don't move
// >  0.3 && <  1.0 = aim at enemy during retreat
// >  0.0 && <  0.4 = only move forward/backward
// >= 0.4 && <  1.0 = circle strafing
// >  0.7 && <  1.0 = random strafe direction change
#define CHARACTERISTIC_ATTACK_SKILL					2	//float [0, 1]
//weapon weight file
#define CHARACTERISTIC_WEAPONWEIGHTS				3	//string
//view angle difference to angle change factor
#define CHARACTERISTIC_VIEW_FACTOR					4	//float <0, 1]
//maximum view angle change
#define CHARACTERISTIC_VIEW_MAXCHANGE				5	//float [1, 360]
//reaction time in seconds
#define CHARACTERISTIC_REACTIONTIME					6	//float [0, 5]
//accuracy when aiming
#define CHARACTERISTIC_AIM_ACCURACY					7	//float [0, 1]
//weapon specific aim accuracy
#define CHARACTERISTIC_AIM_ACCURACY_BLASTER         8   //float [0, 1]
#define CHARACTERISTIC_AIM_ACCURACY_MACHINEGUN		9	//float [0, 1]
#define CHARACTERISTIC_AIM_ACCURACY_PAINSAW         10	//float [0, 1]
#define CHARACTERISTIC_AIM_ACCURACY_SHOTGUN			11	//float [0, 1]
#define CHARACTERISTIC_AIM_ACCURACY_LASGUN	        12	//float [0, 1]
#define CHARACTERISTIC_AIM_ACCURACY_MASSDRIVER		13	//float [0, 1]
#define CHARACTERISTIC_AIM_ACCURACY_PULSERIFLE      14	//float [0, 1]
#define CHARACTERISTIC_AIM_ACCURACY_FLAMER  		15	//float [0, 1]
#define CHARACTERISTIC_AIM_ACCURACY_LUCIFER		    16	//float [0, 1]
#define CHARACTERISTIC_AIM_ACCURACY_GRENADE			17	//float [0, 1]
//skill when aiming
// >  0.0 && <  0.9 = aim is affected by enemy movement
// >  0.4 && <= 0.8 = enemy linear leading
// >  0.8 && <= 1.0 = enemy exact movement leading
// >  0.5 && <= 1.0 = prediction shots when enemy is not visible
// >  0.6 && <= 1.0 = splash damage by shooting nearby geometry
#define CHARACTERISTIC_AIM_SKILL					18	//float [0, 1]
//weapon specific aim skill
#define CHARACTERISTIC_AIM_SKILL_PAINSAW            19	//float [0, 1]
#define CHARACTERISTIC_AIM_SKILL_LASGUN				20	//float [0, 1]
#define CHARACTERISTIC_AIM_SKILL_MASSDRIVER         21	//float [0, 1]
#define CHARACTERISTIC_AIM_SKILL_PULSERIFLE         22	//float [0, 1]
#define CHARACTERISTIC_AIM_SKILL_FLAMER             23	//float [0, 1]
#define CHARACTERISTIC_AIM_SKILL_LUCIFER			24	//float [0, 1]
#define CHARACTERISTIC_AIM_SKILL_GRENADE			25	//float [0, 1]
//========================================================
//chat
//========================================================
//file with chats
#define CHARACTERISTIC_CHAT_FILE					31	//string
//name of the chat character
#define CHARACTERISTIC_CHAT_NAME					32	//string
//characters per minute type speed
#define CHARACTERISTIC_CHAT_CPM						33	//integer [1, 4000]
//tendency to insult/praise
#define CHARACTERISTIC_CHAT_INSULT					34	//float [0, 1]
//tendency to chat misc
#define CHARACTERISTIC_CHAT_MISC					35	//float [0, 1]
//tendency to chat at start or end of level
#define CHARACTERISTIC_CHAT_STARTENDLEVEL			36	//float [0, 1]
//tendency to chat entering or exiting the game
#define CHARACTERISTIC_CHAT_ENTEREXITGAME			37	//float [0, 1]
//tendency to chat when killed someone
#define CHARACTERISTIC_CHAT_KILL					38	//float [0, 1]
//tendency to chat when died
#define CHARACTERISTIC_CHAT_DEATH					39	//float [0, 1]
//tendency to chat when enemy suicides
#define CHARACTERISTIC_CHAT_ENEMYSUICIDE			40	//float [0, 1]
//tendency to chat when hit while talking
#define CHARACTERISTIC_CHAT_HITTALKING				41	//float [0, 1]
//tendency to chat when bot was hit but didn't dye
#define CHARACTERISTIC_CHAT_HITNODEATH				42	//float [0, 1]
//tendency to chat when bot hit the enemy but enemy didn't dye
#define CHARACTERISTIC_CHAT_HITNOKILL				43	//float [0, 1]
//tendency to randomly chat
#define CHARACTERISTIC_CHAT_RANDOM					44	//float [0, 1]
//tendency to reply
#define CHARACTERISTIC_CHAT_REPLY					45	//float [0, 1]
//========================================================
//movement
//========================================================
//tendency to crouch
#define CHARACTERISTIC_CROUCHER						56	//float [0, 1]
//tendency to jump
#define CHARACTERISTIC_JUMPER						57	//float [0, 1]
//tendency to walk
#define CHARACTERISTIC_WALKER						58	//float [0, 1]
//tendency to jump using a weapon
#define CHARACTERISTIC_WEAPONJUMPING				59	//float [0, 1]
//tendency to use the grapple hook when available
#define CHARACTERISTIC_GRAPPLE_USER					60	//float [0, 1]	//use this!!
//========================================================
//goal
//========================================================
//item weight file
#define CHARACTERISTIC_ITEMWEIGHTS					70	//string
//the aggression of the bot
#define CHARACTERISTIC_AGGRESSION					71	//float [0, 1]
//the self preservation of the bot (rockets near walls etc.)
#define CHARACTERISTIC_SELFPRESERVATION				72	//float [0, 1]
//how likely the bot is to take revenge
#define CHARACTERISTIC_VENGEFULNESS					73	//float [0, 1]	//use this!!
//tendency to camp
#define CHARACTERISTIC_CAMPER						74	//float [0, 1]
//========================================================
//========================================================
//tendency to get easy frags
#define CHARACTERISTIC_EASY_FRAGGER					75	//float [0, 1]
//how alert the bot is (view distance)
#define CHARACTERISTIC_ALERTNESS					76	//float [0, 1]
//how much the bot fires it's weapon
#define CHARACTERISTIC_FIRETHROTTLE					77	//float [0, 1]

#endif //!__CHARS_H__