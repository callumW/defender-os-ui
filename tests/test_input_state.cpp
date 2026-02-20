#include "defender/input_state.h"
#include <cassert>
#include <iostream>

using namespace defender;

void testInputStateCreation() {
    InputState input;
    
    // Test default mouse state
    assert(input.getMouse().x == 0);
    assert(input.getMouse().y == 0);
    assert(input.getMouse().leftButton == false);
    assert(input.getMouse().rightButton == false);
    assert(input.getMouse().leftClicked == false);
    
    // Test default keyboard state
    assert(input.getKeyboard().escapePressed == false);
    assert(input.getKeyboard().enterPressed == false);
    
    std::cout << "✓ Input state creation test passed" << std::endl;
}

void testInputStateModification() {
    InputState input;
    
    // Modify mouse state
    input.getMouse().x = 100;
    input.getMouse().y = 200;
    input.getMouse().leftButton = true;
    input.getMouse().leftClicked = true;
    
    assert(input.getMouse().x == 100);
    assert(input.getMouse().y == 200);
    assert(input.getMouse().leftButton == true);
    assert(input.getMouse().leftClicked == true);
    
    // Modify keyboard state
    input.getKeyboard().escapePressed = true;
    input.getKeyboard().enterPressed = true;
    
    assert(input.getKeyboard().escapePressed == true);
    assert(input.getKeyboard().enterPressed == true);
    
    std::cout << "✓ Input state modification test passed" << std::endl;
}

void testInputStateReset() {
    InputState input;
    
    // Set some per-frame states
    input.getMouse().leftClicked = true;
    input.getKeyboard().escapePressed = true;
    input.getKeyboard().enterPressed = true;
    
    // Mouse position and button held should remain
    input.getMouse().x = 100;
    input.getMouse().leftButton = true;
    
    // Reset per-frame states
    input.reset();
    
    // Per-frame states should be reset
    assert(input.getMouse().leftClicked == false);
    assert(input.getKeyboard().escapePressed == false);
    assert(input.getKeyboard().enterPressed == false);
    
    // Persistent states should remain
    assert(input.getMouse().x == 100);
    assert(input.getMouse().leftButton == true);
    
    std::cout << "✓ Input state reset test passed" << std::endl;
}

int main() {
    std::cout << "Running Input State tests..." << std::endl;
    
    try {
        testInputStateCreation();
        testInputStateModification();
        testInputStateReset();
        
        std::cout << "All Input State tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
