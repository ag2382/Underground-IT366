#include "simple_logger.h"
#include "mole.h"
#include "level.h"
#include "boulder.h"
#include "camera.h"

static Entity* mole = NULL;
Vector2D mole_screen = { 0 };

Entity* mole_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->think = mole_think;
    ent->draw = mole_draw;
    ent->update = mole_update;
    ent->shape = gfc_shape_rect(0, 0, 64, 58);
    ent->drawOffset = vector2d(64, 74);
    ent->speed = 2.5;
    ent->team = 2;      // enemies are on their own team
    vector2d_copy(ent->position, position);

    return ent;
}

Entity* mole_get()
{
    return mole;
}

void mole_draw(Entity* ent)
{
    Vector2D camera;
    camera = camera_get_position();
    if (!ent) return;

    vector2d_sub(mole_screen, ent->position, camera);

    gf2d_sprite_draw(
        gf2d_sprite_load_all(
            "images/enemies/mole_small.png",
            64,
            64,
            1,
            0),		// draw the sprite
        mole_screen,				// to its designated screen position
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (int)ent->frame);
}

void mole_think(Entity* ent)
{
    //ent->position.x -= 1;

    // * WHAT IF MOLE GETS SQUASHED BY BOULDER * //
    Vector2D a, b;

    a = ent->position;
    b = boulder_get_position();

    float dist = vector2d_magnitude_between(a, b);
    if (dist < 64) entity_free(ent);
}

// update - updates player sprites accordingly
void mole_update(Entity* ent)
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

void mole_free(Entity* ent)
{
    if (!ent)return;
    mole = NULL;
}