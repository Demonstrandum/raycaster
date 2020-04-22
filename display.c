#include "display.h"

char *display_xy(char *name, xy_t *data)
{
	char *s = malloc(512 * sizeof(char));
	sprintf(s, "%s{  x: %0.3lf,  y: %0.3lf }",
		name, data->x, data->y);
	return s;
}

char *display_position(Position *pos)
{
	return display_xy("Position", (xy_t *)pos);
}

char *display_velocity(Velocity *vel)
{
	char *s = malloc(512 * sizeof(char));
	sprintf(s, "Velocity{ dx: %0.3lf, dy: %0.3lf }",
		vel->dx, vel->dy);
	return s;
}

char *display_direction(Direction *dir)
{
	return display_xy("Direction", (xy_t *)dir);
}

char *display_plane(Plane *pln)
{
	return display_xy("Plane", (xy_t *)pln);
}

char *display_player(Player *player)
{
	char *s = malloc(512 * sizeof(char));
	size_t end = 0;
	end += sprintf(s, "Player {\n");
	end += sprintf(s + end, "     %s,\n",
		display_position(&player->position));
	end += sprintf(s + end, "     %s,\n",
		display_velocity(&player->velocity));
	end += sprintf(s + end, "    %s,\n",
		display_direction(&player->direction));
	end += sprintf(s + end, "        %s\n",
		display_plane(&player->plane));
	sprintf(s + end, "}");
	return s;
}
