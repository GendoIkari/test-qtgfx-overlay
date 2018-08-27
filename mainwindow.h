#pragma once

#include "overlay.h"
#include "page.h"
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent*) override;

private:
    Page* page = nullptr;
    Overlay* overlay = nullptr;

    void createBackground();
    void createForeground();
};
