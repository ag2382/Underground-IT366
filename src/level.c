#include "simple_logger.h"
#include "simple_json.h"

#include "gfc_config.h"
#include "gfc_list.h"
#include "gf2d_graphics.h"
#include "gfc_shape.h"
#include "camera.h"
#include "level.h"

void level_build(Level* level);

static Level* activeLevel = NULL;

Level* level_get_active_level()
{
    return activeLevel;
}

void level_set_active_level(Level* level)
{
    activeLevel = level;
}

Level* level_load(const char* filename)
{
    int tile;
    int i, c;
    int j, d;
    int tileFPL;
    const char* str;
    SJson* json, * lj, * list, * row, * item;
    Level* level;
    if (!filename)return NULL;
    json = sj_load(filename);
    if (!json)return NULL;
    level = level_new();
    if (!level)
    {
        slog("failed to allocate new level");
        sj_free(json);
        return NULL;
    }
    lj = sj_object_get_value(json, "level");
    if (!lj)
    {
        slog("file %s missing level object", filename);
        sj_free(json);
        level_free(level);
        return NULL;
    }
    // Retrieve "level - name" from JSON
    str = sj_object_get_value_as_string(lj, "name");
    if (str)gfc_line_cpy(level->name, str);

    // Retrieve "level - tileSize" from JSON
    sj_value_as_vector2d(sj_object_get_value(lj, "tileSize"), &level->tileSize);

    // Retrieve "level - tileFPL" from JSON
    sj_object_get_value_as_int(lj, "tileFPL", &tileFPL);

    // Retrieve "level - tileSet" from JSON, then load all possible sprites for a tile
    str = sj_object_get_value_as_string(lj, "tileSet");
    if (str)
    {
        level->tileSet = gf2d_sprite_load_all(str, (Sint32)level->tileSize.x, (Sint32)level->tileSize.y, tileFPL, 1);
    }

    // Retrieve "level - tileMap" from JSON
    list = sj_object_get_value(lj, "tileMap");

    c = sj_array_get_count(list);                   // find # of rows
    row = sj_array_get_nth(list, 0);                // find # of columns
    d = sj_array_get_count(row);
    if ((c * d) == 0)
    {
        slog("corrupt row or column count for %s level", filename);
        level_free(level);
        sj_free(json);
        return NULL;
    }
    level->mapSize.x = d;           // level width
    level->mapSize.y = c;           // level height
    level->tileMap = gfc_allocate_array(sizeof(int), c * d);
    if (!level->tileMap)
    {
        slog("failed to allocate tileMap for level %s", filename);
        level_free(level);
        sj_free(json);
        return NULL;
    }
    for (i = 0; i < c; i++)// i is row
    {
        row = sj_array_get_nth(list, i);
        if (!row)continue;
        d = sj_array_get_count(row);
        for (j = 0; j < d; j++)// j is column
        {
            item = sj_array_get_nth(row, j);
            if (!item)continue;
            tile = 0;//default
            sj_get_integer_value(item, &tile);
            level->tileMap[(i * (int)level->mapSize.x) + j] = tile;
        }
    }
    sj_free(json);
    level_build(level);
    return level;
}

int level_shape_clip(Level* level, Shape shape)
{
    int i, c;
    Shape* clip;
    if (!level)return 0;
    c = gfc_list_get_count(level->clips);
    for (i = 0; i < c; i++)
    {
        clip = gfc_list_get_nth(level->clips, i);
        if (!clip)continue;
        if (gfc_shape_overlap(*clip, shape))return 1;
    }
    return 0;
}

void level_build_clip_space(Level* level)
{
    Shape* shape;
    int i, j;
    if (!level)return;
    for (j = 0; j < level->mapSize.y; j++)//j is row
    {
        for (i = 0; i < level->mapSize.x; i++)// i is column
        {
            if (level->tileMap[(j * (int)level->mapSize.x) + i] <= 0)continue;//skip zero
            shape = gfc_allocate_array(sizeof(Shape), 1);
            if (!shape)continue;
            *shape = gfc_shape_rect(i * level->tileSize.x, j * level->tileSize.y, level->tileSize.x, level->tileSize.y);
            gfc_list_append(level->clips, shape);
        }
    }
}

void level_remove_clip_space(Level* level)
{
    
}

// * create the surface of the level as you go * //
void level_create_surface(Level* level)
{
    if (!level) return;
    int i, j;
    // if there is a default surface, free it
    if (level->tileLayer->surface)SDL_FreeSurface(level->tileLayer->surface);
    //create a surface the size we need it
    level->tileLayer->surface = gf2d_graphics_create_surface(level->tileSize.x * level->mapSize.x, level->tileSize.y * level->mapSize.y);
    if (!level->tileLayer->surface)
    {
        slog("failed to create tileLayer surface");
        return;
    }
    slog("created surface of size (%i,%i)", level->tileLayer->surface->w, level->tileLayer->surface->h);
    //make sure the surface is compatible with our graphics settings
    level->tileLayer->surface = gf2d_graphics_screen_convert(&level->tileLayer->surface);
    if (!level->tileLayer->surface)
    {
        slog("failed to create surface for tileLayer");
        return;
    }
    //draw the tile sprite to the surface
    for (j = 0; j < level->mapSize.y; j++)//j is row
    {
        for (i = 0; i < level->mapSize.x; i++)// i is column
        {
            if (level->tileMap[(j * (int)level->mapSize.x) + i] <= 0)continue; //skip zero
            gf2d_sprite_draw_to_surface(
                level->tileSet,
                vector2d(i * level->tileSize.x, j * level->tileSize.y),
                NULL,
                NULL,
                level->tileMap[(j * (int)level->mapSize.x) + i] - 1,
                level->tileLayer->surface);
        }
    }
}

void level_build(Level* level)
{
    if (!level)return;
    if (level->tileLayer)gf2d_sprite_free(level->tileLayer);
    level->tileLayer = gf2d_sprite_new();
    if (!level->tileLayer)
    {
        slog("failed to create sprite for tileLayer");
        return;
    }

    // * create the level's surface, then convert it to a texture * //
    level_create_surface(level);
    level->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), level->tileLayer->surface);
    SDL_SetTextureBlendMode(level->tileLayer->texture, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(level->tileLayer->texture,
        NULL,
        level->tileLayer->surface->pixels,
        level->tileLayer->surface->pitch);
    level->tileLayer->frame_w = level->tileLayer->surface->w;
    level->tileLayer->frame_h = level->tileLayer->surface->h;
    level->tileLayer->frames_per_line = 1;
    camera_set_world_size(vector2d(level->tileLayer->frame_w, level->tileLayer->frame_h));
    level_build_clip_space(level);
}

void level_draw(Level* level)
{
    if (!level)return;
    if (!level->tileLayer)return;
    gf2d_sprite_draw_image(level->tileLayer, camera_get_draw_offset());
}

Level* level_new()
{
    Level* level;
    level = gfc_allocate_array(sizeof(Level), 1);
    level->clips = gfc_list_new();
    return level;
}
void level_free(Level* level)
{
    if (!level)return;
    if (level->tileSet)gf2d_sprite_free(level->tileSet);
    if (level->tileLayer)gf2d_sprite_free(level->tileLayer);
    if (level->tileMap)free(level->tileMap);
    gfc_list_foreach(level->clips, free);
    gfc_list_delete(level->clips);
    free(level);
}

Space* level_get_space()
{
    return activeLevel->space;
}

void level_add_entity(Level* level, Entity* entity)
{
    if ((!level) || (!entity))return;
    gf2d_space_add_body(level->space, &entity->body);
}

int get_level_tile(Level* level, int x, int y) {
    if (x >= 0 && x < level->mapSize.x * level->tileSize.x && y >= 0 && y < level->mapSize.y * level->tileSize.y)
    {
        return level->tileMap[(y * (int)level->mapSize.x) + x];
    }
}

// update the level as you dig through the world
void level_removeTile(Level* level, int x, int y)
{
    if (!level) return;

    // Retrieve tile registered from pickaxe contact //
    if (get_level_tile(level, x, y) > 0)
    {
        slog("terrain detected");
        level->tileMap[(y * (int)level->mapSize.x) + x] = 0;    // change value to 0 in JSON file->NO TILE
    }
    // Redraw with the loose tile in mind
    level_create_surface(level);

    //level->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), level->tileLayer->surface);
    SDL_SetTextureBlendMode(level->tileLayer->texture, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(level->tileLayer->texture,
        NULL,
        level->tileLayer->surface->pixels,
        level->tileLayer->surface->pitch);

    // * REBUILD CLIP SPACE with detected tile in mind * //
    // that will be a later problem
}

/*eol@eof*/