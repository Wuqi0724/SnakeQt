#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include "square.h"

// 蛇的移动方向
enum class Direction
{
    Up,
    Down,
    Left,
    Right
};

class Snake
{
public:
    Snake();

    // 蛇移动一步
    void move(bool grow = false);

    // 修改方向
    void setDirection(Direction dir);

    // 获取整条蛇，给外部查看
    const std::deque<SnakeBody>& body() const;

    //边界
    bool willHitWall(int rows, int cols) const;
    bool willEatFood(int foodRow, int foodCol) const;
    bool willHitSelf(bool grow) const;

private:
    std::deque<SnakeBody> m_body;  //整条蛇的身体
    Direction m_direction;         //当前准备移动方向
    Direction m_lastMoveDirection; //上一次蛇移动方向

    bool isOpposite(Direction a, Direction b) const; //判断反向
};

#endif // SNAKE_H
