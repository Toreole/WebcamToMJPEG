#include "CVirtualCamera.h"
#include <assert.h>

HRESULT CVirtualCamera::AddDeviceSourceInfo(LPCWSTR DeviceSourceInfo)
{
    IMFAttributes* pAtt;
    assert(pAtt);
    return E_NOTIMPL;
}

HRESULT CVirtualCamera::SetGUID(REFGUID guidKey, REFGUID guidValue)
{
    m_GUID_att[guidKey] = guidValue;
    return S_OK;
}

STDMETHODIMP CVirtualCamera::GetItem(REFGUID guidKey, PROPVARIANT* pValue)
{
    assert(m_pAttributes);
    return m_pAttributes->GetItem(guidKey, pValue);
}
