#include "widgets.hpp"
#include <iostream>

Widget::Widget() : color_{120,120,120} {
}

void Widget::Paint(sf::RenderWindow& window) {
  sf::RectangleShape rectangle(sf::Vector2f(Width(), Height()));
  rectangle.setFillColor(color_);
  rectangle.setPosition(GetWindowX(), GetWindowY());
  window.draw(rectangle);
}

void Widget::SetBackgroundColor(sf::Color color) {
  color_ = color;
}


Button::Button(size_t width, size_t height)
 : width_(width), height_(height) {
  text_color_ = sf::Color::Black;
}

Button::Button(const std::string& text, size_t width, size_t height)
 : Button(width, height) {
  SetText(text);
}

void Button::SetText(const std::string& text) {
  text_ = text;
}

void Button::Paint(sf::RenderWindow& window) {
  Widget::Paint(window);
  sf::RectangleShape rectangle(sf::Vector2f(Width(), Height()));
  rectangle.setPosition(GetWindowX(), GetWindowY());
  rectangle.setFillColor(Widget::color_);
  rectangle.setOutlineColor(sf::Color::Black);
  rectangle.setOutlineThickness(2);
  window.draw(rectangle);

  sf::Text text;
  std::string text_str = text_;
  text.setString(text_str);
  text.setPosition(GetWindowX() + 14, GetWindowY() + 4);
  text.setFillColor(text_color_);
  text.setCharacterSize(32);
  sf::Font font;
  font.loadFromFile(font_path_);
  text.setFont(font);
  window.draw(text);
}

FrameApplication::FrameApplication(size_t width, size_t height, std::string title)
  : window_(sf::VideoMode(width, height), title), pane_(nullptr) {
}

void FrameApplication::SetContentPane(Container* pane) {
  if (pane_ != nullptr) {
    delete pane_;
  }
  pane_ = pane;
}

Container* FrameApplication::GetContentPane() {
  return pane_;
}

void FrameApplication::Show() {
  while (window_.isOpen()) {
    sf::Event event;
    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
          window_.close();
      }
      else if (pane_ != nullptr && event.type == sf::Event::MouseButtonPressed) {
        Widget* widget = pane_->ChildAt(event.mouseButton.x, event.mouseButton.y);
        if (widget != nullptr) {
          ClickListener* listener = widget->GetClickListener();
          if (listener == nullptr) {
            continue;
          }
          if (event.mouseButton.button == sf::Mouse::Left) {
            listener->LeftClick();
          } else if (event.mouseButton.button == sf::Mouse::Right) {
            listener->RightClick();
          }
        }
      }
    }
    window_.clear();
    sf::RectangleShape rectangle(sf::Vector2f(100, 100));
  //  grid_.Paint(window_);
    if (pane_ != nullptr) {
      pane_->Paint(window_);
    }
    window_.display();
  }
}
