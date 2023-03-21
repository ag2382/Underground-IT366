#include "simple_logger.h"
#include "gfc_text.h"
#include "tools.h"
#include "camera.h"

// to pick up items, crouch and press X (keyboard)

static Entity* tool = NULL;

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
	self->whip_active = 1;
	return self;
}

void Whip_Think(Entity* self)
{
	if (!self)return;
	self->frame += 0.15;
	if (self->frame >= 6) {
		self->whip_active = 0;
		slog("whip can be used again");
		entity_free(self);
	}
}

// **ROPE**
Entity* Rope(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/tools/rope.png",
		128,
		128,
		5,
		0);
	vector2d_copy(self->position, position);
	self->think = Rope_Think;
	return self;
}

void Rope_Think(Entity* self)
{
	if (!self)return;
	self->frame += 0.1;
	if (self->frame >= 5) {
		self->whip_active = 0;
		slog("rope can be used again");
		entity_free(self);
	}
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
	self->draw = Bomb_Draw;
	return self;
}

void Bomb_Explode(Entity* self)
{
	slog("bomb explodes here");
	entity_free(self);
}

void Bomb_Draw(Entity* self)
{
	Vector2D drawPosition, camera;
	if (!self) return;
	camera = camera_get_draw_offset();
	vector2d_add(drawPosition, self->position, camera);
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
		"images/tools/shotgun_blast.png",
		128,
		128,
		1,
		0);
	vector2d_copy(self->position, position);
	self->think = Shotgun_Think;
	return self;
}

void Shotgun_Think(Entity* self)
{
	self->frame += 0.1;
	if (self->frame >= 4)
	{
		self->frame = 0;
		entity_free(self);
	}
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
		"images/tools/freeze_ray_ice.png",
		128,
		128,
		1,
		0);
	vector2d_copy(self->position, position);
	self->think = FreezeRay_Think;
	return self;
}

void FreezeRay_Think(Entity* self)
{
	self->position.x += 8;
}


// **ROCKET BOOTS**
Entity* RocketBoots(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/tools/rocket_boots.png",
		128,
		128,
		1,
		0);
	vector2d_copy(self->position, position);
	return self;
}

// **DRILL GUN**
Entity* DrillGun(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/tools/drill_bullet.png",
		128,
		128,
		1,
		0);
	vector2d_copy(self->position, position);
	self->think = DrillGun_Think;
	return self;
}

void DrillGun_Think(Entity* self)
{
	self->position.x += 4;
}

Entity* tool_get()
{
	return tool;
}