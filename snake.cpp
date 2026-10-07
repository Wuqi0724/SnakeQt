#include "snake.h"

//负责蛇自身的数据和行为规则，使用 deque 双端队列保存整条蛇，实现蛇的移动、转向、身体增长、撞墙预测以及自碰撞检测等功能。

Snake::Snake()
    : m_direction(Direction::Right), //初始方向向右
        m_lastMoveDirection(Direction::Right)
{
    m_body.push_back(SnakeBody(5,5)); //蛇头
    m_body.push_back(SnakeBody(5, 4)); // 身体
    m_body.push_back(SnakeBody(5, 3)); // 蛇尾
}



//接受方向
void Snake::setDirection(Direction dir)
{
    if (isOpposite(dir, m_lastMoveDirection))
    {
        return;
    }

    m_direction = dir;  //外面告诉蛇往哪走，就把方向记下来
}


//判断是否反向
bool Snake::isOpposite(Direction a, Direction b) const
{

    return ( a == Direction::Up     && b == Direction::Down) ||
           ( a == Direction::Down   && b == Direction::Up)   ||
           ( a == Direction::Left   && b == Direction::Right) ||
           ( a == Direction::Right  && b == Direction::Left);
}


//移动
void Snake::move(bool grow)
{
    int newRow = m_body[0].row();  //取出蛇头，获取蛇头当前所在的行。
    int newCol = m_body[0].col();

    //对方向进行响应
    switch (m_direction)
    {
    case Direction::Up:
        newRow--;
        break;

    case Direction::Down:
        newRow++;
        break;

    case Direction::Left:
        newCol--;
        break;

    case Direction::Right:
        newCol++;
        break;
    }

    // 添加新蛇头
    m_body.push_front(SnakeBody(newRow, newCol));

    // 只有没吃到食物时，才删除蛇尾
    if (!grow)
    {
        m_body.pop_back();
    }

    // 记录本次真正移动的方向
    m_lastMoveDirection = m_direction;
}


//便于外部代码查看蛇身体全部节点
const std::deque<SnakeBody>& Snake::body() const
{
    return m_body;
}

//边界
bool Snake::willHitWall(int rows, int cols) const
{
    const SnakeBody &head = m_body.front();

    int newRow = head.row();
    int newCol = head.col();

    switch (m_direction)
    {
    case Direction::Up:
        newRow--;
        break;

    case Direction::Down:
        newRow++;
        break;

    case Direction::Left:
        newCol--;
        break;

    case Direction::Right:
        newCol++;
        break;
    }

    return newRow < 0 ||
           newRow >= rows ||
           newCol < 0 ||
           newCol >= cols;
}


bool Snake::willEatFood(int foodRow, int foodCol) const
{
    const SnakeBody &head = m_body.front();

    int newRow = head.row();
    int newCol = head.col();

    switch (m_direction)
    {
    case Direction::Up:
        newRow--;
        break;

    case Direction::Down:
        newRow++;
        break;

    case Direction::Left:
        newCol--;
        break;

    case Direction::Right:
        newCol++;
        break;
    }

    return newRow == foodRow && newCol == foodCol;
}



bool Snake::willHitSelf(bool grow) const
{
    const SnakeBody &head = m_body.front();

    int newRow = head.row();
    int newCol = head.col();

    // 计算蛇头下一步的位置
    switch (m_direction)
    {
    case Direction::Up:
        newRow--;
        break;

    case Direction::Down:
        newRow++;
        break;

    case Direction::Left:
        newCol--;
        break;

    case Direction::Right:
        newCol++;
        break;
    }

    // 普通移动时，尾巴会让出位置
    // 吃到食物时，尾巴不会让出位置
    size_t count = m_body.size();

    if (!grow)
    {
        count--;
    }

    // 检查下一步是否撞到身体
    for (size_t i = 0; i < count; ++i)
    {
        if (m_body[i].row() == newRow &&
            m_body[i].col() == newCol)
        {
            return true;
        }
    }

    return false;
}
