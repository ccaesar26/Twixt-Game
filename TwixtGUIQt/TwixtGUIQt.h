#pragma once

#include <QtWidgets/QMainWindow>
#include <QGridLayout>
#include <QLabel>

#include "IGameListener.h"
#include "IGame.h"

#include "HoleButton.h"

class TwixtGUIQt : public QMainWindow, public IGameListener
{
    Q_OBJECT

public:
    // Constructors and destructor
    explicit TwixtGUIQt(QWidget* parent = nullptr);
    ~TwixtGUIQt() override = default;

    // No other constructors or assignment operators?
    // See https://doc.qt.io/qt-6/qobject.html#no-copy-constructor-or-assignment-operator

    // Setters
    void SetGameLogic(std::shared_ptr<IGame>&& gameLogic);

    void MapCoordinates();

public slots:
	//// UI event handlers
	void OnRestartButtonClicked();
	void OnSaveButtonClicked();
	void OnLoadButtonClicked();

    void OnGetHintButtonClicked();
	void OnRequestDrawButtonClicked();
	void OnEndTurnButtonClicked();
    void OnChangeConfigurationButtonClicked();

	void OnHoleButtonClicked(const Position& pos);
    void OnHoleButtonRightClicked(const Position& pos);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    // IGameListener overrides
    void OnBoardChanged(int newSize, int newMaxPegs, int newMaxLinks) override;
    void OnPiecePlaced(const Position& pos) override;
    void OnGameOver(const EGameResult& result) override;
    void OnGameRestarted() override;
    void OnGameLoaded() override;
    void OnLinkPlaced(const Position& pos1, const Position& pos2) override;
    void OnLinkRemoved(const Position& pos1, const Position& pos2) override;
    void OnDrawRequested(EColor current_player) override;
    void OnHintRecommended(std::pair<Position, Position> link) override;

    // UI initializations
    void InitializeUI();

    void InitializeTitleLabel();
	void InitializeCurrentPlayerLabel();
    void InitializeErrorLabel();
    void InitializeHintLabel();
	void InitializeGameControlButtons();
    void InitializeGameActionsButtons();
    void InitializeBoard();
    void InitializeStats();

    // UI Update methods
    void UpdateCurrentPlayerLabel();
    void UpdateErrorLabel(QString error);
    void UpdateHintLabel(QString hint);

    void ClearHint();

    // Style methods
    void SetFont();
    void SetStyle(QWidget* widget, const QString& styleSheetPath);

private:
    QSharedPointer<QLabel> m_titleLabel;
    QSharedPointer<QLabel> m_currentPlayerLabel;
    QSharedPointer<QLabel> m_errorLabel;
    QSharedPointer<QLabel> m_hintLabel;

    QSharedPointer<QWidget> m_controlButtonsContainer;
    QSharedPointer<QGridLayout> m_controlButtonsContainerLayout;
    QSharedPointer<QPushButton> m_restartButton;
    QSharedPointer<QPushButton> m_saveButton;
    QSharedPointer<QPushButton> m_loadButton;
    QSharedPointer<QPushButton> m_quitButton;

    QSharedPointer<QWidget> m_actionsButtonsContainer;
    QSharedPointer<QGridLayout> m_actionsButtonsContainerLayout;
    QSharedPointer<QPushButton> m_getHintButton;
    QSharedPointer<QPushButton> m_requestDrawButton;
    QSharedPointer<QPushButton> m_changeConfiguration;
    QSharedPointer<QPushButton> m_endTurnButton;

    QSharedPointer<QWidget> m_boardContainer;
    QSharedPointer<QGridLayout> m_boardContainerLayout;
    QVector<QVector<QSharedPointer<HoleButton>>> m_board;

    QSharedPointer<QWidget> m_statsContainer;
    QSharedPointer<QGridLayout> m_statsContainerLayout;
    QSharedPointer<QLabel> m_statsLabel;

    QSharedPointer<QGridLayout> m_mainGridLayout;

    QVector<QPair<QLine, EColor>> m_links;

    std::shared_ptr<IGame> m_gameLogic = nullptr;

    int m_clickCount;
    Position m_firstClick;
    Position m_secondClick;

    QVector<Position> m_hint = {};

    bool m_isFirstTurn;
};
