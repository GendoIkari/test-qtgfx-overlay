#pragma once

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QWidget>

enum OverlayMode {
    Idle,
    Edit,
    Creating,
};

class RectItem : public QGraphicsRectItem {
public:
    RectItem(QGraphicsItem* parent = nullptr);
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
};

class Overlay : public QGraphicsView {
    Q_OBJECT
public:
    OverlayMode mode;

    Overlay(QWidget* page, QWidget* parent = nullptr);
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;

private:
    QWidget* page;
    QWidget* eventTarget();
    RectItem* editedItem = nullptr;
};
