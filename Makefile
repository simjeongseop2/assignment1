CXX = g++
TARGET = hw1
OBJECTS = main.o member.o resource.o library.o

$(TARGET) : member.h resource.h date.h library.h main.o member.o resource.o library.o
	$(CXX) -o $(TARGET) $(OBJECTS)
main.o : member.h resource.h date.h library.h main.cpp
	$(CXX) -c -o main.o main.cpp

member.o : member.h date.h member.cpp
	$(CXX) -c -o member.o member.cpp

resource.o : resource.h date.h resource.cpp
	$(CXX) -c -o resource.o resource.cpp

library.o : member.h resource.h date.h member.cpp resource.cpp library.cpp
	$(CXX) -c -o library.o library.cpp

clean :
	rm *.o $(TARGET)
