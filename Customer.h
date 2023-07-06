//
// Created by GBT B450M-S2H on 07.05.2023.
//

#ifndef PG2PRAKTIKUM3_CUSTOMER_H
#define PG2PRAKTIKUM3_CUSTOMER_H


#include <string>
#include <vector>
#include "Travel.h"

class Customer {

    long id;
    std::string name;
    std::vector<std::shared_ptr<Travel>> travelList;
    double totalBookingPrice{};

public:
    Customer(long _id, std::string _name);

    void addTravel(std::shared_ptr<Travel> travel);

    long getId() const;

    const std::string &getName() const;

    std::vector<std::shared_ptr<Travel>> getTravelList() ;

    double getTotalBookingPrice() const;

    void setTotalBookingPrice(double totalBookingPrice);
};


#endif //PG2PRAKTIKUM3_CUSTOMER_H
