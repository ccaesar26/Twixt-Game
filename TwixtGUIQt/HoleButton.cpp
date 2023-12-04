#include "HoleButton.h"

HoleButton::HoleButton(const Position& position, QWidget* parent)
	: QPushButton(parent)
	, m_position(position)
	, m_color(std::nullopt)
{
}

HoleButton::HoleButton(const Position& position, EColor color, QWidget* parent)
	: QPushButton(parent)
	, m_position(position)
	, m_color(color)
{
}
