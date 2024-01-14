#include "ConfigurationDialog.h"

ConfigurationDialog::ConfigurationDialog(QWidget* parent, int boardSize, int maxPegs, int maxLinks)
	: QDialog{parent}
{
    m_boardSizeEdit = QSharedPointer<QLineEdit>::create(QString::number(boardSize));
    m_maxPegsEdit = QSharedPointer<QLineEdit>::create(QString::number(maxPegs));
    m_maxLinksEdit = QSharedPointer<QLineEdit>::create(QString::number(maxLinks));

    m_okButton = QSharedPointer<QPushButton>::create("OK");
    m_cancelButton = QSharedPointer<QPushButton>::create("Cancel");

    InitializeUi();
}

void ConfigurationDialog::InitializeUi()
{
    m_formLayout = QSharedPointer<QFormLayout>::create(this);
    m_formLayout->addRow("Board Size:", m_boardSizeEdit.data());
    m_formLayout->addRow("Max Pegs:", m_maxPegsEdit.data());
    m_formLayout->addRow("Max Links:", m_maxLinksEdit.data());
    m_formLayout->addRow(m_okButton.data(), m_cancelButton.data());

    connect(m_okButton.data(), &QPushButton::clicked, this, &ConfigurationDialog::accept);
    connect(m_cancelButton.data(), &QPushButton::clicked, this, &ConfigurationDialog::reject);
}

int ConfigurationDialog::GetBoardSize() const
{
    return m_boardSizeEdit->text().toInt();
}

int ConfigurationDialog::GetMaxPegs() const
{
    return m_maxPegsEdit->text().toInt();
}

int ConfigurationDialog::GetMaxLinks() const
{
    return m_maxLinksEdit->text().toInt();
}