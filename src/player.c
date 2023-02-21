#include "player.h"
#include "simple_logger.h"
#include "gfc_text.h"

static Entity *player = NULL;

// draw - obtain sprites
void player_draw(Entity *self);

// think - handles player actions
void player_think(Entity *self);

// update - updates player sprites accordingly
void player_update(Entity *self);

typedef struct 
{
	int walkSpeed;		// player moving from left to right
	int jumpHeight;		// how high will the player jump (after pressing X ~ PS4 controller)
	float basePosition;
}PlayerData;

static PlayerData data = {
	3, 10, 3.5
};

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

Entity *player_new()
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("Spelunky hass not loaded in");
		return NULL;
	}

	/*gfc_line_cpy(self->name, "player");
	self->parent = NULL;*/
	slog("Spelunky has spawned");
	self->sprite = gf2d_sprite_load_all(
		"images/spelunky_walk.png",
		128,
		128,
		9,
		0);
	self->think = player_think;
	if (self->think) {
		slog("in player_think!");
	}
	self->draw = player_draw;
	self->update = player_update;
	return self;
}

void player_draw(Entity *self)
{

}

void player_think(Entity *self)
{
	int mx, my;
	const Uint8* keys;
	keys = SDL_GetKeyboardState(NULL);
	SDL_GetRelativeMouseState(&mx, &my);

	// PLAYER MOVEMENT HANDLED HERE

	// spelunky walks left
	if ((mx < 0) || (keys)[SDL_SCANCODE_A] || (keys[SDL_SCANCODE_LEFT]))
	{
		self->position.x -= 3;
	}
	// spelunky walks right
	if ((mx > 0) || (keys[SDL_SCANCODE_D]) || (keys[SDL_SCANCODE_RIGHT]))
	{
		self->position.x += 3;
	}
	// currently, spelunky walks up
	if ((my < 0) || (keys[SDL_SCANCODE_W]) || (keys[SDL_SCANCODE_UP]))
	{
		self->position.y -= 3;
	}
	// currently, spelunky walks down
	if ((my > 0) || (keys[SDL_SCANCODE_S]) || (keys[SDL_SCANCODE_DOWN]))
	{
		self->position.y += 3;
	}
}

void player_update(Entity *self)
{
	
}