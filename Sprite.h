#pragma once
#include <graphics.h> // 画图库
#include <cstdlib>    // rand

// 精灵类
class Sprite
{
protected:
	int m_x;
	int m_y;
	COLORREF m_color; // COLORREF 是一个用于表示颜色的 32 位整数类型。你可以通过 RGB 宏来定义颜色，并将其用于图形绘制
public:
	Sprite() :Sprite(0, 0, RED) {} // 委托构造函数
	Sprite(int x, int y, COLORREF mycolor = BLACK) :m_x(x), m_y(y), m_color(mycolor) {} // 实际构造函数, 使用列表初始化
	virtual void draw(); // 虚函数
	void moveBy(int x, int y); // 每次移动量
	bool collision(const Sprite& other); // 判断碰撞
	void changeFoodPos(); // 改变食物坐标（蛇得吃后）
	int getX() const { return m_x; }
	int getY() const { return m_y; }
};