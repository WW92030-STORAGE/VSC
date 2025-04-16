CXX = g++
CXXFLAGS = -O3 -w

TARGET = main
FILE = main.cpp
PY = render.py
PY_VIDEO = video.py
PY_GIF = gif.py

all: $(FILE)
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