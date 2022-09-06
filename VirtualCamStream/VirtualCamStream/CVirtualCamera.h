#pragma once
#include <mfvirtualcamera.h> // https://docs.microsoft.com/en-us/windows/win32/api/mfvirtualcamera/
#include <mfapi.h>

// For now the goal of this class is to receive input from a hardware video capture device (camera), 
// and copy the data to a new virtual output stream. essentially duplicating the device.
class CVirtualCamera :
    public IMFVirtualCamera // see https://docs.microsoft.com/en-us/windows/win32/api/mfvirtualcamera/nn-mfvirtualcamera-imfvirtualcamera 
{
private:
    // Use an attribute store provided by the MF API via MFCreateAttributes: https://docs.microsoft.com/en-us/windows/win32/api/mfapi/nf-mfapi-mfcreateattributes
    // also see https://docs.microsoft.com/en-us/windows/win32/medfound/attributes-and-properties#implementing-imfattributes 
    IMFAttributes *m_pAttributes;

    ULONG m_refCount = 0;

protected:
    // This version of the constructor does not initialize the attribute store.
    CVirtualCamera() : m_pAttributes(NULL){ }

    // This constructor initializes the attribute store, but requires an HRESULT parameter.
    CVirtualCamera(HRESULT& hr, UINT32 cInitialSize = 0) : m_pAttributes(NULL)
    {
        hr = Initialize(cInitialSize);
    }

    // Same as above but with an IUnkown
    CVirtualCamera(HRESULT& hr, IUnknown* pUnk)
    {
        hr = Initialize(pUnk);
    }

    // Initializes the object by creating the standard Media Foundation attribute store.
    HRESULT Initialize(UINT32 cInitialSize = 0)
    {
        if (m_pAttributes == NULL)
            return MFCreateAttributes(&m_pAttributes, cInitialSize);
        return S_OK;
    }

    // Initializes this object from a caller-provided attribute store.
    // pUnk: Pointer to an object that exposes IMFAttributes.
    HRESULT Initialize(IUnknown* pUnk)
    {
        if (m_pAttributes)
        {
            m_pAttributes->Release();
            m_pAttributes = NULL;
        }

        return pUnk->QueryInterface(IID_PPV_ARGS(&m_pAttributes));
    }

public: 
    
    static HRESULT CreateInstance(CVirtualCamera** ppObject)
    {
        HRESULT hr = S_OK;

        CVirtualCamera* pObject = new CVirtualCamera();
        if (pObject == NULL)
            return E_OUTOFMEMORY;
    
        hr = pObject->Initialize();
        if (FAILED(hr))
        {
            delete pObject;
            return hr;
        }

        *ppObject = pObject;
        (*ppObject)->AddRef();

        return S_OK;
    }


    // This first section is purely the IMFVirtualCamera "device" methods. 

    // Informs the pipeline the virtual camera will require exclusive control to the physical camera specified by the specified device symbolic name.
    HRESULT AddDeviceSourceInfo(
        LPCWSTR DeviceSourceInfo
    );

    // Adds custom device interface properties to the virtual camera.
    HRESULT AddProperty(
        const DEVPROPKEY* pKey, 
        DEVPROPTYPE dType, 
        const BYTE* pbData, 
        ULONG cbData
    );

    // Adds a custom registry entry to the device interface registry key.
    HRESULT AddRegistryEntry(
        LPCWSTR EntryName,
        LPCWSTR SupkeyPath,
        DWORD dwRegType,
        const BYTE* pbData,
        ULONG cbData
    );

    // A wrapper around the IKsControl::KsEvent method, which enables or disables an event.
    HRESULT CreateSyncEvent(
        REFGUID kseventSet, 
        ULONG kseventId, 
        ULONG kseventFlags, 
        HANDLE evenHandle, 
        IMFCameraSyncObject** cameraSyncObject
    );

    // A wrapper around the IKsControl::KsEvent method, which enables or disables an semaphore.
    HRESULT CreateSyncSemaphore(
        REFGUID kseventSet, 
        ULONG kseventId,
        ULONG kseventFlags,
        HANDLE semaphoreHandle,
        LONG semaphoreAdjustment, 
        IMFCameraSyncObject** cameraSyncObject
    );

    // Gets an IMFMediaSource that provides media data from the virtual camera.
    HRESULT GetMediaSource(
        IMFMediaSource** ppMediaSource
    );

    // Disables and unregisters the virtual camera from the system. This is the equivalent to physically removing an external camera and uninstalling the driver.
    HRESULT Remove();

    // A wrapper around the internal IKsControl::KsProperty method, which sets a property or retrieves property information, 
    // along with any other defined support operations available on a property set.
    HRESULT SendCameraProperty(
        REFGUID propertySet,
        ULONG   propertyId,
        ULONG   propertyFlags,
        void* propertyPayload,
        ULONG   propertyPayloadLength,
        void* data,
        ULONG   dataLength,
        ULONG* dataWritten
    );

    // Releases all of the virtual camera's internal resources.
    HRESULT Shutdown();

    // Creates and registers the virtual camera.
    HRESULT Start(
        IMFAsyncCallback* pCallback
    );

    // Disables the registered virtual camera, blocking apps from being able to enumerate or activate the virtual camera.
    HRESULT Stop();

    // This next bunch of methods are part of the IMFAttributes interface. 
    // see: https://docs.microsoft.com/en-us/windows/win32/api/mfobjects/nn-mfobjects-imfattributes

    // Compares the attributes on this object with the attributes on another object.
    HRESULT Compare(
        /*[in]*/  IMFAttributes* pTheirs,
        /*[in]*/  MF_ATTRIBUTES_MATCH_TYPE MatchType,
        /*[out]*/ BOOL* pbResult
    );

    // Queries whether a stored attribute value equals to a specified PROPVARIANT.
    HRESULT CompareItem(
        /*[in]*/  REFGUID        guidKey,
        /*[in]*/  REFPROPVARIANT Value,
        /*[out]*/ BOOL           *pbResult
    );

    // Copies all of the attributes from this object into another attribute store.
    HRESULT CopyAllItems(
        /*[in]*/ IMFAttributes* pDest
    );

    // Removes all key/value pairs from the object's attribute list.
    HRESULT DeleteAllItems();

    // Removes a key/value pair from the object's attribute list.
    HRESULT DeleteItem(
        /*[in]*/ REFGUID guidKey
    );

    // Retrieves a byte array associated with a key. This method allocates the memory for the array.
    HRESULT GetAllocatedBlob(
        /*[in]*/  REFGUID guidKey,
        /*[out]*/ UINT8** ppBuf,
        /*[out]*/ UINT32* pcbSize
    );

    // Gets a wide-character string associated with a key. This method allocates the memory for the string.
    HRESULT GetAllocatedString(
        /*[in]*/  REFGUID guidKey,
        /*[out]*/ LPWSTR* ppwszValue,
        /*[out]*/ UINT32* pcchLength
    );

    // Retrieves a byte array associated with a key. This method copies the array into a caller-allocated buffer.
    HRESULT GetBlob(
        /*[in]*/  REFGUID guidKey,
        /*[out]*/ UINT8* pBuf,
        /*[in]*/  UINT32  cbBufSize,
        /*[out]*/ UINT32* pcbBlobSize
    );

    // Retrieves the length of a byte array associated with a key.
    HRESULT GetBlobSize(
        /*[in]*/  REFGUID guidKey,
        /*[out]*/ UINT32* pcbBlobSize
    );

    // Retrieves the number of attributes that are set on this object.
    HRESULT GetCount(
        /*[out]*/ UINT32* pcItems
    );

    // Retrieves a double value associated with a key.
    HRESULT GetDouble(
        /*[in]*/  REFGUID guidKey,
        /*[out]*/ double* pfValue
    );

    // Retrieves a GUID value associated with a key.
    HRESULT GetGUID(
        /*[in]*/  REFGUID guidKey,
        /*[out]*/ GUID* pguidValue
    );

    // Retrieves the value associated with a key.
    HRESULT GetItem(
        /*[in]*/      REFGUID     guidKey,
        /*[in, out]*/ PROPVARIANT* pValue
    );

    // Retrieves an attribute at the specified index.
    HRESULT GetItemByIndex(
        /*[in]*/      UINT32      unIndex,
        /*[out]*/     GUID* pguidKey,
        /*[in, out]*/ PROPVARIANT* pValue
    );

    // Retrieves the data type of the value associated with a key.
    HRESULT GetItemType(
        /*[in]*/  REFGUID           guidKey,
        /*[out]*/ MF_ATTRIBUTE_TYPE* pType
    );

    // Retrieves a wide-character string associated with a key.
    HRESULT GetString(
        /*[in]*/  REFGUID guidKey,
        /*[out]*/ LPWSTR  pwszValue,
        /*[in]*/  UINT32  cchBufSize,
        /*[out]*/ UINT32* pcchLength
    );

    // Retrieves the length of a string value associated with a key.
    HRESULT GetStringLength(
        /*[in]*/  REFGUID guidKey,
        /*[out]*/ UINT32* pcchLength
    );

    // Retrieves a UINT32 value associated with a key.
    HRESULT GetUINT32(
        /*[in]*/  REFGUID guidKey,
        /*[out]*/ UINT32* punValue
    );

    // Retrieves a UINT64 value associated with a key.
    HRESULT GetUINT64(
        /*[in]*/  REFGUID guidKey,
        /*[out]*/ UINT64* punValue
    );

    // Retrieves an interface pointer associated with a key.
    HRESULT GetUnknown(
        /*[in]*/  REFGUID guidKey,
        /*[in]*/  REFIID  riid,
        /*[out]*/ LPVOID* ppv
    );

    // Locks the attribute store so that no other thread can access it. 
    HRESULT LockStore();

    // Associates a byte array with a key.
    HRESULT SetBlob(
        /*[in]*/ REFGUID     guidKey,
        /*[in]*/ const UINT8* pBuf,
        /*[in]*/ UINT32      cbBufSize
    );

    // Associates a double value with a key.
    HRESULT SetDouble(
        /*[in]*/ REFGUID guidKey,
        /*[in]*/ double  fValue
    );

    // Associates a GUID value with a key.
    HRESULT SetGUID(
        /*[in]*/ REFGUID guidKey,
        /*[in]*/ REFGUID guidValue
    );

    // Adds an attribute value with a specified key.
    HRESULT SetItem(
        /*[in]*/ REFGUID        guidKey,
        /*[in]*/ REFPROPVARIANT Value
    );

    // Associates a wide-character string with a key.
    HRESULT SetString(
        /*[in]*/ REFGUID guidKey,
        /*[in]*/ LPCWSTR wszValue
    );

    // Associates a UINT32 value with a key.
    HRESULT SetUINT32(
        /*[in]*/ REFGUID guidKey,
        /*[in]*/ UINT32  unValue
    );

    // Associates a UINT64 value with a key.
    HRESULT SetUINT64(
        /*[in]*/ REFGUID guidKey,
        /*[in]*/ UINT64  unValue
    );

    // Associates an IUnknown pointer with a key.
    HRESULT SetUnknown(
        /*[in]*/ REFGUID  guidKey,
        /*[in]*/ IUnknown* pUnknown
    );

    // Unlocks the attribute store after a call to the IMFAttributes::LockStore method.
    HRESULT UnlockStore();

    // Finally, IUnknown Interface methods
    // see: https://docs.microsoft.com/en-us/windows/win32/api/unknwn/nn-unknwn-iunknown

    // Increments the reference count for an interface pointer to a COM object. 
    // You should call this method whenever you make a copy of an interface pointer
    STDMETHODIMP_(ULONG) AddRef() { return InterlockedIncrement(&m_refCount); };

    // Decrements the reference count for an interface on a COM object.
    STDMETHODIMP_(ULONG) Release() 
    {
        ULONG uCount = InterlockedDecrement(&m_refCount);
        if (uCount == 0)
        {
            delete this;
        }
        return uCount;
    }

    // Queries a COM object for a pointer to one of its interface; identifying the interface by a reference to its interface identifier (IID
    // If the COM object implements the interface, then it returns a pointer to that interface after calling IUnknown::AddRef on it.
    HRESULT QueryInterface(
        REFIID riid,
        void** ppvObject
    );
};

