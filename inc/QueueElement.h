#define NULLPOINTER 0 //Warto�� wska�nika na NULL dla system�w Linux

class QueueElement {
private:
	int value;		//Warto�� przechowywana w elemencie kolejki
	QueueElement * next;	//Wska�nik na kolejny element kolejki
public:
	//Konstruktor 
	QueueElement(int a, QueueElement * ptr = NULLPOINTER)
	{
		value = a;
		next = ptr;
	}
		
	//Destruktor
	~QueueElement()
	{
		delete next;
	}

	//Metoda zwracaj�ca warto�� przechowywan� w elemencie
	int GetValue() const
	{
		return value;
	}
	
	//Metoda zwracaj�ca warto�� wska�nik na kolejny element kolejki
	QueueElement * GetNext() const
	{
		return next;
	}
	
	//Metoda zmieniaj�ca warto�� wska�nika na kolejny element kolejki
	void SetNext(QueueElement * ptr)
	{
		next = ptr;
	}
	
	//Metoda zmieniaj�ca warto�� elementu
	void SetElement(int a)
	{
		value = a;
	}
};


