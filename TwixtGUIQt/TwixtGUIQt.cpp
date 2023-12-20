#include "TwixtGUIQt.h"

TwixtGUIQt::TwixtGUIQt(QWidget *parent)
    : QMainWindow(parent)
{
    const auto mainWidget = new QWidget{};
    m_mainGridLayout = QSharedPointer<QGridLayout>{ new QGridLayout{} };

    //TODO: initialize components of the main grid

    mainWidget->setLayout(m_mainGridLayout.data());

    // Note: QMainWindow takes ownership of the widget pointer and deletes it at the appropriate time.
    // See https://doc.qt.io/qt-6/qmainwindow.html#setCentralWidget
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

void TwixtGUIQt::InitializeTitleLabel()
{
    m_titleLabel = QSharedPointer<QLabel>{ new QLabel{} };
    m_titleLabel->setText("Twixt");

    // TODO: setAlignment, setStyleSheet

    m_mainGridLayout->addWidget(m_titleLabel.data(), 0, 0, 1, 1);
}

void TwixtGUIQt::InitializeCurrentPlayerLabel()
{
    m_currentPlayerLabel = QSharedPointer<QLabel>{ new QLabel{} };
    m_currentPlayerLabel->setText("Current player\nRed");

    // TODO: setAlignment, setStyleSheet

    m_mainGridLayout->addWidget(m_currentPlayerLabel.data(), 0, 2, 1, 1);
}

void TwixtGUIQt::InitializeHintLabel()
{
    m_hintLabel = QSharedPointer<QLabel>{ new QLabel{} };
    m_hintLabel->setText("");

    // TODO: setAlignment, setStyleSheet

    m_mainGridLayout->addWidget(m_hintLabel.data(), 2, 2, 1, 1);
}

void TwixtGUIQt::InitializeGameControlButtons()
{
    m_restartButton = QSharedPointer<QPushButton>{ new QPushButton{"Restart"} };
    m_saveButton = QSharedPointer<QPushButton>{ new QPushButton{"Save"} };
    m_loadButton = QSharedPointer<QPushButton>{ new QPushButton{"Load"} };
    m_quitButton = QSharedPointer<QPushButton>{ new QPushButton{"Quit"} };

    m_controlButtonsContainer = QSharedPointer<QWidget>{ new QWidget{} };
    m_controlButtonsContainerLayout = QSharedPointer<QGridLayout>{ new QGridLayout{} };

    m_controlButtonsContainerLayout->addWidget(m_restartButton.data(), 0, 0);
    m_controlButtonsContainerLayout->addWidget(m_saveButton.data(), 1, 0);
    m_controlButtonsContainerLayout->addWidget(m_loadButton.data(), 2, 0);
    m_controlButtonsContainerLayout->addWidget(m_quitButton.data(), 3, 0);

    // TODO: connect signals and slots

    // TODO: setStyleSheet

    m_controlButtonsContainer->setLayout(m_controlButtonsContainerLayout.data());
    m_mainGridLayout->addWidget(m_controlButtonsContainer.data(), 3, 0, 1, 1);
}

void TwixtGUIQt::InitializeGameActionsButtons()
{
    m_placeBridgeButton = QSharedPointer<QPushButton>{ new QPushButton{"Place bridge"} };
    m_removeBridgeButton = QSharedPointer<QPushButton>{ new QPushButton{"Remove bridge"} };
    m_requestDrawButton = QSharedPointer<QPushButton>{ new QPushButton{"Request draw"} };
    m_endTurnButton = QSharedPointer<QPushButton>{ new QPushButton{"End turn"} };

    m_actionsButtonsContainer = QSharedPointer<QWidget>{ new QWidget{} };
    m_actionsButtonsContainerLayout = QSharedPointer<QGridLayout>{ new QGridLayout{} };

    m_actionsButtonsContainerLayout->addWidget(m_placeBridgeButton.data(), 0, 0);
    m_actionsButtonsContainerLayout->addWidget(m_removeBridgeButton.data(), 1, 0);
    m_actionsButtonsContainerLayout->addWidget(m_requestDrawButton.data(), 2, 0);
    m_actionsButtonsContainerLayout->addWidget(m_endTurnButton.data(), 3, 0);

    // TODO: connect signals and slots

    // TODO: setStyleSheet

    m_actionsButtonsContainer->setLayout(m_actionsButtonsContainerLayout.data());
    m_mainGridLayout->addWidget(m_actionsButtonsContainer.data(), 3, 2, 1, 1);
}
