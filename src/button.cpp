#include "defender/button.h"
#include "defender/renderer.h"
#include "defender/theme.h"
#include "defender/logger.h"

namespace defender {

Button::Button(const std::string& text)
    : UIElement()
    , text_(text)
    , isHovered_(false)
    , isPressed_(false)
    , onClick_(nullptr) {
    
    setName("Button");
    setSize(120, 40);
}

void Button::update(const InputState& input, float deltaTime) {
    if (!isActive_) {
        return;
    }

    // Get absolute position for hit testing
    int absX, absY;
    getAbsolutePosition(absX, absY);

    // Check if mouse is over button
    bool wasHovered = isHovered_;
    isHovered_ = containsPoint(input.getMouse().x, input.getMouse().y);

    // Handle click
    if (isHovered_ && input.getMouse().leftButton) {
        isPressed_ = true;
    } else if (isPressed_ && !input.getMouse().leftButton) {
        // Button was released
        if (isHovered_ && onClick_) {
            Logger::getInstance().debug("Button clicked: " + text_);
            onClick_();
        }
        isPressed_ = false;
    }

    // Update children
    UIElement::update(input, deltaTime);
}

void Button::render(Renderer& renderer) {
    if (!isActive_) {
        return;
    }

    int absX, absY;
    getAbsolutePosition(absX, absY);

    auto& theme = Theme::getInstance();

    // Choose color based on state
    Color buttonColor;
    if (isPressed_) {
        buttonColor = theme.getColor("buttonActive");
    } else if (isHovered_) {
        buttonColor = theme.getColor("buttonHover");
    } else {
        buttonColor = theme.getColor("button");
    }

    // Draw button background
    renderer.setDrawColor(buttonColor);
    renderer.fillRect(absX, absY, width_, height_);

    // Draw border
    renderer.setDrawColor(theme.getColor("border"));
    renderer.drawRect(absX, absY, width_, height_);

    // Draw text centered
    if (!text_.empty()) {
        int textWidth, textHeight;
        renderer.getTextSize(text_, "button", textWidth, textHeight);

        int textX = absX + (width_ - textWidth) / 2;
        int textY = absY + (height_ - textHeight) / 2;

        renderer.drawText(text_, textX, textY, "button", theme.getColor("text"));
    }

    // Render children
    UIElement::render(renderer);
}

bool Button::containsPoint(int x, int y) const {
    int absX, absY;
    getAbsolutePosition(absX, absY);

    return x >= absX && x < absX + width_ &&
           y >= absY && y < absY + height_;
}

} // namespace defender
