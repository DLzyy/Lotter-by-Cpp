#include "SimpleWin.h"

SimpleWin::SimpleWin(QWidget *p, Lotter* l)
    : QWidget(p)
{
    ui.setupUi(this);
    this->parent = p;
    this->setWindowFlags(Qt::SplashScreen | Qt::WindowStaysOnTopHint);
    this->lotter = l;
    isDoubleClicked = 0;
    moving = false;
    pQTimerDoubleClicked = new QTimer(this);
    connect(pQTimerDoubleClicked, SIGNAL(timeout()), this, SLOT(timerSingleClick()));
}

SimpleWin::~SimpleWin()
{
    delete pQTimerDoubleClicked;
}

void SimpleWin::Display() {
    lotter->StopRunning();
    lotter->SetUI(ui.mainButton, ui.outputLabel);
    show();
}

void SimpleWin::timerSingleClick() {
    isDoubleClicked = 0; 
    pQTimerDoubleClicked->stop(); 
}

void SimpleWin::leaveEvent(QEvent* event) {
    switch (edgeStatus) {
    case EdgeStatus::EG_TOP:
        setGeometry(pos().x(), -height() + SHOW_EDGE_WIDTH, width(), height());
        break;
    case EdgeStatus::EG_LEFT:
        setGeometry(-width() + SHOW_EDGE_WIDTH, pos().y(), width(), height());
        break;
    case EdgeStatus::EG_RIGHT:
        setGeometry(QApplication::desktop()->width() - SHOW_EDGE_WIDTH, pos().y(), width(), height());
        break;
    }
    return QWidget::leaveEvent(event);
}

void SimpleWin::enterEvent(QEvent* event) {
    switch (edgeStatus) {
    case EdgeStatus::EG_TOP:
        setGeometry(pos().x(), -SHOW_EDGE_WIDTH, width(), height());
        break;
    case EdgeStatus::EG_LEFT:
        setGeometry(-SHOW_EDGE_WIDTH, y(), width(), height());
        break;
    case EdgeStatus::EG_RIGHT:
        setGeometry(QApplication::desktop()->width() - width() + SHOW_EDGE_WIDTH, y(), width(), height());
        break;
    }
    return QWidget::enterEvent(event);
}

void SimpleWin::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDoubleClicked++;
        moving = true;
        lastPos = event->globalPos() - pos();
    }
    if (isDoubleClicked == 1)
        pQTimerDoubleClicked->start(430);
    else if (isDoubleClicked == 2) {
        isDoubleClicked = 0;
        pQTimerDoubleClicked->stop();
        hide();
        emit hideWin();
    }
    return QWidget::mousePressEvent(event);
}

void SimpleWin::mouseMoveEvent(QMouseEvent* event) {
    if (moving && (event->buttons() && Qt::LeftButton) && (event->globalPos() - lastPos).manhattanLength() > QApplication::startDragDistance()) {
        move(event->globalPos() - lastPos);
        lastPos = event->globalPos() - pos();
    }
    return QWidget::mouseMoveEvent(event);
}

void SimpleWin::mouseReleaseEvent(QMouseEvent* event) {
    if (y() <= 0) {
        edgeStatus = EdgeStatus::EG_TOP;
    }
    else if (QApplication::desktop()->width() <= (this->x() + width())) {
        edgeStatus = EdgeStatus::EG_RIGHT;
    }
    else if (this->x() <= 0) {
        edgeStatus = EdgeStatus::EG_LEFT;
    }
    else {
        edgeStatus = EdgeStatus::EG_NORMAL;
    }
    moving = false;
    return QWidget::mouseReleaseEvent(event);
}
