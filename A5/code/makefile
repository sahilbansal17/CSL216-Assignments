CXX = g++

CFLAGS = -std=c++11 -w

MAIN = a.out

execute:
	lex scanLatency.l && yacc -d parseLatency.y
	$(CXX) main.cpp lex.yy.c y.tab.c $(CFLAGS) -o $(MAIN) -DDebug=0
	./$(MAIN) $(input)

debug:
	lex scanLatency.l && yacc -d parseLatency.y
	$(CXX) main.cpp lex.yy.c y.tab.c $(CFLAGS) -o $(MAIN) -DDebug=1
	./$(MAIN) $(input)

clean:
	rm $(MAIN)
