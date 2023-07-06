//
// Created by GBT B450M-S2H on 06.07.2023.
//

#ifndef PG2PRAKTIKUM5_TRAVELCHECK_H
#define PG2PRAKTIKUM5_TRAVELCHECK_H

#include <QMainWindow>
#include "TravelAgency.h"


QT_BEGIN_NAMESPACE
namespace Ui { class TravelCheck; }
QT_END_NAMESPACE

class TravelCheck : public QMainWindow {
Q_OBJECT

public:
    explicit TravelCheck(std::shared_ptr<TravelAgency> travelAgency,QWidget *parent = nullptr);

    ~TravelCheck() override;
    void checkTravels();

private:
    Ui::TravelCheck *ui;
    std::shared_ptr<TravelAgency> travelAgency;
    std::map<bool, QIcon> icons;
};


#endif //PG2PRAKTIKUM5_TRAVELCHECK_H
