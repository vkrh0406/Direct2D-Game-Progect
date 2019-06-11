#include "Bullet.h"



Bullet::Bullet()
{
}
Bullet::Bullet(const Bullet& r)
{
	x = r.x;
	y = r.y;
	a = r.a;
	size = r.size;
	translation_size = r.translation_size;
}

Bullet::Bullet(float mouse_x, float mouse_y)
{
	x = mouse_x-150.0f;
	y = mouse_y-70.0f;
	a = 0;
}

Bullet::~Bullet()
{
}
