#ifndef BOARD_HPP
#define BOARD_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

sf::Color colors[9] = {
	sf::Color(192, 192, 192),
	sf::Color(0, 0, 255),
	sf::Color(0, 126, 0),
	sf::Color(255, 0, 0),
	sf::Color(0, 0, 125),
	sf::Color(125, 0, 0),
	sf::Color(0, 125, 125),
	sf::Color(0, 0, 0),
	sf::Color(125, 125, 125),
};

struct Cell {
	bool has_mine = false;
};

class Board {
public:

	Board();

	Board(unsigned int dim);

	void draw(sf::RenderWindow& window);

	bool click(const sf::Vector2i& mouse);

	bool flag(const sf::Vector2i& mouse);

	int surround(int x, int y);
protected:
private:

	unsigned int _dim;

	std::vector<Cell> _cells;
	std::vector<bool> _revealed;
	std::vector<bool> _flagged;
	std::vector<int> mines;

	sf::Font font;

	int to_index(int x, int y);

	void reveal(int x, int y);
};

inline Board::Board() {
	// Do literally nothing
}

inline Board::Board(unsigned int dim) {
	_dim = dim;

	_cells.resize(_dim * _dim);
	_revealed.resize(_dim * _dim, false);
	_flagged.resize(_dim * _dim, false);

	font.loadFromFile("./resources/mine-sweeper.ttf");

	// Randomize mines

	std::vector<int> generated;

	for (int i = 0; i < 10; ++i) {

		int r = rand() % (_dim * _dim);

		while (std::find(generated.begin(), generated.end(), r) != generated.end()) {
			r = rand() % (_dim * _dim);
		}

		generated.push_back(r);
		mines.push_back(r);

		_cells[r].has_mine = true;
	}

	generated.clear();
}

inline void Board::draw(sf::RenderWindow& window) {

	float _size = 1000.f / _dim;

	sf::RectangleShape cell(sf::Vector2f(_size, _size));
	cell.setOutlineThickness(2);
	cell.setOutlineColor(sf::Color(125, 125, 125));

	for (int y = 0; y < _dim; ++y) {
		for (int x = 0; x < _dim; ++x) {

			cell.setPosition(x * _size, y * _size);

			sf::Text count("", font, _size * .75);
			count.setPosition(cell.getPosition());

			bool text = false;
			int index = to_index(x, y);



			if (_flagged[index]) {
				cell.setFillColor(sf::Color(200, 200, 0));		// Flagged
			}
			else if (!_revealed[index]) {
				cell.setFillColor(sf::Color(255, 255, 255));	// Hidden
			}
			else {
				int surr = surround(x, y);

				cell.setFillColor(sf::Color(200, 200, 200));
				count.setFillColor(colors[surr]);
				count.setString(std::to_string(surr));

				sf::Vector2f newPos;

				newPos.x = count.getPosition().x + (_size * .5) -(count.getGlobalBounds().getSize().x * .5);
				newPos.y = count.getPosition().y + (_size * .5) -(count.getGlobalBounds().getSize().y * .5);

				count.setPosition(newPos);

				if (_cells[index].has_mine) {
					cell.setFillColor(sf::Color::Black);
				}
				else {
					text = true;
					//window.draw(count);
				}


			}

			window.draw(cell);
			if (text) {
				window.draw(count);
			}

		}
	}
}

inline bool Board::click(const sf::Vector2i& mouse) {
	float _size = 1000.f / _dim;

	int x = mouse.x / _size, y = mouse.y / _size;

	if (!_flagged[to_index(x, y)]) {
		_revealed[to_index(x, y)] = true;

		if (_cells[to_index(x, y)].has_mine) {

			_flagged.clear();
			_revealed.clear();

			_flagged.resize(_dim * _dim, false);
			_revealed.resize(_dim * _dim, true);

			return true;
		}
		else {
			// Recursive reveal function
			
			if (surround(x, y) == 0) {
				reveal(x, y);
			}
		}

	}

	return false;
}

inline bool Board::flag(const sf::Vector2i& mouse) {
	float _size = 1000.f / _dim;

	int x = mouse.x / _size, y = mouse.y / _size;

	if (!_revealed[to_index(x, y)]) {
		_flagged[to_index(x, y)] = !_flagged[to_index(x, y)];
	}

	bool finished = true;
	for (auto i : mines) {
		if (!_flagged[i]) {
			finished = false;
			break;
		}
	}

	if (finished) {
		_flagged.clear();
		_revealed.clear();

		_flagged.resize(_dim * _dim, false);
		_revealed.resize(_dim * _dim, true);
	}

	return finished;
}

inline int Board::surround(int x, int y) {
	int surr = 0;

	for (int i = x - 1; i <= x + 1; ++i) {
		for (int ii = y - 1; ii <= y + 1; ++ii) {

			if (i >= 0 && i < _dim && ii >= 0 && ii < _dim) {
				if (_cells[to_index(i, ii)].has_mine) { ++surr; }
			}

		}
	}

	return surr;
}

inline int Board::to_index(int x, int y) {
	return (y * _dim + x);
}

inline void Board::reveal(int x, int y) {
	// Visit all (at most 8) around the cell and reveal

	for (int i = x - 1; i <= x + 1; ++i) {
		for (int ii = y - 1; ii <= y + 1; ++ii) {

			int index = to_index(i, ii);

			if (i >= 0 && i < _dim && ii >= 0 && ii < _dim && !_revealed[index]) {

				_revealed[index] = true;

				if (surround(i, ii) == 0) {
					reveal(i, ii);
				}
			}

		}
	}
}

#endif // BOARD_HPP