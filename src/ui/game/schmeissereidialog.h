#ifndef SCHMEISSEREIDIALOG_H
#define SCHMEISSEREIDIALOG_H

#include <QWidget>

namespace Ui {
    class SchmeissereiWidget;
}

class Round;
class Game;

class SchmeissereiDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SchmeissereiDialog(QWidget *parent = 0);
    ~SchmeissereiDialog();

    QSharedPointer<Round> round() const;
    QSharedPointer<Game> game() const;
    void setGame(const QSharedPointer<Game> &game);

private slots:
    void save();
    void checkContents();

    void on_spinBoxRoundNumber_valueChanged(int value);

private:
    Ui::SchmeissereiWidget *ui;

    QSharedPointer<Round> m_round;
    QSharedPointer<Game> m_game;

    void setRound(const QSharedPointer<Round> &round);
};

#endif // SCHMEISSEREIDIALOG_H
