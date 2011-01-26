#ifndef WINUTILS_H
#define WINUTILS_H
#include <shlobj.h>

#define DEFINE_GUID_(n,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) const GUID n GUID_SECT = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

typedef enum KNOWNDESTCATEGORY
{
    KDC_FREQUENT = 1,
    KDC_RECENT = (KDC_FREQUENT + 1)
} KNOWNDESTCATEGORY;

typedef enum _SIGDN
{
    SIGDN_NORMALDISPLAY	= 0,
    SIGDN_PARENTRELATIVEPARSING	= (int) 0x80018001,
    SIGDN_DESKTOPABSOLUTEPARSING = (int) 0x80028000,
    SIGDN_PARENTRELATIVEEDITING	= (int) 0x80031001,
    SIGDN_DESKTOPABSOLUTEEDITING = (int) 0x8004c000,
    SIGDN_FILESYSPATH = (int) 0x80058000,
    SIGDN_URL = (int) 0x80068000,
    SIGDN_PARENTRELATIVEFORADDRESSBAR = (int) 0x8007c001,
    SIGDN_PARENTRELATIVE = (int) 0x80080001
} SIGDN;

enum _SICHINTF
{
    SICHINT_DISPLAY = 0,
    SICHINT_ALLFIELDS = (int) 0x80000000,
    SICHINT_CANONICAL = 0x10000000,
    SICHINT_TEST_FILESYSPATH_IF_NOT_EQUAL = 0x20000000
};
typedef DWORD SICHINTF;

DECLARE_INTERFACE_(IObjectArray, IUnknown)
{
    STDMETHOD (GetCount) (UINT *pcObjects) PURE;
    STDMETHOD (GetAt) (UINT uiIndex, REFIID riid, void **ppv) PURE;
};
typedef IObjectArray *LPIObjectArray;

DECLARE_INTERFACE_(ICustomDestinationList, IUnknown)
{
    STDMETHOD (SetAppID) (LPCWSTR pszAppID);
    STDMETHOD (BeginList) (UINT *pcMinSlots, REFIID riid, void **ppv) PURE;
    STDMETHOD (AppendCategory) (LPCWSTR pszCategory, IObjectArray *poa) PURE;
    STDMETHOD (AppendKnownCategory) (KNOWNDESTCATEGORY category) PURE;
    STDMETHOD (AddUserTasks) (IObjectArray *poa) PURE;
    STDMETHOD (CommitList) (void) PURE;
    STDMETHOD (GetRemovedDestinations) (REFIID riid, void **ppv) PURE;
    STDMETHOD (DeleteList) (LPCWSTR pszAppID) PURE;
    STDMETHOD (AbortList)  (void) PURE;

};
typedef ICustomDestinationList *LPICustomDestinationList;

DECLARE_INTERFACE_(IShellItem, IUnknown)
{
    STDMETHOD (BindToHandler) (IBindCtx *pbc, REFGUID bhid, REFIID riid,void **ppv) PURE;
    STDMETHOD (GetParent) (IShellItem **ppsi) PURE;
    STDMETHOD (GetDisplayName) (SIGDN sigdnName, LPWSTR *ppszName) PURE;
    STDMETHOD (GetAttributes) (SFGAOF sfgaoMask, SFGAOF *psfgaoAttribs) PURE;
    STDMETHOD (Compare) (IShellItem *psi, SICHINTF hint, int *piOrder) PURE;
};
typedef IShellItem *LPIShellItem;

DECLARE_INTERFACE_(IObjectCollection, IObjectArray)
{
    STDMETHOD (AddObject) (IUnknown *punk) PURE;
    STDMETHOD (AddFromArray) (IObjectArray *poaSource)PURE;
    STDMETHOD (RemoveObjectAt) (UINT uiIndex) PURE;
    STDMETHOD (Clear) (void) PURE;
};
typedef IObjectCollection *LPIObjectCollection;

DEFINE_GUID_(CLSID_DestinationList,0x77f10cf0,0x3db5,0x4966,0xb5,0x20,0xb7,0xc5,0x4f,0xd3,0x5e,0xd6);
DEFINE_GUID_(CLSID_ShellItem,0x9ac9fbe1,0xe0a2,0x4ad6,0xb4,0xee,0xe2,0x12,0x01,0x3e,0xa9,0x17);
DEFINE_GUID_(CLSID_EnumerableObjectCollection,0x2d3468c1,0x36a7,0x43b6,0xac,0x24,0xd3,0xf0,0x2f,0xd9,0x60,0x7a);

DEFINE_GUID_(IID_IObjectArray,0x92ca9dcd,0x5622,0x4bba,0xa8,0x05,0x5e,0x9f,0x54,0x1b,0xd8,0xc9);
DEFINE_GUID_(IID_ICustomDestinationList,0x6332debf,0x87b5,0x4670,0x90,0xc0,0x5e,0x57,0xb4,0x08,0xa4,0x9e);
DEFINE_GUID_(IID_IShellItem,0x43826d1e,0xe718,0x42ee,0xbc,0x55,0xa1,0xe2,0x61,0xc3,0x7b,0xfe);
DEFINE_GUID_(IID_IObjectCollection,0x5632b1a4,0xe38a,0x400a,0x92,0x8a,0xd4,0xcd,0x63,0x23,0x02,0x95);


extern "C"
{
    typedef HRESULT (WINAPI *t_SHCreateItemFromParsingName)(PCWSTR pszPath, IBindCtx *pbc, REFIID riid, void **ppv);
}


#endif // WINUTILS_H
