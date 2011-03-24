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

#include <QBitmap>
#include <QPixmap>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define IDTB_FIRST 3000

void MainWindow::W7ToolbarInit() {
    HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList3,
                                  reinterpret_cast<void**> (&(m_w7toolbar)));

    if (SUCCEEDED(hr)){

        hr = m_w7toolbar->HrInit();

        if (FAILED(hr)) {
            m_w7toolbar->Release();
            m_w7toolbar = NULL;
        }
    }
}

void MainWindow::W7ToolbarButtonsInit() {
    QString tooltips[3] = {"Prev", "Play", "Next"};

    for (int index = 0; index < 3; index++) {
        wcscpy(m_thbButtons[index].szTip, tooltips[index].toStdWString().c_str());

        m_thbButtons[index].iId = IDTB_FIRST + index;
        m_thbButtons[index].iBitmap = index;
        m_thbButtons[index].dwMask = (THUMBBUTTONMASK)(THB_BITMAP | THB_FLAGS | THB_TOOLTIP);
        m_thbButtons[index].dwFlags = (THUMBBUTTONFLAGS)(THBF_ENABLED);

    }

    if (m_w7toolbar) m_w7toolbar->ThumbBarAddButtons(this->winId(), 3, m_thbButtons);
}

void MainWindow::W7ToolbarButtonsUpdate() {
    if (m_w7toolbar) m_w7toolbar->ThumbBarUpdateButtons(this->winId(), 3, m_thbButtons);
}

void MainWindow::W7ToolbarSetImages() {
    QPixmap img;
    QBitmap mask;
    HIMAGELIST himl = ImageList_Create(20, 20, ILC_COLOR32, 4, 0);

    img = QIcon(":/back.png").pixmap(20);
    mask  = img.createMaskFromColor(Qt::transparent);
    ImageList_Add(himl, img.toWinHBITMAP(QPixmap::PremultipliedAlpha), mask.toWinHBITMAP());

    img = QIcon(":/play.png").pixmap(20);
    mask  = img.createMaskFromColor(Qt::transparent);
    ImageList_Add(himl, img.toWinHBITMAP(QPixmap::PremultipliedAlpha), mask.toWinHBITMAP());

    img = QIcon(":/forward.png").pixmap(20);
    mask  = img.createMaskFromColor(Qt::transparent);
    ImageList_Add(himl, img.toWinHBITMAP(QPixmap::PremultipliedAlpha), mask.toWinHBITMAP());

    img = QIcon(":/pause.png").pixmap(20);
    mask  = img.createMaskFromColor(Qt::transparent);
    ImageList_Add(himl, img.toWinHBITMAP(QPixmap::PremultipliedAlpha), mask.toWinHBITMAP());

    if (m_w7toolbar) m_w7toolbar->ThumbBarSetImageList(this->winId(), himl);

    ImageList_Destroy(himl);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

bool MainWindow::winEvent(MSG * message, long * result)
{
    static UINT taskBarCreatedId = WM_NULL;
    if (taskBarCreatedId == WM_NULL) {
        taskBarCreatedId = RegisterWindowMessage(L"TaskbarButtonCreated");
    }

    if (message->message == taskBarCreatedId) {
        W7ToolbarInit();
        W7ToolbarSetImages();
        W7ToolbarButtonsInit();

    } else switch (message->message){
    case WM_COMMAND:
        {
            int buttonId = LOWORD(message->wParam) - IDTB_FIRST;

            if ((buttonId >= 0) && (buttonId < 3)) {

                qDebug() << "Button " << buttonId << " was pressed";
                if (buttonId == 1) { //if "Play|Pause" was pressed

                    if (m_thbButtons[1].iBitmap == 1) {
                        m_thbButtons[1].iBitmap = 3;
                        wcscpy(m_thbButtons[1].szTip, L"Pause");
                    } else {
                        m_thbButtons[1].iBitmap = 1;
                        wcscpy(m_thbButtons[1].szTip, L"Play");
                    }

                    W7ToolbarButtonsUpdate();
                }

            }


            break;
        }

    default:
        return false;

    }


    return false;
}


MainWindow::~MainWindow()
{
    delete ui;
}
