#include "commentwidget.h"
#include "ui_commentwidget.h"

#include "data/game.h"

CommentWidget::CommentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommentWidget)
{
    ui->setupUi(this);
}

CommentWidget::~CommentWidget()
{
    delete ui;
}

void CommentWidget::setGame(QSharedPointer<Game> game)
{
    m_game = game;

    ui->lineEditName->setText(m_game->name());
    ui->textEditComment->setText(m_game->comment());
}

void CommentWidget::on_textEditComment_textChanged()
{
    if(!m_game){
        return;
    }

    m_game->setComment(ui->textEditComment->toPlainText());
}

void CommentWidget::on_lineEditName_editingFinished()
{
    if(!m_game){
        return;
    }

    m_game->setName(ui->lineEditName->text());
}
