#ifndef __SW_ENGINE_H__
#define __SW_ENGINE_H__

#include <array>
#include <memory>
#include <list>
#include <curses.h>

template <typename T, size_t rows, size_t cols>
using GameField = std::array<std::array<T, cols>, rows>;

struct coord {
	int row, col;
};

class Engine {

public:
	Engine();
	bool initialize();
	void new_game();
	void run();
	void render();
	int process_event();
	void loop();
	void cleanup();

	void dedug_visualize_field();

private:

	void refresh_game_field();
	void clear_game_field();
	void place_snake_in_field(std::shared_ptr<std::list<coord>> snake, int player);
	void place_yum_yum_randomly();

	bool running;
	int game_field_width = 78;
	int game_field_height = 22;
	std::shared_ptr<GameField<int, 22, 78>> game_field;
	std::shared_ptr<std::list<coord>> snake_p1;
	std::shared_ptr<std::list<coord>> snake_p2;
	int last_snake_p1_direction;
	int last_snake_p2_direction;
	std::shared_ptr<coord> broccoli;

	WINDOW *window;
};

#endif
