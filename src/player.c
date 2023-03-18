#include "player.h"
#include "simple_logger.h"
#include "gfc_text.h"
#include "gfc_input.h"
#include "tools.h"

static Entity *player = NULL;

typedef struct 
{
	float walkSpeed;		// player moving from left to right
	float jump;
	float max_jump;		// how high will the player jump (after pressing X ~ PS4 controller)
	int health;			// player's starting + max health
	int jump_rise, jump_fall, jump_input;
	int bomb_count, rope_count;
	int score;
	//enum PlayerState st;
}PlayerData;

static PlayerData data = {
	7.5f,
	0.5f,
	10.0f,
	4,
	0, 0, 0,
	4, 4,
	0
};

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

Entity *player_new(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;

	self->sprite = gf2d_sprite_load_all(
		"images/spelunky_walk.png",
		128,
		128,
		9,
		0);
	self->think = player_think;
	self->update = player_update;
	vector2d_copy(self->position, position);
	
	// load default tools: pickaxe, whip, bomb, rope

	return self;
}

Entity* player_get()
{
	return player;
}

void player_draw(Entity *self)
{

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
	self->active = 1;
}

void player_UseRope(Entity* self)
{
	if (data.rope_count < 1)
	{
		slog("You ran out of rope!");
	}
	else
	{
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
	self->active = 1;
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
	self->active = 1;
}

// PLAYER USES FREEZE RAY
void player_UseFreezeRay(Entity* self)
{
	FreezeRay(vector2d(self->position.x + 60, self->position.y + 20));
	self->active = 1;
}

// PLAYER USES FREEZE RAY
void player_UseDrillGun(Entity* self)
{
	DrillGun(vector2d(self->position.x + 50, self->position.y + 20));
	self->active = 1;
}

// PLAYER USES ROCKET BOOTS

// PLAYER USES DRILL GUN

void player_think(Entity* self)
{
	//int mx, my;
	const Uint8* keys;
	keys = SDL_GetKeyboardState(NULL);
	//SDL_GetRelativeMouseState(&mx, &my);

	// PLAYER MOVEMENT HANDLED HERE

	// spelunky walks left
	if ((keys)[SDL_SCANCODE_A] || (keys[SDL_SCANCODE_LEFT]))
	{
		self->position.x -= data.walkSpeed;
	}
	// spelunky walks right
	if ((keys[SDL_SCANCODE_D]) || (keys[SDL_SCANCODE_RIGHT]))
	{
		self->position.x += data.walkSpeed;
	}

	// JUMP

	// player enters jump state
	if (data.jump_rise)
	{
		if (self->velocity.y == (-data.max_jump))
		{
			self->velocity.y = 0;
			data.jump_rise = 0;
			data.jump_fall = 1;
		}
		else
		{
			self->velocity.y -= data.jump;
		}
	}
	// player is in the jump fall state
	else if (data.jump_fall)
	{
		if (self->velocity.y == data.max_jump) {
			self->velocity.y = 0;
			data.jump_fall = 0;
		}
		else
		{
			self->velocity.y += data.jump;
		}
	}
	// initiate jump
	else
	{
		if ((keys[SDL_SCANCODE_Z]))
		{
			data.jump_input++;
			if (data.jump_input > 1)
			{
				self->velocity.y = 0;
				data.jump_rise = 0;
			}
			else
			{
				data.jump_rise = 1;
			}
		}
		else
		{
			data.jump_input = 0;
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
	if ((keys)[SDL_SCANCODE_V])
	{
		if (self->active > 0)
		{
			slog("Whip is being used already");
		}
		else
		{
			player_UseWhip(self);
		}
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
	if ((keys)[SDL_SCANCODE_R])
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
		player_UseShield(self);
	}

	// FREEZE RAY
	if ((keys)[SDL_SCANCODE_F])
	{
		player_UseFreezeRay(self);
	}

	// ROCKET BOOTS
	if ((keys)[SDL_SCANCODE_Q])
	{
		slog("use rocket boots");
	}

	// DRILL GUN
	if ((keys)[SDL_SCANCODE_T])
	{
		player_UseDrillGun(self);
	}

}

void player_update(Entity *self)
{
	if (!self)return;
	self->frame += 0.1;
	if (self -> frame >= 9) self->frame = 0;  // handles each frame of designated sprite row
	vector2d_add(self->position, self->position, self->velocity);
}