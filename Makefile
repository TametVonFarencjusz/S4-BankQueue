CPPFLAGS= -c -g -Iinc -Wall -pedantic -std=c++17 

__start__: clear Driver Bank
	./Bank

Bank: 
	g++ -o Bank src/main.cpp inc/Queue.h inc/QueueElement.h inc/EmptyQueueException.h -lpthread -lsfml-graphics -lsfml-window -lsfml-system

Driver:
	g++ -o Driver src/driver.cpp inc/Queue.h inc/QueueElement.h inc/EmptyQueueException.h -lpthread

clear:
	rm -f obj/*.o Bank Driver
