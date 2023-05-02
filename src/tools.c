#include "simple_logger.h"
#include "gfc_text.h"
#include "tools.h"
#include "camera.h"
#include "level.h"
#include "player.h"

// operate tools with respect to player position //
int tileX;
int tileY;

// to pick up items, crouch and press X (keyboard)

static Entity* tool = NULL;

// * SCREEN POSITIONS FOR EACH TOOL * //
Vector2D pickaxe_screen = { 0 };
Vector2D whip_screen = { 0 };
Vector2D rope_screen = { 0 };
Vector2D bomb_screen = { 0 };
Vector2D freezeray_screen = { 0 };
Vector2D drill_screen = { 0 };

// * PICKAXE *
Entity* Pickaxe(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;

	vector2d_copy(self->position, position);

	if (pickaxe_upgrade) self->draw = Diamond_Pickaxe_Draw;
	else self->draw = Pickaxe_Draw;

	self->think = Pickaxe_Think;
	self->shape = gfc_shape_rect(24, 0, 38, 64);
	return self;
}

void Pickaxe_Think(Entity* self)
{
	if (!self)return;
	self->frame += 0.1;

	// * Detect digging at this time * //
	// * Put away the pickaxe * //
	if (self->frame >= 4)
	{
		if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(self)) == 1)
		{
			// check for each type of terrain //
			tileX = (self->position.x + 40) / level_get_active_level()->tileSize.x;
			tileY = (self->position.y + 64) / level_get_active_level()->tileSize.y;
			//slog("digging at tile %i, %i", tileX, tileY);

			level_removeTile(level_get_active_level(), tileX, tileY);
		} 
		entity_free(self);
	}

}

void Pickaxe_Draw(Entity* self)
{
	Vector2D camera;
	camera = camera_get_position();

	if (!self) return;
	vector2d_sub(pickaxe_screen, self->position, camera);
	
	gf2d_sprite_draw(
			gf2d_sprite_load_all(
				"images/tools/pickaxe_small.png", 64, 64, 4, 0),		// draw the sprite
			pickaxe_screen,				// to its designated screen position
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			(int)self->frame);
}

void Diamond_Pickaxe_Draw(Entity* self)
{
	Vector2D camera;
	camera = camera_get_position();

	if (!self) return;
	vector2d_sub(pickaxe_screen, self->position, camera);

	gf2d_sprite_draw(
		gf2d_sprite_load_all(
			"images/tools/diamond_pickaxe.png", 64, 64, 4, 0),		// draw the sprite
		pickaxe_screen,				// to its designated screen position
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(int)self->frame);
}

// * WHIP *
Entity* Whip(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/tools/whip_attack_small.png",
		64,
		64,
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

// * ROPE *
Entity* Rope(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	//self->sprite = gf2d_sprite_load_all(
	//	"images/tools/rope_small.png",
	//	64,
	//	64,
	//	5,
	//	0);
	vector2d_copy(self->position, position);
	self->think = Rope_Think;
	self->draw = Rope_Draw;
	return self;
}

void Rope_Think(Entity* self)
{
	if (!self)return;
	self->frame += 0.1;
	if (self->frame >= 4) 
	{
		self->frame = 4;
		//entity_free(self);
	}
}

void Rope_Draw(Entity* self)
{
	Vector2D camera;
	camera = camera_get_position();
	if (!self) return;
	vector2d_sub(rope_screen, self->position, camera);
	gf2d_sprite_draw(
		gf2d_sprite_load_all(
			"images/tools/rope_small.png",
			64,
			64,
			5,
			0),		// draw the sprite
		rope_screen,				// to its designated screen position
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(int)self->frame);
}

// * BOMB *
Entity *Bomb(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;

	vector2d_copy(self->position, position);
	self->think = Bomb_Think;

	// draw the different bombs according to active upgrade //
	if (bomb_upgrade) self->draw = Cross_Bomb_Draw;
	else self->draw = Bomb_Draw;

	self->update = Bomb_Update;
	self->shape = gfc_shape_rect(0, 0, 64, 64);
	//self->team = team;
	return self;
}

void Bomb_Explode(Entity* self)
{
	slog("bomb explodes here");
	entity_free(self);
}

void Bomb_Draw(Entity* self)
{
	Vector2D camera;
	camera = camera_get_position();
	if (!self) return;
	vector2d_sub(bomb_screen, self->position, camera);

	// * Check for bomb upgrades * //
	gf2d_sprite_draw(
		gf2d_sprite_load_image("images/tools/test_bomb.png"),		// draw the sprite
		bomb_screen,				// to its designated screen position
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(int)self->frame);
}

void Cross_Bomb_Draw(Entity* self)
{
	Vector2D camera;
	camera = camera_get_position();
	if (!self) return;
	vector2d_sub(bomb_screen, self->position, camera);

	// * Check for bomb upgrades * //
	gf2d_sprite_draw(
		gf2d_sprite_load_image("images/tools/cross_bomb.png"),		// draw the sprite
		bomb_screen,				// to its designated screen position
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(int)self->frame);
}

#define BOMB_TIMER		20.0
void Bomb_Think(Entity* self)
{
	self->bomb_time += 0.1;
	if (self->bomb_time > BOMB_TIMER)
	{
		// * bombs and cross bombs will behave differently * //
		Bomb_Explode(self);
	}
}

// * Bombs SHOULD NOT be levitating in the air * //
void Bomb_Update(Entity* self)
{
	if (!self)return;

	self->velocity.y += 0.25;

	if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(self)) == 1)
	{
		self->velocity.y = 0;
	}
	vector2d_add(self->position, self->position, self->velocity);
}

// * SHOTGUN *
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

// * BOOMERANG *
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

// * SHIELD *
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

// * FREEZE RAY *
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

Entity* GunBoot_Bullet(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/tools/gunboot_bullet.png",
		64,
		64,
		1,
		0);
	self->update = GunBoot_Bullet_Update;
	vector2d_copy(self->position, position);
	return self;
}

void GunBoot_Bullet_Update(Entity* self)
{
	self->velocity.y = 10;
	vector2d_add(self->position, self->position, self->velocity);
}

// * DRILL GUN * //
Entity* DrillGun(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;


	// * check for drill upgrades * //
	if (drill_upgrade)
	{
		self->sprite = gf2d_sprite_load_all(
			"images/tools/mega_drill.png",
			256,
			256,
			1,
			0);
		self->think = MegaDrillGun_Think;
	}
	else
	{
		self->sprite = gf2d_sprite_load_all(
			"images/tools/drill_bullet.png",
			128,
			128,
			1,
			0);
		self->think = DrillGun_Think;
	}
	vector2d_copy(self->position, position);
	return self;
}

void DrillGun_Think(Entity* self)
{
	self->position.x += 4;
}

void MegaDrillGun_Think(Entity* self)
{
	self->position.x += 2;
}