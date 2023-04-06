
#include "MineSweeperMap.hpp"

#include <unordered_map>
#include <random>

MineSweeperMap::MineSweeperMap(size_t rows, size_t columns, size_t mines) 
  : rows_(rows), columns_(columns), mines_(mines) {
    field_.assign(rows_, std::vector<char>(columns_, '0'));

  }

std::vector<std::pair<size_t, size_t>> MineSweeperMap::GenerateMines(
  size_t no_x, 
  size_t no_y, 
  size_t mines
) {
  std::unordered_map<std::string, std::pair<size_t, size_t> > cells;
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < columns_; ++j) {
      cells[CoordsToString(i, j)] = {i, j};
    }
  }
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      if (Check(no_x + dx, no_y + dy)) {
        cells.erase(cells.find(CoordsToString(no_x + dx, no_y + dy)));
      }
    }
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::vector<std::pair<size_t, size_t>> mines_coords;
  for (size_t i = 0; i < mines; ++i) {
    std::uniform_int_distribution<size_t> distribution(0, cells.size() - 1);
    size_t ind = distribution(gen);
    auto it = std::next(std::begin(cells), ind);
    auto [key, value] = *it;
    auto [x, y] = value;
    mines_coords.emplace_back(x, y);
    cells.erase(it);
  }
  return mines_coords;
}

void MineSweeperMap::GenerateField(std::vector<std::pair<size_t, size_t>>& mines) {
  for (auto& [x, y] : mines) {
    field_[x][y] = '*';
    for (int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        if (Check(x + dx, y + dy)) {
          field_[x + dx][y + dy] += 1;
        }
      }
    }
  }
}

std::vector<char>& MineSweeperMap::operator[] (size_t row) {
  return field_[row];
}

bool MineSweeperMap::IsInitialized() {
  return initialized_;
}

void MineSweeperMap::InitMap(size_t x, size_t y) {
  initialized_ = true;
  auto mines_coords = GenerateMines(x, y, mines_);
  GenerateField(mines_coords);
}

bool MineSweeperMap::Check(size_t x, size_t y) {
  if (x < 0 || x >= rows_) {
    return false;
  }
  if (y < 0 || y >= columns_) {
    return false;
  }
  return field_[x][y] != '*';
}

size_t MineSweeperMap::Rows() {
  return rows_;
}

std::string MineSweeperMap::CoordsToString(size_t x, size_t y) {
  return std::to_string(x) + "," + std::to_string(y);
}
