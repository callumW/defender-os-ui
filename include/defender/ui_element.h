#pragma once

#include "defender/input_state.h"
#include <vector>
#include <memory>
#include <string>

namespace defender {

// Forward declaration
class Renderer;

/**
 * @brief Base class for all UI elements
 * 
 * Implements a tree structure where each element can have a parent and children.
 * Coordinates and scaling are relative to the parent element.
 * Active/inactive state propagates down the tree.
 */
class UIElement {
public:
    UIElement();
    virtual ~UIElement() = default;

    /**
     * @brief Update the element based on input and time
     * @param input Current input state
     * @param deltaTime Time since last update in seconds
     */
    virtual void update(const InputState& input, float deltaTime);

    /**
     * @brief Render the element
     * @param renderer The renderer to use
     */
    virtual void render(Renderer& renderer);

    /**
     * @brief Set the active state of this element and all children
     * @param active True to activate, false to deactivate
     */
    void setActive(bool active);

    /**
     * @brief Check if this element is active
     */
    bool isActive() const { return isActive_; }

    /**
     * @brief Add a child element
     * @param child The child to add
     */
    void addChild(std::unique_ptr<UIElement> child);

    /**
     * @brief Remove a child element
     * @param child Pointer to the child to remove
     * @return true if child was found and removed
     */
    bool removeChild(UIElement* child);

    /**
     * @brief Get the number of children
     */
    size_t getChildCount() const { return children_.size(); }

    /**
     * @brief Get a child by index
     */
    UIElement* getChild(size_t index);

    /**
     * @brief Get the parent element
     */
    UIElement* getParent() { return parent_; }

    /**
     * @brief Get the parent element (const)
     */
    const UIElement* getParent() const { return parent_; }

    /**
     * @brief Set position relative to parent
     * @param x X coordinate
     * @param y Y coordinate
     */
    void setPosition(int x, int y);

    /**
     * @brief Get position relative to parent
     */
    void getPosition(int& x, int& y) const;

    /**
     * @brief Set size
     * @param width Width in pixels
     * @param height Height in pixels
     */
    void setSize(int width, int height);

    /**
     * @brief Get size
     */
    void getSize(int& width, int& height) const;

    /**
     * @brief Get absolute screen position
     * @param x Output X coordinate
     * @param y Output Y coordinate
     */
    void getAbsolutePosition(int& x, int& y) const;

    /**
     * @brief Set element name for debugging
     */
    void setName(const std::string& name) { name_ = name; }

    /**
     * @brief Get element name
     */
    const std::string& getName() const { return name_; }

protected:
    bool isActive_;
    int x_;
    int y_;
    int width_;
    int height_;
    std::string name_;

    UIElement* parent_;
    std::vector<std::unique_ptr<UIElement>> children_;
};

} // namespace defender
