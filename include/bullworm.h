#ifndef __BULLWORM_H__
#define __BULLWORM_H__

#include "gfc_vector.h"
#include "entity.h"

Bool isHostile;
int moveTime;
int random;

typedef enum
{
    LEFT,
    RIGHT,
} direction;

// * BOSS 1 of Project Underground * //

Entity* bullworm_new(Vector2D position);

Entity* bullworm_get();

Vector2D bullworm_get_position();

void bullworm_think(Entity* ent);

void bullworm_move(Entity* ent);

void bullworm_draw(Entity* ent);

// update - updates player sprites accordingly
void bullworm_update(Entity* ent);

void bullworm_free(Entity* ent);
#endif