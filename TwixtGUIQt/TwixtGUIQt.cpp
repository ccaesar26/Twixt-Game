#include "TwixtGUIQt.h"

#include <QPainter>
#include <QPen>
#include <QSizePolicy>
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>
#include <QFontDatabase>

#include <algorithm>
#include <ranges>

#include "ConfigurationDialog.h"
#include "GameException.h"
#include "ConverterLibrary.h"

namespace
{
	template <typename T>
	std::string ConcatenateStrings(const T& str)
	{
		return str;
	}

	template <typename T, typename... Args>
	std::string ConcatenateStrings(const T& first, const Args&... rest)
	{
		return std::string(first) + ConcatenateStrings(rest...);
	}
}

TwixtGUIQt::TwixtGUIQt(QWidget* parent)
	: QMainWindow{parent}, m_clickCount{0}, m_isFirstTurn{true}
{
	const auto mainWidget = new QWidget{};
	m_mainGridLayout = QSharedPointer<QGridLayout>{new QGridLayout{}};

	InitializeUI();

	mainWidget->setLayout(m_mainGridLayout.data());

	// Note: QMainWindow takes ownership of the widget pointer and deletes it at the appropriate time.
	// See https://doc.qt.io/qt-6/qmainwindow.html#setCentralWidget
	this->setCentralWidget(mainWidget);

	this->setFixedSize(mainWidget->sizeHint());

	SetFont();
}

void TwixtGUIQt::SetGameLogic(std::shared_ptr<IGame>&& gameLogic)
{
	m_gameLogic = std::move(gameLogic);
}

void TwixtGUIQt::MapCoordinates()
{
	const QPoint topLeft = m_boardContainer->pos();
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

void TwixtGUIQt::OnRestartButtonClicked()
{
	m_gameLogic->Reset();
}

void TwixtGUIQt::OnSaveButtonClicked()
{
	if (m_gameLogic->IsGameOver())
	{
		return;
	}

	const QString fileName = QFileDialog::getSaveFileName(
		this,
		"Save game",
		QDir::homePath(),
		tr("Text File (*.txt);;All files (*.*)")
	);

	try
	{
		m_gameLogic->SaveToFile(fileName.toStdString());
	}
	catch (const GameException&)
	{
	}
	catch (...)
	{
		//throw std::runtime_error("Unknown exception");
	}
}

void TwixtGUIQt::OnLoadButtonClicked()
{
	const QString fileName = QFileDialog::getOpenFileName(
		this,
		"Load game",
		QDir::homePath(),
		tr("Text File (*.txt);;All files (*.*)")
	);

	try
	{
		m_gameLogic->LoadFromFile(fileName.toStdString());
	}
	catch (const GameException& e)
	{
		qDebug() << e.what();
	}
	catch (...)
	{
		throw std::runtime_error("Unknown exception");
	}
}

void TwixtGUIQt::OnGetHintButtonClicked()
{
	if (m_gameLogic->IsGameOver())
	{
		return;
	}

	m_gameLogic->Recommend();
}

void TwixtGUIQt::OnRequestDrawButtonClicked()
{
	if (m_gameLogic->IsGameOver())
	{
		return;
	}

	if (const QMessageBox::StandardButton reply = QMessageBox::question(
		this,
		"Draw Request",
		"Do you both want a draw?",
		QMessageBox::Yes | QMessageBox::No
	); reply == QMessageBox::Yes)
	{
		m_gameLogic->EndInDraw();
	}
}

void TwixtGUIQt::OnEndTurnButtonClicked()
{
	if (m_gameLogic->IsGameOver())
	{
		return;
	}

	m_endTurnButton->setEnabled(false);
	m_getHintButton->setEnabled(true);

	try
	{
		if (m_isFirstTurn)
		{
			m_isFirstTurn = false;

			QMessageBox::StandardButton reply = QMessageBox::question(
				this,
				"Swap Colors",
				"You're playing as Black.\nDo you want to swap colors and be red?",
				QMessageBox::Yes | QMessageBox::No
			);
		}

		m_gameLogic->SwitchTurn();

		m_clickCount = 0;

		UpdateCurrentPlayerLabel();
	}
	catch (const GameException&)
	{
	}

	ClearHint();
}

void TwixtGUIQt::OnChangeConfigurationButtonClicked()
{
	if (m_gameLogic->IsGameOver())
	{
		return;
	}

	ConfigurationDialog dialog{this};

	if (dialog.exec() == QDialog::Accepted)
	{
		const auto boardSize = dialog.GetBoardSize();
		const auto maxPegs = dialog.GetMaxPegs();
		const auto maxLinks = dialog.GetMaxLinks();

		m_gameLogic->ReconfigureGame(boardSize, maxPegs, maxLinks);
	}
}

void TwixtGUIQt::OnHoleButtonClicked(const Position& pos)
{
	if (m_gameLogic->IsGameOver())
	{
		return;
	}

	enum class Action
	{
		None,
		PlacePeg,
		PlaceLink,
	};

	auto action = Action::None;

	UpdateErrorLabel("");

	if (m_clickCount > 2)
	{
		m_clickCount = 1;
	}

	try
	{
		if (m_clickCount == 0)
		{
			action = Action::PlacePeg;
			m_gameLogic->PlacePiece(pos);
		}
		else if (m_clickCount == 1)
		{
			m_firstClick = pos;
		}
		else if (m_clickCount == 2)
		{
			m_secondClick = pos;
			action = Action::PlaceLink;
			m_gameLogic->CreateLink(m_firstClick, m_secondClick);
		}

		m_clickCount++;
	}
	catch (const GameException& e)
	{
		UpdateErrorLabel(e.what());
		switch (action)
		{
		case Action::PlacePeg:
			m_clickCount = 0;
			break;
		case Action::PlaceLink:
			m_clickCount = 1;
			break;
		default:
			break;
		}
	}
	catch (...)
	{
		throw std::runtime_error("Unknown exception");
	}
}


void TwixtGUIQt::OnHoleButtonRightClicked(const Position& pos)
{
	if (m_gameLogic->IsGameOver())
	{
		return;
	}

	if (m_clickCount > 2)
	{
		m_clickCount = 1;
	}

	try
	{
		if (m_clickCount == 0)
		{
			throw GameException("You must place a peg first");
		}
		if (m_clickCount == 1)
		{
			m_firstClick = pos;
		}
		else if (m_clickCount == 2)
		{
			m_secondClick = pos;

			m_gameLogic->RemoveLink(m_firstClick, m_secondClick);

			m_clickCount = 1;
		}

		m_clickCount++;
	}
	catch (const GameException& e)
	{
		if (m_clickCount == 0)
		{
			UpdateErrorLabel(e.what());
		}
		else
		{
			UpdateErrorLabel("");
			m_clickCount = 1;
		}
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
	QPainter painter{this};

	auto colorConverter = [](const EColor color) -> QColor
	{
		switch (color)
		{
		case EColor::Red:
			return Qt::red;
		case EColor::Black:
			return Qt::black;
		default:
			throw std::runtime_error("Unknown color");
		}
	};

	MapCoordinates();

	const auto& board = m_board;
	auto getLines = [board]() -> QVector<QPair<QLine, QColor>>
	{
		QVector<QPair<QLine, QColor>> lines{};

		const auto& size = board.size();

		const auto upperLeftRed = (board[0][1]->GetCenter() + board[1][1]->GetCenter()) / 2;
		const auto upperRightRed = (board[0][size - 2]->GetCenter() + board[1][size - 2]->GetCenter()) / 2;

		const auto lowerLeftRed = (board[size - 2][1]->GetCenter() + board[size - 1][1]->GetCenter()) / 2;
		const auto lowerRightRed = (board[size - 2][size - 2]->GetCenter() + board[size - 1][size - 2]->GetCenter()) /
			2;

		lines.emplaceBack(QLine{upperLeftRed, upperRightRed}, Qt::red);
		lines.emplaceBack(QLine{lowerLeftRed, lowerRightRed}, Qt::red);

		const auto upperLeftBlack = (board[1][0]->GetCenter() + board[1][1]->GetCenter()) / 2;
		const auto upperRightBlack = (board[1][size - 2]->GetCenter() + board[1][size - 1]->GetCenter()) / 2;

		const auto lowerLeftBlack = (board[size - 2][0]->GetCenter() + board[size - 2][1]->GetCenter()) / 2;
		const auto lowerRightBlack = (board[size - 2][size - 2]->GetCenter() + board[size - 2][size - 1]->GetCenter()) /
			2;

		lines.emplaceBack(QLine{upperLeftBlack, lowerLeftBlack}, Qt::black);
		lines.emplaceBack(QLine{upperRightBlack, lowerRightBlack}, Qt::black);

		return lines;
	};

	for (const auto& position : m_hint)
	{
		const auto center = m_board[position.row][position.col]->GetCenter();
		painter.setPen(QPen{Qt::cyan, 12});
		painter.drawEllipse(center, 6, 6);
	}

	for (const auto& [line, color] : getLines())
	{
		painter.setPen(QPen{color, 2});
		painter.drawLine(line);
	}

	for (const auto& [line, color] : m_links)
	{
		painter.setPen(QPen{colorConverter(color), 8});
		painter.drawLine(line);
	}
}

void TwixtGUIQt::OnBoardChanged(int newSize, int newMaxPegs, int newMaxLinks)
{
	m_boardContainer->setFixedSize(m_boardSize);
	m_board.resize(newSize);

	const auto size = m_board.size();
	auto isCorner = [size](const int i, const int j) -> bool
		{
			return (i == 0 && j == 0) || (i == 0 && j == size - 1) || (i == size - 1 && j == 0) || (i == size - 1 && j ==
				size - 1);
		};

	for (int i = 0; i < m_board.size(); ++i)
	{
		m_board[i].resize(newSize);

		for (int j = 0; j < m_board[i].size(); ++j)
		{
			if (isCorner(i, j))
			{
				m_board[i][j] = nullptr;
			}
			else
			{
				m_board[i][j] = QSharedPointer<HoleButton>::create(Position{ i, j });
				m_boardContainerLayout->addWidget(m_board[i][j].data(), i, j);

				if (m_gameLogic != nullptr && m_gameLogic->GetPiecePtr(Position{ i, j }) != nullptr)
				{
					m_board[i][j]->SetPeg(m_gameLogic->GetPiecePtr(Position{ i, j })->GetColor());
				}

				m_board[i][j]->UpdatePeg();
			}
			connect(m_board[i][j].data(), &HoleButton::Clicked, this, &TwixtGUIQt::OnHoleButtonClicked);
			connect(m_board[i][j].data(), &HoleButton::RightClicked, this, &TwixtGUIQt::OnHoleButtonRightClicked);
		}
	}

	update();

	MapCoordinates();

	update();
}

void TwixtGUIQt::OnPiecePlaced(const Position& pos)
{
	m_board[pos.row][pos.col]->SetPeg(m_gameLogic->GetCurrentPlayerColor());

	UpdateStats();

	m_endTurnButton->setEnabled(true);
	m_getHintButton->setEnabled(false);

	if (!m_hint.empty())
	{
		m_hint.pop_back();
	}

	update();
}

void TwixtGUIQt::OnGameOver(const EGameResult& result)
{
	const QString gameResult = GameResultToString(static_cast<int>(result));
	if (const QMessageBox::StandardButton reply = QMessageBox::question(
		this,
		"Game Over",
		gameResult + ".\nDo you want to play again?",
		QMessageBox::Yes | QMessageBox::No
	); reply == QMessageBox::Yes)
	{
		m_gameLogic->Reset();
	}
	else
	{
		qApp->quit();
	}
}

void TwixtGUIQt::OnGameRestarted()
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

			if (m_gameLogic->GetPiecePtr(Position{i, j}) != nullptr)
			{
				m_board[i][j]->SetPeg(m_gameLogic->GetPiecePtr(Position{i, j})->GetColor());
			}
			else
			{
				m_board[i][j]->ResetPeg();
			}
		}
	}

	m_links.clear();

	update();

	m_clickCount = 0;
	m_isFirstTurn = true;

	UpdateCurrentPlayerLabel();
	UpdateStats();
	UpdateHintLabel("");
	UpdateErrorLabel("");
}

void TwixtGUIQt::OnGameLoaded()
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

			if (m_gameLogic->GetPiecePtr(Position{i, j}) != nullptr)
			{
				m_board[i][j]->SetPeg(m_gameLogic->GetPiecePtr(Position{i, j})->GetColor());
				m_isFirstTurn = false;
			}
			else
			{
				m_board[i][j]->ResetPeg();
			}
		}
	}

	m_clickCount = 1;

	m_endTurnButton->setEnabled(true);

	UpdateCurrentPlayerLabel();
}

void TwixtGUIQt::OnLinkPlaced(const Position& pos1, const Position& pos2)
{
	const QPoint center1 = m_board[pos1.row][pos1.col]->GetCenter();
	const QPoint center2 = m_board[pos2.row][pos2.col]->GetCenter();

	m_links.emplaceBack(QLine{center1, center2}, m_gameLogic->GetCurrentPlayerColor());

	ClearHint();

	UpdateStats();

	update();
}

void TwixtGUIQt::OnLinkRemoved(const Position& pos1, const Position& pos2)
{
	const QPoint center1 = m_board[pos1.row][pos1.col]->GetCenter();
	const QPoint center2 = m_board[pos2.row][pos2.col]->GetCenter();

	auto predicate = [center1, center2](const auto& link)
	{
		const QLine& line = link.first;
		return (line.p1() == center1 && line.p2() == center2) ||
			(line.p1() == center2 && line.p2() == center1);
	};

	const auto it = std::remove_if(m_links.begin(), m_links.end(), predicate);
	m_links.erase(it, m_links.end());

	UpdateStats();

	update();
}

void TwixtGUIQt::OnDrawRequested(EColor current_player)
{
	if (const QMessageBox::StandardButton reply = QMessageBox::question(
		this,
		"Draw Request",
		QString::fromStdString(ConcatenateStrings(
			"Player ", QString(ColorToString(static_cast<int>(current_player), true)).toStdString(),
			" requested a draw.\nDo you accept?")),
		QMessageBox::Yes | QMessageBox::No
	); reply == QMessageBox::Yes)
	{
		m_gameLogic->EndInDraw();
	}
}

void TwixtGUIQt::OnHintRecommended(std::pair<Position, Position> link)
{
	m_hint.clear();

	if (m_gameLogic->IsGameOver())
	{
		return;
	}

	if (link.first == link.second)
	{
		return;
	}

	m_hint.push_back(link.first);
	m_hint.push_back(link.second);

	const auto& newPegPosition = link.second;
	const auto& linkPegPosition = link.first;

	const auto hintString = ConcatenateStrings(
		"Hint:\n",
		"Place a peg at position ",
		"(", std::to_string(newPegPosition.row), ", ", std::to_string(newPegPosition.col), ")",
		" and link it to the peg from position ",
		"(", std::to_string(linkPegPosition.row), ", ", std::to_string(linkPegPosition.col), ")",
		"."
	);
	UpdateHintLabel(QString::fromStdString(hintString));

	update();
}

void TwixtGUIQt::InitializeUI()
{
	SetStyle(this, "stylesheets/MainWindow.css");

	InitializeTitleLabel();
	InitializeGameControlButtons();
	InitializeGameActionsButtons();
	InitializeCurrentPlayerLabel();
	InitializeErrorLabel();
	InitializeHintLabel();
	InitializeBoard();
	InitializeStats();
}

void TwixtGUIQt::InitializeTitleLabel()
{
	m_titleLabel = QSharedPointer<QLabel>{new QLabel{}};
	m_titleLabel->setText("Twixt");

	m_titleLabel->setObjectName("titleLabel");
	SetStyle(m_titleLabel.data(), "stylesheets/TitleLabel.css");

	m_mainGridLayout->addWidget(m_titleLabel.data(), 0, 0, 1, 1);
}

void TwixtGUIQt::InitializeCurrentPlayerLabel()
{
	m_currentPlayerLabel = QSharedPointer<QLabel>{new QLabel{}};
	m_currentPlayerLabel->setText("Current player\nRed");

	m_currentPlayerLabel->setObjectName("currentPlayerLabel");
	SetStyle(m_currentPlayerLabel.data(), "stylesheets/Label.css");

	m_mainGridLayout->addWidget(m_currentPlayerLabel.data(), 0, 2, 1, 1);
}

void TwixtGUIQt::InitializeErrorLabel()
{
	m_errorLabel = QSharedPointer<QLabel>{new QLabel{}};
	m_errorLabel->setText("");

	m_errorLabel->setObjectName("errorLabel");
	SetStyle(m_errorLabel.data(), "stylesheets/Label.css");

	m_mainGridLayout->addWidget(m_errorLabel.data(), 1, 2, 1, 1);
}

void TwixtGUIQt::InitializeHintLabel()
{
	m_hintLabel = QSharedPointer<QLabel>{new QLabel{}};
	m_hintLabel->setText("");

	// TODO: setAlignment, setStyleSheet
	m_hintLabel->setObjectName("hintLabel");
	SetStyle(m_hintLabel.data(), "stylesheets/Label.css");

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

	connect(m_restartButton.data(), &QPushButton::clicked, this, &TwixtGUIQt::OnRestartButtonClicked);
	connect(m_saveButton.data(), &QPushButton::clicked, this, &TwixtGUIQt::OnSaveButtonClicked);
	connect(m_loadButton.data(), &QPushButton::clicked, this, &TwixtGUIQt::OnLoadButtonClicked);
	connect(m_quitButton.data(), &QPushButton::clicked, qApp, &QApplication::quit);

	SetStyle(m_controlButtonsContainer.data(), "stylesheets/Button.css");

	m_controlButtonsContainer->setLayout(m_controlButtonsContainerLayout.data());
	m_mainGridLayout->addWidget(m_controlButtonsContainer.data(), 3, 0, 2, 1);
}

void TwixtGUIQt::InitializeGameActionsButtons()
{
	m_getHintButton = QSharedPointer<QPushButton>{new QPushButton{"Get Hint"}};
	m_requestDrawButton = QSharedPointer<QPushButton>{new QPushButton{"Request draw"}};
	m_changeConfiguration = QSharedPointer<QPushButton>{new QPushButton{"Change configuration"}};
	m_endTurnButton = QSharedPointer<QPushButton>{new QPushButton{"End turn"}};

	m_actionsButtonsContainer = QSharedPointer<QWidget>{new QWidget{}};
	m_actionsButtonsContainerLayout = QSharedPointer<QGridLayout>{new QGridLayout{}};

	m_actionsButtonsContainerLayout->addWidget(m_getHintButton.data(), 1, 0);
	m_actionsButtonsContainerLayout->addWidget(m_requestDrawButton.data(), 2, 0);
	m_actionsButtonsContainerLayout->addWidget(m_changeConfiguration.data(), 3, 0);
	m_actionsButtonsContainerLayout->addWidget(m_endTurnButton.data(), 4, 0);

	// TODO: connect signals and slots
	connect(m_getHintButton.data(), &QPushButton::clicked, this, &TwixtGUIQt::OnGetHintButtonClicked);
	connect(m_requestDrawButton.data(), &QPushButton::clicked, this, &TwixtGUIQt::OnRequestDrawButtonClicked);
	connect(m_changeConfiguration.data(), &QPushButton::clicked, this, &TwixtGUIQt::OnChangeConfigurationButtonClicked);
	connect(m_endTurnButton.data(), &QPushButton::clicked, this, &TwixtGUIQt::OnEndTurnButtonClicked);

	m_endTurnButton->setEnabled(false);

	SetStyle(m_actionsButtonsContainer.data(), "stylesheets/Button.css");

	m_actionsButtonsContainer->setLayout(m_actionsButtonsContainerLayout.data());
	m_mainGridLayout->addWidget(m_actionsButtonsContainer.data(), 3, 2, 2, 1);
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

				if (m_gameLogic != nullptr && m_gameLogic->GetPiecePtr(Position{i, j}) != nullptr)
				{
					m_board[i][j]->SetPeg(m_gameLogic->GetPiecePtr(Position{i, j})->GetColor());
				}

				m_board[i][j]->UpdatePeg();
			}
			connect(m_board[i][j].data(), &HoleButton::Clicked, this, &TwixtGUIQt::OnHoleButtonClicked);
			connect(m_board[i][j].data(), &HoleButton::RightClicked, this, &TwixtGUIQt::OnHoleButtonRightClicked);
		}
	}

	m_boardContainer->setLayout(m_boardContainerLayout.data());
	m_mainGridLayout->addWidget(m_boardContainer.data(), 0, 1, 4, 1);

	m_boardSize = m_boardContainer->sizeHint();
}

void TwixtGUIQt::InitializeStats()
{
	m_statsContainer = QSharedPointer<QWidget>{new QWidget{}};
	m_statsContainerLayout = QSharedPointer<QGridLayout>{new QGridLayout{}};

	m_statsLabel = QSharedPointer<QLabel>{new QLabel{}};
	m_statsLabel->setText(QString::fromStdString(ConcatenateStrings(
		"Red pegs:\t", "50 / 50\t\t\t",
		"Black pegs:\t", "50 / 50\n",
		"Red links:\t", "50 / 50\t\t\t",
		"Black links:\t", "50 / 50"
	)));

	m_statsContainerLayout->addWidget(m_statsLabel.data(), 0, 0);

	m_statsLabel->setObjectName("statsLabel");
	SetStyle(m_statsContainer.data(), "stylesheets/Label.css");

	m_statsContainer->setLayout(m_statsContainerLayout.data());
	m_mainGridLayout->addWidget(m_statsContainer.data(), 4, 1, 1, 1);
}

void TwixtGUIQt::UpdateCurrentPlayerLabel()
{
	m_currentPlayerLabel->setText(
		"Current player\n" + QString(ColorToString(static_cast<int>(m_gameLogic->GetCurrentPlayerColor()), true)));
}

void TwixtGUIQt::UpdateErrorLabel(QString error)
{
	m_errorLabel->setText(error);
}

void TwixtGUIQt::UpdateHintLabel(QString hint)
{
	m_hintLabel->setText(hint);
}

void TwixtGUIQt::UpdateStats()
{
	const auto& redPegsCount = m_gameLogic->GetAvailablePegsNumber(EColor::Red);
	const auto& blackPegsCount = m_gameLogic->GetAvailablePegsNumber(EColor::Black);

	const auto& redLinksCount = m_gameLogic->GetAvailableLinksNumber(EColor::Red);
	const auto& blackLinksCount = m_gameLogic->GetAvailableLinksNumber(EColor::Black);

	const auto& redPegsLimit = m_gameLogic->GetPegsLimitNumber(EColor::Red);
	const auto& blackPegsLimit = m_gameLogic->GetPegsLimitNumber(EColor::Black);

	const auto& redLinksLimit = m_gameLogic->GetLinksLimitNumber(EColor::Red);
	const auto& blackLinksLimit = m_gameLogic->GetLinksLimitNumber(EColor::Black);

	const auto pegsString = ConcatenateStrings(
		"Red pegs:\t", std::to_string(redPegsCount), " / ", std::to_string(redPegsLimit), "\t\t\t",
		"Black pegs:\t", std::to_string(blackPegsCount), " / ", std::to_string(blackPegsLimit)
	);

	const auto linksString = ConcatenateStrings(
		"Red links:\t", std::to_string(redLinksCount), " / ", std::to_string(redLinksLimit), "\t\t\t",
		"Black links:\t", std::to_string(blackLinksCount), " / ", std::to_string(blackLinksLimit)
	);

	m_statsLabel->setText(QString::fromStdString(ConcatenateStrings(
		pegsString,
		"\n",
		linksString
	)));
}

void TwixtGUIQt::UpdateBoard()
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

			if (m_gameLogic->GetPiecePtr(Position{i, j}) != nullptr)
			{
				m_board[i][j]->SetPeg(m_gameLogic->GetPiecePtr(Position{i, j})->GetColor());
			}
			else
			{
				m_board[i][j]->ResetPeg();
			}
		}
	}

	m_links.clear();

	update();
}

void TwixtGUIQt::ClearHint()
{
	m_hint.clear();
	UpdateHintLabel("");
	update();
}

void TwixtGUIQt::SetFont()
{
	const int id = QFontDatabase::addApplicationFont("fonts/Poppins-Regular.ttf");
	QFontDatabase::addApplicationFont("fonts/Poppins-Bold.ttf");
	const QString family = QFontDatabase::applicationFontFamilies(id).at(0);
	const QFont font{family};
	this->setFont(font);
}

void TwixtGUIQt::SetStyle(QWidget* widget, const QString& styleSheetPath)
{
	QFile styleFile{styleSheetPath};
	styleFile.open(QFile::ReadOnly);
	const QString styleSheet = QLatin1String(styleFile.readAll());
	widget->setStyleSheet(styleSheet);
}
