
#include "widgets.hpp"
#include "design.hpp"

#include "MineSweeperMap.hpp"
#include "GameSingleton.hpp"

#include <iostream>
#include <string>
#include <algorithm>

/*
class MineSweeperButton : public Button {
public:
  MineSweeperButton(size_t x, size_t y, size_t width, size_t height, size_t i, size_t j) 
   : Button(x, y, width, height), i_(i), j_(j) {
    SetText("");

  }
 
  void ClickListener(sf::Event& evt) override {
    auto& game = GameSingleton::GetGame();
    if (evt.mouseButton.button == sf::Mouse::Left && !flagged_) {
      game.LeftClickCell(i_, j_);
      if (!game.IsOpened(i_, j_)) {
        game.SetOpened(i_, j_);
        auto text = game.GetContent(i_, j_);
        SetText(text == "0" ? "" : text);
      }
      return;
    }
    if (evt.mouseButton.button == sf::Mouse::Right) {
      flagged_ = !flagged_;
    }
    if (flagged_) {
       SetText("F");
    }
  }

  void Paint(sf::RenderWindow& window) override {
    auto& game = GameSingleton::GetGame();
    auto& opened = game.OpenedLink();
    
    if (opened[i_][j_]) {
      if (game.GetStatus() == lose) {
        SetBackgroundColor({255, 166, 207});
      } else {
        SetBackgroundColor({255,255,255});
      }
      
      auto text = game.GetContent(i_, j_);
      SetText(text == "0" ? "" : text);
    }
    Button::Paint(window);
  }

private:
  bool* visible_;
  bool flagged_{false};
  size_t i_;
  size_t j_;
};

class MineSweeperApp : public FrameApplication {
public:
  MineSweeperApp() 
    : FrameApplication(1000, 1000, "MineSweeper") {
    for (size_t i = 0; i < 20; ++i) {
      for (size_t j = 0; j < 20; ++j) {
        Button* btn = new MineSweeperButton(i * 50, j * 50, 50, 50, i , j);
      //  grid_.AddChild(i, j, btn);
      }
    }
  }
private:

};*/

class MinesweeperButton : public Button, public ClickListener, public GameEventFollower {
public:
  MinesweeperButton(size_t i, size_t j) : Button(50, 50), i_(i), j_(j) {
    SetText("");
    SetClickListener(this);
    GameSingleton::GetGame().Follow(i, j, this);
    SetBackgroundColor(sf::Color(r, g, b));
  }

  void LeftClick() {
    GameSingleton::GetGame().LeftClickCell(i_, j_);
  }

  void RightClick() {
    GameSingleton::GetGame().RightClickCell(i_, j_);
  }

  void EventPerformed(Event event) {
    if (event.type == EventTypes::OpenCell) {
      SetText(event.text);
      SetTextColor(GetNumberColor(event.text[0]));
      SetBackgroundColor(sf::Color::White);
    }
  }
private:
  size_t i_;
  size_t j_;

  size_t r{100};
  size_t g{100};
  size_t b{100};
};

class MinesCounter : public Button {
  public:
    MinesCounter() : Button(400, 50) {
      SetText("|> 10 / * 40");
    }
};


class InGameMenu : public BoxContainer {
  public:
    InGameMenu() : BoxContainer(Y_AXIS) {
      AddChild(new Button("Game menu!", 400, 50));
      AddChild(new MinesCounter());
    }
};


class StartGameButton : public Button, public ClickListener {
public:
	StartGameButton(size_t n, size_t m, size_t width, size_t height, TabContainer* container) : Button("Start", width, height), container_(container) {
		SetClickListener(this);
	}

	virtual void LeftClick() {
		container_->SwitchTo(1);
		std::cout<<":::"<<std::endl;

	}
	virtual void RightClick() {
		container_->SwitchTo(1);
	}
private:
	TabContainer* container_;
};

class MenuContainer: public BoxContainer {
public:
	MenuContainer(TabContainer* container) : BoxContainer(Y_AXIS) {
		Button* menu_button = new Button("Menu", 200,40);
		StartGameButton* game1_button = new StartGameButton(1, 1, 200, 40, container);
		AddChild(menu_button);
		AddChild(game1_button);


	}
};

class FieldContainer: public BoxContainer {
public:
	FieldContainer() : BoxContainer(X_AXIS) {
		GridContainer* mines_grid = new GridContainer(20, 20);
		for (size_t i = 0; i < 20; ++i) {
			for (size_t j = 0; j < 20; ++j) {
				mines_grid->AddChild(i, new MinesweeperButton(i, j));
			}
		}
		AddChild(mines_grid);
		AddChild(new InGameMenu());
	}
};

class LoseContainer: public BoxContainer {
public:
	LoseContainer() : BoxContainer(Y_AXIS) {
		Button* menu_button = new Button("You LOSE!", 200,40);
		AddChild(menu_button);
	}
};

class WinContainer: public BoxContainer {
public:
	WinContainer() : BoxContainer(Y_AXIS) {
		Button* menu_button = new Button("You WIN!", 200,40);
		AddChild(menu_button);

	}
};



class Activities : public TabContainer {
public:
	Activities() : TabContainer({}) {
		AddChild(new MenuContainer(this));
		AddChild(new FieldContainer());
		AddChild(new LoseContainer());
		AddChild(new WinContainer());
	}
};



class TestApp : public FrameApplication {
public:
	TestApp()
			: FrameApplication(1400, 1000, "MineSweeper") {
		BoxContainer* container = new BoxContainer(X_AXIS);
		SetContentPane(new Activities());

	}


private:

};

int main() {
  TestApp app;
  app.Show();

  /*
  MineSweeperApp app;
  app.Show();
  */
}

//TODO: Make "win" and "lose" tabs
