#include "noisygradientitemdelegate.h"

#include <QPainter>
#include <QAbstractItemView>
#include <QApplication>
#include <QMouseEvent>

// Text
const QPoint NoisyGradientItemDelegate::TITLE_OFFSET_SHADOW(0, -1); // relative to title offset
QColor NoisyGradientItemDelegate::COLOR_TITLE_NORMAL(75,77,80);
QColor NoisyGradientItemDelegate::COLOR_TITLE_NORMAL_SHADOW(222,222,222);
const QColor NoisyGradientItemDelegate::COLOR_TITLE_SELECTED(231,240,249);
const QColor NoisyGradientItemDelegate::COLOR_TITLE_SELECTED_SHADOW(45,130,201);
const QColor NoisyGradientItemDelegate::COLOR_TEXT_NORMAL(148,149,151);
const QColor NoisyGradientItemDelegate::COLOR_TEXT_SELECTED(231,240,249);
const bool NoisyGradientItemDelegate::TITLE_BOLD = true;
const int NoisyGradientItemDelegate::TEXT_FOOT_DISTANCE(5);
const int NoisyGradientItemDelegate::TEXT_RIGHT_DISTANCE(5);

// Background
const QColor NoisyGradientItemDelegate::COLOR_LINE_TOP1_NORMAL(255,255,255,0);
const QColor NoisyGradientItemDelegate::COLOR_LINE_TOP2_NORMAL(255,255,255,0);
const QColor NoisyGradientItemDelegate::COLOR_LINE_BOTTOM_NORMAL(210,210,210);
QColor NoisyGradientItemDelegate::COLOR_GRADIENT_TOP_NORMAL(255,255,255,0);
QColor NoisyGradientItemDelegate::COLOR_GRADIENT_BOTTOM_NORMAL(255,255,255,0);

const QColor NoisyGradientItemDelegate::COLOR_LINE_TOP1_SELECTED(79,154,221);
const QColor NoisyGradientItemDelegate::COLOR_LINE_TOP2_SELECTED(138,185,231);
const QColor NoisyGradientItemDelegate::COLOR_LINE_BOTTOM_SELECTED(34,101,157);
const QColor NoisyGradientItemDelegate::COLOR_GRADIENT_TOP_SELECTED(111,171,226);
const QColor NoisyGradientItemDelegate::COLOR_GRADIENT_BOTTOM_SELECTED(37,125,199);

static QRect CheckBoxRect(const QStyleOptionViewItem &view_item_style_options, const QPoint &offset)
{
    QStyleOptionButton check_box_style_option;
    QRect check_box_rect = QApplication::style()->subElementRect(
                QStyle::SE_CheckBoxIndicator,
                &check_box_style_option);
    QPoint check_box_point(view_item_style_options.rect.x() + offset.x(),
                           view_item_style_options.rect.y() + offset.y());
    return QRect(check_box_point, check_box_rect.size());
}

NoisyGradientItemDelegate::NoisyGradientItemDelegate(QAbstractItemView *view, QObject *parent) :
    QStyledItemDelegate(parent),
    m_view(view),
    m_checkBoxOffset(8,8)
{
    connect(m_view->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &NoisyGradientItemDelegate::repaintItemsWhenSelectionChanges);
}

void NoisyGradientItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    static const QPixmap NOISY_TEXTURE(":/general/noisy-texture-200x200");
    QColor COLOR_LINE_TOP1 = COLOR_LINE_TOP1_NORMAL;
    QColor COLOR_LINE_TOP2 = COLOR_LINE_TOP2_NORMAL;
    QColor COLOR_LINE_BOTTOM = COLOR_LINE_BOTTOM_NORMAL;
    QColor COLOR_GRADIENT_TOP = COLOR_GRADIENT_TOP_NORMAL;
    QColor COLOR_GRADIENT_BOTTOM = COLOR_GRADIENT_BOTTOM_NORMAL;

    QColor COLOR_TITLE = COLOR_TITLE_NORMAL;
    QColor COLOR_TITLE_SHADOW = COLOR_TITLE_NORMAL_SHADOW;

    QColor COLOR_TEXT = COLOR_TEXT_NORMAL;

    if (option.state & QStyle::State_Selected) {
        COLOR_LINE_TOP1 = COLOR_LINE_TOP1_SELECTED;
        COLOR_LINE_TOP2 = COLOR_LINE_TOP2_SELECTED;
        COLOR_LINE_BOTTOM = COLOR_LINE_BOTTOM_SELECTED;
        COLOR_GRADIENT_TOP = COLOR_GRADIENT_TOP_SELECTED;
        COLOR_GRADIENT_BOTTOM = COLOR_GRADIENT_BOTTOM_SELECTED;

        COLOR_TITLE = COLOR_TITLE_SELECTED;
        COLOR_TITLE_SHADOW = COLOR_TITLE_SELECTED_SHADOW;

        COLOR_TEXT = COLOR_TEXT_SELECTED;
    }

    painter->save();

    // Top lines
    QRect topLineRect = option.rect;
    topLineRect.adjust(0,-1, 0, -1);
    painter->setPen(COLOR_LINE_TOP1);
    painter->drawLine(topLineRect.topLeft(), topLineRect.topRight());

    painter->setPen(COLOR_LINE_TOP2);
    painter->drawLine(option.rect.topLeft(), option.rect.topRight());

    // Bottom line
    painter->setPen(COLOR_LINE_BOTTOM);
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    // Gradient
    QRect gradientRect = option.rect;
    gradientRect.adjust(0,1,0,-1);

    QLinearGradient gradient(option.rect.topLeft(), option.rect.bottomLeft());
    gradient.setColorAt(0, COLOR_GRADIENT_TOP);
    gradient.setColorAt(1, COLOR_GRADIENT_BOTTOM);

    QBrush gradientBrush(gradient);
    painter->setBrush(gradientBrush);
    painter->setPen(Qt::NoPen);
    painter->drawRect(gradientRect);

    painter->restore();

    if (option.state & QStyle::State_Selected) {
        painter->drawTiledPixmap(option.rect, NOISY_TEXTURE);
    }
}


QRectF NoisyGradientItemDelegate::drawTitleText(QPainter *painter,
                                                const QStyleOptionViewItem &option,
                                                const QString &text,
                                                const QPoint &offset) const
{
    return drawTitleText(painter, option.rect.adjusted(offset.x(),
                                                       offset.y(),
                                                       -TEXT_RIGHT_DISTANCE,
                                                       -TEXT_FOOT_DISTANCE),
                         Qt::AlignLeft,
                         text,
                         option);
}

QRectF NoisyGradientItemDelegate::drawTitleText(QPainter *painter, const QRect rect, int flags, const QString &text, const QStyleOptionViewItem &option) const
{
    QColor COLOR_TITLE = COLOR_TITLE_NORMAL;
    QColor COLOR_TITLE_SHADOW = COLOR_TITLE_NORMAL_SHADOW;

    if (option.state & QStyle::State_Selected) {
        COLOR_TITLE = COLOR_TITLE_SELECTED;
        COLOR_TITLE_SHADOW = COLOR_TITLE_SELECTED_SHADOW;
    }

    painter->save();

    QFont font = painter->font();
    font.setBold(TITLE_BOLD);
    painter->setFont(font);


    // Shadow title
    QRectF shadowBoundingRect;
    painter->setPen(COLOR_TITLE_SHADOW);
    painter->drawText(rect.adjusted(TITLE_OFFSET_SHADOW.x(),
                                    TITLE_OFFSET_SHADOW.y(),
                                    0,0),
                      flags,
                      text,
                      &shadowBoundingRect);

    // Foreground title
    QRectF titleBoundingRect;
    painter->setPen(COLOR_TITLE);
    painter->drawText(rect,
                      flags,
                      text,
                      &titleBoundingRect);
    painter->restore();

    return shadowBoundingRect.united(titleBoundingRect);
}

QRectF NoisyGradientItemDelegate::drawText(QPainter *painter, const QRect rect, int flags, const QString &text, const QStyleOptionViewItem &option, const QColor textColor) const
{
    QColor COLOR_TEXT = COLOR_TEXT_NORMAL;
    if(textColor.isValid()) {
        COLOR_TEXT = textColor;
    }

    if (option.state & QStyle::State_Selected) {
        COLOR_TEXT = COLOR_TEXT_SELECTED;
    }
    painter->save();

    painter->setPen(COLOR_TEXT);

    QRectF boundingRect;
    QFont font = painter->font();
    font.setBold(false);
    painter->setFont(font);
    painter->drawText(rect,
                      flags,
                      text,
                      &boundingRect);
    painter->restore();
    return boundingRect;
}

QRectF NoisyGradientItemDelegate::drawText(QPainter *painter,
                                           const QStyleOptionViewItem &option,
                                           const QString &text,
                                           const QPoint &offset,
                                           int alignment) const
{
    return drawText(painter, option.rect.adjusted(offset.x(), offset.y(),-TEXT_RIGHT_DISTANCE,
                                                  -TEXT_FOOT_DISTANCE),
                    alignment, text, option);
}

QSize NoisyGradientItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(0,18);

    /* Returns extremly large sizes for cells with pixmaps in it
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    if (m_view->selectionModel()->selectedIndexes().contains(index)) {
        size.setHeight(size.height() + 1);
    }

    return size;
    */
}

void NoisyGradientItemDelegate::setCheckBoxOffset(const QPoint &offset)
{
    m_checkBoxOffset = offset;
}

void NoisyGradientItemDelegate::repaintItemsWhenSelectionChanges(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);

    foreach (QModelIndex index, deselected.indexes()) {
        if (index.row() == 0)
            continue;

        m_view->update(m_view->model()->index(index.row() - 1, 0));
    }

    QSize size = m_view->size();
    size.setHeight(size.height() + 1);
    m_view->resize(size);
    size.setHeight(size.height() - 1);
    m_view->resize(size);
}

QAbstractItemView *NoisyGradientItemDelegate::view() const
{
    return m_view;
}

void NoisyGradientItemDelegate::drawCheckBox(QPainter *painter,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{
    QStyleOptionButton buttonOption;
    buttonOption.rect = CheckBoxRect(option, m_checkBoxOffset);
    buttonOption.state = QStyle::State_Enabled | QStyle::State_Active;
    Qt::CheckState checkState = static_cast<Qt::CheckState>(index.data(Qt::CheckStateRole).toInt());
    if (checkState == Qt::PartiallyChecked) {
        buttonOption.state |= QStyle::State_NoChange; // represents "partially"
    }
    else if (checkState == Qt::Checked) {
        buttonOption.state |= QStyle::State_On;
    }
    else if (checkState == Qt::Unchecked) {
        buttonOption.state |= QStyle::State_Off;
    }

    QApplication::style()->drawControl(QStyle::CE_CheckBox, &buttonOption, painter, NULL);
}

bool NoisyGradientItemDelegate::editorEvent(QEvent *event,
                                            QAbstractItemModel *model,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index)
{
    if ((event->type() == QEvent::MouseButtonRelease)
            || (event->type() == QEvent::MouseButtonDblClick)) {
        QMouseEvent *mouse_event = static_cast<QMouseEvent*>(event);
        if (mouse_event->button() != Qt::LeftButton ||
                !CheckBoxRect(option, m_checkBoxOffset).contains(mouse_event->pos())) {
            return false;
        }
        if (event->type() == QEvent::MouseButtonDblClick) {
            return true;
        }
    } else if (event->type() == QEvent::KeyPress) {
        if (static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space &&
                static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select) {
            return false;
        }
    } else {
        return false;
    }

    Qt::CheckState checkState = static_cast<Qt::CheckState>(index.data(Qt::CheckStateRole).toInt());
    if (checkState == Qt::PartiallyChecked) {
        checkState = Qt::Checked;
    }
    else if (checkState == Qt::Checked) {
        checkState = Qt::Unchecked;
    }
    else if (checkState == Qt::Unchecked) {
        checkState = Qt::PartiallyChecked;
    }
    return model->setData(index, checkState, Qt::CheckStateRole);
}

void NoisyGradientItemDelegate::drawPixmap(QPainter *painter, const QStyleOptionViewItem &option,const QPixmap &pixmap, const QPoint &offset) const
{
    QRect rect(option.rect.adjusted(offset.x(), offset.y(),
                                    offset.x(), offset.y()).topLeft(),
               pixmap.size());
    painter->drawPixmap(rect,
                        pixmap);
}
