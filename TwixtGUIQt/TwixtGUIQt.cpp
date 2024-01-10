#include "TwixtGUIQt.h"

namespace
{
    bool isCorner(int i, int j)
    {
        return (i == 0 && j == 0) || (i == 0 && j == 23) || (i == 23 && j == 0) || (i == 23 && j == 23);
    }
}

TwixtGUIQt::TwixtGUIQt(QWidget *parent)
    : QMainWindow(parent)
{
    const auto mainWidget = new QWidget{};
    m_mainGridLayout = QSharedPointer<QGridLayout>{ new QGridLayout{} };

    InitializeTitleLabel();
    InitializeGameControlButtons();
    InitializeGameActionsButtons();
    InitializeCurrentPlayerLabel();
    InitializeHintLabel();
    InitializeBoard();

    mainWidget->setLayout(m_mainGridLayout.data());

    // Note: QMainWindow takes ownership of the widget pointer and deletes it at the appropriate time.
    // See https://doc.qt.io/qt-6/qmainwindow.html#setCentralWidget
    this->setCentralWidget(mainWidget);
}

void TwixtGUIQt::SetGameLogic(std::shared_ptr<IGame>&& gameLogic)
{
	m_gameLogic = std::move(gameLogic);
}

void TwixtGUIQt::MapCoordinates()
{
    for (int i = 0; i < m_board.size(); ++i) 
    {
        for (int j = 0; j < m_board.size(); ++j)
        {
            if (i == 0 && j == 0 || i == m_board.size() - 1 && j == 0 
                || i == 0 && j == m_board.size() - 1 || i == m_board.size() - 1 && j == m_board.size() - 1)
			{
				continue;
			}

            QPoint centerLocal = m_board[i][j]->rect().center();
            QPoint centerGlobal = m_board[i][j]->mapToGlobal(centerLocal);

            m_board[i][j]->SetCenter(centerGlobal);
        }
    }
}

void TwixtGUIQt::resizeEvent(QResizeEvent* event)
{
    MapCoordinates();
	QMainWindow::resizeEvent(event);
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

void TwixtGUIQt::OnLinkPlaced(const Position& pos1, const Position& pos2)
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

void TwixtGUIQt::InitializeBoard()
{
    m_boardContainer = QSharedPointer<QWidget>{ new QWidget{} };
    m_boardContainerLayout = QSharedPointer<QGridLayout>{ new QGridLayout{} };

    m_board.resize(24);

    for (int i = 0; i < 24; ++i)
    {
        m_board[i].resize(24);

        for (int j = 0; j < 24; ++j)
        {
            if (isCorner(i, j))
            {
                m_board[i][j] = nullptr;
            }
            else
            {
                m_board[i][j] = new HoleButton{ Position{i, j} };
                m_boardContainerLayout->addWidget(m_board[i][j], i, j);
                m_board[i][j]->UpdatePeg();
            }
        }
    }

    m_boardContainer->setLayout(m_boardContainerLayout.data());
    m_mainGridLayout->addWidget(m_boardContainer.data(), 0, 1, 4, 1);
}
