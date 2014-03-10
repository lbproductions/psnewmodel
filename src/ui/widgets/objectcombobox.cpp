#include "objectcombobox.h"

ObjectComboBoxBase::ObjectComboBoxBase(QWidget *parent):
    QComboBox(parent)
{
    connect(this, SIGNAL(currentIndexChanged(int)), this, SIGNAL(currentObjectChanged()));
}
