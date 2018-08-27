#include "mainwindow.h"
#include <QDockWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setGeometry(0, 0, 1024, 768);
    auto centralWidget = new QWidget();
    centralWidget->setGeometry(0, 0, 1024, 768);
    centralWidget->setLayout(new QVBoxLayout(centralWidget));
    centralWidget->show();
    setCentralWidget(centralWidget);

    auto dockWidget = new QDockWidget(tr("Toolbar"), this);
    auto dockWidgetInside = new QWidget(dockWidget);
    auto button = new QPushButton(tr("Edit"), dockWidget);
    button->setCheckable(true);
    dockWidgetInside->setLayout(new QVBoxLayout(dockWidgetInside));
    dockWidgetInside->layout()->addWidget(button);
    dockWidgetInside->layout()->setAlignment(button, Qt::AlignTop);
    dockWidget->setWidget(dockWidgetInside);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockWidget->setFocusPolicy(Qt::NoFocus);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

    createBackground();
    createForeground();

    connect(button, &QPushButton::clicked, [=](bool checked) {
        overlay->mode = checked ? Edit : Idle;
    });
}

MainWindow::~MainWindow()
{
}

void MainWindow::resizeEvent(QResizeEvent*)
{
    if (overlay && page)
        overlay->setGeometry(page->geometry());
}

void MainWindow::createBackground()
{
    page = new Page(centralWidget());
    page->load(QUrl("http://www.google.com"));
    centralWidget()->layout()->addWidget(page);
    page->show();
}

void MainWindow::createForeground()
{
    overlay = new Overlay(page, centralWidget());
    overlay->setGeometry(page->geometry());
    overlay->show();
}
