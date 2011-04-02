
#include "common.h"
#include "scene.h"
#include "raytracer.h"

#include <string>

namespace jdxyw
{
namespace RayTracer
{

    void jdxyw::RayTracer::Primitive::SetName(char *a_Name)
    {
        delete m_Name;
        m_Name=new char[strlen(a_Name)+1];
        strcpy(m_Name,a_Name);
    }

    jdxyw::RayTracer::Material::Material():
            m_Color(Color(0.2f,0.2f,0.2f)),
            m_Refl(0),m_Diff(0.2f)
    {}

    int jdxyw::RayTracer::Sphere::Intersect(Ray &a_Ray, float &a_Dist)
    {
        vector3 v=a_Ray.GetOrigin()-m_Centre;
        float b=-DOT3(v,a_Ray.GetDirection());
        float det=(b*b)-DOT3(v,v)+m_SqRadius;
        int retval=MISS;

        if(det>0)
        {
            det=sqrtf(det);
            float i1=b-det;
            float i2=b+det;
            if (i2 > 0)
            {
                if (i1 < 0)
                {
                    if (i2 < a_Dist)
                    {
                        a_Dist = i2;
                        retval = INPRIM;
                    }
                }
                else
                {
                    if (i1 < a_Dist)
                    {
                        a_Dist = i1;
                        retval = HIT;
                    }
                }
            }
        }
        return retval;
    }

    int jdxyw::RayTracer::PlanePrim::Intersect(Ray &a_Ray, float &a_Dist)
    {
        float d = DOT3( m_plane.N, a_Ray.GetDirection() );
        if (d != 0)
        {
            float dist = -(DOT3( m_plane.N, a_Ray.GetOrigin() ) + m_plane.D) / d;
            if (dist > 0)
            {
                if (dist < a_Dist)
                {
                    a_Dist = dist;
                    return HIT;
                }
            }
        }
        return MISS;
    }

    vector3 jdxyw::RayTracer::PlanePrim::GetNormal(vector3 &a_pos)
    {
        return m_plane.N;
    }

    jdxyw::RayTracer::Scene::~Scene()
    {
        delete m_Primitive;
    }

    void jdxyw::RayTracer::Scene::InitScene()
    {
        m_Primitive = new Primitive*[100];
        // ground plane
        m_Primitive[0] = new PlanePrim( vector3( 0.0, 1, 0 ), 4.4f );
        m_Primitive[0]->SetName( "plane" );
        m_Primitive[0]->GetMaterial()->SetReflection( 0 );
        m_Primitive[0]->GetMaterial()->SetDiffuse( 1.0f );
        m_Primitive[0]->GetMaterial()->SetColor( Color( 0.4f, 0.3f, 0.3f ) );
        // big sphere
        m_Primitive[1] = new Sphere( vector3( 1, -0.8f, 3 ), 2.5f );
        m_Primitive[1]->SetName( "big sphere" );
        m_Primitive[1]->GetMaterial()->SetReflection( 0.6f );
        m_Primitive[1]->GetMaterial()->SetColor( Color( 0.7f, 0.7f, 0.7f ) );
        // small sphere
        m_Primitive[2] = new Sphere( vector3( -5.5f, -0.5, 7 ), 2 );
        m_Primitive[2]->SetName( "small sphere" );
        m_Primitive[2]->GetMaterial()->SetReflection( 1.0f );
        m_Primitive[2]->GetMaterial()->SetDiffuse( 0.1f );
        m_Primitive[2]->GetMaterial()->SetColor( Color( 0.7f, 0.7f, 1.0f ) );
        // light source 1
        m_Primitive[3] = new Sphere( vector3( 0, 5, 5 ), 0.1f );
        m_Primitive[3]->Light( true );
        m_Primitive[3]->GetMaterial()->SetColor( Color( 0.6f, 0.2f, 0.9f ) );
        // light source 2
        m_Primitive[4] = new Sphere( vector3( 2, 5, 1 ), 0.1f );
        m_Primitive[4]->Light( true );
        m_Primitive[4]->GetMaterial()->SetColor( Color( 0.9f, 0.3f, 0.1f ) );
        // set number of primitives
        m_Primitives = 5;
    }
}
}