CC = g++
DEBUG = -g
CFLAGS = -Wall -c -std=c++14 -Wno-unused-variable $(DEBUG)
LFLAGS = -Wall -std=c++14 -Wno-unused-variable $(DEBUG)

OBJS = parser.o code_parser.o lexer.o parser_debug.o parser_error.o data_parser.o encoder.o

PARSER_INCLUDES = $(wildcard parser/*.hpp)

pasm: main.cpp $(OBJS) $(PARSER_INCLUDES)
	$(CC) $(LFLAGS) main.cpp $(OBJS) -o pasm

encoder.o: encoder/encoder.cpp encoder/encoder.hpp
	$(CC) $(CFLAGS) encoder/encoder.cpp

parser.o: parser/parser.cpp parser/parser.hpp $(PARSER_INCLUDES)
	$(CC) $(CFLAGS) parser/parser.cpp


code_parser.o: parser/code_parser.cpp parser/code_parser.hpp $(PARSER_INCLUDES)
	$(CC) $(CFLAGS) parser/code_parser.cpp

data_parser.o: parser/data_parser.cpp parser/data_parser.hpp $(PARSER_INCLUDES)
	$(CC) $(CFLAGS) parser/data_parser.cpp

lexer.o: parser/lexer.cpp parser/lexer.hpp $(PARSER_INCLUDES)
	$(CC) $(CFLAGS) parser/lexer.cpp

parser_debug.o: parser/parser_debug.cpp parser/parser_debug.hpp $(PARSER_INCLUDES)
	$(CC) $(CFLAGS) parser/parser_debug.cpp

parser_error.o: parser/parser_error.cpp parser/parser_error.hpp $(PARSER_INCLUDES)
	$(CC) $(CFLAGS) parser/parser_error.cpp
