# Definitions
CXX = g++
CXXFLAGS = -O3 -fcompare-debug-second -MMD -MP -flto
INCLUDES = -Iinclude

# add -pg to CXXFLAGS to enable gprof

TARGET = main
FILE = main.cpp
BUILD_DIR = build

# Python files for subsequent operations
PY = render.py
PY_VIDEO = video.py
PY_GIF = gif.py

SRCS = $(FILE) $(shell find src -name '*.cpp')

# List of desired objects from sources
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# Dependencies
DEPS = $(OBJS:.o=.d)

# Build!
build: $(TARGET)

# Run!
run: 
	./$(TARGET)
	python3 $(PY)

# Build and run!
all: build run

# Build, run, and visualize frames into video
video: $(FILE) build run
	python3 $(PY_VIDEO)
	python3 $(PY_GIF)

# LINK IT UP!!!
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) -o $(TARGET)

# automatically compile some source file
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# idk what this does, include dependencies?
-include $(DEPS)

# Cleanup
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Testing and profiling (some things might require modifying the flags)

vg:
	valgrind ./main --track-origins=yes

doprofile: $(FILE)
	./$(TARGET)
	gprof $(TARGET) gmon.out > analysis.txt

# Convenience

collada: 
	cd python && python3 colladaparser.py


# failsafe
.PHONY: all clean


