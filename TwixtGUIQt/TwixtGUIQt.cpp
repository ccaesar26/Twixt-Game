#include "TwixtGUIQt.h"

TwixtGUIQt::TwixtGUIQt(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

TwixtGUIQt::~TwixtGUIQt()
{}

void TwixtGUIQt::OnPiecePlaced(const Position& pos)
{
}

void TwixtGUIQt::OnGameOver(const EGameResult& result)
{
}

void TwixtGUIQt::OnGameRestarted()
{
}
