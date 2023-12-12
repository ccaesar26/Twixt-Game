#include "TwixtGUIQt.h"

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

void TwixtGUIQt::InitializeGameControlButtons(QGridLayout* mainGridLayout)
{
    const auto restartButton = new QPushButton{ "Restart" };
    const auto saveButton = new QPushButton{ "Save" };
    const auto loadButton = new QPushButton{ "Load" };
    const auto quitButton = new QPushButton{ "Quit" };

    const auto buttonContainer = new QWidget{};
    const auto buttonContainerLayout = new QGridLayout{};

    buttonContainerLayout->addWidget(restartButton, 0, 0);
    buttonContainerLayout->addWidget(saveButton, 0, 1);
    buttonContainerLayout->addWidget(loadButton, 0, 2);
    buttonContainerLayout->addWidget(quitButton, 0, 3);

    // TODO: connect signals and slots

    // TODO: setStyleSheet

    buttonContainer->setLayout(buttonContainerLayout);
    mainGridLayout->addWidget(buttonContainer, 3, 0, 1, 1);
}

void TwixtGUIQt::InitializeGameActionsButtons(QGridLayout* mainGridLayout)
{
    const auto placeButton = new QPushButton{ "Place Link" };
    const auto removeButton = new QPushButton{ "Remove Link" };
    const auto drawButton = new QPushButton{ "Request Draw" };
    const auto endTurnButton = new QPushButton{ "End Turn" };

    const auto buttonContainer = new QWidget{};
    const auto buttonContainerLayout = new QGridLayout{};

    buttonContainerLayout->addWidget(placeButton, 0, 0);
    buttonContainerLayout->addWidget(removeButton, 0, 1);
    buttonContainerLayout->addWidget(drawButton, 0, 2);
    buttonContainerLayout->addWidget(endTurnButton, 0, 3);

    // TODO: connect signals and slots

    // TODO: setStyleSheet

    buttonContainer->setLayout(buttonContainerLayout);
    mainGridLayout->addWidget(buttonContainer, 3, 2, 1, 1);
}

void TwixtGUIQt::InitializeCurrentPlayerLabel(QGridLayout* mainGridLayout)
{
    const auto currentPlayerLabel = new QLabel{};
    currentPlayerLabel->setText("Current player\nRed");

    // TODO: setAlignment, setStyleSheet

    mainGridLayout->addWidget(currentPlayerLabel, 0, 2, 1, 1);
}
