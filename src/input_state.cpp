#include "defender/input_state.h"

namespace defender {

void InputState::reset() {
    // Reset per-frame states
    mouse_.leftClicked = false;
    keyboard_.escapePressed = false;
    keyboard_.enterPressed = false;
    keyboard_.upPressed = false;
    keyboard_.downPressed = false;
    keyboard_.leftPressed = false;
    keyboard_.rightPressed = false;
}

} // namespace defender
