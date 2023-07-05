//
// Created by GBT B450M-S2H on 07.05.2023.
//

#include "Travel.h"
#include "FlightBooking.h"

Travel::Travel(long _id, long _customerId) : id(_id), customerId(_customerId) {
    graph = new Graph<std::shared_ptr<Booking>, 10>;
}

void Travel::addBooking(std::shared_ptr<Booking> booking) {
    travelBookings.push_back(booking);
    graph->insertVertex(travelBookings.size(), booking);
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

const std::vector<std::shared_ptr<Booking>> &Travel::getTravelBookings() const {
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
        if(!graph->getVertexValue(i)->getPredecessors()[0].empty()){
            pre1Id = graph->getVertexValue(i)->getPredecessors()[0];
            for(int j = 0; j < travelBookings.size(); j++){
                if(pre1Id == travelBookings[j]->getId()){
                    pre1 = j;
                    break;
                }
            }
            graph->insertEdge(pre1, i);
        }
        if(!graph->getVertexValue(i)->getPredecessors()[1].empty()){
            pre2Id = graph->getVertexValue(i)->getPredecessors()[1];
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

}
