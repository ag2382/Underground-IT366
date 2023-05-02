#ifndef __GOLD_H__
#define __GOLD_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new space bug at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* gold_new(Vector2D position);

void gold_draw(Entity* ent);

void gold_think(Entity* ent, Entity* other);

// update - updates player sprites accordingly
void gold_update(Entity* ent);

void gold_free(Entity* ent);

#endif