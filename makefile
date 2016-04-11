CC = g++
DEBUG = -g
CFLAGS = -Wall -c -std=c++14 -Wno-unused-variable $(DEBUG)
LFLAGS = -Wall -std=c++14 -Wno-unused-variable $(DEBUG)

OBJS = parser.o code_parser.o lexer.o parser_debug.o parser_error.o data_parser.o

pasm: main.cpp $(OBJS)
	$(CC) $(LFLAGS) main.cpp $(OBJS) -o pasm

parser.o: parser/parser.cpp parser/parser.hpp
	$(CC) $(CFLAGS) parser/parser.cpp


code_parser.o: parser/code_parser.cpp parser/code_parser.hpp
	$(CC) $(CFLAGS) parser/code_parser.cpp

data_parser.o: parser/data_parser.cpp parser/data_parser.hpp
	$(CC) $(CFLAGS) parser/data_parser.cpp

lexer.o: parser/lexer.cpp parser/lexer.hpp
	$(CC) $(CFLAGS) parser/lexer.cpp

parser_debug.o: parser/parser_debug.cpp parser/parser_debug.hpp
	$(CC) $(CFLAGS) parser/parser_debug.cpp

parser_error.o: parser/parser_error.cpp parser/parser_error.hpp
	$(CC) $(CFLAGS) parser/parser_error.cpp
