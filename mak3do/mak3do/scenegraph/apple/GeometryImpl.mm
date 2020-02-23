#include "GeometryImpl.h"
#include "MaterialImpl.h"
#include <mak3do/scenegraph/Geometry.h>
#include <mak3do/scenegraph/Material.h>
#include <mak3do/scenegraph/Color.h>
#import <SceneKit/SceneKit.h>

#if TARGET_OS_OSX
#define UIColor NSColor
#endif

namespace mak3do {

GeometryImpl::GeometryImpl(Geometry* parent)
: m_abstract(parent)
{
    SCNGeometry* geometry = [SCNGeometry geometry];
    
    //FIXME: Delete me! Just for testing
    SCNBox* box = [SCNBox boxWithWidth:1 height:1 length:1 chamferRadius:0];
    box.materials.firstObject.diffuse.contents = [UIColor redColor];
    
    geometry = box;
    
    m_native_geometry = (void*)CFBridgingRetain(geometry);
}

void GeometryImpl::replace_geometry(void* geometry)
{
    SCNGeometry* __geometry = (SCNGeometry*)CFBridgingRelease(m_native_geometry);//-1 ARC
    __geometry = nil;
    SCNGeometry* __new_geometry = (__bridge SCNGeometry*)geometry;
    m_native_geometry = (void*)CFBridgingRetain(__new_geometry); //+1 ARC
}

void GeometryImpl::color(const color::RGBA& color)
{
    SCNGeometry* __geometry = (__bridge SCNGeometry*)m_native_geometry;
    UIColor* __color = [UIColor colorWithRed:color.r green:color.g blue:color.b alpha:color.a];
    __geometry.materials.firstObject.diffuse.contents = __color;
}

void GeometryImpl::material(MaterialPtr material)
{
    SCNGeometry* __geometry = (__bridge SCNGeometry*)m_native_geometry;
    
    auto pimpl = std::dynamic_pointer_cast<MaterialImpl>(material->pimpl());
    SCNMaterial* __material = (__bridge SCNMaterial*)pimpl->m_native_material;
    __geometry.firstMaterial = __material;
}

void GeometryImpl::modify_shader_geometry(const std::string& modifier_code)
{
    SCNGeometry* __geometry = (__bridge SCNGeometry*)m_native_geometry;
    
    __geometry.shaderModifiers =
        @{SCNShaderModifierEntryPointGeometry :
            [NSString stringWithUTF8String:modifier_code.c_str()]};
}

GeometryImpl::~GeometryImpl()
{
    SCNGeometry* geometry = (SCNGeometry*)CFBridgingRelease(m_native_geometry);
    geometry = nil;
    m_native_geometry = nullptr;
}

}


