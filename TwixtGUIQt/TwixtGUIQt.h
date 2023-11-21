#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TwixtGUIQt.h"
#include "IGameListener.h"

class TwixtGUIQt : public QMainWindow, public IGameListener
{
    Q_OBJECT

public:
    TwixtGUIQt(QWidget *parent = nullptr);
    ~TwixtGUIQt();

    void OnPiecePlaced(const Position& pos) override;
    void OnGameOver(const EGameResult& result) override;
    void OnGameRestarted() override;

private:
    Ui::TwixtGUIQtClass ui;
};