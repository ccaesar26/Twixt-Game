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

#include "GameException.h"
#include "ConverterLibrary.h"

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

	const auto [pos, links] = m_gameLogic->Recommend();

	UpdateHintLabel("Hint: " + QString::fromStdString(std::to_string(pos.row)) + " " +
		QString::fromStdString(std::to_string(pos.col)));

	for (const auto& [pos1, pos2] : links)
	{
		m_board[pos1.row][pos1.col]->Select();
		m_board[pos2.row][pos2.col]->Select();
	}
}

void TwixtGUIQt::OnRequestDrawButtonClicked()
{
	if (m_gameLogic->IsGameOver())
	{
		return;
	}

	QMessageBox::StandardButton reply = QMessageBox::question(
		this,
		"Draw Request",
		"Do you both want a draw?",
		QMessageBox::Yes | QMessageBox::No
	);

	if (reply == QMessageBox::Yes)
	{
		m_gameLogic->RequestDraw(m_gameLogic->GetCurrentPlayerColor());
	}
}

void TwixtGUIQt::OnEndTurnButtonClicked()
{
	if (m_gameLogic->IsGameOver())
	{
		return;
	}

	m_endTurnButton->setEnabled(false);

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

	try
	{
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
	catch (const GameException&)
	{
		m_clickCount = 1;
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

void TwixtGUIQt::OnPiecePlaced(const Position& pos)
{
	m_board[pos.row][pos.col]->SetPeg(m_gameLogic->GetCurrentPlayerColor());
	m_endTurnButton->setEnabled(true);
}

void TwixtGUIQt::OnGameOver(const EGameResult& result)
{
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

	m_clickCount = 0;
	m_isFirstTurn = true;

	UpdateCurrentPlayerLabel();
}

void TwixtGUIQt::OnLinkPlaced(const Position& pos1, const Position& pos2)
{
	const QPoint center1 = m_board[pos1.row][pos1.col]->GetCenter();
	const QPoint center2 = m_board[pos2.row][pos2.col]->GetCenter();

	m_links.emplaceBack(QLine{center1, center2}, m_gameLogic->GetCurrentPlayerColor());

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

	update();
}

void TwixtGUIQt::OnDrawRequested(EColor current_player)
{
	QMessageBox::StandardButton reply = QMessageBox::question(
		this,
		"Draw Request",
		"Player " + QString(ColorToString(static_cast<int>(current_player), true)) +
		" requested a draw.\nDo you accept?",
		QMessageBox::Yes | QMessageBox::No
	);

	if (reply == QMessageBox::Yes)
	{
		//m_gameLogic->SetDraw();
	}
}

void TwixtGUIQt::InitializeUI()
{
	InitializeTitleLabel();
	InitializeGameControlButtons();
	InitializeGameActionsButtons();
	InitializeCurrentPlayerLabel();
	InitializeErrorLabel();
	InitializeHintLabel();
	InitializeBoard();
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
	m_mainGridLayout->addWidget(m_controlButtonsContainer.data(), 3, 0, 1, 1);
}

void TwixtGUIQt::InitializeGameActionsButtons()
{
	m_getHintButton = QSharedPointer<QPushButton>{new QPushButton{"Get Hint"}};
	m_requestDrawButton = QSharedPointer<QPushButton>{new QPushButton{"Request draw"}};
	m_endTurnButton = QSharedPointer<QPushButton>{new QPushButton{"End turn"}};

	m_actionsButtonsContainer = QSharedPointer<QWidget>{new QWidget{}};
	m_actionsButtonsContainerLayout = QSharedPointer<QGridLayout>{new QGridLayout{}};

	m_actionsButtonsContainerLayout->addWidget(m_getHintButton.data(), 1, 0);
	m_actionsButtonsContainerLayout->addWidget(m_requestDrawButton.data(), 2, 0);
	m_actionsButtonsContainerLayout->addWidget(m_endTurnButton.data(), 3, 0);

	// TODO: connect signals and slots
	connect(m_getHintButton.data(), &QPushButton::clicked, this, &TwixtGUIQt::OnGetHintButtonClicked);
	connect(m_requestDrawButton.data(), &QPushButton::clicked, this, &TwixtGUIQt::OnRequestDrawButtonClicked);
	connect(m_endTurnButton.data(), &QPushButton::clicked, this, &TwixtGUIQt::OnEndTurnButtonClicked);

	m_endTurnButton->setEnabled(false);

	SetStyle(m_actionsButtonsContainer.data(), "stylesheets/Button.css");

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

	//m_boardContainer->setFixedSize(760, 760);

	m_boardContainer->setLayout(m_boardContainerLayout.data());
	m_mainGridLayout->addWidget(m_boardContainer.data(), 0, 1, 4, 1);
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

void TwixtGUIQt::SetFont()
{
	const int id = QFontDatabase::addApplicationFont("fonts/Poppins-Regular.ttf");
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
