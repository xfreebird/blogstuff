#ifndef WINUTILS_H
#define WINUTILS_H
#include <shlobj.h>

#define DEFINE_GUID_(n,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) const GUID n GUID_SECT = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

typedef enum KNOWNDESTCATEGORY
{
    KDC_FREQUENT = 1,
    KDC_RECENT = (KDC_FREQUENT + 1)
} KNOWNDESTCATEGORY;

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


DEFINE_GUID_(CLSID_DestinationList,0x77f10cf0,0x3db5,0x4966,0xb5,0x20,0xb7,0xc5,0x4f,0xd3,0x5e,0xd6);

DEFINE_GUID_(IID_IObjectArray,0x92ca9dcd,0x5622,0x4bba,0xa8,0x05,0x5e,0x9f,0x54,0x1b,0xd8,0xc9);
DEFINE_GUID_(IID_ICustomDestinationList,0x6332debf,0x87b5,0x4670,0x90,0xc0,0x5e,0x57,0xb4,0x08,0xa4,0x9e);


#endif // WINUTILS_H
