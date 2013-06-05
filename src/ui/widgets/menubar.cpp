#include "menubar.h"

#include <ui/mainwindow.h>

#include <QApplication>
#include <QAction>
#include <QWindow>

MenuBar::MenuBar(QWidget *parent) :
    QMenuBar(parent)
{
    connect(QApplication::instance(), SIGNAL(focusChanged(QWidget*,QWidget*)),
            this, SLOT(enableActionsBasedOnCurrentWindow()));

    QMenu *menuFile = addMenu(tr("&File"));
    m_actionCloseWindow = menuFile->addAction(tr("Close"));
    m_actionCloseWindow->setShortcut(QKeySequence::Close);
    connect(m_actionCloseWindow, &QAction::triggered,
            this, &MenuBar::closeFocusWindow);

    menuFile->addSeparator();
    m_menus.insert(menuFile->title(), menuFile);

    QMenu *menuView = addMenu(tr("&View"));
    m_actionShowLibrary = menuView->addAction(tr("Library"));
    m_actionShowLibrary->setShortcut(Qt::CTRL + Qt::Key_0);
    connect(m_actionShowLibrary, &QAction::triggered,
            this, &MenuBar::showLibrary);

    menuView->addSeparator();
    m_menus.insert(menuView->title(), menuView);

}

MenuBar *MenuBar::instance()
{
    static MenuBar *i = nullptr;
    if(!i)
        i = new MenuBar;
    return i;
}

void MenuBar::addAction(const QString &menu, QAction *action, QWidget *widget)
{
    QMenu *m;
    if(m_menus.contains(menu))
        m = m_menus.value(menu);
    else
        m = addMenu(menu);

    m_menus.insert(menu, m);

    QAction *a;
    if(m_actions.value(menu).contains(action->text())) {
        a = m_actions.value(menu).value(action->text());
    }
    else {
        a = m->addAction(action->icon(), action->text());
        copyActionState(action, a);
        m_actions[menu].insert(action->text(), a);
        connect(a, &QAction::triggered,
                this, &MenuBar::triggerAction);
    }

    m_actionsPerWidget[widget].insert(a, action);
}

QMenu *MenuBar::menu(const QString &name)
{
    return m_menus.value(name);
}

void MenuBar::triggerAction()
{
    QAction *a = static_cast<QAction *>(sender());

    QWidget *w = QApplication::activeWindow();
    if(!w)
        return;

    QPointer<QAction> widgetAction = m_actionsPerWidget.value(w).value(a);
    if(!widgetAction)
        return;

    widgetAction->trigger();
    enableActionsBasedOnCurrentWindow();
}

void MenuBar::enableActionsBasedOnCurrentWindow()
{
    QWidget *focus = QApplication::activeWindow();
    if(!focus)
        return;

    foreach(QWidget *w, m_actionsPerWidget.keys()) {
        foreach(QAction *action, m_actionsPerWidget.value(w).keys()) {
            if(w == focus)
                continue;

            action->setEnabled(false);
        }
    }

    foreach(QAction *action, m_actionsPerWidget.value(focus).keys()) {
        QPointer<QAction> oriAction = m_actionsPerWidget.value(focus).value(action);
        if(oriAction) {
            copyActionState(oriAction, action);
        }
    }
}

void MenuBar::enableGlobalActionsBasedOnCurrentWindow()
{

}

void MenuBar::closeFocusWindow()
{
    QWidget *focus = QApplication::activeWindow();
    if(!focus)
        return;

    focus->close();
}

void MenuBar::showLibrary()
{
    bool hasLibraryWindow = false;
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        if(widget->metaObject()->className() == MainWindow::staticMetaObject.className()) {
            widget->show();
            widget->activateWindow();
            widget->raise();
            widget->setWindowState(Qt::WindowStates(Qt::WindowActive));
            hasLibraryWindow = true;
        }
    }

    if(!hasLibraryWindow) {
        MainWindow *w = new MainWindow;
        w->show();
    }
}

void MenuBar::copyActionState(QAction *src, QAction *dest)
{
    dest->setShortcut(src->shortcut());
    dest->setEnabled(src->isEnabled());
    dest->setCheckable(src->isCheckable());
    dest->setChecked(src->isChecked());
    dest->setText(src->text());
    dest->setIcon(src->icon());
}
