#include "TwixtConsole.h"
#include<utility>

int main()
{
	std::shared_ptr console = std::make_shared<TwixtConsole>();
	std::shared_ptr gameLogic = IGame::CreateGame();

	console->setGame(gameLogic);
	console->printBoard();
	console->printHelperMenu();
	int n;
	std::cin >> n;
	int x, y;
	int x2, y2, x1, y1;
	Position pos1, pos2;
	while (n != 9)
	{
		console->printBoard();
		console->printHelperMenu();
		switch (n)
		{
		case 0:
			std::cout << "Introduceti pozitia in care doriti sa introduceti piesa:\n";
			std::cin >> x >> y;
			pos1 = Position(x, y);
			console->PlacePiece(pos1);
			break;
		case 1:
			std::cout << "Introduceti poziitiile pieselor intre care doriti sa faceti pod:\n";
			std::cout << "Introduceti pozitiilei primei piese: ";
			std::cin >> x1 >> y1;
			pos1 = Position(x1, y1);
			std::cout << "\n";
			std::cout << "Introduceti pozitiilei piesei a2-a: ";
			std::cin >> x2 >> y2;
			pos2 = Position(x2, y2);
			console->PlaceBridge(pos1, pos2);
			break;
		case 2:
			std::cout << "Introduceti poziitiile pieselor intre care doriti sa stergeti pod-ul:\n";
			std::cout << "Introduceti pozitiilei primei piese: ";
			std::cin >> x1 >> y1;
			pos1 = Position(x1, y1);
			std::cout << "\n";
			std::cout << "Introduceti pozitiilei piesei a2-a: ";
			std::cin >> x2 >> y2;
			pos2 = Position(x2, y2);
			console->DeleteBridge(pos1, pos2);
			break;
		case 3:
			console->LoadFromFile();
			break;
		case 4:
			console->SaveToFile();
			break;
		case 5:
			console->SolicitDraw();
			break;
		case 6:
			console->EndTurn();
			console->printHelperMenu();
			std::cin >> n;
			break;
		case 9:
			std::cout << "Ati parasit jocul!!";
			break;
		}
		return 0;
	}
}