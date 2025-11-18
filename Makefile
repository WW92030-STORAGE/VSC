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

# gprof profiling (O0)
profile: $(FILE) preprofile doprofile

# Run and analyze
doprofile: $(FILE)
	./$(TARGET)
	gprof $(TARGET) gmon.out > analysis.txt

# Build for profiling
preprofile: $(FILE)
	$(CXX) -pg $(FILE) -o $(TARGET) -O0 -w -fcompare-debug-second

# gprof profiling (Os)
profiles: $(FILE) preprofiles doprofile

# Build for profiling
preprofiles: $(FILE)
	$(CXX) -pg $(FILE) -o $(TARGET) -Os -w -fcompare-debug-second

# gprof profiling (O3)
profile3: $(FILE) preprofile3 doprofile

# Build for profiling
preprofile3: $(FILE)
	$(CXX) -pg $(FILE) -o $(TARGET) -O3 -w -fcompare-debug-second

# Convenience

collada: 
	cd python && python3 colladaparser.py