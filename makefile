CXX = g++
CXXFLAGS = -std=c++20 -Wall -ID:\vcpkg\installed\x64-windows\include -I./lab
LDFLAGS = -lpthread -lws2_32 -lmswsock -lPowrProf -lsetupapi
TARGET = Handsome_serv
SRC = main.cpp  $(wildcard lab/*.cpp)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	del /F $(TARGET).exe
