#include "overlay_factory.h"

namespace gui {

void OverlayFactory::addOverlay(const OverlayDefintion& overlay)
{
    m_overlayMap.emplace(overlay.id, m_overlays.size());
    m_overlays.push_back(overlay);
};

std::unique_ptr<Overlay> OverlayFactory::createOverlay(const std::string& name)
{
    auto itr = m_overlayMap.find(name);
    if (itr != m_overlayMap.end()) {
        std::unique_ptr<Overlay> overlay;
        m_overlays.at(itr->second).create(overlay.get());
        return std::move(overlay);
    }
    throw std::runtime_error("Unable to find gui with name" + name);
};
} // namespace gui