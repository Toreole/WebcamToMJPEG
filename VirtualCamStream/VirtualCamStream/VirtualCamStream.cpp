// VirtualCamStream.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <mfapi.h>
#include <mfcaptureengine.h>
#include <mfidl.h>
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mf.lib")

#include "CVirtualCamera.h"

int main()
{
    //example code largely taken from https://docs.microsoft.com/de-de/windows/win32/medfound/audio-video-capture-in-media-foundation 

    HRESULT hr;
    // holds the attributes for the search.
    IMFAttributes* pMFAttributes = NULL;
    // the amount of devices
    UINT32 count = 0;

    IMFActivate** ppDevices = NULL;

    // create an attribute to hold search criteria
    hr = MFCreateAttributes(&pMFAttributes, 1);

    // Request information on the video capture devices present.
    if (SUCCEEDED(hr))
    {
        hr = pMFAttributes->SetGUID(
            MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, // key = looking for source type
            MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID // value = looking for video capture device
        );
    }

    // enumerate the devices
    if (SUCCEEDED(hr))
    {
        hr = MFEnumDeviceSources(pMFAttributes, &ppDevices, &count); // enumerates all devices that match the attribute.
    }

    // stores a c lpw string
    LPWSTR str;
    // the length of str.
    UINT l;

    if (SUCCEEDED(hr))
    {
        ppDevices[0]->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &str, &l);
        // for now its easiest to just print each character of str individually.
        for (UINT i = 0; i < l; ++i)
        {
            char c = str[i];
            std::cout << c;
        }
    }

    //the categories of the camera we are defining.
    //NULL => defaults to KSCATEGORY_ VIDEO_CAMERA, VIDEO, and CAPTURE
    GUID* category = NULL;

    //pointer to a virtual camera. must not be nullptr.
    IMFVirtualCamera *pVCam = new CVirtualCamera();

    // fully create the virtual camera see https://docs.microsoft.com/en-us/windows/win32/api/mfvirtualcamera/nf-mfvirtualcamera-mfcreatevirtualcamera
    hr = MFCreateVirtualCamera(
        MFVirtualCameraType_SoftwareCameraSource,
        MFVirtualCameraLifetime_Session,
        MFVirtualCameraAccess_CurrentUser,
        L"Copy (copy)",
        L"{1E882AC0-923E-4183-BA56-F1E7C61ABDF8}",
        category,
        0, //categoryCount must be 0 if NULL is passed.
        &pVCam
    );
    if (SUCCEEDED(hr))
    {
        std::cout << "yay\n";
    }
}