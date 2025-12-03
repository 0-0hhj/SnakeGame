#pragma once
#include <deque> // 使用双端队列以便高效在头部插入/尾部删除
#include <graphics.h> // 画图库（用于 VK_* 等）
#include "Sprite.h"
#include <vector>

// 蛇类
class Snake
{
private:
	// 蛇有很多节，使用 deque 以便在头部插入和尾部弹出
	std::deque<Sprite> nodes; // 蛇的所有节点

public:
	int dir; // 表示蛇的方向
	COLORREF m_color; // 蛇的颜色
	Snake() : dir(VK_RIGHT), m_color(GREEN)
	{
		// 初始化三节蛇（nodes.front() 为头）
		nodes.push_back(Sprite(30, 10, m_color));
		nodes.push_back(Sprite(20, 10, m_color));
		nodes.push_back(Sprite(10, 10, m_color));
	}
	// 可指定默认颜色的构造函数
	Snake(int x, int y, COLORREF mycolor) : dir(VK_RIGHT)
	{
		nodes.push_back(Sprite(30, 10, mycolor));
		nodes.push_back(Sprite(20, 10, mycolor));
		nodes.push_back(Sprite(10, 10, mycolor));
	}

	void draw();
	void bodyMove(); // 蛇的身体移动
	bool collision(const Sprite& other); // 判断碰撞
	bool collisionWall(); // 判断碰撞墙
	bool collisionSelf(); // 判断碰撞自己
	void increment(); // 蛇增加一节
	int getLength();

	// 便利访问头部
	Sprite& head() { return nodes.front(); }
	const Sprite& head() const { return nodes.front(); }

	// 返回以网格为单位的各节坐标，编码为 (gx<<6 | gy)
	std::vector<int> getGridCells() const;
};

