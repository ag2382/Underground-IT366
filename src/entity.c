#include "simple_logger.h"
#include "entity.h"
#include "level.h"

typedef struct 
{
	Uint32 entity_max;
	Entity* entity_list;
} EntityManager;


// @brief this variable only exists in this object file
static EntityManager entity_manager = {0};

void entity_manager_close()
{
    entity_free_all();
    if (entity_manager.entity_list) free(entity_manager.entity_list);
    slog("entity system closed");
}

void entity_manager_init(Uint32 max)
{
    if (max <= 0)
    {
        slog("cannot intialize entity system: zero entities specified!");
        return;
    }
    entity_manager.entity_list = gfc_allocate_array(sizeof(Entity), max);
    if (!entity_manager.entity_list)
    {
        slog("failed to initialize entity system!");
        return;
    }
    entity_manager.entity_max = max;
    atexit(entity_manager_close);
    slog("entity system initialized");
}

Entity* entity_new()
{
    int i;
    for (i = 0; i < entity_manager.entity_max; i++)
    {
        if (entity_manager.entity_list[i]._inuse) continue;
        entity_manager.entity_list[i]._inuse = 1;
        return &entity_manager.entity_list[i];
    }
    return NULL;
}

void entity_free_all()
{
    int i;
    for (i = 0; i < entity_manager.entity_max; i++)
    {
        if (!entity_manager.entity_list[i]._inuse) continue;
        entity_free(&entity_manager.entity_list[i]);
    }
}

void entity_free(Entity* ent)
{
    if (!ent)
    {
        slog("no entity provided");
        return;
    }
    if (ent->sprite)gf2d_sprite_free(ent->sprite);
    memset(ent, 0, sizeof(Entity));
}

void entity_draw(Entity* ent)
{
    if (!ent)return;
    if (ent->sprite)
    {
        gf2d_sprite_draw(
            ent->sprite,
            ent->position,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            (int)ent->frame);
    }
}

void entity_draw_all()
{
    int i;
    for (i = 0; i < entity_manager.entity_max; i++)
    {
        if (!entity_manager.entity_list[i]._inuse) continue;
        entity_draw(&entity_manager.entity_list[i]);
    }
}

void entity_update(Entity* ent)
{
    if (!ent)return;
    if (ent->update != NULL)        // entity is populated for update function to execute
    {
        ent->update(ent);
    }
    //if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)))
    //{
    //    //our next position is a hit, so don't move
    //    return;
    //}
}

void entity_update_all()
{
    int i;
    for (i = 0; i < entity_manager.entity_max; i++)
    {
        if (!entity_manager.entity_list[i]._inuse) continue;
        entity_update(&entity_manager.entity_list[i]);
    }
}

void entity_think(Entity* ent)
{
    if (!ent)return;
    if (ent->think)ent->think(ent);
}

void entity_think_all()
{
    int i;
    for (i = 0; i < entity_manager.entity_max; i++)
    {
        if (!entity_manager.entity_list[i]._inuse) continue;
        entity_think(&entity_manager.entity_list[i]);
    }
}

//Shape entity_get_shape_after_move(Entity* ent)
//{
//    Shape shape = { 0 };
//    if (!ent)return shape;
//    gfc_shape_copy(&shape, ent->shape);
//    gfc_shape_move(&shape, ent->position);
//    gfc_shape_move(&shape, ent->velocity);
//    return shape;
//}
//
//Shape entity_get_shape(Entity* ent)
//{
//    Shape shape = { 0 };
//    if (!ent)return shape;
//    gfc_shape_copy(&shape, ent->shape);
//    gfc_shape_move(&shape, ent->position);
//    return shape;
//}