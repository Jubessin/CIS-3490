#
# Course: CIS*3490 The Design and Analysis of Algorithms
# Assignment: 2
#
# Name: Justin Erogun
# ID: 1144036
# Date: 02/12/2022
#

CFLAGS= -std=c99 -lm
SOURCES = $(wildcard ./*.c)
TARGET = a2

all: $(TARGET)

$(TARGET): $(SOURCES)
	gcc $(CFLAGS) -o $@ $^

clean:
	rm -rf $(TARGET)