#ifndef __SHOP_H__
#define __SHOP_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new space bug at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* shop_new(Vector2D position);

Entity* shop_get();

Vector2D shop_get_position();

void shop_think(Entity* ent);

void shop_draw(Entity* ent);

// update - updates player sprites accordingly
void shop_update(Entity* ent);

void shop_free(Entity* ent);

#endif