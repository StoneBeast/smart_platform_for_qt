#include "smarthome.h"

SmartHome::SmartHome(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 400);
    this->setMinimumSize(800, 480);

    slide = new MySlide(this);
    slide->resize(this->size());

    visibleNetwork = new VisibleNetwork(this);
    slide->addPage(visibleNetwork);

    calendarpage = new Calendar(this);
    slide->addPage(calendarpage);

    weatherpage = new Weather(this);
    slide->addPage(weatherpage);
}

SmartHome::~SmartHome()
{
}

void SmartHome::resizeEvent(QResizeEvent *ev) {
    Q_UNUSED(ev);
    slide->resize(this->size());
}

