#include "clicklabel.h"


ClickLabel::ClickLabel(QWidget *parent)
    :QLabel (new QLabel(parent))
{
    qDebug() << "create";
}


void ClickLabel::mousePressEvent(QMouseEvent *ev)
{
#ifdef Q_OS_WIN
    mousePos = QPoint(ev->position().x(), ev->position().y());
#else
    mousePos = QPoint(ev->x(), ev->y());
#endif
}

void ClickLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
#ifdef Q_OS_WIN
    if(mousePos == QPoint(ev->position().x(), ev->position().y())){
#else
    if(mousePos == QPoint(ev->x(), ev->y())){
#endif
        emit clicked();
    }


}
