#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_draw.h"

#include "dragon.h"
#include "player.h"
#include "level.h"
#include "tools.h"
#include "boulder.h"
#include "camera.h"

static Entity* dragon = NULL;
Vector2D dragon_screen = { 0 };

Entity* dragon_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->think = dragon_think;
    ent->draw = dragon_draw;
    ent->update = dragon_update;
    ent->shape = gfc_shape_rect(15, 0, 48, 59);
    ent->drawOffset = vector2d(64, 74);
    ent->speed = 2.5;
    ent->team = 2;      // enemies are on their own team
    vector2d_copy(ent->position, position);

    return ent;
}

Entity* dragon_get()
{
    return dragon;
}

void dragon_draw(Entity* ent)
{
    Vector2D camera;
    camera = camera_get_position();
    if (!ent) return;
    vector2d_sub(dragon_screen, ent->position, camera);

    gf2d_sprite_draw_image(gf2d_sprite_load_image("images/enemies/dragon_small.png"), dragon_screen);

    ent->rect = ent->shape.s.r;
    ent->rect.x += ent->position.x;
    ent->rect.y += ent->position.y;

    // * player shape - used for testing purposes * //
    vector2d_add(ent->rect, ent->rect, camera_get_draw_offset());
    gf2d_draw_rect(ent->rect, gfc_color8(255, 255, 255, 255));
}

void dragon_think(Entity* ent)
{
    Vector2D a, b, p;

    a = ent->position;
    b = boulder_get_position();
    p = player_get_position();

    float p_dist = vector2d_magnitude_between(a, p);
    /*if (p_dist < 300)
    {
        slog("shooting fire");
    }*/

    // * WHAT IF DRAGON GETS SQUASHED BY BOULDER * //
    float dist = vector2d_magnitude_between(a, b);
    if (dist < 64)
    {
        entity_free(ent);
    }
}

void dragon_update(Entity* ent)
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

void dragon_free(Entity* ent)
{
    if (!ent)return;
    dragon = NULL;
}