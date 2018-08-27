#pragma once

#include "overlay.h"
#include <QEvent>
#include <QWebEngineView>

class Page : public QWebEngineView {
public:
    Page(QWidget* parent = nullptr);
};
