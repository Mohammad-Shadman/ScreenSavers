#include "MainFuncs.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
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
    InitParticle();
    while (isRunning > 0){
        //get time

        start = clock();
        
        //set up the message loop for the window (the heart of the window)
        //peekmsg will let things pass and wont hold up the program
        while (PeekMessage(&Msg, NULL, 0 ,0, PM_REMOVE) > 0){
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
            
        }
        
        GetUserInput();

        RenderFrameGraphics();
        Sleep(0);
        end = clock();
        dT = (double)(end - start)/(double)1000;        
        
    }
    ReleaseTrail();
    VirtualFree(gBackBuffer.Memory, SCRN_MEM_SIZE, MEM_DECOMMIT);
    return Msg.wParam;

}