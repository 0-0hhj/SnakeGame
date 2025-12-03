#include "Sprite.h"

/*Sprite method*/
void Sprite::draw()
{
	//设置填充颜色
	setfillcolor(m_color);
	//绘制矩形
	fillrectangle(m_x, m_y, m_x + 10, m_y + 10);//四个参数，分别表示矩形的左上角到右下角的坐标
};

void Sprite::moveBy(int x, int y)
{
	m_x += x;
	m_y += y;
}
bool Sprite::collision(const Sprite& other)
{
	return m_x == other.m_x && m_y == other.m_y;
}
void Sprite::changeFoodPos()
{
	int tem_x;
	int tem_y;
	int wallData[64][48] = { 0 };
	tem_x = rand() % 64 * 10;//保证坐标是10的倍数，这样才能吃到
	tem_y = rand() % 48 * 10;
	//用一个数组把墙的位置装上
	// 设置上边墙
	for (int i = 0; i < 64; ++i) {
		wallData[i][0] = 1;
	}

	// 设置下边墙
	for (int i = 0; i < 64; ++i) {
		wallData[i][48 - 1] = 1;
	}

	// 设置左边墙
	for (int i = 0; i < 48; ++i) {
		wallData[0][i] = 1;
	}

	// 设置右边墙
	for (int i = 0; i < 48; ++i) {
		wallData[64 - 1][i] = 1;
	}
	//如果食物长在墙上，重新生成食物
	while (wallData[tem_x / 10][tem_y / 10] == 1)
	{
		tem_x = rand() % 64 * 10;
		tem_y = rand() % 48 * 10;
	}
	m_x = tem_x;
	m_y = tem_y;
}

