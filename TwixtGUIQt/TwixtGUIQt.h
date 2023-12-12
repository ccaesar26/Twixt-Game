#pragma once

#include <QtWidgets/QMainWindow>
#include <QGridLayout>

#include "IGameListener.h"
#include "IGame.h"

#include "HoleButton.h"

class TwixtGUIQt : public QMainWindow, public IGameListener
{
    Q_OBJECT

public:
    // Constructors and destructor
    TwixtGUIQt(QWidget *parent = nullptr);
    ~TwixtGUIQt() override;

    // Copy and move semantics

    // IGameListener overrides
    void OnPiecePlaced(const Position& pos) override;
    void OnGameOver(const EGameResult& result) override;
    void OnGameRestarted() override;

    // UI initializations
    static void InitializeTitleLabel(QGridLayout* mainGridLayout);
	static void InitializeGameControlButtons(QGridLayout* mainGridLayout);
private:
};
