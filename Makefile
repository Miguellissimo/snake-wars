# specify the used compiler
CXX = g++

# program name
APP_NAME = snake_wars

# specified compiler and linker flags
CFLAGS = -std=c++1y -Wall #-stdlib=libc++
LDFLAGS = -lncurses -lSDL2

# paths
CATCH_PATH = tests/catch.hpp
CPP_FILES := $(wildcard src/*.cpp)
TEST_FILES := $(wildcard tests/*.cpp)
SRC_DIR = -I./src

LIBRARY_INCLUDE = -I./extern/include

# Libraries:
CATCH = extern/include/catch
EASYLOGGING = extern/include/easylogging

all: debug

release: logger
	$(CXX) $(CFLAGS) $(CPP_FILES) $(LIBRARY_INCLUDE) -o $(APP_NAME) $(LDFLAGS)

debug: logger
	$(CXX) $(CFLAGS) -g -O0 $(CPP_FILES) $(LIBRARY_INCLUDE) -o $(APP_NAME)-debug $(LDFLAGS)

logger:
ifeq ("$(wildcard $(EASYLOGGING)/*)","")
	# TODO use latest stable release
	wget -P $(EASYLOGGING) https://github.com/easylogging/easyloggingpp/blob/master/src/easylogging%2B%2B.h
endif

test: 
ifeq ("$(wildcard $(CATCH))","")
	wget -P $(CATCH) https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp
endif
	$(CXX) $(CFLAGS) -DUNIT_TEST=1 -g $(SRC_DIR) $(filter-out src/main.cpp, $(CPP_FILES)) $(TEST_FILES) $(LIBRARY_INCLUDE) -o tester $(LDFLAGS)
	./tester

clean:
	rm -f *.o tester $(APP_NAME)-debug $(APP_NAME)
	rm -rf extern
	rm -rf logs

run:
	./$(APP_NAME)-debug