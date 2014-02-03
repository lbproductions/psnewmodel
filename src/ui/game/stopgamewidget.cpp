#include "stopgamewidget.h"
#include "ui_stopgamewidget.h"

#include <data/game.h>
#include <misc/tools.h>

StopGameWidget::StopGameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StopGameWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setPalette(Tools::darkPalette(this));

    connect(ui->buttonBox, &QDialogButtonBox::rejected,
            this, &QWidget::close);
}

StopGameWidget::~StopGameWidget()
{
    delete ui;
}

void StopGameWidget::setGame(QSharedPointer<Game> game)
{
    m_game = game;
}

void StopGameWidget::on_buttonBox_accepted()
{
    if(!m_game)
        return;

    m_game->setState(Game::Finished);
    close();
}
