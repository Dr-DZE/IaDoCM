CXX = g++
CXXFLAGS = -std=c++20 -Wall -w -ID:/vcpkg/installed/x64-windows/include -I"D:/lr1/Hadsome_Jack/opencv/include" -I./lab
LDFLAGS = -L"D:/lr1/Hadsome_Jack/opencv/x64/mingw/lib" -lopencv_core455 -lopencv_imgproc455 -lopencv_videoio455 -lopencv_highgui455 -lpthread -lws2_32 -lmswsock -lPowrProf -lsetupapi -lstrmiids -lquartz -lavicap32
TARGET = Handsome_serv
SRC = main.cpp lab/lab01.cpp lab/lab_02.cpp lab/lab04.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	del /F $(TARGET).exe
