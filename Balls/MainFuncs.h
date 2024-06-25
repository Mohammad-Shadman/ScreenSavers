#include "MainConsts.h"

#define XYSPACE(x,y) ((int)y * SCRN_WIDTH + x)

//draw and calc functions
void scrprint(Vector root,Vector point,Pix32 pix){
    
    Vector a = VaddR(root,point);
     
    if (a.x < MIN_BOUND_X || a.y < MIN_BOUND_Y || a.x > MAX_BOUND_X || a.y > MAX_BOUND_Y) {return;}
    memcpy((PPix32)gBackBuffer.Memory+(int)(XYSPACE(a.x,a.y)),&pix,sizeof(Pix32));

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
        if (dot.radius <1){dot.radius = 2;}
        DrawCircle(dot,dot.radius*dot.radius*PI+100,pix);

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
    for (size_t i = 0; i< gParticleCount; i++){
        gParticleSet[i].pos.center = (Vector) {rand()%200 - rand()%200 + gCenterVec.x, rand()%200 - rand()%200 + gCenterVec.y};
        gParticleSet[i].pos.radius = rand()%6+5;
        gParticleSet[i].vel = (Vector){rand()%(speed+1)-speed/2,rand()%(speed+1)-speed/2};
        gParticleSet[i].acc = (Vector){rand()%(speed+1)-speed/2,rand()%(speed+1)-speed/2};
        Vrotate(&gParticleSet[i].vel,(double)rand()/(rand()%100+1));
        gParticleSet[i].pixel = (Pix32){0xff-(int)Map(0,MAX_VELOCITY,0,0xff,VmagR(gParticleSet[i].vel)),0,(int)Map(0,MAX_VELOCITY,0,0xff,VmagR(gParticleSet[i].vel))};
        gParticleSet[i].isOutOFBounds = 0;
        //gParticleSet[i].trail = (PParticle)malloc(sizeof(Particle)*TRAIL_COUNT);
        //make trail
        // for (size_t j = 0; j < TRAIL_COUNT; j++)
        // {
        //
        //     //normalize velocity and then mult by j
        //     gParticleSet[i].trail[j].pos.radius = gParticleSet[i].pos.radius*((double)TRAIL_COUNT/(((double)TRAIL_COUNT+(double)j+1)));
        //     gParticleSet[i].trail[j].pos.center = VaddR(gParticleSet[i].pos.center,VscaleR(VnormR(gParticleSet[i].vel), -gParticleSet[i].pos.radius*(j+1)));
        //     gParticleSet[i].trail[j].vel = gParticleSet[i].vel;
        //     gParticleSet[i].trail[j].acc = gParticleSet[i].acc;
        //     gParticleSet[i].trail[j].pixel = (Pix){(gParticleSet[i].pixel.Blue)/(j+2), 0,gParticleSet[i].pixel.Red/(j+2)};
        //    
        // }
        LLinitList(&gParticleSet[i].trail,gParticleSet->pos);
        gParticleSet[i].trailSpawnTime = 1/(1+VmagR(gParticleSet[i].vel)/(MAX_VELOCITY/4));
        gParticleSet[i].timeOfLastSpawn = clock();
    }
    
}


void UpdateParticles(){
    if (dT>0.16){dT=0.16;}
    Vector oldPos1,oldPos2;
    for (size_t i = 0; i< gParticleCount; i++){
        oldPos1 = gParticleSet[i].pos.center;
        //phisics
        BOOL xOutOfBounds = (gParticleSet[i].pos.center.x + gParticleSet[i].pos.radius > MAX_BOUND_X || gParticleSet[i].pos.center.x - gParticleSet[i].pos.radius < MIN_BOUND_X);
        BOOL yOutOfBounds = (gParticleSet[i].pos.center.y + gParticleSet[i].pos.radius > MAX_BOUND_Y || gParticleSet[i].pos.center.y - gParticleSet[i].pos.radius < MIN_BOUND_Y);

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
        
        //Vadd(&gParticleSet[i].vel,VscaleR(gParticleSet[i].acc,dT));
        Vadd(&gParticleSet[i].pos.center,VscaleR(gParticleSet[i].vel,dT));
        gParticleSet[i].pixel = (Pix32){0xff-(int)Map(0,MAX_VELOCITY,0,gParticleSet[i].pixel.Blue,VmagR(gParticleSet[i].vel)),0,(int)Map(0,MAX_VELOCITY,0,gParticleSet[i].pixel.Red,VmagR(gParticleSet[i].vel))};
        //Vadd(&gParticleSet[i].acc,VscaleR(gParticleSet[i].acc,-0.2));

        //random crashes from here     
        //make trail with linked lists if its time to spawn it
        if((clock()-gParticleSet[i].timeOfLastSpawn)/CLOCKS_PER_SEC >= gParticleSet[i].trailSpawnTime){

            LLinsert(&gParticleSet[i].trail,0,gParticleSet[i].pos);
            gParticleSet[i].timeOfLastSpawn = clock();
        }
        long long cc = 0;
        NodePtr curr =  gParticleSet[i].trail.head;
            while (curr != NULL){                
                if (curr->data.radius < 1){
                    //LLremoveAtIndex(&gParticleSet[i].trail,cc);
                }else{
                    curr->data.radius*= (1-gParticleSet[i].trailSpawnTime*dT) > 0? (1-gParticleSet[i].trailSpawnTime*dT) :-(1-gParticleSet[i].trailSpawnTime*dT);
                    curr->data.pixel = (Pix){0xff-(int)Map(0,gParticleSet[i].pos.radius,1,0xfe,curr->data.radius),0,(int)Map(0,gParticleSet[i].pos.radius,1,0xfe,curr->data.radius)};
                    DrawDot(curr->data.center,curr->data.radius,1,curr->data.pixel);
                    cc++;
                }
                
                curr = curr->next;
            }
            

        //show trail
        // for(int j = TRAIL_COUNT-1; j>=0; j--){
        //     gParticleSet[i].trail[j].pos.radius *=0.9;
        //    
        //     //check the radius less than 0, if so, shift all the elemints in the array to the left and then place the one that shrank to the start
        //     if (gParticleSet[i].trail[j].pos.radius < 1){
        //         gParticleSet[i].trail[j].pos = gParticleSet[i].pos;
        //     }
        //     DrawDot(gParticleSet[i].trail[j].pos.center,gParticleSet[i].trail[j].pos.radius,1,gParticleSet[i].trail[j].pixel);
        // }
        /*
        for (int j = TRAIL_COUNT-1; j >= 0; j--)
        {   
            if (j<0) continue;
            BOOL txOutOfBounds = (gParticleSet[i].trail[j].pos.center.x + gParticleSet[i].trail[j].pos.radius > MAX_BOUND_X || gParticleSet[i].trail[j].pos.center.x - gParticleSet[i].trail[j].pos.radius < MIN_BOUND_X);
            BOOL tyOutOfBounds = (gParticleSet[i].trail[j].pos.center.y + gParticleSet[i].trail[j].pos.radius > MAX_BOUND_Y || gParticleSet[i].trail[j].pos.center.y - gParticleSet[i].trail[j].pos.radius < MIN_BOUND_Y);
            if (gParticleSet[i].trail[j].isOutOFBounds==0){
        
                if (txOutOfBounds){

                    VratioScale(&gParticleSet[i].trail[j].vel,(Vector){-1,1});
                    
                    gParticleSet[i].trail[j].isOutOFBounds = 1;
                    
                }

                if (tyOutOfBounds){
                    
                    VratioScale(&gParticleSet[i].trail[j].vel,(Vector){1,-1});
                    
                    gParticleSet[i].trail[j].isOutOFBounds = 1;

                }
            }
            if (tyOutOfBounds==0 && txOutOfBounds==0){
                    gParticleSet[i].trail[j].isOutOFBounds = 0;
                    
            }
            //Vadd(&gParticleSet[i].trail[j].pos.center,VscaleR(gParticleSet[i].trail[j].vel,dT));
            //gParticleSet[i].trail[j].pos.center = j-1 > 0 ? VaddR(gParticleSet[i].trail[j-1].pos.center,VscaleR(VnormR(gParticleSet[i].trail[j-1].vel),-1)) : VaddR(gParticleSet[i].pos.center,VscaleR(VnormR(gParticleSet[i].vel),-1));
            //gParticleSet[i].trail[j].pos.center = j-1 > 0 ? VaddR(gParticleSet[i].trail[j-1].pos.center,VscaleR( VnormR(gParticleSet[i].trail[j-1].vel), -gParticleSet[i].trail[j-1].pos.radius)) : VaddR(gParticleSet[i].pos.center,VscaleR( VnormR(gParticleSet[i].vel), -gParticleSet[i].trail[j-1].pos.radius));
            
            DrawDot(gParticleSet[i].trail[j].pos.center,gParticleSet[i].trail[j].pos.radius,1,gParticleSet[i].trail[j].pixel);

        }*/
        //DrawDot(gParticleSet[i].trail[0].pos.center,gParticleSet[i].trail[0].pos.radius,1,gParticleSet[i].trail[0].pixel);
        
        //draw main particle
        DrawDot(gParticleSet[i].pos.center,gParticleSet[i].pos.radius,1,gParticleSet[i].pixel);     
        
    }
}

void ReleaseTrail(){
    for (size_t i = 0; i < gParticleCount; i++)
    {
        LLfreeAll(&gParticleSet[i].trail);
        
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

//temp test vector, delete later


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

