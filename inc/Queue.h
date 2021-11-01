#include "QueueElement.h"
#include "EmptyQueueException.h"

#define NULLPOINTER 0 //Warto�� wska�nika na NULL dla system�w Linux

class Queue {
private:
	QueueElement * front; //Wska�nik na pierwszy element kolejki
public:
	//Konstruktor pustej kolejki
	Queue()
	{
		front = NULLPOINTER;
	}	
	//Destruktor
	~Queue()
	{
		delete front;
	}
	
	//Metoda publiczna zwracaj�ca wska�nik na pierwszy element w kolejce. 
	QueueElement * Front() const
	{
		if (IsEmpty()) throw EmptyQueueException(); //Wyj�tek pustej kolejki
		return front;
	}

	//Metoda publiczna zwracaj�ca
	bool IsEmpty() const
	{
		return front == NULLPOINTER;
	}
	
	//Metoda publiczna zwracaj�ca ilo�� element�w w kolejce
	int Size() const
	{
		int size = 0;
		QueueElement * ptr = front;	
		while (ptr != NULLPOINTER)	//Szukanie ostatniego elementu (wskazuje na warto�� NULL)
		{
			size++;
			ptr = ptr->GetNext();	//Przechodzenie do kolejnego elementu kolejki
		}
		return size;
	}
	
	//Metoda publiczna zwracaj�ca warto�� pierwszego elementu w kolejce (najdawniej dodanego) wraz z usuni�ciem go
	int Dequeue() 
	{
		if (IsEmpty()) throw EmptyQueueException(); 	//Wyj�tek pustej kolejki
		
		QueueElement * futureFront = front->GetNext();	//Zapami�tanie wska�nika na drugi element
		int popValue = front->GetValue();		//Zapami�tanie warto�ci przechowywanej w pierwszym elemencie
		
		front->SetNext(NULLPOINTER); 			//Zmiania wska�nika next, aby nie usun�� innych element�w
		delete front;					//Usuwanie zdekolejkowanego elementu
		
		front = futureFront; 				//Zmiania wska�nika "front", aby wskazywa� nowy pierwszy element (poprzedni drugi element)
		
		return popValue;
	}
	
	//Metoda publiczna dodaj�ca element o podanej warto�ci
	void Enqueue(int a)
	{
		if (IsEmpty())
		{
			QueueElement * newElement = new QueueElement(a);	//Tworzenie nowego elementu kolejki
			front = newElement; 					//Zmiania wska�nika "front", aby wskazywa� nowo utworzony pierwszy element
		}
		else
		{
			//Szukanie ostatniego elementu kolejki
			QueueElement * ptr = front;				
			while (ptr->GetNext() != NULLPOINTER) 			
			{
				ptr = ptr->GetNext();
			} 
			QueueElement * newElement = new QueueElement(a); 	//Tworzenie nowego elementu
			ptr->SetNext(newElement);  				//Zmiania wska�nika "next" ostatniego elementu na nowo utworzony element
		}
	}
};


