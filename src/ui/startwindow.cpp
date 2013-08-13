#include "startwindow.h"

#include <QVBoxLayout>
#include <QPushButton>

#include "game/gamewindow.h"
#include "widgets/menubar.h"

StartWindow::StartWindow(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QHBoxLayout* buttonLayout = new QHBoxLayout(this);
    QPushButton* newGame = new QPushButton("New Game", this);
    buttonLayout->addWidget(newGame);
    connect(newGame, SIGNAL(clicked()), this, SLOT(onNewGameButtonPressed()));
    QPushButton* libraryButton = new QPushButton("Library", this);
    buttonLayout->addWidget(libraryButton);
    layout->addLayout(buttonLayout);
    connect(libraryButton, SIGNAL(clicked()), this, SLOT(onLibraryButtonPressed()));

    this->setLayout(layout);
}

void StartWindow::onNewGameButtonPressed()
{
    GameWindow *window = new GameWindow(this);
    window->show();
}

void StartWindow::onLibraryButtonPressed()
{
    MenuBar::instance()->showLibrary();
}
