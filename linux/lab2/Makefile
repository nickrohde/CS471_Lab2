SOURCE_DRIVER=Driver.cpp
SOURCE_MAIN=main.cpp
DRIVER=driver
MAIN=main

CC=g++
STD=-std=c++17
FLAGS=-O2 -pipe -msse3

all: $(MAIN) 

$(MAIN):
	$(CC) $(SOURCE_MAIN) $(STD) $(FLAGS) -o $(DRIVER)

$(DRIVER):
	$(CC) -c $(SOURCE_DRIVER) $(STD) $(FLAGS)
	
clean: 
	rm -f $(DRIVER) *.h.gch *.o *.csv

