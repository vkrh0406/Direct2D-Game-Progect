#include "Bullet.h"



Bullet::Bullet()
{
}
Bullet::Bullet(const Bullet& r)
{
	x = r.x;
	y = r.y;
	size = r.size;
	translation_size = r.translation_size;
}

Bullet::Bullet(float mouse_x, float mouse_y)
{
	x = mouse_x;
	y = mouse_y;
}

Bullet::~Bullet()
{
}
