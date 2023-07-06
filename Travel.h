//
// Created by GBT B450M-S2H on 07.05.2023.
//



#ifndef PG2PRAKTIKUM3_TRAVEL_H
#define PG2PRAKTIKUM3_TRAVEL_H

#include <vector>
#include <QDate>
#include "Booking.h"
#include "graph.h"
#include "algorithmen.cpp"


class Travel {

private:
    long id;
    long customerId;
    std::vector<std::shared_ptr<Booking>> travelBookings;
    Graph<std::shared_ptr<Booking>, 20>* graph;

public:
    Travel(long _id, long _customerId);

    void addBooking(std::shared_ptr<Booking> booking);

    long getId() const;

    long getCustomerId() const;

    std::vector<std::shared_ptr<Booking>> getTravelBookings();

    QDate getStartDate();

    QDate getEndDate();

    void topologicalSort();

    bool checkRoundTrip();
    bool checkEnoughHotels();
    bool checkNoUselessHotels();
    bool checkNoUselessRentalCars();

};


#endif //PG2PRAKTIKUM3_TRAVEL_H
