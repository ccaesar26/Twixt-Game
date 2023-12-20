#include "HoleButton.h"

namespace
{
	QString ColorToString(const EColor color)
	{
		switch (color)
		{
		case EColor::Red:
			return "red";
		case EColor::Black:
			return "black";
		default:
			return "white";
		}
	}
}

HoleButton::HoleButton(const Position& position, QWidget* parent)
	: QPushButton(parent)
	, m_position(position)
	, m_color(std::nullopt)
{
	setFixedSize(QSize(18, 18));
	UpdatePeg();
}

HoleButton::HoleButton(const Position& position, EColor color, QWidget* parent)
	: QPushButton(parent)
	, m_position(position)
	, m_color(color)
{
	setFixedSize(QSize(18, 18));
	UpdatePeg();
}

void HoleButton::PlacePeg(EColor color)
{
	m_color = color;
}

std::optional<EColor> HoleButton::GetColor() const
{
	return m_color;
}

void HoleButton::mouseReleaseEvent(QMouseEvent* event)
{
	QPushButton::mouseReleaseEvent(event);
	if (isEnabled())
	{
		emit Clicked(m_position);
	}
}

void HoleButton::UpdatePeg()
{
	if (m_color.has_value())
	{
		setStyleSheet("background-color: " + ColorToString(m_color.value()) + ";");
	}
	else
	{
		setStyleSheet("background-color: white;");
	}
	
	setStyleSheet("border-radius: 25px;");
}

HoleButton::HoleButton(const HoleButton& other)
	: QPushButton{ other.parentWidget() }
	, m_position{ other.m_position }
	, m_color{ other.m_color }
{
}

HoleButton::HoleButton(HoleButton&& other) noexcept
	: QPushButton{ other.parentWidget() }
	, m_position{ std::move(other.m_position) }
	, m_color{ std::move(other.m_color) }
{
}

HoleButton& HoleButton::operator=(const HoleButton& other)
{
	if (this != &other)
	{
		m_position = other.m_position;
		m_color = other.m_color;
	}
	return *this;
}

HoleButton& HoleButton::operator=(HoleButton&& other) noexcept
{
	if (this != &other)
	{
		m_position = std::move(other.m_position);
		m_color = std::move(other.m_color);
	}
	return *this;
}