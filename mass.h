/**
 * Bajo la licencia GPL-3 ó superior.
 *
 * Copyright (C) 2008 Rafael Fernández López
 */

#ifndef MASS_H
#define MASS_H

#include "vector2d.h"
#include "surface.h"

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QRadialGradient>
#include <QtGui/QStyleOptionGraphicsItem>

class Mass;

class Arrow
    : public QGraphicsItem
{
public:
    Arrow(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0, Mass *source = 0, Mass *dest = 0)
        : QGraphicsItem(parent, scene)
        , source(source)
        , dest(dest)
    {
    }

    ~Arrow()
    {
    }

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    virtual QPainterPath shape() const
    {
        QPainterPath path;
        path.addRect(boundingRect());
        return path;
    }

private:
    Mass *source;
    Mass *dest;
};

class Mass
    : public QGraphicsItem
{
public:
    Mass(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0, bool isDragMass = false)
        : QGraphicsItem(parent, scene)
        , massRadio(5)
        , surface(dynamic_cast<Surface*>(scene))
        , beingDragged(false)
        , isDragMass(isDragMass)
    {
        setFlag(QGraphicsItem::ItemIsMovable);

        Q_ASSERT_X(surface, "Mass", "You need to pass a Surface as the second parameter to the "
                                    "mass object");
        pos = QPoint(random() % 400, random() % 400);
        movement.x = random() % 3;
        movement.y = random() % 3;
        futureMovement = movement;
        if (!isDragMass) {
            setPos(movement.x, movement.y);
        }
    }

    ~Mass()
    {
    }

    Vector2D movementVector() const
    {
        return movement;
    }

    // Reimplemented from QGraphicsItem
    virtual void advance (int phase)
    {
        if (beingDragged || isDragMass) {
            return;
        }

        if (!phase) {
            futureMovement.x *= surface->friction();
            futureMovement.y *= surface->friction();
            pos.setX(pos.x() + futureMovement.x);
            pos.setY(pos.y() + futureMovement.y);
            if (pos.y() <= (massRadio * 2) || pos.y() >= (surface->height() - massRadio * 2)) {
                futureMovement.y *= -1;
            }
            if (pos.x() <= (massRadio * 2) || pos.x() >= (surface->width() - massRadio * 2)) {
                futureMovement.x *= -1;
            }
        } else {
            movement = futureMovement;
            setPos(pos.x(), pos.y());
        }
    }

    virtual QRectF boundingRect() const
    {
        return QRectF(-massRadio, -massRadio, massRadio * 2, massRadio * 2);
    }

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        painter->save();
        if (isDragMass) {
            painter->setOpacity(0.5);
        }
        QRadialGradient grad(0, 0, massRadio);
        grad.setColorAt(0, Qt::transparent);
        grad.setColorAt(1, Qt::yellow);
        painter->setBrush(grad);
        painter->drawRoundedRect(-massRadio, -massRadio, massRadio * 2, massRadio * 2, massRadio, massRadio);
        painter->restore();
    }

    virtual QPainterPath shape() const
    {
        QPainterPath path;
        path.addEllipse(boundingRect());
        return path;
    }

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        dragMass = new Mass(parentItem(), const_cast<Surface*>(surface), true);
        dragMass->setPos(event->scenePos());
        dragArrow = new Arrow(parentItem(), const_cast<Surface*>(surface), this, dragMass);
        dragArrow->setPos(event->scenePos());
        beingDragged = true;
        dragPoint = event->scenePos();
        QGraphicsItem::mousePressEvent(event);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        delete dragMass;
        delete dragArrow;
        QGraphicsItem::mouseReleaseEvent(event);
        movement.x = (event->scenePos().x() - dragPoint.x()) / 50.0;
        movement.y = (event->scenePos().y() - dragPoint.y()) / 50.0;
        futureMovement = movement;
        pos = event->scenePos();
        setPos(event->scenePos());
        beingDragged = false;
    }

private:
    QPointF pos;
    Vector2D movement;
    Vector2D futureMovement;
    const int massRadio;
    const Surface *surface;
    bool beingDragged;
    QPointF dragPoint;
    Mass *dragMass;
    Arrow *dragArrow;
    bool isDragMass;
};

QRectF Arrow::boundingRect() const
{
    QPointF topLeft(qMin(source->scenePos().x(), dest->scenePos().x()),
                    qMin(source->scenePos().y(), dest->scenePos().y()));

    QPointF bottomRight(qMax(source->scenePos().x(), dest->scenePos().x()),
                        qMax(source->scenePos().y(), dest->scenePos().y()));

    return QRectF(topLeft, bottomRight);
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::red);
    painter->drawLine(option->rect.topLeft(), option->rect.bottomRight());
}

#endif
