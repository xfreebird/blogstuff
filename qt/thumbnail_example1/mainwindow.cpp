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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <windows.h>

#define WM_DWMSENDICONICTHUMBNAIL         0x0323
#define WM_DWMSENDICONICLIVEPREVIEWBITMAP 0x0326
#define DWMWA_FORCE_ICONIC_REPRESENTATION      7
#define DWMWA_HAS_ICONIC_BITMAP               10


extern "C"
{
    typedef HRESULT (WINAPI *t_DwmSetIconicThumbnail)(HWND hwnd, HBITMAP hbmp, DWORD dwSITFlags);
    typedef HRESULT (WINAPI *t_DwmSetWindowAttribute)(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
    typedef HRESULT (WINAPI *t_DwmSetIconicLivePreviewBitmap)(HWND hwnd, HBITMAP hbmp, POINT *pptClient, DWORD dwSITFlags);
}

void DwmSetIconicThumbnail(HWND hwnd, HBITMAP hbmp, DWORD dwSITFlags) {
    HMODULE shell;

    shell = LoadLibrary(L"dwmapi.dll");
    if (shell) {
        t_DwmSetIconicThumbnail set_iconic_thumbnail = reinterpret_cast<t_DwmSetIconicThumbnail>(GetProcAddress (shell, "DwmSetIconicThumbnail"));
        set_iconic_thumbnail(hwnd, hbmp, dwSITFlags);

        FreeLibrary (shell);
    }
}

void DwmSetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute) {
    HMODULE shell;

    shell = LoadLibrary(L"dwmapi.dll");
    if (shell) {
        t_DwmSetWindowAttribute set_window_attribute = reinterpret_cast<t_DwmSetWindowAttribute>(GetProcAddress (shell, "DwmSetWindowAttribute"));
        set_window_attribute(hwnd, dwAttribute, pvAttribute, cbAttribute);

        FreeLibrary (shell);
    }
}

void DwmSetIconicLivePreviewBitmap(HWND hwnd, HBITMAP hbmp, POINT *pptClient, DWORD dwSITFlags) {
    HMODULE shell;

    shell = LoadLibrary(L"dwmapi.dll");
    if (shell) {
        t_DwmSetIconicLivePreviewBitmap set_live_preview = reinterpret_cast<t_DwmSetIconicLivePreviewBitmap>(GetProcAddress (shell, "DwmSetIconicLivePreviewBitmap"));
        set_live_preview(hwnd, hbmp, pptClient, dwSITFlags);

        FreeLibrary (shell);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_thumbnail = QPixmap(":/qtlogo.png");

    connect(ui->cbThumbnailPreview, SIGNAL(clicked(bool)), this, SLOT(EnableIconicPreview(bool)));

}

void MainWindow::EnableIconicPreview(bool enable) {
    BOOL fForceIconic = enable ? TRUE : FALSE;
    BOOL fHasIconicBitmap = enable ? TRUE : FALSE;

    DwmSetWindowAttribute(
        this->winId(),
        DWMWA_FORCE_ICONIC_REPRESENTATION,
        &fForceIconic,
        sizeof(fForceIconic));


    DwmSetWindowAttribute(
        this->winId(),
        DWMWA_HAS_ICONIC_BITMAP,
        &fHasIconicBitmap,
        sizeof(fHasIconicBitmap));
}

bool MainWindow::winEvent(MSG * message, long * result)
{
    switch (message->message)
    {
    case WM_DWMSENDICONICTHUMBNAIL: {
            HBITMAP hbitmap = m_thumbnail.toWinHBITMAP();
            DwmSetIconicThumbnail(this->winId(), hbitmap, 0);

            if (hbitmap) DeleteObject(hbitmap);
        }
        break;

    case WM_DWMSENDICONICLIVEPREVIEWBITMAP: {
            HBITMAP hbitmap = m_thumbnail.toWinHBITMAP();
            POINT point;
            point.x = 0; point.y = 0;

            DwmSetIconicLivePreviewBitmap(this->winId(), hbitmap, &point, 0);
            if (hbitmap) DeleteObject(hbitmap);
        }
        break;
    default:
        return false;
    }


    return false;
}

MainWindow::~MainWindow()
{
    delete ui;
}
