#include "gui.h"
// #include "gf2d_shape.h"
#include "gf2d_draw.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"

typedef struct
{
    Sprite* hud;
    int   healthPercent;
    int   bombCount;
}GUI;

static GUI gui = { 0 };

void gui_close_hud()
{
    gf2d_sprite_free(gui.hud);
}

void gui_setup_hud()
{
    memset(&gui, 0, sizeof(GUI));
    gui.hud = gf2d_sprite_load_image("images/hud/hud_bg.png");
    atexit(gui_close_hud);
}

void gui_draw_hud()
{
    Vector4D color = { 255,255,255,255 };
    /*if (gui.healthPercent < 0.2)
    {
        gui.alert = (gui.alert + 0.02);
        if (gui.alert >= M_PI)gui.alert = 0;
        color.y = color.z = sin(gui.alert) * 255;
    }*/
    gf2d_sprite_draw(
        gui.hud, vector2d(0, 0),
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        0);
    // gui_draw_percent_bar_horizontal(gf2d_rect(495, 680, 145, 10), gui.healthPercent, vector4d((1 - gui.healthPercent) * 255, gui.healthPercent * 255, 0, 255), vector4d(128, 0, 0, 128), 0);
    //gui_draw_percent_bar_horizontal(gf2d_rect(640, 680, 145, 10), gui.shieldPercent, vector4d(0, 0, 255, 255), vector4d(128, 0, 0, 128), 1);
    //gui_draw_percent_bar_vertical(gf2d_rect(475, 680, 15, 30), gui.thrustPercent, vector4d(0, 255, 255, 255), vector4d(128, 0, 0, 128), 1);
    //gui_draw_percent_bar_vertical(gf2d_rect(790, 680, 15, 30), gui.chargePercent, vector4d(128 + (127 * gui.chargePercent), 128 - (127 * gui.chargePercent), 128 - (127 * gui.chargePercent), 255), vector4d(128, 0, 0, 128), 1);
    //gf2d_text_draw_line("Health", FT_Small, gf2d_color8(0, 255, 0, 255), vector2d(495, 665));
    //gf2d_text_draw_line("Shields", FT_Small, gf2d_color8(0, 0, 255, 255), vector2d(672, 665));
}

void gui_set_health(int health)
{
    gui.healthPercent = health;
}

void gui_set_bombs(int bombs)
{
    gui.bombCount = bombs;
}

//void gui_set_charge(float charge)
//{
//    gui.chargePercent = charge;
//}
//
//void gui_set_thrust(float thrust)
//{
//    gui.thrustPercent = thrust;
//}