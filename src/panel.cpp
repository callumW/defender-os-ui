#include "defender/panel.h"
#include "defender/renderer.h"
#include "defender/theme.h"

namespace defender {

Panel::Panel()
    : UIElement()
    , drawBackground_(true)
    , drawBorder_(true) {
    
    setName("Panel");
    setSize(200, 200);
}

void Panel::render(Renderer& renderer) {
    if (!isActive_) {
        return;
    }

    int absX, absY;
    getAbsolutePosition(absX, absY);

    auto& theme = Theme::getInstance();

    // Draw background
    if (drawBackground_) {
        renderer.setDrawColor(theme.getColor("panel"));
        renderer.fillRect(absX, absY, width_, height_);
    }

    // Draw border
    if (drawBorder_) {
        renderer.setDrawColor(theme.getColor("border"));
        renderer.drawRect(absX, absY, width_, height_);
    }

    // Render children
    UIElement::render(renderer);
}

} // namespace defender
