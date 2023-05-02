#ifndef __MOLE_H__
#define __MOLE_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new space bug at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* mole_new(Vector2D position);

Entity* mole_get();

void mole_draw(Entity* ent);

void mole_think(Entity* ent);

// update - updates player sprites accordingly
void mole_update(Entity* ent);

void mole_free(Entity* ent);

#endif