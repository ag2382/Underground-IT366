#ifndef __DRAGONFLY_H__
#define __DRAGONFLY_H__

#include "gfc_vector.h"
#include "entity.h"

//Bool isHostile;
//int moveTime;
//int random;
//
typedef enum
{
    FLY_IDLE,
    FLY_MOVE,
} State;

// * BOSS 3 of Project Underground * //

Entity* dragonfly_new(Vector2D position);

Entity* dragonfly_get();

Vector2D dragonfly_get_position();

void dragonfly_think(Entity* ent);

//void dragonfly_move(Entity* ent);

void dragonfly_draw(Entity* ent);

// update - updates player sprites accordingly
void dragonfly_update(Entity* ent);

void dragonfly_free(Entity* ent);
#endif