#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_draw.h"

#include "camera.h"
#include "level.h"
#include "player.h"

#include "bullworm.h"

static Entity* bullworm = NULL;
Vector2D bullworm_screen = { 0 };
direction dir;

// * DETERMINE HOW THE WORM WILL MOVE IN GAME * //
enum direciton bullworm_ChooseDirection();
enum direction bullworm_ChangeDirection();
enum direction bullworm_GetDirection();

Entity* bullworm_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;

	ent->think = bullworm_think;
	ent->draw = bullworm_draw;
	ent->update = bullworm_update;
	ent->shape = gfc_shape_rect(25, 15, 480, 225);
	ent->free_entity = bullworm_free;
	vector2d_copy(ent->position, position);
	bullworm_ChooseDirection();

	bullworm = ent;
	return ent;
}

Entity* bullworm_get()
{
	return bullworm;
}

Vector2D bullworm_get_position()
{
	Vector2D v = { 0 };
	if (!bullworm) return v;
	return bullworm->position;
}

void bullworm_move(Entity* ent)
{
	if (!ent) return;

	dir = bullworm_GetDirection();
	switch (dir) {
		case LEFT:
			ent->position.x -= 1;
			moveTime++;
			return;

		case RIGHT:
			ent->position.x += 1;
			moveTime++;
			return;
	}
}

void bullworm_think(Entity* ent)
{
	Vector2D a, b;
	if (!ent) return;

	a = player_get_position();
	b = ent->position;

	float dist = vector2d_magnitude_between(a, b);

	// * BULLWORM GETS HOSTILE IF PLAYER GETS CLOSE * //
	if (dist < 575) isHostile = true;
	else isHostile = false;

	if (isHostile)
	{
		bullworm_move(ent);
		if (moveTime >= 150)
		{
			moveTime = 0;
			bullworm_ChangeDirection();
		}
	}
}

enum direction bullworm_ChooseDirection()
{
	random = rand() % 2;
	if (random == 0)
	{
		dir = LEFT;
	}
	else
	{
		dir = RIGHT;
	}

	return dir;
}

enum direction bullworm_ChangeDirection()
{
	
	if (dir == LEFT) dir = RIGHT;
	else dir = LEFT;

	return dir;
}

enum direction bullworm_GetDirection()
{
	return dir;
}

void bullworm_draw(Entity* ent)
{
	Vector2D camera;
	camera = camera_get_position();
	if (!ent) return;

	// draw the worm here, in accordance to the camera
	vector2d_sub(bullworm_screen, ent->position, camera);		// screen = world - camera

	gf2d_sprite_draw_image(gf2d_sprite_load_image("images/bosses/bullworm.png"), bullworm_screen);

	// * SET UP CAMERA * //
	ent->rect = ent->shape.s.r;
	ent->rect.x += ent->position.x;
	ent->rect.y += ent->position.y;

	// * player shape - used for testing purposes * //
	vector2d_add(ent->rect, ent->rect, camera_get_draw_offset());
	gf2d_draw_rect(ent->rect, gfc_color8(255, 255, 255, 255));
}

void bullworm_update(Entity* ent)
{

}

void bullworm_free(Entity* ent)
{
	if (!ent) return;
	bullworm = NULL;
}