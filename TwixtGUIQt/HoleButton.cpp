#include "HoleButton.h"

#include <QMouseEvent>

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

void HoleButton::SetPeg(EColor color)
{
	m_color = color;
	UpdatePeg();
}

void HoleButton::ResetPeg()
{
	m_color = std::nullopt;
	UpdatePeg();
}

std::optional<EColor> HoleButton::GetColor() const
{
	return m_color;
}

void HoleButton::SetCenter(const QPoint& center)
{
	m_center = center;
}

QPoint HoleButton::GetCenter() const
{
	return m_center;
}

void HoleButton::mouseReleaseEvent(QMouseEvent* event)
{
	QPushButton::mouseReleaseEvent(event);

	Deselect();

	if (isEnabled())
	{
		if (event->button() == Qt::RightButton)
		{
			emit RightClicked(m_position);
		}
		else
		{
			emit Clicked(m_position);
		}
	}
}

void HoleButton::mousePressEvent(QMouseEvent* event)
{
	QPushButton::mousePressEvent(event);
	Select();
}

void HoleButton::UpdatePeg()
{
	QString path = "assets/";

	if (m_color.has_value())
	{
		path += ColorToString(m_color.value());
	}
	else
	{
		path += "empty";
	}

	if (m_isSelected)
	{
		path += "_pressed";
	}

	path += ".png";

	const QPixmap pixmap(path);
	const QIcon icon(pixmap);
	setIcon(icon);
	setIconSize(pixmap.rect().size());

	setStyleSheet("border: none;");
}

void HoleButton::Select()
{
	m_isSelected = true;
	UpdatePeg();
}

void HoleButton::Deselect()
{
	m_isSelected = false;
	UpdatePeg();
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