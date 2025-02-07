CXX = g++
CXXFLAGS = -O3 -w -std=c++11

TARGET = main
FILE = main.cpp
PY = render.py

all: $(FILE)
	$(CXX) $(FILE) -o $(TARGET) $(CXXFLAGS) 
	./$(TARGET)
	python $(PY)

clean:
	rm -f $(TARGET)