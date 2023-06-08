#ifndef SMARTHOME_H
#define SMARTHOME_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include "myslide.h"
#include "calendar.h"
#include "weather.h"
#include "visiblenetwork.h"

class SmartHome : public QMainWindow
{
    Q_OBJECT

public:
    SmartHome(QWidget *parent = nullptr);
    ~SmartHome();

private:
    MySlide *slide;
    Calendar *calendarpage;
    Weather  *weatherpage;
    VisibleNetwork *visibleNetwork;
    void resizeEvent(QResizeEvent *ev);
};
#endif // SMARTHOME_H
