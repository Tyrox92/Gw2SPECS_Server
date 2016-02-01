#ifndef DAMAGELIST_H
#define DAMAGELIST_H

#include <QWidget>

namespace Ui {
class damageList;
}

class damageList : public QWidget
{
    Q_OBJECT

public:
    explicit damageList(QWidget *parent = 0);
    ~damageList();
    void changeName();

private:
    Ui::damageList *ui;
};

#endif // DAMAGELIST_H
