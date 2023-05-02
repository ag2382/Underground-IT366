#include "simple_logger.h"
#include "spider.h"
#include "level.h"
#include "boulder.h"
#include "camera.h"

static Entity* spider = NULL;
Vector2D spider_screen = { 0 };

Entity* spider_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->think = spider_think;
    ent->draw = spider_draw;
    ent->update = spider_update;
    ent->shape = gfc_shape_rect(0, 0, 64, 60);
    ent->drawOffset = vector2d(64, 74);
    ent->speed = 2.5;
    ent->team = 2;      // enemies are on their own team
    vector2d_copy(ent->position, position);

    return ent;
}

Entity* spider_get()
{
    return spider;
}

void spider_draw(Entity* ent)
{
    Vector2D camera;
    camera = camera_get_position();
    if (!ent) return;

    vector2d_sub(spider_screen, ent->position, camera);

    gf2d_sprite_draw(
        gf2d_sprite_load_all(
            "images/enemies/spider_small.png",
            64,
            64,
            1,
            0),		// draw the sprite
        spider_screen,				// to its designated screen position
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (int)ent->frame);

    Rect rect = ent->shape.s.r;
    rect.x += ent->position.x;
    rect.y += ent->position.y;

    // * player shape - used for testing purposes * //
    vector2d_add(rect, rect, camera_get_draw_offset());
    //gf2d_draw_rect(rect, gfc_color8(255, 255, 0, 255));
}

void spider_think(Entity* ent)
{
    // * WHAT IF SPIDER GETS SQUASHED BY BOULDER * //
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
void spider_update(Entity* ent)
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

void spider_free(Entity* ent)
{
    if (!ent)return;
    spider = NULL;
}