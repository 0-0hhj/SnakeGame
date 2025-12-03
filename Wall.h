#pragma once
#include <vector>
#include <graphics.h>
#include "Sprite.h"

// 墙类，使用组合而非继承
class Wall
{
private:
    COLORREF m_color;
    std::vector<int> m_wallRect;    //存储墙的矩形坐标 （上, 下, left, right）

    static inline int packGrid(int gx, int gy) {
        return (gx << 6) | (gy & 0x3F);
    }

public:
    Wall() : m_color(LIGHTGRAY), m_wallRect({0,48,0,48}) {}

    // 返回墙体在网格坐标的所有格子（packed int）
    std::vector<int> getWallGridCells() const
    {
        std::vector<int> cells;
        int left = m_wallRect[2];
        int right = m_wallRect[3];
        int top = m_wallRect[0];
        int bottom = m_wallRect[1];
        // 上边
        int gy = top;
        for (int gx = left; gx < right; ++gx) {
            cells.push_back(packGrid(gx, gy));
        }
        // 下边 (bottom-1 as drawing uses that)
        gy = bottom - 1;
        for (int gx = left; gx < right; ++gx) {
            cells.push_back(packGrid(gx, gy));
        }
        // 左边
        int gx = left;
        for (int gy2 = top; gy2 < bottom; ++gy2) {
            cells.push_back(packGrid(gx, gy2));
        }
        // 右边
        gx = right - 1;
        for (int gy2 = top; gy2 < bottom; ++gy2) {
            cells.push_back(packGrid(gx, gy2));
        }
        return cells;
    }

    void setWall()
    {
        setfillcolor(m_color);
        int m_x = 0;
        int m_y = 0;
        //上边墙和下边墙
        m_y = m_wallRect[0]*10;
        for (int i = m_wallRect[2]; i < m_wallRect[3]; i++)
        {
            m_x = i * 10;
            fillrectangle(m_x, m_y, m_x + 10, m_y + 10);
        }

        m_y = (m_wallRect[1]-1) * 10;
        for (int i = m_wallRect[2]; i < m_wallRect[3]; i++)
        {
            m_x = i * 10;
            fillrectangle(m_x, m_y, m_x + 10, m_y + 10);
        }

        //左边墙和右边墙
        m_x = m_wallRect[2] * 10;
        for (int i = m_wallRect[0]; i < m_wallRect[1]; i++)
        {
            m_y = i * 10;
            fillrectangle(m_x, m_y, m_x + 10, m_y + 10);
        }

        m_x = (m_wallRect[3] - 1) * 10;
        for (int i = m_wallRect[0]; i < m_wallRect[1]; i++)
        {
            m_y = i * 10;
            fillrectangle(m_x, m_y, m_x + 10, m_y + 10);
        }
    }

    // 修改墙的矩形范围
    void setWallRect(int top, int bottom, int left, int right)
    {
        m_wallRect = {top, bottom, left, right};
    }

};