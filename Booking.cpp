//
// Created by GBT B450M-S2H on 14.04.2023.
//

#include "Booking.h"

Booking::Booking(std::string _id, double _price, std::string _fromDate, std::string _toDate, long _travelId, std::string predecessor1, std::string predecessor2) : id(_id),
                                                                                                               price(_price),
                                                                                                               fromDate(
                                                                                                                       _fromDate),
                                                                                                               toDate(_toDate),
                                                                                                               travelId(
                                                                                                                       _travelId), predecessor1(predecessor1), predecessor2(predecessor2) {
}

Booking::~Booking() {
    std::cout << "Booking object destroyed" << std::endl;
}

std::string &Booking::getId() {
    return id;
}

double Booking::getPrice() const {
    return price;
}

QDate Booking::getFromDate() const {
    return QDate::fromString(QString::fromStdString(fromDate), "yyyyMMdd");
}

QDate Booking::getToDate() const {
    return QDate::fromString(QString::fromStdString(toDate), "yyyyMMdd");
}

long Booking::getTravelId() const {
    return travelId;
}

void Booking::setId(const std::string &id) {
    Booking::id = id;
}

void Booking::setPrice(double price) {
    Booking::price = price;
}

void Booking::setFromDate(const std::string &fromDate) {
    Booking::fromDate = fromDate;
}

void Booking::setToDate(const std::string &toDate) {
    Booking::toDate = toDate;
}

void Booking::setTravelId(long travelId) {
    Booking::travelId = travelId;
}

const std::string &Booking::getPredecessor1() const {
    return predecessor1;
}

const std::string &Booking::getPredecessor2() const {
    return predecessor2;
}





