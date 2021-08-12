#ifndef DRAGCUSTOMWIDGEWT_H
#define DRAGCUSTOMWIDGEWT_H

#include <QFrame>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>

class ULabel;
class DragCustomWidget : public QFrame
{
    Q_OBJECT
public:
    DragCustomWidget(QString name = "" ,QWidget *parent = nullptr);

    QString name();
    void setName(QString name);

private:
    void slot_chenckBox_clicked(bool isChecked);

private:
    QString m_name;


    QHBoxLayout *m_layout;
    QLabel *m_pTextLabel;
    QCheckBox *m_pcheckBox;
};

#endif // DRAGCUSTOMWIDGEWT_H
