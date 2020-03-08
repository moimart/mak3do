#pragma once
#include <memory>
#include <mak3do/math/all>
#include <mak3do/io/all>
#include <Rocket/Core/EventListenerInstancer.h>
#include <Rocket/Core/FileInterface.h>
#include <Rocket/Core/RenderInterface.h>

#include <Rocket/Core/SystemInterface.h>

namespace mak3do {
namespace rocket {

class Impl;


class PrivateSystemInterface : public Rocket::Core::SystemInterface {
public:
    PrivateSystemInterface()
        : m_at(0)
    {
    }
    float GetElapsedTime();
    bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message);

private:
    float m_at;
};

class RocketMetalRenderer : public Rocket::Core::RenderInterface,
                            public Rocket::Core::FileInterface,
                            public Rocket::Core::EventListenerInstancer
{
public:
    //ROCKET
    static RocketMetalRenderer* get(const Vec2& size = Vec2::ZERO, void* __device = nullptr);
    
    RocketMetalRenderer(const Vec2& size, void* device);
    virtual ~RocketMetalRenderer();
    
    void render(const Vec2& viewport, void* commandbuffer, void* pass_descriptor, void* extra = nullptr);

    void RenderGeometry(Rocket::Core::Vertex* vertices,
                        int num_vertices,
                        int* indices,
                        int num_indices,
                        Rocket::Core::TextureHandle texture,
                        const Rocket::Core::Vector2f& translation);
    
    void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry,
                                const Rocket::Core::Vector2f& translation);
    
    Rocket::Core::CompiledGeometryHandle
    CompileGeometry(Rocket::Core::Vertex* vertices,
                    int num_vertices,
                    int* indices,
                    int num_indices,
                    Rocket::Core::TextureHandle texture);
    
    void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry);

    void EnableScissorRegion(bool enable);

    void SetScissorRegion(int x, int y, int width, int height);

    bool LoadTexture(Rocket::Core::TextureHandle& texture_handle,
                     Rocket::Core::Vector2i& texture_dimensions,
                     const Rocket::Core::String& source);

    bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle,
                         const Rocket::Core::byte* source,
                         const Rocket::Core::Vector2i& source_dimensions);

    void ReleaseTexture(Rocket::Core::TextureHandle texture_handle);

    //File

    Rocket::Core::FileHandle Open(const Rocket::Core::String& path);
    void Close(Rocket::Core::FileHandle file);
    size_t Read(void* buffer, size_t size, Rocket::Core::FileHandle file);
    bool Seek(Rocket::Core::FileHandle file, long offset, int origin);
    size_t Tell(Rocket::Core::FileHandle file);

    // EVENT LISTENER

    Rocket::Core::EventListener* InstanceEventListener(const Rocket::Core::String& value, Rocket::Core::Element* element);
    void processEvent(Rocket::Core::Event& event, Rocket::Core::String& value);
    void Release();

    
    Rocket::Core::Context* context() const;
protected:
    void setup_events();

private:

    Rocket::Core::Context* m_main_context;
    //std::map<std::string,void*> m_texture_cache

    std::shared_ptr<Impl> m_impl;
    std::shared_ptr<Rocket::Core::FileInterface> m_file_interface;
    
    Mat4 m_projection;
    Vec2 m_size;
    
    io::TouchCallbackPtr m_press_callback;
    io::TouchCallbackPtr m_move_callback;
    io::TouchCallbackPtr m_release_callback;
};
}
}

