#pragma once

#include <QWidget>
#include <qdesktopwidget.h>
#include <qtimer.h>
#include <QMouseEvent>
#include <qpoint.h>
#include "ui_SimpleWin.h"
#include "Lotter.h"
#include <qmessagebox.h>

class SimpleWin : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleWin(QWidget *parent, Lotter* lotter);
    ~SimpleWin();
    void Display();

private:
    Ui::SimpleWin ui;
    QWidget* parent;
    Lotter* lotter;
    QTimer* pQTimerDoubleClicked;
    int isDoubleClicked;
    bool moving;
    QPoint lastPos;

    enum class EdgeStatus{ EG_TOP , EG_LEFT , EG_RIGHT , EG_NORMAL };
    EdgeStatus edgeStatus = EdgeStatus::EG_NORMAL;
    int SHOW_EDGE_WIDTH = 10;

private slots:
    void timerSingleClick();

signals:
    void hideWin();

protected:
    void leaveEvent(QEvent* event);
    void enterEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
};
