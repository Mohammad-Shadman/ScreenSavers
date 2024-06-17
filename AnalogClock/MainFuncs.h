#include "MainConsts.h"

#define XYSPACE(x,y) ((int)y * SCRN_WIDTH + x)


void scrprint(Vector root,Vector point,Pix32 pix){
    
    Vector a = VaddR(root,point);
     
    memcpy((PPix32)gBackBuffer.Memory+(int)(XYSPACE(a.x,a.y)),&pix,sizeof(Pix32));

}

void mlerp (Vector root, Vector endP, double count, int thickness, Pix32 pix){
    double inc = 1/count;
    //Vector rot = VTurn90R(endP);
    for (double i = 0; i <1 ; i+=inc){
        scrprint(root,VscaleR(endP,i), pix);
    }   
}

void DrawCircle(Circle C, double definition, Pix32 pix){
    Vector pointer = {0, -1,1};
    double angle = 2*PI/definition;

    for (double i = 0; i<2*PI; i+=angle){
        scrprint(C.center,VscaleR(pointer,C.radius),pix);
        Vrotate(&pointer,angle);
    }
}

void DrawDot(Vector point, double radius, BOOL fill, Pix32 pix){
    Vector pointer = {0,1,1};
    int count = fill>0? radius:1;
    Circle dot = {point,radius};
    for (size_t i = 0; i < count; i++)
    {
        DrawCircle(dot,dot.radius*radius*radius+10,pix);
        dot.radius--;
    }
    


    
    
}


void initHands(){
    gVsecondsHand.y = SECONDS_HAND_LENGTH;
    gVminutesHand.y = MINUTES_HAND_LENGTH;
    gVhoursHand.y = HOURS_HAND_LENGTH;
    gVsecondsHand.m = SECONDS_HAND_LENGTH;
    gVminutesHand.m = MINUTES_HAND_LENGTH;
    gVhoursHand.m = HOURS_HAND_LENGTH;
    gVsecondsHand.x = 0;
    gVminutesHand.x = 0;
    gVhoursHand.x = 0;
    
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    //angle between 12o'clock and current time
    Vrotate(&gVsecondsHand,-(2*PI/60)*((double)(timeinfo->tm_sec)));
    Vrotate(&gVminutesHand,-(2*((double)timeinfo->tm_min+(double)(timeinfo->tm_sec)/60)*PI)/60);
    Vrotate(&gVhoursHand,-(2*((double)timeinfo->tm_hour+((double)timeinfo->tm_min/60+(double)(timeinfo->tm_sec)/60/60))*PI)/12);


}

void CreatClock(Circle c, Pix32 pix, long deltaTime){
    Circle clockHourDials = {c.center,c.radius*0.8};
    Circle clockMinDials = {c.center,c.radius*0.95};
    DrawCircle(c,10000,pix);
    //DrawCircle(clockHourDials,12,pix);
    //DrawCircle(clockMinDials,60,pix);
    

    //do hours first
    Vector dailPointer = {0,clockHourDials.radius,clockHourDials.radius};
    for (size_t i = 0; i < 12; i++)
    {
        DrawDot(VaddR(gCenterVec,dailPointer),5,1,pix);
        Vrotate(&dailPointer,PI/6);
    }
    //do mins next
    dailPointer = (Vector){0,clockMinDials.radius,clockMinDials.radius};
    for (size_t i = 0; i < 60; i++)
    {
        DrawDot(VaddR(gCenterVec,dailPointer),2,1,pix);
        Vrotate(&dailPointer,PI/30);
    }

    //make hands and rotate them
    
    
    initHands();

    mlerp(gCenterVec, gVsecondsHand, gVsecondsHand.m *2, 1, pix);
    mlerp(gCenterVec, gVminutesHand, gVminutesHand.m *2, 1, pix);
    mlerp(gCenterVec, gVhoursHand ,  gVhoursHand.m   *2, 1, pix);
    // if (dT >=1 ){
    //     Vrotate(&gVsecondsHand, -SECONDS_ANGLE);
    //     Vrotate(&gVminutesHand, -MINUTES_ANGLE);
    //     Vrotate(&gVhoursHand, -HOURS_ANGLE );
    // }
    //mlerp(gCenterVec,*pointer,VmagR(*pointer)*3, 1, pix);
    //Vrotate(pointer,-SECONDS_ANGLE*dT);


}

//////////////////////////////////////////////////////////////////////////////


void forceToFront(HWND hWnd) {
HWND foregroundWindow = GetForegroundWindow();

if (foregroundWindow == hWnd) {
    // Window is already Foreground-window
    return;
}

if (SetForegroundWindow(hWnd)) {
    // could set window to foreground without any tricks
    return;
}

// attach thread of foreground-window to this window
DWORD foregroundThread = GetWindowThreadProcessId(foregroundWindow, NULL);
DWORD myThread = GetWindowThreadProcessId(hWnd, NULL);
AttachThreadInput(foregroundThread, myThread, 1);
SetForegroundWindow(hWnd);
AttachThreadInput(foregroundThread, myThread, 1);
}

//makes the window procedure (the brains of the window)
LRESULT CALLBACK WndProc(HWND gTheWindow, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
        case WM_CHAR:
            SendMessageA(gTheWindow, WM_CLOSE, 0,0);
            DestroyWindow(gTheWindow);
            isRunning = 0; 
            break;
        case WM_CLOSE:
            DestroyWindow(gTheWindow);
            isRunning = 0; 
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(gTheWindow, msg, wParam, lParam);
    }
    return 0;
}

void mCreateWindow(){
    
    //adjust for the zoom of desktop zoom
    PGNSI pGNSI = (PGNSI) GetProcAddress(GetModuleHandleA((LPCSTR)TEXT("user32.dll")), "SetProcessDpiAwarenessContext");
    if (NULL != pGNSI) {
        if (!pGNSI(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) {
            //winerr();
            isRunning =0;
        }
    }
    
    ShowWindow(GetConsoleWindow(),SW_HIDE);
    WNDCLASSEXA wc;
    
    //assigning the window class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;//function pointer
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_CROSS);
    wc.hbrBackground = CreateSolidBrush(RGB(170,100,230));
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_ClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    //check to see if wc or window class was initiallized properly
    if (!RegisterClassExA(&wc)){
        MessageBoxA(NULL, "Window Registration Failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        isRunning =0;
    }

    //create the window   /the style of window (WinStyle)                                         /can be CW_USEDEFAULT /pointer to parent /pointer to instance application
    //                                     /name of class /title of win     /(WinStyle)          / X,Y coords  /x,y size        /pointer to menu  /window creation data
    gTheWindow = CreateWindowExA(WS_EX_CLIENTEDGE, g_ClassName, "My first Window", WS_OVERLAPPEDWINDOW, 0, 0 , 1920, 1080, NULL, NULL, GetModuleHandle(NULL), NULL);
    
   
    
    //check to see if all went well
    if (gTheWindow == NULL){
        MessageBoxA(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        isRunning =0;
    }

    forceToFront(gTheWindow);
}

//temp test vector, delete later


void RenderFrameGraphics(){
    Pix32 Pixle= {0};
    Pixle.Blue = 0xff;
    Pixle.Green = 0x00;
    Pixle.Red = 0x00;
    Pixle.Alpha = 0xff;

    Circle ClockOuterRing = {gCenterVec,450};

    HDC deviceContext = GetDC(gTheWindow);
    
    StretchDIBits(deviceContext,
                    0, 0, gMonitorWidth, gMonitorHeight,
                    0, 0, SCRN_WIDTH, SCRN_HEIGHT,
                    gBackBuffer.Memory, &gBackBuffer.BitmapInfo,
                    DIB_RGB_COLORS, SRCCOPY);

    // for (size_t i = 0; i < SCRN_WIDTH*SCRN_HEIGHT/2; i++)
    // {
    //     Pixle.Blue  = i%0xff;
    //     Pixle.Green = (i+0x88)%0xff;
    //     Pixle.Red   = (i+0xcf)%0xff;
    //     memcpy((PPix32)gBackBuffer.Memory +i , &Pixle,sizeof(Pix32));
    // }
    
    
    //manipulate buffer
    //clear
    memset(gBackBuffer.Memory,0,SCRN_MEM_SIZE);
    
    CreatClock(ClockOuterRing,Pixle,dT);

    ReleaseDC(gTheWindow,deviceContext);
}

void GetUserInput(){
    int16_t EscapeKeyDown = GetAsyncKeyState(VK_ESCAPE);
    if (EscapeKeyDown){
        SendMessageA(gTheWindow, WM_CLOSE, 0,0);
    }
}

