#include <stdint.h>
#include <windows.h>
#include <winuser.h>
#include <time.h>
#include <stdlib.h>
#include <threads.h>
#include "Vec.h"




#define SCRN_WIDTH 1920
#define SCRN_HEIGHT 1080
#define SCRN_BPP 32
#define SCRN_MEM_SIZE (SCRN_HEIGHT * SCRN_WIDTH * SCRN_BPP / 8)


typedef struct GameBitMap{
BITMAPINFO BitmapInfo;
void* Memory;
}GBM, *PGBM;


typedef struct Pixil32{
    uint8_t Blue;
    uint8_t Green;
    uint8_t Red;
    uint8_t Alpha;
    

}Pix32, *PPix32;

typedef struct Circle{
    Vector center;
    double radius;
} Circle,*PCircle;


////////////////////////////////////////////////////////////////////////////////
//constants

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

double dT =0;

Vector gVminutesHand, gVhoursHand, gVsecondsHand;

time_t rawtime;
struct tm * timeinfo;
    
//////////////////////////////////////////////////////////////////////////////

//clock Defines
#define PI 3.14159265358979323846
#define SECONDS_ANGLE  PI/60*2
#define MINUTES_ANGLE  PI/60/60*2
#define HOURS_ANGLE  PI/60/60/12*2
#define SECONDS_HAND_LENGTH 135*3
#define MINUTES_HAND_LENGTH 140*3
#define HOURS_HAND_LENGTH 90*3
#define PIXEL_RATIO_X 2.0
#define PIXEL_RATIO_Y 5.0
#define nScreenWidth  (1920.0)/PIXEL_RATIO_X        // Console Screen Size X (columns)
#define nScreenHeight (1080.0)/PIXEL_RATIO_Y		// Console Screen Size Y (rows)

Vector gCenterVec = {SCRN_WIDTH/2,SCRN_HEIGHT/2,0};



