#ifndef SQUARE_H
#define SQUARE_H

#include <string>

//地图格子的抽象基类，提供行列坐标等共同属性，并定义格子名称和符号的接口。SnakeBody、Food 和 Wall 都继承自 Square，实现各自的具体功能。

//地图格子的总图纸
class Square
{
public:
    Square(int row, int col);
    virtual ~Square();

    int row() const;
    int col() const;

    virtual std::string name() const = 0;//定义各自的格子样子，=0表示为纯虚函数不需要实现但必须有
    virtual char symbol() const = 0;


//表示这个格子在哪
protected://允许内部访问
    int m_row;
    int m_col;
};


class SnakeBody : public Square
{
public:
    SnakeBody(int row, int col);

    std::string name() const override;
    char symbol() const override;  //重写父类 Square 的 symbol()
};


class Food : public Square
{
public:
    Food(int row, int col);

    std::string name() const override;
    char symbol() const override;
};


class Wall : public Square
{
public:
    Wall(int row, int col);

    std::string name() const override;
    char symbol() const override;
};


#endif // SQUARE_H
