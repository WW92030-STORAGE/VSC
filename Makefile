CXX = g++
CXXFLAGS = -O3 -w

TARGET = main
FILE = main.cpp
PY = render.py

all: $(FILE)
	$(CXX) $(FILE) -o $(TARGET) $(CXXFLAGS) 
	./$(TARGET)
	python $(PY)

clean:
	rm -f $(TARGET)