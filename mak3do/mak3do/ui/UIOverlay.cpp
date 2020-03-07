#include "UIOverlay.h"
#include <mak3do/rendering/apple/RocketMetalRenderer.h>
#include <Rocket/Core.h>

namespace mak3do {

class UIOverlayImpl
{
public:
    UIOverlayImpl();
    
    void fonts(const std::vector<std::string>& font_resources);
    bool load(const std::string& resource, const std::string& package = "");
private:
    rocket::RocketMetalRenderer* m_renderer;
};

UIOverlay* UIOverlay::get()
{
    static UIOverlay* overlay = nullptr;
    
    if (overlay == nullptr) {
        overlay = new UIOverlay();
    }
    
    return overlay;
}

UIOverlay::UIOverlay()
: m_pimpl(std::make_shared<UIOverlayImpl>())
{
}

void UIOverlay::fonts(const std::vector<std::string>& font_resources)
{
    m_pimpl->fonts(font_resources);
}

bool UIOverlay::load(const std::string& resource, const std::string& package)
{
    return m_pimpl->load(resource,package);
}

UIOverlayImpl::UIOverlayImpl()
: m_renderer(rocket::RocketMetalRenderer::get())
{
}

void UIOverlayImpl::fonts(const std::vector<std::string>& font_resources)
{
    for (auto& font: font_resources) {
        Rocket::Core::FontDatabase::LoadFontFace(font.c_str());
    }
}

bool UIOverlayImpl::load(const std::string& resource, const std::string& package)
{
    m_renderer->context()->UnloadAllDocuments();
    
    auto doc = m_renderer->context()->LoadDocument(resource.c_str());
    
    if (doc != nullptr) {
        doc->Show();
        return true;
    }
    
    return false;
}


}
