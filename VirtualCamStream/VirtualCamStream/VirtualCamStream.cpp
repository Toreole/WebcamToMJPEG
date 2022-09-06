// VirtualCamStream.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "Ole32.lib")

#include <iostream>
#include <string>
#include <objbase.h>
#include <Dshow.h>

#include "CVirtualCamera.h"

int main()
{
    //const GUID* pVideoInputDeviceCategory = &CLSID_VideoInputDeviceCategory;
    
    // Create the System Device Enumerator.
    HRESULT hr = 0L;
    ICreateDevEnum *pSysDevEnum = NULL;

    hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pSysDevEnum);

    if (FAILED(hr))
    {
        if (hr == REGDB_E_CLASSNOTREG)
            std::cout << "Reason: class not registered in database. or type of server is not registered, or registry corrupt.\n";
        if (hr == CLASS_E_NOAGGREGATION)
            std::cout << "Reason: This class cannot be created as part of an aggregate.\n";
        if (hr == E_NOINTERFACE)
            std::cout << "Reason: specified class does not implement requested interface\n";
        if (hr == E_POINTER)
            std::cout << "Reason: ppv is NULL\n";
        if (hr == S_OK)
            std::cout << "wtf why does it think OK is fail?\n";
        std::cout << hr << '\n';
        std::cout << "woah it failed on the very first step. Great.\n";
        return -666;
    }

    // Obtain a class enumerator for the video capture category.
    IEnumMoniker* pEnumCat = NULL;
    hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0); //--what do flags do in here?
    if (hr == S_OK) //will return S_Fail (success fail) when 0 devices of that category are found.
    {

        // Enumerate the monikers.
        IMoniker* pMoniker = NULL;
        ULONG cFetched; //count of fetched objects.

        while (pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK) //until the end is reached
        {
            IPropertyBag* pPropBag = NULL;
            //bind the moniker to the prop bag.
            hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pPropBag);
            if (SUCCEEDED(hr))
            {
                //retrieve the filters friendly (human readable) name do the following:
                VARIANT varName;
                VariantInit(&varName);
                //read the FriendlyName property of the filter.
                hr = pPropBag->Read(L"FriendlyName", &varName, 0);
                if (SUCCEEDED(hr))
                {
                    std::cout << varName.bstrVal;
                }
                VariantClear(&varName);
            }
        }


    }

    pSysDevEnum->Release();
}