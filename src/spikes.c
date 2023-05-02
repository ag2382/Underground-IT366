#include "simple_logger.h"
#include "spikes.h"
#include "camera.h"
#include "level.h"
#include "player.h"

Vector2D spikes_screen = { 0 };

Entity* spikes_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->think = spikes_think;
    ent->draw = spikes_draw;
    //ent->update = spikes_update;
    ent->shape = gfc_shape_rect(0, 0, 128, 128);
    ent->drawOffset = vector2d(64, 74);
    ent->speed = 2.5;
    vector2d_copy(ent->position, position);

    return ent;
}

void spikes_draw(Entity* ent)
{
    Vector2D camera;
    camera = camera_get_position();
    if (!ent) return;

    vector2d_sub(spikes_screen, ent->position, camera);

    gf2d_sprite_draw(
        gf2d_sprite_load_all(
            "images/interactables/spikes_small.png",
            64,
            64,
            1,
            0),		// draw the sprite
        spikes_screen,				// to its designated screen position
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (int)ent->frame);
}

void spikes_think(Entity* ent)
{
    if (!ent) return;

    Vector2D a, b;
    a = player_get_position();
    b = ent->position;

    Entity* p = player_get();
    PlayerData* data = p->data;

    float dist = vector2d_magnitude_between(a, b);

    // * WHAT HAPPENS WHEN A PLAYER TOUCHES SPIKES * //
    if (dist < 16 && !touched)
    {
        touched = true;
        data->health = 0;
        slog("player died from touching spikes. health: %i", data->health);
    }
}