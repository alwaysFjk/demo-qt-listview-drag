#include "dragtableview.h"
#include "ui_dragtableview.h"

#include <QDebug>
#include <QScrollBar>
#include <QGraphicsDropShadowEffect>
#include <QMimeData>
#include <QDrag>
#include <QDesktopWidget>
#include <QTime>


static QString DRAGMiniData = "workAreaDragCurrencyCustomWidgewt";

DragCustomWidget *getIDenoiseWidget(QWidget *widget)
{
    if (!widget)
    {
        return nullptr;
    }
    DragCustomWidget *iWidget = nullptr;
    iWidget = qobject_cast<DragCustomWidget*>(widget);
    if (iWidget)
    {
        return iWidget;
    }

    if (widget->parentWidget())
    {
        return getIDenoiseWidget(widget->parentWidget());
    }
    return nullptr;
}

DragTableView::DragTableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DragTableView)
{
    ui->setupUi(this);
//    this->setMinimumWidth(ITEM_WIDTH);
//    this->setMaximumWidth(ITEM_WIDTH);
    setWindowFlag(Qt::Popup);

    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0, 0);          //设置向哪个方向产生阴影效果(dx,dy)，特别地，(0,0)代表向四周发散
    effect->setColor(Qt::gray);       //设置阴影颜色，也可以setColor(QColor(220,220,220))
    effect->setBlurRadius(20);        //设定阴影的模糊半径，数值越大越模糊
    ui->widget->setGraphicsEffect(effect);
    setAcceptDrops(true);

    m_tmpWidget = new QLabel(this);
    m_tmpWidget->setObjectName("tmpWidget");
    m_tmpWidget->hide();
    m_tmpWidget->setMinimumHeight(ITEM_HEIGHT);
    m_tmpWidget->setMaximumHeight(ITEM_HEIGHT);
    m_tmpWidget->setAlignment(Qt::AlignCenter);

    m_scrollarea = new QScrollArea(this);
    this->layout()->addWidget(m_scrollarea);
    m_scrollarea->setWidgetResizable(true);
    m_scrollarea->setObjectName("myScrollarea");

    m_scrollarea->setWidget(ui->widget);
    m_scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_layout = new  QVBoxLayout(ui->widget);
    m_layout->setObjectName("myLayout");
    m_layout->addWidget(m_tmpWidget);
    QSpacerItem *item = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    m_layout->addItem(item);
    m_layout->setMargin(0);
    m_layout->setSpacing(3);
    this->m_scrollarea->installEventFilter(this);
    m_desIndex = 0;
    m_srcIndex = 0;

}

DragTableView::~DragTableView()
{
    delete ui;
}

void DragTableView::showParent()
{
    QWidget *parentWidget = static_cast<QWidget*>(this->parent());
    if ( parentWidget != nullptr)
    {
        QPoint globalPoint(parentWidget->mapToGlobal(QPoint(0,0)));
        globalPoint.setX( globalPoint.x() + parentWidget->width() - this->width());
        globalPoint.setY( globalPoint.y() + parentWidget->height());
        this->show();
        this->move(globalPoint);
    }
    else
    {
        qWarning() << __LINE__ << __FUNCTION__ << "parent widget is nullptr";
    }
}

DragCustomWidget *DragTableView::addExecuWidget(QString itemWidgetName)
{
    DragCustomWidget *iWidget = new DragCustomWidget(itemWidgetName,this);
    m_layout->insertWidget( m_layout->indexOf(m_tmpWidget), iWidget);
    m_DragCustomWidgetList.append(iWidget);
    return iWidget;
}

void DragTableView::resetList(QStringList coinNameList)
{
//    qDebug() << __LINE__ << __FUNCTION__ << coinNameList;
    // 先清空
    QList<DragCustomWidget*> list = this->findChildren<DragCustomWidget*>();
    foreach(DragCustomWidget* w , list)
    {
        m_layout->removeWidget(w);
        w->deleteLater();
        w = nullptr;
    }
    // 再添加
    foreach(auto v , coinNameList)
    {
        addExecuWidget(v);
    }
}

void DragTableView::dragEnterEvent(QDragEnterEvent *event)
{
    //只支持从本区域
    if (event->mimeData()->hasFormat(DRAGMiniData) )
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void DragTableView::dragLeaveEvent(QDragLeaveEvent *event)
{
    (void)event;
    m_tmpWidget->hide();
}

void DragTableView::dragMoveEvent(QDragMoveEvent *event)
{
    if (!event->mimeData()->hasFormat(DRAGMiniData) )
    {
        qDebug() << __LINE__ << __FUNCTION__ << "not this drag area";
        return;
    }
    QWidget *widget = this->childAt(event->pos());
    DragCustomWidget *iWidget = qobject_cast<DragCustomWidget*>(widget);
    DragCustomWidget *iWidget2 = qobject_cast<DragCustomWidget*>(widget->parent());
    if ( iWidget2)
    {
        iWidget = iWidget2;
    }
    if (iWidget)
    {
        m_desIndex = m_layout->indexOf(iWidget);
    }

    m_layout->insertWidget(m_desIndex , m_tmpWidget);
    m_tmpWidget->show();
    if ( m_moveDragedWidget )
    {
        m_tmpWidget->setPixmap( m_moveDragedWidget->grab());
    }
}

void DragTableView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(DRAGMiniData))
    {
        if (m_moveDragedWidget == nullptr)
        {
            qDebug() << __LINE__ << __FUNCTION__  << QTime::currentTime();
            return;
        }
        m_layout->insertWidget( m_layout->indexOf(m_tmpWidget),
                                m_moveDragedWidget);
        m_srcIndex = m_layout->indexOf(m_tmpWidget);

        if(m_srcIndex >= m_desIndex && m_srcIndex>=m_DragCustomWidgetList.size()-1 )
        {
            m_tmpWidget->hide();
            return;
        }
        if(m_srcIndex==0 && m_desIndex==1)
        {
            m_tmpWidget->hide();
            return;
        }
    }
    else
    {
        qDebug() << __LINE__ << __FUNCTION__  << QTime::currentTime();
    }
    //在放下之后隐藏临时界面。
    m_tmpWidget->hide();
    //执行位置索引位置初始化，避免出现索引位置异常
    m_desIndex=0;
    m_srcIndex=0;
    // 记录当前的coinInfo 的顺序
    updataCoinInfoIndex();
}

void DragTableView::mousePressEvent(QMouseEvent *event)
{
    auto scrollBar = m_scrollarea->horizontalScrollBar();
    if (event->button() == Qt::LeftButton)
    {
        scrollBar = m_scrollarea->horizontalScrollBar();
        scrollBar->setSingleStep(200);
        dragPosition = event->pos();
        m_IsPress = true;
    }
    if(event->button() == Qt::RightButton)
    {
        dragPosition = event->pos();
    }
    return QWidget::mousePressEvent(event);
}

void DragTableView::mouseMoveEvent(QMouseEvent *event)
{
    //在鼠标Press的时候，记录起始点为dragPosition（此为成员变量)；
    //在Move的时候，检测与起始点的距离，当拖动一定距离的时候，认为开启了一个拖动操作。
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragPosition).manhattanLength()  < QApplication::startDragDistance())
    {
        qDebug() << __LINE__ << __FUNCTION__ ;
        return;
    }
    if ( !m_IsPress)
    {
        return;
    }
    QWidget *widget = this->childAt(event->pos());
    if (!widget || widget == m_tmpWidget)
    {
        qDebug() << __LINE__ << __FUNCTION__  << QTime::currentTime();
        return;
    }
    DragCustomWidget *iWidget = getIDenoiseWidget(widget);
    if (!iWidget)
    {
        qDebug() << __LINE__ << __FUNCTION__  << QTime::currentTime();
        return;
    }
    int index=0;
    if (iWidget)
    {
        index = m_layout->indexOf(iWidget);
    }
    m_moveDragedWidget = iWidget;
    m_moveDragedWidget->hide();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << 1;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(DRAGMiniData, itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(m_moveDragedWidget->grab());
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect desktopRect = desktopWidget->screenGeometry(this); //返回包含当前widget的QRect
    int screen_width = desktopRect.width();
    //    int screen_height = desktopRect.height();
    double xfactor = (double)event->pos().x()/screen_width;
    double yfactor = (double)event->pos().y()/screen_width;
    QPoint point;
    point.setX(xfactor*m_moveDragedWidget->geometry().width());
    point.setY(yfactor*m_moveDragedWidget->geometry().height()*3);

    drag->setHotSpot(point);
    if (drag->exec(Qt::MoveAction) != Qt::MoveAction)
    {
        if ( m_moveDragedWidget )
        {
            m_moveDragedWidget->show();
        }
    }
    m_IsPress = false;
    m_moveDragedWidget = nullptr;
    return QWidget::mouseMoveEvent(event);
}


void DragTableView::hideEvent(QHideEvent *event)
{
    (void)event;
}

void DragTableView::updataCoinInfoIndex()
{
    for(int i=0;i <m_layout->count(); i++)
    {
    }
}
