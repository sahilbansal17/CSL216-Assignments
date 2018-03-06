CXX = g++

CFLAGS = -std=c++11

MAIN = a.out

execute:
	$(CXX) main.cpp $(CFLAGS) -o $(MAIN) -DDebug=0
	./$(MAIN) $(input)

debug:
	$(CXX) main.cpp $(CFLAGS) -o $(MAIN) -DDebug=1
	./$(MAIN) $(input)

clean:
	rm $(MAIN)