#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <deque>

#include "square.h"


class QPainter;

class BoardWidget : public QWidget
{
    Q_OBJECT

public:
    static constexpr int COLS = 30;
    static constexpr int ROWS = 20;
    static constexpr int CELL = 28;
    static constexpr int TOP  = 40;

public:
    explicit BoardWidget(QWidget *parent = nullptr);

    void setSnake(const std::deque<SnakeBody> &body);
    void setPaused(bool paused);
    void setGameOver(bool gameOver);
    void setScore(int score);
    void setStarted(bool started);

    void setFood(int row, int col);


protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawBoard(QPainter &p);
    void drawSnake(QPainter &p);
    void drawInfoBar(QPainter &p);
    void drawFood(QPainter &p);
    void drawGameOver(QPainter &p);
    void drawPaused(QPainter &p);
    void drawStart(QPainter &p);

    int m_score = 0;

    QRectF cellRect(int col, int row) const;
    int boardBottom() const;

private:
    std::deque<SnakeBody> m_snakeBody;

    int m_foodRow = 8;
    int m_foodCol = 15;

    bool m_started = false;
    bool m_paused = false;
    bool m_gameOver = false;
};

#endif // BOARDWIDGET_H


//BoardWidget
//├─ COLS / ROWS / CELL / TOP    棋盘参数
//├─ setSnake()                  外面告诉它蛇在哪
//├─ setPaused()                 外面告诉它是否暂停
//├─ paintEvent()                真正开始画
//├─ drawBoard()                 画棋盘
//├─ drawSnake()                 画蛇
//└─ drawInfoBar()               画顶部信息
