#ifndef __PLANT_H__
#define __PLANT_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new space bug at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* plant_new(Vector2D position);

Entity* plant_get();

void plant_draw(Entity* ent);

void plant_think(Entity* ent);

// update - updates player sprites accordingly
void plant_update(Entity* ent);

void plant_free(Entity* ent);

#endif