#pragma once

class Bullet
{
public:
	Bullet();
	Bullet(float mouse_x, float mouse_y);
	Bullet(const Bullet& r);
	~Bullet();
	float x, y;
	float size = 100;
	float translation_size = 0;
private:

};
