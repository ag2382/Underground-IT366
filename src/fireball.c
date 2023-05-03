#include "simple_logger.h"

#include "gfc_audio.h"

#include "camera.h"
#include "level.h"
#include "fireball.h"

static Entity* fireball = NULL;
Vector2D fireball_screen = { 0 };
Sound* sfx;

Entity* fireball_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->think = fireball_think;
    ent->draw = fireball_draw;
    ent->update = fireball_update;
    ent->shape = gfc_shape_rect(0, 0, 20, 20);
    ent->drawOffset = vector2d(64, 74);
    ent->speed = 1;
    ent->team = 2;      // enemies are on their own team
    vector2d_copy(ent->position, position);
    sfx = gfc_sound_load("audio/sfx/smw_fireball.mp3", 1, 1);
    gfc_sound_play(sfx, 0, 0.25, 2, -1);

    fireball = ent;
    return ent;
}

Entity* fireball_get()
{
    return fireball;
}

void fireball_draw(Entity* ent)
{
    Vector2D camera;
    camera = camera_get_position();
    if (!ent) return;

    vector2d_sub(fireball_screen, ent->position, camera);

    gf2d_sprite_draw_image(gf2d_sprite_load_image("images/effects/fireball.png"), fireball_screen);

    ent->rect = ent->shape.s.r;
    ent->rect = ent->shape.s.r;
    ent->rect.x += ent->position.x;
    ent->rect.y += ent->position.y;

}

void fireball_think(Entity* ent)
{
    if (!ent) return;
    ent->position.x -= ent->speed;
    ent->position.y += (ent->speed / 2);
}

// update - updates player sprites accordingly
void fireball_update(Entity* ent)
{
    if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)))
    {
        entity_free(ent);
    }
}

void fireball_free(Entity* ent)
{
    if (!ent) return;
    fireball = NULL;
}