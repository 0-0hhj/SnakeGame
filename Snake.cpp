#include"Snake.h"

static inline int packGrid(int gx, int gy) { return (gx << 6) | (gy & 0x3F); }

/*Snake method*/
void Snake::draw()
{
	for (size_t i = 0; i < nodes.size(); ++i)
	{
		nodes[i].draw();//调用了sprite的draw
	}
}

void Snake::bodyMove()
{
	if (nodes.empty()) return;

	// 计算新的头部位置（基于当前头部复制并移动）
	Sprite newHead = nodes.front();
	switch (dir)
	{
	case VK_UP://vkcode的上键的返回值的宏定义
		newHead.moveBy(0, -10);
		break;
	case VK_DOWN:
		newHead.moveBy(0, 10);
		break;
	case VK_LEFT:
		newHead.moveBy(-10, 0);
		break;
	case VK_RIGHT:
		newHead.moveBy(10, 0);
		break;

	}

	// 在头部插入新位置，尾部删除，保持长度不变
	nodes.push_front(newHead);
	nodes.pop_back();
}

bool Snake::collision(const Sprite& other)
{
	return nodes.front().collision(other);
}

bool Snake::collisionWall()
{
	const Sprite& head = nodes.front();
	int hx = head.getX();
	int hy = head.getY();
	// 如果头位于边界（与原实现等价的检查）
	if (hy == 0 || hy == 470 || hx == 0 || hx == 630)
	{
		return true;
	}
	return false;
}

void Snake::increment()
{
	// 增加一节，默认构造的 Sprite 会被放到尾部
	nodes.push_back(Sprite());
}

bool Snake::collisionSelf()
{
	if (nodes.size() <= 1) return false;
	for (size_t i = 1; i < nodes.size(); ++i)
	{
		if (nodes.front().collision(nodes[i]))
		{
			return true;
		}
	}
	return false;
}

int Snake::getLength()
{
	return static_cast<int>(nodes.size());
}

std::vector<int> Snake::getGridCells() const
{
	std::vector<int> cells;
	cells.reserve(nodes.size());
	for (const auto& n : nodes) {
		int gx = n.getX() / 10;
		int gy = n.getY() / 10;
		cells.push_back(packGrid(gx, gy));
	}
	return cells;
}

