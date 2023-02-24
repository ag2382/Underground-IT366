#include "player.h"
#include "simple_logger.h"
#include "gfc_text.h"

#include <stdbool.h>

static Entity *player = NULL;

//enum PlayerState {
//	PLAYER_IDLE,
//	PLAYER_MOVE_LEFT,
//	PLAYER_MOVE_RIGHT,
//	PLAYER_MOVE_UP,
//	PLAYER_MOVE_DOWN,
//	PLAYER_JUMP_RISE,
//	PLAYER_JUMP_FALL,
//	PLAYER_GROUND
//};

typedef struct 
{
	int walkSpeed;		// player moving from left to right
	float jump, max_jump;		// how high will the player jump (after pressing X ~ PS4 controller)
	int health;			// player's starting + max health
	int armor, maxArmor;
	int jump_rise, jump_apex, jump_fall, jump_input;
	//enum PlayerState st;
}PlayerData;

static PlayerData data = {5, 1.0f, 6.0f, 3, 0, 3, 0, 0, 0, 0};

Entity *player_get()
{
	return player;
}

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

	//gfc_line_cpy(self->name, "player");
	//self->parent = null;
	slog("Spelunky has spawned");
	self->sprite = gf2d_sprite_load_all(
		"images/spelunky_walk.png",
		128,
		128,
		9,
		0);
	self->think = player_think;
	self->update = player_update;
	vector2d_copy(self->position, position);
	//self->draw = player_draw;
	return self;
}

void player_draw(Entity *self)
{

}

void player_think(Entity *self)
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
	// currently, spelunky walks up
	/*if ((keys[SDL_SCANCODE_W]) || (keys[SDL_SCANCODE_UP]))
	{
		self->position.y -= data.walkSpeed;
	}*/
	// currently, spelunky walks down
	/*if ((keys[SDL_SCANCODE_S]) || (keys[SDL_SCANCODE_DOWN]))
	{
		self->position.y += data.walkSpeed;
	}*/
	
	// JUMP

	// when you're in the jump state, check if you are exceeding the jump height
	// otherwise, increment jump
	if (data.jump_rise)
	{
		if (self->velocity.y < (-data.max_jump))
		{
			slog("max jump height reached");
			data.jump_rise = 0;
			data.jump_apex = 1;
		}
		else
		{
			self->velocity.y -= data.jump;
			slog("y-velocity up: %f", self->velocity.y);
		}
	}
	else if (data.jump_apex)
	{
		// hold key to jump higher
		slog("Entering fall state");
		data.jump_apex = 0;
		data.jump_fall = 1;
	}
	else if (data.jump_fall)
	{
		if (self->velocity.y > data.max_jump) {
			slog("Spelunky touched ground");
			data.jump_fall = 0;
			self->velocity.y = 0;
		}
		else
		{
			self->velocity.y += data.jump;
			slog("y-velocity down: %f", self->velocity.y);
		}
	}
	else
	{
		if ((keys[SDL_SCANCODE_Z]))
		{
			data.jump_input++;
			slog("how many jumps? %i", data.jump_input);
			if (data.jump_input > 1)
			{
				slog("No infinite jumping");
				self->velocity.y = 0;
				data.jump_rise = 0;
			}
			else
			{
				data.jump_rise = 1;
			}
			//data.jump_state = 1;
		}
		else
		{
			data.jump_input = 0;
		}
	}
}

void player_update(Entity *self)
{
	if (self->velocity.y < -data.max_jump)
	{
		slog("found from player update");
	}
}