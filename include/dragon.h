#ifndef __DRAGON_H__
#define __DRAGON_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new space bug at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* dragon_new(Vector2D position);

Entity* dragon_get();

void dragon_draw(Entity* ent);

void dragon_think(Entity* ent);

// update - updates player sprites accordingly
void dragon_update(Entity* ent);

void dragon_free(Entity* ent);

#endif