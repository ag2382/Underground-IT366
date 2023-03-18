#include "simple_logger.h"
#include "gfc_text.h"
#include "tools.h"

// to pick up items, crouch and press X (keyboard)

// **PICKAXE**
Entity* Pickaxe(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/tools/pickaxe.png",
		128,
		128,
		4,
		0);
	vector2d_copy(self->position, position);
	self->think = Pickaxe_Think;
	return self;
}

void Pickaxe_Think(Entity* self)
{
	if (!self)return;
	self->frame += 0.1;
	if (self->frame >= 4) entity_free(self);
}

// **WHIP**
Entity* Whip(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/tools/whip_attack.png",
		128,
		128,
		6,
		0);
	vector2d_copy(self->position, position);
	self->think = Whip_Think;
	return self;
}

void Whip_Think(Entity* self)
{
	if (!self)return;
	self->frame += 0.1;
	if (self->frame >= 6) entity_free(self);
}

// **ROPE**
void Tool_Rope(Entity* self)
{

}

// **BOMB**
Entity *Bomb(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/tools/test_bomb.png",
		64,
		64,
		1,
		0);
	vector2d_copy(self->position, position);
	self->think = Bomb_Think;
	return self;
}

void Bomb_Explode(Entity* self)
{
	slog("bomb explodes here");
	entity_free(self);
}

#define BOMB_TIMER		20.0
void Bomb_Think(Entity* self)
{
	self->bomb_time += 0.1;
	if (self->bomb_time > BOMB_TIMER)
	{
		Bomb_Explode(self);
	}
}

// **SHOTGUN**
Entity* Shotgun(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/tools/shotgun.png",
		128,
		128,
		1,
		0);
	vector2d_copy(self->position, position);
	return self;
}

// **BOOMERANG**
Entity* Boomerang(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/tools/boomerang.png",
		64,
		64,
		8,
		0);
	vector2d_copy(self->position, position);
	self->think = Boomerang_Think;
	return self;
}

#define BOOMERANG_DISTANCE		8.0
void Boomerang_Think(Entity* self)
{
	if (!self)return;
	self->frame += 0.25;
	
	if (self->frame >= 8) self->frame = 0;

	// boomerang is initially thrown
	if (self->boomerang_dist > BOOMERANG_DISTANCE)
	{
		self->boomerang_return = 1;
		self->boomerang_dist -=0.01;
	}

	// boomerang is returning
	else if (self->boomerang_return)
	{
		if (self->boomerang_dist < 0)
		{
			self->boomerang_return = 0;
			slog("boomerang should go away");
			entity_free(self);
		}
		else
		{
			self->position.x -= 5.0;
			self->boomerang_dist -= 0.1;
		}
	}
	else
	{
		self->position.x += 5.0;
		self->boomerang_dist += 0.1;
	}

}

// **SHIELD**
Entity* Shield(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/tools/shield.png",
		128,
		128,
		1,
		0);
	vector2d_copy(self->position, position);
	return self;
}

// **FREEZE RAY**
Entity* FreezeRay(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/tools/freeze_ray.png",
		128,
		128,
		1,
		0);
	vector2d_copy(self->position, position);
	return self;
}

// **ROCKET BOOTS**
void Tool_RocketBoots(Entity* self)
{

}

// **DRILL GUN**
Entity* DrillGun(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/tools/drillgun.png",
		128,
		128,
		1,
		0);
	vector2d_copy(self->position, position);
	return self;
}