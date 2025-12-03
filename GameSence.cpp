#include "GameSence.h"
#include <iostream>
#include <chrono>

void GameSence::onMsg(const ExMessage& msg)
{
	//如果有键盘消息
	if (msg.message == WM_KEYDOWN)//msg.message为消息类型，WM_KEYDOWN反应按键
	{
		//判断具体按键virtual code 虚拟键码
		switch (msg.vkcode)
		{
		case VK_UP:
			if (snake.dir != VK_DOWN)
				snake.dir = msg.vkcode;
			break;
		case VK_DOWN:
			if (snake.dir != VK_UP)
				snake.dir = msg.vkcode;
			break;
		case VK_RIGHT:
			if (snake.dir != VK_LEFT)
				snake.dir = msg.vkcode;
			break;
		case VK_LEFT:
			if (snake.dir != VK_RIGHT)
				snake.dir = msg.vkcode;
			break;
		}

	}
}
void GameSence::run()
{
	// 1) 处理输入消息（尽早处理以减少输入延迟）
	ExMessage msg = { 0 };
	while (peekmessage(&msg, EX_KEY))//如果有消息
	{
		onMsg(msg);
	}

	// 2) 更新（按时间间隔移动蛇并检测）
	auto now = std::chrono::steady_clock::now();
	auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMoveTime).count();
	if (elapsedMs >= moveIntervalMs)
	{
		// 在移动前从地图中移除蛇的尾部标记
		if (snake.getLength() > 0) {
			auto tailCells = snake.getGridCells();
			int tailPacked = tailCells.back();
			int tgx = (tailPacked >> 6);
			int tgy = (tailPacked & 0x3F);
			if (inBounds(tgx,tgy)) m_mapGrid[idx(tgx,tgy)] = 0;
		}

		// 移动蛇就是改变蛇的坐标
		snake.bodyMove();

		// 在地图中添加新的蛇头标记
		auto head = snake.head();
		int hgx = head.getX() / 10;
		int hgy = head.getY() / 10;
		if (inBounds(hgx,hgy)) m_mapGrid[idx(hgx,hgy)] = 2; // 2 表示蛇

		// 各种检测
		snakeEatFood();
		snakeStrikeWall();
		snakeHeadStrikeBody();
		lastMoveTime = now;
	}

	// 3) 渲染（双缓冲）
	BeginBatchDraw();
	cleardevice();
	wall.setWall();
	snake.draw();
	food.draw();
	displaySurviveTime();
	displayPoints();
	EndBatchDraw();
}

void GameSence::snakeEatFood()
{
	// 使用地图判断是否吃到食物
	auto head = snake.head();
	int hgx = head.getX() / 10;
	int hgy = head.getY() / 10;
	int val = getMapData(hgx,hgy);
	if (val == 3) // 3 表示食物
	{
		// 吃到食物，节数增加
		snake.increment();
		// 从地图上移除旧食物标记并重置食物位置
		if (inBounds(hgx,hgy)) m_mapGrid[idx(hgx,hgy)] = 0;
		food.changeFoodPos();
		// 将新的 food 位置加入地图
		auto fcellx = food.getX()/10;
		auto fcelly = food.getY()/10;
		if (inBounds(fcellx,fcelly)) m_mapGrid[idx(fcellx,fcelly)] = 3;
	}
}
void GameSence::snakeStrikeWall()
{
	// 使用全局地图判断碰撞
	const Sprite& head = snake.head();
	int hgx = head.getX() / 10;
	int hgy = head.getY() / 10;
	int val = getMapData(hgx,hgy);
	if (val == 1) // 1 表示墙
	{
		std::cout << "撞墙辣！";
		exit(0);
	}

}
void GameSence::snakeHeadStrikeBody()
{
	const Sprite& head = snake.head();
	int hgx = head.getX() / 10;
	int hgy = head.getY() / 10;
	int val = getMapData(hgx,hgy);
	// 如果当前位置是蛇且不是头自身的旧位置（getLength>1 且在 map 中存在且标识为蛇）则撞到自己
	if (val == 2)
	{
		// 需要额外判断是否该格子是头自身（因为我们在移动前已经移除了尾部标记）
		// 为简化，若 val==2 且蛇体长度>1，认为撞到自己
		if (snake.getLength() > 1) {
			std::cout << "撞到自己辣！";
			exit(1);
		}
	}
}

void GameSence::displaySurviveTime()
{
	time_t currentTime = time(nullptr); // 获取当前时间
	double elapsed = difftime(currentTime, startTime); // 计算存活时间（秒）
	// 转换存活时间为字符串
	TCHAR timeStr[50];
	swprintf_s(timeStr, _T("存活时间:%.0f"), elapsed); // 使用 _stprintf 将 double 类型转换为字符串
	// 设置文本颜色
	settextcolor(WHITE);
	// 设置文本大小
	settextstyle(15, 15, L"微软雅黑");
	// 在屏幕上绘制文本
	outtextxy(500, 50, timeStr);
}

void GameSence::displayPoints()
{
	//读取当前的蛇的节数，算出积分
	int length = snake.getLength() - 3;
	// 转换为字符串
	TCHAR lengthstr[50];
	swprintf_s(lengthstr, _T("当前得分:%.0d "), length); // 使用 _stprintf 将 double 类型转换为字符串
	// 设置文本颜色
	settextcolor(LIGHTMAGENTA);
	// 设置文本大小
	settextstyle(15, 15, L"微软雅黑");
	// 在屏幕上绘制文本
	outtextxy(500, 100, lengthstr);
}

void GameSence::showStartScreen() {
	cleardevice(); // 清空屏幕
	setfillcolor(LIGHTCYAN);
	fillrectangle(0, 0, 640, 480); // 填充整个窗口背景
	settextcolor(BROWN); // 设置文字颜色

	settextstyle(40, 30, L"微软"); // 设置文字样式
	wchar_t s0[] = L"欢迎使用贪吃蛇定制版";
	outtextxy(20, 0, s0);

	settextstyle(40, 30, L"微软"); // 设置文字样式
	TCHAR s1[] = _T("请按回车键开始游戏");
	outtextxy(20, 50, s1);

	settextstyle(40, 30, L"微软"); // 设置文字样式
	TCHAR s2[] = _T("按ESC退出游戏");
	outtextxy(20, 90, s2);
}