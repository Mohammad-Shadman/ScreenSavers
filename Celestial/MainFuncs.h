#include "MainConsts.h"

#define XYSPACE(x,y) (int)((int)y * SCRN_WIDTH + x)

//draw and calc functions
void scrprint(Vector root,Vector point,Pix32 pix){
    
    Vector a = VaddR(root,point);
     
    if (a.x < MIN_BOUND_X || a.y < MIN_BOUND_Y || a.x > MAX_BOUND_X || a.y > MAX_BOUND_Y) {return;}
    // PPix32 p = &(((PPix32)(gBackBuffer.Memory))[XYSPACE(a.x,a.y)]);
    // p->Blue *= p->Alpha/0xff;
    // p->Red *= p->Alpha/0xff;
    // p->Green *= p->Alpha/0xff;
    
    memcpy((PPix32)gBackBuffer.Memory+(XYSPACE(a.x,a.y)),&pix,sizeof(Pix32));

}

double Map(double start,double end, double setStart, double setEnd, double number){
    
    //caps number to macx or min we set
    number = ((number<start?start:number)>end?end:number);
    if (end-start == 0){
        return 0;
    }
    return (number/(end -start))*(setEnd-setStart)+setStart;
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

    for (double i = 0; i<=2*PI; i+=angle){
        scrprint(C.center,VscaleR(pointer,C.radius),pix);
        Vrotate(&pointer,angle);
    }
}

void DrawDot(Vector point, double radius, BOOL fill, Pix32 pix){
    Vector pointer = {0,1,1};
    if (radius <=0)
    {
        radius =1;
    }
    
    int count = fill>0? radius:1;
    Circle dot = {point,radius};
    for (size_t i = 0; i < count; i++)
    {
        if (dot.radius <1){dot.radius = 1;continue;}
        DrawCircle(dot,dot.radius*PI*4,pix);

        dot.radius--;
    }
    scrprint(VaddR(dot.center,VscaleR(dot.center,-1)),dot.center,pix);
    
}
//////////////////////////////////////////////////////////////////////////////
//program functions

void InitParticle(){
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    int speed = MAX_VELOCITY;
    srand(clock()+timeinfo->tm_min+timeinfo->tm_hour+timeinfo->tm_sec);
    for (size_t i = 0; i< PARTICLE_COUNT; i++){
        gParticleSet[i].pos.center = (Vector) {rand()%200 - rand()%200 + gCenterVec.x, rand()%200 - rand()%200 + gCenterVec.y, rand()%100 +50};
        gParticleSet[i].pos.radius = rand()%10+5;
        gParticleSet[i].vel = (Vector){0,0};
        gParticleSet[i].acc = (Vector){rand()%(speed+1)-speed/2,rand()%(speed+1)-speed/2};
        Vrotate(&gParticleSet[i].vel,(double)rand()/(rand()%100+1));
        gParticleSet[i].pixel = (Pix32){0xff-(int)Map(0,MAX_VELOCITY,0,0xff,VmagR(gParticleSet[i].vel)),0,(int)Map(0,MAX_VELOCITY,0,0xff,VmagR(gParticleSet[i].vel))};
        gParticleSet[i].isOutOFBounds = 0;
        gParticleSet[i].isCollided = 0;
        
    }
    
}


void UpdateParticles(){
    if (dT>0.16){dT=0.16;}
    Vector oldPos1,oldPos2;
    for (int i = 0; i< PARTICLE_COUNT; i++){
        
        //phisics
        BOOL xOutOfBounds = (gParticleSet[i].pos.center.x + gParticleSet[i].pos.radius > MAX_BOUND_X || gParticleSet[i].pos.center.x - gParticleSet[i].pos.radius < MIN_BOUND_X);
        BOOL yOutOfBounds = (gParticleSet[i].pos.center.y + gParticleSet[i].pos.radius > MAX_BOUND_Y || gParticleSet[i].pos.center.y - gParticleSet[i].pos.radius < MIN_BOUND_Y);
        //border boundery collision detection
        if (gParticleSet[i].isOutOFBounds==0){
        
            if (xOutOfBounds){

                VratioScale(&gParticleSet[i].vel,(Vector){-1,1});
                VratioScale(&gParticleSet[i].acc,(Vector){-1,1});
                gParticleSet[i].isOutOFBounds = 1;
                
            }

            if (yOutOfBounds){
                
                VratioScale(&gParticleSet[i].vel,(Vector){1,-1});
                VratioScale(&gParticleSet[i].acc,(Vector){1,-1});
                gParticleSet[i].isOutOFBounds = 1;

            }
        }
        if (yOutOfBounds==0 && xOutOfBounds==0){
                gParticleSet[i].isOutOFBounds = 0;
                
        }



        //handel gravity with all objects and circular colisions
        // for(int j = 0; j < PARTICLE_COUNT; j++){
        //     if(i==j){continue;}
        //     Vector forceDir = VsubR(gParticleSet[j].pos.center, gParticleSet[i].pos.center);
        //     double dist = VmagR(forceDir);
        //     if (gParticleSet[i].isCollided==1 && dist <= gParticleSet[j].pos.radius + gParticleSet[i].pos.radius){break;}
            
        //     if (gParticleSet[i].isCollided==0 && dist <= gParticleSet[j].pos.radius + gParticleSet[i].pos.radius){
        //         gParticleSet[i].isCollided=1;
                
        //         //gParticleSet[j].isCollided=1;

        //         Vrotate(&gParticleSet[i].vel,VangleBetween(VnormR(gParticleSet[i].vel),VnormR(forceDir)));
        //         //Vscale(&gParticleSet[i].acc,-1);
                
        //         //Vscale(&gParticleSet[j].vel,-1);
        //         //Vscale(&gParticleSet[j].acc,-1);
                
        //         break;
        //     }
        //     if(gParticleSet[i].isCollided==1 && dist > gParticleSet[j].pos.radius+gParticleSet[i].pos.radius){
        //         gParticleSet[i].isCollided=0;
        //     }
        // }

        Vector sumForces = (Vector){0,0};
        #define G_CONST 0.1
        for (int j = 0; j < PARTICLE_COUNT; j++)
        {   
            if (i!=j){
                
                Vector forceDir = VsubR(gParticleSet[j].pos.center,gParticleSet[i].pos.center);
                double dist = VmagR(forceDir);
                double forceQuant = G_CONST*gParticleSet[j].pos.center.m*gParticleSet[i].pos.center.m/(dist*dist);
                Vnorm(&forceDir);
                if(dist <= gParticleSet[j].pos.radius + gParticleSet[i].pos.radius){
                    Vadd(&gParticleSet[i].pos.center,VscaleR(forceDir,0.5*(dist-(gParticleSet[j].pos.radius + gParticleSet[i].pos.radius))));
                    Vadd(&gParticleSet[j].pos.center,VscaleR(forceDir,-0.5*(dist-(gParticleSet[j].pos.radius + gParticleSet[i].pos.radius))));
                    
                    Vadd(&sumForces,VscaleR(forceDir,-forceQuant*gParticleSet[i].pos.center.m/gParticleSet[j].pos.center.m));
                }else{
                
                    Vadd(&sumForces, VscaleR(forceDir,forceQuant));
                }
            }
        }
        
        gParticleSet[i].acc = VscaleR(sumForces,1/gParticleSet[i].pos.center.m);
        Vadd(&gParticleSet[i].vel,VscaleR(gParticleSet[i].acc,1));
        
        Vadd(&gParticleSet[i].pos.center,VscaleR(gParticleSet[i].vel,dT));
        gParticleSet[i].pixel = (Pix32){0xff-(int)Map(0,MAX_VELOCITY,0,0xff,VmagR(gParticleSet[i].vel)),0,(int)Map(0,MAX_VELOCITY,0,0xff,VmagR(gParticleSet[i].vel))};
        //draw main particle
        DrawDot(gParticleSet[i].pos.center,gParticleSet[i].pos.radius,1,gParticleSet[i].pixel);     
        
    }
}

void ReleaseTrail(){
    for (size_t i = 0; i < PARTICLE_COUNT; i++)
    {
        //LLfreeAll(&gParticleSet[i].trail);
        
    }
    
    
}



//////////////////////////////////////////////////////////////////////////////
//window management function

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
        /*
        case WM_CHAR:
            SendMessageA(gTheWindow, WM_CLOSE, 0,0);
            DestroyWindow(gTheWindow);
            isRunning = 0; 
            break;
        
        */
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




void RenderFrameGraphics(){
     

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
    UpdateParticles();
    
    

    ReleaseDC(gTheWindow,deviceContext);
}

void GetUserInput(){
    int16_t EscapeKeyDown = GetAsyncKeyState(VK_ESCAPE);
    if (EscapeKeyDown){
        SendMessageA(gTheWindow, WM_CLOSE, 0,0);
    }
}

