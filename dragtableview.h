#ifndef DRAGTABLEVIEW_H
#define DRAGTABLEVIEW_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QPoint>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QHideEvent>

#include "dragcustomwidget.h"

#define ITEM_HEIGHT 50
#define ITEM_WIDTH 240

namespace Ui {
class DragTableView;
}

class DragTableView : public QWidget
{
    Q_OBJECT

public:
    explicit DragTableView(QWidget *parent = nullptr);
    ~DragTableView();

    void showParent();

    // 根据对象名字 添加widget
    DragCustomWidget* addExecuWidget(QString itemWidgetName);
    void resetList(QStringList coinNameList);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;


    void hideEvent(QHideEvent *event) override;

    void updataCoinInfoIndex();
private:
    Ui::DragTableView *ui;
    QScrollArea *m_scrollarea = nullptr;
    QVBoxLayout *m_layout = nullptr;
    QLabel *m_tmpWidget = nullptr;

    // brief desIndex 记录执行模块拖动后的位置
    int m_desIndex;
    // brief srcIndex 记录执行模块拖动前的位置
    int m_srcIndex;
    // brief dragPosition 记录按下时的位置
    QPoint dragPosition;
    bool m_IsPress = false;
    // m_moveDragedWidget 在execute界面中，改变模块之间的顺序时，临时记录拖动的界面。
    DragCustomWidget * m_moveDragedWidget;
    // m_denoiseWidgets 记录所有的执行模块，和显示顺序无关。
    QList <DragCustomWidget*>m_DragCustomWidgetList;
};

#endif // DRAGTABLEVIEW_H
