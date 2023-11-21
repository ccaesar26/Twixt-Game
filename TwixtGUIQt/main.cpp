#include "TwixtGUIQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::shared_ptr<TwixtGUIQt> w = std::make_shared<TwixtGUIQt>();

    std::shared_ptr<IGame> gameLogic = IGame::CreateGame();

    gameLogic->AddListener(w);

    w->show();

    return a.exec();
}
