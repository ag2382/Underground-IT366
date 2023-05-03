#include "simple_logger.h"

#include "gfc_text.h"
#include "gfc_input.h"
#include "gfc_audio.h"

#include "gfc_shape.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "gf2d_sprite.h"
#include "camera.h"

#include "tools.h"
#include "level.h"
#include "boulder.h"
#include "gold.h"
#include "totem.h"
#include "shop.h"

#include "player.h"

static Entity* miner = NULL;

// cooldown defined for multiple tools
Uint32 pickaxe_Cooldown = 400;
Uint32 shotgun_Cooldown;
Uint32 freezeray_Cooldown = 600;	// maybe add charging capabilities??
Uint32 drill_Cooldown = 1200;		// powerful weapon  ->  longer cooldown

Uint32 timeOfSwing = 0;				// pickaxe

Uint32 timeOfFire = 0;				// find time at which you started firing
Uint32 timeCurrent = 0;

// player jump stuff
float airTime = 0;
bool onGround;
bool isJumping;
bool stuck;

// WIP TIMER
bool tStarted;

// TOOL UPGRADES - temporary solution

Vector2D screen = { 0 };	// screen position of player being drawn

void player_think(Entity* self);
void player_draw(Entity* self);
void player_update(Entity* self);
void player_free(Entity* self);

/*
* @brief sets player's position in the level
* @return NULL if the player does not show up in-game
*/
void player_set_position(Vector2D position)
{
	if (!miner)
	{
		slog("playable miner is not present");
		return;
	}
	vector2d_copy(miner->position, position);
}

Entity* player_new(Vector2D position)
{
	Entity* self;
	PlayerData* data;
	SJson* json, * player;
	int health, bombs, rope, gold;

	self = entity_new();
	if (!self) return NULL;

	// * LOAD PLAYER ENTITY INTO GAME AND GIVE IT PROPERTIES * //
	// 
	//self->sprite = gf2d_sprite_load_image("images/spelunky_idle.png");
	self->think = player_think;
	self->update = player_update;
	self->draw = player_draw;
	self->shape = gfc_shape_rect(10, 0, 40, 58);
	//self->body.shape = &self->shape;
	self->free_entity = player_free;
	self->speed = 3.0;
	self->acceleration = vector2d(0, 12);
	self->team = 1;				// player is on its own team

	// * SCREEN POSITION FOR PLAYER IS DEFINED IN game.c * //
	vector2d_copy(self->position, position);

	// * JSON IMPLEMENTATION * //
	json = sj_load("config/player.json");
	if (!json)
	{
		slog("Player JSON does not exist");
		return NULL;
	}

	// * FIND PLAYER KEY * //
	player = sj_object_get_value(json, "player");

	// * EXTRACT VALUES FROM PLAYER * //
	sj_object_get_value_as_int(player, "health", &health);
	sj_object_get_value_as_int(player, "bombs", &bombs);
	sj_object_get_value_as_int(player, "rope", &rope);
	sj_object_get_value_as_int(player, "gold", &gold);


	data = gfc_allocate_array(sizeof(PlayerData), 1);
	if (data)
	{
		data->health = health;
		data->bomb_count = bombs;
		data->rope_count = rope;
		data->drill_count = 5;
		data->gold = 1000;				// for class demonstration purposes
		data->isHit = false;
		data->inShop = false;
		data->newPos = vector2d(0, 0);

		for (int i = 0; i < 5; i++) data->upgrade_count[i] = 0;
	}
	self->data = data;

	//sj_free(json);
	miner = self;
	return self;
}

// * RETURN THE POSITION OF THE PLAYER ENTITY * //	
Vector2D player_get_position()
{
	Vector2D v = { 0 };
	if (!miner) return v;
	return miner->position;
}

// * RETURN A POINTER TO THE PLAYER ENTITY * //
Entity* player_get()
{
	return miner;
}

// * DRAW THE PLAYER HERE * //
void player_draw(Entity* self)
{
	PlayerData* data = self->data;
	Vector2D camera;
	camera = camera_get_position();
	if (!self) return;

	//camera = camera_get_draw_offset();
	vector2d_sub(screen, self->position, camera);		// screen = world - camera

	gf2d_sprite_draw_image(gf2d_sprite_load_image("images/spelunky_idle.png"), screen);

	// * SET UP CAMERA * //
	self->rect = self->shape.s.r;
	self->rect.x += self->position.x;
	self->rect.y += self->position.y;

	// * player shape - used for testing purposes * //
	vector2d_add(self->rect, self->rect, camera_get_draw_offset());
	gf2d_draw_rect(self->rect, gfc_color8(255, 255, 255, 255));

	// * INTERACT WITH THE SHOPKEEPER * //
	if (data->inShop)
	{
		gf2d_sprite_draw_image(gf2d_sprite_load_image("images/shop_menu.png"), vector2d(0, 0));
	}

}

// * PICKAXE * //	
void player_UsePickaxe(Entity* self)
{
	//Pickaxe(vector2d(screen.x + 35, screen.y + 10));
	Pickaxe(vector2d(self->position.x + 35, self->position.y + 7));
}

// * WHIP * //
void player_UseWhip(Entity* self)
{
	Whip(vector2d(screen.x + 50, screen.y));
}

// * ROPE * //
void player_UseRope(Entity* self)
{
	PlayerData* data = self->data;
	if (data->rope_count < 1)
	{
		slog("You ran out of rope!");
	}
	else
	{
		Rope(vector2d(self->position.x + 64, self->position.y));
		slog("You used a rope!");
		data->rope_count--;
		self->rope_active = 1;
	}
}

// * BOMB * //
void player_UseBomb(Entity* self)
{
	PlayerData* data = self->data;

	// cross bombs
	if (bomb_upgrade)
	{
		if (data->upgrade_count[1] < 1)
		{
			slog("You ran out of cross bombs. Reverting to normal bombs");
			bomb_upgrade = 0;
		}
		else
		{
			Bomb(vector2d(self->position.x + 5, self->position.y));
			slog("You spawned a cross bomb!");
			data->upgrade_count[1]--;
		}
	}
	// default bombs
	else
	{
		if (data->bomb_count < 1)
		{
			slog("You ran out of bombs!");
		}
		else
		{
			Bomb(vector2d(self->position.x + 5, self->position.y));
			slog("You spawned a bomb!");
			data->bomb_count--;
		}
	}
}

// * SHOTGUN is fired relative to player's screen position * //
void player_UseShotgun(Entity* self)
{
	Shotgun(vector2d(screen.x + 40, screen.y - 30));
	self->shotgun_active = 1;
}

// * BOOMERANG * //
void player_UseBoomerang(Entity* self)
{
	Boomerang(vector2d(screen.x + 10, screen.y));
	self->boomerang_active = 1;
}

// * SHIELD * //
void player_UseShield(Entity* self)
{
	Shield(vector2d(self->position.x + 45, self->position.y + 10));
	self->shield_active = 1;
}

// * FREEZE RAY is fired relative to player's screen position * //
void player_UseFreezeRay(Entity* self)
{
	FreezeRay(vector2d(screen.x + 40, screen.y - 30));
	FreezeRay(vector2d(self->position.x + 40, self->position.y - 30));
}

void player_UseRocketBoots(Entity* self)	// ROCKET BOOTS
{
	//RocketBoots(vector2d(self->position.x + 60, self->position.y + 20));
	//self->rocket_boots_active = 1;

	self->velocity.y -= 2;
}

// * DRILL is fired relative to player's screen position * //
void player_UseDrillGun(Entity* self)
{
	PlayerData* data = self->data;
	// * DRILL HAS LIMITED AMMUNITION * //

	if (drill_upgrade)
	{
		if (data->upgrade_count[4] < 1)
		{
			slog("You ran out of mega drill bullets! Reverting to regular drill bullets!");
			drill_upgrade = 0;
		}
		else
		{
			DrillGun(vector2d(screen.x + 30, screen.y - 120));
			data->upgrade_count[4]--;
		}
	}
	else
	{
		if (data->drill_count < 1)
		{
			slog("You ran out of drill bullets!");
		}
		else
		{
			DrillGun(vector2d(screen.x + 30, screen.y - 30));
			data->drill_count--;
		}
	}
}

// * temporary hard-coded solution for tool upgrades * //

void checkUpgrades(Entity *self)
{
	PlayerData* data = self->data;

	// Only activate upgrades if you are not in the shop
	if (!data->inShop)
	{
		// * PICKAXE * //
		
		// attempt to upgrade
		if (gfc_input_command_pressed("upgrade pickaxe"))
		{
			// do you have the upgrade?
			if (data->upgrade_count[0])
			{
				// change accordingly
				if (!pickaxe_upgrade)
				{
					pickaxe_upgrade = 1;
					slog("diamond pickaxe activated");
				}
				else
				{
					pickaxe_upgrade = 0;
					slog("revert to regular pickaxe");
				}
			}
			else
			{
				slog("No upgrade available for the pickaxe!");
			}
		}

		// * BOMB * //
		if (gfc_input_command_pressed("upgrade bomb"))
		{
			// do you have the upgrade?
			if (data->upgrade_count[1])
			{
				// change accordingly
				if (!bomb_upgrade)
				{
					bomb_upgrade = 1;
					slog("cross bombs activated");
				}
				else
				{
					bomb_upgrade = 0;
					slog("revert to regular bomb");
				}
			}
			else
			{
				slog("No upgrade available for the bomb!");
			}
		}

		// * SHOTGUN * //
		if (gfc_input_command_pressed("upgrade shotgun"))
		{
			// do you have the upgrade?
			if (data->upgrade_count[2])
			{
				// change accordingly
				if (!shotgun_upgrade)
				{
					shotgun_upgrade = 1;
					slog("rapid shotgun activated");
				}
				else
				{
					shotgun_upgrade = 0;
					slog("revert to regular shotgun");
				}
			}
			else
			{
				slog("No upgrade available for the shotgun!");
			}
		}

		// * ROCKET BOOTS * //
		if (gfc_input_command_pressed("upgrade rocketboots"))
		{
			// do you have the upgrade?
			if (data->upgrade_count[3])
			{
				// change accordingly
				if (!rocketboots_upgrade)
				{
					rocketboots_upgrade = 1;
					slog("gun boots activated");
				}
				else
				{
					rocketboots_upgrade = 0;
					slog("revert to regular rocket boots");
				}
			}
			else
			{
				slog("No upgrade available for the rocket boots!");
			}
		}

		// * DRILL GUN * //
		if (gfc_input_command_pressed("upgrade drillgun"))
		{
			// do you have the upgrade?
			if (data->upgrade_count[4])
			{
				// change accordingly
				if (!drill_upgrade)
				{
					drill_upgrade = 1;
					slog("mega drill gun activated");
				}
				else
				{
					drill_upgrade = 0;
					slog("revert to regular drill gun");
				}
			}
			else
			{
				slog("No upgrade available for the drillgun!");
			}
		}
	}
}

// * HOW WILL THE PLAYER OPERATE WITHIN THE CONFINES OF THE GAME? * //
void player_think(Entity* self)
{
	Vector2D walk = { 0 };
	PlayerData* data = self->data;
	if (!self)return;

	const Uint8* keys;
	keys = SDL_GetKeyboardState(NULL);

	// if player presses 'A'
	if (gfc_input_command_down("walk_left") && !data->inShop)
	{	// set negative walking direction
		walk.x -= 1;
	}

	// if player presses 'D'
	if (gfc_input_command_down("walk_right") && !data->inShop)
	{	// set positive walking direction
		walk.x += 1;
	}

	if (walk.x)				// spleunky is walking in either direction
	{
		vector2d_normalize(&walk);
		// set velocity according to given speed
		vector2d_scale(walk, walk, self->speed);
		vector2d_copy(self->velocity, walk);
	}
	else
	{
		vector2d_clear(self->velocity);
	}

	// * JUMP IS INITIATED * //
	if (isJumping)
	{
		// * CALCULATE JUMP HEIGHT AND AIR TIME * //
		self->velocity.y -= self->acceleration.y;
		airTime += 0.1;

		// * IF MAX AIR TIME IS EXCEEDED or YOU BONK YOUR HEAD * //
		if (airTime > 2.5 || level_shape_clip(level_get_active_level(), entity_get_shape_after_move(self)) == 1)
		{
			// * MAKE HIM FALL * //
			isJumping = false;
			self->velocity.y = 0;
			airTime = 0;
		}
	}

	else
	{
		if (gfc_input_command_pressed("jump") && level_shape_clip(level_get_active_level(), entity_get_shape_after_move(self)) == 1)
		{
			isJumping = 1;
			Sound* sfx = gfc_sound_load("audio/sfx/classic_jump.wav", 1, 1);
			gfc_sound_play(sfx, 0, 0.25, 0, -1);
		}
		// * ONLY USE ROCKET BOOTS WHILE FALLING AFTER JUMP * //
		else if ((keys)[SDL_SCANCODE_Q] && level_shape_clip(level_get_active_level(), entity_get_shape_after_move(self)) == 0)
		{
			self->velocity.y -= 3;

			// * TAKE GUN BOOT UPGRADE INTO ACCOUNT * //
			if (gfc_input_command_pressed("boot_shoot") && rocketboots_upgrade)
			{
				GunBoot_Bullet(vector2d(screen.x, screen.y + 5));
			}
		}
	}

	// PICKAXE
	if (gfc_input_command_pressed("use_pickaxe"))
	{
		if (tStarted && timeCurrent < pickaxe_Cooldown)
		{
			timeCurrent = SDL_GetTicks() - timeOfSwing;
			pickaxe_active = 1;
		}
		else
		{
			tStarted = true;
			pickaxe_active = 0;
			timeOfSwing = SDL_GetTicks();
			timeCurrent = 0;
			player_UsePickaxe(self);
		}
	}

	// WHIP
	if (gfc_input_command_pressed("use_whip"))
	{
		player_UseWhip(self);
	}

	// ROPE
	if (gfc_input_command_pressed("use_rope"))
	{
		player_UseRope(self);
	}

	// BOMB
	if (gfc_input_command_pressed("use_bomb"))
	{
		player_UseBomb(self);
	}

	// SHOTGUN
	if (shotgun_upgrade)
	{
		if (gfc_input_command_held("use_shotgun"))
		{
			shotgun_Cooldown = 200;
			if (tStarted && timeCurrent < shotgun_Cooldown)
			{
				timeCurrent = SDL_GetTicks() - timeOfFire;
			}
			else
			{
				tStarted = true;
				timeOfFire = SDL_GetTicks();
				timeCurrent = 0;
				player_UseShotgun(self);
			}
		}
	}
	else
	{
		shotgun_Cooldown = 600;
		if (gfc_input_command_pressed("use_shotgun"))
		{
			if (tStarted && timeCurrent < shotgun_Cooldown)
			{
				timeCurrent = SDL_GetTicks() - timeOfFire;
			}
			else
			{
				tStarted = true;
				timeOfFire = SDL_GetTicks();
				timeCurrent = 0;
				player_UseShotgun(self);
			}
		}
	}

	//if (gfc_input_command_pressed("use_shotgun"))
	//{
	//	// * rapid fire shotgun * //
	//	if (shotgun_upgrade)
	//	{
	//		Uint32 shotgun_CooldownUpgrade = shotgun_Cooldown / 3;
	//		if (tStarted && timeCurrent < shotgun_CooldownUpgrade)
	//		{
	//			timeCurrent = SDL_GetTicks() - timeOfFire;
	//		}
	//		else
	//		{
	//			tStarted = true;
	//			timeOfFire = SDL_GetTicks();
	//			timeCurrent = 0;
	//			player_UseShotgun(self);
	//		}
	//	}
	//	// * regular shotgun * //
	//	else
	//	{
	//		if (tStarted && timeCurrent < shotgun_Cooldown)
	//		{
	//			timeCurrent = SDL_GetTicks() - timeOfFire;
	//		}
	//		else
	//		{
	//			tStarted = true;
	//			timeOfFire = SDL_GetTicks();
	//			timeCurrent = 0;
	//			player_UseShotgun(self);
	//		}
	//	}
	//}

	// BOOMERANG
	if ((keys)[SDL_SCANCODE_B])
	{
		if (self->boomerang_active == 0) player_UseBoomerang(self);
	}
	else
	{
		self->boomerang_active = 0;
	}

	// SHIELD
	if ((keys)[SDL_SCANCODE_G])
	{
		if (self->shield_active == 0) player_UseShield(self);
	}

	// FREEZE RAY
	if (gfc_input_command_pressed("use_freezeray"))
	{
		// * start the timer when you use the freeze ray * //
		if (tStarted && timeCurrent < freezeray_Cooldown)
		{
			timeCurrent = SDL_GetTicks() - timeOfFire;
		}
		else
		{
			tStarted = true;
			timeOfFire = SDL_GetTicks();
			timeCurrent = 0;
			player_UseFreezeRay(self);
		}
	}

	// DRILL GUN
	if (gfc_input_command_pressed("use_drillgun"))
	{
		// * start the timer when you fire the weapon * //
		if (tStarted && timeCurrent < drill_Cooldown)
		{
			timeCurrent = SDL_GetTicks() - timeOfFire;
		}
		else
		{
			tStarted = true;
			timeOfFire = SDL_GetTicks();
			timeCurrent = 0;
			timeCurrent = 0;
			player_UseDrillGun(self);
		}
	}
	camera_center_at(self->position);
}

void player_update(Entity* self)
{
	PlayerData* data = self->data;

	if (!self)return;
	if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(self)) == 0)
	{
		self->velocity.y += 4;
	}
	vector2d_add(self->position, self->position, self->velocity);
	checkUpgrades(self);
}

void player_free(Entity* self)
{
	if (!self)return;
	miner = NULL;
}