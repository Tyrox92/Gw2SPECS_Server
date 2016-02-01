#include "damagelist.h"
#include "ui_damagelist.h"

damageList::damageList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::damageList)
{
    ui->setupUi(this);
}

damageList::~damageList()
{
    delete ui;
}


void damageList::changeName()
{
 ui->u1->text() = "Peter";
}
