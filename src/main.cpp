#include <iostream>
#include <stdlib.h>
#include <time.h>
#include<unistd.h>
#include<string>

#include <SFML/Graphics.hpp>

#include "../inc/Queue.h"



/*
Program obsługi kolejki bankowej - autor: Szymona Startek

*Pliki i biblioteki*
W programie zostały wykorzystane:
-klasy Kolejki oraz Elementów Kolejki, struktura Wyjątku Pustej Kolejki - autor: Szymona Startek 
-biblioteka SFML do obsługi GUI,
-czcionka arial pobrana ze strony https://www.freefontspro.com/14454/arial.ttf

*Działanie programu*
Program obsługi kolejki bankowej pozwala na wybranie początkowej ilości osób w kolejce. Następnie zaczyna się automatyczne pobieranie oraz dodawanie nowych elementów do kolejki. Program kończy automatyczną pracę, gdy kolejka jest pusta. Jedyną rzeczą jaką może wtedy zrobić użytkownik jest wyłączenie programu.

Program jest podzielony na trzy fazy.

Podczas pierwszej fazy użytkownik może wybrać ile elementów chce posiadać na początku w kolejce. Do obsługi używa się przycisków UP, DOWN oraz ENTER. Faza się kończy po potwierdzeniu ilości osób w kolejce.
Podczas drugiej fazy co pewien czas (TIME_DEQUEUE) zostaje pobrany element z kolejki i wartość tego elementu jest wypisywana w oknie. Wypisywana jest również wartość czasu do pobrania kolejnego elementu oraz ilość elementów przechowywanych w kolejce. Jednocześnie dodawany jest nowy element do kolejki co pewien losowy czas (TIME_ENQUEUE). Faza kończy się po pobraniu ostatniego elementu kolejki.
Podczas trzeciej fazy jest wypisywana wartość ostatniego pobranego elementu oraz wiadomość o skończeniu pracy.

Podczas każdej fazy można wyłączyć program wciskając przycisk ESCAPE lub wciskając X na pasku okna.

*/

//Polskie znaki - UTF-8: ą ć ę ł ń ó ś ź ż

#define TIME_ENQUEUE 7000		//Czas do pobrania kolejnego elementu z kolejki [ms]
#define TIME_DEQUEUE 5000		//Średni czas do dodania elementu do kolejki [ms]
#define TIME_SLEEP 100			//Czas uśpienia programu [ms]
#define TIME_KEYPRESS 200		//Czas nieprzyjmowania powiadomnienia o wciśnięciu przycisków (poza ESCAPE), aby nie naliczyć za dużo wciśnięć na pojedynczym wciśnięciu [ms]

int main()
{
	srand(time(NULL)); 				//Inicjowanie generatora liczb pseudolosowych
	
	sf::Font font; 				//Tworzenie obiektu czcionki
	if (!font.loadFromFile("font/arial.ttf")) 	//Wczytywanie czcionki "arial.tff" z pliku
	{
		std::cerr << "ERROR: Font not found" << std::endl; 	//Błąd przy wczytywaniu pliku: Brak pliku z czcionką
	    	return -1;						//Zakończenie programu z powodu nie znalezienia czcionki
	}

	

	//************************//
    	sf::RenderWindow window(sf::VideoMode(800, 600), "Bank by Szymon Startek"); 	//Utworzenie obiektu okna o wymiarach 600x800 i nazwie "Bank by Szymon Startek"
	//************************//
	
	sf::Image icon;				//Tworzenie obiektu obrazu (ikony)
	if (icon.loadFromFile("icon/Icon.png"))	//Wczytywanie ikony "Icon.png" z pliku
	{
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());	//Ustawianie wczytanego obrazu jako ikona okna
	} 
	else std::cerr << "ERROR: Icon not found" << std::endl; //Błąd przy wczytywaniu pliku: Brak pliku z ikoną
	
	//************************//
	Queue queue; 				//Utworzenie kolejki, na której będzie się opierać cała praca programu
	int id = 1; 				//Zmienna, której wartość wpisywana do elemtów kolejki (jest powiększana po dodaniu elementu do kolejki)
	int lastID;				//Wartość wypisywana przez obiekt "messageUserID" mówiąca o ID ostatniego pobranego elementu kolejki
	bool flagStart = true;			//Zmienna odpowiadająca za aktualne działanie fazy pierwszej
	int waitKeyPressed = 0;		//Czas, w którym niektóre funkcje odpowiadające za sprawdzanie wciśniętego przycisku są wyłączone
	int waitAutoDequeue = 0;		//Czas, po którym zostaje automatycznie pobrany element z kolejki
	int startLine = 10; 			//Ilość początkowych elementów w kolejce
	//************************//

	while (window.isOpen()) //pętla trwająca dopóki jest otwarte okno
	{
		//************************************************************************//

		//***Sprawdzanie czy została otrzymana proźba o zamknięcie okna (np. wciśnięcie X na pasku okna)***//
        	sf::Event event;
        	while (window.pollEvent(event))
		{
		    if (event.type == sf::Event::Closed)
		        window.close();
		}
		
		//************************************************************************//

		window.clear();	//Wyczyszczenie okna "window" ze wszystkich obiektów, aby móc narysowane nowe metodą "draw"	

		//************************************************************************//

		//Uśpienie programu na 1ms, aby odmierzać czas 
		if (waitKeyPressed > 0 || waitAutoDequeue > 0) //Sprawdzanie czy "waitKeyPressed" lub "waitAutoDequeue" są większe od 0
		{
			sf::sleep(sf::milliseconds(TIME_SLEEP)); //zatrzymanie programu na 100ms (TIME_SLEEP), aby nie otrzymywał żadnych wejść - mniejsza wartość spowalnia znacznie bardziej program niż powinno (dla wartości 1ms program wydłużał się 2.5 razy i zamiast zatrzymywać się na 5s zatrzymywał się na ok. 12s)
			waitKeyPressed-=TIME_SLEEP; 			//zmniejszenie wartości zmiennej "waitKeyPressed" o 100 (TIME_SLEEP)
			waitAutoDequeue-=TIME_SLEEP; 			//zmniejszenie wartości zmiennej "waitKeyPressed" o 100 (TIME_SLEEP)
		} 

		if (flagStart) //Pierwsza faza programu, w której ustala się ilość osób na początku
		{
			if (waitKeyPressed <= 0) //Sprawdzanie czy czas, po którym można ponownie wcisnąć przycisk minął 
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) //Sprawdzenie czy wciśnięty jest przycisk UP
				{
				    	startLine++; //Zwiększenie ilości osób na początku
					waitKeyPressed = TIME_KEYPRESS; //Uśpienie programu na 0.2s, aby przytrzymanie przycisku UP nie zmieniało za szybko ilości osób
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && startLine > 1) //Sprawdzenie czy wciśnięty jest przycisk DOWN oraz czy ilość osób w kolejce jest większa od 1
				{
				    	startLine--; //Zmiejszanie ilości osób na początku
					waitKeyPressed = TIME_KEYPRESS; //Uśpienie programu na 0.2s, aby przytrzymanie przycisku DOWN nie zmieniało za szybko ilości osób

				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) //Sprawdzenie czy wciśnięty jest przycisk ENTER/RETURN
				{
				    flagStart = false; //Przejście do drugiej fazy programu, w której zaczyna się praca na kolejce
					for (int j = 0; j < startLine ; j++) //Dodanie początkowych elementów do kolejki
					{
						queue.Enqueue(id);		//Dodawanie elemetów do kolejki
						id++;				//Zwiększenie wartości id, aby kolejne wartości elementów się różniły
					}
					waitKeyPressed = TIME_KEYPRESS; //Uśpienie programu na 0.2s, aby przytrzymanie przycisku ENTER nie wywołało za dużo razy tego samego
				}
				
			//***************DRAW***************//
				
			//Wiadomość o początkowej ilości osób w kolejce
			sf::Text message("Count of users at the begining: " + std::to_string(startLine), font);		//Tworzenie obiektu wiadomości
			message.move(100.f, 200.f);									//Pozycja wiadomości w oknie
			window.draw(message);										//Rysowanie wiadomości w oknie - musi zostać później wywołana metoda window.display()
			//Wiadomość informująca o przycisku zwiększającym ilość osób na początku w kolejce
			sf::Text messageUP("Press UP to add user", font);
			messageUP.move(100.f, 300.f);
			window.draw(messageUP);
			//Wiadomość informująca o przycisku zmiejszającym ilość osób na początku w kolejce
			sf::Text messageDOWN("Press DOWN to delete user", font);
			messageDOWN.move(100.f, 350.f);
			window.draw(messageDOWN);
			//Wiadomość informująca o przycisku, który pozwala zacząć pracę na kolejce
			sf::Text messageENTER("Press ENTER/RETURN to start", font);
			messageENTER.move(100.f, 400.f);
			window.draw(messageENTER);
			//Wiadomość informująca o przycisku wyjścia z aplikacji
			sf::Text messageESCAPE("Press ESCAPE to quit", font);
			messageESCAPE.move(100.f, 450.f);
			window.draw(messageESCAPE);
		}
		else if(!queue.IsEmpty()) //Druga faza programu, w której zaczyna się praca na kolejce
		{
			//Pobieranie nowego elementu wraz z zatrzymaniem działania tej funkcji na czas TIME_DEQUEUE
			if (waitAutoDequeue <= 0) //Sprawdzanie czy kolejka nie jest pusta oraz czy czas "waitAutoDequeue" jest niewiększy niż 0
			{
				lastID	= queue.Dequeue(); //pobranie elementu z kolejki		
				waitAutoDequeue = TIME_DEQUEUE; //Zatrzymanie działania niektórych funkcji na czas TIME_DEQUEUE
			}
			else if (waitKeyPressed <= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) //Sprawdzenie czy wciśnięty jest przycisk ENTER oraz czy czas, po którym można ponownie wcisnąć przycisk minął 
			{
			    	lastID	= queue.Dequeue(); //pobranie elementu z kolejki		
				waitAutoDequeue = TIME_DEQUEUE; //Zatrzymanie działania niektórych funkcji na czas TIME_DEQUEUE
				waitKeyPressed = TIME_KEYPRESS; //Uśpienie programu na 0.2s, aby przytrzymanie przycisku ENTER nie wywołało za dużo razy tego samego
			}
			//Losowe dodawanie elementu do kolejki średnio co wartość TIME_ENQUEUE - TIME_SLEEP odpowiada za uśpienie programu, kiedy nie jest dodawany element, więc trzeba zwiększyć prawdopodobieństwo poprzez podzielenie przez wartość TIME_SLEEP
			if (rand()%(TIME_ENQUEUE/TIME_SLEEP) == 0) 
			{
				queue.Enqueue(id);
				id++;			//zwiększanie wartości
			}

			//***************DRAW***************//

			//Wiadomość o id aktualnej osoby
			sf::Text messageUserID("Current User ID: " + std::to_string(lastID), font);	//Tworzenie obiektu wiadomości	
			messageUserID.move(100.f, 200.f);						//Pozycja wiadomości w oknie						
			window.draw(messageUserID);							//Rysowanie wiadomości w oknie - musi zostać później wywołana metoda window.display()
			//Wiadomość o czasie obsługi aktualnej osby
			sf::Text messageQueueTime("Next user will be automatically got in " + std::to_string((waitAutoDequeue-waitAutoDequeue%1000)/1000) + "." + std::to_string((waitAutoDequeue%1000-waitAutoDequeue%100)/100) + "s", font);
			messageQueueTime.move(100.f, 250.f);
			window.draw(messageQueueTime);						
			//Wiadomość o aktualnej ilości osób w kolejce
			sf::Text messageQueueSize("Users in queue: " + std::to_string(queue.Size()), font);
			messageQueueSize.move(100.f, 300.f);
			window.draw(messageQueueSize);
			//Wiadomość informująca o przycisku ENTER pozwalającym szybciej pobrać element
			sf::Text messageENTER("Press ENTER/RETUN to get next user", font);
			messageENTER.move(100.f, 400.f);
			window.draw(messageENTER);
			//Wiadomość informująca o przycisku wyjścia z aplikacji
			sf::Text messageESCAPE("Press ESCAPE to quit", font);
			messageESCAPE.move(100.f, 450.f);
			window.draw(messageESCAPE);
		} 
		else //Trzecia faza programu - zakończenie pracy na kolejce
		{
			//Wiadomość o id ostatniej obsłużonej osoby
			sf::Text messageUserID("Last User ID: " + std::to_string(lastID), font);	//Tworzenie obiektu wiadomości	
			messageUserID.move(100.f, 200.f);						//Pozycja wiadomości w oknie						
			window.draw(messageUserID);							//Rysowanie wiadomości w oknie - musi zostać później wywołana metoda window.display()
			//Wiadomość o zakończeniu pracy
			sf::Text messageEmpty("Queue is empty - Work is over", font);	
			messageEmpty.move(100.f, 250.f);						
			window.draw(messageEmpty);	
			//Wiadomość informująca o przycisku wyjścia z aplikacji
			sf::Text messageESCAPE("Press ESCAPE to quit", font);
			messageESCAPE.move(100.f, 450.f);
			window.draw(messageESCAPE);
		}

		//************************************************************************//

		window.display();		//Wyświetlenie w oknie "window" wszystkich obiektów, które zostały narysowane metodą "draw"

		//************************************************************************//

		//Możliwość wyjścia z aplikacji
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) //Sprawdzenie czy wciśnięty jest przycisk ESCAPE
		window.close();	//Zamknięcie okna
    	}
}

