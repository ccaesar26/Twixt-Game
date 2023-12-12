#include "TwixtGUIQt.h"

#include <QGridLayout>

TwixtGUIQt::TwixtGUIQt(QWidget *parent)
    : QMainWindow(parent)
{
    auto mainWidget = new QWidget{};
    auto mainGridLayout = new QGridLayout{};

    //TODO: initialize components of the main grid

    mainWidget->setLayout(mainGridLayout);
    this->setCentralWidget(mainWidget);
}

TwixtGUIQt::~TwixtGUIQt()
{
}

void TwixtGUIQt::OnPiecePlaced(const Position& pos)
{
}

void TwixtGUIQt::OnGameOver(const EGameResult& result)
{
}

void TwixtGUIQt::OnGameRestarted()
{
}
