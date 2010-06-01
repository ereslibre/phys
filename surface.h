/**
 * Bajo la licencia GPL-3 ó superior.
 *
 * Copyright (C) 2008 Rafael Fernández López
 */

#ifndef SURFACE_H
#define SURFACE_H

#include <QtCore/QTimer>

#include <QtGui/QGraphicsScene>

class Surface
    : public QGraphicsScene
{
public:
    Surface(QObject *parent = 0)
        : QGraphicsScene(parent)
        , m_friction(0.99)
    {
        QTimer *timer = new QTimer(this);
        timer->setInterval(10);
        connect(timer, SIGNAL(timeout()), this, SLOT(advance()));
        timer->start();
    }

    ~Surface()
    {
    }

    void setFriction(float friction)
    {
        m_friction = friction;
    }

    float friction() const
    {
        return m_friction;
    }

private:
    float m_friction;
};

#endif
