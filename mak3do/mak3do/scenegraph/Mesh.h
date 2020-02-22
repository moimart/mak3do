#pragma once

#include "types.h"

namespace mak3do {

struct VertexAttribute
{
    enum class Format {
      INVALID = -1,
        FLOAT,
        CHAR,
        UCHAR,
        SHORT,
        USHORT
    };
    std::string name;
    Format format;
};
    
class Mesh : public Base
{
public:
    Mesh(const std::vector<std::vector<Vec3> >& vertices);
    
    std::vector<Vec3> vertices(int index) const;
private:
    MeshImplPtr m_pimpl;
};

}
