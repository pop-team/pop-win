parser.tab.cc parser.tab.h: y.tab.c y.tab.h
	mv y.tab.c parser.tab.cc
	mv y.tab.h parser.tab.h

y.tab.c y.tab.h: parser.y
	yacc -d parser.y

clean:
	rm *.tab.*
