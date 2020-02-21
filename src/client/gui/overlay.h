#pragma once

namespace gui {

struct OverlayDefinition;

struct Overlay {
    Overlay(const OverlayDefinition& overlayDefinition)
        : p_definition(&overlayDefinition)
    {
    }
    const OverlayDefinition* p_definition;
};

} // namespace gui
