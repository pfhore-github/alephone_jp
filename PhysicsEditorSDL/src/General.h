#ifndef _PES_GENERAL_
#define _PES_GENERAL_

#include <guichan.hpp>
#include <guichan/sdl.hpp>
#include "SDL.h"
#include "HPLLib/HPLStringModifier.h"

static char* LIST_MONSTER_TYPES_PATH ="data/MonsterTypes.txt";
static char* LIST_COLLECTIONS_PATH ="data/Collections.txt";
static char* LIST_EFFECT_TYPES_PATH ="data/EffectTypes.txt";
static char* LIST_PROJECTILE_TYPES_PATH ="data/ProjectileTypes.txt";
static char* LIST_WEAPON_TYPES_PATH ="data/WeaponTypes.txt";
const int PAGE_X = 100, PAGE_Y = 10;
const int PAGE_WIDTH = 640 - PAGE_X - 20;
const int PAGE_HEIGHT = 480 - PAGE_Y - 20;
const int PANE_X = 10, PANE_Y = 10;
const int PANE_WIDTH = PAGE_WIDTH - 20;
const int PANE_HEIGHT = PANE_HEIGHT - 20;

#endif
