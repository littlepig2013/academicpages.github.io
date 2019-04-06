CXX = g++
RM = -rm -f
CXXFLAGS =-g -std=c++0x
LDLIBS =-I ~/include/

OTHER_OBJECTS = Utils.o HIN_Graph.o TopKCalculator.o DataReader.o

all: topKQuery topKQueryTest

topKQuery: $(OTHER_OBJECTS)
	$(CXX) $(CXXFLAGS) -o topKQuery $(OTHER_OBJECTS) main.cpp

topKQueryTest: $(OTHER_OBJECTS)
	$(CXX) $(CXXFLAGS) -o topKQueryTest $(OTHER_OBJECTS) test.cpp 

clean:
	$(RM) topKQuery topKQueryTest $(OTHER_OBJECTS)



