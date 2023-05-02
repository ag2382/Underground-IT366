#ifndef __BAT_H__
#define __BAT_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new space bug at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* bat_new(Vector2D position);

Entity* bat_get();

void bat_draw(Entity* ent);

void bat_think(Entity* ent);

// update - updates player sprites accordingly
void bat_update(Entity* ent);

void bat_free(Entity* ent);

#endif