#ifndef __BOULDER_H__
#define __BOULDER_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new space bug at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* boulder_new(Vector2D position);

Entity* boulder_get();

Vector2D boulder_get_position();

void boulder_draw(Entity* ent);

void boulder_think(Entity* ent, Entity* other);

// update - updates player sprites accordingly
void boulder_update(Entity* ent);

void boulder_free(Entity* ent);

#endif