#ifndef __CHEST_H__
#define __CHEST_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new space bug at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* chest_new(Vector2D position);

void chest_think(Entity* ent);

void chest_draw(Entity* ent);

// update - updates player sprites accordingly
void chest_update(Entity* ent);

#endif