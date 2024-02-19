CC = g++
LANG_STD = -std=c++17
CFLAGS = -Wall -Wfatal-errors
INCLUDE_PATH = -I./Framework/inc/
FRAMEWORK_PATH = ./Framework/build
LIB_PATH = -L$(FRAMEWORK_PATH)
SRC_FILES = Project/src/*.cpp
LFLAGS = 	-lFrameworkRelease
OBJ = gameengine

# Makefile rules
build:
	$(CC) $(CFLAGS) $(LANG_STD) $(SRC_FILES) $(INCLUDE_PATH) $(LIB_PATH) $(LFLAGS) -o $(OBJ) -Wl,-rpath,$(FRAMEWORK_PATH);

run:
	./$(OBJ) -window 420 600

clean:
	rm $(OBJ)