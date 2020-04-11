#pragma once

#include "../../gl/vertex_array.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>

#include "../gui_constants.h"

/**
 * Components are the rendererable primitives of a ui overlay
 *
 */

namespace gui {

    /**
     * @brief Descibes offset and scale of either a size or position
     * For example, a position scale of (0.2, 0.4) would be 20% from the bottom and 40%
     * from the left of the screen
     */
    struct GuiDimension final {
        glm::vec2 scale{0.0f};
        glm::vec2 offset{0.0f};

        GuiDimension() = default;

        GuiDimension(float xScale, float xOffset, float yScale, float yOffset)
            : scale{xScale, yScale}
            , offset{xOffset, yOffset}
        {
        }

        glm::vec2 apply() const
        {
            glm::vec2 vector;
            vector.x = scale.x * GUI_WIDTH;
            vector.y = scale.y * GUI_HEIGHT;
            return vector + offset;
        }
    };

    class Component {
      public:
        virtual ~Component() = default;

        void hide()
        {
            m_isHidden = true;
        }

        void show()
        {
            m_isHidden = false;
        }

        bool isHidden() const
        {
            return m_isHidden;
        }

        virtual void setPosition(const GuiDimension& position) = 0;
        virtual void setSize(const GuiDimension& position) = 0;

        glm::vec3 colour{1.0f};

      private:
        bool m_isHidden = false;
    };

} // namespace gui
