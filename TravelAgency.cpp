//
// Created by GBT B450M-S2H on 14.04.2023.
//

#include "TravelAgency.h"
#include "json.hpp"
#include <fstream>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QCoreApplication>
#include <QObject>
#include <QFileDialog>

using namespace std;
using json = nlohmann::json;

TravelAgency::TravelAgency() {
    bookingClasses["Y"] = "Economy class";
    bookingClasses["W"] = "Premium Economy class";
    bookingClasses["J"] = "Business class";
    bookingClasses["F"] = "First class";

    roomTypes["EZ"] = "Einzelzimmer";
    roomTypes["DZ"] = "Doppelzimmer";
    roomTypes["AP"] = "Appartment";
    roomTypes["SU"] = "Suite";
}
TravelAgency::~TravelAgency() {

}

void
TravelAgency::readFile(string filePath, int startRow, int flightCount, int hotelCount, int carCount, int travelCount,
                       int customerCount, double totalPrice) {
    ifstream file(filePath);
    msgBox = new QMessageBox;
    //static QMessageBox msgBox;

    //msgBox = new QMessageBox();
    if (!file) {

        msgBox->setWindowTitle("Fehler beim Einlesen der Buchungen");
        msgBox->setText("Datei konnte nicht gelesen werden");
        msgBox->exec();
        return;
    }
    json data;
    file >> data;
    string type, id, fromDate, toDate;
    double price;
    long travelId;

    shared_ptr<Booking> booking;
    for (int i = startRow; i < data.size(); i++) {

        try {
            if (data.at(i)["type"].is_null() || data.at(i)["type"].empty() ||
                data.at(i)["type"].get<string>().length() == 0) {
                throw runtime_error("Leeres Attribut 'type' in Zeile " + to_string(i + 1));
            }
            if (data.at(i)["id"].is_null() || data.at(i)["id"].empty() ||
                data.at(i)["id"].get<string>().length() == 0) {
                throw runtime_error("Leeres Attribut 'id' in Zeile " + to_string(i + 1));
            }
            if (data.at(i)["fromDate"].is_null() || data.at(i)["fromDate"].empty() ||
                data.at(i)["fromDate"].get<string>().length() == 0) {
                throw runtime_error("Leeres Attribut 'fromDate' in Zeile " + to_string(i + 1));
            }
            if (data.at(i)["toDate"].is_null() || data.at(i)["toDate"].empty() ||
                data.at(i)["toDate"].get<string>().length() == 0) {
                throw runtime_error("Leeres Attribut 'toDate' in Zeile " + to_string(i + 1));
            }
            if (data.at(i)["price"].is_null() || data.at(i)["price"].empty()) {
                throw runtime_error("Leeres Attribut 'price' in Zeile " + to_string(i + 1));
            }
            if (data.at(i)["travelId"].is_null() || data.at(i)["travelId"].empty()) {
                throw runtime_error("Leeres Attribut 'travelId' in Zeile " + to_string(i + 1));
            }
            type = data.at(i)["type"];
            id = data.at(i)["id"];

            if(findBooking(id) != nullptr){
                continue;
            }


            price = data.at(i)["price"];
            fromDate = data.at(i)["fromDate"];
            toDate = data.at(i)["toDate"];
            travelId = data.at(i)["travelId"];


            if (type == "Flight") {
                if (data.at(i)["fromDest"].is_null() || data.at(i)["fromDest"].empty() ||
                    data.at(i)["fromDest"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'fromDest' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["toDest"].is_null() || data.at(i)["toDest"].empty() ||
                    data.at(i)["toDest"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'toDest' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["airline"].is_null() || data.at(i)["airline"].empty() ||
                    data.at(i)["airline"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'airline' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["bookingClass"].is_null() || data.at(i)["bookingClass"].empty() ||
                    data.at(i)["bookingClass"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'bookingClass' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["fromDest"].get<string>().length() != 3 ||
                    data.at(i)["toDest"].get<string>().length() != 3) {
                    throw runtime_error("Falsches Flughafenkuerzel in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["fromDestLatitude"].is_null() || data.at(i)["fromDestLatitude"].empty() ||
                    data.at(i)["fromDestLatitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'fromDestLatitude' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["fromDestLongitude"].is_null() || data.at(i)["fromDestLongitude"].empty() ||
                    data.at(i)["fromDestLongitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'fromDestLongitude' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["toDestLatitude"].is_null() || data.at(i)["toDestLatitude"].empty() ||
                    data.at(i)["toDestLatitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'toDestLatitude' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["toDestLongitude"].is_null() || data.at(i)["toDestLongitude"].empty() ||
                    data.at(i)["toDestLongitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'toDestLongitude' in Zeile " + to_string(i + 1));
                }

                booking = shared_ptr<FlightBooking>(new FlightBooking(id, price, fromDate, toDate, travelId, data.at(i)["fromDest"],
                                            data.at(i)["toDest"], data.at(i)["airline"],
                                            bookingClasses[data.at(i)["bookingClass"]],
                                            data.at(i)["fromDestLongitude"].get<string>() + "," + data.at(i)["fromDestLatitude"].get<string>(),
                                            data.at(i)["toDestLongitude"].get<string>() + "," + data.at(i)["toDestLatitude"].get<string>()));
                flightCount++;
                totalPrice += price;

            } else if (type == "Hotel") {
                if (data.at(i)["hotel"].is_null() || data.at(i)["hotel"].empty() ||
                    data.at(i)["hotel"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'hotel' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["town"].is_null() || data.at(i)["town"].empty() ||
                    data.at(i)["town"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'town' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["roomType"].is_null() || data.at(i)["roomType"].empty() ||
                    data.at(i)["roomType"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'roomType' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["hotelLatitude"].is_null() || data.at(i)["hotelLatitude"].empty() ||
                    data.at(i)["hotelLatitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'hotelLatitude' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["hotelLongitude"].is_null() || data.at(i)["hotelLongitude"].empty() ||
                    data.at(i)["hotelLongitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'hotelLongitude' in Zeile " + to_string(i + 1));
                }
                booking = shared_ptr<HotelBooking>(new HotelBooking(id, price, fromDate, toDate, travelId, data.at(i)["hotel"],
                                           data.at(i)["town"], roomTypes[data.at(i)["roomType"]], data.at(i)["hotelLongitude"].get<string>() + "," + data.at(i)["hotelLatitude"].get<string>()));
                hotelCount++;
                totalPrice += price;
            } else if (type == "RentalCar") {
                if (data.at(i)["pickupLocation"].is_null() || data.at(i)["pickupLocation"].empty() ||
                    data.at(i)["pickupLocation"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'pickupLocation' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["returnLocation"].is_null() || data.at(i)["returnLocation"].empty() ||
                    data.at(i)["returnLocation"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'returnLocation' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["company"].is_null() || data.at(i)["company"].empty() ||
                    data.at(i)["company"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'company' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["vehicleClass"].is_null() || data.at(i)["vehicleClass"].empty() ||
                    data.at(i)["vehicleClass"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'vehicleClass' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["pickupLatitude"].is_null() || data.at(i)["pickupLatitude"].empty() ||
                    data.at(i)["pickupLatitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'pickupLatitude' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["pickupLongitude"].is_null() || data.at(i)["pickupLongitude"].empty() ||
                    data.at(i)["pickupLongitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'pickupLongitude' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["returnLatitude"].is_null() || data.at(i)["returnLatitude"].empty() ||
                    data.at(i)["returnLatitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'returnLatitude' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["returnLongitude"].is_null() || data.at(i)["returnLongitude"].empty() ||
                    data.at(i)["returnLongitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'returnLongitude' in Zeile " + to_string(i + 1));
                }
                booking = shared_ptr<RentalCarReservation> (new RentalCarReservation(id, price, fromDate, toDate, travelId, data.at(i)["pickupLocation"],
                                                   data.at(i)["returnLocation"], data.at(i)["company"],
                                                   data.at(i)["vehicleClass"], data.at(i)["pickupLongitude"].get<string>() + "," + data.at(i)["pickupLatitude"].get<string>(),
                                                   data.at(i)["returnLongitude"].get<string>() + "," + data.at(i)["returnLatitude"].get<string>()));


                carCount++;
                totalPrice += price;
            }

            if (data.at(i)["customerId"].is_null() || data.at(i)["customerId"].empty()) {
                throw runtime_error("Leeres Attribut 'customerId' in Zeile " + to_string(i + 1));
            }
            long customerId = data.at(i)["customerId"];
            allBookings.push_back(std::shared_ptr<Booking>(booking));

            shared_ptr<Travel> travel;
            if (findTravel(travelId) == nullptr) {
                travel = shared_ptr<Travel> (new Travel(travelId, customerId));
                travel->addBooking(booking);
                allTravels.push_back(std::shared_ptr<Travel>(travel));
                travelCount++;
            } else {
                findTravel(travelId)->addBooking(booking);
            }

            if (findCustomer(customerId) == nullptr) {
                shared_ptr<Customer> customer = shared_ptr<Customer> (new Customer(customerId, data.at(i)["customerName"]));
                customer->addTravel(travel);
                allCustomers.push_back(std::shared_ptr<Customer>(customer));
                customerCount++;
            } else {
                findCustomer(customerId)->addTravel(travel);
            }


        }
        catch (runtime_error r) {
            msgBox->setWindowTitle("Fehler beim Einlesen der Buchungen");
            msgBox->setText(QString::fromStdString(r.what()));
            msgBox->setInformativeText(
                    "Wenn Sie die Datei bereits korrigiert haben, wählen Sie 'Wiederholen'. Wählen Sie 'Verwerfen', um alle Buchungen zu löschen "
                    "und 'Abbrechen', um die vorhandenen Buchungen stehenzulassen und diesen Dialog zu verlassen");


            QAbstractButton *retryButton = reinterpret_cast<QAbstractButton *>(msgBox->addButton("Wiederholen",
                                                                                                QMessageBox::ActionRole));
            QAbstractButton *discardButton = reinterpret_cast<QAbstractButton *>(msgBox->addButton("Verwerfen",
                                                                                                  QMessageBox::ActionRole));

            QAbstractButton *cancelButton = reinterpret_cast<QAbstractButton *>(msgBox->addButton("Abbrechen",
                                                                                                 QMessageBox::ActionRole));
            QAbstractButton *detailsButton = msgBox->addButton("Details ausblenden", QMessageBox::ActionRole);


            detailsButton->disconnect();
            QObject::connect(detailsButton, &QAbstractButton::clicked, msgBox, [=]() {
                if (detailsButton->text() == "Details ausblenden") {
                    detailsButton->setText("Details anzeigen");
                    msgBox->setInformativeText("");
                    //msgBox->exec();
                } else {
                    detailsButton->setText("Details ausblenden");
                    msgBox->setInformativeText(
                            "Wenn Sie die Datei bereits korrigiert haben, wählen Sie 'Wiederholen'. Wählen Sie 'Verwerfen', um alle Buchungen zu löschen "
                            "und 'Abbrechen', um die vorhandenen Buchungen stehenzulassen und diesen Dialog zu verlassen");
                    msgBox->setFixedSize(400, 200);
                    //msgBox->exec();

                }
            });
            msgBox->exec();


            if (msgBox->clickedButton() == retryButton) {
                readFile(filePath, i, flightCount, hotelCount, carCount, travelCount, customerCount, totalPrice);
                return;
            }
            if (msgBox->clickedButton() == discardButton) {
                allBookings.clear();
                return;
            }
            if (msgBox->clickedButton() == cancelButton) {
                return;
            }

        }
        catch (json::type_error) {
            msgBox->setWindowTitle("Fehler beim Einlesen der Buchungen");
            msgBox->setText(
                    "Falscher Datentyp in Zeile " + QString::fromStdString(to_string(i + 1)));
            msgBox->setInformativeText(
                    "Wenn Sie die Datei bereits korrigiert haben, wählen Sie 'Wiederholen'. Wählen Sie 'Verwerfen', um alle Buchungen zu löschen "
                    "und 'Abbrechen', um die vorhandenen Buchungen stehenzulassen und diesen Dialog zu verlassen");


            QAbstractButton *retryButton = reinterpret_cast<QAbstractButton *>(msgBox->addButton("Wiederholen",
                                                                                                QMessageBox::ActionRole));
            QAbstractButton *discardButton = reinterpret_cast<QAbstractButton *>(msgBox->addButton("Verwerfen",
                                                                                                  QMessageBox::ActionRole));


            QAbstractButton *cancelButton = reinterpret_cast<QAbstractButton *>(msgBox->addButton("Abbrechen",
                                                                                                 QMessageBox::ActionRole));

            msgBox->exec();


            if (msgBox->clickedButton() == retryButton) {
                readFile(filePath, i, flightCount, hotelCount, carCount, travelCount, customerCount, totalPrice);
                return;
            }
            if (msgBox->clickedButton() == discardButton) {
                allBookings.clear();
                return;
            }

            if (msgBox->clickedButton() == cancelButton) {
                return;
            }

        }


    }
    file.close();


    msgBox->setWindowTitle("Datei erfolgreich eingelesen");
    msgBox->setText(QString::fromStdString(
            "Es wurden " + to_string(flightCount) + " Flugbuchungen, " + to_string(hotelCount) +
            " Hotelbuchungen und " +
            to_string(carCount) + " Mietwagenreservierungen im Gesamtwert von " + to_string(totalPrice) +
            " Euro eingelesen. Es wurden " + to_string(travelCount) + " Reisen und "
            + to_string(customerCount) + " Kunden angelegt. Der Kunde mit der ID 1 hat " +
            to_string(getTravelCount(1)) + " Reisen gebucht. Zur Reise mit der ID "
                                           "17 gehören " + to_string(
                    getBookingCount(17)) + " Buchungen."));
    msgBox->exec();
    return;


}

bool TravelAgency::readIataCodes(string filePath) {
    ifstream file(filePath);
    msgBox = new QMessageBox();

    if (!file) {

        msgBox->setWindowTitle("Fehler beim Einlesen der Iata-Codes");
        msgBox->setText("Datei konnte nicht gelesen werden");
        msgBox->exec();
        return false;
    }
    json data;
    file >> data;
    std::string name{}, isoCountry{}, municipality{}, iataCode{};
    try{
        for(int i = 0; i < data.size(); i++){
            if (data.at(i)["name"].is_null() || data.at(i)["name"].empty() ||
                data.at(i)["name"].get<string>().length() == 0) {
                continue;
            }
            if (data.at(i)["iso_country"].is_null() || data.at(i)["iso_country"].empty() ||
                data.at(i)["iso_country"].get<string>().length() == 0) {
                continue;
            }
            if (data.at(i)["municipality"].is_null() || data.at(i)["municipality"].empty() ||
                data.at(i)["municipality"].get<string>().length() == 0) {
                continue;
            }
            if (data.at(i)["iata_code"].is_null() || data.at(i)["iata_code"].empty() ||
                data.at(i)["iata_code"].get<string>().length() == 0) {
                continue;
            }

            name = data.at(i)["name"];
            //std::cout << name << endl;
            isoCountry = data.at(i)["iso_country"];
            municipality = data.at(i)["municipality"];
            iataCode = data.at(i)["iata_code"];

            iataCodes[iataCode] = shared_ptr<Airport>(new Airport(name, isoCountry, municipality));
        }
    }
    catch(...){
        return false;
    }

    return true;
}

std::shared_ptr<Booking> TravelAgency::findBooking(std::string id) {

    for (const auto& b: allBookings) {
        if(b.get() != nullptr){
            if (b->getId() == id) {
                return b;
            }
        }
    }

    return nullptr;
}

std::shared_ptr<Travel> TravelAgency::findTravel(long id) {
    for (auto t: allTravels) {
        if (t->getId() == id) {
            return t;
        }
    }
    return nullptr;
}

std::shared_ptr<Customer> TravelAgency::findCustomer(long id) {
    for (auto c: allCustomers) {
        if (c->getId() == id) {
            return c;
        }
    }
    return nullptr;
}

std::shared_ptr<Airport> TravelAgency::getAirport(std::string iataCode) {
    return iataCodes[iataCode];
}


const vector<shared_ptr<Booking>> &TravelAgency::getBookings() const {
    return allBookings;
}

int TravelAgency::getTravelCount(long customerId) {
    int travelCount{};
    for (auto t: allTravels) {
        if (t->getCustomerId() == customerId) {
            travelCount++;
        }
    }
    return travelCount;
}

int TravelAgency::getBookingCount(long travelId) {
    int bookingCount{};
    for (auto b: allBookings) {
        if (b->getTravelId() == travelId) {
            bookingCount++;
        }
    }
    return bookingCount;
}

const vector<shared_ptr<Booking>> &TravelAgency::getAllBookings() const {
    return allBookings;
}

const vector<shared_ptr<Customer>> &TravelAgency::getAllCustomers() const {
    return allCustomers;
}

const vector<shared_ptr<Travel>> &TravelAgency::getAllTravels() const {
    return allTravels;
}

long TravelAgency::getNextCustomerId() {
    long id = 1;

    for(auto c : allCustomers){
        if(c->getId() >= id){
            id = c->getId() + 1;
        }
    }
    return id;
}

void TravelAgency::addCustomer(std::shared_ptr<Customer> customer) {
    allCustomers.push_back(customer);
}

long TravelAgency::getNextTravelId() {
    long id = 1;

    for(auto t : allTravels){
        if(t->getId() >= id){
            id = t->getId() + 1;
        }
    }
    return id;
}

void TravelAgency::addTravel(std::shared_ptr<Travel> travel) {
    allTravels.push_back(travel);
}

const map<std::string, std::shared_ptr<Airport>> &TravelAgency::getIataCodes() {
    return iataCodes;
}

std::string TravelAgency::getIataCode(std::string airportName) {
    for(auto a : iataCodes){
        if(a.second->getName() == airportName){
            return a.first;
        }
    }
    return nullptr;
}

void TravelAgency::addBooking(std::shared_ptr<Booking> booking) {
    allBookings.push_back(booking);
}

std::string TravelAgency::getBookingClassChar(std::string bookingClass) {
    for(auto s : bookingClasses){
        if(s.second == bookingClass){
            return s.first;
        }
    }
}

std::string TravelAgency::getRoomTypeAcronym(std::string roomType) {
    for(auto s : roomTypes){
        if(s.second == roomType){

            return s.first;
        }
    }
}

bool TravelAgency::saveToJson(SortFunktor funktor, std::string filePath) {
    std::sort(allBookings.begin(), allBookings.end(), funktor);

    json bookingJson;
    json bookingListJson;

    for(auto b : allBookings){
        bookingJson["id"] = b->getId();
        bookingJson["price"] = b->getPrice();
        bookingJson["fromDate"] = b->getFromDate().toString("yyyyMMdd").toStdString();
        bookingJson["toDate"] = b->getToDate().toString("yyyyMMdd").toStdString();
        bookingJson["travelId"] = b->getTravelId();
        std::shared_ptr<Travel> travel = findTravel(b->getTravelId());
        std::shared_ptr<Customer> customer = findCustomer(travel->getCustomerId());
        bookingJson["customerId"] = customer->getId();
        bookingJson["customerName"] = customer->getName();

        if (std::shared_ptr<FlightBooking> flightBooking = dynamic_pointer_cast<FlightBooking>(b)){
            bookingJson["type"] = "Flight";
            bookingJson["fromDest"] = flightBooking->getFromDestination();
            bookingJson["toDest"] = flightBooking->getToDestination();
            bookingJson["airline"] = flightBooking->getAirline();
            //std::cout << "\n" << getBookingClassChar(flightBooking->getBookingClass());
            std::string test2 = getBookingClassChar(flightBooking->getBookingClass());
            //std::cout << test2;



            bookingJson["bookingClass"] = getBookingClassChar(flightBooking->getBookingClass());
            bookingJson["fromDestLongitude"] = QString::fromStdString(flightBooking->getFromDestCoordinates()).split(",")[0].toStdString();
            bookingJson["fromDestLatitude"] = QString::fromStdString(flightBooking->getFromDestCoordinates()).split(",")[1].toStdString();
            bookingJson["toDestLongitude"] = QString::fromStdString(flightBooking->getToDestCoordinates()).split(",")[0].toStdString();
            bookingJson["toDestLatitude"] = QString::fromStdString(flightBooking->getToDestCoordinates()).split(",")[1].toStdString();
        }
        else if(std::shared_ptr<HotelBooking> hotelBooking = dynamic_pointer_cast<HotelBooking>(b)){
            bookingJson["type"] = "Hotel";
            bookingJson["hotel"] = hotelBooking->getHotel();
            bookingJson["town"] = hotelBooking->getTown();
            bookingJson["roomType"] = getRoomTypeAcronym(hotelBooking->getRoomType());
            bookingJson["hotelLongitude"] = QString::fromStdString(hotelBooking->getHotelCoordinates()).split(",")[0].toStdString();
            bookingJson["hotelLatitude"] = QString::fromStdString(hotelBooking->getHotelCoordinates()).split(",")[1].toStdString();
        }
        else if(std::shared_ptr<RentalCarReservation> rentalCarReservation = dynamic_pointer_cast<RentalCarReservation>(b)){
            bookingJson["type"] = "RentalCar";
            bookingJson["pickupLocation"] = rentalCarReservation->getPickupLocation();
            bookingJson["returnLocation"] = rentalCarReservation->getReturnLocation();
            bookingJson["company"] = rentalCarReservation->getCompany();
            bookingJson["vehicleClass"] = rentalCarReservation->getVehicleClass();
            bookingJson["pickupLongitude"] = QString::fromStdString(rentalCarReservation->getPickupCoordinates()).split(",")[0].toStdString();
            bookingJson["pickupLatitude"] = QString::fromStdString(rentalCarReservation->getPickupCoordinates()).split(",")[1].toStdString();
            bookingJson["returnLongitude"] = QString::fromStdString(rentalCarReservation->getReturnCoordinates()).split(",")[0].toStdString();
            bookingJson["returnLatitude"] = QString::fromStdString(rentalCarReservation->getReturnCoordinates()).split(",")[1].toStdString();
        }
        bookingListJson.push_back(bookingJson);

    }
    //ofstream outputStream("../Studierende.json");
    ofstream outputStream(filePath);
    if (!outputStream) {
        return false;
    }
    outputStream << bookingListJson.dump(4);
    outputStream.close();
    return true;

}

