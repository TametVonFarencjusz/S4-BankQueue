#include "QueueElement.h"
#include "EmptyQueueException.h"

#define NULLPOINTER 0 //Warto¶æ wska¼nika na NULL dla systemów Linux

class Queue {
private:
	QueueElement * front; //Wska¼nik na pierwszy element kolejki
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
	
	//Metoda publiczna zwracaj±ca wska¼nik na pierwszy element w kolejce. 
	QueueElement * Front() const
	{
		if (IsEmpty()) throw EmptyQueueException(); //Wyj±tek pustej kolejki
		return front;
	}

	//Metoda publiczna zwracaj±ca
	bool IsEmpty() const
	{
		return front == NULLPOINTER;
	}
	
	//Metoda publiczna zwracaj¹ca ilo¶æ elementów w kolejce
	int Size() const
	{
		int size = 0;
		QueueElement * ptr = front;	
		while (ptr != NULLPOINTER)	//Szukanie ostatniego elementu (wskazuje na warto¶æ NULL)
		{
			size++;
			ptr = ptr->GetNext();	//Przechodzenie do kolejnego elementu kolejki
		}
		return size;
	}
	
	//Metoda publiczna zwracaj¹ca warto¶æ pierwszego elementu w kolejce (najdawniej dodanego) wraz z usuniêciem go
	int Dequeue() 
	{
		if (IsEmpty()) throw EmptyQueueException(); 	//Wyj±tek pustej kolejki
		
		QueueElement * futureFront = front->GetNext();	//Zapamiêtanie wska¼nika na drugi element
		int popValue = front->GetValue();		//Zapamiêtanie warto¶ci przechowywanej w pierwszym elemencie
		
		front->SetNext(NULLPOINTER); 			//Zmiania wska¼nika next, aby nie usun¹æ innych elementów
		delete front;					//Usuwanie zdekolejkowanego elementu
		
		front = futureFront; 				//Zmiania wska¼nika "front", aby wskazywa³ nowy pierwszy element (poprzedni drugi element)
		
		return popValue;
	}
	
	//Metoda publiczna dodaj¹ca element o podanej warto¶ci
	void Enqueue(int a)
	{
		if (IsEmpty())
		{
			QueueElement * newElement = new QueueElement(a);	//Tworzenie nowego elementu kolejki
			front = newElement; 					//Zmiania wska¼nika "front", aby wskazywa³ nowo utworzony pierwszy element
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
			ptr->SetNext(newElement);  				//Zmiania wska¼nika "next" ostatniego elementu na nowo utworzony element
		}
	}
};


