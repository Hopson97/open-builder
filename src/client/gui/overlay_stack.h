#pragma once

#include "overlay.h"
#include <memory>
#include <vector>

namespace gui {

struct OverlayStack {
    void pushLayer(std::unique_ptr<Overlay>);
    void popLayer();
    void removeLayerByName(const std::string& overlayId);

    std::vector<std::unique_ptr<Overlay>> m_overlayStack;
};
} // namespace gui