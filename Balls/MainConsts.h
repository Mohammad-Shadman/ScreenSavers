#ifndef MAINCONSTS_H
#define MAINCONSTS_H

#include <stdint.h>
#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include <time.h>
#include <stdlib.h>
//#include <threads.h>
#include "Vec.h"
#include "LinkedLists.h"


//const defines
#define MAX_VELOCITY 40
#define PI 3.14159265358979323846
#define nScreenWidth  (1920.0)   // Console Screen Size X (columns)
#define nScreenHeight (1080.0)	// Console Screen Size Y (rows)

#define SCRN_WIDTH 1920
#define SCRN_HEIGHT 1080
#define SCRN_BPP 32
#define SCRN_MEM_SIZE (SCRN_HEIGHT * SCRN_WIDTH * SCRN_BPP / 8)



#define NORMAL_BOUNDS_ON
#ifndef NORMAL_BOUNDS_ON
#define MIN_BOUND_X 200
#define MIN_BOUND_Y 200
#define MAX_BOUND_X 1800
#define MAX_BOUND_Y 1000
#else
#define MIN_BOUND_X 1
#define MIN_BOUND_Y 1
#define MAX_BOUND_X 1919
#define MAX_BOUND_Y 1079
#endif



typedef struct GameBitMap{
BITMAPINFO BitmapInfo;
void* Memory;
}GBM, *PGBM;


typedef struct Particle{
    
    Circle pos;
    Vector vel;
    Vector acc;
    double mass;
    Pix32 pixel; 
    BOOL isOutOFBounds;
    LL trail;
    double trailSpawnTime;
    double timeOfLastSpawn;
    
}Particle,*PParticle;

/*
typedef struct Planet{
    int celestialCount;
    int cbSize;
    Particle mainBody;
    PParticle celestialBodies;


} Planet, *PPlanet;
*/

////////////////////////////////////////////////////////////////////////////////
//globals

typedef BOOL (WINAPI *PGNSI)(DPI_AWARENESS_CONTEXT);

const char* g_ClassName = "myWindowClass";

int isRunning = 1;

//back buffer or drawing surface
GBM gBackBuffer;

HWND gTheWindow;//handle for a window ()

MONITORINFO gMonitorInfo= {sizeof(MONITORINFO)};

int gMonitorWidth;
int gMonitorHeight;

clock_t start = 0;
clock_t end = 0;
time_t rawtime;
struct tm * timeinfo;



double dT =0;

//#define TRAIL_COUNT 20
Particle gParticleSet[100];
int gParticleCount = sizeof(gParticleSet)/sizeof(Particle);
//double trailSpawnTime = 0.333333;
//double timeOfLastSpawn = 0;


//////////////////////////////////////////////////////////////////////////////

//center of the scrn
Vector gCenterVec = {SCRN_WIDTH/2,SCRN_HEIGHT/2,0};


#endif