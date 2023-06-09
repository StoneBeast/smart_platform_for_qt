#include "calendar.h"
#include <QDebug>


Calendar::Calendar(QWidget *parent)
    : QWidget{parent}
{
    this->resize(800, 480);
    this->setMinimumSize(800, 480);

//    mainwidget = new QWidget(this);
    //日历字号20
    QFont font;
    font.setPixelSize(20);
    calendar = new QCalendarWidget();
    calendar->setFont(font);
    //LCD数字8位，显示当前时间
    number = new QLCDNumber();
    number->setSegmentStyle(QLCDNumber::Flat);
    number->setDigitCount(8);
    QTime time = QTime::currentTime();//当前时间
    number->display(time.toString("HH:mm:ss"));
    //秒定时器，刷新LCD数字框
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, &Calendar::ontimeoutslot);
    connect(calendar, &QCalendarWidget::selectionChanged, this, &Calendar::selectchangedslot);
    //日历和LCD上下排列，LCD占比1，字符也会随之变大
    QVBoxLayout *caltabvbl = new QVBoxLayout();
    caltabvbl->addWidget(calendar, 4);
    caltabvbl->addWidget(number, 1);
    this->setLayout(caltabvbl);
}

Calendar::~Calendar() {}

void Calendar::ontimeoutslot() {
    QTime time = QTime::currentTime();
    number->display(time.toString("HH:mm:ss"));
}
void Calendar::selectchangedslot() {
    qDebug() <<__FILE__ << __LINE__ << "Date Selected: " << calendar->selectedDate().toString();
}

//void Calendar::resizeEvent(QResizeEvent *event)
//{
//    Q_UNUSED(event)
//    mainwidget->resize(this->size());
//}
