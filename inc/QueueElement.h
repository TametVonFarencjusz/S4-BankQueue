#define NULLPOINTER 0 //Warto¶æ wska¼nika na NULL dla systemów Linux

class QueueElement {
private:
	int value;		//Warto¶æ przechowywana w elemencie kolejki
	QueueElement * next;	//Wska¼nik na kolejny element kolejki
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

	//Metoda zwracaj±ca warto¶æ przechowywan± w elemencie
	int GetValue() const
	{
		return value;
	}
	
	//Metoda zwracaj±ca warto¶æ wska¼nik na kolejny element kolejki
	QueueElement * GetNext() const
	{
		return next;
	}
	
	//Metoda zmieniaj±ca warto¶æ wska¼nika na kolejny element kolejki
	void SetNext(QueueElement * ptr)
	{
		next = ptr;
	}
	
	//Metoda zmieniaj±ca warto¶æ elementu
	void SetElement(int a)
	{
		value = a;
	}
};


