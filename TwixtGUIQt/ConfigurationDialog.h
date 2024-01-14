#pragma once

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

class ConfigurationDialog final :
    public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationDialog(QWidget* parent = nullptr, int boardSize = 24, int maxPegs = 50, int maxLinks = 50);

    [[nodiscard]] int GetBoardSize() const;
    [[nodiscard]] int GetMaxPegs() const;
    [[nodiscard]] int GetMaxLinks() const;

private:
    void InitializeUi();

    QSharedPointer<QLineEdit> m_boardSizeEdit;
    QSharedPointer<QLineEdit> m_maxPegsEdit;
    QSharedPointer<QLineEdit> m_maxLinksEdit;

    QSharedPointer<QPushButton> m_okButton;
    QSharedPointer<QPushButton> m_cancelButton;

    QSharedPointer<QFormLayout> m_formLayout;
};

