#ifndef WINUTILS_H
#define WINUTILS_H
#include <shlobj.h>
#include <shlwapi.h>
//#include <initguid.h>
//#include <objidl.h>
//#include <shellapi.h>


#define DEFINE_GUID_(n,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) const GUID n GUID_SECT = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

typedef struct _tagpropertykey
{
    GUID fmtid;
    DWORD pid;
} PROPERTYKEY;

#define REFPROPERTYKEY const PROPERTYKEY &

typedef struct tagPROPVARIANT PROPVARIANT;
#define REFPROPVARIANT const PROPVARIANT &

#define DEFINE_PROPERTYKEY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8, pid) const PROPERTYKEY name \
                = { { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }, pid }

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


//MIDL_INTERFACE("886d8eeb-8cf2-4446-8d02-cdba1dbdcf99")
DECLARE_INTERFACE_(IPropertyStore, IUnknown)
{
    STDMETHOD (GetCount) (DWORD *cProps) PURE;
    STDMETHOD (GetAt) (DWORD iProp, PROPERTYKEY *pkey) PURE;
    STDMETHOD (GetValue) (REFPROPERTYKEY key, PROPVARIANT *pv) PURE;
    STDMETHOD (SetValue) (REFPROPERTYKEY key, REFPROPVARIANT propvar) PURE;
    STDMETHOD (Commit) (void) PURE;
};
typedef IPropertyStore *LPIPropertyStore;

DEFINE_GUID_(CLSID_DestinationList,0x77f10cf0,0x3db5,0x4966,0xb5,0x20,0xb7,0xc5,0x4f,0xd3,0x5e,0xd6);
DEFINE_GUID_(CLSID_ShellItem,0x9ac9fbe1,0xe0a2,0x4ad6,0xb4,0xee,0xe2,0x12,0x01,0x3e,0xa9,0x17);
DEFINE_GUID_(CLSID_EnumerableObjectCollection,0x2d3468c1,0x36a7,0x43b6,0xac,0x24,0xd3,0xf0,0x2f,0xd9,0x60,0x7a);

DEFINE_GUID_(IID_IObjectArray,0x92ca9dcd,0x5622,0x4bba,0xa8,0x05,0x5e,0x9f,0x54,0x1b,0xd8,0xc9);
DEFINE_GUID_(IID_ICustomDestinationList,0x6332debf,0x87b5,0x4670,0x90,0xc0,0x5e,0x57,0xb4,0x08,0xa4,0x9e);
DEFINE_GUID_(IID_IShellItem,0x43826d1e,0xe718,0x42ee,0xbc,0x55,0xa1,0xe2,0x61,0xc3,0x7b,0xfe);
DEFINE_GUID_(IID_IObjectCollection,0x5632b1a4,0xe38a,0x400a,0x92,0x8a,0xd4,0xcd,0x63,0x23,0x02,0x95);
DEFINE_GUID_(IID_IPropertyStore,0x886d8eeb,0x8cf2,0x4446,0x8d,0x02,0xcd,0xba,0x1d,0xbd,0xcf,0x99);

DEFINE_PROPERTYKEY(PKEY_Title, 0xF29F85E0, 0x4FF9, 0x1068, 0xAB, 0x91, 0x08, 0x00, 0x2B, 0x27, 0xB3, 0xD9, 2);
DEFINE_PROPERTYKEY(PKEY_AppUserModel_IsDestListSeparator, 0x9F4C2855, 0x9F79, 0x4B39, 0xA8, 0xD0, 0xE1, 0xD4, 0x2D, 0xE1, 0xD5, 0xF3, 6);



extern "C"
{
    typedef HRESULT (WINAPI *t_SHCreateItemFromParsingName)(PCWSTR pszPath, IBindCtx *pbc, REFIID riid, void **ppv);
}

WINOLEAPI PropVariantClear(PROPVARIANT* pvar);

inline HRESULT InitPropVariantFromString(PCWSTR psz, PROPVARIANT *ppropvar)
{
    ppropvar->vt = VT_LPWSTR;
    HRESULT hr = SHStrDupW(psz, &ppropvar->pwszVal);
    return hr;
}

inline HRESULT InitPropVariantFromBoolean(BOOL fVal, PROPVARIANT *ppropvar)
{
    ppropvar->vt = VT_BOOL;
    ppropvar->boolVal = fVal ? VARIANT_TRUE : VARIANT_FALSE;
    return S_OK;
}

#endif // WINUTILS_H
