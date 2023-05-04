#ifndef WIDGETS_HPP
#define WIDGETS_HPP

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include <thread>
#include <mutex>

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

	sf::Color GetBackgroundColor() const {
		return color_;
	}

	virtual size_t Width() = 0;

	virtual size_t Height() = 0;

	size_t GetWindowX() {
		Widget *now = this;
		size_t x = 0;
		while (now->GetParent() != nullptr) {
			x += now->GetParent()->GetElementX(now);
			now = now->GetParent();
		}
		return x;
	}

	size_t GetWindowY() {
		Widget *now = this;
		size_t y = 0;
		while (now->GetParent() != nullptr) {
			y += now->GetParent()->GetElementY(now);
			now = now->GetParent();
		}
		return y;
	}

	void SetClickListener(ClickListener *listener) {
		listener_ = listener;
	}

	ClickListener *GetClickListener() {
		return listener_;
	}

	virtual void Paint(sf::RenderWindow &window);

	virtual Widget *GetParent() {
		return parent_;
	}

	virtual void SetParent(Widget *parent) {
		parent_ = parent;
	}

	virtual size_t GetElementX(Widget *ch) {
		return 0;
	}

	virtual size_t GetElementY(Widget *ch) {
		return 0;
	}

	virtual Widget *ChildAt(size_t x, size_t y) = 0;


protected:
	ClickListener *listener_;
	Widget *parent_;
	sf::Color color_;
};


class Container : public Widget {
public:
	Container() = default;

	virtual void AddChild(Widget *child) = 0;
};

class BoxContainer : public Container {
public:
	BoxContainer(bool axis) : axis_(axis) {
	}

	virtual size_t Width() {
		size_t width = 0;
		for (auto *widget: children_) {
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

	virtual size_t GetElementX(Widget *ch) {
		size_t x = 0;
		for (auto *widget: children_) {
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
		return 0;
	}

	virtual size_t GetElementY(Widget *ch) {
		size_t y = 0;
		for (auto *widget: children_) {
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
		for (auto *widget: children_) {
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


	virtual Widget *ChildAt(size_t x, size_t y) {
		size_t now_x = 0;
		size_t now_y = 0;
		for (auto *widget: children_) {
			if (widget != nullptr) {
				size_t w_w = widget->Width();
				size_t h_w = widget->Height();
				if (now_x <= x && x < now_x + w_w && now_y <= y && y < now_y + h_w) {
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

	virtual void AddChild(Widget *child) {
		children_.push_back(child);
		child->SetParent(this);
	}

	virtual void Paint(sf::RenderWindow &window) {
		Container::Paint(window);
		for (auto *widget: children_) {
			widget->Paint(window);
		}
	}

private:
	bool axis_;
	std::vector<Widget *> children_;
};


class GridContainer : public Widget {
public:
	GridContainer(
			size_t lines,
			size_t columns) {
		base_container_ = new BoxContainer(Y_AXIS);
		for (size_t i = 0; i < lines; i++) {
			BoxContainer *container = new BoxContainer(X_AXIS);
			lines_.push_back(container);
			base_container_->AddChild(container);
		}
	}

	void Paint(sf::RenderWindow &window) override {
		base_container_->Paint(window);
	}

	size_t Width() override {
		return base_container_->Width();
	}

	size_t Height() override {
		return base_container_->Height();
	}

	void AddChild(size_t i, Widget *child) {
		lines_[i]->AddChild(child);
	}

	Widget *ChildAt(size_t x, size_t y) override {
		return base_container_->ChildAt(x, y);
	}

private:
	std::vector<BoxContainer *> lines_;
	BoxContainer *base_container_;
};


class Button : public Widget {
public:
	Button(size_t width, size_t height);

	Button(const std::string &text, size_t width, size_t height);

	void SetText(const std::string &text);

	void Paint(sf::RenderWindow &window) override;

	size_t Width() override {
		return width_;
	}

	size_t Height() override {
		return height_;
	}

	Widget *ChildAt(size_t x, size_t y) override {
		// better to check x, y
		return this;
	}

	void SetTextColor(sf::Color color) {
		text_color_ = color;
	}
	const std::string &GetText();

private:
	size_t width_;
	size_t height_;
	sf::Color text_color_;
	std::string font_path_{"../widgets_lib/fonts/LCD14.otf"};
	std::string text_;
};

class TabContainer : public Container {
public:
	TabContainer(std::vector<Widget *> tabs) : tabs_(tabs) {
		active_ = 0;
	}

	void Paint(sf::RenderWindow &window) override {
		if (!tabs_.empty()) {
			tabs_[active_]->Paint(window);
		} else {
			return;
		}
	}

	size_t Width() override {
		return tabs_[active_]->Width();
	}

	size_t Height() override {
		return tabs_[active_]->Height();
	}

	Widget *ChildAt(size_t x, size_t y) override {
		return tabs_[active_]->ChildAt(x, y);
	}

	void AddChild(Widget *child) override {
		tabs_.push_back(child);
	}

	void SwitchTo(size_t index) {
		active_ = index;
	}

private:
	size_t active_;
	std::vector<Widget *> tabs_;
};

class FrameApplication {
public:
	FrameApplication(size_t width, size_t height, std::string title);

	void SetContentPane(Container *pane);

	Container *GetContentPane();

	void Show();

protected:
	Container *pane_;
	sf::RenderWindow window_;
};

class WidgetAnimator {
public:
	explicit WidgetAnimator(Widget *widget) : widget_(widget) {}
	
	void Animate(sf::Color from, sf::Color to) {
		Widget* w = widget_;
		std::thread th([w, from, to](){
			int r, g, b;
			r = from.r;
			g = from.g;
			b = from.b;
			while (r != to.r && g!= to.g && b!= to.b) {
				if (r != to.r) {
					r += (to.r-r) / std::abs(to.r-r);
				}
				if (g != to.g) {
					g += (to.g-g) / std::abs(to.g-g);
				}
				if (b != to.b) {
					b += (to.b-b) / std::abs(to.b-b);
				}
				w->SetBackgroundColor(sf::Color(r, g, b));
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		});
		th.detach();
	}
private:
	Widget *widget_;
};






#endif