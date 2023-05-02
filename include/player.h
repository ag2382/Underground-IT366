#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

typedef struct
{
	// * miner's health * //
	int health;

	// * bombs * //
	int bomb_count;
	int cross_bomb_count;

	int rope_count;

	// * drill bullets * //
	int drill_count;
	int mega_drill_Ucount;

	// * miner's loot/currency * //
	int gold;

	// * UPGRADES * //
	
	// 0 - diamond pickaxe, 1 - cross bombs, 2 - rapid shotgun, 3 - gunboots, 4 - mega drill gun
	int upgrade_count[5];
	Bool isHit;
	Bool inShop;
	Vector2D newPos;		// player spawns after checkpoint

} PlayerData;

/*
* @brief spawn a new player entity
* @param position place the player here
* @return a pointer to the player entity
*/
Entity *player_new(Vector2D Position);

/*
* @brief get a pointer to the player entity
* @return a pointer to the player entity
*/
Entity *player_get();

/*
* @brief set the player position to the one specified
* @note risk of solid collision
* @param configure the new position for the player
*/
void player_set_position(Vector2D position);

Vector2D player_get_position();

#endif
