CXX = g++

CFLAGS = -std=c++11 -w
DIR = code
MAIN = processor_simulator

execute:
	cd $(DIR)/; lex scanLatency.l && yacc -d parseLatency.y; cd ../;
	$(CXX) $(DIR)/main.cpp $(DIR)/lex.yy.c $(DIR)/y.tab.c $(DIR)/statistics.cpp $(DIR)/scan.cpp $(DIR)/arm.cpp $(CFLAGS) -o $(MAIN) -DDebug=0
	./$(MAIN) $(input)

debug:
	cd $(DIR); lex scanLatency.l && yacc -d parseLatency.y; cd ../;	
	$(CXX) $(DIR)/main.cpp $(DIR)/lex.yy.c $(DIR)/y.tab.c $(DIR)/statistics.cpp $(DIR)/scan.cpp $(DIR)/arm.cpp $(CFLAGS) -o $(MAIN) -DDebug=1
	./$(MAIN) $(input)

clean:
	rm $(MAIN)
