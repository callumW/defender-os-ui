#pragma once

#include <cstdint>

namespace defender {

/**
 * @brief Represents the state of user input
 * 
 * This class abstracts input events into a simple state that can be
 * queried by UI elements during their update phase.
 */
class InputState {
public:
    /**
     * @brief Mouse button states
     */
    struct MouseState {
        int32_t x = 0;           ///< Current X coordinate
        int32_t y = 0;           ///< Current Y coordinate
        bool leftButton = false;  ///< Left button is pressed
        bool rightButton = false; ///< Right button is pressed
        bool leftClicked = false; ///< Left button was clicked this frame
    };

    /**
     * @brief Keyboard state
     */
    struct KeyboardState {
        bool escapePressed = false; ///< Escape key pressed this frame
        bool enterPressed = false;  ///< Enter key pressed this frame
        bool upPressed = false;     ///< Up arrow pressed this frame
        bool downPressed = false;   ///< Down arrow pressed this frame
        bool leftPressed = false;   ///< Left arrow pressed this frame
        bool rightPressed = false;  ///< Right arrow pressed this frame
    };

    InputState() = default;
    ~InputState() = default;

    /**
     * @brief Get the current mouse state
     */
    const MouseState& getMouse() const { return mouse_; }

    /**
     * @brief Get the current keyboard state
     */
    const KeyboardState& getKeyboard() const { return keyboard_; }

    /**
     * @brief Get mutable mouse state for updating
     */
    MouseState& getMouse() { return mouse_; }

    /**
     * @brief Get mutable keyboard state for updating
     */
    KeyboardState& getKeyboard() { return keyboard_; }

    /**
     * @brief Reset per-frame input states (clicked, pressed, etc.)
     */
    void reset();

private:
    MouseState mouse_;
    KeyboardState keyboard_;
};

} // namespace defender
