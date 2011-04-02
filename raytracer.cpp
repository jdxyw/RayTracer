#include "scene.h"
#include "common.h"
#include "raytracer.h"
#include "stdlib.h"
#include "stdio.h"
#include "memory.h"

using namespace jdxyw::RayTracer;

namespace jdxyw
{
namespace RayTracer
{


    Engine::Engine()
    {
        m_Scene=new Scene();
    }

    Engine::~Engine()
    {
        delete m_Scene;
    }

    void Engine::SetTarget(Pixel *a_Dest, int a_width, int a_height)
    {
        m_Dest=a_Dest;
        m_Width=a_width;
        m_Height=a_height;
    }

    Primitive* Engine::RayTrace(Ray &a_ray, Color &a_Acc, int a_Depth, float a_RIndex, float &a_Dist)
    {
        if(a_Depth>TRACEDEPTH) return 0;

        a_Dist=1000000.0f;
        vector3 pi;
        Primitive *prim=0;
        int result=0;

        for(int s=0;s<m_Scene->GetNrPrimitives();s++)
        {
            Primitive* pr=m_Scene->GetPrimitive(s);
            int res;
            if(res=pr->Intersect(a_ray,a_Dist))
            {
                prim=pr;
                result=res;
            }
        }

        if(!prim) return 0;

        if(prim->IsLight())
        {
            a_Acc=Color(1,1,1);
        }
        else
        {
            //pi is the intersection point
            pi=a_ray.GetOrigin()+a_ray.GetDirection()*a_Dist;
            for(int l=0;l<m_Scene->GetNrPrimitives();l++)
            {
                Primitive* p=m_Scene->GetPrimitive(l);
                if(p->IsLight())
                {
                    Primitive* light=p;
                    vector3 L=((Sphere*)light)->GetCentre() - pi;
                    NORMALIZE3( L )
                    vector3 N=prim->GetNormal(pi);
                    if ((prim->GetMaterial())->GetDiffuse() > 0)
                    {
                        float dot = DOT3( N, L );
                        if (dot > 0)
                        {
                            float diff = dot * prim->GetMaterial()->GetDiffuse();
                            // add diffuse component to ray color
                            a_Acc += diff * prim->GetMaterial()->GetColor() * light->GetMaterial()->GetColor();
                        }
                    }
                }
            }
        }
        return prim;
    }

    void Engine::InitRender()
    {
        m_WX1 = -4, m_WX2 = 4, m_WY1 = m_SY = 3, m_WY2 = -3;
                // calculate deltas for interpolation
        m_DX = (m_WX2 - m_WX1) / m_Width;
        m_DY = (m_WY2 - m_WY1) / m_Height;
        //m_SY += 20 * m_DY;
        // allocate space to store pointers to primitives for previous line
        m_LastRow = new Primitive*[m_Width];
        memset( m_LastRow, 0, m_Width * 4 );
        m_PPos=0;
    }

    bool Engine::Render()
    {
        vector3 o(0,0,-5);

        Primitive* lastprim=0;
        for(int y=0;y<m_Height;y++)
        {
            m_SX=m_WX1;
            for(int x=0;x<m_Width;x++)
            {
                Color acc( 0, 0, 0 );
                vector3 dir = vector3( m_SX, m_SY, 0 ) - o;
                NORMALIZE3( dir )
                Ray r( o, dir );
                float dist;
                Primitive* prim = RayTrace( r, acc, 1, 1.0f, dist );
                int red = (int)(acc.r * 256);
                int green = (int)(acc.g * 256);
                int blue = (int)(acc.b * 256);
                if (red > 255) red = 255;
                if (green > 255) green = 255;
                if (blue > 255) blue = 255;
                m_Dest[m_PPos++] = (red << 16) + (green << 8) + blue;
                m_SX += m_DX;
            }
            m_SY+=m_DY;
        }
        return true;
    }
}
}
