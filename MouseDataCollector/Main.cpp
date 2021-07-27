#include <iostream>
#include <vector>
#include <Windows.h>

/*
// monitors the raw input from the mouse
// when the target is clicked then it will save the last X number of values.... idk what thatg number should be yet
// might be cool to show the data of each click as it is colected
// use prev collecteced values to train a model
// after a certain amount of time or just when specipified run all the colected mouse move to a tensorflow or pytorch
// if there are enough 
*/
LRESULT CALLBACK targetWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS wc = {};
    wc.lpfnWndProc = targetWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("MyRawInputWnd");

    if (!RegisterClass(&wc))
        return -1;

    HWND targetWindow = CreateWindowEx(0, wc.lpszClassName, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, hInstance, NULL);
    if (!targetWindow)
        return -1;

    //register the monitoring device
    RAWINPUTDEVICE rid = {};
    rid.usUsagePage = 0x01; //Mouse
    rid.usUsage = 0x02;
    rid.dwFlags = RIDEV_INPUTSINK;
    rid.hwndTarget = targetWindow;

    if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
        return -1;

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DestroyWindow(targetWindow);

    return 0;
}

LRESULT CALLBACK targetWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        // print out the values that I need
    case WM_INPUT: {
        UINT dataSize = { 0 };
        GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER)); //Need to populate data size first
        if (dataSize > 0)
        {
            std::vector<BYTE> rawdata(dataSize);

            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.data(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
            {
                RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.data());
                if (raw->header.dwType == RIM_TYPEMOUSE)
                {

                    std::cout << "RAWDATA ->";
                    std::cout <<" X: " << raw->data.mouse.lLastX;
                    std::cout <<" Y: " << raw->data.mouse.lLastY;
                    std::cout <<" mousebutton Flags: " << raw->data.mouse.usButtonFlags << std::endl;
                }
            }
        }
        return 0;
    }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}