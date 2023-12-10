#include "TwixtConsole.h"
#include<utility>


int main()
{
	std::shared_ptr console = std::make_shared<TwixtConsole>();
	std::shared_ptr gameLogic = IGame::CreateGame();

	console->setGame(gameLogic);
	console->printHelperMenu();
	int n;
	std::cin >> n;
	switch (n)
	{
	case 0:
		int x, y;
		std::cout << "Introduceti pozitia in care doriti sa introduceti piesa:";
		std::cin >> x >> y;
		console->PlacePiece(x,y);
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	}
	return 0;
}