#ifndef CLICKLABEL_H
#define CLICKLABEL_H

#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <QPropertyAnimation>

class ClickLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int rotation READ rotation WRITE setRotation)

    /**
     * @brief setRotation 为了使用属性动画QPropertyAnimation(this, "rotation")
     * @param rotation
     */
    void setRotation(int rotation);

    /**
     * @brief rotation 为了使用属性动画QPropertyAnimation(this, "rotation")
     * @return
     */
    int rotation() const;

public:
    explicit ClickLabel(QWidget *parent = nullptr);
    QPoint mousePos;
    QPropertyAnimation *animation;


signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
};

#endif // CLICKLABEL_H
