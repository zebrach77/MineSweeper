#ifndef MINESWEEPER_UI_HPP
#define MINESWEEPER_UI_HPP
#include "widgets.hpp"
#include "GameSingleton.hpp"
class MinesweeperButton : public Button, public ClickListener, public GameEventFollower {
public:
	MinesweeperButton(size_t i, size_t j);

	void LeftClick() override;

	void RightClick() override;

	void EventPerformed(Event event) override;

private:
	size_t i_;
	size_t j_;

	size_t r{100};
	size_t g{100};
	size_t b{100};
};

class MinesCounter : public Button {
public:
	MinesCounter();
};


class WLButton : public Button, public GameEventFollower {
public:
	WLButton();

	void EventPerformed(Event event) override;
};


class InGameMenu : public BoxContainer {
public:
	InGameMenu();
};


class StartGameButton : public Button, public ClickListener {
public:
	StartGameButton(size_t n, size_t m, size_t width, size_t height, TabContainer *container);

	void LeftClick() override;

	void RightClick() override;

private:
	TabContainer *container_;
};

class MenuContainer : public BoxContainer {
public:
	MenuContainer(TabContainer *container);
};

class FieldContainer : public BoxContainer {
public:
	FieldContainer();
};

class LoseContainer : public BoxContainer {
public:
	LoseContainer();
};

class WinContainer : public BoxContainer {
public:
	WinContainer();
};


class Activities : public TabContainer, public GameEventFollower {
public:
	Activities();
	void EventPerformed(Event event) override;
};


class TestApp : public FrameApplication {
public:
	TestApp();


private:

};
#endif