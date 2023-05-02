#ifndef __TOTEM_H__
#define __TOTEM_H__

#include "gfc_vector.h"
#include "entity.h"

Bool check;

/**
 * @brief spawn a new space bug at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* totem_new(Vector2D position);

Entity* totem_get();

Vector2D totem_get_position();

void totem_draw(Entity* ent);

void totem_think(Entity* ent);

// update - updates player sprites accordingly
void totem_update(Entity* ent);

void totem_free(Entity *ent);

#endif