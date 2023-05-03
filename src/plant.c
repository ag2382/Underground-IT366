#include "simple_logger.h"

#include "fireball.h"
#include "plant.h"

#include "level.h"
#include "boulder.h"
#include "camera.h"
#include "player.h"

static Entity* plant = NULL;
Vector2D plant_screen = { 0 };
int fireball_active;
float fireball_cooldown;

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

    gf2d_sprite_draw_image(gf2d_sprite_load_image("images/enemies/plant_small.png"), plant_screen);

    ent->rect = ent->shape.s.r;
    ent->rect = ent->shape.s.r;
    ent->rect.x += ent->position.x;
    ent->rect.y += ent->position.y;
}

void plant_think(Entity* ent)
{
    Vector2D a, b, p;
    a = ent->position;
    b = boulder_get_position();
    p = player_get_position();

    float p_dist = vector2d_magnitude_between(a, p);
    // * MAKE THE PLANT SHOOT FIRE * //
    if (p_dist < 500)
    {
        if (!fireball_active)
        {
            fireball_active = 1; 
            fireball_new(ent->position);
        }
        else
        {
            fireball_cooldown += 0.1;
            if (fireball_cooldown >= 30)
            {
                fireball_active = 0;
                fireball_cooldown = 0;
            }
        }
    }
    else
    {
        fireball_cooldown = 0;
    }

    // * WHAT IF PLANT GETS SQUASHED BY BOULDER * //
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