#include "RocketMetalRenderer.h"
#include <Rocket/Core.h>
#include <mak3do/scenegraph/Director.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import "SceneRenderer.h"

namespace mak3do {
namespace rocket {

struct Impl {
    id<MTLRenderPipelineState> pipeline_texture;
    id<MTLRenderPipelineState> pipeline_color;
    id<MTLRenderCommandEncoder> encoder;
    id<MTLSamplerState> sampler;
    
    id<MTLDevice> device;
    
    std::vector<id<MTLTexture> > texture_cache;
    
    id<MTLTexture> load_image(void* data, unsigned long size);
};

id<MTLTexture> Impl::load_image(void *__data,unsigned long size)
{
    NSData* data = [[NSData alloc] initWithBytes:__data length:size];
    MTKTextureLoader *loader = [[MTKTextureLoader alloc] initWithDevice: device];

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

    /// Destroys the event.
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
    
    Rocket::Core::SetRenderInterface(this);
    Rocket::Core::Factory::RegisterEventListenerInstancer(this);
    Rocket::Core::SetFileInterface(this);
    Rocket::Core::SetSystemInterface(new PrivateSystemInterface());
    
    bool init = Rocket::Core::Initialise();
    
    m_main_context =
    Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(size.w, size.h));
    
    MTLSamplerDescriptor *samplerDesc = [MTLSamplerDescriptor new];
    samplerDesc.minFilter = MTLSamplerMinMagFilterLinear;
    samplerDesc.magFilter = MTLSamplerMinMagFilterLinear;
    samplerDesc.mipFilter = MTLSamplerMipFilterNotMipmapped;
    samplerDesc.sAddressMode = MTLSamplerAddressModeClampToEdge;
    samplerDesc.tAddressMode = MTLSamplerAddressModeClampToEdge;
    
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
    desc.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
    desc.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
    desc.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    desc.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    
    m_impl->pipeline_texture = [m_impl->device newRenderPipelineStateWithDescriptor:desc error:&error];
    
    Mat4::createOrthographic(size.width, size.height, -1024, 1024, &m_projection);
}

Rocket::Core::Context* RocketMetalRenderer::context() const
{
    return m_main_context;
}

void RocketMetalRenderer::render(const Vec2& viewport, void* __cb, void* __pd)
{
    m_size = viewport;
    id<MTLCommandBuffer> commandBuffer = (__bridge id<MTLCommandBuffer>)__cb;
    id<MTLTexture> __texture = (__bridge id<MTLTexture>)__pd;
    
    MTLRenderPassColorAttachmentDescriptor *colorAttachment = [MTLRenderPassColorAttachmentDescriptor new];
    colorAttachment.texture = __texture;
    colorAttachment.loadAction = MTLLoadActionDontCare;
    colorAttachment.storeAction = MTLStoreActionStore;
    
    MTLRenderPassDescriptor *renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    renderPassDescriptor.colorAttachments[0] = colorAttachment;
    
    id<MTLRenderCommandEncoder> renderEncoder =
    [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    renderEncoder.label = @"Rocket";
    
    m_impl->encoder = nil;
    m_impl->encoder = renderEncoder;
    
    [renderEncoder setViewport:(MTLViewport){0.0, 0.0, viewport.x, viewport.y, 0.0, 1.0 }];
    
    m_main_context->Render();
    m_main_context->Update();
    
    [renderEncoder endEncoding];
}

void RocketMetalRenderer::RenderGeometry(Rocket::Core::Vertex* vertices,
                                         int num_vertices,
                                         int* indices,
                                         int num_indices,
                                         Rocket::Core::TextureHandle texture,
                                         const Rocket::Core::Vector2f& translation)
{
    Quaternion q;
    q.yaw_pitch_roll(180, 0, 180);
    Mat4 t, o;
    t.rotate(q);
    t.translate(translation.x, translation.y, 0);
    t.translate(-m_size.x, -m_size.y, 0);
    o = m_projection * t;
    
    id<MTLTexture> __texture = (__bridge id<MTLTexture>)reinterpret_cast<void*>(texture);
        
    if (__texture == nil) {
        [m_impl->encoder setRenderPipelineState:m_impl->pipeline_color];
    } else {
        [m_impl->encoder setRenderPipelineState:m_impl->pipeline_texture];
        [m_impl->encoder setFragmentTexture:__texture atIndex:0];
        [m_impl->encoder setFragmentSamplerState:m_impl->sampler atIndex:0];
    }
    
    auto id_buffer = [m_impl->device newBufferWithBytes:indices
                                                      length:num_indices*sizeof(unsigned int)
                                                     options:MTLResourceStorageModeShared];
    
    [m_impl->encoder setVertexBytes:vertices
                             length:sizeof(num_vertices)*sizeof(Rocket::Core::Vertex)
                            atIndex:0];
    
    [m_impl->encoder setVertexBytes:o.m
                             length:sizeof(float)*16
                            atIndex:1];
    
    [m_impl->encoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                indexCount:num_indices
                                 indexType:MTLIndexTypeUInt32
                               indexBuffer:id_buffer
                        indexBufferOffset:0];
}

void RocketMetalRenderer::EnableScissorRegion(bool enable)
{
    
}

void RocketMetalRenderer::SetScissorRegion(int x, int y, int width, int height)
{
    MTLScissorRect rect { (NSUInteger)x, (NSUInteger)y, (NSUInteger)width, (NSUInteger)height };
    
    if (x < 0) {
        rect.x = 0;
    }
    
    if (y < 0) {
        rect.y = 0;
    }
    
    if (x + width <= m_size.width && y + height <= m_size.height) {
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

    textureDescriptor.usage = MTLTextureUsagePixelFormatView | MTLTextureUsageShaderRead;
    
    id<MTLTexture> _mtlTexture = [m_impl->device newTextureWithDescriptor:textureDescriptor];
    
    MTLRegion region = MTLRegionMake2D(0, 0, source_dimensions.x, source_dimensions.y);

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
    
}

void RocketMetalRenderer::Release()
{
    
}

float PrivateSystemInterface::GetElapsedTime()
{
    m_at += Director::get()->delta();
    return m_at;
}

bool PrivateSystemInterface::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message)
{
    std::cout << "ROCKET" + std::to_string(type) + " " + message.CString() << std::endl;

    return true;
}

}
}
