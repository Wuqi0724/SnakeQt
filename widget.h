#ifndef WIDGET_H
#define WIDGET_H

#include "boardwidget.h"
#include "snake.h"

#include <QWidget>
#include <QTimer>

class QPainter;
class QTimer;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:

    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_btnStart_clicked();

    void on_btnPause_clicked();

    void on_btnReset_clicked();

    void onTick();

private:

    void pushToBoard();
    void generateFood();

    Snake m_snake;
    Food m_food{8, 15};
    int m_score = 0;  // 当前得分

    QTimer *m_timer = nullptr;

    bool m_started = false;     // 是否已经开始本局游戏
    bool m_paused = false;      // 是否暂停
    bool m_gameOver = false;    // 是否游戏结束

    Ui::Widget *ui;

};
#endif // WIDGET_H
