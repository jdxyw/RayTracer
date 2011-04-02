#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "common.h"

namespace jdxyw
{
namespace RayTracer
{

class Ray
{
private:
    vector3 m_origin;
    vector3 m_direction;

public:
    Ray():m_origin(vector3(0,0,0)),m_direction(vector3(0,0,0)){}
    Ray(vector3& a_Origin, vector3& a_Dir):m_origin(a_Origin),m_direction(a_Dir){}

    void SetOrigin(vector3& a_orign){m_origin=a_orign;}
    void SetDirection(vector3& a_direction){m_direction=a_direction;}

    vector3& GetOrigin(){return m_origin;}
    vector3& GetDirection(){return m_direction;}
};

class Scene;
class Primitive;
class Engine
{
public:
    Engine();
    ~Engine();
    void SetTarget(Pixel* a_Dest,int a_width,int a_height);
    Scene* GetScene(){return m_Scene;}
    Primitive* RayTrace(Ray& a_ray,Color& a_Acc,int a_Depth, float a_RIndex, float& a_Dist);
    void InitRender();
    bool Render();

protected:
    float m_WX1, m_WY1, m_WX2, m_WY2, m_DX, m_DY, m_SX, m_SY;
    Scene* m_Scene;
    Pixel* m_Dest;
    int m_Width, m_Height, m_CurrLine, m_PPos;
    Primitive** m_LastRow;
};

}
}

#endif // RAYTRACER_H
