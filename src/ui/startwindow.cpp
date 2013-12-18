#include "startwindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>

#include "game/gamewindow.h"
#include "widgets/menubar.h"
#include "league/addleaguedialog.h"
#include "league/leaguewindow.h"

#include "data/game.h"
#include "data/league.h"

#include <misc/tools.h>

StartWindow::StartWindow(QWidget *parent) :
    QWidget(parent)
{
    QList<QSharedPointer<Game> > allGames = Qp::readAll<Game>();
    for(int i = 0; i<5; i++) {
        m_games.append(allGames.takeLast());
    }

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setPalette(Tools::darkPalette(this));

    QVBoxLayout* layout = new QVBoxLayout(this);

    QHBoxLayout* logoLayout = new QHBoxLayout(this);
    logoLayout->addStretch();
    QLabel* logo = new QLabel(this);
    logo->setPixmap(QPixmap(":/general/logo.png").scaledToWidth(200));
    logoLayout->addWidget(logo);
    logoLayout->addStretch();
    layout->addLayout(logoLayout);
    layout->addStretch(0);

    QVBoxLayout* gameGrouBoxLayout = new QVBoxLayout(this);
    QHBoxLayout* buttonLayout = new QHBoxLayout(this);

    QPushButton* newGame = new QPushButton("New Game", this);
    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", newGame);
    buttonLayout->addWidget(newGame);
    connect(newGame, SIGNAL(clicked()), this, SLOT(onNewGameButtonPressed()));

    QPushButton* libraryButton = new QPushButton("Library", this);
    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", libraryButton);
    buttonLayout->addWidget(libraryButton);
    gameGrouBoxLayout->addLayout(buttonLayout);
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
    gameGrouBoxLayout->addWidget(m_treeWidget);
    connect(m_treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(onGameTreeWidgetDoubleClicked(QTreeWidgetItem*,int)));

    QGroupBox* gameGroup = new QGroupBox(this);
    gameGroup->setTitle(tr("Games"));
    gameGroup->setAlignment(Qt::AlignHCenter);
    gameGroup->setLayout(gameGrouBoxLayout);
    layout->addWidget(gameGroup);

    layout->addStretch(100);

    QVBoxLayout* leagueGrouBoxLayout = new QVBoxLayout(this);
    QPushButton* addLeagueButton = new QPushButton(tr("New League"), this);
    Tools::setStyleSheetFromResource(":/stylesheets/pushbutton-dark.qss", addLeagueButton);
    leagueGrouBoxLayout->addWidget(addLeagueButton);
    connect(addLeagueButton, SIGNAL(pressed()), this, SLOT(onNewLeagueButtonPressed()));

    m_leagues = Qp::readAll<League>();

    m_leagueTreeWidget = new QTreeWidget(this);
    m_leagueTreeWidget->headerItem()->setHidden(true);
    for(int i = 0; i<m_leagues.size(); i++) {
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << m_leagues.at(i)->name());
        m_leagueTreeWidget->addTopLevelItem(item);
        height = item->sizeHint(0).height();
    }
    m_leagueTreeWidget->setFixedHeight(m_leagueTreeWidget->visualItemRect(m_leagueTreeWidget->topLevelItem(0)).height()*m_leagues.size() + 5);
    //m_leagueTreeWidget->sortByColumn(0);
    connect(m_leagueTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(onLeagueTreeWidgetDoubleClicked(QTreeWidgetItem*,int)));
    leagueGrouBoxLayout->addWidget(m_leagueTreeWidget);

    QGroupBox* leagueGroup = new QGroupBox(this);
    leagueGroup->setTitle(tr("Leagues"));
    leagueGroup->setAlignment(Qt::AlignHCenter);
    leagueGroup->setLayout(leagueGrouBoxLayout);
    layout->addWidget(leagueGroup);

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

void StartWindow::onLeagueTreeWidgetDoubleClicked(QTreeWidgetItem *item, int column)
{
    int index = m_leagueTreeWidget->indexOfTopLevelItem(item);
    QSharedPointer<League> league = m_leagues.at(index);
    if(!league)
        return;

    LeagueWindow* leagueWindow = new LeagueWindow(this);
    leagueWindow->setLeague(league);
    leagueWindow->show();
}

void StartWindow::onNewLeagueButtonPressed()
{
    AddLeagueDialog* dialog = new AddLeagueDialog(this);
    dialog->show();
}
