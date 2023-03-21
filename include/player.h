#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

typedef enum {
	PLAYER_WalkLeft,
	PLAYER_WalkRight,
	PLAYER_JumpRise,
	PLAYER_JumpFall,
}PlayerActions;

typedef enum {
	PLAYER_Idle,
	PLAYER_Walk,
	Player_Jump,
	Player_Death
}PlayerState;

typedef struct
{
	float jump;
	float max_jump;		// how high will the player jump (after pressing X ~ PS4 controller)
	int health;			// player's starting + max health
	int jump_rise, jump_fall, jump_input;
	int bomb_count, rope_count;
	int score;
	//enum PlayerState st;
}PlayerData;

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

// draw - obtain sprites
void player_draw(Entity* self);

// think - handles player actions
void player_think(Entity* self);

// update - updates player sprites accordingly
void player_update(Entity* self);

void player_touch(Entity* self);

#endif
