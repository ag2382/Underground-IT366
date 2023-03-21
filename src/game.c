#include <SDL.h>
#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gfc_input.h"
#include "level.h"
#include "gui.h"
#include "camera.h"
#include "entity.h"
#include "tools.h"
#include "player.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    Level* level;
    const Uint8 * keys;
    Sprite *sprite;
    Entity *player;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Color mouseColor = gfc_color8(255,100,255,200);
    SDL_Renderer* renderTarget = NULL;
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "UNDERGROUND",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    gfc_input_init("config/input.cfg");
    gui_setup_hud();
    entity_manager_init(1024);
    SDL_ShowCursor(SDL_DISABLE);

    level = level_load("config/test.level");
    //SDL_Rect camera = { 0, 0, level->tileLayer->frame_w, level->tileLayer->frame_h };
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_dirt.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    /*main game loop*/

    player = player_new(vector2d(650, 380));
    gfc_input_init("config/input.cfg");

    // graphics -> sprites -> entities -> things
    while(!done)
    {
        gfc_input_update();
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0) mf = 0;
        entity_think_all();
        entity_update_all();
        camera_world_snap();      // moves camera based on window resolution

        //camera.x = player->position.x - 600;
        //camera.y = player->position.y - 360;

        ////slog("camera dimensions: %i, %i", camera.x, camera.y);
        //slog("camera size: %i, %i", camera.w, camera.h);

        /*if (camera.x < 0) camera.x = 0;
        if (camera.y < 0) camera.y = 0;*/


        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
        //backgrounds drawn first
        gf2d_sprite_draw_image(sprite,vector2d(0,0));
        level_draw(level);
        entity_draw_all();
        player_draw(player);
        gui_draw_hud();
            
        //UI elements last
        gf2d_sprite_draw(
            mouse,
            vector2d(mx,my),
            NULL,
            NULL,
            NULL,
            NULL,
            &mouseColor,
            (int)mf);

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    FILE* file;
    file = fopen("stats.txt", "w");
    fprintf(file, "Spelunky walked at a speed of %f", player->speed);
    level_free(level);
    slog("---==== END ====---");
    return 0;
}

/*eol@eof*/
