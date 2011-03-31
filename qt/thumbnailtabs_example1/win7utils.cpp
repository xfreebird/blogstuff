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

#include "win7utils.h"

extern "C"
{
    typedef HRESULT (WINAPI *t_DwmInvalidateIconicBitmaps)(HWND hwnd);
    typedef HRESULT (WINAPI *t_DwmSetIconicThumbnail)(HWND hwnd, HBITMAP hbmp, DWORD dwSITFlags);
    typedef HRESULT (WINAPI *t_DwmSetIconicLivePreviewBitmap)(HWND hwnd, HBITMAP hbmp, POINT *pptClient, DWORD dwSITFlags);
    typedef HRESULT (WINAPI *t_DwmSetWindowAttribute)(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
}

// Window attributes
enum DWMWINDOWATTRIBUTE
{
    DWMWA_NCRENDERING_ENABLED = 1,      // [get] Is non-client rendering enabled/disabled
    DWMWA_NCRENDERING_POLICY,           // [set] Non-client rendering policy
    DWMWA_TRANSITIONS_FORCEDISABLED,    // [set] Potentially enable/forcibly disable transitions
    DWMWA_ALLOW_NCPAINT,                // [set] Allow contents rendered in the non-client area to be visible on the DWM-drawn frame.
    DWMWA_CAPTION_BUTTON_BOUNDS,        // [get] Bounds of the caption button area in window-relative space.
    DWMWA_NONCLIENT_RTL_LAYOUT,         // [set] Is non-client content RTL mirrored
    DWMWA_FORCE_ICONIC_REPRESENTATION,  // [set] Force this window to display iconic thumbnails.
    DWMWA_FLIP3D_POLICY,                // [set] Designates how Flip3D will treat the window.
    DWMWA_EXTENDED_FRAME_BOUNDS,        // [get] Gets the extended frame bounds rectangle in screen space
    DWMWA_HAS_ICONIC_BITMAP,            // [set] Indicates an available bitmap when there is no better thumbnail representation.
    DWMWA_DISALLOW_PEEK,                // [set] Don't invoke Peek on the window.
    DWMWA_EXCLUDED_FROM_PEEK,           // [set] LivePreview exclusion information
    DWMWA_LAST
};

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

void DwmInvalidateIconicBitmaps(HWND hwnd) {
    HMODULE shell;

    shell = LoadLibrary(L"dwmapi.dll");
    if (shell) {
        t_DwmInvalidateIconicBitmaps invalidate_icon_bitmap = reinterpret_cast<t_DwmInvalidateIconicBitmaps>(GetProcAddress (shell, "DwmInvalidateIconicBitmaps"));
        invalidate_icon_bitmap(hwnd);

        FreeLibrary (shell);
    }

}

void EnableWidgetIconicPreview(HWND hwnd, bool enable) {
    BOOL _enable = enable ? TRUE : FALSE;

    DwmSetWindowAttribute(
        hwnd,
        DWMWA_FORCE_ICONIC_REPRESENTATION,
        &_enable,
        sizeof(_enable));

    DwmSetWindowAttribute(
        hwnd,
        DWMWA_HAS_ICONIC_BITMAP,
        &_enable,
        sizeof(_enable));
}

void InvalidateIconicBitmaps(HWND hwnd) {
    DwmInvalidateIconicBitmaps(hwnd);
}

