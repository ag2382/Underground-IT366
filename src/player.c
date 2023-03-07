#include "player.h"
#include "simple_logger.h"
#include "gfc_text.h"
#include "tools.h"

static Entity *player = NULL;

typedef struct 
{
	float walkSpeed;		// player moving from left to right
	float jump;
	float max_jump;		// how high will the player jump (after pressing X ~ PS4 controller)
	int health;			// player's starting + max health
	int armor, maxArmor;
	int jump_rise, jump_fall, jump_input;
	int bomb_count;
	//enum PlayerState st;
}PlayerData;

static PlayerData data = {
	7.5f, 
	0.5f, 
	10.0f, 
	4,
	0, 3, 
	0, 0, 0, 
	4
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

	slog("Spelunky has spawned with %i health", data.health);
	return self;
}

Entity* player_get()
{
	return player;
}

void player_draw(Entity *self)
{

}


// @brief player spawns a bomb under a specific keybind ("C" for now)
// implement 60 second cooldown after spawning a bomb --> DON'T SPAWN EVERY FRAME
void player_useBomb(Entity* self)
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
		self->active = 1;

		// start cooldown as soon as bomb is dropped
		//Tool_Bomb_Cooldown(self);
		self->cooldown += 1.0;

	}
}

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
		slog("Spelunky used pickaxe");
	}

	// BOMB
	if ((keys)[SDL_SCANCODE_C])
	{
		// when you press the button, spawn a bomb
		// bombs can only spawn in 1 SECOND intervals

		/*Entity* bomb = NULL;

		if (bomb != NULL)
		{
			slog("There is an active bomb already");

		}*/
		// if you hold down the button
		if (self->cooldown > 0)
		{
			slog("There is an active bomb already");
			self->cooldown += 1.0;
			if (self->cooldown == 60) self->cooldown = 0;
		}
		else
		{
			player_useBomb(self);
		}
	}
}

void player_update(Entity *self)
{
	if (!self)return;
	self->frame += 0.1;
	if (self -> frame >= 9) self->frame = 0;  // handles each frame of designated sprite row
	vector2d_add(self->position, self->position, self->velocity);
}