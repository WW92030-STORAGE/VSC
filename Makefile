CXX = g++
CXXFLAGS = -O3 -w -fcompare-debug-second

TARGET = main
FILE = main.cpp
PY = render.py
PY_VIDEO = video.py
PY_GIF = gif.py

# Build and run
all: $(FILE) build run

# Build (compile)
build: $(FILE)
	$(CXX) $(FILE) -o $(TARGET) $(CXXFLAGS) 

# Run and visualize
run: $(FILE)
	./$(TARGET)
	python3 $(PY)

# Build, run, and visualize frames into video
video: $(FILE) build run
	python3 $(PY_VIDEO)
	python3 $(PY_GIF)

# clean
clean:
	rm -f $(TARGET)

# gprof profiling
profile: $(FILE) preprofile doprofile

# Build for profiling
preprofile: $(FILE)
	$(CXX) -pg $(FILE) -o $(TARGET) -w -fcompare-debug-second

# Run and analyze
doprofile: $(FILE)
	./$(TARGET)
	gprof $(TARGET) gmon.out > analysis.txt

# Convenience

collada: 
	cd python && python3 colladaparser.py