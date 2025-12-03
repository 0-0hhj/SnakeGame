#pragma once
#include <ctime>
#include <chrono>
#include <graphics.h>
#include "Snake.h"
#include "Food.h"
#include "Wall.h"
#include <vector>

// 游戏场景类
class GameSence
{
private:
	Snake snake;
	Food food;
	Wall wall;
	time_t startTime; // 游戏开始时间

	// 全局地图格子尺寸（单位：格），与项目一致
	static constexpr int GRID_W = 64;
	static constexpr int GRID_H = 48;

	// 全局地图数据：一维数组表示二维网格，索引为 gy*GRID_W + gx
	// value 为 int 类型代表不同实体(例如 0=空,1=wall,2=snake,3=food)
	std::vector<int> m_mapGrid;

	// 蛇移动计时控制
	std::chrono::steady_clock::time_point lastMoveTime;
	int moveIntervalMs; // 蛇移动间隔（毫秒）

	// 辅助：坐标检查与索引转换
	static inline bool inBounds(int gx, int gy) { return gx >= 0 && gx < GRID_W && gy >= 0 && gy < GRID_H; }
	static inline int idx(int gx, int gy) { return gy * GRID_W + gx; }

public:
	GameSence()
	{
		startTime = time(nullptr);
		lastMoveTime = std::chrono::steady_clock::now();
		moveIntervalMs = 150; // 默认 150 ms
		// 初始化格子并填充墙体数据
		m_mapGrid.assign(GRID_W * GRID_H, 0);
		auto wallCells = wall.getWallGridCells();
		for (int packed : wallCells) {
			int gx = (packed >> 6);
			int gy = (packed & 0x3F);
			if (inBounds(gx, gy)) m_mapGrid[idx(gx, gy)] = 1; // 1 表示墙
		}

		// 初始化食物位置到地图
		m_mapGrid[idx(food.getX()/10, food.getY()/10)] = 3;
		// 初始化蛇位置到地图
		auto snakeCells = snake.getGridCells();
		for (int packed : snakeCells) {
			int gx = (packed >> 6);
			int gy = (packed & 0x3F);
			if (inBounds(gx, gy)) m_mapGrid[idx(gx, gy)] = 2; // 2 表示蛇
		}
	};
	void run();
	void onMsg(const ExMessage& msg); // 响应消息：鼠标消息，键盘消息 
	void snakeEatFood();
	void snakeStrikeWall();
	void snakeHeadStrikeBody();
	void displaySurviveTime();
	void displayPoints();
	void showStartScreen();

	// 地图数据 API
	void setMapData(int gx, int gy, int value) { if (inBounds(gx,gy)) m_mapGrid[idx(gx, gy)] = value; }
	int getMapData(int gx, int gy) const { return (inBounds(gx,gy) ? m_mapGrid[idx(gx, gy)] : 0); }
	bool isMapData(int gx, int gy, int value) const { return inBounds(gx,gy) && m_mapGrid[idx(gx, gy)] == value; }
};