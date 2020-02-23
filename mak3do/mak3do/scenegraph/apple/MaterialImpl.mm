#include "MaterialImpl.h"
#include "TextureImpl.h"
#include <mak3do/scenegraph/Material.h>
#include <mak3do/scenegraph/Texture.h>
#import <SceneKit/SceneKit.h>

#if TARGET_OS_OSX
#define UIColor NSColor
#endif

namespace mak3do {

MaterialImpl::MaterialImpl(Material* parent)
: m_abstract(parent)
{
    SCNMaterial* material = [SCNMaterial material];
    m_native_material = (void*)CFBridgingRetain(material);
}

MaterialImpl::~MaterialImpl()
{
    SCNMaterial* material = (SCNMaterial*)CFBridgingRelease(m_native_material);
    m_native_material = nullptr;
    material = nil;
}

void MaterialImpl::diffuse(MaterialPropertyPtr property)
{
    if (property->texture != nullptr) {
        auto texture_pimpl = std::dynamic_pointer_cast<TextureImpl>(property->texture->pimpl());
        SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
        __material.diffuse.contents = [NSString stringWithUTF8String:texture_pimpl->m_native_texture.c_str()];
    } else {
        SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
        __material.diffuse.contents = [UIColor colorWithRed:property->color.r
                                                      green:property->color.g
                                                       blue:property->color.b
                                                      alpha:1.0];
    }
}

inline void fill_material(SCNMaterial* __material, MaterialPropertyPtr material, id contents)
{
    if ([__material.diffuse.contents isKindOfClass: [UIColor class]]) {
        UIColor* color = contents;
        CGFloat r,g,b;
        [color getRed:&r green:&g blue:&b alpha:nil];
        material->color = color::RGB(r,g,b);
    } else if ([__material.diffuse.contents isKindOfClass: [NSString class]]) {
        NSString* texture_filename = contents;
        material->texture = std::make_shared<Texture>([texture_filename UTF8String]);
    }
}

MaterialPropertyPtr MaterialImpl::diffuse()
{
    auto material = std::make_shared<MaterialProperty>();
    SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
    
    fill_material(__material, material, __material.diffuse.contents);
    
    return material;
}

void MaterialImpl::specular(MaterialPropertyPtr property)
{
    if (property->texture != nullptr) {
        auto texture_pimpl = std::dynamic_pointer_cast<TextureImpl>(property->texture->pimpl());
        SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
        __material.specular.contents = [NSString stringWithUTF8String:texture_pimpl->m_native_texture.c_str()];
    } else {
        SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
        __material.specular.contents = [UIColor colorWithRed:property->color.r
                                                      green:property->color.g
                                                       blue:property->color.b
                                                      alpha:1.0];
    }
}

MaterialPropertyPtr MaterialImpl::specular()
{
    auto material = std::make_shared<MaterialProperty>();
    SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
    
    fill_material(__material, material, __material.specular.contents);
    
    return material;
}

void MaterialImpl::ambient(MaterialPropertyPtr property)
{
    SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
    __material.ambient.contents = [UIColor colorWithRed:property->color.r
                                                  green:property->color.g
                                                   blue:property->color.b
                                                  alpha:1.0];
}

MaterialPropertyPtr MaterialImpl::ambient()
{
    auto material = std::make_shared<MaterialProperty>();
    SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
    
    fill_material(__material, material, __material.ambient.contents);
    
    return material;
}

void MaterialImpl::emission(MaterialPropertyPtr property)
{
    if (property->texture != nullptr) {
        auto texture_pimpl = std::dynamic_pointer_cast<TextureImpl>(property->texture->pimpl());
        SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
        __material.emission.contents = [NSString stringWithUTF8String:texture_pimpl->m_native_texture.c_str()];
    } else {
        SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
        __material.emission.contents = [UIColor colorWithRed:property->color.r
                                                      green:property->color.g
                                                       blue:property->color.b
                                                      alpha:1.0];
    }
}

MaterialPropertyPtr MaterialImpl::emission()
{
    auto material = std::make_shared<MaterialProperty>();
    SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
    
    fill_material(__material, material, __material.emission.contents);
    
    return material;
}

void MaterialImpl::transparent(MaterialPropertyPtr property)
{
    if (property->texture != nullptr) {
        auto texture_pimpl = std::dynamic_pointer_cast<TextureImpl>(property->texture->pimpl());
        SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
        __material.transparent.contents = [NSString stringWithUTF8String:texture_pimpl->m_native_texture.c_str()];
    } else {
        SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
        __material.transparent.contents = [UIColor colorWithRed:property->color.r
                                                      green:property->color.g
                                                       blue:property->color.b
                                                      alpha:1.0];
    }
}

MaterialPropertyPtr MaterialImpl::transparent()
{
    auto material = std::make_shared<MaterialProperty>();
    SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
    
    fill_material(__material, material, __material.transparent.contents);
    
    return material;
}

void MaterialImpl::displacement(MaterialPropertyPtr property)
{
    if (property->texture != nullptr) {
        auto texture_pimpl = std::dynamic_pointer_cast<TextureImpl>(property->texture->pimpl());
        SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
        __material.displacement.contents = [NSString stringWithUTF8String:texture_pimpl->m_native_texture.c_str()];
    }
}

MaterialPropertyPtr MaterialImpl::displacement()
{
    auto material = std::make_shared<MaterialProperty>();
    SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
    
    fill_material(__material, material, __material.displacement.contents);
    
    return material;
}

void MaterialImpl::roughness(MaterialPropertyPtr property)
{
    if (property->texture != nullptr) {
        auto texture_pimpl = std::dynamic_pointer_cast<TextureImpl>(property->texture->pimpl());
        SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
        __material.roughness.contents = [NSString stringWithUTF8String:texture_pimpl->m_native_texture.c_str()];
    }
}

MaterialPropertyPtr MaterialImpl::roughness()
{
    auto material = std::make_shared<MaterialProperty>();
    SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
    
    fill_material(__material, material, __material.roughness.contents);
    
    return material;
}

void MaterialImpl::occlusion(MaterialPropertyPtr property)
{
    if (property->texture != nullptr) {
        auto texture_pimpl = std::dynamic_pointer_cast<TextureImpl>(property->texture->pimpl());
        SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
        __material.ambientOcclusion.contents = [NSString stringWithUTF8String:texture_pimpl->m_native_texture.c_str()];
    }
}

MaterialPropertyPtr MaterialImpl::occlusion()
{
    auto material = std::make_shared<MaterialProperty>();
    SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
    
    fill_material(__material, material, __material.ambientOcclusion.contents);
    
    return material;
}

void MaterialImpl::normal(MaterialPropertyPtr property)
{
    if (property->texture != nullptr) {
        auto texture_pimpl = std::dynamic_pointer_cast<TextureImpl>(property->texture->pimpl());
        SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
        __material.normal.contents = [NSString stringWithUTF8String:texture_pimpl->m_native_texture.c_str()];
    } else {
        SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
        __material.normal.contents = [UIColor colorWithRed:property->color.r
                                                      green:property->color.g
                                                       blue:property->color.b
                                                      alpha:1.0];
    }
}

MaterialPropertyPtr MaterialImpl::normal()
{
    auto material = std::make_shared<MaterialProperty>();
    SCNMaterial* __material = (__bridge SCNMaterial*)m_native_material;
    
    fill_material(__material, material, __material.normal.contents);
    
    return material;
}

}

