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
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////The simplest way to deal with the light
//                if(p->IsLight())
//                {
//                    Primitive* light=p;
//                    vector3 L=((Sphere*)light)->GetCentre() - pi;
//                    NORMALIZE3( L )
//                    vector3 N=prim->GetNormal(pi);
//                    if ((prim->GetMaterial())->GetDiffuse() > 0)
//                    {
//                        float dot = DOT3( N, L );
//                        if (dot > 0)
//                        {
//                            float diff = dot * prim->GetMaterial()->GetDiffuse();
//                            // add diffuse component to ray color
//                            a_Acc += diff * prim->GetMaterial()->GetColor() * light->GetMaterial()->GetColor();
//                        }
//                    }
//                }

                // handle point light source
                if(p->IsLight())
                {
                    Primitive* light=p;
                    float shade=1.0f;
                    if(light->GetType()==Primitive::SPHERE)
                    {
                        vector3 L=((Sphere*)light)->GetCentre() - pi;
                        float tdist=LENGTH3( L );
                        L *= (1.0f / tdist);
                        Ray r = Ray( pi + L * EPSILON, L );
                        for ( int s = 0; s < m_Scene->GetNrPrimitives(); s++ )
                        {
                            Primitive* pr = m_Scene->GetPrimitive( s );
                            if ((pr != light) && (pr->Intersect( r, tdist )))
                            {
                                shade = 0;
                                break;
                            }
                        }
                    }

                    if(shade>0)
                    {

                    //calucate diffuse shading
                        vector3 L = ((Sphere*)light)->GetCentre() - pi;
                        NORMALIZE3( L )
                        vector3 N = prim->GetNormal( pi );
                        if (prim->GetMaterial()->GetDiffuse() > 0)
                        {
                            float dot = DOT3( L, N );
                            if (dot > 0)
                            {
                                float diff = dot * prim->GetMaterial()->GetDiffuse() * shade;
                                // add diffuse component to ray color
                                a_Acc += diff * light->GetMaterial()->GetColor() * prim->GetMaterial()->GetColor();
                            }
                        }
                        // determine specular component
                        if (prim->GetMaterial()->GetSpecular() > 0)
                        {
                            // point light source: sample once for specular highlight
                            vector3 V = a_ray.GetDirection();
                            vector3 R = L - 2.0f * DOT3( L, N ) * N;
                            float dot = DOT3( V, R );
                            if (dot > 0)
                            {
                                float spec = powf( dot, 20 ) * prim->GetMaterial()->GetSpecular() * shade;
                                // add specular component to ray color
                                a_Acc += spec * light->GetMaterial()->GetColor();
                            }
                        }
                    }
                }
            }

            // calculate reflection
            float refl = prim->GetMaterial()->GetReflection();
            if (refl > 0.0f)
            {
                vector3 N = prim->GetNormal( pi );
                vector3 R = a_ray.GetDirection() - 2.0f * DOT3( a_ray.GetDirection(), N ) * N;
                if (a_Depth < TRACEDEPTH)
                {
                    Color rcol( 0, 0, 0 );
                    float dist;
                    RayTrace( Ray( pi + R * EPSILON, R ), rcol, a_Depth + 1, a_RIndex, dist );
                    a_Acc += refl * rcol * prim->GetMaterial()->GetColor();
                }
            }

            //calculate refraction
//            float refr = prim->GetMaterial()->GetRefraction();
//            if ((refr > 0) && (a_Depth < TRACEDEPTH))
//            {
//                float rindex = prim->GetMaterial()->GetRefrIndex();
//                float n = a_RIndex / rindex;
//                vector3 N = prim->GetNormal( pi ) * (float)result;
//                float cosI = -DOT3( N, a_ray.GetDirection() );
//                float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
//                if (cosT2 > 0.0f)
//                {
//                        vector3 T = (n * a_ray.GetDirection()) + (n * cosI - sqrtf( cosT2 )) * N;
//                        Color rcol( 0, 0, 0 );
//                        float dist;
//                        RayTrace( Ray( pi + T * EPSILON, T ), rcol, a_Depth + 1, rindex, dist );
//                        // apply Beer's law
//                        Color absorbance = prim->GetMaterial()->GetColor() * 0.15f * -dist;
//                        Color transparency = Color( expf( absorbance.r ), expf( absorbance.g ), expf( absorbance.b ) );
//                        a_Acc += rcol * transparency;
//                }
//            }
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
