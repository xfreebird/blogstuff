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

#ifndef TABSMANAGER_H
#define TABSMANAGER_H

#include <QList>
#include <QIcon>
#include <QString>
#include <QPixmap>
#include <QWidget>
#include <QCoreApplication>

#include "win7utils.h"

class TabsManager : public QObject
{
    Q_OBJECT
public:

    static TabsManager* getInstance();
    static void releaseInstance();
    static bool eventFilter(void *message_, long *result);

    void setParentWidget(QWidget* widget);

    void addTab(QWidget* widget, QString title, QIcon icon, QPixmap pixmap);
    void updateTab(QWidget* widget, QString title, QIcon icon, QPixmap pixmap);
    void setActiveTab(QWidget* widget);
    void removeTab(QWidget* widget);
    void invalidateTabThumbnail(QWidget* widget);


private:
    struct TaskbarTab {
        TaskbarTab() : m_widget(NULL), m_tab_widget(NULL) {}

        QPixmap  m_thumbnail;
        QWidget* m_widget;
        QWidget* m_tab_widget;
    };

    enum TABEVENT {
        TAB_CLICK = 0,
        TAB_CLOSE = 1,
        TAB_HOVER = 2
    };

    QWidget* m_parentWidget;
    QList<TaskbarTab*> m_tabs;
    ITaskbarList4* m_taskbarHandler;

    static TabsManager* m_instance;
    static QCoreApplication::EventFilter m_oldEventFilter;

    TabsManager();
    ~TabsManager();

    void allocTaskbar();
    void tabAction(WId id, TABEVENT action);
    TaskbarTab* findTabByWId(WId id, bool inserted = false);
    void setPeekBitmap(WId id, QSize size, bool isLive = false);

signals:
    void isReady();
    void onTabClicked(QWidget* widget);
    void onTabClose(QWidget* widget);
    void onTabHover(QWidget* widget);
};
#endif // TABSMANAGER_H
