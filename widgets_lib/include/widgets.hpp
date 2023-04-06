#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>

static constexpr bool X_AXIS = true;
static constexpr bool Y_AXIS = false;

class ClickListener {
public:
  virtual void LeftClick() = 0;
  virtual void RightClick() = 0;
};

class Widget {
public:
  Widget();
  virtual ~Widget() = default;

  void SetBackgroundColor(sf::Color color);

  virtual size_t Width() = 0;
  virtual size_t Height() = 0;

  size_t GetWindowX() {
    Widget* now = this;
    size_t x = 0;
    while (now->GetParent() != nullptr) {
      x += now->GetParent()->GetElementX(now);
      now = now->GetParent();
    }
    return x;
  }

  size_t GetWindowY() {
    Widget* now = this;
    size_t y = 0;
    while (now->GetParent() != nullptr) {
      y += now->GetParent()->GetElementY(now);
      now = now->GetParent();
    }
    return y;
  }
  
  void SetClickListener(ClickListener* listener) {
    listener_ = listener;
  }

  ClickListener* GetClickListener() {
    return listener_;
  }

  virtual void Paint(sf::RenderWindow& window);

  virtual Widget* GetParent() {
    return parent_;
  }

  virtual Widget* SetParent(Widget* parent) {
    parent_ = parent;
  }

  virtual size_t GetElementX(Widget* ch) {
    return 0;
  }

  virtual size_t GetElementY(Widget* ch) {
    return 0;
  }

  virtual Widget* ChildAt(size_t x, size_t y) = 0;


protected:
  ClickListener* listener_;
  Widget* parent_;
  sf::Color color_;
};


class Container : public Widget {
public:
  Container() = default;

  virtual void AddChild(Widget* child) = 0;
};

class BoxContainer : public Container {
  public:
    BoxContainer(bool axis) : axis_(axis) {
    }

    virtual size_t Width() {
      size_t width = 0;
      for (auto* widget : children_) {
        if (widget != nullptr) {
          if (axis_ == X_AXIS) {
            width += widget->Width();
          } else {
            width = std::max(width, widget->Width());
          }
        }
      }
      return width;
    }

    virtual size_t GetElementX(Widget* ch) {
      size_t x = 0;
      for (auto* widget : children_) {
        if (widget == ch) {
          return x;
        }
        if (widget != nullptr) {
          if (axis_ == X_AXIS) {
            x += widget->Width();
          } else {
            return 0;
          }
        }
      }
    }
  
    virtual size_t GetElementY(Widget* ch) {
      size_t y = 0;
      for (auto* widget : children_) {
        if (widget == ch) {
          return y;
        }
        if (widget != nullptr) {
          if (axis_ == Y_AXIS) {
            y += widget->Height();
          } else {
            return 0;
          }
        }
      }
      return y;
    }

    virtual size_t Height() {
      size_t height = 0;
      for (auto* widget : children_) {
        if (widget != nullptr) {
          if (axis_ == Y_AXIS) {
            height += widget->Height();
          } else {
            height = std::max(height, widget->Height());
          }
        }
      }
      return height;
    }


    virtual Widget* ChildAt(size_t x, size_t y) {
      size_t now_x = 0;
      size_t now_y = 0;
      for (auto* widget : children_) {
        if (widget != nullptr) {
          size_t w_w = widget->Width();
          size_t h_w = widget->Height();
          if (now_x <= x && x < now_x + w_w && now_y <= y && y < now_y + h_w ) {
            return widget->ChildAt(x - now_x, y - now_y);
          }
          if (axis_ == X_AXIS) {
            now_x += widget->Width();
          } else {
            now_y += widget->Height();
          }
        }
      }
      return nullptr; // to do
    }
  
    virtual void AddChild(Widget* child) {
      children_.push_back(child);
      child->SetParent(this);
    }

    virtual void Paint(sf::RenderWindow& window) {
      Container::Paint(window);
      for (auto* widget : children_) {
        widget->Paint(window);
      }
    }
  private:
    bool axis_;
    std::vector<Widget*> children_;
};


class GridContainer : public Widget {
public:
  GridContainer(
    size_t lines, 
    size_t columns) {
      base_container_ = new BoxContainer(Y_AXIS);
      for (size_t i = 0; i < lines; i++) {
        BoxContainer* container = new BoxContainer(X_AXIS);
        lines_.push_back(container);
        base_container_->AddChild(container);
      }
    }

  void Paint(sf::RenderWindow& window) override {
    base_container_->Paint(window);
  }

  size_t Width() override {
    return base_container_->Width();
  }

  size_t Height() override {
    return base_container_->Height();
  }

  void AddChild(size_t i, Widget* child) {
    lines_[i]->AddChild(child);
  }

  Widget* ChildAt(size_t x, size_t y) override {
    return base_container_->ChildAt(x, y);
  }

private:
  std::vector<BoxContainer*> lines_;
  BoxContainer* base_container_;
}; 


class Button : public Widget {
public:
  Button(size_t width, size_t height);
  Button(const std::string& text, size_t width, size_t height);

  void SetText(const std::string& text);

  void Paint(sf::RenderWindow& window) override;

  virtual size_t Width() {
    return width_;
  }

  virtual size_t Height() {
    return height_;
  }

  virtual Widget* ChildAt(size_t x, size_t y) {
    // better to check x, y
    return this;
  }

  void SetTextColor(sf::Color color) {
    text_color_ = color;
  }
private:
  size_t width_;
  size_t height_;
  sf::Color text_color_;
  std::string font_path_{"../widgets_lib/fonts/LCD14.otf"};
  std::string text_;
};

class TabContainer : public Container {
public:
  TabContainer(std::vector<Widget*> tabs) : tabs_(tabs) {
    active_ = 0;
  }

  void Paint(sf::RenderWindow& window) override {
	if (!tabs_.empty()) {
		tabs_[active_]->Paint(window);
	}
	else {
		return;
	}
  }

  virtual size_t Width() {
    return tabs_[active_]->Width();
  }

  virtual size_t Height() {
    return tabs_[active_]->Height();
  }

  virtual Widget* ChildAt(size_t x, size_t y) {
    return tabs_[active_]->ChildAt(x, y);
  }

  virtual void AddChild(Widget* child) {
    tabs_.push_back(child);
  }

  void SwitchTo(size_t index) {
	  active_ = index;
  }

private:
  size_t active_;
  std::vector<Widget*> tabs_;
};

class FrameApplication {
public:
  FrameApplication(size_t width, size_t height, std::string title);

  void SetContentPane(Container* pane);

  Container* GetContentPane();

  void Show();

protected:
  Container* pane_;
  sf::RenderWindow window_;
};