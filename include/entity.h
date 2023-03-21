#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_text.h"
#include "gfc_types.h"
#include "gfc_shape.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

typedef struct Entity_S
{
    Bool    _inuse;
    Sprite* sprite;
    float   frame;
    float   rotation;
    float   speed;
    Vector2D drawOffset;

    Shape shape;

    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;

    struct Entity_S* parent;

    void (*think)(struct Entity_S* self);
    void (*update)(struct Entity_S* self);
    void (*draw)(struct Entity_S* self);
    void (*touch)(struct Entity_S* self);       // armor and gold can be touched by player entity
    void (*free_entity)(struct Entity_S* self); // cleanup of custom data if defined
    void* data;
    
    // FOR TOOLS
    int tool_active;
    float cooldown;

    // PICKAXE
    int pickaxe_active;

    // WHIP
    int whip_active;

    // ROPE
    int rope_active;

    // BOMB
    int bomb_active;
    float bomb_time;

    // SHOTGUN
    int ammo;
    int shotgun_active;

    // BOOMERANG
    int boomerang_active;       // only one boomerang can be active on screen
    int boomerang_return;       // flag used to begin return to player
    float boomerang_dist;       // current distance of boomerang traveled

    // SHIELD
    int shield_active;

    // FREEZE RAY
    int freeze_ray_active;

    // ROCKET BOOTS
    int rocket_boots_active;

    // DRILL GUN
    int drill_gun_active;

}Entity;

/**
 * @brief initialize the internal manager for the entity system
 * @note this will automatically queue up the close function for program exit
 * @param max this is the maximum number of supported entities at a given time
 */
void entity_manager_init(Uint32 max);

/**
 * @brief alloate an initialize a new entity
 * @return NULL if there are no entities left, an empty entity otherwise
 */
Entity* entity_new();

/**
 * @brief free a previously allocated entity
 * @param ent the entity to free
 */
void entity_free(Entity* ent);

/**
 * @brief free all allocated entities
 */
void entity_free_all();

/**
 * @brief draw all active entities if they have graphics
 */
void entity_draw_all();

/**
 * @brief update all active entities
 */
void entity_update_all();

/**
 * @brief call all the think functions for the entities, if they have one
 */
void entity_think_all();

//Shape entity_get_shape(Entity *ent);
//
//Shape entity_get_shape_after_move(Entity* ent);

#endif