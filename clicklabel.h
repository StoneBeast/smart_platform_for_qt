#ifndef CLICKLABEL_H
#define CLICKLABEL_H

#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <QPropertyAnimation>

class ClickLabel : public QLabel
{
    Q_OBJECT


public:
    explicit ClickLabel(QWidget *parent = nullptr);
    QPoint mousePos;


signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
};

#endif // CLICKLABEL_H
