//
// Created by GBT B450M-S2H on 06.07.2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TravelCheck.h" resolved

#include "travelcheck.h"
#include "ui_TravelCheck.h"


TravelCheck::TravelCheck(std::shared_ptr<TravelAgency> travelAgency, QWidget *parent) :
        travelAgency(travelAgency), QMainWindow(parent), ui(new Ui::TravelCheck) {
    ui->setupUi(this);
    this->show();

    icons[false] = QIcon("../Images/thumbDown.png");
    icons[true] = QIcon("../Images/thumbUp.png");
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setColumnWidth(0, 100);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 150);
    ui->tableWidget->setColumnWidth(3, 150);
    ui->tableWidget->setColumnWidth(4, 150);


    checkTravels();



}

TravelCheck::~TravelCheck() {
    delete ui;
}

void TravelCheck::checkTravels() {
    //ui->tableWidget->clear();
    int currentRow = 0;
    for(auto t : travelAgency->getAllTravels()){
        ui->tableWidget->setRowCount(currentRow + 1);
        QTableWidgetItem* imageItem = new QTableWidgetItem();
        t->topologicalSort();
        ui->tableWidget->setItem(currentRow, 0, new QTableWidgetItem(QString::number(t->getId())));
        imageItem->setIcon(icons[t->checkEnoughHotels()]);
        ui->tableWidget->setItem(currentRow, 1, imageItem);
        imageItem = new QTableWidgetItem();
        imageItem->setIcon(icons[t->checkNoUselessHotels()]);
        ui->tableWidget->setItem(currentRow, 2, imageItem);
        imageItem = new QTableWidgetItem();
        imageItem->setIcon(icons[t->checkNoUselessRentalCars()]);
        ui->tableWidget->setItem(currentRow, 3, imageItem);
        imageItem = new QTableWidgetItem();
        imageItem->setIcon(icons[t->checkRoundTrip()]);
        ui->tableWidget->setItem(currentRow, 4, imageItem);


        currentRow = ui->tableWidget->rowCount();

    }
}
