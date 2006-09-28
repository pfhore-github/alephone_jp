/*
GAME_WAD.C

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

Sunday, July 3, 1994 10:45:17 PM

Routines for loading an entire game.

Sunday, September 25, 1994 5:03:54 PM  (alain)
	call recalculate_redundant_endpoint_data() upon restoring saved game since
	the redundant data isn't saved.
Sunday, November 6, 1994 5:35:34 PM
	added support for the unified platforms/doors, cleaned up some old code of mine...
Saturday, August 26, 1995 2:28:56 PM
	made portable.

Jan 30, 2000 (Loren Petrich):
	Added some typecasts
	Removed some "static" declarations that conflict with "extern"

Feb 4, 2000 (Loren Petrich):
	Changed halt() to assert(false) for better debugging

Feb 6, 2000 (Loren Petrich):
	Added loading and saving of physics models in savegames and from map files

Feb 12, 2000 (Loren Petrich):
	Added MARATHON_INFINITY_DATA_VERSION where appropriate

Feb 14, 2000 (Loren Petrich):
	Added more Pfhorte-friendly error checking to reading in of
	map-info ('Minf') chunk; allowing it to be 2 bytes shorter.

Feb 17, 2000 (Loren Petrich):
	Hides cursor after warning user about loading non-Bungie map files
	(strERRORS, warningExternalMapsFile)

Feb 19, 2000 (Loren Petrich):
	Fixed off-by-one asserts in load_***() routines;

Feb 26, 2000 (Loren Petrich):
	Added chase-cam initialization

June 15, 2000 (Loren Petrich):
	Added supprt for Chris Pruett's Pfhortran

Aug 12, 2000 (Loren Petrich):
	Using object-oriented file handler

Aug 25, 2000 (Loren Petrich):
	Cleared errors (game_errors.c/h) produced by Pfhortran
	and by checking on a scenario's image files

Aug 28, 2000 (Loren Petrich):
	Started on using new pack/unpack routines

Nov 26, 2000 (Loren Petrich):
	Movied a RunLevelScript() before some other stuff, such as entering_map(),
	so that textures to be loaded can be specified before they actually get loaded.

Feb 15, 2002 (Br'fin (Jeremy Parsons)):
	Additional save data is now applied to the Temporary file instead of the original
	(Old level preview info is now saved under Macintosh again)
*/

// This needs to do the right thing on save game, which is storing the precalculated crap.

//#include "cseries.h"

#include <string.h>
#include <stdlib.h>

#include "map.h"
#include "monsters.h"
//#include "network.h"
#include "projectiles.h"
#include "effects.h"
#include "player.h"
#include "platforms.h"
#include "flood_map.h"
#include "scenery.h"
#include "lightsource.h"
#include "media.h"
#include "weapons.h"
/*#include "shell.h"
#include "preferences.h"
*/
#include "FileHandler.h"

#include "tags.h"
#include "wad.h"
#include "game_wad.h"
#include "interface.h"
#include "game_errors.h"
#include "mysound.h"
#include "Packing.h"
#include "editor.h"
#include "computer_interface.h" // for loading/saving terminal state.
#include "render.h"

/*
#include "game_window.h"
#include "images.h"
#include "shell.h"
#include "preferences.h"

// LP change: added chase-cam init and render allocation
#include "ChaseCam.h"

#include "XML_LevelScript.h"

// For packing and unpacking some of the stuff

#include "motion_sensor.h"	// ZZZ for reset_motion_sensor()

#ifdef env68k
#pragma segment file_io
#endif

// unify the save game code into one structure.
*/
#include <windows.h>

/* -------- local globals */
FileSpecifier MapFileSpec;
static bool file_is_set= false;

// LP addition: was a physics model loaded from the previous level loaded?
static bool PhysicsModelLoadedEarlier = false;

// The following local globals are for handling games that need to be restored.
struct revert_game_info
{
	bool game_is_from_disk;
	struct game_data game_information;
	struct player_start_data player_start;
	struct entry_point entry_point;
	FileSpecifier SavedGame;
};
static struct revert_game_info revert_game_data;

/* -------- static functions */
static void scan_and_add_scenery(void);
static void complete_restoring_level(struct wad_data *wad);
//static void load_redundant_map_data(short *redundant_data, size_t count);
static void allocate_map_structure_for_map(struct wad_data *wad);
static struct wad_data *build_save_game_wad(struct wad_header *header, long *length);
static uint8 *tag_to_global_array_and_size(uint32 tag, 
	size_t *size
	);

static void allocate_map_for_counts(size_t polygon_count, size_t side_count,
	size_t endpoint_count, size_t line_count);
static void load_points(uint8 *points, size_t count);
static void load_lines(uint8 *lines, size_t count);
static void load_sides(uint8 *sides, size_t count, short version);
static void load_polygons(uint8 *polys, size_t count, short version);
static void load_lights(uint8 *_lights, size_t count, short version);
static void load_annotations(uint8 *annotations, size_t count);
static void load_objects(uint8 *map_objects, size_t count);
static void load_media(uint8 *_medias, size_t count);
static void load_map_info(uint8 *map_info);
static void load_ambient_sound_images(uint8 *data, size_t count);
static void load_random_sound_images(uint8 *data, size_t count);
static void load_terminal_data(uint8 *data, size_t length);

/* Used _ONLY_ by game_wad.c internally and precalculate.c. */
// ZZZ: hmm, no longer true, now using when resuming a network saved-game... hope that's ok?...
//static bool process_map_wad(struct wad_data *wad, bool restoring_game, short version);

/* Final three calls, must be in this order! */
static void recalculate_redundant_map(void);
static void scan_and_add_platforms(uint8 *platform_static_data, size_t count);
static void complete_loading_level(short *_map_indexes, size_t map_index_count, 
	uint8 *_platform_data, size_t platform_data_count,
	uint8 *actual_platform_data, size_t actual_platform_data_count, short version);

static uint8 *unpack_directory_data(uint8 *Stream, directory_data *Objects, size_t Count);
//static uint8 *pack_directory_data(uint8 *Stream, directory_data *Objects, int Count);

static void alert_user( int errType, int cstr, int detail, int err)
{
    //CString msg = CString(cstr);
    //MessageBox(NULL, msg, MB_OK | MB_ICONEXCLAMATION);
    printf("err");
}
static void load_redundant_map_data(
	short *redundant_data,
	size_t count)
{
	if (redundant_data)
	{
		// assert(redundant_data && map_indexes);
		uint8 *Stream = (uint8 *)redundant_data;
		MapIndexList.resize(count);
		StreamToList(Stream,map_indexes,count);
		assert(count == static_cast<size_t>(static_cast<int16>(count)));
		assert(0 <= static_cast<int16>(count));
		dynamic_world->map_index_count= static_cast<int16>(count);
	}
	else
	{
		static bool have_been_warned= false;

#if !defined(ALPHA) && !defined(BETA)
		if(!have_been_warned && false)
		{
			/* Only warn the gatherer.. */
			if(!game_is_networked || (game_is_networked && local_player_index==0))
			{
                                //show_cursor();
				alert_user(infoError, strERRORS, warningExternalMapsFile, -1);
				// LP addition: makes the game look normal
				//hide_cursor();
			}
			have_been_warned= true;
		}
#endif
		
		recalculate_redundant_map();
		precalculate_map_indexes();
	}
}

/*
 *  Unpacking/packing functions
 */

static uint8 *unpack_directory_data(uint8 *Stream, directory_data *Objects, size_t Count)
{
	uint8* S = Stream;
	directory_data* ObjPtr = Objects;

	for (size_t k = 0; k < Count; k++, ObjPtr++)
	{
		StreamToValue(S,ObjPtr->mission_flags);
		StreamToValue(S,ObjPtr->environment_flags);
		StreamToValue(S,ObjPtr->entry_point_flags);
		StreamToBytes(S,ObjPtr->level_name,LEVEL_NAME_LENGTH);
	}

	assert((S - Stream) == SIZEOF_directory_data);
	return S;
}

// ZZZ: gnu cc swears this is currently unused, and I don't see any sneaky #includes that might need it...
/*
static uint8 *pack_directory_data(uint8 *Stream, directory_data *Objects, int Count)
{
	uint8* S = Stream;
	directory_data* ObjPtr = Objects;

	for (int k = 0; k < Count; k++, ObjPtr++)
	{
		ValueToStream(S,ObjPtr->mission_flags);
		ValueToStream(S,ObjPtr->environment_flags);
		ValueToStream(S,ObjPtr->entry_point_flags);
		BytesToStream(S,ObjPtr->level_name,LEVEL_NAME_LENGTH);
	}

	assert((S - Stream) == SIZEOF_directory_data);
	return S;
}
*/
/* This takes a cstring */
void set_map_file(FileSpecifier& File)
{
	// Do whatever parameter restoration is specified before changing the file
	//if (file_is_set) RunRestorationScript();

	MapFileSpec = File;
	//set_scenario_images_file(File);
	// Only need to do this here
	//LoadLevelScripts(File);

	// Don't care whether there was an error when checking on the file's scenario images
	clear_game_error();

	file_is_set= true;
}

/* Set to the default map.. (Only if no map doubleclicked upon on startup.. */
void set_to_default_map(
	void)
{
	FileSpecifier NewMapFile;
	
	get_default_map_spec(NewMapFile);
	set_map_file(NewMapFile);
}

/* Return true if it finds the file, and it sets the mapfile to that file. */
/* Otherwise it returns false, meaning that we need have the file sent to us. */
bool use_map_file(
	uint32 checksum)
{
	FileSpecifier File;
	bool success= false;

	//if(find_wad_file_that_has_checksum(File, _typecode_scenario, strPATHS, checksum))
	//{
		set_map_file(File);
		success= true;
	//}

	return success;
}

bool load_level_from_map(
	short level_index)
{
	OpenedFile OFile;
	struct wad_header header;
	struct wad_data *wad;
	short index_to_load;
	bool restoring_game= false;

	if(file_is_set)
	{
		/* Determine what we are trying to do.. */
		if(level_index==NONE)
		{
			restoring_game= true;
			index_to_load= 0; /* Saved games are always index 0 */
		} else {
			index_to_load= level_index;
		}
		
		OpenedFile MapFile;
		if (open_wad_file_for_reading(MapFileSpec,MapFile))
		{
			/* Read the file */
			if(read_wad_header(MapFile, &header))
			{
                //struct directory_entry entry;
                //read_indexed_directory_data(MapFile, &header, index, &entry);
                //log header informations
                logHeader(&header, MapFileSpec.GetPath());

				if(index_to_load>=0 && index_to_load<header.wad_count)
				{
                    
					wad= read_indexed_wad_from_file(MapFile, &header, index_to_load, true);
					if (wad)
					{
						/* Process everything... */
						process_map_wad(wad, restoring_game, header.data_version);
		
						/* Nuke our memory... */
						free_wad(wad);
					} else {
						// error code has been set...
					}
				} else {
					set_game_error(gameError, errWadIndexOutOfRange);
				}
			} else {
				// error code has been set...
			}
		
			/* Close the file.. */
			close_wad_file(MapFile);
		} else {
			// error code has been set..
		}
	} else {
		set_game_error(gameError, errMapFileNotSet);
	}

	/* ... and bail */
	return (!error_pending());
}

//HogePiyo added
/**
    export level
    @ref process_map,export_physics_wad_data
*/
static struct wad_data* export_level_wad_data()
{
    //empty wad
    struct wad_data* wad = create_empty_wad();

    size_t size;//offset, alloc_size;
    Uint8 *data;
    //size_t count;

    //offset = 0;

    //one of lines/side/polygons must be first of all !!!
    int tags[] ={
        LINE_TAG, ENDPOINT_DATA_TAG, SIDE_TAG, POLYGON_TAG,
        ANNOTATION_TAG, OBJECT_TAG,
        MAP_INFO_TAG, ITEM_PLACEMENT_STRUCTURE_TAG,
        TERMINAL_DATA_TAG, MEDIA_TAG, AMBIENT_SOUND_TAG,
        RANDOM_SOUND_TAG, PLATFORM_STRUCTURE_TAG,
        MAP_INDEXES_TAG,

    };
    for(int i = 0; i < sizeof(tags) / sizeof(int); i ++){
        data = tag_to_global_array_and_size(tags[i], &size);
        if(data){
	        append_data_to_wad(wad, tags[i], data, size, 0);
	        //offset += alloc_size;
            delete data;
        }
    }


    //light
    //light's data is illigal
    size_t count = LightList.size();
    if(count > 0){
        size = SIZEOF_static_light_data * count;
        data = new Uint8[size];
        struct static_light_data* slights = new struct static_light_data[count];
        for(size_t i = 0; i < count; i ++){
            memcpy(&slights[i], &LightList[i].static_data, SIZEOF_static_light_data);
        }
        pack_static_light_data(data, slights, count);
        delete slights;
        append_data_to_wad(wad, LIGHTSOURCE_TAG, data, size, 0);
        delete data;
    }
    //terminal
    /*count = map_terminal_text.size();
    if(count > 0){
        size = calculate_packed_terminal_data_length();
        data = new Uint8[alloc_size];
        pack_map_terminal_data(data, count);
        append_data_to_wad(wad, TERMINAL_DATA_TAG, data, size, 0);
	    offset += alloc_size;
        delete data;
    }*/

    //map info
    /*
	int16 environment_code;
	
	int16 physics_model;
	int16 song_index;
	int16 mission_flags;
	int16 environment_flags;
	
	int16 unused[4];

	char level_name[LEVEL_NAME_LENGTH];
	uint32 entry_point_flags;
    */
    //static_world->physics_model = 0;
    /*count = 1;
    alloc_size = SIZEOF_static_data * count;
    data = new Uint8[alloc_size];
    pack_static_data(data, static_world, count);
    */

    //object placement
    /*count = 2 * MAXIMUM_OBJECT_TYPES;
    alloc_size = SIZEOF_object_frequency_definition * count;
    object_frequency_definition* objectPlacementData =
        new object_frequency_definition[count * SIZEOF_object_frequency_definition];
    //copy them
    memcpy(objectPlacementData, item_placement_info, alloc_size / 2);
    memcpy(objectPlacementData + alloc_size / 2,
        monster_placement_info, alloc_size / 2);
    data = new Uint8[alloc_size];
    */

    //physics models (disable)

    //platforms
    /*count = PlatformList.size();
    if(count > 0){
        alloc_size = SIZEOF_platform_data * count;
        data = new Uint8[alloc_size];
        pack_platform_data(data, platforms, count);
        append_data_to_wad(wad, PLATFORM_STRUCTURE_TAG, data, alloc_size, offset);
        offset += alloc_size;
        delete data;
    }*/

    //map_index
    /*count = MapIndexList.size();
    if(count > 0){
        //alloc_size = count;
        //data = new Uint8[alloc_size];
        data = tag_to_global_array_and_size(MAP_INDEXES_TAG, &count);
        //ListToStream<int16>(data, (int16*)map_indexes, count);
        append_data_to_wad(wad, MAP_INDEXES_TAG, data, count, offset);
        offset += count;
        delete data;
    }*/

    return wad;
}


//HogePiyo added
bool save_level(const char* filename){
    //
    FileSpecifier mapFileSpecifier(filename);
	OpenedFile OFile;
	struct wad_header header;
	struct wad_data *wad;

    struct directory_entry entry = {0,1,0};

    static_world->physics_model = 0;


    wad = export_level_wad_data();
    if(!wad){
        return false;
    }
    //set header to default
    fill_default_wad_header(mapFileSpecifier, CURRENT_WADFILE_VERSION,
        MARATHON_INFINITY_DATA_VERSION,
        1, 0, &header);
    //calcurate wad'd length
    long wad_length = calculate_wad_length(&header, wad);
    long offset = SIZEOF_wad_header;
    //set entry data (bugs?)
    set_indexed_directory_offset_and_length(&header, 
		&entry, 0, offset, wad_length, 0);
    //header.entry_header_size = 0;
    logEntry(&entry, header.entry_header_size, 0);
    //when use this, then crash!
    //entry.index = 0;
    //entry.length = wad_length;
    //entry.offset_to_start = offset;
    
    //open wad
    if(!open_wad_file_for_writing(mapFileSpecifier, OFile)){
        return false;
    }
    
    //write wad's header
    if(!write_wad_header(OFile, &header)){
        return false;
    }
    //unuse
    //calculate_and_store_wadfile_checksum(OFile);
    //recalculate_map_counts();

    //write wad
	if(!write_wad(OFile, &header, wad, offset)){
		//�s����
        return false;
	}

    //construct header
    offset+= wad_length;
	header.directory_offset= offset;
    header.parent_checksum = 0;
    //header.entry_header_size = 16;
    logHeader(&header, MapFileSpec.GetPath());
	write_wad_header( OFile, &header);
	write_directorys( OFile, &header, &entry);
    //calculate_and_store_wadfile_checksum(OFile);

	close_wad_file(OFile);
    free_wad(wad);
    return true;
}

/* Hopefully this is in the correct order of initialization... */
/* This sucks, beavis. */
void complete_loading_level(
	short *_map_indexes,
	size_t map_index_count,
	uint8 *_platform_data,
	size_t platform_data_count,
	uint8 *actual_platform_data,
	size_t actual_platform_data_count,
	short version)
{
	/* Scan, add the doors, recalculate, and generally tie up all loose ends */
	/* Recalculate the redundant data.. */
	load_redundant_map_data(_map_indexes, map_index_count);

	/* Add the platforms. */
	if(_platform_data || (_platform_data==NULL && actual_platform_data==NULL))
	{
		scan_and_add_platforms(_platform_data, platform_data_count);
	} else {
		assert(actual_platform_data);
		PlatformList.resize(actual_platform_data_count);
		unpack_platform_data(actual_platform_data,platforms,actual_platform_data_count);
		assert(actual_platform_data_count == static_cast<size_t>(static_cast<int16>(actual_platform_data_count)));
		assert(0 <= static_cast<int16>(actual_platform_data_count));
		dynamic_world->platform_count= static_cast<int16>(actual_platform_data_count);
	}

	scan_and_add_scenery();
	
	/* Gotta do this after recalculate redundant.. */
	if(version==MARATHON_ONE_DATA_VERSION)
	{
		short loop;
		
		for(loop= 0; loop<dynamic_world->polygon_count; ++loop)
		{
			guess_side_lightsource_indexes(loop);
		}
	}
}

/* Call with location of NULL to get the number of start locations for a */
/* given team or player */
short get_player_starting_location_and_facing(
	short team, 
	short index, 
	struct object_location *location)
{
#if 1
	short ii;
	struct map_object *saved_object;
	short count= 0;
	bool done= false;
	
	saved_object= saved_objects;
	for(ii=0; !done && ii<dynamic_world->initial_objects_count; ++ii)
	{
		if(saved_object->type==_saved_player)
		{
			/* index=NONE means use any starting location */
			if(saved_object->index==team || team==NONE)
			{
				if(location && count==index)
				{
					location->p= saved_object->location;
					location->polygon_index= saved_object->polygon_index;
					location->yaw= saved_object->facing;
					location->pitch= 0;
					location->flags= saved_object->flags;
					done= true;
				}
				count++;
			}
		}
		++saved_object;
	}
	
	/* If they asked for a valid location, make sure that we gave them one */
	if(location) vassert(done, csprintf(temporary, "Tried to place: %d only %d starting pts.", index, count));
	
	return count;
#else
	location->x= 0x14e9;
	location->y= 0x1ba0;
	*facing= 0x00;
	*polygon_index= 0x6f;

	return 1;
#endif
}

uint32 get_current_map_checksum(
	void)
{
	// fileref file_handle;
	struct wad_header header;

	assert(file_is_set);
	OpenedFile MapFile;
	open_wad_file_for_reading(MapFileSpec, MapFile);
	assert(MapFile.IsOpen());

	/* Read the file */
	read_wad_header(MapFile, &header);
	
	/* Close the file.. */
	close_wad_file(MapFile);	
	
	return header.checksum;
}

// ZZZ: split this out from new_game for sharing
void set_saved_game_name_to_default()
{
#if defined(mac) || defined(SDL_RFORK_HACK)
	revert_game_data.SavedGame.SetToApp();
	revert_game_data.SavedGame.SetName(getcstr(temporary, strFILENAMES, filenameDEFAULT_SAVE_GAME),_typecode_savegame);
#endif
#if defined(SDL) && !defined(SDL_RFORK_HACK)
	revert_game_data.SavedGame.SetToSavedGamesDir();
	revert_game_data.SavedGame += getcstr(temporary, strFILENAMES, filenameDEFAULT_SAVE_GAME);
#endif
}
/*
bool new_game(
	short number_of_players, 
	bool network,
	struct game_data *game_information,
	struct player_start_data *player_start_information,
	struct entry_point *entry_point)
    */

bool get_indexed_entry_point(
	struct entry_point *entry_point, 
	short *index, 
	int32 type)
{
	short actual_index;
	
	// Open map file
	assert(file_is_set);
	OpenedFile MapFile;
	if (!open_wad_file_for_reading(MapFileSpec,MapFile))
		return false;

	// Read header
	wad_header header;
	if (!read_wad_header(MapFile, &header)) {
		close_wad_file(MapFile);
		return false;
	}

	bool success = false;
	if (header.application_specific_directory_data_size == SIZEOF_directory_data)
	{

		// New style wad
		void *total_directory_data= read_directory_data(MapFile, &header);

		assert(total_directory_data);
		for(actual_index= *index; actual_index<header.wad_count; ++actual_index)
		{
			uint8 *p = (uint8 *)get_indexed_directory_data(&header, actual_index, total_directory_data);
			directory_data directory;
			unpack_directory_data(p, &directory, 1);

			/* Find the flags that match.. */
			if(directory.entry_point_flags & type)
			{
				/* This one is valid! */
				entry_point->level_number= actual_index;
				strcpy(entry_point->level_name, directory.level_name);
			
				*index= actual_index+1;
				success= true;
				break; /* Out of the for loop */
			}
		}
		free(total_directory_data);

	} else {

		// Old style wad, find the index
		for(actual_index= *index; !success && actual_index<header.wad_count; ++actual_index)
		{
			struct wad_data *wad;

			/* Read the file */
			wad= read_indexed_wad_from_file(MapFile, &header, actual_index, true);
			if (wad)
			{
				/* IF this has the proper type.. */
				size_t length;
				uint8 *p = (uint8 *)extract_type_from_wad(wad, MAP_INFO_TAG, &length);
				assert(length == SIZEOF_static_data);
				static_data map_info;
				unpack_static_data(p, &map_info, 1);

				if(map_info.entry_point_flags & type)
				{
					/* This one is valid! */
					entry_point->level_number= actual_index;
					assert(strlen(map_info.level_name)<LEVEL_NAME_LENGTH);
					strcpy(entry_point->level_name, map_info.level_name);
		
					*index= actual_index+1;
					success= true;
				}
				
				free_wad(wad);
			}
		}
	}

	return success;
}

// Get vector of map entry points matching given type
bool get_entry_points(vector<entry_point> &vec, int32 type)
{
	vec.clear();

	// Open map file
	assert(file_is_set);
	OpenedFile MapFile;
	if (!open_wad_file_for_reading(MapFileSpec,MapFile))
		return false;

	// Read header
	wad_header header;
	if (!read_wad_header(MapFile, &header)) {
		close_wad_file(MapFile);
		return false;
	}

	bool success = false;
	if (header.application_specific_directory_data_size == SIZEOF_directory_data) {

		// New style wad, read directory data
		void *total_directory_data = read_directory_data(MapFile, &header);
		assert(total_directory_data);

		// Push matching directory entries into vector
		for (int i=0; i<header.wad_count; i++) {
			uint8 *p = (uint8 *)get_indexed_directory_data(&header, i, total_directory_data);
			directory_data directory;
			unpack_directory_data(p, &directory, 1);

			if (directory.entry_point_flags & type) {

				// This one is valid
				entry_point point;
				point.level_number = i;
				strcpy(point.level_name, directory.level_name);
				vec.push_back(point);
				success = true;
			}
		}
		free(total_directory_data);

	} else {

		// Old style wad
		for (int i=0; i<header.wad_count; i++) {

			wad_data *wad = read_indexed_wad_from_file(MapFile, &header, i, true);
			if (!wad)
				continue;

			// Read map_info data
			size_t length;
			uint8 *p = (uint8 *)extract_type_from_wad(wad, MAP_INFO_TAG, &length);
			assert(length == SIZEOF_static_data);
			static_data map_info;
			unpack_static_data(p, &map_info, 1);

			if (map_info.entry_point_flags & type) {

				// This one is valid
				entry_point point;
				point.level_number = i;
				assert(strlen(map_info.level_name) < LEVEL_NAME_LENGTH);
				strcpy(point.level_name, map_info.level_name);
				vec.push_back(point);
				success = true;
			}
				
			free_wad(wad);
		}
	}

	return success;
}

/* This is called when the game level is changed somehow */
/* The only thing that has to be valid in the entry point is the level_index */

/* Returns a short that is an OSErr... *
bool goto_level(
	struct entry_point *entry, 
	bool new_game)
*/

/* -------------------- Private or map editor functions */
void allocate_map_for_counts(
	size_t polygon_count, 
	size_t side_count,
	size_t endpoint_count,
	size_t line_count)
{
	//long cumulative_length= 0;
	size_t automap_line_count, automap_polygon_count, map_index_count;
	// long automap_line_length, automap_polygon_length, map_index_length;

	/* Give the map indexes a whole bunch of memory (cause we can't calculate it) */
	// map_index_length= (polygon_count*32+1024)*sizeof(int16);
	map_index_count= (polygon_count*32+1024);
	
	/* Automap lines. */
	// automap_line_length= (line_count/8+((line_count%8)?1:0))*sizeof(byte);
	automap_line_count= (line_count/8+((line_count%8)?1:0));
	
	/* Automap Polygons */
	// automap_polygon_length= (polygon_count/8+((polygon_count%8)?1:0))*sizeof(byte);
	automap_polygon_count= (polygon_count/8+((polygon_count%8)?1:0));

	// cumulative_length+= polygon_count*sizeof(struct polygon_data);
	// cumulative_length+= side_count*sizeof(struct side_data);
	// cumulative_length+= endpoint_count*sizeof(struct endpoint_data);
	// cumulative_length+= line_count*sizeof(struct line_data);
	// cumulative_length+= map_index_length;
	// cumulative_length+= automap_line_length;
	// cumulative_length+= automap_polygon_length;

	/* Okay, we now have the length.  Allocate our block.. */
	// reallocate_map_structure_memory(cumulative_length);

	/* Tell the recalculation data how big it is.. */
	// set_map_index_buffer_size(map_index_length);

	/* Setup our pointers. */
	// map_polygons= (struct polygon_data *) get_map_structure_chunk(polygon_count*sizeof(struct polygon_data));
	// map_sides= (struct side_data *) get_map_structure_chunk(side_count*sizeof(struct side_data));
	// map_endpoints= (struct endpoint_data *) get_map_structure_chunk(endpoint_count*sizeof(struct endpoint_data));
	// map_lines= (struct line_data *) get_map_structure_chunk(line_count*sizeof(struct line_data));
	// map_indexes= (short *) get_map_structure_chunk(map_index_length);
	// automap_lines= (uint8 *) get_map_structure_chunk(automap_line_length);
	// automap_polygons= (uint8 *) get_map_structure_chunk(automap_polygon_length);
	
	// Most of the other stuff: reallocate here
	EndpointList.resize(endpoint_count);
	LineList.resize(line_count);
	SideList.resize(side_count);
	PolygonList.resize(polygon_count);
	AutomapLineList.resize(automap_line_count);
	AutomapPolygonList.resize(automap_polygon_count);
	
	// Map indexes: start off with none of them (of course),
	// but reserve a size equal to the map index length
	MapIndexList.clear();
	MapIndexList.reserve(map_index_count);
	dynamic_world->map_index_count= 0;
	
	// Stuff that needs the max number of polygons
	//allocate_render_memory();
	allocate_flood_map_memory();
}

void load_points(
	uint8 *points,
	size_t count)
{
	size_t loop;
	
	// OK to modify input-data pointer since it's called by value
	for(loop=0; loop<count; ++loop)
	{
		world_point2d& vertex = map_endpoints[loop].vertex;
		StreamToValue(points,vertex.x);
		StreamToValue(points,vertex.y);
	}
	assert(count == static_cast<size_t>(static_cast<int16>(count)));
	assert(0 <= static_cast<int16>(count));
	dynamic_world->endpoint_count= static_cast<int16>(count);
}

void load_lines(
	uint8 *lines, 
	size_t count)
{
	// assert(count>=0 && count<=MAXIMUM_LINES_PER_MAP);
	unpack_line_data(lines,map_lines,count);
	assert(count == static_cast<size_t>(static_cast<int16>(count)));
	assert(0 <= static_cast<int16>(count));
	dynamic_world->line_count= static_cast<int16>(count);
}

void load_sides(
	uint8 *sides, 
	size_t count,
	short version)
{
	size_t loop;
	
	// assert(count>=0 && count<=MAXIMUM_SIDES_PER_MAP);

	unpack_side_data(sides,map_sides,count);

	for(loop=0; loop<count; ++loop)
	{
		if(version==MARATHON_ONE_DATA_VERSION)
		{
			map_sides[loop].transparent_texture.texture= UNONE;
			map_sides[loop].ambient_delta= 0;
		}
		++sides;
	}

	assert(count == static_cast<size_t>(static_cast<int16>(count)));
	assert(0 <= static_cast<int16>(count));
	dynamic_world->side_count= static_cast<int16>(count);
}

void load_polygons(
	uint8 *polys, 
	size_t count,
	short version)
{
	size_t loop;

	// assert(count>=0 && count<=MAXIMUM_POLYGONS_PER_MAP);
	
	unpack_polygon_data(polys,map_polygons,count);
	assert(count == static_cast<size_t>(static_cast<int16>(count)));
	assert(0 <= static_cast<int16>(count));
	dynamic_world->polygon_count= static_cast<int16>(count);

	/* Allow for backward compatibility! */
	switch(version)
	{
		case MARATHON_ONE_DATA_VERSION:
			for(loop= 0; loop<count; ++loop)
			{
				map_polygons[loop].media_index= NONE;
				map_polygons[loop].floor_origin.x= map_polygons[loop].floor_origin.y= 0;
				map_polygons[loop].ceiling_origin.x= map_polygons[loop].ceiling_origin.y= 0;
			}
			break;
			
		case MARATHON_TWO_DATA_VERSION:
		// LP addition:
		case MARATHON_INFINITY_DATA_VERSION:
			break;
			
		default:
			assert(false);
			break;
	}
}

void load_lights(
	uint8 *_lights, 
	size_t count,
	short version)
{
	unsigned short loop, new_index;
	
	LightList.resize(count);
	objlist_clear(lights,count);
	// vassert(count>=0 && count<=MAXIMUM_LIGHTS_PER_MAP, csprintf(temporary, "Light count: %d vers: %d",
	//	count, version));
	
	old_light_data *OldLights;
	
	switch(version)
	{
	case MARATHON_ONE_DATA_VERSION: {
		
		// Unpack the old lights into a temporary array
		OldLights = new old_light_data[count];
		unpack_old_light_data(_lights,OldLights,count);
		
		old_light_data *OldLtPtr = OldLights;
		for(loop= 0; loop<count; ++loop, OldLtPtr++)
		{
			static_light_data TempLight;
			convert_old_light_data_to_new(&TempLight, OldLtPtr, 1);
			
			new_index = new_light(&TempLight);
			assert(new_index==loop);
		}
		delete []OldLights;
		break;			
	}
		
	case MARATHON_TWO_DATA_VERSION:
	case MARATHON_INFINITY_DATA_VERSION:
		// OK to modify the data pointer since it was passed by value
		for(loop= 0; loop<count; ++loop)
		{
			static_light_data TempLight;
			_lights = unpack_static_light_data(_lights, &TempLight, 1);
			
			new_index = new_light(&TempLight);
			assert(new_index==loop);
		}
		break;			
		
	default:
		assert(false);
		break;
	}
}

void load_annotations(
	uint8 *annotations, 
	size_t count)
{
	// assert(count>=0 && count<=MAXIMUM_ANNOTATIONS_PER_MAP);
	MapAnnotationList.resize(count);
	unpack_map_annotation(annotations,map_annotations,count);
	assert(count == static_cast<size_t>(static_cast<int16>(count)));
	assert(0 <= static_cast<int16>(count));
	dynamic_world->default_annotation_count= static_cast<int16>(count);
}

void load_objects(uint8 *map_objects, size_t count)
{
	// assert(count>=0 && count<=MAXIMUM_SAVED_OBJECTS);
	SavedObjectList.resize(count);
	unpack_map_object(map_objects,saved_objects,count);
	assert(count == static_cast<size_t>(static_cast<int16>(count)));
	assert(0 <= static_cast<int16>(count));
	dynamic_world->initial_objects_count= static_cast<int16>(count);
}

void load_map_info(
	uint8 *map_info)
{
	unpack_static_data(map_info,static_world,1);
}

void load_media(
	uint8 *_medias,
	size_t count)
{
	// struct media_data *media= _medias;
	size_t ii;
	
	MediaList.resize(count);
	objlist_clear(medias,count);
	// assert(count>=0 && count<=MAXIMUM_MEDIAS_PER_MAP);
	
	for(ii= 0; ii<count; ++ii)
	{
		media_data TempMedia;
		_medias = unpack_media_data(_medias,&TempMedia,1);
		
		size_t new_index = new_media(&TempMedia);
		assert(new_index==ii);
	}
}

void load_ambient_sound_images(
	uint8 *data,
	size_t count)
{
	// assert(count>=0 &&count<=MAXIMUM_AMBIENT_SOUND_IMAGES_PER_MAP);
	AmbientSoundImageList.resize(count);
	unpack_ambient_sound_image_data(data,ambient_sound_images,count);
	assert(count == static_cast<size_t>(static_cast<int16>(count)));
	assert(0 <= static_cast<int16>(count));
	dynamic_world->ambient_sound_image_count= static_cast<int16>(count);
}

void load_random_sound_images(
	uint8 *data,
	size_t count)
{
	// assert(count>=0 &&count<=MAXIMUM_RANDOM_SOUND_IMAGES_PER_MAP);
	RandomSoundImageList.resize(count);
	unpack_random_sound_image_data(data,random_sound_images,count);
	assert(count == static_cast<size_t>(static_cast<int16>(count)));
	assert(0 <= static_cast<int16>(count));
	dynamic_world->random_sound_image_count= static_cast<int16>(count);
}

/* Recalculate all the redundant crap- must be done before platforms/doors/etc.. */
void recalculate_redundant_map(
	void)
{
	short loop;

	for(loop=0;loop<dynamic_world->polygon_count;++loop) recalculate_redundant_polygon_data(loop);
	for(loop=0;loop<dynamic_world->line_count;++loop) recalculate_redundant_line_data(loop);
	for(loop=0;loop<dynamic_world->endpoint_count;++loop) recalculate_redundant_endpoint_data(loop);
}

extern bool load_game_from_file(FileSpecifier& File);

//bool load_game_from_file(FileSpecifier& File)

void setup_revert_game_info(
	struct game_data *game_info, 
	struct player_start_data *start, 
	struct entry_point *entry)
{
	revert_game_data.game_is_from_disk = false;
	obj_copy(revert_game_data.game_information, *game_info);
	obj_copy(revert_game_data.player_start, *start);
	obj_copy(revert_game_data.entry_point, *entry);
}

/*bool revert_game(
	void)
*/

void get_current_saved_game_name(FileSpecifier& File)
{
	File = revert_game_data.SavedGame;
}

/* The current mapfile should be set to the save game file... */
bool save_game_file(FileSpecifier& File)
{
	struct wad_header header;
	short err = 0;
	bool success= false;
	long offset, wad_length;
	struct directory_entry entry;
	struct wad_data *wad;

	/* Save off the random seed. */
	dynamic_world->random_seed= get_random_seed();

	/* Setup to revert the game properly */
	revert_game_data.game_is_from_disk= true;
	revert_game_data.SavedGame = File;
	
	// LP: add a file here; use temporary file for a safe save.
	// Write into the temporary file first
	FileSpecifier TempFile;
	DirectorySpecifier TempFileDir;
	File.ToDirectory(TempFileDir);
	TempFile.FromDirectory(TempFileDir);
#if defined(mac) || defined(SDL_RFORK_HACK)
	TempFile.SetName("savetemp.dat",NONE);
#else
	TempFile.AddPart("savetemp.dat");
#endif
	
	/* Fill in the default wad header (we are using File instead of TempFile to get the name right in the header) */
	fill_default_wad_header(File, CURRENT_WADFILE_VERSION, EDITOR_MAP_VERSION, 1, 0, &header);
		
	/* Assume that we confirmed on save as... */
	if (create_wadfile(TempFile,_typecode_savegame))
	{
		OpenedFile SaveFile;
		if(open_wad_file_for_writing(TempFile,SaveFile))
		{
			/* Write out the new header */
			if (write_wad_header(SaveFile, &header))
			{
				offset= SIZEOF_wad_header;
		
				wad= build_save_game_wad(&header, &wad_length);
				if (wad)
				{
					/* Set the entry data.. */
					set_indexed_directory_offset_and_length(&header, 
						&entry, 0, offset, wad_length, 0);
					
					/* Save it.. */
					if (write_wad(SaveFile, &header, wad, offset))
					{
						/* Update the new header */
						offset+= wad_length;
						header.directory_offset= offset;
						header.parent_checksum= read_wad_file_checksum(MapFileSpec);
						if (write_wad_header(SaveFile, &header) && write_directorys(SaveFile, &header, &entry))
						{
							/* This function saves the overhead map as a thumbnail, as well */
							/*  as adding the string resource that tells you what it is when */
							/*  it is clicked on & Marathon2 isn't installed.  Obviously, both */
							/*  of these are superfluous for a dos environment. */
							add_finishing_touches_to_save_file(TempFile);

							/* We win. */
							success= true;
						} 
					}

					free_wad(wad);
				}
			}

			err = SaveFile.GetError();
			close_wad_file(SaveFile);
		}
		
		// LP addition: exchange with temporary file;
		// create target file if necessary
		if (!err)
		{
			if (!File.Exists())
				create_wadfile(File,_typecode_savegame);
			
			TempFile.Exchange(File);
			err = TempFile.GetError();
			TempFile.Delete(); // it's not an error if this fails
		}
	}
	
	if(err || error_pending())
	{
		if(!err) err= get_game_error(NULL);
		//alert_user(infoError, strERRORS, fileError, err);
		clear_game_error();
		success= false;
	}
	
	return success;
}

/* -------- static functions */
static void scan_and_add_platforms(
	uint8 *platform_static_data,
	size_t count)
{
	struct polygon_data *polygon;
	short loop;
	
	PlatformList.resize(count);
	objlist_clear(platforms,count);
	polygon= map_polygons;
	for(loop=0; loop<dynamic_world->polygon_count; ++loop)
	{
		if (polygon->type==_polygon_is_platform)
		{
			/* Search and find the extra data.  If it is not there, use the permutation for */
			/* backwards compatibility! */

			uint8 *_static_data= platform_static_data;
			size_t platform_static_data_index;
			for(platform_static_data_index = 0; platform_static_data_index<count; ++platform_static_data_index)
			{
				static_platform_data TempPlatform;
				_static_data = unpack_static_platform_data(_static_data, &TempPlatform, 1);
				if(TempPlatform.polygon_index==loop)
				{
					new_platform(&TempPlatform, loop);
					break;
				}
			}
			
			/* DIdn't find it- use a standard platform */
			if(platform_static_data_index==count)
			{
				polygon->permutation= 1;
				new_platform(get_defaults_for_platform_type(polygon->permutation), loop);
			}	
		}
		++polygon;
	}
}

struct wad_data* store_data_to_wad()
{
    struct wad_data* wad = NULL;
    return wad;
}


/* Load a level from a wad-> mainly used by the net stuff. */
bool process_map_wad(
	struct wad_data *wad, 
	bool restoring_game,
	short version)
{
	size_t data_length;
	uint8 *data;
	size_t count;
	bool is_preprocessed_map= false;

	assert(version==MARATHON_INFINITY_DATA_VERSION || version==MARATHON_TWO_DATA_VERSION || version==MARATHON_ONE_DATA_VERSION);

	/* zero everything so no slots are used */	
	initialize_map_for_new_level();

	/* Calculate the length (for reallocate map) */
	allocate_map_structure_for_map(wad);

	/* Extract points */
	data= (uint8 *)extract_type_from_wad(wad, POINT_TAG, &data_length);
    if(data){
	    count= data_length/SIZEOF_world_point2d;
        assert(data_length == count*SIZEOF_world_point2d);
    }else{
        count = 0;
    }
	
	if(count)
	{
		load_points(data, count);
	} else {
         
		data= (uint8 *)extract_type_from_wad(wad, ENDPOINT_DATA_TAG, &data_length);
        assert(data);
	    count= data_length/SIZEOF_endpoint_data;
	    assert(data_length == count*SIZEOF_endpoint_data);
	    // assert(count>=0 && count<MAXIMUM_ENDPOINTS_PER_MAP);

	    /* Slam! */
	    unpack_endpoint_data(data,map_endpoints,count);
	    assert(count == static_cast<size_t>(static_cast<int16>(count)));
	    assert(0 <= static_cast<int16>(count));
	    dynamic_world->endpoint_count= static_cast<int16>(count);

	    is_preprocessed_map= true;
	}

	/* Extract lines */
	data= (uint8 *)extract_type_from_wad(wad, LINE_TAG, &data_length);
    assert(data);
    count = data_length/SIZEOF_line_data;
    assert(data_length == count*SIZEOF_line_data);
    load_lines(data, count);

    /* Order is important! */
	data= (uint8 *)extract_type_from_wad(wad, SIDE_TAG, &data_length);
    assert(data);
    count = data_length/SIZEOF_side_data;
    assert(data_length == count*SIZEOF_side_data);
    load_sides(data, count, version);

	/* Extract polygons */
	data= (uint8 *)extract_type_from_wad(wad, POLYGON_TAG, &data_length);
    assert(data);
    if(data){
	    count = data_length/SIZEOF_polygon_data;
	    assert(data_length == count*SIZEOF_polygon_data);
	    load_polygons(data, count, version);
    }

	/* Extract the lightsources */
	if(restoring_game)
	{
		// Slurp them in
		data= (uint8 *)extract_type_from_wad(wad, LIGHTSOURCE_TAG, &data_length);
        if(data){
		    count = data_length/SIZEOF_light_data;
		    assert(data_length == count*SIZEOF_light_data);
		    LightList.resize(count);
		    unpack_light_data(data,lights,count);
        }
	}
	else
	{
		/* When you are restoring a game, the actual light structure is set. */
		data= (uint8 *)extract_type_from_wad(wad, LIGHTSOURCE_TAG, &data_length);
        if(data){
		    if(version==MARATHON_ONE_DATA_VERSION) 
		    {
			    /* We have an old style light */
			    count= data_length/SIZEOF_old_light_data;
			    assert(count*SIZEOF_old_light_data==data_length);
			    load_lights(data, count, version);
		    } else {
			    count= data_length/SIZEOF_static_light_data;
			    assert(count*SIZEOF_static_light_data==data_length);
			    load_lights(data, count, version);
		    }
        }

		//	HACK!!!!!!!!!!!!!!! vulcan doesn�t NONE .first_object field after adding scenery
		{
            if(PolygonList.size() > 0){
			    for (count= 0; count<static_cast<size_t>(dynamic_world->polygon_count) &&
                    count < PolygonList.size(); ++count)
			    {
				    map_polygons[count].first_object= NONE;
			    }
            }
		}
	}

	/* Extract the annotations */
	data= (uint8 *)extract_type_from_wad(wad, ANNOTATION_TAG, &data_length);
    if(data){
	    count = data_length/SIZEOF_map_annotation;
	    assert(data_length == count*SIZEOF_map_annotation);
	    load_annotations(data, count);
    }else{
        MapAnnotationList.clear();
    }

	/* Extract the objects */
	data= (uint8 *)extract_type_from_wad(wad, OBJECT_TAG, &data_length);
    if(data){
	    count = data_length/SIZEOF_map_object;
	    assert(data_length == count*static_cast<size_t>(SIZEOF_map_object));
	    load_objects(data, count);
    }
	/* Extract the map info data */
	data= (uint8 *)extract_type_from_wad(wad, MAP_INFO_TAG, &data_length);
    if(data){
	    // LP change: made this more Pfhorte-friendly
	    assert(static_cast<size_t>(SIZEOF_static_data)==data_length 
		    || static_cast<size_t>(SIZEOF_static_data-2)==data_length);
	    load_map_info(data);
    }

	/* Extract the game difficulty info.. */
	data= (uint8 *)extract_type_from_wad(wad, ITEM_PLACEMENT_STRUCTURE_TAG, &data_length);
    if(data){
	    // In case of an absent placement chunk...
	    if (data_length == 0)
	    {
		    data = new uint8[2*MAXIMUM_OBJECT_TYPES*SIZEOF_object_frequency_definition];
		    memset(data,0,2*MAXIMUM_OBJECT_TYPES*SIZEOF_object_frequency_definition);
	    }
	    else
		    assert(data_length == 2*MAXIMUM_OBJECT_TYPES*SIZEOF_object_frequency_definition);
	    load_placement_data(data + MAXIMUM_OBJECT_TYPES*SIZEOF_object_frequency_definition, data);
	    if (data_length == 0)
		    delete []data;
    }
	
	/* Extract the terminal data. */
	data= (uint8 *)extract_type_from_wad(wad, TERMINAL_DATA_TAG, &data_length);
    if(data){
    	load_terminal_data(data, data_length);
    }

	/* Extract the media definitions */
	if(restoring_game)
	{
		// Slurp it in
		data= (uint8 *)extract_type_from_wad(wad, MEDIA_TAG, &data_length);
		count= data_length/SIZEOF_media_data;
		assert(count*SIZEOF_media_data==data_length);
		MediaList.resize(count);
		unpack_media_data(data,medias,count);
	}
	else
	{
		data= (uint8 *)extract_type_from_wad(wad, MEDIA_TAG, &data_length);
        if(data){
		    count= data_length/SIZEOF_media_data;
		    assert(count*SIZEOF_media_data==data_length);
		    load_media(data, count);
        }
	}

	/* Extract the ambient sound images */
	data= (uint8 *)extract_type_from_wad(wad, AMBIENT_SOUND_TAG, &data_length);
    if(data){
	    count = data_length/SIZEOF_ambient_sound_image_data;
	    assert(data_length == count*SIZEOF_ambient_sound_image_data);
	    load_ambient_sound_images(data, count);
	    load_ambient_sound_images(data, data_length/SIZEOF_ambient_sound_image_data);
    }

	/* Extract the random sound images */
	data= (uint8 *)extract_type_from_wad(wad, RANDOM_SOUND_TAG, &data_length);
    if(data){
	    count = data_length/SIZEOF_random_sound_image_data;
	    assert(data_length == count*SIZEOF_random_sound_image_data);
	    load_random_sound_images(data, count);
    }

	// LP addition: load the physics-model chunks (all fixed-size)
	bool PhysicsModelLoaded = false;
	
	data= (uint8 *)extract_type_from_wad(wad, MONSTER_PHYSICS_TAG, &data_length);
	count = data_length/SIZEOF_monster_definition;
	assert(count*SIZEOF_monster_definition == data_length);
	assert(count <= NUMBER_OF_MONSTER_TYPES);
	if (data_length > 0)
	{
		PhysicsModelLoaded = true;
		unpack_monster_definition(data,count);
	}
	
	data= (uint8 *)extract_type_from_wad(wad, EFFECTS_PHYSICS_TAG, &data_length);
	count = data_length/SIZEOF_effect_definition;
	assert(count*SIZEOF_effect_definition == data_length);
	assert(count <= NUMBER_OF_EFFECT_TYPES);
	if (data_length > 0)
	{
		PhysicsModelLoaded = true;
		unpack_effect_definition(data,count);
	}
	
	data= (uint8 *)extract_type_from_wad(wad, PROJECTILE_PHYSICS_TAG, &data_length);
	count = data_length/SIZEOF_projectile_definition;
	assert(count*SIZEOF_projectile_definition == data_length);
	assert(count <= NUMBER_OF_PROJECTILE_TYPES);
	if (data_length > 0)
	{
		PhysicsModelLoaded = true;
		unpack_projectile_definition(data,count);
	}
	
	data= (uint8 *)extract_type_from_wad(wad, PHYSICS_PHYSICS_TAG, &data_length);
	count = data_length/SIZEOF_physics_constants;
	assert(count*SIZEOF_physics_constants == data_length);
	assert(count <= get_number_of_physics_models());
	if (data_length > 0)
	{
		PhysicsModelLoaded = true;
		unpack_physics_constants(data,count);
	}
	
	data= (uint8 *)extract_type_from_wad(wad, WEAPONS_PHYSICS_TAG, &data_length);
	count = data_length/SIZEOF_weapon_definition;
	assert(count*SIZEOF_weapon_definition == data_length);
	assert(count <= get_number_of_weapon_types());
	if (data_length > 0)
	{
		PhysicsModelLoaded = true;
		unpack_weapon_definition(data,count);
	}
	
	// LP addition: Reload the physics model if it had been loaded in the previous level,
	// but not in the current level. This avoids the persistent-physics bug.
	if (PhysicsModelLoadedEarlier && !PhysicsModelLoaded)
		import_definition_structures();
	PhysicsModelLoadedEarlier = PhysicsModelLoaded;
		
	/* If we are restoring the game, then we need to add the dynamic data */
	if(restoring_game)
	{
		// Slurp it all in...
		data= (uint8 *)extract_type_from_wad(wad, MAP_INDEXES_TAG, &data_length);
		count= data_length/sizeof(short);
		assert(count*long(sizeof(short))==data_length);
		MapIndexList.resize(count);
		StreamToList(data,map_indexes,count);
		
		data= (uint8 *)extract_type_from_wad(wad, PLAYER_STRUCTURE_TAG, &data_length);
		count= data_length/SIZEOF_player_data;
		assert(count*SIZEOF_player_data==data_length);
		unpack_player_data(data,players,count);
		team_damage_from_player_data();
		
		data= (uint8 *)extract_type_from_wad(wad, DYNAMIC_STRUCTURE_TAG, &data_length);
		assert(data_length == SIZEOF_dynamic_data);
		unpack_dynamic_data(data,dynamic_world,1);
		
		data= (uint8 *)extract_type_from_wad(wad, OBJECT_STRUCTURE_TAG, &data_length);
		count= data_length/SIZEOF_object_data;
		assert(count*SIZEOF_object_data==data_length);
		vassert(count <= MAXIMUM_OBJECTS_PER_MAP,
			csprintf(temporary,"Number of map objects %lu > limit %u",count,MAXIMUM_OBJECTS_PER_MAP));
		unpack_object_data(data,objects,count);
		
		// Unpacking is E-Z here...
		data= (uint8 *)extract_type_from_wad(wad, AUTOMAP_LINES, &data_length);
		memcpy(automap_lines,data,data_length);
		data= (uint8 *)extract_type_from_wad(wad, AUTOMAP_POLYGONS, &data_length);
		memcpy(automap_polygons,data,data_length);

		data= (uint8 *)extract_type_from_wad(wad, MONSTERS_STRUCTURE_TAG, &data_length);
		count= data_length/SIZEOF_monster_data;
		assert(count*SIZEOF_monster_data==data_length);
		vassert(count <= MAXIMUM_MONSTERS_PER_MAP,
			csprintf(temporary,"Number of monsters %lu > limit %u",count,MAXIMUM_MONSTERS_PER_MAP));
		unpack_monster_data(data,monsters,count);

		data= (uint8 *)extract_type_from_wad(wad, EFFECTS_STRUCTURE_TAG, &data_length);
		count= data_length/SIZEOF_effect_data;
		assert(count*SIZEOF_effect_data==data_length);
		vassert(count <= MAXIMUM_EFFECTS_PER_MAP,
			csprintf(temporary,"Number of effects %lu > limit %u",count,MAXIMUM_EFFECTS_PER_MAP));
		unpack_effect_data(data,effects,count);

		data= (uint8 *)extract_type_from_wad(wad, PROJECTILES_STRUCTURE_TAG, &data_length);
		count= data_length/SIZEOF_projectile_data;
		assert(count*SIZEOF_projectile_data==data_length);
		vassert(count <= MAXIMUM_PROJECTILES_PER_MAP,
			csprintf(temporary,"Number of projectiles %lu > limit %u",count,MAXIMUM_PROJECTILES_PER_MAP));
		unpack_projectile_data(data,projectiles,count);
		
		data= (uint8 *)extract_type_from_wad(wad, PLATFORM_STRUCTURE_TAG, &data_length);
		count= data_length/SIZEOF_platform_data;
		assert(count*SIZEOF_platform_data==data_length);
		PlatformList.resize(count);
		unpack_platform_data(data,platforms,count);
		
		data= (uint8 *)extract_type_from_wad(wad, WEAPON_STATE_TAG, &data_length);
		count= data_length/SIZEOF_player_weapon_data;
		assert(count*SIZEOF_player_weapon_data==data_length);
		unpack_player_weapon_data(data,count);
		
		data= (uint8 *)extract_type_from_wad(wad, TERMINAL_STATE_TAG, &data_length);
		count= data_length/SIZEOF_player_terminal_data;
		assert(count*SIZEOF_player_terminal_data==data_length);
		unpack_player_terminal_data(data,count);
		
		complete_restoring_level(wad);
	} else {
		uint8 *map_index_data;
		size_t map_index_count;
		uint8 *platform_structures;
		size_t platform_structure_count;

		if(version==MARATHON_ONE_DATA_VERSION)
		{
			/* Force precalculation */
			map_index_data= NULL;
			map_index_count= 0; 
		} else {
			map_index_data= (uint8 *)extract_type_from_wad(wad, MAP_INDEXES_TAG, &data_length);
			map_index_count= data_length/sizeof(short);
			assert(map_index_count*sizeof(short)==data_length);
		}

		assert(is_preprocessed_map&&map_index_count || !is_preprocessed_map&&!map_index_count);

		data= (uint8 *)extract_type_from_wad(wad, PLATFORM_STATIC_DATA_TAG, &data_length);
        //if(data){
		    count= data_length/SIZEOF_static_platform_data;
		    assert(count*SIZEOF_static_platform_data==data_length);
    		
		    platform_structures= (uint8 *)extract_type_from_wad(wad, PLATFORM_STRUCTURE_TAG, &data_length);
		    platform_structure_count= data_length/SIZEOF_platform_data;
		    assert(platform_structure_count*SIZEOF_platform_data==data_length);
    		
		    complete_loading_level((short *) map_index_data, map_index_count,
			    data, count, platform_structures,
			    platform_structure_count, version);
        //}
	}
	
	/* ... and bail */
	return true;
}

static void allocate_map_structure_for_map(
	struct wad_data *wad)
{
	size_t data_length;
	size_t line_count, polygon_count, side_count, endpoint_count;

	/* Extract points */
	extract_type_from_wad(wad, POINT_TAG, &data_length);
	endpoint_count= data_length/SIZEOF_world_point2d;
	if(endpoint_count*SIZEOF_world_point2d!=data_length) alert_user(fatalError, strERRORS, corruptedMap, 0x7074); // 'pt'
	
	if(!endpoint_count)
	{
		extract_type_from_wad(wad, ENDPOINT_DATA_TAG, &data_length);
		endpoint_count= data_length/SIZEOF_endpoint_data;
        assert(data_length);
		if(endpoint_count*SIZEOF_endpoint_data!=data_length) alert_user(fatalError, strERRORS, corruptedMap, 0x6570); // 'ep'
	}

	/* Extract lines */
	extract_type_from_wad(wad, LINE_TAG, &data_length);
	line_count= data_length/SIZEOF_line_data;
    assert(data_length);
	if(line_count*SIZEOF_line_data!=data_length) alert_user(fatalError, strERRORS, corruptedMap, 0x6c69); // 'li'

	/* Sides.. */
	extract_type_from_wad(wad, SIDE_TAG, &data_length);
	side_count= data_length/SIZEOF_side_data;
    assert(data_length);
	if(side_count*SIZEOF_side_data!=data_length) alert_user(fatalError, strERRORS, corruptedMap, 0x7369); // 'si'

	/* Extract polygons */
	extract_type_from_wad(wad, POLYGON_TAG, &data_length);
	polygon_count= data_length/SIZEOF_polygon_data;
    assert(data_length);
	if(polygon_count*SIZEOF_polygon_data!=data_length) alert_user(fatalError, strERRORS, corruptedMap, 0x7369); // 'si'

	allocate_map_for_counts(polygon_count, side_count, endpoint_count, line_count);
}

/* Note that we assume the redundant data has already been recalculated... */
/*static void load_redundant_map_data(
	short *redundant_data,
	size_t count)
{
	if (redundant_data)
	{
		// assert(redundant_data && map_indexes);
		uint8 *Stream = (uint8 *)redundant_data;
		MapIndexList.resize(count);
		StreamToList(Stream,map_indexes,count);
		assert(count == static_cast<size_t>(static_cast<int16>(count)));
		assert(0 <= static_cast<int16>(count));
		dynamic_world->map_index_count= static_cast<int16>(count);
	}
	else
	{
		static bool have_been_warned= false;

#if !defined(ALPHA) && !defined(BETA)
		if(!have_been_warned && environment_preferences->non_bungie_warning)
		{
			// Only warn the gatherer.. *
			if(!game_is_networked || (game_is_networked && local_player_index==0))
			{
                                show_cursor();
				alert_user(infoError, strERRORS, warningExternalMapsFile, -1);
				// LP addition: makes the game look normal
				hide_cursor();
			}
			have_been_warned= true;
		}
#endif
		
		recalculate_redundant_map();
		precalculate_map_indexes();
	}
}
*/
void load_terminal_data(
	uint8 *data, 
	size_t length)
{
	/* I would really like it if I could get these into computer_interface.c statically */
	unpack_map_terminal_data(data,length);
}

static void scan_and_add_scenery(
	void)
{
	short ii;
	struct map_object *saved_object;
	
	saved_object= saved_objects;
	for(ii=0; ii<dynamic_world->initial_objects_count; ++ii)
	{
		if (saved_object->type==_saved_object)
		{
			struct object_location location;
			
			location.p= saved_object->location;
			location.flags= saved_object->flags;
			location.yaw= saved_object->facing;
			location.polygon_index= saved_object->polygon_index;
			new_scenery(&location, saved_object->index);
		}
		
		++saved_object;
	} 
}

struct save_game_data 
{
	uint32 tag;
	short unit_size;
	bool loaded_by_level;
};

#define NUMBER_OF_SAVE_ARRAYS (sizeof(save_data)/sizeof(struct save_game_data))
struct save_game_data save_data[]=
{
	{ ENDPOINT_DATA_TAG, SIZEOF_endpoint_data, true },
	{ LINE_TAG, SIZEOF_line_data, true },
	{ SIDE_TAG, SIZEOF_side_data, true },
	{ POLYGON_TAG, SIZEOF_polygon_data, true },
	{ LIGHTSOURCE_TAG, SIZEOF_light_data, true }, // false },
	{ ANNOTATION_TAG, SIZEOF_map_annotation, true },
	{ OBJECT_TAG, SIZEOF_map_object, true },
	{ MAP_INFO_TAG, SIZEOF_static_data, true },
	{ ITEM_PLACEMENT_STRUCTURE_TAG, SIZEOF_object_frequency_definition, true },
	{ MEDIA_TAG, SIZEOF_media_data, true }, // false },
	{ AMBIENT_SOUND_TAG, SIZEOF_ambient_sound_image_data, true },
	{ RANDOM_SOUND_TAG, SIZEOF_random_sound_image_data, true },
	{ TERMINAL_DATA_TAG, sizeof(byte), true },
	
	// LP addition: handling of physics models
	{ MONSTER_PHYSICS_TAG, SIZEOF_monster_definition, true},
	{ EFFECTS_PHYSICS_TAG, SIZEOF_effect_definition, true},
	{ PROJECTILE_PHYSICS_TAG, SIZEOF_projectile_definition, true},
	{ PHYSICS_PHYSICS_TAG, SIZEOF_physics_constants, true},
	{ WEAPONS_PHYSICS_TAG, SIZEOF_weapon_definition, true},

	{ MAP_INDEXES_TAG, sizeof(short), true }, // false },
	{ PLAYER_STRUCTURE_TAG, SIZEOF_player_data, true }, // false },
	{ DYNAMIC_STRUCTURE_TAG, SIZEOF_dynamic_data, true }, // false },
	{ OBJECT_STRUCTURE_TAG, SIZEOF_object_data, true }, // false },
	{ AUTOMAP_LINES, sizeof(byte), true }, // false },
	{ AUTOMAP_POLYGONS, sizeof(byte), true }, // false },
	{ MONSTERS_STRUCTURE_TAG, SIZEOF_monster_data, true }, // false },
	{ EFFECTS_STRUCTURE_TAG, SIZEOF_effect_data, true }, // false },
	{ PROJECTILES_STRUCTURE_TAG, SIZEOF_projectile_data, true }, // false },
	{ PLATFORM_STRUCTURE_TAG, SIZEOF_platform_data, true }, // false },
	{ WEAPON_STATE_TAG, SIZEOF_player_weapon_data, true }, // false },
	{ TERMINAL_STATE_TAG, SIZEOF_player_terminal_data, true }, // false }
};

/* the sizes are the sizes to save in the file, be aware! */
static uint8 *tag_to_global_array_and_size(
	uint32 tag, 
	size_t *size
	)
{
	uint8 *array= NULL;
	size_t unit_size = 0;
	size_t count = 0;
	unsigned index;
	
	for (index=0; index<NUMBER_OF_SAVE_ARRAYS; ++index)
	{
		if(save_data[index].tag==tag)
		{
			unit_size= save_data[index].unit_size;
			break;
		}
	}
	assert(index != NUMBER_OF_SAVE_ARRAYS);
	
	// LP: had fixed off-by-one error in medias saving,
	// and had added physics-model saving
	
	switch (tag)
	{
		case ENDPOINT_DATA_TAG:
			count= dynamic_world->endpoint_count;
			break;
		case LINE_TAG:
			count= dynamic_world->line_count;
			break;
		case SIDE_TAG:
			count= dynamic_world->side_count;
			break;
		case POLYGON_TAG:
			count= dynamic_world->polygon_count;
			break;
		case LIGHTSOURCE_TAG:
			count= dynamic_world->light_count;
			break;
		case ANNOTATION_TAG:
			count= dynamic_world->default_annotation_count;
			break;
		case OBJECT_TAG:
			count= dynamic_world->initial_objects_count;
			break;
		case MAP_INFO_TAG:
			count= 1;
			break;
		case PLAYER_STRUCTURE_TAG:
			count= dynamic_world->player_count;
			break;
		case DYNAMIC_STRUCTURE_TAG:
			count= 1;
			break;
		case OBJECT_STRUCTURE_TAG:
			count= dynamic_world->object_count;
			break;
		case MAP_INDEXES_TAG:
			count= dynamic_world->map_index_count;
			break;
		case AUTOMAP_LINES:
			count= (dynamic_world->line_count/8+((dynamic_world->line_count%8)?1:0)); 
			break;
		case AUTOMAP_POLYGONS:
			count= (dynamic_world->polygon_count/8+((dynamic_world->polygon_count%8)?1:0));
			break;
		case MONSTERS_STRUCTURE_TAG:
			count= dynamic_world->monster_count;
			break;
		case EFFECTS_STRUCTURE_TAG:
			count= dynamic_world->effect_count;
			break;
		case PROJECTILES_STRUCTURE_TAG:
			count= dynamic_world->projectile_count;
			break;
		case MEDIA_TAG:
			count= count_number_of_medias_used();
			break;
		case ITEM_PLACEMENT_STRUCTURE_TAG:
			count= 2*MAXIMUM_OBJECT_TYPES;
			break;
		case PLATFORM_STRUCTURE_TAG:
			count= dynamic_world->platform_count;
			break;
		case AMBIENT_SOUND_TAG:
			count= dynamic_world->ambient_sound_image_count;
			break;
		case RANDOM_SOUND_TAG:
			count= dynamic_world->random_sound_image_count;
			break;
		case TERMINAL_DATA_TAG:
			count= calculate_packed_terminal_data_length();
			break;
		case WEAPON_STATE_TAG:
			count= dynamic_world->player_count;
			break;
		case TERMINAL_STATE_TAG:
			count= dynamic_world->player_count;
			break;
		case MONSTER_PHYSICS_TAG:
			count= NUMBER_OF_MONSTER_TYPES;
			break;
		case EFFECTS_PHYSICS_TAG:
			count= NUMBER_OF_EFFECT_TYPES;
			break;
		case PROJECTILE_PHYSICS_TAG:
			count= NUMBER_OF_PROJECTILE_TYPES;
			break;
		case PHYSICS_PHYSICS_TAG:
			count= get_number_of_physics_models();
			break;
		case WEAPONS_PHYSICS_TAG:
			count= get_number_of_weapon_types();
			break;
		default:
			assert(false);
			break;
	}
	
	// Allocate a temporary packed-data chunk;
	// indicate if there is nothing to be written
	*size= count*unit_size;
	if (*size > 0)
		array = new byte[*size];
	else
		return NULL;
	
	// An OK-to-alter version of that array pointer
	uint8 *temp_array = array;
	
	switch (tag)
	{
		case ENDPOINT_DATA_TAG:
			pack_endpoint_data(array,map_endpoints,count);
			break;
		case LINE_TAG:
			pack_line_data(array,map_lines,count);
			break;
		case SIDE_TAG:
			pack_side_data(array,map_sides,count);
			break;
		case POLYGON_TAG:
			pack_polygon_data(array,map_polygons,count);
			break;
		case LIGHTSOURCE_TAG:
			pack_light_data(array,lights,count);
			break;
		case ANNOTATION_TAG:
			pack_map_annotation(array,map_annotations,count);
			break;
		case OBJECT_TAG:
			pack_map_object(array,saved_objects,count);
			break;
		case MAP_INFO_TAG:
			pack_static_data(array,static_world,count);
			break;
		case PLAYER_STRUCTURE_TAG:
			pack_player_data(array,players,count);
			break;
		case DYNAMIC_STRUCTURE_TAG:
			pack_dynamic_data(array,dynamic_world,count);
			break;
		case OBJECT_STRUCTURE_TAG:
			pack_object_data(array,objects,count);
			break;
		case MAP_INDEXES_TAG:
			ListToStream(temp_array,map_indexes,count); // E-Z packing here...
			break;
		case AUTOMAP_LINES:
			memcpy(array,automap_lines,*size);
			break;
		case AUTOMAP_POLYGONS:
			memcpy(array,automap_polygons,*size);
			break;
		case MONSTERS_STRUCTURE_TAG:
			pack_monster_data(array,monsters,count);
			break;
		case EFFECTS_STRUCTURE_TAG:
			pack_effect_data(array,effects,count);
			break;
		case PROJECTILES_STRUCTURE_TAG:
			pack_projectile_data(array,projectiles,count);
			break;
		case MEDIA_TAG:
			pack_media_data(array,medias,count);
			break;
		case ITEM_PLACEMENT_STRUCTURE_TAG:
			pack_object_frequency_definition(array,get_placement_info(),count);
			break;
		case PLATFORM_STRUCTURE_TAG:
			pack_platform_data(array,platforms,count);
			break;
		case AMBIENT_SOUND_TAG:
			pack_ambient_sound_image_data(array,ambient_sound_images,count);
			break;
		case RANDOM_SOUND_TAG:
			pack_random_sound_image_data(array,random_sound_images,count);
			break;
		case TERMINAL_DATA_TAG:
			pack_map_terminal_data(array,count);
			break;
		case WEAPON_STATE_TAG:
			pack_player_weapon_data(array,count);
			break;
		case TERMINAL_STATE_TAG:
			pack_player_terminal_data(array,count);
			break;
		case MONSTER_PHYSICS_TAG:
			pack_monster_definition(array,count);
			break;
		case EFFECTS_PHYSICS_TAG:
			pack_effect_definition(array,count);
			break;
		case PROJECTILE_PHYSICS_TAG:
			pack_projectile_definition(array,count);
			break;
		case PHYSICS_PHYSICS_TAG:
			pack_physics_constants(array,count);
			break;
		case WEAPONS_PHYSICS_TAG:
			pack_weapon_definition(array,count);
			break;
		default:
			assert(false);
			break;
	}
	
	return array;
}

/* Build the wad, with all the crap */
static struct wad_data *build_save_game_wad(
	struct wad_header *header, 
	long *length)
{
	struct wad_data *wad= NULL;
	uint8 *array_to_slam;
	size_t size;

	wad= create_empty_wad();
	if(wad)
	{
		recalculate_map_counts();
		for(unsigned loop= 0; loop<NUMBER_OF_SAVE_ARRAYS; ++loop)
		{
			/* If there is a conversion function, let it handle it */
			array_to_slam= tag_to_global_array_and_size(save_data[loop].tag, &size);
	
			/* Add it to the wad.. */
			if(size)
			{
				wad= append_data_to_wad(wad, save_data[loop].tag, array_to_slam, size, 0);
				delete []array_to_slam;
			}
		}
		if(wad) *length= calculate_wad_length(header, wad);
	}
	
	return wad;
}

/* Load and slam all of the arrays */
static void complete_restoring_level(
	struct wad_data *wad)
{
	/* Loading games needs this done. */
	//reset_action_queues();
}
