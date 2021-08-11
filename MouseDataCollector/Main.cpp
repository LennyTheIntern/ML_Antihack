#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <time.h>
#include <string>
#include <array>


/*
// monitors the raw input from the mouse
// when the target is clicked then it will save the last X number of values.... idk what thatg number should be yet
// might be cool to show the data of each click as it is colected
// use prev collecteced values to train a model
// after a certain amount of time or just when specipified run all the colected mouse move to a tensorflow or pytorch
// if there are enough 
*/

const int NUMFEATURES = 4;
std::vector<std::string> mousefeaturlables = {"time","dx","dy","xrawline","yrawline","clickID"};
std::ofstream file;

LRESULT CALLBACK targetWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main()
{
    auto xx = mousefeaturlables.back();
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
    file.close();
    DestroyWindow(targetWindow);

    return 0;
}


// might be benificial to colect all of the mouse movement up to a half send before the gun was fired.
// rawmousedatastruct
// rawmouse event
LRESULT CALLBACK targetWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    // make a struct for this
    static std::vector<POINT> rawmousebuffor;
    static std::vector<long> rawmousetime;
    static int clickID = 0;
    static const int MAXRAWMOUSEBUFFERSIZE = 500;
    static long startclk = std::clock();
    
    switch (uMsg)
    {
        // print out the values that I need
        case WM_INPUT: {
            UINT dataSize = { 0 };
            GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER)); //Need to populate data size first
            if (dataSize > 0)
            {
                std::vector<BYTE> rawdata(dataSize); // should probaly just use a smart pointer

                if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.data(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
                {
                    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.data());
                    if (raw->header.dwType == RIM_TYPEMOUSE)
                    {
                        std::cout << raw->data.mouse.ulExtraInformation << '\n';
                        std::cout << clock()<<'\n';


                        POINT temp;
                        temp.x = raw->data.mouse.lLastX;
                        temp.y = raw->data.mouse.lLastY;
                        rawmousebuffor.push_back(temp);
                        rawmousetime.push_back(std::clock());

                        if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_1_DOWN)
                        {
                            if (!file.is_open())
                            {
                                file.open("MouseData.csv");
                                for (std::string element : mousefeaturlables)
                                {
                                    file << element;
                                    if (element != mousefeaturlables.back())
                                    {                 
                                       file << ',';
                                    }
                                    else
                                    {
                                        file << '\n';                                    
                                    }
                                }
                            }

                            int xrawmouseacc = 0;
                            int yrawmouseacc = 0;
                            int acc = 0;
                            //i would cum i I could make an enum
                            for (POINT i : rawmousebuffor)
                            {
                                xrawmouseacc += i.x;
                                yrawmouseacc += i.y;
                                file << rawmousetime[acc] << ',' << i.x << ',' << i.y << ',' << xrawmouseacc << ',' << yrawmouseacc << ',' << clickID << '\n';
                                acc++;
                            }
                            clickID++;
                        }
                    }
                    if (rawmousebuffor.size() > MAXRAWMOUSEBUFFERSIZE)
                    {
                        rawmousebuffor.erase(rawmousebuffor.begin());
                    }
                }
            }
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
        
    }
}