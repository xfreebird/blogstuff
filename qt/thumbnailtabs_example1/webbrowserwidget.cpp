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

 #include <QtWebKit>

#include "webbrowserwidget.h"
#include "ui_webbrowserwidget.h"

WebBrowserWidget::WebBrowserWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WebBrowserWidget)
{
    ui->setupUi(this);

    connect(ui->goButton, SIGNAL(clicked()), this, SLOT(openUrl()));
    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(onLoaded()));
    connect(ui->webView, SIGNAL(selectionChanged()), this, SLOT(onLoaded()));
}

void WebBrowserWidget::openUrl() {
    QString url = ui->addressBar->text();

    if (!url.startsWith("http://")) url = "http://" + url;

    ui->webView->load(QUrl(url));
}

void WebBrowserWidget::onLoaded() {
    emit contentChanged();
}

WebBrowserWidget::~WebBrowserWidget()
{
    delete ui;
}
