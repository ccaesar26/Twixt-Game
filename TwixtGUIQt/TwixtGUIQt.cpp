#include "TwixtGUIQt.h"

#include <QPainter>
#include <QPen>
#include <QSizePolicy>

#include "GameException.h"

namespace
{
	QString ColorToString(const EColor color)
	{
		switch (color)
		{
		case EColor::Red:
			return "red";
		case EColor::Black:
			return "black";
		default:
			return "";
		}
	}
}

TwixtGUIQt::TwixtGUIQt(QWidget* parent)
	: QMainWindow(parent)
{
	const auto mainWidget = new QWidget{};
	m_mainGridLayout = QSharedPointer<QGridLayout>{new QGridLayout{}};

	InitializeTitleLabel();
	InitializeGameControlButtons();
	InitializeGameActionsButtons();
	InitializeCurrentPlayerLabel();
	InitializeHintLabel();
	InitializeBoard();

	mainWidget->setLayout(m_mainGridLayout.data());

	m_clickCount = 0;

	// Note: QMainWindow takes ownership of the widget pointer and deletes it at the appropriate time.
	// See https://doc.qt.io/qt-6/qmainwindow.html#setCentralWidget
	this->setCentralWidget(mainWidget);

	this->setFixedSize(mainWidget->sizeHint());
}

void TwixtGUIQt::SetGameLogic(std::shared_ptr<IGame>&& gameLogic)
{
	m_gameLogic = std::move(gameLogic);
}

void TwixtGUIQt::MapCoordinates()
{
	QPoint topLeft = m_boardContainer->pos();
	for (int i = 0; i < m_board.size(); ++i)
	{
		for (int j = 0; j < m_board.size(); ++j)
		{
			if (i == 0 && j == 0 || i == m_board.size() - 1 && j == 0
				|| i == 0 && j == m_board.size() - 1 || i == m_board.size() - 1 && j == m_board.size() - 1)
			{
				continue;
			}

			QPoint pos = m_board[i][j]->pos();

			pos.setX(pos.x() + topLeft.x() + 9);
			pos.setY(pos.y() + topLeft.y() + 9);

			m_board[i][j]->SetCenter(pos);
		}
	}
}

void TwixtGUIQt::OnEndTurnButtonClicked()
{
	if (m_gameLogic->IsGameOver())
	{
		return;
	}

	try
	{
		m_clickCount = 0;
		m_gameLogic->SwitchTurn();
	}
	catch (const GameException&)
	{
	}
}

void TwixtGUIQt::OnHoleButtonClicked(const Position& pos)
{
	if (m_gameLogic->IsGameOver())
	{
		return;
	}

	try
	{
		m_clickCount++;

		if (m_clickCount == 1)
		{
			m_gameLogic->PlacePiece(pos);
		}
		else if (m_clickCount == 2)
		{
			m_firstClick = pos;
		}
		else if (m_clickCount == 3)
		{
			m_secondClick = pos;

			m_gameLogic->CreateLink(m_firstClick, m_secondClick);

			m_clickCount = 0;
		}
	}
	catch (const GameException&)
	{
	}
	catch (...)
	{
		throw std::runtime_error("Unknown exception");
	}
}

void TwixtGUIQt::resizeEvent(QResizeEvent* event)
{
	MapCoordinates();
	QMainWindow::resizeEvent(event);
}

void TwixtGUIQt::paintEvent(QPaintEvent* event)
{
	QPainter painter{ this };
	painter.setPen(QPen(Qt::black, 8));

	for (const auto& line : m_links)
	{
		painter.drawLine(line);
	}
}

void TwixtGUIQt::OnPiecePlaced(const Position& pos)
{
	m_board[pos.row][pos.col]->SetPeg(m_gameLogic->GetCurrentPlayerColor());
}

void TwixtGUIQt::OnGameOver(const EGameResult& result)
{
}

void TwixtGUIQt::OnGameRestarted()
{
}

void TwixtGUIQt::OnLinkPlaced(const Position& pos1, const Position& pos2)
{
	const QPoint center1 = m_board[pos1.row][pos1.col]->GetCenter();
	const QPoint center2 = m_board[pos2.row][pos2.col]->GetCenter();

	m_links.emplaceBack(center1, center2);

	update();
}

void TwixtGUIQt::InitializeTitleLabel()
{
	m_titleLabel = QSharedPointer<QLabel>{new QLabel{}};
	m_titleLabel->setText("Twixt");

	// TODO: setAlignment, setStyleSheet

	m_mainGridLayout->addWidget(m_titleLabel.data(), 0, 0, 1, 1);
}

void TwixtGUIQt::InitializeCurrentPlayerLabel()
{
	m_currentPlayerLabel = QSharedPointer<QLabel>{new QLabel{}};
	m_currentPlayerLabel->setText("Current player\nRed");

	// TODO: setAlignment, setStyleSheet

	m_mainGridLayout->addWidget(m_currentPlayerLabel.data(), 0, 2, 1, 1);
}

void TwixtGUIQt::InitializeHintLabel()
{
	m_hintLabel = QSharedPointer<QLabel>{new QLabel{}};
	m_hintLabel->setText("");

	// TODO: setAlignment, setStyleSheet

	m_mainGridLayout->addWidget(m_hintLabel.data(), 2, 2, 1, 1);
}

void TwixtGUIQt::InitializeGameControlButtons()
{
	m_restartButton = QSharedPointer<QPushButton>{new QPushButton{"Restart"}};
	m_saveButton = QSharedPointer<QPushButton>{new QPushButton{"Save"}};
	m_loadButton = QSharedPointer<QPushButton>{new QPushButton{"Load"}};
	m_quitButton = QSharedPointer<QPushButton>{new QPushButton{"Quit"}};

	m_controlButtonsContainer = QSharedPointer<QWidget>{new QWidget{}};
	m_controlButtonsContainerLayout = QSharedPointer<QGridLayout>{new QGridLayout{}};

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
	m_placeBridgeButton = QSharedPointer<QPushButton>{new QPushButton{"Place bridge"}};
	m_removeBridgeButton = QSharedPointer<QPushButton>{new QPushButton{"Remove bridge"}};
	m_requestDrawButton = QSharedPointer<QPushButton>{new QPushButton{"Request draw"}};
	m_endTurnButton = QSharedPointer<QPushButton>{new QPushButton{"End turn"}};

	m_actionsButtonsContainer = QSharedPointer<QWidget>{new QWidget{}};
	m_actionsButtonsContainerLayout = QSharedPointer<QGridLayout>{new QGridLayout{}};

	m_actionsButtonsContainerLayout->addWidget(m_placeBridgeButton.data(), 0, 0);
	m_actionsButtonsContainerLayout->addWidget(m_removeBridgeButton.data(), 1, 0);
	m_actionsButtonsContainerLayout->addWidget(m_requestDrawButton.data(), 2, 0);
	m_actionsButtonsContainerLayout->addWidget(m_endTurnButton.data(), 3, 0);

	// TODO: connect signals and slots
	connect(m_endTurnButton.data(), &QPushButton::clicked, this, &TwixtGUIQt::OnEndTurnButtonClicked);

	// TODO: setStyleSheet

	m_actionsButtonsContainer->setLayout(m_actionsButtonsContainerLayout.data());
	m_mainGridLayout->addWidget(m_actionsButtonsContainer.data(), 3, 2, 1, 1);
}

void TwixtGUIQt::InitializeBoard()
{
	m_boardContainer = QSharedPointer<QWidget>{new QWidget{}};
	m_boardContainerLayout = QSharedPointer<QGridLayout>{new QGridLayout{}};

	m_board.resize(24);

	const auto size = m_board.size();
	auto isCorner = [size](const int i, const int j) -> bool
	{
		return (i == 0 && j == 0) || (i == 0 && j == size - 1) || (i == size - 1 && j == 0) || (i == size - 1 && j ==
			size - 1);
	};

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
				m_board[i][j] = QSharedPointer<HoleButton>::create(Position{i, j});
				m_boardContainerLayout->addWidget(m_board[i][j].data(), i, j);
				m_board[i][j]->UpdatePeg();
			}
			connect(m_board[i][j].data(), &HoleButton::Clicked, this, &TwixtGUIQt::OnHoleButtonClicked);
		}
	}

	//m_boardContainer->setFixedSize(760, 760);

	m_boardContainer->setLayout(m_boardContainerLayout.data());
	m_mainGridLayout->addWidget(m_boardContainer.data(), 0, 1, 4, 1);
}

void TwixtGUIQt::UpdateCurrentPlayerLabel()
{
	m_currentPlayerLabel->setText("Current player\n" + ColorToString(m_gameLogic->GetCurrentPlayerColor()));
}
