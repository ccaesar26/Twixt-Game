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
    explicit TwixtGUIQt(QWidget *parent = nullptr);
    ~TwixtGUIQt() override;

    // Copy and move semantics

private:
    // IGameListener overrides
    void OnPiecePlaced(const Position& pos) override;
    void OnGameOver(const EGameResult& result) override;
    void OnGameRestarted() override;

    // UI initializations
    void InitializeTitleLabel();
	void InitializeCurrentPlayerLabel();
	void InitializeGameControlButtons(QGridLayout* mainGridLayout);
    static void InitializeGameActionsButtons(QGridLayout* mainGridLayout);

private:
    QSharedPointer<QLabel> m_titleLabel;
    QSharedPointer<QLabel> m_currentPlayerLabel;

    QSharedPointer<QGridLayout> m_mainGridLayout;
};
