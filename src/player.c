#include "simple_logger.h"
#include "gfc_text.h"
#include "gfc_input.h"
#include "gfc_shape.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "tools.h"
#include "camera.h"
#include "player.h"
#include "level.h"

static Entity* player = NULL;

//Screen dimension constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 720;

// player jump stuff
float airTime = 0;
bool isJumping;

typedef struct
{
	int health;			// player's starting + max health
	int bomb_count, rope_count;
	int score;
	bool onGround;
	//enum PlayerState st;
}PlayerData;

static PlayerData data = {
	4,
	4, 4,
	0,
	false
};

void player_draw(Entity* self);
void player_think(Entity* self);
void player_update(Entity* self);

/*
* @brief sets player's position in the level
* @return NULL if the player does not show up in-game
*/
void player_set_position(Vector2D position)
{
	if (!player)
	{
		slog("playable miner is not present");
		return;
	}
	vector2d_copy(player->position, position);
}

Entity* player_new(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;

	self->sprite = gf2d_sprite_load_all(
		"images/spelunky_idle.png",
		64,
		64,
		1,
		0);
	self->think = player_think;
	self->draw = player_draw;
	self->update = player_update;
	self->shape = gfc_shape_rect(64, 64, 128, 128);
	//self->shape = gfc_shape_circle(0, 0, 10);
	vector2d_copy(self->position, position);
	self->speed = 2.5;
	self->acceleration = vector2d(0, 20);
	self->drawOffset = vector2d(64, 74);
	// load default tools: pickaxe, whip, bomb, rope

	return self;
}

Vector2D player_get_position()
{
	Vector2D v = { 0 };
	if (!player) return v;
	return player->position;
}

Entity* player_get()
{
	return player;
}

void player_draw(Entity* self)
{
	Vector2D drawPosition, camera;
	Rect rect = { self->position.x, self->position.y, 64, 64 };
	if (!self) return;
	camera = camera_get_draw_offset();
	gf2d_draw_rect(rect, gfc_color8(255, 255, 0, 255));

	//slog("player drawn does not work");
}

// PLAYER USES PICKAXE
void player_UsePickaxe(Entity* self)
{
	Pickaxe(vector2d(self->position.x + 50, self->position.y + 10));
	self->pickaxe_active = 1;
}

// PLAYER USES WHIP
void player_UseWhip(Entity* self)
{
	Whip(vector2d(self->position.x + 50, self->position.y));
}

void player_UseRope(Entity* self)
{
	if (data.rope_count < 1)
	{
		slog("You ran out of rope!");
	}
	else
	{
		Rope(vector2d(self->position.x + 40, self->position.y + 50));
		slog("You used a rope!");
		data.rope_count--;
		self->rope_active = 1;
	}
}

// @brief player spawns a bomb under a specific keybind ("C" for now)
void player_UseBomb(Entity* self)
{
	if (data.bomb_count < 1)
	{
		slog("You ran out of bombs!");
	}
	else
	{
		Bomb(vector2d(self->position.x + 40, self->position.y + 50));
		slog("You have spawned a bomb!");
		data.bomb_count--;
		self->bomb_active = 1;
	}
}

// PLAYER USES ROPE

// PLAYER USES SHOTGUN
void player_UseShotgun(Entity* self)
{
	Shotgun(vector2d(self->position.x + 60, self->position.y + 20));
	self->shotgun_active = 1;
}

// PLAYER USES BOOMERANG
void player_UseBoomerang(Entity* self)
{
	Boomerang(vector2d(self->position.x + 60, self->position.y + 50));
	self->boomerang_active = 1;
}

// PLAYER USES SHIELD
void player_UseShield(Entity* self)
{
	Shield(vector2d(self->position.x + 45, self->position.y + 10));
	self->shield_active = 1;
}

// PLAYER USES FREEZE RAY
void player_UseFreezeRay(Entity* self)
{
	FreezeRay(vector2d(self->position.x + 60, self->position.y + 20));
	self->freeze_ray_active = 1;
}

// PLAYER USES ROCKET BOOTS
void player_UseRocketBoots(Entity* self)
{
	RocketBoots(vector2d(self->position.x + 60, self->position.y + 20));
	self->rocket_boots_active = 1;
}

// PLAYER USES DRILL GUN
void player_UseDrillGun(Entity* self)
{
	DrillGun(vector2d(self->position.x + 50, self->position.y-50));
	self->drill_gun_active = 1;
}

void player_think(Entity* self)
{
	//slog("rect.x: %i", rect.x);
	// no longer moving down
	//int clip = level_shape_clip(level_get_active_level(), entity_get_shape_after_move(self));

	if (!self)return;

	Vector2D walk = { 0 };
	//camera = camera_get_position();
	
	const Uint8* keys;
	keys = SDL_GetKeyboardState(NULL);

	// if mapsize[self->position.x, self->position.y] == 1
		// collision occurs

	// spelunky walks left
	if (gfc_input_command_down("walk_left"))
	{
		walk.x -= 1;
		//self->position.x -= self->speed;
	}
	// spelunky walks right
	if (gfc_input_command_down("walk_right"))
	{
		walk.x += 1;
		//self->position.x += self->speed;
	}

	if (gfc_input_command_down("walk_up"))
	{
		walk.y -= 1;
		//self->position.x -= self->speed;
	}
	// spelunky walks right
	if (gfc_input_command_down("walk_down"))
	{
		walk.y += 1;
		//self->position.x += self->speed;
	}

	// EITHER WALKING LEFT OR RIGHT
	if ((walk.x) || (walk.y))
	{
		//slog("position: %f", self->position.x);
		vector2d_normalize(&walk);
		vector2d_scale(walk, walk, self->speed);
		vector2d_copy(self->velocity, walk);
		//if (clip) vector2d_copy(self->velocity, -walk);
	}
	else
	{
		vector2d_clear(self->velocity);
	}

	if (isJumping)
	{
		if (data.onGround)
		{
			self->velocity.y -= self->acceleration.y;
			// CALCULATE HOW LONG PLAYER IS IN THE AIR FOR WHILE JUMPING
			airTime += 0.1;
			if (airTime > 2.5)					// ONCE PLAYER REACHES MAXIMUM AIR TIME
			{
				isJumping = false;				// MAKE HIM FALL
				self->velocity.y = 0;
				airTime = 0;					// RESET AIR TIME
			}
		}
	}
	else
	{
		if (gfc_input_command_pressed("jump") && data.onGround)
		{
			isJumping = 1;
			data.onGround = false;
		}
	}

	// PICKAXE
	if ((keys)[SDL_SCANCODE_X])
	{
		if (self->pickaxe_active == 0) player_UsePickaxe(self);
	}
	else
	{
		self->pickaxe_active = 0;
	}

	// WHIP

	if (gfc_input_command_pressed("use_whip"))
	{
		player_UseWhip(self);

		// make whip inactive once it goes through active frames
	}

	// ROPE
	if ((keys)[SDL_SCANCODE_H])
	{
		if (self->rope_active == 0) player_UseRope(self);
	}
	else
	{
		self->rope_active = 0;
	}

	// BOMB
	if ((keys)[SDL_SCANCODE_C])		// button is held
	{
		if (self->bomb_active == 0) player_UseBomb(self);
	}
	else
	{
		self->bomb_active = 0;
	}

	// SHOTGUN
	if (gfc_input_command_pressed("use_shotgun"))
	{
		player_UseShotgun(self);
	}

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
		player_UseFreezeRay(self);
	}

	// ROCKET BOOTS
	if ((keys)[SDL_SCANCODE_Q])
	{
		if (self->rocket_boots_active == 0) player_UseRocketBoots(self);
	}

	// DRILL GUN
	if (gfc_input_command_pressed("use_drillgun"))
	{
		player_UseDrillGun(self);
	}

	//camera_center_at(self->position);
}

void player_update(Entity* self)
{
	if (!self)return;
	self->velocity.y += SDL_STANDARD_GRAVITY;

	//if (self->frame >= 9) self->frame = 0;  // handles each frame of designated sprite row
	vector2d_add(self->position, self->position, self->velocity);
	// CHECK FOR COLLISIONS IN THE LEVEL

	Vector2D newPositions = self->position;

	Level *lev = level_get_active_level();

	if (self->position.y >= 640)
	{
		self->position.y = 640;
		self->velocity.y = 0;
		data.onGround = true;
	}

	// CALCULATE PLAYER'S POSITION RELATIVE TO LEVEL
	int player_x = (int)self->position.x / lev->tileSize.x;		// x
	int player_y = (int)self->position.y / lev->tileSize.x;		// y

	slog("tile locations: %i, %i", (int)player_x, (int)player_y);

	//slog("tile size y: %f", lev->mapSize.y);

	/*
	 *	WIP COLLISION SYSTEM
	 *     (Tile-by-Tile)
	 */

	// CHECK FOR COLLISIONS WHILE MOVING TO THE LEFT
	if (self->velocity.x <= 0) 
	{
		// TOP AND BOTTOM LEFT OF SPRITE
		if (get_level_tile(lev, player_x, player_y) != 0
			|| get_level_tile(lev, player_x, player_y + 0.9f) != 0)
		{
			newPositions.x = player_x + lev->tileSize.x;
			self->velocity.x = 0;
		}
	}
	// CHECK FOR COLLISIONS WHILE MOVING TO THE RIGHT
	else
	{
		// TOP AND BOTTOM RIGHT OF SPRITE
		if (get_level_tile(lev, player_x + 1.0f, player_y) != 0
			|| get_level_tile(lev, player_x + 1.0f, player_y + 0.9f) != 0)
		{
			newPositions.x = player_x * lev->tileSize.x;
			self->velocity.x = 0;
		}
	}

	self->position.x = newPositions.x;

	// CHECK FOR COLLISIONS WHILE JUMPING UP AND DOWN
	//if (self->velocity.y <= 0)
	//{
	//	// UPPER LEFT AND RIGHT OF SPRITE
	//	if (get_level_tile(lev, (int)player_x, (int)player_y) != 0
	//		|| get_level_tile(lev, (int)player_x + 1.0f, (int)player_y) != 0)
	//	{
	//		self->position.y = (int)player_y * lev->tileSize.y;
	//		self->velocity.y = 0;
	//	}
	//}
	//else
	//{
	//	// LOWER LEFT AND RIGHT OF SPRITE
	//	if (get_level_tile(lev, (int)player_x, (int)player_y + 0.9f) != 0
	//		|| get_level_tile(lev, (int)player_x + 1.0f, (int)player_y + 0.9f) != 0)
	//	{
	//		self->position.y = (int)player_y;
	//		self->velocity.y = 0;
	//		//data.onGround = true;
	//	}
	//}
}

void player_free(Entity* self)
{
	if (!self)return;
	player = NULL;
}