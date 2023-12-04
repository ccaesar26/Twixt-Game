#pragma once
#include <qpushbutton.h>

#include "EColor.h"
#include "Position.h"

class HoleButton :
    public QPushButton
{
    Q_OBJECT
public:
    HoleButton(const Position& position, QWidget* parent = nullptr);


signals:

private:
    Position m_position;
    std::optional<EColor> m_color;
};

