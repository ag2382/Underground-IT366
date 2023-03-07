#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <stdbool.h>
#include "gfc_types.h"
#include "gf2d_sprite.h"
#include "entity.h"

/*
* NOTES ABOUT TOOLS:

* array of 3 disposable tools (NOT INCLUDING BOMBS --> mapped to diffrent key)
* only player can pick them up, NOT ENEMIES
* max of 5 in player inventory - pickaxe, whip, rope, bomb, item
*/

// @brief flags used to sort default tools and additional tools obtained in a run
#define FL_DEFAULT		1
#define FL_OBTAIN		2

// @brief tool definitions
#define TOOL_PICKAXE		1
#define TOOL_WHIP			2
#define TOOL_ROPE			3
#define TOOL_BOMB			4
#define TOOL_SHOTGUN		5
#define TOOL_BOOMERANG		6
#define TOOL_SHIELD			7
#define TOOL_FREEZERAY		8
#define TOOL_ROCKETBOOTS	9
#define TOOL_DRILLGUN		10

/**
 * @purpose the tool information is what is loaded from disk to describe how the tool entity
 * is loaded
 */
typedef struct Tool_S {

	TextLine name;			// name of the tool
	void (*tool_think)		(Entity* ent, struct Tool_S* tool);
	int count;				// for limited ammunition (ropes, bombs, shotgun ammo, drillgun ammo)
	int flags;
} Tool;

extern Tool tool_list[];

//void tool_draw (Entity* self);
//void tool_think (Entity *self);
//void tool_update (Entity *self);

// certain "tools" can be touched by player entity

void Tool_Pickaxe(Entity* self);
void Tool_Whip(Entity* self);
void Tool_Rope(Entity* self);

// BOMB STUFF
Entity* Bomb(Vector2D position);
void Tool_Bomb_Get(Entity* self);
void Tool_Bomb_Cooldown(Entity* self);

void Tool_Shotgun(Entity* self);
void Tool_Boomerang(Entity* self);
void Tool_Shield(Entity* self);
void Tool_FreezeRay(Entity* self);
void Tool_RocketBoots(Entity* self);
void Tool_DrillGun(Entity* self);


#endif