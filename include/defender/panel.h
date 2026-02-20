#pragma once

#include "defender/ui_element.h"

namespace defender {

/**
 * @brief Panel UI element
 * 
 * A simple container panel for grouping other UI elements.
 */
class Panel : public UIElement {
public:
    Panel();
    ~Panel() override = default;

    /**
     * @brief Render the panel
     */
    void render(Renderer& renderer) override;

    /**
     * @brief Set whether to draw background
     */
    void setDrawBackground(bool draw) { drawBackground_ = draw; }

    /**
     * @brief Set whether to draw border
     */
    void setDrawBorder(bool draw) { drawBorder_ = draw; }

private:
    bool drawBackground_;
    bool drawBorder_;
};

} // namespace defender
