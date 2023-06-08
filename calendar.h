#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QCalendarWidget>
#include <QLCDNumber>
#include <QTimer>
#include <QVBoxLayout>

class Calendar : public QWidget
{
    Q_OBJECT
public:
    explicit Calendar(QWidget *parent = nullptr);
    ~Calendar();

private:
//    QWidget *mainwidget;
    QCalendarWidget *calendar;
    QLCDNumber *number;
    QTimer *timer;
//    void resizeEvent(QResizeEvent *event);

private slots:
    void ontimeoutslot();
    void selectchangedslot();

};

#endif // CALENDAR_H
