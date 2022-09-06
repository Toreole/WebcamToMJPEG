#include "CVirtualCamera.h"
#include <assert.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

STDMETHODIMP CVirtualCamera::AddDeviceSourceInfo(LPCWSTR DeviceSourceInfo)
{
    IMFAttributes* pSearchAttributes = NULL;
    HRESULT hr = MFCreateAttributes(&pSearchAttributes, 1);
    IMFActivate** ppSources;
    UINT32 count = 0;

    if (SUCCEEDED(hr))
    {
        pSearchAttributes->SetString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, DeviceSourceInfo);
    }

    hr = MFEnumDeviceSources(pSearchAttributes, &ppSources, &count);

    if (SUCCEEDED(hr))
    {
        IMFActivate* source = ppSources[0];
        source->AddRef();
    }
    else
    {
        // if no devices with that name could be found, the argument was wrong.
        return E_INVALIDARG;
    }

    return hr;
}

STDMETHODIMP CVirtualCamera::AddProperty(
    const DEVPROPKEY* pKey,
    DEVPROPTYPE Type,
    const BYTE *pbData,
    ULONG cbData)
{
    return E_NOTIMPL;
}

STDMETHODIMP CVirtualCamera::AddRegistryEntry(
    LPCWSTR EntryName,
    LPCWSTR SubkeyPath,
    DWORD dwRegType,
    const BYTE* pbData,
    ULONG cbData
)
{
    return E_NOTIMPL;
}

STDMETHODIMP CVirtualCamera::CreateSyncEvent(
    REFGUID kseventSet,
    ULONG kseventId,
    ULONG kseventFlags,
    HANDLE eventHandle,
    IMFCameraSyncObject** cameraSyncObject
)
{
    return E_NOTIMPL;
}

STDMETHODIMP CVirtualCamera::CreateSyncSemaphore(
    REFGUID kseventSet,
    ULONG kseventId,
    ULONG kseventFlags,
    HANDLE semaphoreHandle,
    LONG semaphoreAdjustment,
    IMFCameraSyncObject** cameraSyncObject
)
{
    return E_NOTIMPL;
}

STDMETHODIMP CVirtualCamera::GetMediaSource
(
    IMFMediaSource** ppMediaSource
)
{
    return E_NOTIMPL;
}

STDMETHODIMP CVirtualCamera::Remove()
{
    return E_NOTIMPL;
}

STDMETHODIMP CVirtualCamera::SendCameraProperty(
    REFGUID propertySet,
    ULONG propertyId,
    ULONG propertyFlags,
    void* propertyPayload,
    ULONG propertyPayloadLength,
    void* data,
    ULONG dataLength,
    ULONG* dataWritten
)
{
    return E_NOTIMPL;
}

STDMETHODIMP CVirtualCamera::Shutdown()
{
    return E_NOTIMPL;
}

STDMETHODIMP CVirtualCamera::Start(
    IMFAsyncCallback* pCallback
)
{
    return E_NOTIMPL;
}

STDMETHODIMP CVirtualCamera::Stop()
{
    return E_NOTIMPL;
}

//implementations for IMFAttributes interface, all being passed through to m_pAttributes.

STDMETHODIMP CVirtualCamera::GetItem(REFGUID guidKey, PROPVARIANT* pValue)
{
    assert(m_pAttributes);
    return m_pAttributes->GetItem(guidKey, pValue);
}

STDMETHODIMP CVirtualCamera::GetItemType(REFGUID guidKey, MF_ATTRIBUTE_TYPE* pType)
{
    assert(m_pAttributes);
    return m_pAttributes->GetItemType(guidKey, pType);
}

STDMETHODIMP CVirtualCamera::CompareItem(REFGUID guidKey, REFPROPVARIANT Value, BOOL* pbResult)
{
    assert(m_pAttributes);
    return m_pAttributes->CompareItem(guidKey, Value, pbResult);
}

STDMETHODIMP CVirtualCamera::Compare(
    IMFAttributes* pTheirs,
    MF_ATTRIBUTES_MATCH_TYPE MatchType,
    BOOL* pbResult
)
{
    assert(m_pAttributes);
    return m_pAttributes->Compare(pTheirs, MatchType, pbResult);
}

STDMETHODIMP CVirtualCamera::GetUINT32(REFGUID guidKey, UINT32* punValue)
{
    assert(m_pAttributes);
    return m_pAttributes->GetUINT32(guidKey, punValue);
}

STDMETHODIMP CVirtualCamera::GetUINT64(REFGUID guidKey, UINT64* punValue)
{
    assert(m_pAttributes);
    return m_pAttributes->GetUINT64(guidKey, punValue);
}

STDMETHODIMP CVirtualCamera::GetDouble(REFGUID guidKey, double* pfValue)
{
    assert(m_pAttributes);
    return m_pAttributes->GetDouble(guidKey, pfValue);
}

STDMETHODIMP CVirtualCamera::GetGUID(REFGUID guidKey, GUID* pguidValue)
{
    assert(m_pAttributes);
    return m_pAttributes->GetGUID(guidKey, pguidValue);
}

STDMETHODIMP CVirtualCamera::GetStringLength(REFGUID guidKey, UINT32* pcchLength)
{
    assert(m_pAttributes);
    return m_pAttributes->GetStringLength(guidKey, pcchLength);
}

STDMETHODIMP CVirtualCamera::GetString(REFGUID guidKey, LPWSTR pwszValue, UINT32 cchBufSize, UINT32* pcchLength)
{
    assert(m_pAttributes);
    return m_pAttributes->GetString(guidKey, pwszValue, cchBufSize, pcchLength);
}

STDMETHODIMP CVirtualCamera::GetAllocatedString(REFGUID guidKey, LPWSTR* ppwszValue, UINT32* pcchLength)
{
    assert(m_pAttributes);
    return m_pAttributes->GetAllocatedString(guidKey, ppwszValue, pcchLength);
}

STDMETHODIMP CVirtualCamera::GetBlobSize(REFGUID guidKey, UINT32* pcbBlobSize)
{
    assert(m_pAttributes);
    return m_pAttributes->GetBlobSize(guidKey, pcbBlobSize);
}

STDMETHODIMP CVirtualCamera::GetBlob(REFGUID guidKey, UINT8* pBuf, UINT32 cbBufSize, UINT32* pcbBlobSize)
{
    assert(m_pAttributes);
    return m_pAttributes->GetBlob(guidKey, pBuf, cbBufSize, pcbBlobSize);
}

STDMETHODIMP CVirtualCamera::GetAllocatedBlob(REFGUID guidKey, UINT8** ppBuf, UINT32* pcbSize)
{
    assert(m_pAttributes);
    return m_pAttributes->GetAllocatedBlob(guidKey, ppBuf, pcbSize);
}

STDMETHODIMP CVirtualCamera::GetUnknown(REFGUID guidKey, REFIID riid, LPVOID* ppv)
{
    assert(m_pAttributes);
    return m_pAttributes->GetUnknown(guidKey, riid, ppv);
}

STDMETHODIMP CVirtualCamera::SetItem(REFGUID guidKey, REFPROPVARIANT Value)
{
    assert(m_pAttributes);
    return m_pAttributes->SetItem(guidKey, Value);
}

STDMETHODIMP CVirtualCamera::DeleteItem(REFGUID guidKey)
{
    assert(m_pAttributes);
    return m_pAttributes->DeleteItem(guidKey);
}

STDMETHODIMP CVirtualCamera::DeleteAllItems()
{
    assert(m_pAttributes);
    return m_pAttributes->DeleteAllItems();
}

STDMETHODIMP CVirtualCamera::SetUINT32(REFGUID guidKey, UINT32 unValue)
{
    assert(m_pAttributes);
    return m_pAttributes->SetUINT32(guidKey, unValue);
}

STDMETHODIMP CVirtualCamera::SetUINT64(REFGUID guidKey, UINT64 unValue)
{
    assert(m_pAttributes);
    return m_pAttributes->SetUINT64(guidKey, unValue);
}

STDMETHODIMP CVirtualCamera::SetDouble(REFGUID guidKey, double fValue)
{
    assert(m_pAttributes);
    return m_pAttributes->SetDouble(guidKey, fValue);
}

STDMETHODIMP CVirtualCamera::SetGUID(REFGUID guidKey, REFGUID guidValue)
{
    assert(m_pAttributes);
    return m_pAttributes->SetGUID(guidKey, guidValue);
}

STDMETHODIMP CVirtualCamera::SetString(REFGUID guidKey, LPCWSTR wszValue)
{
    assert(m_pAttributes);
    return m_pAttributes->SetString(guidKey, wszValue);
}

STDMETHODIMP CVirtualCamera::SetBlob(REFGUID guidKey, const UINT8* pBuf, UINT32 cbBufSize)
{
    assert(m_pAttributes);
    return m_pAttributes->SetBlob(guidKey, pBuf, cbBufSize);
}

STDMETHODIMP CVirtualCamera::SetUnknown(REFGUID guidKey, IUnknown* pUnknown)
{
    assert(m_pAttributes);
    return m_pAttributes->SetUnknown(guidKey, pUnknown);
}

STDMETHODIMP CVirtualCamera::LockStore()
{
    assert(m_pAttributes);
    return m_pAttributes->LockStore();
}

STDMETHODIMP CVirtualCamera::UnlockStore()
{
    assert(m_pAttributes);
    return m_pAttributes->UnlockStore();
}

STDMETHODIMP CVirtualCamera::GetCount(UINT32* pcItems)
{
    assert(m_pAttributes);
    return m_pAttributes->GetCount(pcItems);
}

STDMETHODIMP CVirtualCamera::GetItemByIndex(UINT32 unIndex, GUID* pguidKey, PROPVARIANT* pValue)
{
    assert(m_pAttributes);
    return m_pAttributes->GetItemByIndex(unIndex, pguidKey, pValue);
}

STDMETHODIMP CVirtualCamera::CopyAllItems(IMFAttributes* pDest)
{
    assert(m_pAttributes);
    return m_pAttributes->CopyAllItems(pDest);
}

// IUnknown
STDMETHODIMP CVirtualCamera::QueryInterface(REFIID riid, void** ppv)
{
    static const QITAB qit[] =
    {
        QITABENT(CVirtualCamera, IMFAttributes),
        { 0 },
    };
    return QISearch(this, qit, riid, ppv);
}