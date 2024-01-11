#include "TwixtGUIQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    const auto w = std::make_shared<TwixtGUIQt>();

    auto gameLogic = IGame::CreateGame();

    gameLogic->AddListener(w);

    w->SetGameLogic(std::move(gameLogic));

    w->show();

    w->MapCoordinates();

    return QApplication::exec();
}
