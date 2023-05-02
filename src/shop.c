#include "simple_logger.h"

#include "gfc_input.h"

#include "player.h"
#include "level.h"
#include "camera.h"
#include "shop.h"

static Entity* shop = NULL;
Vector2D shop_screen = { 0 };

Entity* shop_new(Vector2D position) 
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->think = shop_think;
    ent->draw = shop_draw;
    ent->update = shop_update;
    ent->drawOffset = vector2d(64, 74);
    vector2d_copy(ent->position, position);
    ent->shape = gfc_shape_rect(10, 0, 42, 58);

    shop = ent;

    return ent;
}

Entity* shop_get() 
{
    return shop;
}

Vector2D shop_get_position()
{
    Vector2D v = { 0 };
    if (!shop) return v;
    return shop->position;
}

void shop_draw(Entity* ent)
{
    Vector2D camera;
    camera = camera_get_position();
    if (!ent) return;

    vector2d_sub(shop_screen, ent->position, camera);
    gf2d_sprite_draw_image(gf2d_sprite_load_image("images/shopkeeper.png"), shop_screen);

    // * WHERE IS THE ENTITY RELATIVE TO THE CAMERA? * //
    ent->rect = ent->shape.s.r;
    ent->rect.x += ent->position.x;
    ent->rect.y += ent->position.y;
        
    vector2d_add(ent->rect, ent->rect, camera_get_draw_offset());
    gf2d_draw_rect(ent->rect, gfc_color8(255, 255, 255, 255));
}

void shop_think(Entity* ent)
{
    PlayerData* data = player_get()->data;

    if (gfc_input_command_pressed("action"))
    {
        if (gfc_rect_overlap(player_get()->rect, ent->rect))
        {
            if (data->inShop) data->inShop = false;			// leave the shopping area
            else data->inShop = true;						// or buy stuff
        }
    }

	// * WHAT TO BUY * //

	// Need more lives? //
	if (data->inShop && gfc_input_command_pressed("buy_health"))
	{
		if (data->gold < 25)
		{
			slog("Not enough gold");
		}
		else
		{
			data->health++;
			data->gold -= 25;
			slog("health increased to %i", data->health);
			slog("you have %i gold left", data->gold);
		}
	}

	// A diamond pickaxe? //
	if (data->inShop && gfc_input_command_pressed("buy_diamond_pickaxe"))
	{
		if (data->gold < 100)
		{
			slog("Not enough gold to buy diamond pickaxe");
		}
		else
		{
			if (data->upgrade_count[0])
			{
				slog("You already have a diamond pickaxe!");
			}
			else
			{
				data->upgrade_count[0]++;		// you only have the one diamond pickaxe
				data->gold -= 100;
				slog("diamond pickaxe: %i", data->upgrade_count[0]);
				slog("you have %i gold left", data->gold);
			}
		}
	}

	// Some cross bombs? //
	if (data->inShop && gfc_input_command_pressed("buy_crossbomb"))
	{
		if (data->gold < 50)
		{
			slog("Not enough gold");
		}
		else
		{
			data->upgrade_count[1] += 4;
			data->gold -= 50;
			slog("You added 4 Cross Bombs added to inventory. Current count: %i", data->upgrade_count[1]);
			slog("You have %i gold left", data->gold);
		}
	}

	// A rapid shotgun? //
	if (data->inShop && gfc_input_command_pressed("buy_rapid_shotgun"))
	{
		if (data->gold < 75)
		{
			slog("Not enough gold");
		}
		else
		{
			if (data->upgrade_count[2])
			{
				slog("You already have a rapid shotgun!");
			}
			else
			{
				data->upgrade_count[2]++;
				data->gold -= 75;
				slog("Rapid Shotgun added to inventory. You have %i gold left", data->gold);
			}
		}
	}

	// How about some gunboots? //
	if (data->inShop && gfc_input_command_pressed("buy_gunboots"))
	{
		if (data->gold < 150)
		{
			slog("Not enough gold");
		}
		else
		{
			if (data->upgrade_count[3])
			{
				slog("You are already wearing gunboots");
			}
			else
			{
				data->upgrade_count[3]++;
				data->gold -= 150;
				slog("GunBoots added to inventory. You have %i gold left", data->gold);
			}
		}
	}

	// Or even a gun that shoots MEGA DRILLS? //
	if (data->inShop && gfc_input_command_pressed("buy_megadrill"))
	{
		if (data->gold < 300)
		{
			slog("Not enough gold");
		}
		else
		{
			data->upgrade_count[4] += 5;
			data->gold -= 300;
			slog("Mega Drill Gun added to inventory (w/ 5 bullets)");
			slog("You have% i gold left", data->gold);
		}
	}
}

// update - updates player sprites accordingly
void shop_update(Entity* ent)
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

void shop_free(Entity* ent)
{
	if (!ent) return;
	shop = NULL;
}