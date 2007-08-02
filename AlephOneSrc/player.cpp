/*
PLAYER.C

	Copyright (C) 1991-2001 and beyond by Bungie Studios, Inc.
	and the "Aleph One" developers.
 
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	This license is contained in the file "COPYING",
	which is included with this source code; it is available online at
	http://www.gnu.org/licenses/gpl.html

Saturday, December 11, 1993 10:25:55 AM

Friday, September 30, 1994 5:48:25 PM (Jason)
	moved nearly all sounds out of the damage_definition structure and into shapes.
Wednesday, October 26, 1994 3:18:59 PM (Jason)
	invincible players are now damaged by fusion projectiles.
Wednesday, November 30, 1994 6:56:20 PM  (Jason)
	oxygen is used up faster by running and by firing.
Thursday, January 12, 1995 11:18:18 AM  (Jason')
	dead players don�t continue to use up oxygen.
Thursday, July 6, 1995 4:53:52 PM
	supports multi-player cooperative games. (Ryan)

Feb 4, 2000 (Loren Petrich):
	Added SMG wielding stuff

	Changed halt() to assert(false) for better debugging

Feb 18, 2000 (Loren Petrich):
	Added support for a chase cam.
	Note that mark_player_collections() always loads the player sprites
	in expectation of a chase cam; this could be made to conditional on
	whether a chase cam will ever be active.

Feb 21, 2000 (Loren Petrich):
	Changed NO_TELEPORTATION_DESTINATION to SHRT_MAX, an idiot-proof value,
	since there are unlikely to be that many polygons in a map.
	
	Added upward and rightward shifts of the chase-cam position

Feb 25, 2000 (Loren Petrich):
	Moved chase-cam data into preferences data; using accessor in "interface.h"
	Made it possible to swim under a liquid if one has the ball

Feb 26, 2000 (Loren Petrich):
	Fixed level-0 teleportation bug; the hack is to move the destination
	down by 1.
	
	Added chase-cam reset feature, for the purpose of doing chase-cam inertia.
	The reset is necessary to take into account teleporting or entering a level.

Mar 2, 2000 (Loren Petrich):
	Moved the chase-cam stuff into ChaseCam.c/h
	
Mar 22, 2000 (Loren Petrich):
	Added a function to revive_player() to reset the field of view properly
	when reviving

May 14, 2000 (Loren Petrich):
	Added XML-configuration support for various player features

May 22, 2000 (Loren Petrich):
	Added XML configurability for the powerup durations

May 27, 2000 (Loren Petrich):
	Added oxygen depletion and replenishment rates

Jun 11, 2000 (Loren Petrich):
	Pegging health and oxygen to maximum values when damaged;
	takes into account negative damage from healing projectiles.
	Also turned "agressor" into "aggressor".

Jun 15, 2000 (Loren Petrich):
	Added support for Chris Pruett's Pfhortran

Jun 28, 2000 (Loren Petrich):
	Generalized the invincibility-powerup vulnerability and added XML support for that

Jul 1, 2000 (Loren Petrich):
	Added Benad's changes

Jul 10, 2000 (Loren Petrich):
	Changed calculate_player_team() slightly; no more first vassert()

Aug 31, 2000 (Loren Petrich):
	Added stuff for unpacking and packing

Apr 27, 2001 (Loren Petrich):
	Made player guided missiles optional
        
Oct 21, 2001 (Woody Zenfell):
        Made player_shape_definitions available to the rest of the system -
        in particular, so that SDL network dialog widgets can use it to render
        player icons.

Feb 20, 2002 (Woody Zenfell):
    Ripped action_queue support out into new ActionQueues class (see ActionQueues.h)
    Providing pointer gRealActionQueues to help others find the set of queues they are
    accustomed to using.

May 20, 2002 (Woody Zenfell):
    get_ticks_since_local_player_in_terminal() mechanism

Jan 12, 2003 (Woody Zenfell):
	Single entry point (reset_action_queues()) to reset all ActionQueues that need to be reset

May 22, 2003 (Woody Zenfell):
	Fixing damaging polygon types; giving player netgame penalty feedback; announcing player
	net disconnects.

 May 27, 2003 (Woody Zenfell):
	I hear dead people.  (netmic, star protocol or newer only)

 June 14, 2003 (Woody Zenfell):
	update_players() now has a predictive mode of execution which takes many fewer actions
	(i.e. tries to alter only state like the player's location and facing etc.)

 May 21, 2004 (Alexei Svitkine):
	Made all the MML-settable stuff in this file have a ResetValues method that resets to
	old values (which we now save). Had to move some free-standing variables into structs
	for this.
*/

#define DONT_REPEAT_DEFINITIONS

#include "cseries.h"
#include "map.h"
#include "player.h"
#include "monster_definitions.h"
#include "monsters.h"
#include "interface.h"
#include "mysound.h"
#include "fades.h"
#include "media.h"
#include "items.h"
#include "weapons.h"
//#include "game_window.h"
#include "computer_interface.h"
#include "projectiles.h"
//#include "network_games.h"
//#include "network.h"
//#include "screen.h"
//#include "shell.h" // for screen_printf()

/*
//anybody on the receiving pad of a teleport should explode (what happens to invincible guys?)
// Really should create a function that initializes the player state.
??new players should teleport in
*/

// LP addition:
//#include "ChaseCam.h"
#include "Packing.h"
//#include "network.h"

// ZZZ additions:
//#include "ActionQueues.h"

// jkvw addition:
//#include "lua_script.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h> 

#ifdef env68k
#pragma segment player
#endif

/* ---------- constants */

struct player_powerup_durations_definition {
	short InvisibilityDuration;
	short InvincibilityDuration;
	short ExtravisionDuration;
	short InfravisionDuration;
};

// These are variables, because they can be set with an XML parser
struct player_powerup_durations_definition player_powerup_durations = {
	70*TICKS_PER_SECOND,
	50*TICKS_PER_SECOND,
	3*TICKS_PER_MINUTE,
	3*TICKS_PER_MINUTE
};

#define kINVISIBILITY_DURATION  player_powerup_durations.InvisibilityDuration
#define kINVINCIBILITY_DURATION player_powerup_durations.InvincibilityDuration
#define kEXTRAVISION_DURATION   player_powerup_durations.ExtravisionDuration
#define kINFRAVISION_DURATION   player_powerup_durations.InfravisionDuration

#define MINIMUM_REINCARNATION_DELAY (TICKS_PER_SECOND)
#define NORMAL_REINCARNATION_DELAY (10*TICKS_PER_SECOND)
#define SUICIDE_REINCARNATION_DELAY (15*TICKS_PER_SECOND)

#define DEAD_PLAYER_HEIGHT WORLD_ONE_FOURTH

#define OXYGEN_WARNING_LEVEL TICKS_PER_MINUTE
#define OXYGEN_WARNING_FREQUENCY (TICKS_PER_MINUTE/4)
#define OXYGEN_WARNING_OFFSET (10*TICKS_PER_SECOND)

#define LAST_LEVEL 100

/* ---------- structures */

// ZZZ: moved struct action_queue inside ActionQueues (see ActionQueues.cpp).
// ZZZ: moved struct player_shape_information to player.h for sharing

struct damage_response_definition
{
	short type;
	short damage_threshhold; /* NONE is none, otherwise bumps fade by one if over threshhold */
	
	short fade;
	short sound, death_sound, death_action;
};

struct player_powerup_definition
{
	short Powerup_Invincibility;
	short Powerup_Invisibility;
	short Powerup_Infravision;
	short Powerup_Extravision;
	short Powerup_TripleEnergy;
	short Powerup_DoubleEnergy;
	short Powerup_Energy;
	short Powerup_Oxygen;
};

/* ---------- globals */

struct player_data *players;
struct damage_record team_damage_given[NUMBER_OF_TEAM_COLORS];
struct damage_record team_damage_taken[NUMBER_OF_TEAM_COLORS];
struct damage_record team_monster_damage_taken[NUMBER_OF_TEAM_COLORS];
struct damage_record team_monster_damage_given[NUMBER_OF_TEAM_COLORS];
struct damage_record team_friendly_fire[NUMBER_OF_TEAM_COLORS];

struct player_data *local_player, *current_player;
short local_player_index, current_player_index;

// ZZZ: Let folks ask for a pointer to the main set of ActionQueues.
/*static ActionQueues*   sRealActionQueues = NULL;
ActionQueues* GetRealActionQueues() { return sRealActionQueues; }
*/
static struct player_shape_definitions player_shapes=
{
	6, /* collection */
	
	9, 8, /* dying hard, dying soft */
	11, 10, /* dead hard, dead soft */
	{7, 0, 0, 24, 23}, /* legs: stationary, walking, running, sliding, airborne */
	// LP additions: SMG-wielding/firing shapes (just before last two)
	{1, 3, 20, 26, 14, 12, 31, 16, 28, 33, 5, 18}, /* idle torsos: fists, magnum, fusion, assault, rocket, flamethrower, alien, shotgun, double pistol, double shotgun, da ball */
	{1, 3, 21, 26, 14, 12, 31, 16, 28, 33, 5, 18}, /* charging torsos: fists, magnum, fusion, assault, rocket, flamethrower, alien, shotgun, double pistol, double shotgun, ball */
	{2, 4, 22, 27, 15, 13, 32, 17, 28, 34, 6, 19}, /* firing torsos: fists, magnum, fusion, assault, rocket, flamethrower, alien, shotgun, double pistol, double shotgun, ball */
};

#define NUMBER_OF_PLAYER_INITIAL_ITEMS (sizeof(player_initial_items)/sizeof(short))
static short player_initial_items[]= 
{ 
	_i_magnum,  // First weapon is the weapon he will use...
	_i_knife,
	_i_knife,
	_i_magnum_magazine, 
	_i_magnum_magazine,
	_i_magnum_magazine,
	
	// LP additions, in case one wants to start very loaded
     // AS: if we want to start loaded, shouldn't it be '_i_bong'?
	_i_knife,
	_i_knife,
	_i_knife,
	_i_knife,
	_i_knife,
	
	_i_knife,
	_i_knife,
	_i_knife,
	_i_knife,
	_i_knife
};
	
#define NUMBER_OF_DAMAGE_RESPONSE_DEFINITIONS (sizeof(damage_response_definitions)/sizeof(struct damage_response_definition))

static struct damage_response_definition damage_response_definitions[]=
{
	{_damage_explosion, 100, _fade_yellow, NONE, _snd_human_scream, _monster_is_dying_hard},
	{_damage_crushing, NONE, _fade_red, NONE, _snd_human_wail, _monster_is_dying_hard},
	{_damage_projectile, NONE, _fade_red, NONE, _snd_human_scream, NONE},
	{_damage_shotgun_projectile, NONE, _fade_red, NONE, _snd_human_scream, NONE},
	{_damage_electrical_staff, NONE, _fade_cyan, NONE, _snd_human_scream, NONE},
	{_damage_hulk_slap, NONE, _fade_cyan, NONE, _snd_human_scream, NONE},
	{_damage_absorbed, 100, _fade_white, _snd_absorbed, NONE, NONE},
	{_damage_teleporter, 100, _fade_white, _snd_absorbed, NONE, NONE},
	{_damage_flame, NONE, _fade_orange, NONE, _snd_human_wail, _monster_is_dying_flaming},
	{_damage_hound_claws, NONE, _fade_red, NONE, _snd_human_scream, NONE},
	{_damage_compiler_bolt, NONE, _fade_static, NONE, _snd_human_scream, NONE},
	{_damage_alien_projectile, NONE, _fade_dodge_purple, NONE, _snd_human_wail, _monster_is_dying_flaming},
	{_damage_hunter_bolt, NONE, _fade_burn_green, NONE, _snd_human_scream, NONE},
	{_damage_fusion_bolt, 60, _fade_negative, NONE, _snd_human_scream, NONE},
	{_damage_fist, 40, _fade_red, NONE, _snd_human_scream, NONE},
	{_damage_yeti_claws, NONE, _fade_burn_cyan, NONE, _snd_human_scream, NONE},
	{_damage_yeti_projectile, NONE, _fade_dodge_yellow, NONE, _snd_human_scream, NONE},
	{_damage_defender, NONE, _fade_purple, NONE, _snd_human_scream, NONE},
	{_damage_lava, NONE, _fade_long_orange, NONE, _snd_human_wail, _monster_is_dying_flaming},
	{_damage_goo, NONE, _fade_long_green, NONE, _snd_human_wail, _monster_is_dying_flaming},
	{_damage_suffocation, NONE, NONE, NONE, _snd_suffocation, _monster_is_dying_soft},
	{_damage_energy_drain, NONE, NONE, NONE, NONE, NONE},
	{_damage_oxygen_drain, NONE, NONE, NONE, NONE, NONE},
	{_damage_hummer_bolt, NONE, _fade_flicker_negative, NONE, _snd_human_scream, NONE},
};

// LP change: made this much bigger than the number of M2/Moo polygons
#define NO_TELEPORTATION_DESTINATION INT16_MAX

// These are all configureable with MML.
struct player_settings_definition player_settings = {
	PLAYER_MAXIMUM_SUIT_ENERGY,    // InitialEnergy
	PLAYER_MAXIMUM_SUIT_OXYGEN,    // InitialOxygen
	PLAYER_MAXIMUM_SUIT_ENERGY/4,  // StrippedEnergy
	PLAYER_MAXIMUM_SUIT_ENERGY,    // SingleEnergy
	2*PLAYER_MAXIMUM_SUIT_ENERGY,  // DoubleEnergy
	3*PLAYER_MAXIMUM_SUIT_ENERGY,  // TripleEnergy
	FIXED_ONE_HALF,                // PlayerSelfLuminosity
	true,                          // CanSwim
	false,                         // PlayerShotsGuided
	QUARTER_CIRCLE/3,              // PlayerHalfVisualArc
	QUARTER_CIRCLE/3,              // PlayerHalfVertVisualArc
	31,                            // PlayerVisualRange
	31,                            // PlayerDarkVisualRange
	1,                             // OxygenDepletion
	0,                             // OxygenReplenishment
	0,                             // OxygenChange
	_damage_fusion_bolt            // Vulnerability
};

// LP: the various powerup item ID's are changeable, but set to appropriate defaults here
struct player_powerup_definition player_powerups = {
	_i_invincibility_powerup,
	_i_invisibility_powerup,
	_i_infravision_powerup,
	_i_extravision_powerup,
	_i_triple_energy_powerup,
	_i_double_energy_powerup,
	_i_energy_powerup,
	_i_oxygen_powerup
};

/* ---------- private prototypes */

static void set_player_shapes(short player_index, bool animate);
static void revive_player(short player_index);
static void recreate_player(short player_index);
/*
static void kill_player(short player_index, short aggressor_player_index, short action);
static void give_player_initial_items(short player_index);
static void get_player_transfer_mode(short player_index, short *transfer_mode, short *transfer_period);
static void set_player_dead_shape(short player_index, bool dying);
static void remove_dead_player_items(short player_index);
static void update_player_teleport(short player_index);
static void handle_player_in_vacuum(short player_index, uint32 action_flags);
static void update_player_media(short player_index);
static short calculate_player_team(short base_team);

static void try_and_strip_player_items(short player_index);
*/
// LP additions:
static void ReplenishPlayerOxygen(short player_index, uint32 action_flags);

// From AlexJLS patch; monster data necessary so that player as monster can be activated
// to make guided missiles work
static void adjust_player_physics(monster_data *me);


/* ---------- code */

player_data *get_player_data(
	const size_t player_index)
{
	player_data *data = GetMemberWithBounds(players,player_index,dynamic_world->player_count);
	vassert(data,
		csprintf(temporary, "asked for player #%lu/#%d", player_index, dynamic_world->player_count));
	
	return data;
}

void allocate_player_memory(
	void)
{
	/* allocate space for all our players */
	players= new player_data[MAXIMUM_NUMBER_OF_PLAYERS];
	assert(players);

#ifdef BETA
	dprintf("#%d players at %p (%x bytes each) ---------------------------------------;g;", MAXIMUM_NUMBER_OF_PLAYERS, players, sizeof(struct player_data));
#endif

	//sRealActionQueues = new ActionQueues(MAXIMUM_NUMBER_OF_PLAYERS, ACTION_QUEUE_BUFFER_DIAMETER, false);
}

/* returns player index */
short new_player(
	short team,
	short color,
	short identifier)
{
	short player_index, loop;
	struct player_data *player;

	// find a free slot *
	player_index= dynamic_world->player_count;
	assert(player_index<MAXIMUM_NUMBER_OF_PLAYERS);
	dynamic_world->player_count += 1;
	player= get_player_data(player_index);

	// and initialize it *
	obj_clear(*player);
	player->teleporting_destination= NO_TELEPORTATION_DESTINATION;
	player->interface_flags= 0; // Doesn't matter-> give_player_initial_items will take care of it.
	// LP change: using variables for these
	player->suit_energy= player_settings.InitialEnergy;
	player->suit_oxygen= player_settings.InitialOxygen;
	player->color= color;
	player->team= team;
	player->flags= 0;
	
	player->invincibility_duration= 0;
	player->invisibility_duration= 0;
	player->infravision_duration= 0;
	player->extravision_duration= 0;
	player->identifier= player_identifier_value(identifier);

	SET_PLAYER_DOESNT_AUTO_RECENTER_STATUS(player, player_identifier_doesnt_auto_recenter(identifier));
	SET_PLAYER_DOESNT_AUTO_SWITCH_WEAPONS_STATUS(player, player_identifier_doesnt_auto_switch_weapons(identifier));
	
	// initialize inventory *
	for (loop=0;loop<NUMBER_OF_ITEMS;++loop) player->items[loop]= NONE;

	// create the player.. *
	recreate_player(player_index);

	// Mark the player's inventory as dirty *
	//mark_player_inventory_as_dirty(player_index, NONE);
	//initialize_player_weapons(player_index);
	
	// give the player his initial items *
	//give_player_initial_items(player_index);
	//try_and_strip_player_items(player_index);
	
	return player_index;
}
/*
void walk_player_list(
	void)*/

// ZZZ: need to reset other queues now besides just the RealActionQueues
// This doesn't necessarily belong in this file, but I wasn't sure quite where to put it.
static void
reset_other_queues() {
	// Not sure if we want to do this (not my code), put here as a reminder
	//GetPfhortranActionQueues->reset();
	
	//reset_intermediate_action_queues();
}

void initialize_players(
	void)
{
	short i;
	
	/* no players */
	dynamic_world->player_count= 0;
	
	/* reset the action flag queues and zero the player slots */
	for (i=0;i<MAXIMUM_NUMBER_OF_PLAYERS;++i)
	{
		obj_clear(players[i]);
	}

//	sRealActionQueues->reset();
	reset_other_queues();

	for (i = 0; i < NUMBER_OF_TEAM_COLORS; i++) {
	  obj_clear(team_damage_given[i]);
	  obj_clear(team_damage_taken[i]);
	  obj_clear(team_monster_damage_taken[i]);
	  obj_clear(team_monster_damage_given[i]);
	  obj_clear(team_friendly_fire[i]);
	}
}

/* This will be called by entering map for two reasons:
 * 1) get rid of crap typed between levels, by accident.
 * 2) loading a game doesn't currently reset the player queues, so garbage will cause lags.
 */
/* The above comment is stale.  Now loading map calls this and so does new_game. Calling this */
/*  from entering map would bone us. *
static void
reset_player_queues()

// ZZZ addition: need to reset (potentially) multiple sets of ActionQueues, not just the RealActionQueues.
// This function doesn't necessarily belong in this file, but I wasn't sure where else to put it.
void
reset_action_queues()
{
	reset_player_queues();
	reset_other_queues();
}

// ZZZ: queue_action_flags() replaced by ActionQueues::enqueueActionFlags()
// ZZZ: dequeue_action_flags() replaced by ActionQueues::dequeueActionFlags()
// ZZZ: get_action_queue_size() replaced by ActionQueues::countActionFlags()


// ZZZ addition: keep track of the number of ticks since the local player was in terminal mode
// Note this mechanism is not very careful; should not be used for _important_ decisions.
static int  sLocalPlayerTicksSinceTerminal = 1 * TICKS_PER_MINUTE;

int
get_ticks_since_local_player_in_terminal() {
    return sLocalPlayerTicksSinceTerminal;
}

/* assumes �t==1 tick *
void update_players(ActionQueues* inActionQueuesToUse, bool inPredictive)


*/
void damage_player(
	short monster_index,
	short aggressor_index,
	short aggressor_type,
	struct damage_definition *damage,
short projectile_index){}
short player_identifier_to_player_index(
	short player_identifier)
{
	struct player_data *player;
	short player_index;
	
	for (player_index=0;player_index<dynamic_world->player_count;++player_index)
	{
		player= get_player_data(player_index);
		
		if (player->identifier==player_identifier) break;
	}
	assert(player_index!=dynamic_world->player_count);
	
	return player_index;
}

/*void mark_player_collections(
	bool loading)
{
	mark_collection(player_shapes.collection, loading);
	// LP change: unload player shapes for single-player game only if
	// a chase cam cannot exist;
//	if (!ChaseCam_CanExist())
//		if (dynamic_world->player_count==1&&loading) strip_collection(player_shapes.collection);

	mark_weapon_collections(loading);
	mark_item_collections(loading);
	mark_interface_collections(loading);
}
**/
player_shape_definitions*
get_player_shape_definitions() {
    return &player_shapes;
}

void set_local_player_index(
	short player_index)
{
	local_player_index= player_index;
	local_player= get_player_data(player_index);
}

void set_current_player_index(
	short player_index)
{
	current_player_index= player_index;
	current_player= get_player_data(player_index);
}

/* We just teleported in as it were-> recreate all the players..  *
void recreate_players_for_new_level(
	void)
{
*/
static void recreate_player(
	short player_index)
{
	short monster_index = 0;
//	struct monster_data *monster;
	struct player_data *player= get_player_data(player_index);
	short placement_team;
	struct object_location location;
	bool  player_teleported_dead= false;
	
	/* Determine the location */
	placement_team= 0;//calculate_player_team(player->team);
	get_random_player_starting_location_and_facing(player_index, placement_team, &location);

	/* create an object and a monster for this player */
	player->location.x = location.p.x;
	player->location.y = location.p.y;
	player->location.z = location.p.z;
	player->camera_location.x = location.p.x;
	player->camera_location.y = location.p.y;
	player->camera_location.z = location.p.z;

	player->camera_polygon_index = location.polygon_index;
	player->facing = location.yaw;
	player->elevation = location.pitch;
//	monster_index= new_monster(&location, _monster_marine);
//	monster= get_monster_data(monster_index);

	/* add our parasitic torso */
//	attach_parasitic_object(monster->object_index, 0, location.yaw);
	
	/* and initialize it */
	if(PLAYER_IS_TOTALLY_DEAD(player) || PLAYER_IS_DEAD(player))
	{
		player_teleported_dead= true;
	}

	/* Clear the transient flags, leave the persistant flags, like Player has cheated */
	player->flags &= (_player_is_teleporting_flag | _player_is_interlevel_teleporting_flag | PLAYER_PERSISTANT_FLAGS );
	//player->monster_index= monster_index;
	//player->object_index= monster->object_index;

	/* initialize_player_physics_variables sets all of these */
	//player->facing= player->elevation= 0;
	//player->location.x= player->location.y= player->location.z= 0;
	//player->camera_location.x= player->camera_location.y= player->camera_location.z= 0;

	/* We don't change... */
	/* physics_model, suit_energy, suit_oxygen, current_weapon, desired_weapon */
	/* None of the weapons array data... */
	/* None of the items array data.. */
	/* The inventory offset/dirty flags.. */
	// ZZZ: netdead...
	//mark_player_inventory_screen_as_dirty(player_index, _weapon);

	/* Nuke the physics */
	obj_clear(player->variables);

	/* Reset the player weapon data and the physics variable.. (after updating player_count) */
	//initialize_player_physics_variables(player_index);
	//set_player_shapes(player_index, false);

	player->control_panel_side_index = NONE; // not using a control panel.
	//initialize_player_terminal_info(player_index);

	//try_and_strip_player_items(player_index);

	/*if(player_teleported_dead)
	{
		kill_player(player_index, NONE, _monster_is_dying_soft);
	}*/
	
	// LP addition: handles the current player's chase cam;
	// in screen.c, we find that it's the current player whose view gets rendered
	//if (player_index == current_player_index) ChaseCam_Reset();

	// Done here so that players' missiles will always be guided
	// if they are intended to be guided
	//adjust_player_physics(get_monster_data(player->monster_index));
}
void team_damage_from_player_data(void)
{
  for (short player_index = 0; player_index < dynamic_world->player_count; player_index++) {
    struct player_data *player = get_player_data(player_index);
    team_damage_given[player->team].damage += player->total_damage_given.damage;
    team_damage_given[player->team].kills += player->total_damage_given.kills;
    team_monster_damage_given[player->team].damage += player->monster_damage_given.damage;
    team_monster_damage_given[player->team].kills += player->monster_damage_given.kills;
    team_monster_damage_taken[player->team].damage += player->monster_damage_taken.damage;
    team_monster_damage_taken[player->team].kills += player->monster_damage_taken.kills; 
    for (short opponent_index = 0; opponent_index < dynamic_world->player_count; opponent_index++) {
      struct player_data *opponent = get_player_data(player_index);
      team_damage_taken[player->team].damage += player->damage_taken[opponent_index].damage;
      team_damage_taken[player->team].kills += player->damage_taken[opponent_index].kills;
      if (player->team == opponent->team) {
	team_friendly_fire[player->team].damage += player->damage_taken[opponent_index].damage;
	team_friendly_fire[player->team].kills += player->damage_taken[opponent_index].kills;
      }
    }
  }
}   

short monster_index_to_player_index(
	short monster_index)
{
	struct player_data *player;
	short player_index;
	
	for (player_index=0;player_index<dynamic_world->player_count;++player_index)
	{
		player= get_player_data(player_index);
		if (player->monster_index==monster_index) break;
	}
	assert(player_index!=dynamic_world->player_count);
	
	return player_index;
}

short get_polygon_index_supporting_player(
	short monster_index)
{
	short player_index= monster_index_to_player_index(monster_index);
	struct player_data *player= get_player_data(player_index);
	
	return player->supporting_polygon_index;
}

bool legal_player_powerup(
	short player_index,
	short item_index)
{
	struct player_data *player= get_player_data(player_index);
	bool legal= true;

	if (item_index == player_powerups.Powerup_Invincibility)
	{
		if (player->invincibility_duration) legal= false;
	}
	else if (item_index == player_powerups.Powerup_Invisibility)
	{
		if (player->invisibility_duration>kINVISIBILITY_DURATION) legal= false;
	}
	else if (item_index == player_powerups.Powerup_Infravision)
	{
		if (player->infravision_duration) legal= false;
	}
	else if (item_index == player_powerups.Powerup_Extravision)
	{
		if (player->extravision_duration) legal= false;
	}
	else if (item_index == player_powerups.Powerup_TripleEnergy)
	{
		if (player->suit_energy>=player_settings.TripleEnergy) legal= false;
	}
	else if (item_index == player_powerups.Powerup_DoubleEnergy)
	{
		if (player->suit_energy>=player_settings.DoubleEnergy) legal= false;
	}
	else if (item_index == player_powerups.Powerup_Energy)
	{
		if (player->suit_energy>=player_settings.SingleEnergy) legal= false;
	}
	else if (item_index == player_powerups.Powerup_Oxygen)
	{
		if (player->suit_oxygen>=5*PLAYER_MAXIMUM_SUIT_OXYGEN/6) legal= false;
	}

	return legal;
}
/*
void process_player_powerup(
	short player_index,
	short item_index)
{
	struct player_data *player= get_player_data(player_index);
	
	if (item_index == player_powerups.Powerup_Invincibility)
	{
		player->invincibility_duration+= kINVINCIBILITY_DURATION;
	}
	else if (item_index == player_powerups.Powerup_Invisibility)
	{
		player->invisibility_duration+= kINVISIBILITY_DURATION;
	}
	else if (item_index == player_powerups.Powerup_Infravision)
	{
		player->infravision_duration+= kINFRAVISION_DURATION;
	}
	else if (item_index == player_powerups.Powerup_Extravision)
	{
		if (player_index==current_player_index) start_extravision_effect(true);
		player->extravision_duration+= kEXTRAVISION_DURATION;
	}
	else if (item_index == player_powerups.Powerup_TripleEnergy)
	{
		if (player->suit_energy<player_settings.TripleEnergy)
		{
			player->suit_energy= player_settings.TripleEnergy;
			if (player_index==current_player_index) mark_shield_display_as_dirty();
		}
	}
	else if (item_index == player_powerups.Powerup_DoubleEnergy)
	{
		if (player->suit_energy<player_settings.DoubleEnergy)
		{
			player->suit_energy= player_settings.DoubleEnergy;
			if (player_index==current_player_index) mark_shield_display_as_dirty();
		}
	}
	else if (item_index == player_powerups.Powerup_Energy)
	{
		if (player->suit_energy<player_settings.SingleEnergy)
		{
			player->suit_energy= player_settings.SingleEnergy;
			if (player_index==current_player_index) mark_shield_display_as_dirty();
		}
	}
	else if (item_index == player_powerups.Powerup_Oxygen)
	{
		player->suit_oxygen= CEILING(player->suit_oxygen+PLAYER_MAXIMUM_SUIT_OXYGEN/2, PLAYER_MAXIMUM_SUIT_OXYGEN);
		if (player_index==current_player_index) mark_oxygen_display_as_dirty();
	}
}
*/
world_distance dead_player_minimum_polygon_height(
	short polygon_index)
{
	short player_index;
	struct player_data *player;
	world_distance minimum_height= 0;
	
	for (player_index= 0, player= players; player_index<dynamic_world->player_count; ++player_index, ++player)
	{
		if (polygon_index==player->camera_polygon_index)
		{
			if (PLAYER_IS_DEAD(player)) minimum_height= DEAD_PLAYER_HEIGHT;
			break;
		}
	}
	
	return minimum_height;
}
/*
bool try_and_subtract_player_item(
	short player_index,
	short item_type)
{
	struct player_data *player= get_player_data(player_index);
	bool found_one= false;

	assert(item_type>=0 && item_type<NUMBER_OF_ITEMS);
	if (player->items[item_type]>=0)
	{
		if (!(player->items[item_type]-= 1)) player->items[item_type]= NONE;
		mark_player_inventory_as_dirty(player_index, item_type);
		found_one= true;
	}
	
	return found_one;
}

/* ---------- private prototypes */

// LP change: assumes nonpositive change rate
static void handle_player_in_vacuum(
	short player_index,
	uint32 action_flags)
{
}

// LP: assumes nonnegative change rate
static void ReplenishPlayerOxygen(short player_index, uint32 action_flags)
{
	(void)(action_flags);
	
	struct player_data *player= get_player_data(player_index);
	
	// Be careful to avoid short-integer wraparound
	assert(player_settings.OxygenChange >= 0);
	if (player->suit_oxygen < PLAYER_MAXIMUM_SUIT_OXYGEN)
	{
		if (player->suit_oxygen < PLAYER_MAXIMUM_SUIT_OXYGEN - player_settings.OxygenChange)
			player->suit_oxygen += player_settings.OxygenChange;
		else
			player->suit_oxygen = PLAYER_MAXIMUM_SUIT_OXYGEN;
	}
}
/*
static void update_player_teleport(
	short player_index)

static void update_player_media(
	short player_index)

static void set_player_shapes(
	short player_index,
	bool animate)

/* We can rebuild him!! *
static void revive_player(
	short player_index)
/* The player just changed map levels, recreate him, and all of the objects */
/*  associated with him. *
static void recreate_player(
	short player_index)

static void kill_player(
	short player_index,
	short aggressor_player_index,
	short action)

static void give_player_initial_items(
	short player_index)

static void remove_dead_player_items(
	short player_index)

static void get_player_transfer_mode(
	short player_index,
	short *transfer_mode,
	short *transfer_period)
static void set_player_dead_shape(
	short player_index,
	bool dying)
{
	struct player_data *player= get_player_data(player_index);
	struct monster_data *monster= get_monster_data(player->monster_index);
	short shape;
	
	if (monster->action==_monster_is_dying_flaming)
	{
		shape= dying ? FLAMING_DYING_SHAPE : FLAMING_DEAD_SHAPE;
	}
	else
	{
		if (dying)
		{
			shape= (monster->action==_monster_is_dying_hard) ? player_shapes.dying_hard : player_shapes.dying_soft;
		}
		else
		{
			shape= (monster->action==_monster_is_dying_hard) ? player_shapes.dead_hard : player_shapes.dead_soft;
		}
	
		shape= BUILD_DESCRIPTOR(BUILD_COLLECTION(player_shapes.collection, player->team), shape);
	}

	if (dying)
	{
		set_object_shape_and_transfer_mode(monster->object_index, shape, NONE);
	}
	else
	{
		randomize_object_sequence(monster->object_index, shape);
	}
}

static short calculate_player_team(
	short base_team)
{

// #define STRIPPED_ENERGY (PLAYER_MAXIMUM_SUIT_ENERGY/4)
static void try_and_strip_player_items(
	short player_index)
{
	struct player_data *player= get_player_data(player_index);
	short item_type;
	
	if (static_world->environment_flags&_environment_rebellion)
	{
		for (item_type= 0; item_type<NUMBER_OF_ITEMS; ++item_type)
		{
			switch (item_type)
			{
				case _i_knife:
					break;
				
				default:
					player->items[item_type]= NONE;
					break;
			}
		}
	
		mark_player_inventory_as_dirty(player_index, NONE);
		initialize_player_weapons(player_index);
		
		// LP change: using variable for this
		if (player->suit_energy > player_settings.StrippedEnergy)
			player->suit_energy = player_settings.StrippedEnergy;
	}
}


static void adjust_player_physics(monster_data *me)
{	
	// LP: Fix the player physics so that guided missiles will work correctly
	if (player_settings.PlayerShotsGuided)
	{
		struct monster_definition *vacbob = get_monster_definition_external(_civilian_fusion_security);
		// AlexJLS patch: make this player active, so guided weapons can work
		SET_MONSTER_ACTIVE_STATUS(me,true);
		
		// Gets called once for every player character created or re-created;
		// that seems to be OK
		SetPlayerViewAttribs(vacbob->half_visual_arc,vacbob->half_vertical_visual_arc / 3,
			vacbob->visual_range * 2,
			vacbob->dark_visual_range * 2);
	}
}
*/

static void StreamToPhysVars(uint8* &S, physics_variables& Object)
{
		StreamToValue(S,Object.head_direction);
		StreamToValue(S,Object.last_direction);
		StreamToValue(S,Object.direction);
		StreamToValue(S,Object.elevation);
		StreamToValue(S,Object.angular_velocity);
		StreamToValue(S,Object.vertical_angular_velocity);	
		StreamToValue(S,Object.velocity);
		StreamToValue(S,Object.perpendicular_velocity);
		StreamToValue(S,Object.last_position.x);
		StreamToValue(S,Object.last_position.y);
		StreamToValue(S,Object.last_position.z);
		StreamToValue(S,Object.position.x);
		StreamToValue(S,Object.position.y);
		StreamToValue(S,Object.position.z);
		StreamToValue(S,Object.actual_height);
		
		StreamToValue(S,Object.adjusted_pitch);
		StreamToValue(S,Object.adjusted_yaw);

		StreamToValue(S,Object.external_velocity.i);
		StreamToValue(S,Object.external_velocity.j);
		StreamToValue(S,Object.external_velocity.k);
		StreamToValue(S,Object.external_angular_velocity);
		
		StreamToValue(S,Object.step_phase);
		StreamToValue(S,Object.step_amplitude);	
		
		StreamToValue(S,Object.floor_height);
		StreamToValue(S,Object.ceiling_height);	
		StreamToValue(S,Object.media_height);
		
		StreamToValue(S,Object.action);
		StreamToValue(S,Object.old_flags);	
		StreamToValue(S,Object.flags);
}

static void PhysVarsToStream(uint8* &S, physics_variables& Object)
{
		ValueToStream(S,Object.head_direction);
		ValueToStream(S,Object.last_direction);
		ValueToStream(S,Object.direction);
		ValueToStream(S,Object.elevation);
		ValueToStream(S,Object.angular_velocity);
		ValueToStream(S,Object.vertical_angular_velocity);	
		ValueToStream(S,Object.velocity);
		ValueToStream(S,Object.perpendicular_velocity);
		ValueToStream(S,Object.last_position.x);
		ValueToStream(S,Object.last_position.y);
		ValueToStream(S,Object.last_position.z);
		ValueToStream(S,Object.position.x);
		ValueToStream(S,Object.position.y);
		ValueToStream(S,Object.position.z);
		ValueToStream(S,Object.actual_height);
		
		ValueToStream(S,Object.adjusted_pitch);
		ValueToStream(S,Object.adjusted_yaw);

		ValueToStream(S,Object.external_velocity.i);
		ValueToStream(S,Object.external_velocity.j);
		ValueToStream(S,Object.external_velocity.k);
		ValueToStream(S,Object.external_angular_velocity);
		
		ValueToStream(S,Object.step_phase);
		ValueToStream(S,Object.step_amplitude);	
		
		ValueToStream(S,Object.floor_height);
		ValueToStream(S,Object.ceiling_height);	
		ValueToStream(S,Object.media_height);
		
		ValueToStream(S,Object.action);
		ValueToStream(S,Object.old_flags);	
		ValueToStream(S,Object.flags);
}


inline void StreamToDamRec(uint8* &S, damage_record& Object)
{
		StreamToValue(S,Object.damage);
		StreamToValue(S,Object.kills);
}

static void DamRecToStream(uint8* &S, damage_record& Object)
{
		ValueToStream(S,Object.damage);
		ValueToStream(S,Object.kills);
}


uint8 *unpack_player_data(uint8 *Stream, player_data *Objects, size_t Count)
{
	uint8* S = Stream;
	player_data* ObjPtr = Objects;
	
	for (size_t k = 0; k < Count; k++, ObjPtr++)
	{
		StreamToValue(S,ObjPtr->identifier);
		StreamToValue(S,ObjPtr->flags);
		
		StreamToValue(S,ObjPtr->color);
		StreamToValue(S,ObjPtr->team);
		StreamToBytes(S,ObjPtr->name,MAXIMUM_PLAYER_NAME_LENGTH+2);
		
		StreamToValue(S,ObjPtr->location.x);
		StreamToValue(S,ObjPtr->location.y);
		StreamToValue(S,ObjPtr->location.z);
		StreamToValue(S,ObjPtr->camera_location.x);
		StreamToValue(S,ObjPtr->camera_location.y);
		StreamToValue(S,ObjPtr->camera_location.z);	
		StreamToValue(S,ObjPtr->camera_polygon_index);
		StreamToValue(S,ObjPtr->facing);
		StreamToValue(S,ObjPtr->elevation);
		StreamToValue(S,ObjPtr->supporting_polygon_index);
		StreamToValue(S,ObjPtr->last_supporting_polygon_index);
		
		StreamToValue(S,ObjPtr->suit_energy);
		StreamToValue(S,ObjPtr->suit_oxygen);
		
		StreamToValue(S,ObjPtr->monster_index);
		StreamToValue(S,ObjPtr->object_index);
		
		StreamToValue(S,ObjPtr->weapon_intensity_decay);
		StreamToValue(S,ObjPtr->weapon_intensity);
		
		StreamToValue(S,ObjPtr->invisibility_duration);
		StreamToValue(S,ObjPtr->invincibility_duration);
		StreamToValue(S,ObjPtr->infravision_duration);	
		StreamToValue(S,ObjPtr->extravision_duration);
		
		StreamToValue(S,ObjPtr->delay_before_teleport);
		StreamToValue(S,ObjPtr->teleporting_phase);
		StreamToValue(S,ObjPtr->teleporting_destination);
		StreamToValue(S,ObjPtr->interlevel_teleport_phase);
		
		StreamToList(S,ObjPtr->items,NUMBER_OF_ITEMS);
		
		StreamToValue(S,ObjPtr->interface_flags);
		StreamToValue(S,ObjPtr->interface_decay);
		
		StreamToPhysVars(S,ObjPtr->variables);
		
		StreamToDamRec(S,ObjPtr->total_damage_given);
		for (int k=0; k<MAXIMUM_NUMBER_OF_PLAYERS; k++)
			StreamToDamRec(S,ObjPtr->damage_taken[k]);
		StreamToDamRec(S,ObjPtr->monster_damage_taken);
		StreamToDamRec(S,ObjPtr->monster_damage_given);
		
		StreamToValue(S,ObjPtr->reincarnation_delay);
		
		StreamToValue(S,ObjPtr->control_panel_side_index);
		
		StreamToValue(S,ObjPtr->ticks_at_last_successful_save);
		
		StreamToList(S,ObjPtr->netgame_parameters,2);
		
		S += 256*2;
	}
	
	assert((S - Stream) == static_cast<ptrdiff_t>(Count*SIZEOF_player_data));
	return S;
}
uint8 *pack_player_data(uint8 *Stream, player_data *Objects, size_t Count)
{
	uint8* S = Stream;
	player_data* ObjPtr = Objects;
	
	for (size_t k = 0; k < Count; k++, ObjPtr++)
	{
		ValueToStream(S,ObjPtr->identifier);
		ValueToStream(S,ObjPtr->flags);
		
		ValueToStream(S,ObjPtr->color);
		ValueToStream(S,ObjPtr->team);
		BytesToStream(S,ObjPtr->name,MAXIMUM_PLAYER_NAME_LENGTH+2);
		
		ValueToStream(S,ObjPtr->location.x);
		ValueToStream(S,ObjPtr->location.y);
		ValueToStream(S,ObjPtr->location.z);
		ValueToStream(S,ObjPtr->camera_location.x);
		ValueToStream(S,ObjPtr->camera_location.y);
		ValueToStream(S,ObjPtr->camera_location.z);	
		ValueToStream(S,ObjPtr->camera_polygon_index);
		ValueToStream(S,ObjPtr->facing);
		ValueToStream(S,ObjPtr->elevation);
		ValueToStream(S,ObjPtr->supporting_polygon_index);
		ValueToStream(S,ObjPtr->last_supporting_polygon_index);
		
		ValueToStream(S,ObjPtr->suit_energy);
		ValueToStream(S,ObjPtr->suit_oxygen);
		
		ValueToStream(S,ObjPtr->monster_index);
		ValueToStream(S,ObjPtr->object_index);
		
		ValueToStream(S,ObjPtr->weapon_intensity_decay);
		ValueToStream(S,ObjPtr->weapon_intensity);
		
		ValueToStream(S,ObjPtr->invisibility_duration);
		ValueToStream(S,ObjPtr->invincibility_duration);
		ValueToStream(S,ObjPtr->infravision_duration);	
		ValueToStream(S,ObjPtr->extravision_duration);
		
		ValueToStream(S,ObjPtr->delay_before_teleport);
		ValueToStream(S,ObjPtr->teleporting_phase);
		ValueToStream(S,ObjPtr->teleporting_destination);
		ValueToStream(S,ObjPtr->interlevel_teleport_phase);
		
		ListToStream(S,ObjPtr->items,NUMBER_OF_ITEMS);
		
		ValueToStream(S,ObjPtr->interface_flags);
		ValueToStream(S,ObjPtr->interface_decay);
		
		PhysVarsToStream(S,ObjPtr->variables);
		
		DamRecToStream(S,ObjPtr->total_damage_given);
		for (int k=0; k<MAXIMUM_NUMBER_OF_PLAYERS; k++)
			DamRecToStream(S,ObjPtr->damage_taken[k]);
		DamRecToStream(S,ObjPtr->monster_damage_taken);
		DamRecToStream(S,ObjPtr->monster_damage_given);
		
		ValueToStream(S,ObjPtr->reincarnation_delay);
		
		ValueToStream(S,ObjPtr->control_panel_side_index);
		
		ValueToStream(S,ObjPtr->ticks_at_last_successful_save);
		
		ListToStream(S,ObjPtr->netgame_parameters,2);
		
		S += 256*2;
	}
	
	assert((S - Stream) == static_cast<ptrdiff_t>(Count*SIZEOF_player_data));
	return S;
}

short *original_player_initial_items = NULL;
/*
class XML_StartItemParser: public XML_ElementParser
{
	short Index;
	short Type;
	
	// What is present?
	bool IndexPresent;
	enum {NumberOfValues = 1};
	bool IsPresent[NumberOfValues];
	
public:
	bool Start();
	bool HandleAttribute(const char *Tag, const char *Value);
	bool AttributesDone();
	bool ResetValues();

	XML_StartItemParser(): XML_ElementParser("item") {}
};

bool XML_StartItemParser::Start()
{
	// back up old values first
	if (!original_player_initial_items) {
		original_player_initial_items = (short *) malloc(sizeof(short) * NUMBER_OF_PLAYER_INITIAL_ITEMS);
		assert(original_player_initial_items);
		for (unsigned i = 0; i < NUMBER_OF_PLAYER_INITIAL_ITEMS; i++)
			original_player_initial_items[i] = player_initial_items[i];
	}

	IndexPresent = false;
	for (int k=0; k<NumberOfValues; k++)
		IsPresent[k] = false;
	
	return true;
}

bool XML_StartItemParser::HandleAttribute(const char *Tag, const char *Value)
{
	if (StringsEqual(Tag,"index"))
	{
		if (ReadBoundedInt16Value(Value,Index,0,NUMBER_OF_PLAYER_INITIAL_ITEMS-1))
		{
			IndexPresent = true;
			return true;
		}
		else return false;
	}
	else if (StringsEqual(Tag,"type"))
	{
		if (ReadBoundedInt16Value(Value,Type,0,NUMBER_OF_DEFINED_ITEMS-1))
		{
			IsPresent[0] = true;
			return true;
		}
		else return false;
	}
	UnrecognizedTag();
	return false;
}

bool XML_StartItemParser::AttributesDone()
{
	// Verify...
	if (!IndexPresent || !IsPresent[0])
	{
		AttribsMissing();
		return false;
	}
	player_initial_items[Index] = Type;
			
	return true;
}

bool XML_StartItemParser::ResetValues()
{
	if (original_player_initial_items) {
		for (unsigned i = 0; i < NUMBER_OF_PLAYER_INITIAL_ITEMS; i++)
			player_initial_items[i] = original_player_initial_items[i];
		free(original_player_initial_items);
		original_player_initial_items = NULL;
	}
	return true;
}

static XML_StartItemParser StartItemParser;


struct damage_response_definition *original_damage_response_definitions = NULL;

class XML_PlayerDamageParser: public XML_ElementParser
{
	short Index;
	damage_response_definition Data;
	
	// What is present?
	bool IndexPresent;
	enum {NumberOfValues = 5};
	bool IsPresent[NumberOfValues];
	
public:
	bool Start();
	bool HandleAttribute(const char *Tag, const char *Value);
	bool AttributesDone();
	bool ResetValues();

	XML_PlayerDamageParser(): XML_ElementParser("damage") {}
};

bool XML_PlayerDamageParser::Start()
{
	// back up old values first
	if (!original_damage_response_definitions) {
		original_damage_response_definitions = (struct damage_response_definition *) malloc(sizeof(struct damage_response_definition) * NUMBER_OF_DAMAGE_RESPONSE_DEFINITIONS);
		assert(original_damage_response_definitions);
		for (unsigned i = 0; i < NUMBER_OF_DAMAGE_RESPONSE_DEFINITIONS; i++)
			original_damage_response_definitions[i] = damage_response_definitions[i];
	}

	IndexPresent = false;
	for (int k=0; k<NumberOfValues; k++)
		IsPresent[k] = false;
	
	return true;
}

bool XML_PlayerDamageParser::HandleAttribute(const char *Tag, const char *Value)
{
	if (StringsEqual(Tag,"index"))
	{
		if (ReadBoundedInt16Value(Value,Index,0,NUMBER_OF_DAMAGE_RESPONSE_DEFINITIONS-1))
		{
			IndexPresent = true;
			return true;
		}
		else return false;
	}
	else if (StringsEqual(Tag,"threshold"))
	{
		if (ReadInt16Value(Value,Data.damage_threshhold))
		{
			IsPresent[0] = true;
			return true;
		}
		else return false;
	}
	else if (StringsEqual(Tag,"fade"))
	{
		if (ReadInt16Value(Value,Data.fade))
		{
			IsPresent[1] = true;
			return true;
		}
		else return false;
	}
	else if (StringsEqual(Tag,"sound"))
	{
		if (ReadInt16Value(Value,Data.sound))
		{
			IsPresent[2] = true;
			return true;
		}
		else return false;
	}
	else if (StringsEqual(Tag,"death_sound"))
	{
		if (ReadInt16Value(Value,Data.death_sound))
		{
			IsPresent[3] = true;
			return true;
		}
		else return false;
	}
	else if (StringsEqual(Tag,"death_action"))
	{
		if (ReadInt16Value(Value,Data.death_action))
		{
			IsPresent[4] = true;
			return true;
		}
		else return false;
	}
	UnrecognizedTag();
	return false;
}

bool XML_PlayerDamageParser::AttributesDone()
{
	// Verify...
	if (!IndexPresent)
	{
		AttribsMissing();
		return false;
	}
	damage_response_definition& OrigData = damage_response_definitions[Index];
	
	if (IsPresent[0]) OrigData.damage_threshhold = Data.damage_threshhold;
	if (IsPresent[1]) OrigData.fade = Data.fade;
	if (IsPresent[2]) OrigData.sound = Data.sound;
	if (IsPresent[3]) OrigData.death_sound = Data.death_sound;
	if (IsPresent[4]) OrigData.death_action = Data.death_action;
		
	return true;
}

bool XML_PlayerDamageParser::ResetValues()
{
	if (original_damage_response_definitions) {
		for (unsigned i = 0; i < NUMBER_OF_DAMAGE_RESPONSE_DEFINITIONS; i++)
			damage_response_definitions[i] = original_damage_response_definitions[i];
		free(original_damage_response_definitions);
		original_damage_response_definitions = NULL;
	}
	return true;
}

static XML_PlayerDamageParser PlayerDamageParser;


struct player_powerup_durations_definition *original_player_powerup_durations = NULL;

class XML_PowerupParser: public XML_ElementParser
{
	
public:
	bool Start();
	bool HandleAttribute(const char *Tag, const char *Value);
	bool ResetValues();

	XML_PowerupParser(): XML_ElementParser("powerup") {}
};

bool XML_PowerupParser::Start()
{
	// back up old values first
	if (!original_player_powerup_durations) {
		original_player_powerup_durations = (struct player_powerup_durations_definition *) malloc(sizeof(struct player_powerup_durations_definition));
		assert(original_player_powerup_durations);
		*original_player_powerup_durations = player_powerup_durations;
	}

	return true;
}

bool XML_PowerupParser::HandleAttribute(const char *Tag, const char *Value)
{
	if (StringsEqual(Tag,"invisibility"))
	{
		return ReadBoundedInt16Value(Value,kINVISIBILITY_DURATION,0,SHRT_MAX);
	}
	else if (StringsEqual(Tag,"invincibility"))
	{
		return ReadBoundedInt16Value(Value,kINVINCIBILITY_DURATION,0,SHRT_MAX);
	}
	else if (StringsEqual(Tag,"extravision"))
	{
		return ReadBoundedInt16Value(Value,kEXTRAVISION_DURATION,0,SHRT_MAX);
	}
	else if (StringsEqual(Tag,"infravision"))
	{
		return ReadBoundedInt16Value(Value,kINFRAVISION_DURATION,0,SHRT_MAX);
	}
	UnrecognizedTag();
	return false;
}

bool XML_PowerupParser::ResetValues()
{
	if (original_player_powerup_durations) {
		player_powerup_durations = *original_player_powerup_durations;
		free(original_player_powerup_durations);
		original_player_powerup_durations = NULL;
	}
	return true;
}

static XML_PowerupParser PowerupParser;


struct player_powerup_definition *original_player_powerups = NULL;

class XML_PowerupAssignParser: public XML_ElementParser
{
	
public:
	bool Start();
	bool HandleAttribute(const char *Tag, const char *Value);
	bool ResetValues();

	XML_PowerupAssignParser(): XML_ElementParser("powerup_assign") {}
};

bool XML_PowerupAssignParser::Start()
{
	if (!original_player_powerups) {
		original_player_powerups = (struct player_powerup_definition *) malloc(sizeof(struct player_powerup_definition));
		assert(original_player_powerups);
		*original_player_powerups = player_powerups;
	}
	return true;
}

bool XML_PowerupAssignParser::HandleAttribute(const char *Tag, const char *Value)
{
	if (StringsEqual(Tag,"invincibility"))
	{
		return ReadBoundedInt16Value(Value,player_powerups.Powerup_Invincibility,NONE,NUMBER_OF_DEFINED_ITEMS-1);
	}
	else if (StringsEqual(Tag,"invisibility"))
	{
		return ReadBoundedInt16Value(Value,player_powerups.Powerup_Invisibility,NONE,NUMBER_OF_DEFINED_ITEMS-1);
	}
	else if (StringsEqual(Tag,"infravision"))
	{
		return ReadBoundedInt16Value(Value,player_powerups.Powerup_Infravision,NONE,NUMBER_OF_DEFINED_ITEMS-1);
	}
	else if (StringsEqual(Tag,"extravision"))
	{
		return ReadBoundedInt16Value(Value,player_powerups.Powerup_Extravision,NONE,NUMBER_OF_DEFINED_ITEMS-1);
	}
	else if (StringsEqual(Tag,"triple_energy"))
	{
		return ReadBoundedInt16Value(Value,player_powerups.Powerup_TripleEnergy,NONE,NUMBER_OF_DEFINED_ITEMS-1);
	}
	else if (StringsEqual(Tag,"double_energy"))
	{
		return ReadBoundedInt16Value(Value,player_powerups.Powerup_DoubleEnergy,NONE,NUMBER_OF_DEFINED_ITEMS-1);
	}
	else if (StringsEqual(Tag,"energy"))
	{
		return ReadBoundedInt16Value(Value,player_powerups.Powerup_Energy,NONE,NUMBER_OF_DEFINED_ITEMS-1);
	}
	else if (StringsEqual(Tag,"oxygen"))
	{
		return ReadBoundedInt16Value(Value,player_powerups.Powerup_Oxygen,NONE,NUMBER_OF_DEFINED_ITEMS-1);
	}
	UnrecognizedTag();
	return false;
}

bool XML_PowerupAssignParser::ResetValues()
{
	if (original_player_powerups) {
		player_powerups = *original_player_powerups;
		free(original_player_powerups);
		original_player_powerups = NULL;
	}
	return true;
}

static XML_PowerupAssignParser PowerupAssignParser;


struct player_shape_definitions *original_player_shapes = NULL;

class XML_PlayerShapeParser: public XML_ElementParser
{
	bool IsPresent[3];
	short Type, Subtype, Seq;

public:
	bool Start();
	bool HandleAttribute(const char *Tag, const char *Value);
	bool AttributesDone();
	bool ResetValues();

	XML_PlayerShapeParser(): XML_ElementParser("shape") {}
};

bool XML_PlayerShapeParser::Start()
{
	if (!original_player_shapes) {
		original_player_shapes = (struct player_shape_definitions *) malloc(sizeof(struct player_shape_definitions));
		*original_player_shapes = player_shapes;
	}

	for (int k=0; k<3; k++)
		IsPresent[k] = false;
	return true;
}

bool XML_PlayerShapeParser::HandleAttribute(const char *Tag, const char *Value)
{
	if (StringsEqual(Tag,"type"))
	{
		if (ReadBoundedInt16Value(Value,Type,0,4))
		{
			IsPresent[0] = true;
			return true;
		}
		else return false;
	}
	else if (StringsEqual(Tag,"subtype"))
	{
		if (ReadBoundedInt16Value(Value,Subtype,0,MAX(MAX(5,NUMBER_OF_PLAYER_ACTIONS),PLAYER_TORSO_SHAPE_COUNT)-1))
		{
			IsPresent[1] = true;
			return true;
		}
		else return false;
	}
	else if (StringsEqual(Tag,"value"))
	{
		if (ReadBoundedInt16Value(Value,Seq,0,MAXIMUM_SHAPES_PER_COLLECTION-1))
		{
			IsPresent[2] = true;
			return true;
		}
		else return false;
	}
	UnrecognizedTag();
	return false;
}

bool XML_PlayerShapeParser::AttributesDone()
{
	// Verify...
	bool AllPresent = true;
	for (int k=0; k<3; k++)
		AllPresent = AllPresent && IsPresent[k];
	
	if (!AllPresent)
	{
		AttribsMissing();
		return false;
	}
	
	// Put into place
	switch(Type)
	{
	case 0:	// Collection, dying, dead
		switch(Subtype)
		{
		case 0:
			if (Seq < MAXIMUM_COLLECTIONS)
			{
				player_shapes.collection = Seq;
			} else {
				OutOfRange();
				return false;
			}
			break;
			
		case 1:
			player_shapes.dying_hard = Seq;
			break;
			
		case 2:
			player_shapes.dying_soft = Seq;
			break;
			
		case 3:
			player_shapes.dead_hard = Seq;
			break;
			
		case 4:
			player_shapes.dead_soft = Seq;
			break;
			
		default:
			OutOfRange();
			return false;
		}
		break;
		
	case 1: // Legs
		if (Subtype < NUMBER_OF_PLAYER_ACTIONS)
		{
			player_shapes.legs[Subtype] = Seq;
		} else {
			OutOfRange();
			return false;
		}
		break;
		
	case 2: // Weapon-idle torsos
		if (Subtype < PLAYER_TORSO_SHAPE_COUNT)
		{
			player_shapes.torsos[Subtype] = Seq;
		} else {
			OutOfRange();
			return false;
		}
		break;
		
	case 3: // Weapon-charging torsos
		if (Subtype < PLAYER_TORSO_SHAPE_COUNT)
		{
			player_shapes.charging_torsos[Subtype] = Seq;
		} else {
			OutOfRange();
			return false;
		}
		break;
		
	case 4: // Weapon-firing torsos
		if (Subtype < PLAYER_TORSO_SHAPE_COUNT)
		{
			player_shapes.firing_torsos[Subtype] = Seq;
		} else {
			OutOfRange();
			return false;
		}
		break;
		
	}
		
	return true;
}

bool XML_PlayerShapeParser::ResetValues()
{
	if (original_player_shapes) {
		player_shapes = *original_player_shapes;
		free(original_player_shapes);
		original_player_shapes = NULL;
	}
	return true;
}

static XML_PlayerShapeParser PlayerShapeParser;


struct player_settings_definition *original_player_settings = NULL;

class XML_PlayerParser: public XML_ElementParser
{
	
public:
	bool Start();
	bool HandleAttribute(const char *Tag, const char *Value);
	bool ResetValues();

	XML_PlayerParser(): XML_ElementParser("player") {}
};

bool XML_PlayerParser::Start()
{
	if (!original_player_settings) {
		original_player_settings = (struct player_settings_definition *) malloc(sizeof(struct player_settings_definition));
		*original_player_settings = player_settings;
	}
	return true;
}

bool XML_PlayerParser::HandleAttribute(const char *Tag, const char *Value)
{
	if (StringsEqual(Tag,"energy"))
	{
		return ReadBoundedInt16Value(Value,player_settings.InitialEnergy,0,SHRT_MAX);
	}
	else if (StringsEqual(Tag,"oxygen"))
	{
		return ReadBoundedInt16Value(Value,player_settings.InitialOxygen,0,SHRT_MAX);
	}
	else if (StringsEqual(Tag,"stripped"))
	{
		return ReadBoundedInt16Value(Value,player_settings.StrippedEnergy,0,SHRT_MAX);
	}
	else if (StringsEqual(Tag,"light"))
	{
		float Luminosity;
		if (ReadBoundedNumericalValue(Value,"%f",Luminosity,float(0),float(SHRT_MAX)))
		{
			player_settings.PlayerSelfLuminosity = long(FIXED_ONE*Luminosity + 0.5);
			return true;
		}
		else
			return false;
	}
	else if (StringsEqual(Tag,"oxygen_deplete"))
	{
		return ReadInt16Value(Value,player_settings.OxygenDepletion);
	}
	else if (StringsEqual(Tag,"oxygen_replenish"))
	{
		return ReadInt16Value(Value,player_settings.OxygenReplenishment);
	}
	else if (StringsEqual(Tag,"vulnerability"))
	{
		return ReadBoundedInt16Value(Value,player_settings.Vulnerability,NONE,NUMBER_OF_DAMAGE_TYPES-1);
	}
	else if (StringsEqual(Tag,"guided"))
	{
		return ReadBooleanValueAsBool(Value,player_settings.PlayerShotsGuided);
	}
	else if (StringsEqual(Tag,"half_visual_arc"))
	{
		return ReadInt16Value(Value,player_settings.PlayerHalfVisualArc);
	}
	else if (StringsEqual(Tag,"half_vertical_visual_arc"))
	{
		return ReadInt16Value(Value,player_settings.PlayerHalfVertVisualArc);
	}
	else if (StringsEqual(Tag,"visual_range"))
	{
		return ReadFloatValue(Value,player_settings.PlayerVisualRange);
	}
	else if (StringsEqual(Tag,"dark_visual_range"))
	{
		return ReadFloatValue(Value,player_settings.PlayerDarkVisualRange);
	}
	else if (StringsEqual(Tag,"single_energy"))
	{
		return ReadInt16Value(Value,player_settings.SingleEnergy);
	}
	else if (StringsEqual(Tag,"double_energy"))
	{
		return ReadInt16Value(Value,player_settings.DoubleEnergy);
	}
	else if (StringsEqual(Tag,"triple_energy"))
	{
		return ReadInt16Value(Value,player_settings.TripleEnergy);
	}
	else if (StringsEqual(Tag,"can_swim"))
	{
		return ReadBooleanValue(Value,player_settings.CanSwim);
	}
	UnrecognizedTag();
	return false;
}

bool XML_PlayerParser::ResetValues()
{
	if (original_player_settings) {
		player_settings = *original_player_settings;
		free(original_player_settings);
		original_player_settings = NULL;
	}
	return true;
}

static XML_PlayerParser PlayerParser;


// XML-parser support
XML_ElementParser *Player_GetParser()
{
	PlayerParser.AddChild(&StartItemParser);
	PlayerParser.AddChild(&PlayerDamageParser);
	PlayerParser.AddChild(&PowerupParser);
	PlayerParser.AddChild(&PowerupAssignParser);
	PlayerParser.AddChild(&PlayerShapeParser);

	return &PlayerParser;
}

*/