#include "overlay_factory.h"

namespace gui {

void OverlayFactory::addOverlay(const OverlayDefinition& overlay)
{
    m_overlayMap.emplace(overlay.id, m_overlays.size());
    m_overlays.push_back(overlay);
}

std::unique_ptr<Overlay> OverlayFactory::createOverlay(const std::string& name)
{
    auto itr = m_overlayMap.find(name);
    if (itr != m_overlayMap.end()) {
        auto& overlayDefintion = m_overlays.at(itr->second);
        auto overlay = std::make_unique<Overlay>(overlayDefintion);
        overlayDefintion.create(overlay.get());
        return overlay;
    }
    throw std::runtime_error("Unable to find gui with name" + name);
}
} // namespace gui