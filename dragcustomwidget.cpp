#include "dragcustomwidget.h"
#include "dragtableview.h"

DragCustomWidget::DragCustomWidget(QString name, QWidget *parent) : QFrame(parent)
{
    m_layout = new QHBoxLayout(this);
    m_pTextLabel = new QLabel(this);
    m_pcheckBox = new QCheckBox(this);

    m_layout->addWidget(m_pTextLabel);
    m_layout->addWidget(m_pcheckBox);
    m_layout->setMargin(0);
    m_layout->setSpacing(6);

    this->setMaximumHeight(ITEM_HEIGHT);
    this->setMinimumHeight(ITEM_HEIGHT);
    this->setMinimumWidth(ITEM_WIDTH);
    this->setMaximumWidth(ITEM_WIDTH);
    setName(name);
}

QString DragCustomWidget::name()
{
    return m_name;
}

void DragCustomWidget::setName(QString name)
{
    m_name = name;
    m_pTextLabel->setText(m_name);
}

void DragCustomWidget::slot_chenckBox_clicked(bool isChecked)
{

}
