#ifndef __TERMINAL_H__
#define __TERMINAL_H__

class Terminal {
public:
	static Terminal& get_instance() {
		static Terminal t;
		return t;
	}

	int get_width() { return width; }
	int get_height() { return height; }

private:
	Terminal();
	int width;
	int height;
};

#endif
