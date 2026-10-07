#include "square.h"


Square::Square(int row, int col)
    : m_row(row), m_col(col)//初始化列表
{

}

Square::~Square()
{

}

int Square::row() const
{
    return m_row;
}

int Square::col() const
{
    return m_col;
}


// ================= 蛇身 =================
SnakeBody::SnakeBody(int row, int col)
    :Square(row,col)
{

}


std::string SnakeBody::name() const
{
    return "蛇身";
}

char SnakeBody::symbol() const
{
    return 'o';
}

// ================= 食物 =================
Food::Food(int row, int col)
    :Square(row,col)
{

}


std::string Food::name() const
{
    return "食物";
}

char Food::symbol() const
{
    return '*';
}

// ================= 墙 =================
Wall::Wall(int row, int col)
    :Square(row,col)
{

}


std::string Wall::name() const
{
    return "墙";
}

char Wall::symbol() const
{
    return '#';
}
