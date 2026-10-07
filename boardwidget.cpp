#include "boardwidget.h"

#include <QFont>
#include <QLinearGradient>
#include <QPainter>

//独立的绘图控件，负责使用 QPainter 绘制棋盘、蛇身、食物、分数信息以及开始、暂停和游戏结束界面。

BoardWidget::BoardWidget(QWidget *parent)
    : QWidget{parent}
{
    setMinimumSize(COLS * CELL, TOP + ROWS * CELL);
}

int BoardWidget::boardBottom() const
{
    return TOP + ROWS * CELL;
}

QRectF BoardWidget::cellRect(int col, int row) const
{
    return QRectF(
        col * CELL,
        TOP + row * CELL,
        CELL,
        CELL
    );
}

void BoardWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    drawBoard(p);
    drawFood(p);
    drawSnake(p);
    drawInfoBar(p);

    drawStart(p);
    drawPaused(p);
    drawGameOver(p);
}


void BoardWidget::drawBoard(QPainter &p)
{
    p.setPen(Qt::NoPen);
    p.setBrush(QColor("#0b1016"));
    p.drawRect(rect());

    QLinearGradient bg(0, TOP, 0, boardBottom());
    bg.setColorAt(0.0, QColor("#16212e"));
    bg.setColorAt(1.0, QColor("#0d151d"));

    p.setBrush(bg);
    p.drawRect(QRectF(0, TOP, width(), ROWS * CELL));

    for (int col = 0; col < COLS; ++col)
    {
        for (int row = 0; row < ROWS; ++row)
        {
            p.fillRect(
                cellRect(col, row),
                (col + row) % 2 == 0
                    ? QColor("#1e2a38")
                    : QColor("#1a2431")
            );
        }
    }

    p.setPen(QPen(QColor("#22303f"), 1));

    for (int col = 1; col < COLS; ++col)
    {
        p.drawLine(
            QPointF(col * CELL + 0.5, TOP),
            QPointF(col * CELL + 0.5, boardBottom())
        );
    }

    for (int row = 1; row < ROWS; ++row)
    {
        p.drawLine(
            QPointF(0, TOP + row * CELL + 0.5),
            QPointF(width(), TOP + row * CELL + 0.5)
        );
    }

    p.setPen(QPen(QColor("#2f4256"), 2));
    p.setBrush(Qt::NoBrush);
    p.drawRect(QRectF(0, TOP, width(), ROWS * CELL));
}


void BoardWidget::drawSnake(QPainter &p)
{
    p.setPen(Qt::NoPen);
    p.setBrush(QColor("#7ee787"));

    for (const SnakeBody &part : m_snakeBody)
    {
        QRectF r = cellRect(part.col(), part.row())
                       .adjusted(1, 1, -1, -1);

        p.drawRoundedRect(r, 6, 6);
    }
}


//信息栏
void BoardWidget::drawInfoBar(QPainter &p)
{
    p.fillRect(0, 0, width(), TOP, QColor("#0d1117"));

    p.setPen(Qt::white);

    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    p.setFont(font);


    QString text = QStringLiteral("得分：%1    长度：%2")
                       .arg(m_score)
                       .arg(m_snakeBody.size());

    p.drawText(
        QRectF(8, 0, width() - 16, TOP),
        Qt::AlignVCenter | Qt::AlignLeft,
        text
    );
}


void BoardWidget::setSnake(const std::deque<SnakeBody> &body)
{
    m_snakeBody = body;
    update();
}

void BoardWidget::setPaused(bool paused)
{
    m_paused = paused;
    update();
}


//食物
void BoardWidget::drawFood(QPainter &p)
{
    QRectF r = cellRect(m_foodCol, m_foodRow)
                   .adjusted(3, 3, -3, -3);

    p.setPen(Qt::NoPen);
    p.setBrush(QColor("#ff5f56"));

    p.drawEllipse(r);
}
void BoardWidget::setFood(int row, int col)
{
    m_foodRow = row;
    m_foodCol = col;
    update();
}


void BoardWidget::setGameOver(bool gameOver)
{
    m_gameOver = gameOver;
    update();
}
void BoardWidget::drawGameOver(QPainter &p)
{
    if (!m_gameOver)
        return;

    // 半透明黑色遮罩
    p.fillRect(
        QRectF(0, TOP, width(), ROWS * CELL),
        QColor(0, 0, 0, 170)
    );

    QFont font;
    font.setPointSize(28);
    font.setBold(true);

    p.setFont(font);
    p.setPen(Qt::white);

    QString text = QStringLiteral("游戏结束\n最终得分：%1\n按 R 重新开始")
                       .arg(m_score);

    p.drawText(
        QRectF(0, TOP, width(), ROWS * CELL),
        Qt::AlignCenter,
        text
    );
}


void BoardWidget::setScore(int score)
{
    m_score = score;
    update();
}


void BoardWidget::drawPaused(QPainter &p)
{
    // 没有暂停，或者已经游戏结束，就不显示暂停画面
    if (!m_paused || m_gameOver)
        return;

    // 给棋盘盖上一层半透明黑色
    p.fillRect(
        QRectF(0, TOP, width(), ROWS * CELL),
        QColor(0, 0, 0, 140)
    );

    QFont font;
    font.setPointSize(26);
    font.setBold(true);

    p.setFont(font);
    p.setPen(Qt::white);

    p.drawText(
        QRectF(0, TOP, width(), ROWS * CELL),
        Qt::AlignCenter,
        QStringLiteral("游戏已暂停\n按空格继续")
    );
}


//开始界面
void BoardWidget::setStarted(bool started)
{
    m_started = started;
    update();
}
void BoardWidget::drawStart(QPainter &p)
{
    // 游戏已经开始，或者游戏已经结束，就不显示开始画面
    if (m_started || m_gameOver)
        return;

    // 半透明黑色遮罩
    p.fillRect(
        QRectF(0, TOP, width(), ROWS * CELL),
        QColor(0, 0, 0, 150)
    );

    QFont font;
    font.setPointSize(28);
    font.setBold(true);

    p.setFont(font);
    p.setPen(Qt::white);

    p.drawText(
        QRectF(0, TOP, width(), ROWS * CELL),
        Qt::AlignCenter,
        QStringLiteral("贪吃蛇\n按空格或点击开始游戏")
    );
}
