#ifndef __FIREBALL_H__
#define __FIREBALL_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new space bug at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* fireball_new(Vector2D position);

Entity* fireball_get();

void fireball_draw(Entity* ent);

void fireball_think(Entity* ent);

// update - updates player sprites accordingly
void fireball_update(Entity* ent);

void fireball_free(Entity* ent);

#endif