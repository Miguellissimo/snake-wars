#include <memory>
#include <random>
#include <chrono>
#include <thread>
#include <curses.h>
#include "engine.h"
#include "terminal.h"
#include "easylogging/easylogging++.h"

Engine::Engine() : running(false) {
	this->initialize();
}

Engine::~Engine() {
	cleanup();
}

bool Engine::initialize() {
	game_field_width = 78;
	game_field_height = 22;

	initscr();
	clear();
	noecho();
	cbreak();
	curs_set(0);

	window = newwin(Terminal::get_instance().get_height(), Terminal::get_instance().get_width(), 0, 0);
	keypad(window, true);
	nodelay(window, true);

	return true;
}

int Engine::process_event() {

	int c = wgetch(window);
	flushinp();

	if (not has_key(c) or is_opposite_key(c, last_snake_p1_direction)) {
		c = last_snake_p1_direction;
	}

	auto head = snake_p1->back();
	coord new_position = {head.row, head.col};

	switch(c) {
	case KEY_UP:
		new_position.row -= 1;
		break;
	case KEY_RIGHT:
		new_position.col += 1;
		break;
	case KEY_DOWN:
		new_position.row += 1;
		break;
	case KEY_LEFT:
		new_position.col -= 1;
		break;
	case KEY_F(1):
		running = false;
		break;
	}

	last_snake_p1_direction = c;

	// check if snake moved out of the map
	new_position = check_if_moving_outside(new_position);

	if ((*game_field)[new_position.row][new_position.col] == 1 or
		(*game_field)[new_position.row][new_position.col] == 2) {
		LOG(INFO) << "COLLISION, Game over!";
	}

	snake_p1->push_back(new_position);

	place_snake_in_field(snake_p1, 1);

	if (not (snake_p1->back().row == broccoli->row and snake_p1->back().col == broccoli->col)) {
		snake_p1->pop_front();
	} else {
		broccoli->row = -1;
		broccoli->col = -1;
	}
}

bool Engine::is_opposite_key(int key, int ref) {
	if (ref == KEY_LEFT and key == KEY_RIGHT or
		ref == KEY_RIGHT and key == KEY_LEFT or
		ref == KEY_UP and key == KEY_DOWN or
		ref == KEY_DOWN and key == KEY_UP) {
		return true;
	} else {
		return false;
	}
}

coord Engine::check_if_moving_outside(coord new_position) {
	if (new_position.row < 0) {
		new_position.row += game_field->size();
	} else if (new_position.row >= game_field->size()) {
		new_position.row -= game_field->size();
	}

	if (new_position.col < 0) {
		new_position.col += (*game_field)[0].size();
	} else if (new_position.col >= (*game_field)[0].size()) {
		new_position.col -= (*game_field)[0].size();
	}

	return new_position;
}

void Engine::new_game() {
	game_field = std::make_shared<GameField<int, 22, 78>>();

	// initialize game_field
	clear_game_field();

	// place starting snake of player 1 and 2
	snake_p1 = std::make_shared<std::list<coord>>();
	snake_p1->push_back({21,0});
	snake_p1->push_back({21,1});
	snake_p1->push_back({21,2});

	snake_p2 = std::make_shared<std::list<coord>>();
	snake_p2->push_back({0,77});
	snake_p2->push_back({0,76});
	snake_p2->push_back({0,75});

	// place stuff to eat on the field
	broccoli = std::make_shared<coord>();
	broccoli->row = 21;
	broccoli->col = 50;

	// give both snakes a default direction
	last_snake_p1_direction = KEY_RIGHT;
	last_snake_p2_direction = KEY_LEFT;
}

void Engine::dedug_visualize_field() {
	for(int r = 0; r != game_field->size(); ++r) {
		for (int c = 0; c != (*game_field)[0].size(); ++c) {
			std::cout << (*game_field)[r][c];
		}

		std::cout << std::endl;
	}
}

void Engine::place_snake_in_field(std::shared_ptr<std::list<coord>> snake, int player) {
	for (auto it = snake->begin(); it != snake->end(); ++it) {
		(*game_field)[it->row][it->col] = player;
	}
}

void Engine::place_yum_yum_randomly() {
	if (broccoli->row == -1 and broccoli->col == -1) {
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> cg(2,game_field_width-2);
		std::uniform_int_distribution<std::mt19937::result_type> rg(2,game_field_height-2);

		broccoli->row = rg(rng);
		broccoli->col = rg(rng);
	}

	(*game_field)[broccoli->row][broccoli->col] = 3;
}

void Engine::loop() {
	running = true;

	while(running) {

		// process events:
		if (process_event() == -1) {
			running = false;
			break;
		}

		// rerender console screen
		render();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	}
}

void Engine::clear_game_field() {
	for(int r = 0; r != game_field->size(); ++r) {
		for (int c = 0; c != (*game_field)[0].size(); ++c) {
			(*game_field)[r][c] = 0;
		}
	}
}

void Engine::refresh_game_field() {
	clear_game_field();

	// apply snake on field
	place_snake_in_field(snake_p1, 1);
	place_snake_in_field(snake_p2, 2);

	// place stuff to eat on the field
	place_yum_yum_randomly();
}

void Engine::render() {
	refresh_game_field();

	// draw map border
	int width = Terminal::get_instance().get_width();
	int height = Terminal::get_instance().get_height();

	using std::string;
	string s(width-2, '-'); // TODO check if width greater than 3
	string horizontal = "+" + s + "+";

	mvwprintw(window, 0, 0, horizontal.c_str());
	mvwprintw(window, height-1, 0, horizontal.c_str());

	for(int r = 0; r != game_field->size(); ++r) {
		mvwprintw(window, r+1, 0, "|");
		for (int c = 0; c != (*game_field)[0].size(); ++c) {
			switch((*game_field)[r][c]) {
			case 0:
				mvwprintw(window, r+1, c+1, " ");
				break;
			case 1:
				mvwprintw(window, r+1, c+1, "o");
				break;
			case 2:
				mvwprintw(window, r+1, c+1, "x");
				break;
			case 3:
				mvwprintw(window, r+1, c+1, "*");
				break;
			default:
				mvwprintw(window, r+1, c+1, "?");
			}
		}

		mvwprintw(window, r+1, (*game_field)[0].size()+1, "|");
	}

	wrefresh(window);
}

void Engine::run() {
	// display some kind of ascii title and menu
	//auto menu = std::make_shared<Menu>();
	//menu->main_menu();
}

void Engine::cleanup() {
	LOG(INFO) << "Cleanup engine";

	curs_set(1);
	clrtoeol();
	refresh();
	endwin();
}
