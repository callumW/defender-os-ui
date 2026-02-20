#pragma once

#include "defender/ui_element.h"
#include <string>
#include <functional>

namespace defender {

/**
 * @brief Button UI element
 * 
 * A clickable button with text label and hover states.
 */
class Button : public UIElement {
public:
    using ClickCallback = std::function<void()>;

    /**
     * @brief Construct a button
     * @param text Button text
     */
    explicit Button(const std::string& text = "Button");
    ~Button() override = default;

    /**
     * @brief Set button text
     */
    void setText(const std::string& text) { text_ = text; }

    /**
     * @brief Get button text
     */
    const std::string& getText() const { return text_; }

    /**
     * @brief Set click callback
     */
    void setOnClick(ClickCallback callback) { onClick_ = callback; }

    /**
     * @brief Update button state
     */
    void update(const InputState& input, float deltaTime) override;

    /**
     * @brief Render the button
     */
    void render(Renderer& renderer) override;

    /**
     * @brief Check if point is inside button
     */
    bool containsPoint(int x, int y) const;

private:
    std::string text_;
    bool isHovered_;
    bool isPressed_;
    ClickCallback onClick_;
};

} // namespace defender
