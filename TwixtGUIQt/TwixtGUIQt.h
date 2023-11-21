#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TwixtGUIQt.h"

class TwixtGUIQt : public QMainWindow
{
    Q_OBJECT

public:
    TwixtGUIQt(QWidget *parent = nullptr);
    ~TwixtGUIQt();

private:
    Ui::TwixtGUIQtClass ui;
};
