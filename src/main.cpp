#include <iostream>

#include "../include/Board.hpp"

const int DIM = 1000;

void clamp(sf::Vector2i& pos);

int main() {
	srand(time(NULL));
	
	sf::RenderWindow window(sf::VideoMode(DIM, DIM), "Minesweeper");
	Board board(9);

	bool clicked = false;
	bool done = false;

	while (window.isOpen()) {
		sf::Event e;
		while (window.pollEvent(e)) {

			if (e.type == sf::Event::Closed) {
				window.close();
			}

			sf::Vector2i mouse_pos = sf::Mouse::getPosition() - window.getPosition() - sf::Vector2i(8, 32);

			clamp(mouse_pos);

			//std::cout << "( " << mouse_pos.x << ", " << mouse_pos.y << ")" << "\n";

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !clicked && !done) {
				//std::cout << "L-CLICKED\n";
				if (board.click(mouse_pos)) {
					std::cout << "MINE EXPLODED!\n";
					done = true;
				}

				clicked = true;
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !clicked && !done) {
				//std::cout << "R-CLICKED\n";

				if (board.flag(mouse_pos)) {
					std::cout << "WINNER!\n";
					done = true;
				}

				clicked = true;
			}
			if (e.type == sf::Event::MouseButtonReleased) {
				clicked = false;
			}
		}

		


		window.clear();


		board.draw(window);
		

		window.display();
	}

}

void clamp(sf::Vector2i& pos) {
	if (pos.x < 0) { pos.x = 0; }
	else if (pos.x >= DIM) { pos.x = DIM - 1; }

	if (pos.y < 0) { pos.y = 0; }
	else if (pos.y >= DIM) { pos.y = DIM - 1; }
}