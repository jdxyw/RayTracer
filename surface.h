#ifndef SURFACE_H
#define SURFACE_H

#include <string>
#include "common.h"

namespace jdxyw{
namespace RayTracer{

class Surface
{
  enum
  {
      OWNER=1
  };

private:
  Pixel* m_Buffer;
  int m_Width,m_Height;

public:
  Surface(int a_Width,int a_Height);
  ~Surface();

  Pixel* GetBuffer(){return m_Buffer;}
  int GetWidth(){return m_Width;}
  int GetHeight(){return m_Height;}

  void Clear(Pixel a_Color);

};

}
}

#endif // SURFACE_H
