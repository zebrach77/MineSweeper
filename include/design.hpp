#ifndef DESIGN_HPP
#define DESIGN_HPP
#include <SFML/Graphics.hpp>

sf::Color GetNumberColor(char x) {
	if (x == '*') {
		return sf::Color::Red;
	}
	uint8_t color = 50u + ((uint8_t) x - (uint8_t) '0') * 25u;
	color = 255 - color;
	if (x == '1' || x == '4' || x == '7') {
		return sf::Color{0, 0, color};
	}
	if (x == '2' || x == '5' || x == '8') {
		return sf::Color{0, color, 0};
	}
	if (x == '3' || x == '6') {
		return sf::Color{color, 0, 0};
	}
	return sf::Color::White;
}
#endif