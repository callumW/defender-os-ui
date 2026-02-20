#include "defender/ui_element.h"
#include "defender/logger.h"
#include <algorithm>

namespace defender {

UIElement::UIElement()
    : isActive_(true)
    , x_(0)
    , y_(0)
    , width_(0)
    , height_(0)
    , name_("UIElement")
    , parent_(nullptr) {
}

void UIElement::update(const InputState& input, float deltaTime) {
    if (!isActive_) {
        return;
    }

    // Update all active children
    for (auto& child : children_) {
        if (child && child->isActive()) {
            child->update(input, deltaTime);
        }
    }
}

void UIElement::render(Renderer& renderer) {
    if (!isActive_) {
        return;
    }

    // Render all active children
    for (auto& child : children_) {
        if (child && child->isActive()) {
            child->render(renderer);
        }
    }
}

void UIElement::setActive(bool active) {
    isActive_ = active;

    // Propagate active state to all children
    for (auto& child : children_) {
        if (child) {
            child->setActive(active);
        }
    }

    Logger::getInstance().debug(
        name_ + " set to " + (active ? "active" : "inactive")
    );
}

void UIElement::addChild(std::unique_ptr<UIElement> child) {
    if (!child) {
        Logger::getInstance().warning("Attempted to add null child to " + name_);
        return;
    }

    child->parent_ = this;
    
    // Inherit active state from parent
    child->setActive(isActive_);
    
    children_.push_back(std::move(child));
    
    Logger::getInstance().debug(
        "Added child to " + name_ + ", now has " + 
        std::to_string(children_.size()) + " children"
    );
}

bool UIElement::removeChild(UIElement* child) {
    if (!child) {
        return false;
    }

    auto it = std::find_if(children_.begin(), children_.end(),
        [child](const std::unique_ptr<UIElement>& ptr) {
            return ptr.get() == child;
        });

    if (it != children_.end()) {
        (*it)->parent_ = nullptr;
        children_.erase(it);
        Logger::getInstance().debug("Removed child from " + name_);
        return true;
    }

    return false;
}

UIElement* UIElement::getChild(size_t index) {
    if (index < children_.size()) {
        return children_[index].get();
    }
    return nullptr;
}

void UIElement::setPosition(int x, int y) {
    x_ = x;
    y_ = y;
}

void UIElement::getPosition(int& x, int& y) const {
    x = x_;
    y = y_;
}

void UIElement::setSize(int width, int height) {
    width_ = width;
    height_ = height;
}

void UIElement::getSize(int& width, int& height) const {
    width = width_;
    height = height_;
}

void UIElement::getAbsolutePosition(int& x, int& y) const {
    x = x_;
    y = y_;

    // Add parent's absolute position
    if (parent_) {
        int parentX, parentY;
        parent_->getAbsolutePosition(parentX, parentY);
        x += parentX;
        y += parentY;
    }
}

} // namespace defender
