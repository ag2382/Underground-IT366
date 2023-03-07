#include "simple_logger.h"
#include "gfc_text.h"
#include "tools.h"

// to pick up items, crouch and press X (keyboard)

Entity* tool_new(Vector2D position)
{
	Entity* tool;
	tool = entity_new();
	if (!tool) return NULL;
	tool->sprite = gf2d_sprite_load_all(
		"images/test_pickaxe.png",
		160,
		160,
		1,
		0);
	return tool;
}

void Tool_Pickaxe(Entity* self)
{

}

void Tool_Whip(Entity* self)
{

}

void Tool_Rope(Entity* self)
{

}

Entity *Bomb(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/test_bomb.png",
		64,
		64,
		1,
		0);
	vector2d_copy(self->position, position);
	return self;
}

void Tool_Bomb_Get(Entity* self)
{
	return self;
}

void Tool_Bomb_Cooldown (Entity* self)
{
	slog("in bomb cooldown");
	if (!self)return;
	self->frame += 1;
	if (self->frame >= 60) self->frame = 0;  // handles each frame of designated sprite row
	
}

void Tool_Shotgun(Entity* self)
{

}

void Tool_Boomerang(Entity* self)
{

}

void Tool_Shield(Entity* self)
{

}

void Tool_FreezeRay(Entity* self)
{

}

void Tool_RocketBoots(Entity* self)
{

}

void Tool_DrillGun(Entity* self)
{

}