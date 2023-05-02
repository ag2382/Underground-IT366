#include "simple_logger.h"
#include "plant.h"
#include "level.h"
#include "boulder.h"
#include "camera.h"

static Entity* plant = NULL;
Vector2D plant_screen = { 0 };

Entity* plant_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->think = plant_think;
    ent->draw = plant_draw;
    ent->update = plant_update;
    ent->shape = gfc_shape_rect(0, 0, 64, 58);
    ent->drawOffset = vector2d(64, 74);
    ent->speed = 2.5;
    ent->team = 2;      // enemies are on their own team
    vector2d_copy(ent->position, position);

    return ent;
}

Entity* plant_get()
{
    return plant;
}

void plant_draw(Entity* ent)
{
    Vector2D camera;
    camera = camera_get_position();
    if (!ent) return;

    vector2d_sub(plant_screen, ent->position, camera);

    gf2d_sprite_draw(
        gf2d_sprite_load_all(
            "images/enemies/plant_small.png",
            64,
            64,
            1,
            0),		// draw the sprite
        plant_screen,				// to its designated screen position
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (int)ent->frame);
}

void plant_think(Entity* ent)
{
    // * WHAT IF PLANT GETS SQUASHED BY BOULDER * //
    Vector2D a, b;

    a = ent->position;
    b = boulder_get_position();

    float dist = vector2d_magnitude_between(a, b);
    if (dist < 64)
    {
        entity_free(ent);
    }
}

// update - updates player sprites accordingly
void plant_update(Entity* ent)
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

void plant_free(Entity* ent)
{
    if (!ent)return;
    plant = NULL;
}