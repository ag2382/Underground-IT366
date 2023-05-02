#ifndef __SPIKES_H__
#define __SPIKES_H__

#include "gfc_vector.h"
#include "entity.h"

Bool touched;

/**
 * @brief spawn a new space bug at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* spikes_new(Vector2D position);

void spikes_think(Entity* ent);

void spikes_draw(Entity* ent);

// update - updates player sprites accordingly
void spikes_update(Entity* ent);

#endif