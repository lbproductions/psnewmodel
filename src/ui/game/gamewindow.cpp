#include "gamewindow.h"
#include "ui_gamewindow.h"

#include "overviewplayerheaderview.h"
#include "overviewhorizontalheaderview.h"
#include "overviewdelegate.h"

#include <data/game.h>
#include <model/gameoverviewmodel.h>

#include <QPersistence.h>
#include <QScrollBar>
#include <QWheelEvent>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow),
    m_gameOverViewModel(new GameOverviewModel(this))
{
    ui->setupUi(this);
    QPalette darkPalette = palette();
    darkPalette.setColor(QPalette::Window, QColor(71,71,71));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(55,55,55));
    darkPalette.setColor(QPalette::AlternateBase, QColor(49,49,49));
    darkPalette.setColor(QPalette::ToolTipBase, darkPalette.color(QPalette::Base));
    darkPalette.setColor(QPalette::ToolTipText, darkPalette.color(QPalette::WindowText));
    darkPalette.setColor(QPalette::Text, QColor(228,228,228));
    darkPalette.setColor(QPalette::Button, QColor(61,61,61));
    darkPalette.setColor(QPalette::ButtonText, QColor(226,226,226));
    darkPalette.setColor(QPalette::BrightText, QColor(43,43,43));
    darkPalette.setColor(QPalette::Highlight, QColor(108,108,108));
    darkPalette.setColor(QPalette::HighlightedText, QColor(187,187,187));
    setPalette(darkPalette);

    ui->tableView->setPalette(darkPalette);
    ui->tableView->setModel(m_gameOverViewModel);
    ui->tableView->setVerticalHeader(new OverviewPlayerHeaderView(Qt::Vertical, this));
    ui->tableView->setHorizontalHeader(new OverviewHorizontalHeaderView(Qt::Horizontal, this));
    ui->tableView->setItemDelegate(new OverviewDelegate(this));

    connect(ui->scrollAreaGraph->horizontalScrollBar(), &QScrollBar::valueChanged,
            ui->tableView->horizontalScrollBar(), &QScrollBar::setValue);

    ui->graphWidget->setPalette(darkPalette);

//QPalette::Window	10	A general background color.
//QPalette::Background	Window	This value is obsolete. Use Window instead.
//QPalette::WindowText	0	A general foreground color.
//QPalette::Foreground	WindowText	This value is obsolete. Use WindowText instead.
//QPalette::Base	9	Used mostly as the background color for text entry widgets, but can also be used for other painting - such as the background of combobox drop down lists and toolbar handles. It is usually white or another light color.
//QPalette::AlternateBase	16	Used as the alternate background color in views with alternating row colors (see QAbstractItemView::setAlternatingRowColors()).
//QPalette::ToolTipBase	18	Used as the background color for QToolTip and QWhatsThis. Tool tips use the Inactive color group of QPalette, because tool tips are not active windows.
//QPalette::ToolTipText	19	Used as the foreground color for QToolTip and QWhatsThis. Tool tips use the Inactive color group of QPalette, because tool tips are not active windows.
//QPalette::Text	6	The foreground color used with Base. This is usually the same as the WindowText, in which case it must provide good contrast with Window and Base.
//QPalette::Button	1	The general button background color. This background can be different from Window as some styles require a different background color for buttons.
//QPalette::ButtonText	8	A foreground color used with the Button color.
//QPalette::BrightText	7	A text color that is very different from WindowText, and contrasts well with e.g. Dark. Typically used for text that needs to be drawn where Text or WindowText would give poor contrast, such as on pressed push buttons. Note that text colors can be used for things other than just words; text colors are usually used for text, but it's quite common to use the text color roles for lines, icons, etc.
//QPalette::Highlight	12	A color to indicate a selected item or the current item. By default, the highlight color is Qt::darkBlue.
//QPalette::HighlightedText	13	A text color that contrasts with Highlight. By default, the highlighted text color is Qt::white.
}

GameWindow::~GameWindow()
{
    delete ui;
}

QSharedPointer<Game> GameWindow::game() const
{
    return m_game;
}

void GameWindow::setGame(const QSharedPointer<Game> &game)
{
    m_game = game;
    m_gameOverViewModel->setGame(game);
    ui->tableView->setFixedHeight(ui->tableView->horizontalHeader()->height() +
                                  m_gameOverViewModel->rowCount() * ui->tableView->rowHeight(0));
    ui->graphWidget->setGame(game);
    ui->graphWidget->setOriginX(ui->tableView->verticalHeader()->width() - 1);
}

void GameWindow::wheelEvent(QWheelEvent *e)
{
    if(e->pixelDelta().x() > 0)
        ui->scrollAreaGraph->horizontalScrollBar()->setValue(ui->scrollAreaGraph->horizontalScrollBar()->value() + e->pixelDelta().x());
    else
        ui->scrollAreaGraph->horizontalScrollBar()->setValue(ui->scrollAreaGraph->horizontalScrollBar()->value() - e->pixelDelta().y());
}
