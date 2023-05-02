#include "simple_logger.h"

#include "gfc_audio.h"
#include "gf2d_collision.h"

#include "gold.h"
#include "camera.h"
#include "level.h"
#include "boulder.h"
#include "player.h"

static Entity *gold = NULL;

Vector2D gold_screen = { 0 };

Entity* gold_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->think = gold_think;
    ent->draw = gold_draw;
    ent->free_entity = gold_free;
    ent->shape = gfc_shape_rect(4, 16, 56, 32);
    ent->drawOffset = vector2d(64, 74);
    vector2d_copy(ent->position, position);

    return ent;
}

Entity* gold_get()
{
    return gold;
}

void gold_draw(Entity* ent)
{
    Vector2D camera;
    camera = camera_get_position();
    if (!ent) return;

    vector2d_sub(gold_screen, ent->position, camera);

    gf2d_sprite_draw(
        gf2d_sprite_load_image("images/interactables/gold.png"),		// draw the sprite
        gold_screen,				// to its designated screen position
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (int)ent->frame);
}

void gold_think(Entity* ent)
{
    if (!ent) return;

    Vector2D a, b, c;
    a = player_get_position();
    b = ent->position;
    c = boulder_get_position();

    Entity* p = player_get();
    PlayerData* data = p->data;

    float dist, b_dist;
    dist = vector2d_magnitude_between(a, b);
    b_dist = vector2d_magnitude_between(b, c);
    
    if (dist < 16)
    {
        data->gold += 100;
        entity_free(ent);
        slog("player has %i gold", data->gold);
        Sound* sfx = gfc_sound_load("audio/sfx/sm64_coin.wav", 1, 1);
        gfc_sound_play(sfx, 0, 0.6, 2, -1);
    }
    if (b_dist < 64) entity_free(ent);
    
}

void gold_free(Entity* ent)
{
    if (!ent) 
    {
        slog("nothing");
        return;
    }
    gold = NULL;
}