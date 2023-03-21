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

// PICKAXE STUFF
Entity* Pickaxe(Vector2D position);
void Pickaxe_Think(Entity* self);

// WHIP STUFF
Entity* Whip(Vector2D position);
void Whip_Think(Entity* self);

// ROPE STUFF
Entity* Rope(Vector2D position);
void Rope_Think(Entity* self);

// BOMB STUFF
Entity* Bomb(Vector2D position);
void Bomb_Think(Entity* self);
void Bomb_Draw(Entity* self);

// SHOTGUN STUFF
Entity* Shotgun(Vector2D position);
void Shotgun_Think(Entity* self);

// BOOMERANG STUFF
Entity* Boomerang(Vector2D position);
void Boomerang_Think(Entity* self);

// SHIELD STUFF
Entity* Shield(Vector2D position);

// FREEZE RAY STUFF
Entity* FreezeRay(Vector2D position);
void FreezeRay_Think(Entity* self);

// ROCKET BOOTS STUFF
Entity* RocketBoots(Vector2D position);

// DRILL GUN STUFF
Entity* DrillGun(Vector2D position);
void DrillGun_Think(Entity* self);


#endif