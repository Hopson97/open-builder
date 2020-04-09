#pragma once

#include "overlay.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace gui {
    /**
     * @brief Stores overlay defintions, and creates overlays using them
     */
    class OverlayFactory {
      public:
        /**
         * @brief Add a new overlay definition to the game
         * @param overlay The new overlay
         */
        void addOverlay(const OverlayDefinition& overlay);

        /**
         * @brief Create a Overlay object from a given overlay definition
         *
         * @param name The "overlay definition id", as defined in the Lua code
         * @return std::unique_ptr<Overlay> A newly created overlay, as a pointer such
         * that the Lua is able to store it without risk
         */
        std::unique_ptr<Overlay> createOverlay(const std::string& name,
                                               const sol::table& data);

      private:
        std::vector<OverlayDefinition> m_overlays;
        std::unordered_map<std::string, int> m_overlayMap;
    };
} // namespace gui
