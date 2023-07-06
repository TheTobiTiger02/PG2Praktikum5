//
// Created by GBT B450M-S2H on 07.05.2023.
//

#include "Travel.h"
#include "FlightBooking.h"
#include "RentalCarReservation.h"
#include "HotelBooking.h"

Travel::Travel(long _id, long _customerId) : id(_id), customerId(_customerId) {
    graph = new Graph<std::shared_ptr<Booking>, 20>;
}

void Travel::addBooking(std::shared_ptr<Booking> booking) {
    travelBookings.push_back(booking);
    graph->insertVertex(travelBookings.size() - 1, booking);
    /*if(!graph->getVertexValue(1)->getPredecessors()[0].empty()){
        std::string test = graph->getVertexValue(1)->getId();
        std::cout << test;
    }
     */
}

long Travel::getId() const {
    return id;
}

long Travel::getCustomerId() const {
    return customerId;
}

std::vector<std::shared_ptr<Booking>> Travel::getTravelBookings() {
    return travelBookings;
}

QDate Travel::getStartDate() {

    QDate startDate = travelBookings.at(0)->getFromDate();
    for (auto b: travelBookings) {
        QDate newDate = b->getFromDate();
        if (newDate < startDate) {
            startDate = newDate;
        }
    }
    return startDate;
}

QDate Travel::getEndDate() {
    QDate endDate = travelBookings.at(0)->getToDate();
    for (auto b: travelBookings) {
        QDate newDate = b->getToDate();
        if (newDate > endDate) {
            endDate = newDate;
        }
    }
    return endDate;
}

void Travel::topologicalSort() {

    struct VertexData {
        int vertexNumber;
        std::shared_ptr<Booking> vertexValue;
        int endTime;
    };


    std::string pre1Id = "";
    std::string pre2Id = "";
    int pre1 = -1;
    int pre2 = -1;

    for(int i = 0; i < travelBookings.size(); i++){
        if(!graph->getVertexValue(i)->getPredecessor1().empty()){
            pre1Id = graph->getVertexValue(i)->getPredecessor1();
            for(int j = 0; j < travelBookings.size(); j++){
                if(pre1Id == travelBookings[j]->getId()){
                    pre1 = j;
                    break;
                }
            }
            graph->insertEdge(pre1, i);
        }
        if(!graph->getVertexValue(i)->getPredecessor2().empty()){
            pre2Id = graph->getVertexValue(i)->getPredecessor2();
            for(int j = 0; j < travelBookings.size(); j++){
                if(pre2Id == travelBookings[j]->getId()){
                    pre2 = j;
                    break;
                }
            }
            graph->insertEdge(pre2, i);
        }

    }

    DepthFirstSearch(*graph);

    std::vector<VertexData> toBeSorted;
    VertexData vertexData;

    for(int i = 0; i < travelBookings.size(); i++){
        vertexData.vertexNumber = i;
        vertexData.vertexValue = graph->getVertexValue(i);
        vertexData.endTime = graph->getEnd(i);

        toBeSorted.push_back(vertexData);
    }

    std::sort(toBeSorted.begin(), toBeSorted.end(), [](const VertexData &a, const VertexData &b){
        return a.endTime > b.endTime;
    });

    travelBookings.clear();
    for(auto v : toBeSorted){
        travelBookings.push_back(v.vertexValue);
    }
}

bool Travel::checkRoundTrip() {
    if(std::shared_ptr<FlightBooking> startFlight = dynamic_pointer_cast<FlightBooking>(travelBookings.front())){
        if(std::shared_ptr<FlightBooking> endFlight = dynamic_pointer_cast<FlightBooking>(travelBookings.back())){
            if(startFlight->getFromDestination() == endFlight->getToDestination()){
                return true;
            }
        }
    }
    return false;
}

bool Travel::checkEnoughHotels() {
    QDate currentDate;
    for(int i = 0; i < travelBookings.size(); i++){
        if(std::shared_ptr<RentalCarReservation> rentalCarReservation = dynamic_pointer_cast<RentalCarReservation>(travelBookings[i])){
            continue;
        }
        currentDate = travelBookings[i]->getToDate();

        if(i + 1 < travelBookings.size()){
            if(std::shared_ptr<RentalCarReservation> rentalCarReservation = dynamic_pointer_cast<RentalCarReservation>(travelBookings[i + 1])){
                continue;
            }
            if(travelBookings[i + 1]->getFromDate() > currentDate){
                return false;
            }
        }

    }
    return true;

}

bool Travel::checkNoUselessHotels() {
    QDate currentStartDate, currentEndDate;
    for(int i = travelBookings.size(); i > 0; i--){
        if(std::shared_ptr<RentalCarReservation> rentalCarReservation = dynamic_pointer_cast<RentalCarReservation>(travelBookings[i])){
            continue;
        }
        std::shared_ptr<Booking> currentBooking = travelBookings[i];
        if(!currentBooking->getPredecessor1().empty()){
            if(std::shared_ptr<RentalCarReservation> rentalCarReservation = dynamic_pointer_cast<RentalCarReservation>(findBooking(currentBooking->getPredecessor1()))){

            }
            else{
                if(findBooking(currentBooking->getPredecessor1())->getFromDate() < currentBooking->getFromDate()){
                    return false;
                }
            }
        }
        if(!currentBooking->getPredecessor2().empty()){
            if(std::shared_ptr<RentalCarReservation> rentalCarReservation = dynamic_pointer_cast<RentalCarReservation>(findBooking(currentBooking->getPredecessor2()))){

            }
            else{
                if(findBooking(currentBooking->getPredecessor2())->getFromDate() < currentBooking->getFromDate()){
                    return false;
                }
            }
        }
        /*currentStartDate = travelBookings[i]->getFromDate();
        currentEndDate = travelBookings[i]->getToDate();

        if(i - 1 > travelBookings.size()){
            if(std::shared_ptr<RentalCarReservation> rentalCarReservation = dynamic_pointer_cast<RentalCarReservation>(travelBookings[i + 1])){
                continue;
            }
            if(currentStartDate > travelBookings[i + 1]->getFromDate()){
                return false;
            }
            if(currentEndDate > travelBookings[i + 1]->getToDate()){
                return false;
            }

        }
         */


    }
    return true;
}

bool Travel::checkNoUselessRentalCars() {
    QDate currentStartDate, currentEndDate;
    for(int i = 0; i < travelBookings.size(); i++){
        if(std::shared_ptr<HotelBooking> hotelBooking = dynamic_pointer_cast<HotelBooking>(travelBookings[i])){
            continue;
        }
        currentStartDate = travelBookings[i]->getFromDate();
        currentEndDate = travelBookings[i]->getToDate();

        if(i + 1 < travelBookings.size()){
            if(std::shared_ptr<HotelBooking> hotelBooking = dynamic_pointer_cast<HotelBooking>(travelBookings[i + 1])){
                continue;
            }
            if(currentStartDate > travelBookings[i + 1]->getFromDate()){
                return false;
            }
            if(currentEndDate > travelBookings[i + 1]->getToDate()){
                return false;
            }

        }


    }
    return true;
}

std::shared_ptr<Booking> Travel::findBooking(std::string id) {
    for(auto b : travelBookings){
        if(b->getId() == id){
            return b;
        }
    }
    return nullptr;
}
