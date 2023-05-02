#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_draw.h"

#include "camera.h"
#include "level.h"
#include "player.h"

#include "armadillo.h"

static Entity* armadillo = NULL;
Vector2D armadillo_screen = { 0 };
action act;
int index;

Entity* armadillo_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;

	ent->think = armadillo_think;
	ent->draw = armadillo_draw;
	ent->update = armadillo_update;
	ent->shape = gfc_shape_rect(0, 0, 160, 191);
	ent->free_entity = armadillo_free;
	vector2d_copy(ent->position, position);

	stateTime = 0;
	act = IDLE;

	armadillo = ent;
	return ent;
}

Entity* armadillo_get()
{
	return armadillo;
}

Vector2D armadillo_get_position()
{
	Vector2D v = { 0 };
	if (!armadillo) return v;
	return armadillo->position;
}

void armadillo_think(Entity* ent)
{
	Vector2D a, b;
	if (!ent) return;

	a = player_get_position();
	b = ent->position;

	float dist = vector2d_magnitude_between(a, b);

	// * BULLWORM GETS HOSTILE IF PLAYER GETS CLOSE * //
	if (dist < 400)
	{
		index = rand() % 4;
		idleTime = idle_TimeSet[index];
		isHostile = true;
	}
	else
	{
		act = IDLE;
		isHostile = false;
	}

	if (isHostile)
	{
		armadillo_takeAction(ent);

		if (act == IDLE)
		{
			if (stateTime > idleTime)
			{
				stateTime = 0;
				int random = rand() % 2;
				if (random == 0) act = ROLL_LEFT;
				else act = ROLL_RIGHT;
			}
		}
		if (act == ROLL_LEFT || act == ROLL_RIGHT)
		{
			if (stateTime >= 200)
			{
				stateTime = 0;
				act = IDLE;
			}
		}
		// * MAKE ARMADILLO ROLL AT RANDOM INTERVALS * //

		// armadillo will always start being idle
		// once idle time is done, armadillo will choose to roll either left or right
	}
}

void armadillo_takeAction(Entity* ent)
{
	switch (act)
	{
		case IDLE:
			ent->frame += 0.1;
			stateTime++;
			slog("active time (IDLE): %i", stateTime);
			return;

		case ROLL_LEFT:
			ent->position.x -= 1;
			stateTime++;
			slog("active time (ROLL_LEFT): %i", stateTime);
			return;
		case ROLL_RIGHT:
			ent->position.x += 1;
			stateTime++;
			slog("active time (ROLL_RIGHT): %i", stateTime);
			return;
	}
}

void armadillo_draw(Entity* ent)
{
	Vector2D camera;
	camera = camera_get_position();
	if (!ent) return;

	// draw the worm here, in accordance to the camera
	vector2d_sub(armadillo_screen, ent->position, camera);		// screen = world - camera

	if (act == IDLE)
		gf2d_sprite_draw_image(gf2d_sprite_load_image("images/bosses/armadillo/stand.png"), armadillo_screen);

	if (act == ROLL_LEFT || act == ROLL_RIGHT)
	{
		gf2d_sprite_draw_image(
			gf2d_sprite_load_all("images/bosses/armadillo/roll.png", 160, 160, 4, 0), vector2d(armadillo_screen.x, armadillo_screen.y + 31));
	}

	// * SET UP CAMERA * //
	ent->rect = ent->shape.s.r;
	ent->rect.x += ent->position.x;
	ent->rect.y += ent->position.y;

	// * player shape - used for testing purposes * //
	vector2d_add(ent->rect, ent->rect, camera_get_draw_offset());
	gf2d_draw_rect(ent->rect, gfc_color8(255, 255, 255, 255));
}


// update - updates player sprites accordingly
void armadillo_update(Entity* ent)
{

}

void armadillo_free(Entity* ent)
{
	if (!ent) return;
	armadillo = NULL;
}