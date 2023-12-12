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
    HoleButton(const Position& position, EColor color, QWidget* parent = nullptr);
    HoleButton(const HoleButton& other);
    HoleButton(HoleButton&& other) noexcept;
    virtual ~HoleButton() override = default;
	
    HoleButton& operator=(const HoleButton& other);
    HoleButton& operator=(HoleButton&& other) noexcept;
	
    void PlacePeg(EColor color);
    std::optional<EColor> GetColor() const;
	
    void mouseReleaseEvent(QMouseEvent* event) override;

signals:
    void Clicked(const std::pair<int, int>& position);

private:
    Position m_position;
    std::optional<EColor> m_color;
};

