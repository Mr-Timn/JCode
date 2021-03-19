#
# Made by Jordan Hoosman
#

# Sources
TARGET = main
JSCENE_DIR = ../../../JScene
DEPS = $(TARGET).o \
		JScene.o

# Building
CXX = g++
COMPILE_FLAGS = -g -w -pthread
LIBRARY_FLAGS = -lX11 \
				-lSDL2 \
				-lSDL2_image \
				-lSDL2_ttf \
				-lSDL2_mixer \
				-lavformat \
				-lavcodec \
				-lavutil \
				-lstdc++fs 

# Compiling
default: $(TARGET)

clean:
	rm *.o

$(TARGET): $(DEPS)
	$(CXX) $(COMPILE_FLAGS) -o $(TARGET) $(DEPS) $(LIBRARY_FLAGS)

$(TARGET).o: $(TARGET).cpp
	$(CXX) $(COMPILE_FLAGS) -c $(TARGET).cpp	

JScene.o: $(JSCENE_DIR)/JScene.cpp $(JSCENE_DIR)/JScene.h
	$(CXX) $(COMPILE_FLAGS) -c $(JSCENE_DIR)/JScene.cpp $(LIBRARY_FLAGS)