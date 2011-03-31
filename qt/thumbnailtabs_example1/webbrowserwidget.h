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

#ifndef WEBBROWSERWIDGET_H
#define WEBBROWSERWIDGET_H

#include <QWidget>

namespace Ui {
    class WebBrowserWidget;
}

class WebBrowserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WebBrowserWidget(QWidget *parent = 0);
    ~WebBrowserWidget();

private:
    Ui::WebBrowserWidget *ui;

private slots:
    void openUrl();
    void onLoaded();

signals:
    void contentChanged();
};

#endif // WEBBROWSERWIDGET_H
