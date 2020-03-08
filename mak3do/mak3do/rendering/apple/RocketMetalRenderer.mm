#include "RocketMetalRenderer.h"
#include <Rocket/Core.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#include "ShaderTypes.h"
#import "SceneRenderer.h"
#include <iostream>

namespace mak3do {
namespace rocket {

struct Impl {
    id<MTLRenderPipelineState> pipeline_texture;
    id<MTLRenderPipelineState> pipeline_color;
    id<MTLRenderCommandEncoder> encoder;
    id<MTLSamplerState> sampler;
    
    
    id<MTLDevice> device;
    
    std::vector<id<MTLTexture> > texture_cache;
    
    bool scissoring { false };
    
    id<MTLTexture> load_image(void* data, unsigned long size);
};

struct RocketGeometry {
    int index_count { 0 };
    id<MTLBuffer> indices;
    id<MTLBuffer> vertices;
    id<MTLTexture> texture;
};

id<MTLTexture> Impl::load_image(void *__data,unsigned long size)
{
    NSData* data = [[NSData alloc] initWithBytes:__data length:size];
    MTKTextureLoader *loader = [[MTKTextureLoader alloc] initWithDevice: device];
    
    //NSURL* url = [[NSBundle mainBundle] URLForResource:@"earth_diffuse" withExtension:@"png"];
    //id<MTLTexture> texture = [loader newTextureWithContentsOfURL:url options:nil error:nil];

    id<MTLTexture> texture = [loader newTextureWithData:data options:nil error:nil];
    
    return texture;
}

class RocketEvent : public Rocket::Core::EventListener {
public:
    RocketEvent(const Rocket::Core::String& value, RocketMetalRenderer* renderer)
        : m_renderer(renderer)
        , m_value(value)
    {
    }

    void ProcessEvent(Rocket::Core::Event& event)
    {
        m_renderer->processEvent(event, m_value);
    }

    void OnDetach(Rocket::Core::Element* element)
    {
        delete this;
    }

private:
    Rocket::Core::String m_value;
    RocketMetalRenderer* m_renderer;
};

RocketMetalRenderer* RocketMetalRenderer::get(const Vec2& size, void* __device)
{
    static RocketMetalRenderer* rmr = nullptr;
    
    if (rmr == nullptr) {
        rmr = new RocketMetalRenderer(size,__device);
    }
    
    return rmr;
}

RocketMetalRenderer::RocketMetalRenderer(const Vec2& size, void* __device)
: m_impl(std::make_shared<Impl>())
{
    m_impl->device = (__bridge id<MTLDevice>)__device;
    
    m_size = size;
    
    Rocket::Core::SetRenderInterface(this);
    Rocket::Core::Factory::RegisterEventListenerInstancer(this);
    Rocket::Core::SetFileInterface(this);
    Rocket::Core::SetSystemInterface(new PrivateSystemInterface());
    
    bool init = Rocket::Core::Initialise();
    
    assert(init);
    
    m_main_context =
    Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(m_size.w, m_size.h));
    
    MTLSamplerDescriptor *samplerDesc = [MTLSamplerDescriptor new];
    samplerDesc.minFilter = MTLSamplerMinMagFilterLinear;
    samplerDesc.magFilter = MTLSamplerMinMagFilterLinear;
    samplerDesc.mipFilter = MTLSamplerMipFilterNotMipmapped;
    samplerDesc.sAddressMode = MTLSamplerAddressModeRepeat;
    samplerDesc.tAddressMode = MTLSamplerAddressModeRepeat;
    
    m_impl->sampler = [m_impl->device newSamplerStateWithDescriptor:samplerDesc];
    
    NSBundle* bundle = [NSBundle bundleForClass:[SceneRenderer class]];
    NSError* error = nil;
    
    id<MTLLibrary> library = [m_impl->device newDefaultLibraryWithBundle:bundle error:&error];
    
    id<MTLFunction> vertex = [library newFunctionWithName:@"rocket_vertex"];
    id<MTLFunction> texture = [library newFunctionWithName:@"rocket_texture"];
    id<MTLFunction> color = [library newFunctionWithName:@"rocket_color"];
    
    MTLRenderPipelineDescriptor *desc = [[MTLRenderPipelineDescriptor alloc] init];
    
    desc.label = @"rocket_color";
    desc.vertexFunction = vertex;
    desc.fragmentFunction = color;
    desc.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
    desc.colorAttachments[0].blendingEnabled = YES;
    desc.depthAttachmentPixelFormat = MTLPixelFormatInvalid;
    desc.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
    desc.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
    desc.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    desc.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    
    m_impl->pipeline_color = [m_impl->device newRenderPipelineStateWithDescriptor:desc error:&error];
    
    desc = nil;
    desc = [[MTLRenderPipelineDescriptor alloc] init];
    
    desc.label = @"rocket_texture";
    desc.vertexFunction = vertex;
    desc.fragmentFunction = texture;
    desc.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
    desc.colorAttachments[0].blendingEnabled = YES;
    desc.depthAttachmentPixelFormat = MTLPixelFormatInvalid;
    desc.stencilAttachmentPixelFormat = MTLPixelFormatInvalid;
    desc.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
    desc.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
    desc.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
    desc.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
    desc.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    desc.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    
    m_impl->pipeline_texture = [m_impl->device newRenderPipelineStateWithDescriptor:desc error:&error];
    
    Mat4::createOrthographic(m_size.width, m_size.height, -1024, 1024, &m_projection);
    
    setup_events();
}

RocketMetalRenderer::~RocketMetalRenderer()
{
    auto pm = io::PointerManager::get();
    
    pm->stop(m_press_callback);
    pm->stop(m_move_callback);
    pm->stop(m_release_callback);
    pm->stop(m_scrolled_callback);
}

void RocketMetalRenderer::setup_events()
{
    m_press_callback = std::make_shared<io::PointerEventCallback>();
    m_move_callback = std::make_shared<io::PointerEventCallback>();
    m_release_callback = std::make_shared<io::PointerEventCallback>();
    m_scrolled_callback = std::make_shared<io::ScrollEventCallback>();
    
    m_press_callback->lambda = [this](const std::vector<io::PointerEventPtr>& touches) -> bool {
        
        if (touches.size() == 0) {
            return false;
        }
        
        this->m_main_context->ProcessMouseMove(touches[0]->location.x,
                                               m_size.y - touches[0]->location.y,
                                               0);
        
        this->m_main_context->ProcessMouseButtonDown(0, 0);

        return true;
    };
    
    m_move_callback->lambda = [this](const std::vector<io::PointerEventPtr>& touches) -> bool {
      
        if (touches.size() == 0) {
                   return false;
        }
        
        this->m_main_context->ProcessMouseMove(touches[0]->location.x,
                                               m_size.y - touches[0]->location.y,
                                               0);
        
        return true;
    };
    
    m_release_callback->lambda = [this](const std::vector<io::PointerEventPtr>& touches) -> bool {
        
        if (touches.size() == 0) {
            return false;
        }
        
        this->m_main_context->ProcessMouseMove(touches[0]->location.x,
                                               m_size.y - touches[0]->location.y,
                                               0);
        
        this->m_main_context->ProcessMouseButtonUp(0, 0);
      
        return true;
    };
    
    m_scrolled_callback->lambda = [this](float dx, float dy) {
        std::cout << "dx " << dx << " dy " << dy << std::endl;
        this->m_main_context->ProcessMouseWheel(-dy, 0);
    };
    
    auto pm = io::PointerManager::get();
    
    pm->touched(m_press_callback);
    pm->moved(m_move_callback);
    pm->released(m_release_callback);
    pm->scrolled(m_scrolled_callback);
}

Rocket::Core::Context* RocketMetalRenderer::context() const
{
    return m_main_context;
}

void RocketMetalRenderer::render(const Vec2& viewport, void* __cb, void* __pd, void* extra)
{
    m_size = viewport;
    m_main_context->SetDimensions(Rocket::Core::Vector2i(m_size.w,m_size.h));
    Mat4::createOrthographic(m_size.width, m_size.height, -1024, 1024, &m_projection);
    id<MTLCommandBuffer> commandBuffer = (__bridge id<MTLCommandBuffer>)__cb;
    id<MTLTexture> __texture = (__bridge id<MTLTexture>)__pd;
    
    MTLRenderPassColorAttachmentDescriptor *colorAttachment = [MTLRenderPassColorAttachmentDescriptor new];
    colorAttachment.texture = __texture;
    colorAttachment.loadAction = MTLLoadActionDontCare;
    colorAttachment.storeAction = MTLStoreActionStore;
    
    MTLRenderPassDescriptor *renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    renderPassDescriptor.colorAttachments[0] = colorAttachment;
    
    if (extra != nullptr) {
        renderPassDescriptor = (__bridge MTLRenderPassDescriptor*)extra;
    }
    
    id<MTLRenderCommandEncoder> renderEncoder =
    [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    renderEncoder.label = @"Rocket";
    [renderEncoder pushDebugGroup:@"drawRocket"];
    
    m_impl->encoder = nil;
    m_impl->encoder = renderEncoder;
    
    [renderEncoder setViewport:(MTLViewport){0.0, 0.0, viewport.x, viewport.y, -1024.0, 1024.0}];
    
    m_main_context->Render();
    m_main_context->Update();
    
    [renderEncoder endEncoding];
}

Rocket::Core::CompiledGeometryHandle
RocketMetalRenderer::CompileGeometry(Rocket::Core::Vertex* vertices,
                                     int num_vertices,
                                     int* indices,
                                     int num_indices,
                                     Rocket::Core::TextureHandle texture)
{
    auto id_buffer = [m_impl->device newBufferWithBytes:indices
                                                         length:num_indices*sizeof(int)
                                                        options:MTLResourceStorageModeShared];
       
   std::vector<RocketVertex> __vertices;

   for (auto i = 0; i < num_vertices; i++) {
       RocketVertex a;
       a.position = simd_make_float4(vertices[i].position.x, vertices[i].position.y,0,1);
       a.color = simd_make_float4(vertices[i].colour.red/255.f, vertices[i].colour.green/255.f, vertices[i].colour.blue/255.f, vertices[i].colour.alpha/255.f);
       a.texcoord = simd_make_float2(vertices[i].tex_coord.x,vertices[i].tex_coord.y);
       
       __vertices.push_back(a);
   }

   auto vertex_buffer = [m_impl->device newBufferWithBytes:&__vertices[0]
                                                    length:__vertices.size()*sizeof(RocketVertex)
                                                   options:MTLResourceStorageModeShared];
    
    auto rocket_geometry = new RocketGeometry();
    
    rocket_geometry->index_count = num_indices;
    rocket_geometry->indices = id_buffer;
    rocket_geometry->vertices = vertex_buffer;
    rocket_geometry->texture = (__bridge id<MTLTexture>)reinterpret_cast<void*>(texture);
    
    return reinterpret_cast<Rocket::Core::CompiledGeometryHandle>(rocket_geometry);
}

void RocketMetalRenderer::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry,
                                                 const Rocket::Core::Vector2f& translation)
{
    Mat4 t, o;
    t.scale(1,-1,1);
    t.translate(translation.x, translation.y, 0);
    t.translate(-m_size.x*.5f, -m_size.y*.5f, 0);
    o = m_projection * t;

    RocketGeometry* geom = reinterpret_cast<RocketGeometry*>(geometry);

    [m_impl->encoder setFrontFacingWinding:MTLWindingClockwise];
    [m_impl->encoder setCullMode:MTLCullModeNone];
      
    if (geom->texture == nil) {
      [m_impl->encoder setRenderPipelineState:m_impl->pipeline_color];
    } else {
      [m_impl->encoder setRenderPipelineState:m_impl->pipeline_texture];
      [m_impl->encoder setFragmentTexture:geom->texture atIndex:0];
      [m_impl->encoder setFragmentSamplerState:m_impl->sampler atIndex:0];
    }

    [m_impl->encoder setVertexBuffer:geom->vertices offset:0 atIndex:0];

    [m_impl->encoder setVertexBytes:o.m
                           length:sizeof(float)*16
                          atIndex:1];

    [m_impl->encoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                              indexCount:geom->index_count
                               indexType:MTLIndexTypeUInt32
                             indexBuffer:geom->indices
                      indexBufferOffset:0];

    [m_impl->encoder popDebugGroup];
}

void RocketMetalRenderer::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry)
{
    RocketGeometry* geom = reinterpret_cast<RocketGeometry*>(geometry);
    
    geom->indices = nil;
    geom->texture = nil;
    geom->vertices = nil;
    
    delete geom;
}

void RocketMetalRenderer::RenderGeometry(Rocket::Core::Vertex* vertices,
                                         int num_vertices,
                                         int* indices,
                                         int num_indices,
                                         Rocket::Core::TextureHandle texture,
                                         const Rocket::Core::Vector2f& translation)
{

}

void RocketMetalRenderer::EnableScissorRegion(bool enable)
{
    m_impl->scissoring = enable;
}

void RocketMetalRenderer::SetScissorRegion(int x, int y, int width, int height)
{
    MTLScissorRect rect {
        (NSUInteger)x,
        (NSUInteger)m_size.height - ((NSUInteger)y + (NSUInteger)height),
        (NSUInteger)width,
        (NSUInteger)height};
    
    if (x >= 0 && rect.x + rect.width <= m_size.width && rect.y + rect.height <= m_size.height) {
        [m_impl->encoder setScissorRect:rect];
    }
}

bool RocketMetalRenderer::LoadTexture(Rocket::Core::TextureHandle& texture_handle,
                                      Rocket::Core::Vector2i& texture_dimensions,
                                      const Rocket::Core::String& source)
{
    Rocket::Core::FileInterface* file_interface = Rocket::Core::GetFileInterface();
    Rocket::Core::FileHandle file_handle = file_interface->Open(source);
    if (!file_handle)
        return false;

    file_interface->Seek(file_handle, 0, SEEK_END);
    size_t buffer_size = file_interface->Tell(file_handle);
    file_interface->Seek(file_handle, 0, SEEK_SET);

    char* buffer = new char[buffer_size];
    file_interface->Read(buffer, buffer_size, file_handle);
    file_interface->Close(file_handle);

    size_t i;
    for (i = source.Length() - 1; i > 0; i--) {
        if (source[i] == '.')
            break;
    }
    
    id<MTLTexture> _mtlTexture = m_impl->load_image(buffer, buffer_size);
    
    if (_mtlTexture == nil) {
        return false;
    }
    
    m_impl->texture_cache.push_back(_mtlTexture);
    
    texture_dimensions.x = (int)[_mtlTexture width];
    texture_dimensions.y = (int)[_mtlTexture height];
    
    texture_handle = reinterpret_cast<Rocket::Core::TextureHandle>((void*)CFBridgingRetain(_mtlTexture));
    
    return true;
}

bool RocketMetalRenderer::GenerateTexture(Rocket::Core::TextureHandle& texture_handle,
                                          const Rocket::Core::byte* source,
                                          const Rocket::Core::Vector2i& source_dimensions)
{
    MTLTextureDescriptor* textureDescriptor =
        [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                           width:source_dimensions.x
                                                          height:source_dimensions.y
                                                       mipmapped:NO];

    textureDescriptor.usage = MTLTextureUsageShaderRead;
    textureDescriptor.textureType = MTLTextureType2D;
    
    id<MTLTexture> _mtlTexture = [m_impl->device newTextureWithDescriptor:textureDescriptor];
    
    MTLRegion region = {
        { 0, 0, 0 },
        { (NSUInteger)source_dimensions.x, (NSUInteger)source_dimensions.y, 1 }
    };


    [_mtlTexture setLabel:@"rocket_generated_texture"];
    [_mtlTexture replaceRegion:region
                   mipmapLevel:0
                     withBytes:source
                   bytesPerRow:4*source_dimensions.x];
    
    m_impl->texture_cache.push_back(_mtlTexture);
    
    texture_handle = reinterpret_cast<Rocket::Core::TextureHandle>((void*)CFBridgingRetain(_mtlTexture));
    
    return true;
}

void RocketMetalRenderer::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
    m_impl->texture_cache.erase(std::remove_if(m_impl->texture_cache.begin(),
                                                    m_impl->texture_cache.end(),
                                                    [=](id<MTLTexture> texture) {
        auto __texture = (__bridge id<MTLTexture>)reinterpret_cast<void*>(texture_handle);
        
        if (__texture == texture) {
            CFBridgingRelease(reinterpret_cast<void*>(texture_handle));
            return true;
        }
        
        return false;
        
    }),m_impl->texture_cache.end());
}

//File

Rocket::Core::FileHandle RocketMetalRenderer::Open(const Rocket::Core::String& path)
{
    std::string fullpath = path.CString();
    
    auto __path =
    [[[NSBundle mainBundle] URLForResource:[NSString stringWithUTF8String:path.CString()]
                             withExtension:@""] path];

    FILE* fp = fopen([__path UTF8String], "rb");
    if (fp != nullptr)
        return (Rocket::Core::FileHandle)fp;

    fp = fopen(path.CString(), "rb");
    return (Rocket::Core::FileHandle)fp;
}

void RocketMetalRenderer::Close(Rocket::Core::FileHandle file)
{
    fclose((FILE*)file);
}

size_t RocketMetalRenderer::Read(void* buffer, size_t size, Rocket::Core::FileHandle file)
{
    return fread(buffer, 1, size, (FILE*)file);
}

bool RocketMetalRenderer::Seek(Rocket::Core::FileHandle file, long offset, int origin)
{
    return fseek((FILE*)file, offset, origin) == 0;
}

size_t RocketMetalRenderer::Tell(Rocket::Core::FileHandle file)
{
    return ftell((FILE*)file);
}

Rocket::Core::EventListener*
RocketMetalRenderer::InstanceEventListener(const Rocket::Core::String& value,
                                           Rocket::Core::Element* element)
{
    return new RocketEvent(value, this);
}

void RocketMetalRenderer::processEvent(Rocket::Core::Event& event,
                                       Rocket::Core::String& value)
{
    std::cout << "processEvent" << std::endl;
}

void RocketMetalRenderer::Release()
{
    
}

float PrivateSystemInterface::GetElapsedTime()
{
    //m_at += Director::get()->delta();
    return m_at;
}

bool PrivateSystemInterface::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message)
{
    std::cout << "ROCKET " + std::to_string(type) + " " + message.CString() << std::endl;

    return true;
}

}
}
