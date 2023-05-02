#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_draw.h"

#include "camera.h"
#include "level.h"
#include "player.h"

#include "dragonfly.h"

static Entity* dragonfly = NULL;
Vector2D dragonfly_screen = { 0 };

Entity* dragonfly_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;

	ent->think = dragonfly_think;
	ent->draw = dragonfly_draw;
	ent->update = dragonfly_update;
	ent->shape = gfc_shape_rect(0, 20, 200, 160);
	ent->free_entity = dragonfly_free;
	vector2d_copy(ent->position, position);

	dragonfly = ent;
	return ent;
}

Entity* dragonfly_get()
{
	return dragonfly;
}

Vector2D dragonfly_get_position()
{
	Vector2D v = { 0 };
	if (!dragonfly) return v;
	return dragonfly->position;
}

void dragonfly_think(Entity* ent)
{
	if (!ent) return;
	Vector2D a, b;
	if (!ent) return;

	a = player_get_position();
	b = ent->position;

	float dist = vector2d_magnitude_between(a, b);
	if (dist < 500)
	{
		slog("activate");
	}
}

void dragonfly_draw(Entity* ent)
{
	Vector2D camera;
	camera = camera_get_position();
	if (!ent) return;

	// draw the worm here, in accordance to the camera
	vector2d_sub(dragonfly_screen, ent->position, camera);		// screen = world - camera

	gf2d_sprite_draw_image(gf2d_sprite_load_image("images/bosses/dragonfly.png"), dragonfly_screen);

	// * SET UP CAMERA * //
	ent->rect = ent->shape.s.r;
	ent->rect.x += ent->position.x;
	ent->rect.y += ent->position.y;

	// * player shape - used for testing purposes * //
	vector2d_add(ent->rect, ent->rect, camera_get_draw_offset());
	gf2d_draw_rect(ent->rect, gfc_color8(255, 255, 255, 255));
}

// update - updates player sprites accordingly
void dragonfly_update(Entity* ent)
{

}

void dragonfly_free(Entity* ent)
{
	if (!ent) return;
	dragonfly = NULL;
}