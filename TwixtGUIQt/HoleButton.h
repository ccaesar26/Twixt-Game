#pragma once
#include <QPushButton>

#include "EColor.h"
#include "Position.h"

class HoleButton :
    public QPushButton
{
    Q_OBJECT
public:
    HoleButton(const Position& position, QWidget* parent = nullptr);
    HoleButton(const Position& position, EColor color, QWidget* parent = nullptr);
    HoleButton(const HoleButton& other);
    HoleButton(HoleButton&& other) noexcept;
    virtual ~HoleButton() override = default;
	
    HoleButton& operator=(const HoleButton& other);
    HoleButton& operator=(HoleButton&& other) noexcept;
	
    void SetPeg(EColor color);

    std::optional<EColor> GetColor() const;

    void SetCenter(const QPoint& center);
    QPoint GetCenter() const;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void UpdatePeg();

signals:
    void Clicked(const Position& position);
    void RightClicked(Position position);

private:
    Position m_position;
    std::optional<EColor> m_color;
    QPoint m_center;
};

