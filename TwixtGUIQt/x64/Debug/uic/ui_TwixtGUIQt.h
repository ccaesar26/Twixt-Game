/********************************************************************************
** Form generated from reading UI file 'TwixtGUIQt.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TWIXTGUIQT_H
#define UI_TWIXTGUIQT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TwixtGUIQtClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TwixtGUIQtClass)
    {
        if (TwixtGUIQtClass->objectName().isEmpty())
            TwixtGUIQtClass->setObjectName("TwixtGUIQtClass");
        TwixtGUIQtClass->resize(600, 400);
        menuBar = new QMenuBar(TwixtGUIQtClass);
        menuBar->setObjectName("menuBar");
        TwixtGUIQtClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TwixtGUIQtClass);
        mainToolBar->setObjectName("mainToolBar");
        TwixtGUIQtClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(TwixtGUIQtClass);
        centralWidget->setObjectName("centralWidget");
        TwixtGUIQtClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(TwixtGUIQtClass);
        statusBar->setObjectName("statusBar");
        TwixtGUIQtClass->setStatusBar(statusBar);

        retranslateUi(TwixtGUIQtClass);

        QMetaObject::connectSlotsByName(TwixtGUIQtClass);
    } // setupUi

    void retranslateUi(QMainWindow *TwixtGUIQtClass)
    {
        TwixtGUIQtClass->setWindowTitle(QCoreApplication::translate("TwixtGUIQtClass", "TwixtGUIQt", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TwixtGUIQtClass: public Ui_TwixtGUIQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TWIXTGUIQT_H
