//
// Created by GBT B450M-S2H on 14.04.2023.
//

#ifndef PG2PRAKTIKUM1_BOOKING_H
#define PG2PRAKTIKUM1_BOOKING_H

#include <iostream>
#include <QDate>


class Booking {
protected:
    std::string id;
    double price;
    std::string fromDate;
    std::string toDate;
    long travelId;
    std::string predecessor1, predecessor2;

public:
    Booking(std::string _id, double _price, std::string _fromDate, std::string _toDate, long _travelId, std::string predecessor1, std::string predecessor2);

    virtual ~Booking();

    virtual std::string showDetails() = 0;

    std::string &getId();

    const std::vector<std::string> &getPredecessors() const;

    double getPrice() const;

    QDate getFromDate() const;

    QDate getToDate() const;

    long getTravelId() const;

    void setId(const std::string &id);

    void setPrice(double price);

    void setFromDate(const std::string &fromDate);

    void setToDate(const std::string &toDate);

    void setTravelId(long travelId);

    const std::string &getPredecessor1() const;

    const std::string &getPredecessor2() const;

};


#endif //PG2PRAKTIKUM1_BOOKING_H
