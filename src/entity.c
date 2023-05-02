#include "simple_logger.h"
#include "simple_json.h"

#include "gfc_config.h"
#include "gf2d_collision.h"

#include "camera.h"
#include "gf2d_draw.h"

#include "player.h"
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
    if (!ent) return;

    if (ent->draw)      // if a defined draw function exists for another entity (PLAYER for example)
    {
        ent->draw(ent);
    }
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
    if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)))
    {
        //our next position is a hit, so don't move
        //slog("clipping");
        ent->clipping = true;
        return;
    }
    ent->clipping = false;
    //slog("not clipping");
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
    if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)))
    {
        // slog("clip");
        //our next position is a hit, so don't move
        return;
    }
}

SJson* entity_get_def_by_name(const char* name)
{
    //    int i,c;
    if (!name)return NULL;

    return NULL;
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

Shape entity_get_shape_after_move(Entity* ent)
{
    Shape shape = { 0 };
    if (!ent)return shape;
    gfc_shape_copy(&shape, ent->shape);
    gfc_shape_move(&shape, ent->position);
    gfc_shape_move(&shape, ent->velocity);
    return shape;
}

Shape entity_get_shape(Entity* ent)
{
    Shape shape = { 0 };
    if (!ent)return shape;
    gfc_shape_copy(&shape, ent->shape);
    gfc_shape_move(&shape, ent->position);
    return shape;
}

int entity_wall_check(Entity* self, Vector2D dir)
{
    Shape s;
    int i, count;
    int hit = 0;
    Collision* c;
    List* collisionList;
    CollisionFilter filter = {
        self->body.worldclip,
        self->body.cliplayer,
        0,
        0,
        &self->body
    };

    if (!self)return 0;
    s = gf2d_body_to_shape(&self->body);
    if (s.type == ST_RECT)
    {
        if (dir.x < 0)
        {
            s.s.r.w = -dir.x + 1;
        }
        else if (dir.x > 0)
        {
            s.s.r.x += (s.s.r.w + dir.x) - 1;
            s.s.r.w = dir.x + 1;
        }
        if (dir.y < 0)
        {
            s.s.r.h = -dir.y + 1;
        }
        else if (dir.y > 0)
        {
            s.s.r.y += (s.s.r.h - dir.y) - 1;
            s.s.r.h = dir.y + 1;
        }
    }
    gfc_shape_move(&s, dir);

    collisionList = gf2d_collision_check_space_shape(level_get_space(), s, filter);
    if (collisionList != NULL)
    {
        count = gfc_list_get_count(collisionList);
        for (i = 0; i < count; i++)
        {
            c = (Collision*)gfc_list_get_nth(collisionList, i);
            if (!c)continue;
            if ((c->body) && (c->body == self->body.ignore))continue;
            if ((dir.y <= 0) && (c->body) && (c->body->cliplayer & PLATFORM_LAYER))
            {
                continue;
            }
            hit = 1;
            /*if (__DebugMode)
            {
                gf2d_draw_shape(c->shape, gfc_color(255, 255, 0, 255), gf2d_camera_get_offset());
            }*/
        }
        gf2d_collision_list_free(collisionList);
        return hit;
    }
    return 0;
}