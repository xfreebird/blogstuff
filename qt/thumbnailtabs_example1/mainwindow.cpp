/***************************************************************************
 *   Copyright (C) 2011 by Nicolae Ghimbovschi                             *
 *     nicolae.ghimbovschi@gmail.com                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 3 of the License.               *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "webbrowserwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    mTabs = TabsManager::getInstance();
    mTabs->setParentWidget(this);

    connect(mTabs, SIGNAL(isReady()), this, SLOT(onTaskbarIsReady()));

    connect(mTabs, SIGNAL(onTabClicked(QWidget*)), this, SLOT(onTabActivate(QWidget*)));
    connect(mTabs, SIGNAL(onTabHover(QWidget*)), this, SLOT(onTabActivate(QWidget*)));
    connect(mTabs, SIGNAL(onTabClose(QWidget*)), this, SLOT(onTabRemove(QWidget*)));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabActivate(int)));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabRemove(int)));

    connect(ui->actionAdd_Tab, SIGNAL(triggered()), this, SLOT(onTabAdd()));

}

void MainWindow::updateTabView() {

    QWidget* sender = dynamic_cast<QWidget*>(QObject::sender());

    mTabs->invalidateTabThumbnail(sender);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    for (int index = 0; index < ui->tabWidget->count(); index++) {
        mTabs->invalidateTabThumbnail(ui->tabWidget->widget(index));
    }
}

void MainWindow::onTaskbarIsReady() {

    ui->tabWidget->addTab(new WebBrowserWidget(), "Tab 1");
    ui->tabWidget->addTab(new WebBrowserWidget(), "Tab 2");
    ui->tabWidget->addTab(new WebBrowserWidget(), "Tab 3");

    mTabs->addTab(ui->tabWidget->widget(0), QString("Tab 1"), QIcon(":/home.png"), QPixmap());
    mTabs->addTab(ui->tabWidget->widget(1), QString("Tab 2"), QIcon(":/flag.png"), QPixmap());
    mTabs->addTab(ui->tabWidget->widget(2), QString("Custom thumbnail"), QIcon(":/feed.png"), QPixmap(":/logo.png"));

    connect((WebBrowserWidget*)ui->tabWidget->widget(0), SIGNAL(contentChanged()), this, SLOT(updateTabView()));
    connect((WebBrowserWidget*)ui->tabWidget->widget(1), SIGNAL(contentChanged()), this, SLOT(updateTabView()));
    connect((WebBrowserWidget*)ui->tabWidget->widget(2), SIGNAL(contentChanged()), this, SLOT(updateTabView()));

    mTabs->setActiveTab(ui->tabWidget->widget(0));
}

void MainWindow::onTabActivate(QWidget* widget) {
    activateWindow();

    for (int index = 0; index < ui->tabWidget->count(); index++) {
        if (ui->tabWidget->widget(index) == widget) {
            ui->tabWidget->setCurrentIndex(index);
            break;
        }
    }

}

void MainWindow::onTabRemove(QWidget* widget) {
    for (int index = 0; index < ui->tabWidget->count(); index++) {
        if (widget == ui->tabWidget->widget(index)) {
            onTabRemove(index);
            break;
        }
    }
}

void MainWindow::onTabActivate(int index) {
    QWidget* widget = ui->tabWidget->widget(index);
    if (widget) mTabs->setActiveTab(widget);
}

void MainWindow::onTabRemove(int index) {
    QWidget* widget = ui->tabWidget->widget(index);

    disconnect(widget, SIGNAL(contentChanged()), this, SLOT(updateTabView()));

    ui->tabWidget->removeTab(index);
    mTabs->removeTab(widget);
}

void MainWindow::onTabAdd() {
    QWidget* widget = new WebBrowserWidget();
    connect((WebBrowserWidget*)widget, SIGNAL(contentChanged()), this, SLOT(updateTabView()));

    ui->tabWidget->addTab(widget, "New Tab");
    mTabs->addTab(widget, "New Tab", QIcon(), QPixmap());
}


MainWindow::~MainWindow()
{
    TabsManager::releaseInstance();
    delete ui;
}
