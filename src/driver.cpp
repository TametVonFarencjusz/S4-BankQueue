#include <iostream>
#include "../inc/Queue.h"

int main()
{
	Queue queue; //Utworzenie obiektu kolejki
	std::cout << "*We have just created an empty queue*" << std::endl;
	std::cout << "Count of elements in a queue: " << queue.Size() << std::endl; //Wyświetlenie elementów w kolejce
	std::cout << "Queue is empty: "; queue.IsEmpty()?std::cout<<"Yes":std::cout<<"No"; std::cout << std::endl << std::endl; //Sprawdzanie czy w kolejce znajduje się jakiś element
	
	
	for (int i = 10; i > 0; i--)
	{
		std::cout << "Count of elements in the queue (before Enqueue): " << queue.Size() << std::endl; //Wyświetlenie elementów w kolejce
		std::cout << "Value of enqueued element: " << i << std::endl;
		queue.Enqueue(i); //dodanie elementu do kolejki
		std::cout << "Count of elements in the queue (after Enqueue): " << queue.Size() << std::endl; //Wyświetlenie elementów w kolejce
		std::cout << std::endl;
	}
	
	std::cout << std::endl;
	std::cout << "*Added 10 elements with values from 10 to 1*" << std::endl << std::endl;
	
	std::cout << "Value of the first element: " << queue.Front()->GetValue() << std::endl << std::endl; //Wyświetlenie wartości pierwszego elementu
	
	while(!queue.IsEmpty())
	{
		std::cout << "Count of elements in the queue (before Dequeue): " << queue.Size() << std::endl; //Wyświetlenie elementów w kolejce
		std::cout << "Value of dequeued element: " << queue.Dequeue() << std::endl; //Pobranie i wyświetlenie wartości pierwszego elementu
		std::cout << "Count of elements in the queue (after Dequeue): " << queue.Size() << std::endl; //Wyświetlenie elementów w kolejce
		std::cout << "Queue is empty: "; queue.IsEmpty()?std::cout<<"Yes":std::cout<<"No"; std::cout << std::endl; //Sprawdzanie czy w kolejce znajduje się jakiś element
		std::cout << std::endl;
	}
	
	//Pierwszy element
	
	std::cout << "*Let's try to dequeue element from an empty queue*"<< std::endl;
	try
	{
		std::cout << "Value of dequeued element: " << queue.Dequeue() << std::endl; //Próba pobrania elementu z kolejki
	}
	catch(EmptyQueueException) //Łapanie wyjątku pustej kolejki
	{
		std::cout << "Exception caught: Empty Queue" << std::endl;
	}
}

