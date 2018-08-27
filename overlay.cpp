#include "overlay.h"
#include <QCoreApplication>
#include <QDebug>
#include <QEvent>

RectItem::RectItem(QGraphicsItem* parent)
    : QGraphicsRectItem(parent)
{
    setBrush(QBrush(Qt::red));
    setFlag(ItemIsFocusable, false);
}

void RectItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    setBrush(QBrush(Qt::yellow));
    event->accept();
}

void RectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    setBrush(QBrush(Qt::red));
    event->accept();
}

Overlay::Overlay(QWidget* page, QWidget* parent)
    : QGraphicsView(parent)
    , page(page)
{
    setStyleSheet("background: transparent");
    setMouseTracking(true);

    auto scene = new QGraphicsScene(this);
    setScene(scene);
}

void Overlay::mousePressEvent(QMouseEvent* event)
{
    event->setAccepted(false);

    if (mode == Edit) {
        auto rect = new RectItem();
        editedItem = rect;
        auto pos = mapToScene(event->pos());
        rect->setPos(pos);
        rect->setRect(0, 0, 0, 0);
        scene()->addItem(rect);
        mode = Creating;
        return;
    } else if (mode == Creating) {
        editedItem = nullptr;
        mode = Edit;
        return;
    }

    QGraphicsView::mousePressEvent(event);

    if (event->isAccepted())
        return;

    QCoreApplication::postEvent(eventTarget(),
        new QMouseEvent{
            event->type(),
            event->pos(),
            event->button(),
            event->buttons(),
            Qt::NoModifier });
}

void Overlay::mouseReleaseEvent(QMouseEvent* event)
{
    event->setAccepted(false);

    QGraphicsView::mouseReleaseEvent(event);

    if (event->isAccepted())
        return;

    QCoreApplication::postEvent(eventTarget(),
        new QMouseEvent{
            event->type(),
            event->pos(),
            event->button(),
            event->buttons(),
            Qt::NoModifier });
}

void Overlay::mouseMoveEvent(QMouseEvent* event)
{
    if (mode == Creating) {
        auto old = editedItem->pos();
        auto pos = mapToScene(event->pos());
        editedItem->setRect(0, 0, pos.x() - old.x(), pos.y() - old.y());
        return;
    }

    QGraphicsView::mouseMoveEvent(event);

    QCoreApplication::postEvent(eventTarget(),
        new QMouseEvent{
            event->type(),
            event->pos(),
            event->button(),
            event->buttons(),
            Qt::NoModifier });

    setCursor(eventTarget()->cursor());
}

void Overlay::resizeEvent(QResizeEvent*)
{
    setSceneRect(0, 0, viewport()->size().width(), viewport()->size().height());
}

void Overlay::wheelEvent(QWheelEvent* event)
{
    QGraphicsView::wheelEvent(event);

    if (event->isAccepted())
        return;

    QCoreApplication::postEvent(eventTarget(),
        new QWheelEvent{
            event->pos(),
            event->globalPos(),
            event->pixelDelta(),
            event->angleDelta(),
            event->delta(),
            event->orientation(),
            event->buttons(),
            Qt::NoModifier });
}

void Overlay::focusInEvent(QFocusEvent*)
{
    eventTarget()->setFocus();
}

QWidget* Overlay::eventTarget()
{
    QWidget* target = nullptr;
    for (auto obj : page->children()) {
        target = qobject_cast<QWidget*>(obj);
        if (target) {
            Q_ASSERT(QString(obj->metaObject()->className()) == "QtWebEngineCore::RenderWidgetHostViewQtDelegateWidget");
            break;
        }
    }
    return target;
}
