#ifndef __ARMADILLO_H__
#define __ARMADILLO_H__

#include "gfc_vector.h"
#include "entity.h"

// * BOSS 2 of Project Underground * //

Bool isHostile;
static float idle_TimeSet[4] = { 100, 200, 300, 400 };
float idleTime;
int stateTime;

typedef enum
{
    IDLE,
    ROLL_LEFT,
    ROLL_RIGHT,
} action;

int random;

Entity* armadillo_new(Vector2D position);

Entity* armadillo_get();

Vector2D armadillo_get_position();

void armadillo_think(Entity* ent);

void armadillo_takeAction(Entity* ent);

void armadillo_draw(Entity* ent);

// update - updates player sprites accordingly
void armadillo_update(Entity* ent);

void armadillo_free(Entity* ent);
#endif