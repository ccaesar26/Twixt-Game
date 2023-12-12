#include "TwixtGUIQt.h"

#include <QLabel>

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

void TwixtGUIQt::InitializeTitleLabel(QGridLayout* mainGridLayout)
{
    const auto titleLabel = new QLabel{};
    titleLabel->setText("Twixt");

    // TODO: setAlignment, setStyleSheet

    mainGridLayout->addWidget(titleLabel, 0, 0, 1, 1);
}