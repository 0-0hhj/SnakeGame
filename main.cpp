#include <iostream>
#include <graphics.h>
#include "GameSence.h"

int main()
{
	initgraph(640, 480, EX_SHOWCONSOLE);
	srand(time(nullptr));//初始化随机数种子
	GameSence scene;
	scene.showStartScreen();
	ExMessage msg1;
	while (true) {
		if (peekmessage(&msg1, EX_KEY)) {
			if (msg1.message == WM_KEYDOWN) {
				if (msg1.vkcode == VK_RETURN) {
					// 按下 Enter 键，开始游戏
					break;
				}
				else if (msg1.vkcode == VK_ESCAPE) {
					// 按下 Esc 键，退出程序
					std::cout << "你真退啊";
					exit(0);
				}
			}
		}
	}

	while (1)
	{
		scene.run();
		// 主循环保持较短的休眠以提高响应性，实际移动由 GameSence 的计时器控制
		Sleep(15);
	}

	return 0;
}