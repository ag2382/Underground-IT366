#include "simple_logger.h"

#include "gfc_input.h"

#include "chest.h"
#include "player.h"
#include "level.h"
#include "boulder.h"
#include "camera.h"

static Entity* chest = NULL;
Vector2D chest_screen = { 0 };

Entity* chest_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->think = chest_think;
    ent->draw = chest_draw;
    ent->update = chest_update;
    ent->shape = gfc_shape_rect(0, 0, 64, 50);
    ent->drawOffset = vector2d(64, 74);
    vector2d_copy(ent->position, position);

    return ent;
}

void chest_draw(Entity* ent)
{
    Vector2D camera;
    camera = camera_get_position();
    if (!ent) return;

    vector2d_sub(chest_screen, ent->position, camera);

    gf2d_sprite_draw(
        gf2d_sprite_load_all(
            "images/interactables/chest_small.png",
            64,
            64,
            1,
            0),		// draw the sprite
        chest_screen,				// to its designated screen position
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (int)ent->frame);
}

void chest_think(Entity* ent)
{
    if (!ent) return;

    Vector2D a, b, c;
    a = player_get_position();
    b = ent->position;
    c = boulder_get_position();

    float dist = vector2d_magnitude_between(a, b);
    float b_dist = vector2d_magnitude_between(b, c);

    // * WHAT HAPPENS WHEN A PLAYER OPENS A CHEST * //
    if (dist < 24)
    {
        if (gfc_input_command_pressed("open_chest"))
        {
            slog("you have opened a chest");
            entity_free(ent);
        }
    }

    // WHAT HAPPENS IF A CHEST GETS CRUSHED BY A BOULDER * //
    if (b_dist < 64) entity_free(ent);

}

void chest_update(Entity* ent)
{
    if (!ent) return;
    ent->velocity.y += 0.1;

    if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) == 1)
    {
        // MAKE HIM STICK THE LANDING
        ent->velocity.y = 0;
    }
    vector2d_add(ent->position, ent->position, ent->velocity);
}

void chest_free(Entity* ent)
{
    if (!ent) return;
    chest = NULL;
}