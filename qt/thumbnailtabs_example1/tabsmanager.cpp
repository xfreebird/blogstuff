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
#include "tabsmanager.h"

TabsManager* TabsManager::m_instance = NULL;
QCoreApplication::EventFilter TabsManager::m_oldEventFilter = NULL;

TabsManager::TabsManager(){
}

TabsManager::~TabsManager() {
    TaskbarTab* tab;
    foreach(tab, m_tabs) {
        m_tabs.removeOne(tab);

        delete tab->m_tab_widget;
        delete tab;
    }

    if (m_oldEventFilter) {
        qApp->setEventFilter(m_oldEventFilter);
    }

    m_taskbarHandler->Release();
}

TabsManager* TabsManager::getInstance() {

    if (m_instance == NULL) {
        m_instance = new TabsManager();
        m_oldEventFilter = qApp->setEventFilter(&TabsManager::eventFilter);
    }

    return m_instance;
}

void TabsManager::releaseInstance() {
    if (m_instance != NULL) {
        delete m_instance;
    }
}

void TabsManager::setParentWidget(QWidget* widget) {
    m_parentWidget = widget;
    getInstance()->allocTaskbar();
}

void TabsManager::addTab(QWidget* widget, QString title, QIcon icon, QPixmap pixmap) {
    TaskbarTab* tab = new TaskbarTab();

    tab->m_widget = widget;
    tab->m_tab_widget = new QWidget();
    tab->m_tab_widget->setWindowTitle(title);
    tab->m_tab_widget->setWindowIcon(icon.isNull() ? widget->windowIcon() : icon);
    tab->m_thumbnail = pixmap;

    m_tabs.append(tab);
    EnableWidgetIconicPreview(tab->m_tab_widget->winId(), true);

    m_taskbarHandler->RegisterTab(tab->m_tab_widget->winId(), m_parentWidget->winId());
    m_taskbarHandler->SetTabOrder(tab->m_tab_widget->winId(), NULL);
    m_taskbarHandler->SetTabActive(NULL, m_tabs.back()->m_tab_widget->winId(), 0);
}

void TabsManager::updateTab(QWidget* widget, QString title, QIcon icon, QPixmap pixmap) {
    TaskbarTab* tab = findTabByWId(widget->winId(), true);

    if (tab == NULL) return;

    tab->m_thumbnail = pixmap;
    tab->m_tab_widget->setWindowIcon(icon);
    tab->m_tab_widget->setWindowTitle(title);

    DwmInvalidateIconicBitmaps(tab->m_tab_widget->winId());
}

void TabsManager::setActiveTab(QWidget* widget) {
    TaskbarTab* tab = findTabByWId(widget->winId(), true);

    if (tab == NULL) return;

    m_taskbarHandler->SetTabActive(tab->m_tab_widget->winId(), m_parentWidget->winId(), 0);
}

void TabsManager::removeTab(QWidget* widget) {
    TaskbarTab* tab = findTabByWId(widget->winId(), true);

    if (tab == NULL) return;

    m_taskbarHandler->UnregisterTab(tab->m_tab_widget->winId());

    m_tabs.removeOne(tab);
    delete tab->m_tab_widget;
    delete tab;
}

void TabsManager::invalidateTabThumbnail(QWidget* widget) {
    TaskbarTab* tab = findTabByWId(widget->winId(), true);

    if (tab == NULL) return;

     DwmInvalidateIconicBitmaps(tab->m_tab_widget->winId());
}

TabsManager::TaskbarTab* TabsManager::findTabByWId(WId id, bool inserted) {
    bool found = false;

    WId tabWId = NULL;
    TaskbarTab* tab;
    foreach(tab, m_tabs) {

        tabWId = inserted ? tab->m_widget->winId() : tab->m_tab_widget->winId();
        if (tabWId == id) {
            found = true;
            break;
        }
    }

    return found ? tab : NULL;
}

void TabsManager::tabAction(WId id, TABEVENT action) {
    TaskbarTab* tab = findTabByWId(id);

    if (tab != NULL) {
        switch (action) {
        case TAB_CLICK :
            emit onTabClicked(tab->m_widget);
            break;

        case TAB_CLOSE:
            emit onTabClose(tab->m_widget);
            break;

        case TAB_HOVER:
            emit onTabHover(tab->m_widget);
            break;
        }
    }
}

void TabsManager::setPeekBitmap(WId id, QSize size, bool isLive) {
    TaskbarTab* tab = findTabByWId(id);

    if (tab == NULL) return;

    QPixmap thumbnail = (!tab->m_thumbnail.isNull() && !isLive) ? tab->m_thumbnail :
                        QPixmap::grabWidget(isLive ? m_parentWidget : tab->m_widget).scaled(size, Qt::KeepAspectRatio);

    HBITMAP hbitmap = thumbnail.toWinHBITMAP(isLive ? QPixmap::NoAlpha : QPixmap::Alpha);

    isLive ? DwmSetIconicLivePreviewBitmap(id, hbitmap, 0, 0) : DwmSetIconicThumbnail(id, hbitmap, 0);

    if (hbitmap) DeleteObject(hbitmap);
}

void TabsManager::allocTaskbar() {
    HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList4,
                                  reinterpret_cast<void**> (&(m_taskbarHandler)));

    if (SUCCEEDED(hr)){
        //init taskbar
        hr = m_taskbarHandler->HrInit();

        if (SUCCEEDED(hr)) emit isReady();
    }
}

bool TabsManager::eventFilter(void *message_, long *result)
{
    static unsigned int taskBarCreatedId = WM_NULL;

    MSG* message = static_cast<MSG*>(message_);

    if (taskBarCreatedId == WM_NULL) {
        taskBarCreatedId = RegisterWindowMessage(L"TaskbarButtonCreated");
        return false;
    }

    if (message->message == taskBarCreatedId && message->hwnd == getInstance()->m_parentWidget->winId()) {
        getInstance()->allocTaskbar();
        return true;
    }

    switch(message->message)
    {
    case WM_DWMSENDICONICTHUMBNAIL : {
            if (message->hwnd == getInstance()->m_parentWidget->winId()) return false;

            getInstance()->setPeekBitmap(message->hwnd, QSize(HIWORD(message->lParam), LOWORD(message->lParam)));
            return true;
        }
    case WM_DWMSENDICONICLIVEPREVIEWBITMAP : {
            getInstance()->tabAction(message->hwnd, TAB_HOVER);

            if (message->hwnd == getInstance()->m_parentWidget->winId()) return false;

            getInstance()->setPeekBitmap(message->hwnd, getInstance()->m_parentWidget->size(), true);
            return true;
        }
    case WM_ACTIVATE : {
            if (LOWORD(message->wParam) == WA_ACTIVE) {
                getInstance()->tabAction(message->hwnd, TAB_CLICK);
            }
            return false;
        }
    case WM_CLOSE : {
            getInstance()->tabAction(message->hwnd, TAB_CLOSE);
            return false;
        }
    }

    if (TabsManager::m_oldEventFilter)
        return TabsManager::m_oldEventFilter(message_, result);
    else
        return false;
}

