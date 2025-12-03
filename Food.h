#pragma once
#include "Sprite.h"

// 食物类
class Food : public Sprite
{
public:
	Food() :Sprite(0, 0, YELLOW)
	{
		changeFoodPos();
	}
	void draw() override
	{
		setfillcolor(m_color);
		solidellipse(m_x, m_y, m_x + 10, m_y + 10);//// Draw a filled ellipse without a border
	}
};