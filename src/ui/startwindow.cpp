#include "startwindow.h"

#include <QVBoxLayout>
#include <QPushButton>

#include "game/gamewindow.h"
#include "widgets/menubar.h"

#include "data/game.h"

StartWindow::StartWindow(QWidget *parent) :
    QWidget(parent)
{
    QList<QSharedPointer<Game> > allGames = Qp::readAll<Game>();
    for(int i = 0; i<5; i++) {
        m_games.append(allGames.takeLast());
    }

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QHBoxLayout* buttonLayout = new QHBoxLayout(this);
    QPushButton* newGame = new QPushButton("New Game", this);
    buttonLayout->addWidget(newGame);
    connect(newGame, SIGNAL(clicked()), this, SLOT(onNewGameButtonPressed()));
    QPushButton* libraryButton = new QPushButton("Library", this);
    buttonLayout->addWidget(libraryButton);
    layout->addLayout(buttonLayout);
    connect(libraryButton, SIGNAL(clicked()), this, SLOT(onLibraryButtonPressed()));

    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setMinimumWidth(500);
    m_treeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_treeWidget->setColumnCount(3);
    m_treeWidget->headerItem()->setHidden(true);
    int height = 0;
    for(int i = 0; i<m_games.size(); i++) {
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << "" << m_games.at(i)->name() << m_games.at(i)->creationTime().date().toString("dd.MM.yyyy"));
        item->setIcon(0, QIcon(m_games.at(i)->statePixmap()));
        item->setTextAlignment(2, Qt::AlignRight);
        m_treeWidget->addTopLevelItem(item);
        height = item->sizeHint(1).height();
    }
    m_treeWidget->resizeColumnToContents(0);
    m_treeWidget->resizeColumnToContents(1);
    m_treeWidget->resizeColumnToContents(2);
    m_treeWidget->setFixedHeight(m_treeWidget->visualItemRect(m_treeWidget->topLevelItem(0)).height()*5 + 5);
    layout->addWidget(m_treeWidget);
    connect(m_treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(onGameTreeWidgetDoubleClicked(QTreeWidgetItem*,int)));

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

void StartWindow::onGameTreeWidgetDoubleClicked(QTreeWidgetItem* item, int column)
{
    int index = m_treeWidget->indexOfTopLevelItem(item);
    QSharedPointer<Game> game = m_games.at(index);
    if(!game)
        return;

    GameWindow *window = new GameWindow(this);
    window->setGame(game);
    window->show();

}
