#ifndef BUBBLEDIALOG_H
#define BUBBLEDIALOG_H

#include <QDialog>

namespace Ui {
class BubbleDialog;
}

class BubbleDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit BubbleDialog(QWidget *parent = 0);
    ~BubbleDialog();
    
private:
    Ui::BubbleDialog *ui;
};

#endif // BUBBLEDIALOG_H
