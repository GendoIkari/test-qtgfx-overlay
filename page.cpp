#include "page.h"
#include <QCoreApplication>
#include <QDebug>
#include <QMouseEvent>

Page::Page(QWidget* parent)
    : QWebEngineView(parent)
{
}
