#include "GameSingleton.hpp"

void GameSingleton::LeftClickCell(size_t i, size_t j) {
	if (is_flag[i][j]) {
		return;
	}
	if (!map_.IsInitialized()) {
		map_.InitMap(i, j);
	}
	if (map_[i][j] == '0') {
		WaveBFS(i, j);
	} else if (map_[i][j] == '*') {
		for (size_t i = 0; i < map_.Rows(); ++i) {
			for (size_t j = 0; j < map_[i].size(); ++j) {
				SetOpened(i, j);
			}
		}

		status_ = lose;
		PushGameStatus(Event(EventTypes::LoseEvent, "LOSE"));
	} else {
		SetOpened(i, j);
	}
	if (CheckWin()) {
		PushGameStatus(Event(EventTypes::WinEvent, "WIN"));
	}
}

void GameSingleton::RightClickCell(size_t i, size_t j) {
	if (is_opened[i][j]) {
		return;
	}
	is_flag[i][j] = !(is_flag[i][j]);
	if (is_flag[i][j]) {
		followers_[i][j]->EventPerformed(Event(EventTypes::SetFlag, "P"));
	}
	else {
		followers_[i][j]->EventPerformed(Event(EventTypes::UnsetFlag, ""));
	}
}

bool GameSingleton::IsOpened(size_t i, size_t j) {
	return is_opened[i][j];
}

void GameSingleton::SetOpened(size_t i, size_t j) {
	std::string text = "";
	text += map_[i][j];
	followers_[i][j]->EventPerformed(Event(EventTypes::OpenCell, text));
	is_opened[i][j] = true;
}

std::string GameSingleton::GetContent(size_t i, size_t j) {
	std::string res;
	return res += map_[i][j];
}

void GameSingleton::WaveBFS(size_t i, size_t j) {
	std::vector<std::pair<size_t, size_t>> current_front;
	current_front.emplace_back(i, j);
	while (!current_front.empty()) {
		std::vector<std::pair<size_t, size_t>> new_front;
		for (auto [x, y]: current_front) {
			if (!is_opened[x][y]) {
				SetOpened(x, y);
				if (map_[x][y] == '0') {
					for (int dx = -1; dx <= 1; ++dx) {
						for (int dy = -1; dy <= 1; ++dy) {
							if (map_.Check(x + dx, y + dy) && !is_opened[x + dx][y + dy]) {
								new_front.emplace_back(x + dx, y + dy);
							}
						}
					}
				}
			}
		}
		current_front = std::move(new_front);
	}
}

GameSingleton &GameSingleton::GetGame() {
	if (game_ == nullptr) {
		game_ = new GameSingleton(20, 20, 5);
	}
	return *game_;
}

std::vector<std::vector<bool>> &GameSingleton::OpenedLink() {
	return is_opened;
}

int GameSingleton::GetStatus() {
	return status_;
}

bool GameSingleton::CheckWin() {
	size_t count = 0;
	for (size_t i = 0; i < is_opened.size(); ++i) {
		for (size_t j = 0; j < is_opened[i].size(); ++j) {
			if (is_opened[i][j]) {
				++count;
			}
		}
	}
	return (is_opened.size()*is_opened[0].size() - map_.GetMines() == count);
}


GameSingleton::GameSingleton(size_t rows, size_t columns, size_t mines)
		: map_(rows, columns, mines),
		  is_opened(rows, std::vector<bool>(columns, false)),
		  followers_(rows, std::vector<GameEventFollower *>(columns, nullptr)),
		  is_flag(rows, std::vector<bool>(columns, false))
		  {
}


GameSingleton *GameSingleton::game_{nullptr};
