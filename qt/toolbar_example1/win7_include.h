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

#ifndef WIN7_INCLUDE_H
#define WIN7_INCLUDE_H
#include <QtGlobal>

#ifdef Q_OS_WIN32
#include <shlobj.h>
#include <shlwapi.h>
#include <initguid.h>
#include <objidl.h>
#include <shellapi.h>

//Definitions for Windows 7 Taskbar

typedef enum THUMBBUTTONMASK
{
    THB_BITMAP = 0x1,
    THB_ICON = 0x2,
    THB_TOOLTIP	= 0x4,
    THB_FLAGS = 0x8
} THUMBBUTTONMASK;

typedef enum THUMBBUTTONFLAGS
{
    THBF_ENABLED = 0,
    THBF_DISABLED = 0x1,
    THBF_DISMISSONCLICK	= 0x2,
    THBF_NOBACKGROUND = 0x4,
    THBF_HIDDEN	= 0x8,
    THBF_NONINTERACTIVE	= 0x10
} THUMBBUTTONFLAGS;

typedef struct THUMBBUTTON
{
    THUMBBUTTONMASK dwMask;
    UINT iId;
    UINT iBitmap;
    HICON hIcon;
    WCHAR szTip[260];
    THUMBBUTTONFLAGS dwFlags;
} THUMBBUTTON;
typedef struct THUMBBUTTON *LPTHUMBBUTTON;

typedef enum TBPFLAG
{
    TBPF_NOPROGRESS = 0,
    TBPF_INDETERMINATE = 0x1,
    TBPF_NORMAL = 0x2,
    TBPF_ERROR = 0x4,
    TBPF_PAUSED = 0x8
} TBPFLAG;


#define DEFINE_GUID_(n,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) const GUID n GUID_SECT = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

typedef enum
{
    KF_FLAG_DEFAULT         = 0x00000000,
    KF_FLAG_CREATE          = 0x00008000,
    KF_FLAG_DONT_VERIFY     = 0x00004000,
    KF_FLAG_DONT_UNEXPAND   = 0x00002000,
    KF_FLAG_NO_ALIAS        = 0x00001000,
    KF_FLAG_INIT            = 0x00000800,
    KF_FLAG_DEFAULT_PATH    = 0x00000400,
    KF_FLAG_NOT_PARENT_RELATIVE = 0x00000200,
    KF_FLAG_SIMPLE_IDLIST   = 0x00000100,
    KF_FLAG_ALIAS_ONLY      = 0x80000000,
} KNOWN_FOLDER_FLAG;


//MIDL_INTERFACE("56FDF342-FD6D-11d0-958A-006097C9A090")
DECLARE_INTERFACE_(ITaskbarList, IUnknown)
{
    STDMETHOD (HrInit) (THIS) PURE;
    STDMETHOD (AddTab) (THIS_ HWND hwnd) PURE;
    STDMETHOD (DeleteTab) (THIS_ HWND hwnd) PURE;
    STDMETHOD (ActivateTab) (THIS_ HWND hwnd) PURE;
    STDMETHOD (SetActiveAlt) (THIS_ HWND hwnd) PURE;
};
typedef ITaskbarList *LPITaskbarList;

//MIDL_INTERFACE("602D4995-B13A-429b-A66E-1935E44F4317")
DECLARE_INTERFACE_(ITaskbarList2, ITaskbarList)
{
    STDMETHOD (MarkFullscreenWindow) (THIS_ HWND hwnd, int fFullscreen) PURE;
};
typedef ITaskbarList2 *LPITaskbarList2;

//MIDL_INTERFACE("ea1afb91-9e28-4b86-90e9-9e9f8a5eefaf")
DECLARE_INTERFACE_(ITaskbarList3, ITaskbarList2)
{
    STDMETHOD (SetProgressValue) (THIS_ HWND hwnd, ULONGLONG ullCompleted, ULONGLONG ullTotal) PURE;
    STDMETHOD (SetProgressState) (THIS_ HWND hwnd, TBPFLAG tbpFlags) PURE;
    STDMETHOD (RegisterTab) (THIS_ HWND hwndTab,HWND hwndMDI) PURE;
    STDMETHOD (UnregisterTab) (THIS_ HWND hwndTab) PURE;
    STDMETHOD (SetTabOrder) (THIS_ HWND hwndTab, HWND hwndInsertBefore) PURE;
    STDMETHOD (SetTabActive) (THIS_ HWND hwndTab, HWND hwndMDI, DWORD dwReserved) PURE;
    STDMETHOD (ThumbBarAddButtons) (THIS_ HWND hwnd, UINT cButtons, LPTHUMBBUTTON pButton) PURE;
    STDMETHOD (ThumbBarUpdateButtons) (THIS_ HWND hwnd, UINT cButtons, LPTHUMBBUTTON pButton) PURE;
    STDMETHOD (ThumbBarSetImageList) (THIS_ HWND hwnd, HIMAGELIST himl) PURE;
    STDMETHOD (SetOverlayIcon) (THIS_ HWND hwnd, HICON hIcon, LPCWSTR pszDescription) PURE;
    STDMETHOD (SetThumbnailTooltip) (THIS_ HWND hwnd, LPCWSTR pszTip) PURE;
    STDMETHOD (SetThumbnailClip) (THIS_ HWND hwnd, RECT *prcClip) PURE;
};
typedef ITaskbarList3 *LPITaskbarList3;


DEFINE_GUID_(CLSID_TaskbarList,0x56fdf344,0xfd6d,0x11d0,0x95,0x8a,0x0,0x60,0x97,0xc9,0xa0,0x90);

DEFINE_GUID_(IID_ITaskbarList,0x56FDF342,0xFD6D,0x11d0,0x95,0x8A,0x00,0x60,0x97,0xC9,0xA0,0x90);
DEFINE_GUID_(IID_ITaskbarList2,0x602D4995,0xB13A,0x429b,0xA6,0x6E,0x19,0x35,0xE4,0x4F,0x43,0x17);
DEFINE_GUID_(IID_ITaskbarList3,0xea1afb91,0x9e28,0x4b86,0x90,0xE9,0x9e,0x9f,0x8a,0x5e,0xef,0xaf);

#endif //Q_OS_WIN32



#endif // WIN7_INCLUDE_H
