#include "simple_logger.h"
#include "camera.h"
#include "level.h"
#include "boulder.h"
#include "bat.h"

static Entity* bat = NULL;
Vector2D bat_screen = { 0 };

Entity* bat_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    
    ent->think = bat_think;
    ent->draw = bat_draw;
    ent->update = bat_update;
    ent->shape = gfc_shape_rect(0, 0, 64, 64);// shape position becomes offset from entity position, in this case zero
    ent->drawOffset = vector2d(64, 74);
    ent->speed = 2.5;
    ent->team = 2;      // enemies are on their own team
    vector2d_copy(ent->position, position);

    return ent;
}

Entity* bat_get()
{
    return bat;
}

void bat_draw(Entity* ent)
{
	Vector2D camera;
    camera = camera_get_position();
	if (!ent) return;

    vector2d_sub(bat_screen, ent->position, camera);

    gf2d_sprite_draw(
        gf2d_sprite_load_all(
            "images/enemies/bat_small.png",
            64,
            64,
            5,
            0),		// draw the sprite
        bat_screen,				// to its designated screen position
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (int)ent->frame);
}

void bat_think(Entity* ent)
{
    if (!ent) return;
    ent->position.x += 1;

    // * WHAT IF BAT GETS SQUASHED BY BOULDER * //
    Vector2D a, b;

    a = ent->position;
    b = boulder_get_position();

    float dist = vector2d_magnitude_between(a, b);
    if (dist < 112)
    {
        entity_free(ent);
    }

}

void bat_update(Entity* ent)
{
    if (!ent) return;
    ent->frame += 0.1;
    if (ent->frame >= 5) ent->frame = 0;
}

void bat_free(Entity* ent)
{
    if (!ent)return;
    bat = NULL;
}