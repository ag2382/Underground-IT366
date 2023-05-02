#include "simple_logger.h"

#include "gfc_shape.h"
#include "gf2d_draw.h"

#include "totem.h"
#include "level.h"
#include "player.h"
#include "camera.h"

static Entity *totem = NULL;
Vector2D totem_screen = { 0 };

Entity* totem_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->think = totem_think;
    ent->draw = totem_draw;
    //ent->update = totem_update;
    ent->shape = gfc_shape_circle(0, 0, 10); // shape position becomes offset from entity position, in this case zero
    ent->shape = gfc_shape_rect(0, 0, 64, 64);
    ent->drawOffset = vector2d(64, 74);
    ent->speed = 2.5;
    vector2d_copy(ent->position, position);

    return ent;
}

Entity* totem_get()
{
    return totem;
}

Vector2D totem_get_position()
{
    Vector2D v = { 0 };
    if (!totem) return v;
    return totem->position;
}

void totem_draw(Entity* ent)
{
    Vector2D camera;
    camera = camera_get_position();
    if (!ent) return;
    vector2d_sub(totem_screen, ent->position, camera);

    gf2d_sprite_draw(
        gf2d_sprite_load_all(
            "images/interactables/totem_small.png",
            64,
            64,
            1,
            0),	// draw the sprite
        totem_screen,				// to its designated screen position
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (int)ent->frame);
}

void totem_think(Entity* ent)
{
    if (!ent) return;

    Vector2D a, b;
    a = player_get_position();
    b = ent->position;

    Entity* p = player_get();
    PlayerData* data = p->data;

    float dist = vector2d_magnitude_between(a, b);
    if (dist < 16 && !check)
    {
        check = true;
        slog("checkpoint reached");

    }
}

void totem_update(Entity* ent)
{

}

void totem_free(Entity* ent)
{
    if (!ent) return;
    totem = NULL;
}