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

void HoleButton::PlacePeg(EColor color)
{
	m_color = color;
}

std::optional<EColor> HoleButton::GetColor() const
{
	return m_color;
}

HoleButton::HoleButton(const HoleButton& other)
	: QPushButton{ other.parentWidget() }
	, m_position{ other.m_position }
	, m_color{ other.m_color }
{
}
