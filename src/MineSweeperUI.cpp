#include "MineSweeperUI.hpp"
#include "design.hpp"


MinesweeperButton::MinesweeperButton(size_t i, size_t j) : Button(50, 50), i_(i), j_(j) {
	SetText("");
	SetClickListener(this);
	GameSingleton::GetGame().Follow(i, j, this);
	GameSingleton::GetGame().FollowGameStatus(this);
	SetBackgroundColor(sf::Color(r, g, b));
}

void MinesweeperButton::LeftClick() {
	GameSingleton::GetGame().LeftClickCell(i_, j_);
}

void MinesweeperButton::RightClick() {
	GameSingleton::GetGame().RightClickCell(i_, j_);
}

void MinesweeperButton::EventPerformed(Event event) {
	if (event.type == EventTypes::OpenCell) {
		SetText(event.text);
		SetTextColor(GetNumberColor(event.text[0]));
//		SetBackgroundColor(sf::Color::White);
		WidgetAnimator widget_animator(this);
		if (GetText() == "0") {
			SetText("");
		}
		widget_animator.Animate(GetBackgroundColor(), sf::Color::White);
	}
	else if (event.type == EventTypes::SetFlag) {
		SetText(event.text);
		SetTextColor(sf::Color::Red);
	}
	else if (event.type == EventTypes::UnsetFlag) {
		SetText(event.text);
	}
	else if (event.type == EventTypes::LoseEvent) {
		SetBackgroundColor(sf::Color(245, 200, 236));
		if (GetText() == "0") {
			SetText("");
		}
		if (GameSingleton::GetGame().GetContent(i_, j_) == "*") {
			SetText("*");
			SetTextColor(sf::Color::White);
			SetBackgroundColor(sf::Color(255, 0, 0));
		}
	}
	else if (event.type == EventTypes::WinEvent) {
		SetBackgroundColor(sf::Color(176, 245, 215));
		if (GetText() == "0") {
			SetText("");
		}
		if (GameSingleton::GetGame().GetContent(i_, j_) == "*") {
			SetText("*");
		}
	}

}



MinesCounter::MinesCounter() : Button(400, 50) {
	SetText("|> 10 / * 40");
}




WLButton::WLButton() : Button(400, 50) {
	GameSingleton::GetGame().FollowGameStatus(this);
}

void WLButton::EventPerformed(Event event) {
	if (event.type == EventTypes::LoseEvent) {
		SetText("You Lose");

	}
	else if (event.type == EventTypes::WinEvent) {
		SetText("You WIN");

	}
}




InGameMenu::InGameMenu() : BoxContainer(Y_AXIS) {
	AddChild(new Button("Game menu!", 400, 50));
	AddChild(new MinesCounter());
	AddChild(new WLButton());

}



StartGameButton::StartGameButton(size_t n, size_t m, size_t width, size_t height, TabContainer *container)
	: Button("Start", width, height), container_(container) {

	SetClickListener(this);

}

void StartGameButton::LeftClick() {
	container_->SwitchTo(1);

}

void StartGameButton::RightClick() {
	container_->SwitchTo(1);
}



MenuContainer::MenuContainer(TabContainer *container) : BoxContainer(Y_AXIS) {
	auto *menu_button = new Button("Menu", 200, 40);
	auto *game1_button = new StartGameButton(1, 1, 200, 40, container);
	AddChild(menu_button);
	AddChild(game1_button);


}

FieldContainer::FieldContainer() : BoxContainer(X_AXIS) {
	auto *mines_grid = new GridContainer(20, 20);
	for (size_t i = 0; i < 20; ++i) {
		for (size_t j = 0; j < 20; ++j) {
			mines_grid->AddChild(i, new MinesweeperButton(i, j));
		}
	}
	AddChild(mines_grid);
	AddChild(new InGameMenu());
}



LoseContainer::LoseContainer() : BoxContainer(Y_AXIS) {
	auto *menu_button = new Button("You LOSE!", 200, 40);
	AddChild(menu_button);
}



WinContainer::WinContainer() : BoxContainer(Y_AXIS) {
	auto *menu_button = new Button("You WIN!", 200, 40);
	AddChild(menu_button);

}




Activities::Activities() : TabContainer({}) {
	AddChild(new MenuContainer(this));
	AddChild(new FieldContainer());
	AddChild(new LoseContainer());
	AddChild(new WinContainer());
	GameSingleton::GetGame().FollowGameStatus(this);
}
void Activities::EventPerformed(Event event) {
	if (event.type == LoseEvent) {
	}
	else if (event.type == WinEvent) {
	}
}



TestApp::TestApp()
		: FrameApplication(1400, 1000, "MineSweeper") {
	auto *container = new BoxContainer(X_AXIS);
	SetContentPane(new Activities());

}
