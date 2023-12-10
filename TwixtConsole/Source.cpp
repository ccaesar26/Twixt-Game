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
	while(n!= 9)
	{
	switch (n)
	{
	case 0:
		int x, y;
		std::cout << "Introduceti pozitia in care doriti sa introduceti piesa:\n";
		std::cin >> x >> y;
		Position pos(x,y);
		console->PlacePiece(pos);
		break;
	case 1:
		int x2, y2, x1, y1;
		std::cout << "Introduceti poziitiile pieselor intre care doriti sa faceti pod:\n";
		std::cout << "Introduceti pozitiilei primei piese: ";
		std::cin >> x1 >> y1;
		Position pos1(x1, y1);
		std::cout << "\n";
		std::cout << "Introduceti pozitiilei piesei a2-a: ";
		std::cin >> x2 >> y2;
		Position pos2(x2, y2);
		console->PlaceBridge(pos1,pos2);
		break;
	case 2:
		int x2, y2, x1, y1;
		std::cout << "Introduceti poziitiile pieselor intre care doriti sa stergeti pod-ul:\n";
		std::cout << "Introduceti pozitiilei primei piese: ";
		std::cin >> x1 >> y1;
		Position pos1(x1, y1);
		std::cout << "\n";
		std::cout << "Introduceti pozitiilei piesei a2-a: ";
		std::cin >> x2 >> y2;
		Position pos2(x2, y2);
		console->DeleteBridge(pos1, pos2);
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		console->EndTurn();
		console->printHelperMenu();
		std::cin >> n;
		break;
	}
	return 0;
}