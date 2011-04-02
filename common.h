#ifndef COMMON_H
#define COMMON_H

#include "math.h"
#include "stdlib.h"

typedef unsigned int Pixel;

inline float Rand(float range)
{
    return ((float)rand()/RAND_MAX)*range;
}

namespace jdxyw{
namespace RayTracer{

#define DOT3(A,B)           (A.x*B.x+A.y*B.y+A.z*B.z)
#define NORMALIZE3(A)       {float l=1/sqrtf(A.x*A.x+A.y*A.y+A.z*A.z);A.x*=l;A.y*=l;A.z*=l;}
#define LENGTH3(A)          (sqrtf(A.x*A.x+A.y*A.y+A.z*A.z))
#define SQRLENGTH3(A)       (A.x*A.x+A.y*A.y+A.z*A.z)
#define SQRDISTANCE3(A,B)   ((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y)+(A.z-B.z)*(A.z-B.z))
#define EPSILON             (0.00001f)
#define TRACEDEPTH          6
#define PI                  3.141592653538979323462f

class vector3
{
public:
    vector3():x(0.0f),y(0.0f),z(0.0f){}
    vector3(float _x,float _y,float _z):x(_x),y(_y),z(_z){}
    void Set(float _x,float _y,float _z){x=_x;y=_y;z=_z;}
    void SetX(float _x){x=_x;}
    void SetY(float _y){y=_y;}
    void SetZ(float _z){z=_z;}
    void Normalize(){float l=1/Length();x*=l;y*=l;z*=l;}
    float Length(){return sqrtf(x*x+y*y+z*y);}
    float SqrtLength(){return x*x+y*y+z*z;}
    float Dot(vector3 _v){return x*_v.x+y*_v.y+z*_v.z;}
    vector3 Cross(vector3 b){return vector3( y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x );}

    void operator +=(vector3& av){x+=av.x;y+=av.y;z+=av.z;}
    void operator +=(vector3* av){x+=av->x;y+=av->y;z+=av->z;}
    void operator -=(vector3& av){x-=av.x;y-=av.y;z-=av.z;}
    void operator -=(vector3* av){x-=av->x;y-=av->y;z-=av->z;}
    void operator *= ( float f ) { x *= f; y *= f; z *= f; }
    void operator *= ( vector3& a_V ) { x *= a_V.x; y *= a_V.y; z *= a_V.z; }
    void operator *= ( vector3* a_V ) { x *= a_V->x; y *= a_V->y; z *= a_V->z; }
    vector3 operator- () const { return vector3( -x, -y, -z ); }
    friend vector3 operator + ( const vector3& v1, const vector3& v2 ) { return vector3( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z ); }
    friend vector3 operator - ( const vector3& v1, const vector3& v2 ) { return vector3( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z ); }
    friend vector3 operator + ( const vector3& v1, vector3* v2 ) { return vector3( v1.x + v2->x, v1.y + v2->y, v1.z + v2->z ); }
    friend vector3 operator - ( const vector3& v1, vector3* v2 ) { return vector3( v1.x - v2->x, v1.y - v2->y, v1.z - v2->z ); }
    friend vector3 operator * ( const vector3& v, float f ) { return vector3( v.x * f, v.y * f, v.z * f ); }
    friend vector3 operator * ( const vector3& v1, vector3& v2 ) { return vector3( v1.x * v2.x, v1.y * v2.y, v1.z * v2.z ); }
    friend vector3 operator * ( float f, const vector3& v ) { return vector3( v.x * f, v.y * f, v.z * f ); }
    union
    {
        struct{float x,y,z;};
        struct{float r,g,b;};
        struct{float cell[3];};
    };
};

class plane
{
public:
    plane():N(0,0,0),D(0){}
    plane(vector3 a_Normal,float a_D):N(a_Normal),D(a_D){}
    union
    {
        struct
        {
            vector3 N;
            float D;
        };
        float cell[4];
    };
    //vector3 N;
    //float D;
};

typedef vector3 Color;

}
}

#endif // COMMON_H
