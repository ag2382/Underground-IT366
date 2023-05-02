#include "simple_logger.h"
#include "boulder.h"
#include "level.h"
#include "player.h"
#include "camera.h"

static Entity* boulder = NULL;
Vector2D boulder_screen = { 0 };

int ground;

Entity* boulder_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->draw = boulder_draw;
    ent->update = boulder_update;
    ent->shape = gfc_shape_rect(0, 0, 128, 128);
    ent->drawOffset = vector2d(64, 74);
    ent->speed = 2.5;
    vector2d_copy(ent->position, position);
    boulder = ent;

    return ent;
}

Entity* boulder_get()
{
    return boulder;
}

Vector2D boulder_get_position()
{
    Vector2D v = { 0 };
    if (!boulder) return v;
    return boulder->position;
}

void boulder_draw(Entity* ent)
{
    Vector2D camera;
    camera = camera_get_position();
    if (!ent) return;

    vector2d_sub(boulder_screen, ent->position, camera);

    gf2d_sprite_draw(
        gf2d_sprite_load_all(
            "images/interactables/boulder_small.png",
            128,
            128,
            1,
            0),		                // draw the sprite
        boulder_screen,				// to its designated screen position
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (int)ent->frame);
}

void boulder_update(Entity* ent)
{
    if (!ent) return;
    ent->velocity.y += 0.05;
    vector2d_add(ent->position, ent->position, ent->velocity);

}

void boulder_free(Entity* ent)
{
    if (!ent) return;
    boulder = NULL;
}