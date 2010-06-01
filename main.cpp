/**
 * Bajo la licencia GPL-3 ó superior.
 *
 * Copyright (C) 2008 Rafael Fernández López
 */

#include "surface.h"
#include "mass.h"

#include <QtCore/QTime>

#include <QtGui/QApplication>
#include <QtGui/QGraphicsView>
#include <QtGui/QBoxLayout>

Surface *surface = 0;

class MyWidget
    : public QWidget
{
public:
    MyWidget(QWidget *parent = 0)
        : QWidget(parent)
    {
    }

    ~MyWidget()
    {
    }

protected:
    void resizeEvent(QResizeEvent *event)
    {
        QWidget::resizeEvent(event);
        if (surface) {
            surface->setSceneRect(layout()->contentsRect());
        }
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    srandom(QTime::currentTime().msec());

    QWidget *main = new MyWidget(0);
    main->setMinimumSize(800, 600);
    QVBoxLayout *layout = new QVBoxLayout;
    main->setLayout(layout);

    QGraphicsView *view = new QGraphicsView(main);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHint(QPainter::Antialiasing);
    view->setResizeAnchor(QGraphicsView::NoAnchor);
    surface = new Surface(view);

    for (int i = 0; i < 5; ++i) {
        Mass *mass = new Mass(0, surface);
    }

    view->setScene(surface);

    layout->addWidget(view);

    main->show();

    return app.exec();
}
