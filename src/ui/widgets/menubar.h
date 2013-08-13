#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>

#include <QPointer>

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = 0);

    static MenuBar *instance();

    void addAction(const QString &menu, QAction *action, QWidget *widget);
    QMenu *menu(const QString &name);

private slots:
    void triggerAction();
    void enableActionsBasedOnCurrentWindow();
    void enableGlobalActionsBasedOnCurrentWindow();

    void closeFocusWindow();

public slots:
    void showLibrary();

private:
    QHash<QString, QMenu *> m_menus;
    QHash<QString, QHash<QString, QAction *> > m_actions;
    QHash<QWidget *, QHash<QAction *, QPointer<QAction> > > m_actionsPerWidget;

    QAction *m_actionCloseWindow;
    QAction *m_actionShowLibrary;

    void copyActionState(QAction *source, QAction *dest);

};

#endif // MENUBAR_H
