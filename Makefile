CXX = g++
CXXFLAGS = -O3 -w -fcompare-debug-second

TARGET = main
FILE = main.cpp
PY = render.py
PY_VIDEO = video.py
PY_GIF = gif.py

build: $(FILE)
	$(CXX) $(FILE) -o $(TARGET) $(CXXFLAGS) 
	./$(TARGET)
	python3 $(PY)

video: $(FILE)
	$(CXX) $(FILE) -o $(TARGET) $(CXXFLAGS) 
	./$(TARGET)
	python3 $(PY_VIDEO)
	python3 $(PY_GIF)


clean:
	rm -f $(TARGET)

profile: $(FILE)
	$(CXX) -pg $(FILE) -o $(TARGET) $(CXXFLAGS)
	./$(TARGET)
	gprof $(TARGET) gmon.out > analysis.txt