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

private:
    // IGameListener overrides
    void OnPiecePlaced(const Position& pos) override;
    void OnGameOver(const EGameResult& result) override;
    void OnGameRestarted() override;

    // UI initializations
    void InitializeTitleLabel();
	void InitializeCurrentPlayerLabel();
    void InitializeHintLabel();
	void InitializeGameControlButtons();
    void InitializeGameActionsButtons();

private:
    QSharedPointer<QLabel> m_titleLabel;
    QSharedPointer<QLabel> m_currentPlayerLabel;
    QSharedPointer<QLabel> m_hintLabel;

    QSharedPointer<QWidget> m_controlButtonsContainer;
    QSharedPointer<QGridLayout> m_controlButtonsContainerLayout;
    QSharedPointer<QPushButton> m_restartButton;
    QSharedPointer<QPushButton> m_saveButton;
    QSharedPointer<QPushButton> m_loadButton;
    QSharedPointer<QPushButton> m_quitButton;

    QSharedPointer<QWidget> m_actionsButtonsContainer;
    QSharedPointer<QGridLayout> m_actionsButtonsContainerLayout;
    QSharedPointer<QPushButton> m_placeBridgeButton;
    QSharedPointer<QPushButton> m_removeBridgeButton;
    QSharedPointer<QPushButton> m_requestDrawButton;
    QSharedPointer<QPushButton> m_endTurnButton;

    QSharedPointer<QGridLayout> m_mainGridLayout;

    std::shared_ptr<IGame> m_gameLogic;
};
