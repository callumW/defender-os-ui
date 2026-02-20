#include "defender/ui_element.h"
#include "defender/renderer.h"
#include <cassert>
#include <iostream>
#include <memory>

using namespace defender;

// Mock renderer for testing
class MockRenderer : public Renderer {
public:
    void clear() override {}
    void present() override {}
    void setDrawColor(const Color&) override {}
    void fillRect(int, int, int, int) override {}
    void drawRect(int, int, int, int) override {}
    void drawText(const std::string&, int, int, const std::string&, const Color&) override {}
    void getTextSize(const std::string&, const std::string&, int& w, int& h) override {
        w = 100;
        h = 20;
    }
};

void testUIElementCreation() {
    auto element = std::make_unique<UIElement>();
    assert(element != nullptr);
    assert(element->isActive() == true);
    assert(element->getChildCount() == 0);
    
    std::cout << "✓ UI element creation test passed" << std::endl;
}

void testUIElementTree() {
    auto parent = std::make_unique<UIElement>();
    auto child1 = std::make_unique<UIElement>();
    auto child2 = std::make_unique<UIElement>();
    
    UIElement* child1Ptr = child1.get();
    UIElement* child2Ptr = child2.get();
    
    parent->addChild(std::move(child1));
    parent->addChild(std::move(child2));
    
    assert(parent->getChildCount() == 2);
    assert(parent->getChild(0) == child1Ptr);
    assert(parent->getChild(1) == child2Ptr);
    assert(child1Ptr->getParent() == parent.get());
    assert(child2Ptr->getParent() == parent.get());
    
    std::cout << "✓ UI element tree test passed" << std::endl;
}

void testActiveStatePropagation() {
    auto parent = std::make_unique<UIElement>();
    auto child = std::make_unique<UIElement>();
    
    UIElement* childPtr = child.get();
    parent->addChild(std::move(child));
    
    // Initially both should be active
    assert(parent->isActive() == true);
    assert(childPtr->isActive() == true);
    
    // Setting parent to inactive should deactivate child
    parent->setActive(false);
    assert(parent->isActive() == false);
    assert(childPtr->isActive() == false);
    
    // Setting parent to active should activate child
    parent->setActive(true);
    assert(parent->isActive() == true);
    assert(childPtr->isActive() == true);
    
    std::cout << "✓ Active state propagation test passed" << std::endl;
}

void testRelativePositioning() {
    auto parent = std::make_unique<UIElement>();
    auto child = std::make_unique<UIElement>();
    
    parent->setPosition(100, 100);
    child->setPosition(20, 30);
    
    UIElement* childPtr = child.get();
    parent->addChild(std::move(child));
    
    int absX, absY;
    childPtr->getAbsolutePosition(absX, absY);
    
    // Child's absolute position should be parent + child position
    assert(absX == 120);
    assert(absY == 130);
    
    std::cout << "✓ Relative positioning test passed" << std::endl;
}

void testRemoveChild() {
    auto parent = std::make_unique<UIElement>();
    auto child = std::make_unique<UIElement>();
    
    UIElement* childPtr = child.get();
    parent->addChild(std::move(child));
    
    assert(parent->getChildCount() == 1);
    
    bool removed = parent->removeChild(childPtr);
    assert(removed == true);
    assert(parent->getChildCount() == 0);
    
    std::cout << "✓ Remove child test passed" << std::endl;
}

int main() {
    std::cout << "Running UI Element tests..." << std::endl;
    
    try {
        testUIElementCreation();
        testUIElementTree();
        testActiveStatePropagation();
        testRelativePositioning();
        testRemoveChild();
        
        std::cout << "All UI Element tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
