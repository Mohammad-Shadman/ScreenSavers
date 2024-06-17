/*
https://www.youtube.com/watch?v=jKb84Ucs9hA&list=PLlaINRtydtNWuRfd4Ra3KeD6L9FP_tDE7&index=4
http://www.winprog.org/tutorial/simple_window.html
this is the website that im using to learn all this.


the compile code
gcc .\WindowedApp.c -oWindowedAppscr -lgdi32

*/

#include "MainFuncs.h"



void mCreateWindow();

void RenderFrameGraphics();

void GetUserInput();



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{   
    MSG Msg;
    

    mCreateWindow();

    if ( GetMonitorInfo(MonitorFromWindow(gTheWindow, MONITOR_DEFAULTTOPRIMARY), &gMonitorInfo) ==0){
        MessageBox(NULL,"failed monitor aquisition","error",MB_OK);
        return 0;
    } 

    gMonitorWidth = gMonitorInfo.rcMonitor.right - gMonitorInfo.rcMonitor.left;
    gMonitorHeight = gMonitorInfo.rcMonitor.bottom - gMonitorInfo.rcMonitor.top;

    //potentially problematic, redraws white.
    SetWindowLongPtrA(gTheWindow,GWL_STYLE, (WS_OVERLAPPED | WS_VISIBLE) & ~WS_OVERLAPPED);    
    if(SetWindowPos(gTheWindow, HWND_TOP, gMonitorInfo.rcMonitor.left, gMonitorInfo.rcMonitor.top, gMonitorWidth, gMonitorHeight, SWP_FRAMECHANGED)== 0){
        isRunning = 0;
    }

    

    //show the window and update it's contents    
    ShowWindow(gTheWindow, nCmdShow);
    UpdateWindow(gTheWindow);

    gBackBuffer.BitmapInfo.bmiHeader.biSize = sizeof(gBackBuffer.BitmapInfo.bmiHeader);
    gBackBuffer.BitmapInfo.bmiHeader.biWidth = SCRN_WIDTH;
    gBackBuffer.BitmapInfo.bmiHeader.biHeight = SCRN_HEIGHT;
    gBackBuffer.BitmapInfo.bmiHeader.biBitCount = SCRN_BPP;
    gBackBuffer.BitmapInfo.bmiHeader.biCompression = BI_RGB;
    gBackBuffer.BitmapInfo.bmiHeader.biPlanes = 1;
    
    if ((gBackBuffer.Memory = (void*)VirtualAlloc(NULL, SCRN_MEM_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE))==NULL)//like malloc but good with large mem allocs
    {
        MessageBox(NULL, "FAILED TO ALLOC MEMOR FOR THE SCRN", "error",MB_OK);
        return 0;
    }
    
    end = clock();
    start = clock();
    initHands();
    while (isRunning > 0){
        //get time

        start = dT >= 1? clock() : start;
        


        //set up the message loop for the window (the heart of the window)
        //peekmsg will let things pass and wont hold up the program
        while (PeekMessage(&Msg, NULL, 0 ,0, PM_REMOVE) > 0){
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
            
        }
        GetUserInput();
        //dT = (double)(end - start)/(double)1000;
        RenderFrameGraphics();
        
        
        //need a work around for the clocks, because of sleep, i cant get accurate time reading do i need to put 
        //the clocks on a seperat thread and make an integrator for the dT var so i can calculate of a set time interval(1 loop instance)
        //learn to use threads and work on that
        Sleep(1);
        end = clock();
        dT = (double)(end - start)/(double)1000;
        
        
    }
    VirtualFree(gBackBuffer.Memory, SCRN_MEM_SIZE, MEM_DECOMMIT);
    return Msg.wParam;

}