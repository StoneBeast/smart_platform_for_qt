#include "clicklabel.h"


ClickLabel::ClickLabel(QWidget *parent)
    :QLabel (parent)
{
    /*
     *  这里遇到一个问题：
     *      在编译时报错说没有提供相应的虚函数，但是在提供后仍然报错
     *      后来发现可能是qt自己的bug，重启了qmake之后九过了编译
     */
    qDebug() <<__FILE__ << __LINE__ << "create";
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
