#include <SDL.h>
#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gfc_input.h"
#include "gfc_audio.h"

#include "level.h"
#include "gui.h"
#include "camera.h"

#include "entity.h"
#include "player.h"
#include "spider.h"
#include "plant.h"
#include "mole.h"
#include "dragon.h"
#include "bat.h"

#include "bullworm.h"
#include "armadillo.h"
#include "dragonfly.h"

#include "shop.h"

#include "chest.h"
#include "boulder.h"
#include "spikes.h"
#include "totem.h"
#include "gold.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    Level* level;
    const Uint8 * keys;
    Sprite *sprite, *title, *pause;
    Sound* sound;
    Entity *player, *enemy, *boss, *interactable, *shop;

    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Color mouseColor = gfc_color8(255,100,255,200);
    SDL_Renderer* renderTarget = NULL;
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "PROJECT UNDERGROUND",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gfc_audio_init(1024, 16, 5, SDL_MIX_MAXVOLUME, 1, 1);
    gf2d_sprite_init(1024);
    gfc_input_init("config/input.json");
    gui_setup_hud();
    entity_manager_init(1024);
    SDL_ShowCursor(SDL_DISABLE);

    level = level_load("config/test.level");
    level_set_active_level(level);
    //SDL_Rect camera = { 0, 0, level->tileLayer->frame_w, level->tileLayer->frame_h };
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_cavern.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    /*main game loop*/

    // * stuff for title screen * //
    title = gf2d_sprite_load_image("images/backgrounds/title_screen.png");
    int onTitle = 1;
    int inGame = 0;
    int inPause = 0;
    pause = gf2d_sprite_load_image("images/pause.png");

    // * players and enemies are positioned relative to world * //
    enemy = spider_new(vector2d(850, 600));
    enemy = plant_new(vector2d(1700, 275));
    enemy = mole_new(vector2d(1824, 1024));
    enemy = dragon_new(vector2d(1300, 344));
    enemy = bat_new(vector2d(400, 150));
    enemy = bat_new(vector2d(1380, 1200));

    // * world interactables * //
    interactable = chest_new(vector2d(930, 1156));
    interactable = boulder_new(vector2d(1408, 896));

    interactable = spikes_new(vector2d(512, 1216));
    interactable = spikes_new(vector2d(576, 1216));
    interactable = spikes_new(vector2d(640, 1216));

    interactable = totem_new(vector2d(224, 1216));

    interactable = gold_new(vector2d(350, 1216));
    interactable = gold_new(vector2d(1000, 450));

    shop = shop_new(vector2d(2150, 1400));

    boss = bullworm_new(vector2d(1975, 1822));
    boss = armadillo_new(vector2d(1600, 2434));
    boss = dragonfly_new(vector2d(1875, 3010));

    // * DRAW THE PLAYER LAST * //
     player = player_new(vector2d(1250, 1175));
    //player = player_new(vector2d(900, 200));

    // * Music system is a WIP * //
    sound = gfc_sound_load("audio/bgm/20. Mines A.mp3", 1, 1);

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

        // * handle title screen * //
        if (onTitle)
        {
            gf2d_sprite_draw_image(title, vector2d(0, 0));
            if (keys[SDL_SCANCODE_RETURN])
            {
                onTitle = 0;
                gfc_sound_play(sound, 999, 0.15, -1, -1);
            }
        }

        // * take player to game after user input * //
        else inGame = 1;

        if (inGame)
        {
            gf2d_graphics_clear_screen();// clears drawing buffers
            // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite, vector2d(0, 0));
            level_draw(level_get_active_level());
            entity_draw_all();
            gui_draw_hud();

            // * only pause in the game * //
            if (gfc_input_command_pressed("esc")) inPause = 1;

            // * freeze all entities when paused * //
            if (inPause)
            {
                Mix_Pause(-1);
                gf2d_sprite_draw_image(pause, vector2d(0, 0));
                if ((keys)[SDL_SCANCODE_Y])
                {
                    onTitle = 1;
                    inGame = 0;
                    inPause = 0;
                }
                if ((keys)[SDL_SCANCODE_N]) 
                {
                    Mix_Resume(-1);
                    inPause = 0;
                }
            }
            else
            {
                entity_think_all();
                entity_update_all();
                camera_world_snap();      // moves camera based on window resolution
            }
        }

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        // if you're not in the game
        if (!inGame) if (gfc_input_command_pressed("esc")) done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    FILE* file;
    file = fopen("stats.txt", "w");
    fprintf(file, "Spelunky walked at a speed of %f", player->speed);
    level_free(level_get_active_level());
    entity_free_all();

    //gfc_sound_free(sound);
    slog("---==== END ====---");
    return 0;
}

/*eol@eof*/
