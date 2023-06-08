#include "clicklabel.h"


ClickLabel::ClickLabel(QWidget *parent)
    :QLabel (new QLabel(parent))
{
    qDebug() << "create";
}


void ClickLabel::mousePressEvent(QMouseEvent *ev)
{
    //mousePos = QPoint(ev->position().x(), ev->position().y());
    mousePos = QPoint(ev->x(), ev->y());
}

void ClickLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    //if(mousePos == QPoint(ev->position().x(), ev->position().y()))
    if(mousePos == QPoint(ev->x(), ev->y())){

        emit clicked();
    }


}
