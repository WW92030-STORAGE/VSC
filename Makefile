# Definitions
CXX = g++
CXXFLAGS = -O3 -w -fcompare-debug-second -MMD -MP
CXXFLAGS_0 = -w -fcompare-debug-second -MMD -MP
INCLUDES = -Iinclude

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

# Default rule: build the target executable
all: $(TARGET)

run: $(TARGET) && ./$(TARGET)

# LINK IT UP!!!
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# automatically compile some source file
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# idk what this does, include dependencies?
-include $(DEPS)

# Cleanup
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# failsafe
.PHONY: all clean