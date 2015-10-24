#include "catch/catch.hpp"
#include "engine.h"
#include <memory>

class EngineTester {
public:
	EngineTester() {
		engine = std::make_shared<Engine>();
	}

	bool is_opposite_key(int key, int ref) {
		return engine->is_opposite_key(key, ref);
	}

	coord check_if_moving_outside(coord new_position) {
		return engine->check_if_moving_outside(new_position);
	}

	int get_game_field_width() {
		return engine->game_field_width;
	}

	int get_game_field_height() {
		return engine->game_field_height;
	}

private:
	std::shared_ptr<Engine> engine;
};

TEST_CASE("are LEFT and RIGHT opposite keys", "[engine]") {
	EngineTester et;
	REQUIRE(et.is_opposite_key(KEY_LEFT, KEY_RIGHT));
}

TEST_CASE("are UP and DOWN opposite keys", "[engine]") {
	EngineTester et;
	REQUIRE(et.is_opposite_key(KEY_UP, KEY_DOWN));
}

TEST_CASE("are UP and RIGHT not opposite keys", "[engine]") {
	EngineTester et;
	REQUIRE(not et.is_opposite_key(KEY_UP, KEY_RIGHT));
}

TEST_CASE("snake moves outside of map", "[engine]") {
	EngineTester et;

	REQUIRE(et.check_if_moving_outside({-1, 10}).row == et.get_game_field_height()-1);
	REQUIRE(et.check_if_moving_outside({1, -1}).col == et.get_game_field_width()-1);
	REQUIRE(et.check_if_moving_outside({et.get_game_field_height(), 10}).row == 0);
	REQUIRE(et.check_if_moving_outside({1, et.get_game_field_width()}).col == 0);

}
