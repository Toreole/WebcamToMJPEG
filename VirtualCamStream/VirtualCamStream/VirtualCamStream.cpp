// VirtualCamStream.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <mfapi.h>
#include <mfcaptureengine.h>
#include <mfidl.h>
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mf.lib")

#include <atlstr.h>

int main()
{
    HRESULT hr;
    IMFAttributes* pMFAttributes = NULL;
    UINT32 count = 0;

    IMFActivate** ppDevices = NULL;

    //create an attribute to hold search criteria
    hr = MFCreateAttributes(&pMFAttributes, 1);

    //Request information on the video capture devices present.
    if (SUCCEEDED(hr))
    {
        hr = pMFAttributes->SetGUID(
            MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
            MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
        );
    }

    //enumerate the devices
    if (SUCCEEDED(hr))
    {
        hr = MFEnumDeviceSources(pMFAttributes, &ppDevices, &count);
    }

    LPWSTR str;
    UINT l;

    if (SUCCEEDED(hr))
    {
        ppDevices[0]->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &str, &l);
        for (UINT i = 0; i < l; ++i)
        {
            char c = str[i];
            std::cout << c;
        }
    }


    std::cout << '\n';

    std::cout << "Hello World!\n";
}