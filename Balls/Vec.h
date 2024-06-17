#ifndef VEC_H
#define VEC_H

#include <math.h>
typedef struct Vector{
    double x,y,m;

} Vector, *PVector;

void VSetEqual (PVector v1,Vector v2){
    v1->x = v2.x;
    v1->y = v2.y;
    v1->m = v2.m;
}

void Vadd (PVector v1,Vector v2){
    v1->x += v2.x;
    v1->y += v2.y;
    
    
}

void Vsub (PVector v1,Vector v2){
    v1->x -= v2.x;
    v1->y -= v2.y;
}

void Vscale (PVector v1, float s1){
    v1->x*=s1;
    v1->y*=s1;
    v1->m*=s1;
}

Vector VaddR (Vector v1,Vector v2){
    return (Vector){v1.x + v2.x, v1.y + v2.y};
    
}

Vector VsubR (Vector v1,Vector v2){
    return (Vector){v1.x - v2.x, v1.y - v2.y};
}

Vector VscaleR (Vector v1, double s1){
    return (Vector){v1.x * s1, v1.y * s1};
}

float Vdot(Vector A, Vector B){
    return (A.x* B.x + A.y * B.y);
}

double VmagR(Vector v1){
    return sqrt(v1.x*v1.x + v1.y*v1.y);;
}

void Vmag(PVector v1){
    v1->m =  sqrt(v1->x*v1->x + v1->y*v1->y);
}

void Vnorm(PVector v1) {
    Vmag(v1);
    if(v1->m==0) return;
    v1->x /= v1->m;
    v1->y /= v1->m;
    v1->m = 1;
}

Vector VnormR(Vector v1){
    double m = VmagR(v1);
    if (m==0)return (Vector){0,0,0};
    
    return VscaleR(v1,1/m);
}

void VratioScale(PVector v1,Vector scalars){
    v1->x *= scalars.x;
    v1->y *= scalars.y;

}
Vector VratioScaleR(Vector v1,Vector scalars){
    v1.x *= scalars.x;
    v1.y *= scalars.y;
    return v1;
}

void Vrotate (PVector v1, double angle){
    double temp = v1->x;
    v1->x = v1->x*cosf(angle) - v1->y*sinf(angle);
    v1->y = temp*sinf(angle) + v1->y*cosf(angle);

}

Vector VTurn90R(Vector v){
    return VnormR((Vector){-v.y,v.x});
}

Vector VrotateR (Vector v1, double angle){
    double temp = v1.x;
    v1.x = (v1.x * cos(angle)) - (v1.y * sin(angle));
    v1.y = (temp * sin(angle)) + (v1.y * cos(angle));
    return v1;
}
//cos -sin |  x
//         |
//sin cos  |  y

#endif