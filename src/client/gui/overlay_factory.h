#pragma once

#include "overlay.h"
#include <memory>
#include <sol/sol.hpp>
#include <unordered_map>
#include <vector>

namespace gui {

struct OverlayDefintion {
    std::string id;
    std::string title;

    sol::function create;
};

class OverlayFactory {
  public:
    void addOverlay(const OverlayDefintion& overlay);
    std::unique_ptr<Overlay> createOverlay(const std::string& name);

  private:
    std::vector<OverlayDefintion> m_overlays;
    std::unordered_map<std::string, int> m_overlayMap;
};
} // namespace gui
