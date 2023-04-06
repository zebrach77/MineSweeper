#include "MineSweeperMap.hpp"

enum GameState {
  in_process = 0,
  win = 1,
  lose = 2
};


enum EventTypes {
  OpenCell,
  SetFlag,
  UnsetFlag
};

struct Event {
    Event(EventTypes type, std::string text) 
      : type(type), text(text) {

    }

    EventTypes type;
    std::string text;
};

class GameEventFollower {
public:
  virtual void EventPerformed(Event event) = 0;
};

/**
 * @brief ClassSingleton for Game 
 */
class GameSingleton {
public:
  GameSingleton(const GameSingleton&) = delete;
  GameSingleton(GameSingleton&&) = delete;
  GameSingleton& operator=(const GameSingleton&) = delete;
  GameSingleton& operator=(GameSingleton&&) = delete;

  void LeftClickCell(size_t i, size_t j);

  void RightClickCell(size_t i, size_t j);

  bool IsOpened(size_t i, size_t j);

  void SetOpened(size_t i, size_t j);

  std::string GetContent(size_t i, size_t j);

  void WaveBFS(size_t i, size_t j);

  static GameSingleton& GetGame();

  std::vector<std::vector<bool>>& OpenedLink();

  int GetStatus();

  void Follow(size_t i, size_t j, GameEventFollower* follower) {
    followers_[i][j] = follower;
  }
private:
  GameSingleton(size_t rows, size_t columns, size_t mines);

  static GameSingleton* game_;

  std::vector<std::vector<bool>> is_opened;
  std::vector<std::vector<GameEventFollower*>> followers_;
  int status_{in_process};
  MineSweeperMap map_;
};
