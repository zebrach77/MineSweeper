#ifndef MINE_SWEEPER_MAP_HPP
#define MINE_SWEEPER_MAP_HPP

#include <string>
#include <vector>

class MineSweeperMap {
public:
  MineSweeperMap(size_t rows, size_t columns, size_t mines);

  std::vector<std::pair<size_t, size_t>> GenerateMines(
    size_t no_x, 
    size_t no_y, 
    size_t mines
  );

  void GenerateField(std::vector<std::pair<size_t, size_t>>& mines);

  std::vector<char>& operator[] (size_t row);

  bool IsInitialized();

  void InitMap(size_t x, size_t y);

  bool Check(size_t x, size_t y);

  size_t Rows();


private:

  static std::string CoordsToString(size_t x, size_t y);

  std::vector<std::vector<char>> field_;
  bool initialized_{false};
  size_t rows_;
  size_t columns_;
  size_t mines_;
};

#endif