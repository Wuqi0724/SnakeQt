#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QRandomGenerator>

//Widget 是整个游戏的主窗口和控制中心，负责管理游戏状态、分数、食物生成、定时器以及键盘和按钮交互。

//构造函数
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    pushToBoard();
    m_timer = new QTimer(this);
    m_timer->setInterval(160);

    connect(m_timer, &QTimer::timeout,
            this, &Widget::onTick);

    m_timer->start();

    setWindowTitle(QStringLiteral("贪吃蛇 - 序号：02 - 姓名：吴海舰"));
    setFixedSize(
        BoardWidget::COLS * BoardWidget::CELL,
        BoardWidget::TOP + BoardWidget::ROWS * BoardWidget::CELL + 36
    );
    setFocusPolicy(Qt::StrongFocus);

    // 防止按钮抢走空格键
    ui->btnStart->setFocusPolicy(Qt::NoFocus);
    ui->btnPause->setFocusPolicy(Qt::NoFocus);
    ui->btnReset->setFocusPolicy(Qt::NoFocus);

    // 让主窗口接收键盘事件
    setFocus();

}

Widget::~Widget()
{
    delete ui;
}


//开始
void Widget::on_btnStart_clicked()
{
    if (m_gameOver)
        return;

    m_started = true;
    m_paused = false;

    ui->btnPause->setText(QStringLiteral("暂停"));

    setFocus();
    pushToBoard();
}


//暂停和继续
void Widget::on_btnPause_clicked()
{
    if (!m_started || m_gameOver)
        return;

    m_paused = !m_paused; //切换true false

    if (m_paused)
        ui->btnPause->setText(QStringLiteral("继续"));
    else
        ui->btnPause->setText(QStringLiteral("暂停"));

    setFocus();
    pushToBoard();
}


//重开
void Widget::on_btnReset_clicked()
{
    m_snake = Snake();
    m_food = Food(8, 15);
    m_score = 0;
    m_started = false;
    m_paused = false;
    m_gameOver = false;

    ui->btnPause->setText(QStringLiteral("暂停"));

    ui->btnStart->setEnabled(true);
    ui->btnPause->setEnabled(true);

    setFocus();
    pushToBoard();
}

//WASD控制
void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
    case Qt::Key_W:
        m_snake.setDirection(Direction::Up);
        break;

    case Qt::Key_Down:
    case Qt::Key_S:
        m_snake.setDirection(Direction::Down);
        break;

    case Qt::Key_Left:
    case Qt::Key_A:
        m_snake.setDirection(Direction::Left);
        break;

    case Qt::Key_Right:
    case Qt::Key_D:
        m_snake.setDirection(Direction::Right);
        break;

    case Qt::Key_Space:

        if (m_gameOver)
            break;

        // 第一次按空格：开始游戏
        if (!m_started)
        {
            m_started = true;
            m_paused = false;
        }
        else
        {
            // 游戏已经开始：暂停 / 继续
            m_paused = !m_paused;
        }

        if (m_paused)
            ui->btnPause->setText(QStringLiteral("继续"));
        else
            ui->btnPause->setText(QStringLiteral("暂停"));

        break;

    case Qt::Key_R:
        m_snake = Snake();
        m_food = Food(8, 15);
        m_score = 0;

        m_started = false;
        m_paused = false;
        m_gameOver = false;

        ui->btnPause->setText(QStringLiteral("暂停"));

        ui->btnStart->setEnabled(true);
        ui->btnPause->setEnabled(true);

        break;

    default:
        QWidget::keyPressEvent(event);
        return;
    }

    pushToBoard();
}


void Widget::onTick()
{
    // 暂停或游戏结束，都不再移动
    if (!m_started || m_paused || m_gameOver)
        return;

    // 判断下一步是否会吃到食物
    bool grow = m_snake.willEatFood(m_food.row(), m_food.col());

    // 判断下一步是否撞墙或撞自己
    if (m_snake.willHitWall(BoardWidget::ROWS, BoardWidget::COLS)
        || m_snake.willHitSelf(grow))
    {
        m_gameOver = true;
        m_paused = true;

        ui->btnPause->setText(QStringLiteral("游戏结束"));

        ui->btnStart->setEnabled(false);
        ui->btnPause->setEnabled(false);

        pushToBoard();
        return;
    }

    // 安全才移动
    m_snake.move(grow);

    // 吃到食物后生成下一颗
    if (grow)
    {
        m_score += 10;
        generateFood();
    }

    pushToBoard();
}




void Widget::generateFood()
{
    // 如果蛇已经占满棋盘，就没有位置生成食物了
    if (m_snake.body().size() >=
        static_cast<size_t>(BoardWidget::ROWS * BoardWidget::COLS))
    {
        return;
    }

    int row;
    int col;
    bool onSnake;

    do
    {
        row = QRandomGenerator::global()->bounded(BoardWidget::ROWS);
        col = QRandomGenerator::global()->bounded(BoardWidget::COLS);

        onSnake = false;

        // 检查随机位置是否被蛇身占据
        for (const SnakeBody &part : m_snake.body())
        {
            if (part.row() == row && part.col() == col)
            {
                onSnake = true;
                break;
            }
        }

    } while (onSnake);

    m_food = Food(row, col);
}



void Widget::pushToBoard()
{
    ui->board->setSnake(m_snake.body());
    ui->board->setFood(m_food.row(), m_food.col());

    ui->board->setPaused(m_paused);
    ui->board->setGameOver(m_gameOver);
    ui->board->setStarted(m_started);

    ui->board->setScore(m_score);
}
