#include <iostream>
#include <windows.h>
#include <string>
#include <iomanip>
#include <limits>
#include <unistd.h>
#include <fstream>
#include <conio.h>
#define ENTER 13
#define BACKSPACE 8

using namespace std;

const string USERNAME_PREFIX = "Username: ";
const string PASSWORD_PREFIX = "Password: ";
const string ECONOMY_FILE = "EconomyCars.txt";
const string COMFORTABLE_FILE = "ComfortableCars.txt";
const string LUXURY_FILE = "LuxuryCars.txt";
const int ESCAPE = 27;

void password_visual(string &password)
{
    char ch;
    while (1)
    {
        ch = _getch();

        if (ch == ENTER)
        {
            cout << endl;
            break;
        }
        else if (ch == BACKSPACE)
        {
            if (!password.empty())
            {
                cout << "\b \b";
                password.pop_back();
            }
        }
        else
        {
            password.push_back(ch);
            cout << '*';
        }
    }
}

enum Rating
{
    Poor = 1,
    Average = 2,
    Good = 3,
    Excellent = 5
};

class Driver

{
protected:
    string name;
    string username;
    string licenseid;
    string category;
    double discount;
    string password;

public:
    virtual void display_driver_info() = 0;
    virtual void set_driver_info() = 0;
    virtual void save_driver_info(string filename) = 0;

    Driver(const string &Category)
    {
        set_basic_info(Category);
        signup();
    }

    Driver(string name, string licenseid, string category, double discount, string username, string password)
        : name(name), licenseid(licenseid), category(category), discount(discount),
          username(username), password(password)
    {
        signup();
    }

    string get_name()
    {
        return name;
    }
    string get_license_id()
    {
        return licenseid;
    }
    string get_category()
    {
        return category;
    }
    string get_username()
    {
        return username;
    }
    string get_password()
    {
        return password;
    }

    double get_discount()
    {
        return discount;
    }

    void signup()
    {
        set_username();
        set_password();
        save_user();
    }

    void save_user()
    {
        ofstream file("Users.txt", ios::app);
        if (file.is_open())
        {
            file << "Name: " << name << endl;
            file << "License ID: " << licenseid << endl;
            file << "Category: " << category << endl;
            file << "Username: " << username << endl;
            file << "Password: " << password << endl;
        }
        else
        {
            cout << "Unable to open Users file" << endl;
        }
    }

    void set_password()
    {
        cout << "Enter Password: ";
        password_visual(password);
    }

    bool is_username_taken(const string &username)
    {
        ifstream usersFile("Users.txt");

        if (!usersFile.is_open())
        {
            return false;
        }

        string line;
        while (getline(usersFile, line))
        {
            size_t posUsername = line.find(USERNAME_PREFIX);
            if (posUsername != string::npos)
            {
                string storedUsername = line.substr(posUsername + USERNAME_PREFIX.length());
                if (storedUsername == username)
                {
                    cout << "Username is already taken. Please choose a different one." << endl;
                    usersFile.close();
                    return true;
                }
            }
        }

        usersFile.close();
        return false;
    }

    void set_username()
    {
        do
        {
            cout << "Enter Username: ";
            cin >> username;
        } while (is_username_taken(username));
    }

    void display_basic_info()
    {
        cout << "Name: " << name << endl;
        cout << "License ID: " << licenseid << endl;
        cout << "Category: " << category << endl;
        cout << "Discount: " << discount << endl;
    }

    void set_basic_info(const string &Category)
    {

        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter License ID: ";
        getline(cin, licenseid);
        category = Category;
        cout << "Enter Discount: ";
        cin >> discount;
    }

    void save_basic_info(string filename)
    {
        ofstream file(filename, ios::app);
        if (file.is_open())
        {
            file << "Name: " << name << endl;
            file << "License ID: " << licenseid << endl;
            file << "Category: " << category << endl;
            file << "Discount: " << discount << endl;
        }
        else
        {
            cout << "Unable to open file: " << filename << endl;
        }
    }
};

class SilverDriver : public Driver
{
private:
    bool rewards_eligible;
    int free_rides;

public:
    SilverDriver() : Driver("Silver Driver")
    {
        set_rewards();
        set_free_rides();
        save_driver_info("SilverDriver.txt");
    }

    SilverDriver(string name, string licenseid, string category, double discount, string username, string password,
                 bool rewards_eligible, int free_rides)
        : Driver(name, licenseid, category, discount, username, password), rewards_eligible(rewards_eligible), free_rides(free_rides) {}

    void display_driver_info() override
    {
        display_basic_info();
        cout << "Rewards Eligible: " << (rewards_eligible ? "Yes" : "No") << endl;
        cout << "Free Rides: " << free_rides << endl;
    }

    void set_driver_info() override
    {
        set_basic_info("Silver Driver");
        cout << "Enter Rewards Eligible (1 for Yes / 0 for No): ";
        cin >> rewards_eligible;
        cout << "Enter Free Rides: ";
        cin >> free_rides;
    }

    void set_rewards()
    {
        cout << "Enter Rewards Eligible (1 for Yes / 0 for No): ";
        cin >> rewards_eligible;
    }

    void set_free_rides()
    {
        cout << "Enter Free Rides: ";
        cin >> free_rides;
    }

    void save_driver_info(string filename) override
    {
        ofstream file(filename, ios::app);
        if (file.is_open())
        {
            save_basic_info(filename);
            file << "Rewards Eligible: " << (rewards_eligible ? "Yes" : "No") << endl;
            file << "Free Rides: " << free_rides << endl;
        }
        else
        {
            cout << "Unable to open file: " << filename << endl;
        }
    }
};

class PlatinumDriver : public Driver
{
private:
    string additional_service;
    bool dedicated_support;

public:
    PlatinumDriver() : Driver("Platinum Driver")
    {
        set_additional_services();
        set_dedicated_support();
        save_driver_info("PlatinumDrivers.txt");
    }
    PlatinumDriver(string name, string licenseid, string category, double discount, string username, string password,
                   string additional_service, bool dedicated_support)
        : Driver(name, licenseid, category, discount, username, password),
          additional_service(additional_service), dedicated_support(dedicated_support) {}

    void set_additional_services()
    {
        cout << "Enter Additional Service: ";
        cin >> additional_service;
    }

    void set_dedicated_support()
    {
        cout << "Enter Dedicated Support (1 for Yes / 0 for No): ";
        cin >> dedicated_support;
    }

    void display_driver_info() override
    {
        display_basic_info();
        cout << "Additional Service: " << additional_service << endl;
        cout << "Dedicated Support: " << (dedicated_support ? "Yes" : "No") << endl;
    }

    void set_driver_info() override
    {
        set_basic_info("Platinum Driver");
        cout << "Enter Additional Service: ";
        cin >> additional_service;
        cout << "Enter Dedicated Support (1 for Yes / 0 for No): ";
        cin >> dedicated_support;
    }

    void save_driver_info(string filename) override
    {
        ofstream file(filename, ios::app);
        if (file.is_open())
        {
            save_basic_info(filename);

            file << "Additional Services: " << additional_service << endl;
            file << "Dedicated support: " << (dedicated_support ? "Yes" : "No") << endl;
        }
        else
        {
            cout << "Unable to open file: " << filename << endl;
        }
    }
};
class GoldDriver : public Driver
{
private:
    int loyalty_points;
    bool priority_support;

public:
    GoldDriver() : Driver("Gold Driver")
    {
        set_loyalty_points();
        set_priority_support();
        save_driver_info("GoldDrivers.txt");
    }
    GoldDriver(string name, string licenseid, string category, double discount, string username, string password, int loyalty_points, bool priority_support)
        : Driver(name, licenseid, category, discount, username, password), loyalty_points(loyalty_points), priority_support(priority_support) {}

    void set_loyalty_points()
    {
        cout << "Enter Loyalty Points: ";
        cin >> loyalty_points;
    }

    void set_priority_support()
    {
        cout << "Enter Priority Support (1 for Yes / 0 for No): ";
        cin >> priority_support;
    }

    void display_driver_info() override
    {
        display_basic_info();
        cout << "Loyalty Points: " << loyalty_points << endl;
        cout << "Priority Support: " << (priority_support ? "Yes" : "No") << endl;
    }

    void set_driver_info() override
    {
        set_basic_info("Gold Driver");
        cout << "Enter Loyalty Points: ";
        cin >> loyalty_points;
        cout << "Enter Priority Support (1 for Yes / 0 for No): ";
        cin >> priority_support;
    }

    void save_driver_info(string filename) override
    {
        ofstream file(filename, ios::app);
        if (file.is_open())
        {
            save_basic_info(filename);

            file << "Loyalty Points: " << loyalty_points << endl;
            file << "Priority Support: " << (priority_support ? "Yes" : "No") << endl;
        }
        else
        {
            cout << "Unable to open file: " << filename << endl;
        }
    }
};

class Car
{
protected:
    static int total_cars;
    int carId;
    string brand;
    int model;
    string category;
    int horse_power;
    int engineCC;
    Rating CarRating;
    double rental_fee;

public:
    virtual void display_car_info() = 0;
    virtual void set_car_info() = 0;
    virtual void save_car_info(string filename) = 0;

    Car(int carId, string brand, int model, string category,
        int horse_power, int engineCC, bool hasDamages, bool hasInteriorIssues,
        bool hasMechanicalIssues, int enginePower, bool hasNoises, bool hasLeakages, double rental_fee)
        : carId(carId), brand(brand), model(model), category(category),
          horse_power(horse_power), engineCC(engineCC), rental_fee(rental_fee)
    {
        calculateRating(hasDamages, hasInteriorIssues, hasMechanicalIssues, enginePower, hasNoises, hasLeakages);
        total_cars++;
    }

    ~Car()
    {
        total_cars--;
    }

    int get_model()
    {
        return model;
    }
    string get_brand()
    {
        return brand;
    }

    int get_carId()
    {
        return carId;
    }

    string get_category()
    {
        return category;
    }

    double get_rental_fee()
    {
        return rental_fee;
    }

    void calculateRating(bool hasDamages, bool hasInteriorIssues, bool hasMechanicalIssues, int enginePower, bool hasNoises, bool hasLeakages)
    {

        if (hasDamages || hasInteriorIssues || hasMechanicalIssues || enginePower < 100 || hasNoises || hasLeakages)
        {
            CarRating = Poor;
        }
        else if (enginePower >= 100 && enginePower <= 200 && engineCC <= 2500)
        {
            CarRating = Average;
        }
        else if (enginePower > 200 && engineCC > 2500)
        {
            CarRating = Excellent;
        }
        else
        {
            CarRating = Good;
        }
    }

    void display_basic_info()
    {
        cout << "Brand: " << brand << endl;
        cout << "Model: " << model << endl;
        cout << "Category: " << category << endl;
        cout << "Horse Power: " << horse_power << " hp" << endl;
        cout << "Engine CC: " << engineCC << " cc" << endl;
        cout << "Car Rating: " << CarRating << endl;
        cout << "Rental fee: " << rental_fee << endl;
    }

    void set_basic_info()
    {
        cout << "Enter carId: ";
        cin >> carId;
        cout << "Enter Brand: ";
        cin.ignore();
        cin >> brand;
        cout << "Enter Model: ";
        cin >> model;
        cout << "Enter Category: ";
        cin >> category;
        cout << "Enter Horse Power: ";
        cin >> horse_power;
        cout << "Enter Engine CC: ";
        cin >> engineCC;
        cout << "Enter Rental fee: ";
        cin >> rental_fee;
    };
    void save_basic_info(string filename)
    {
        ofstream file(filename, ios::app);
        if (file.is_open())
        {
            file << "carId: " << carId << endl;
            file << "Brand: " << brand << endl;
            file << "Model: " << model << endl;
            file << "Category: " << category << endl;
            file << "Horse Power: " << horse_power << endl;
            file << "Engine CC: " << engineCC << endl;
            file << "Rating: " << CarRating << endl;
            file << "Rental fee: " << rental_fee << endl;
        }
        else
        {
            cout << "Unable to open file: " << filename << endl;
        }
    }
};

int Car::total_cars = 0;

class EconomyCar : public Car
{
private:
    bool fuel_efficient;
    bool is_compact;

public:
    EconomyCar(int carId, string brand, int model,
               int horsePower, int engineCC, bool hasDamages, bool hasInteriorIssues,
               bool hasMechanicalIssues, int enginePower, bool hasNoises, bool hasLeakages, bool fuel_efficient, bool is_compact, double rental_fee)
        : Car(carId, brand, model, "EconomyCar", horsePower, engineCC, hasDamages, hasInteriorIssues, hasMechanicalIssues, enginePower, hasNoises, hasLeakages, rental_fee),
          fuel_efficient(fuel_efficient), is_compact(is_compact)
    {
        save_car_info("EconomyCars.txt");
    }

    void display_car_info() override
    {
        display_basic_info();
        cout << "Fuel Efficient: " << (fuel_efficient ? "Yes" : "No") << endl;
        cout << "Is Compact: " << (is_compact ? "Yes" : "No") << endl;
    }

    void set_car_info() override
    {
        set_basic_info();
        cout << "is fuel efficient?: (1 for Yes / 0 for No):";
        cin >> fuel_efficient;
        cout << "is compact?: (1 for Yes / 0 for No): ";
        cin >> is_compact;
    }
    void save_car_info(string filename) override
    {
        ofstream file(filename, ios::app);
        if (file.is_open())
        {
            save_basic_info(filename);

            file << "Fuel Efficient: " << (fuel_efficient ? "Yes" : "No") << endl;
            file << "Is Compact: " << (is_compact ? "Yes" : "No") << endl;
            file << "-----------------------" << endl;
        }
        else
        {
            cout << "Unable to open file: " << filename << endl;
        }
    }
};
class ComfortableCar : public Car
{
private:
    bool Navigation;
    string seat_material;

public:
    ComfortableCar(int carId, string brand, int model,
                   int horse_power, int engineCC, bool hasDamages, bool hasInteriorIssues,
                   bool hasMechanicalIssues, int enginePower, bool hasNoises, bool hasLeakages, bool Navigation, string seat_material, double rental_fee)
        : Car(carId, brand, model, "Comfortable", horse_power, engineCC, hasDamages, hasInteriorIssues, hasMechanicalIssues, enginePower, hasNoises, hasLeakages, rental_fee),
          Navigation(Navigation), seat_material(seat_material)
    {
        save_car_info("ComforableCars.txt");
    }

    void display_car_info() override
    {
        display_basic_info();
        cout << "Has Navigation: " << (Navigation ? "Yes" : "No") << endl;
        cout << "Seat Material: " << seat_material << endl;
    }

    void set_car_info() override
    {
        set_basic_info();
        cout << "Has Navigation: (1 for Yes / 0 for No):";
        cin >> Navigation;
        cout << "Seat Material: ";
        cin >> seat_material;
    }
    void save_car_info(string filename) override
    {
        ofstream file(filename, ios::app);
        if (file.is_open())
        {
            save_basic_info(filename);

            file << "Has Navigation: " << Navigation << endl;
            file << "Seat Material: " << seat_material << endl;
            file << "-----------------------" << endl;
        }
        else
        {
            cout << "Unable to open file: " << filename << endl;
        }
    }
};
class LuxuryCar : public Car
{
private:
    string special_features;
    bool massage_seats;

public:
    LuxuryCar(int carId, string brand, int model,
              int horsePower, int engineCC, bool hasDamages, bool hasInteriorIssues,
              bool hasMechanicalIssues, int enginePower, bool hasNoises, bool hasLeakages, string special_features, bool massage_seats, double rental_fee)
        : Car(carId, brand, model, "Luxury", horsePower, engineCC, hasDamages, hasInteriorIssues, hasMechanicalIssues, enginePower, hasNoises, hasLeakages, rental_fee),
          special_features(special_features), massage_seats(massage_seats)
    {
        save_car_info("LuxuryCars.txt");
    }

    void display_car_info() override
    {
        display_basic_info();
        cout << "Special Features: " << special_features << endl;
        cout << "Has Massage Seats?: " << (massage_seats ? "Yes" : "No") << endl;
    }

    void set_car_info() override
    {
        set_basic_info();
        cout << "Has Massage Seats? : (1 for Yes / 0 for No):";
        cin >> massage_seats;
        cout << "Special Features: ";
        cin >> special_features;
    }
    void save_car_info(string filename) override
    {
        ofstream file(filename, ios::app);
        if (file.is_open())
        {
            save_basic_info(filename);

            file << "Has Massage Seats?: " << massage_seats << endl;
            file << "Special Features: " << special_features << endl;
            file << "-----------------------" << endl;
        }
        else
        {
            cout << "Unable to open file: " << filename << endl;
        }
    }
};

class Booking
{

protected:
    int booking_id;
    double payment;
    double fine = 0;
    int initial_meter_rd;
    int final_meter_rd;
    int available_duration;
    int reservation_duration;
    int distance_traveled;
    Car *book_car;
    Driver *driver;

public:
    Booking()
    {
    }

    int get_booking_id()
    {
        return booking_id;
    }

    void set_booking_info(Car *book_car, Driver *driver)
    {
        cout << "Enter Booking ID: ";
        cin >> booking_id;

        cout << "Enter initial meter reading: ";
        cin >> initial_meter_rd;

        cout << "Enter final meter reading: ";
        cin >> final_meter_rd;

        cout << "Enter available duration (in days): ";
        cin >> available_duration;

        cout << "Enter reservation duration (in days): ";
        cin >> reservation_duration;
        this->book_car = book_car;
        this->driver = driver;

        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void display_booking_info()
    {
        cout << "Booking ID: " << booking_id << endl;
        cout << "Initial Meter Reading: " << initial_meter_rd << endl;
        cout << "Final Meter Reading: " << final_meter_rd << endl;
        cout << "Available Duration: " << available_duration << " days" << endl;
        cout << "Reservation Duration: " << reservation_duration << " days" << endl;
        cout << "Car Details:" << endl;
        cout << "Car ID: " << book_car->get_carId() << endl;
        cout << "Car Model: " << book_car->get_model() << endl;
        cout << "Car Category: " << book_car->get_category() << endl;
        cout << "Car Rental Fee: " << book_car->get_rental_fee() << endl;
        cout << "Driver Name: " << driver->get_name() << endl;
        cout << "Driver License ID: " << driver->get_license_id() << endl;
        cout << "Driver Category: " << driver->get_category() << endl;
        cout << "Driver Discount: " << driver->get_discount() << endl;
        cout << "Total Payment: " << payment << endl;
    }

    void billing()
    {
        cout << fixed << setprecision(2);
        cout << "\n\t\t               Car Rental - Billing and Payment                  " << endl;
        cout << "\t\t	///////////////////////////////////////////////////////////" << endl;
        cout << "\t\t	| Booking ID:                | " << setw(25) << booking_id << " |" << endl;
        cout << "\t\t	| Initial Meter Reading:     | " << setw(25) << initial_meter_rd << " |" << endl;
        cout << "\t\t	| Final Meter Reading:       | " << setw(25) << final_meter_rd << " |" << endl;
        cout << "\t\t	| Available Duration:        | " << setw(25) << available_duration << " |" << endl;
        cout << "\t\t	| Reservation Duration:      | " << setw(25) << reservation_duration << " |" << endl;
        cout << "\t\t	| Distance Traveled:         | " << setw(25) << distance_traveled << " |" << endl;
        cout << "\t\t	| Fine:                      | " << setw(25) << fine << " |" << endl;
        cout << "\t\t	| Car Brand:                 | " << setw(25) << book_car->get_brand() << " |" << endl;
        cout << "\t\t	| Car Model:                 | " << setw(25) << book_car->get_model() << " |" << endl;
        cout << "\t\t	| Car Category:              | " << setw(25) << book_car->get_category() << " |" << endl;
        cout << "\t\t	| Car Rental Fee:            | " << setw(25) << book_car->get_rental_fee() << " |" << endl;
        cout << "\t\t	| Driver Name:               | " << setw(25) << driver->get_name() << " |" << endl;
        cout << "\t\t	| Driver License ID:         | " << setw(25) << driver->get_license_id() << " |" << endl;
        cout << "\t\t	| Driver Category:           | " << setw(25) << driver->get_category() << " |" << endl;
        cout << "\t\t	| Driver Discount:           | " << setw(25) << driver->get_discount() << " |" << endl;
        cout << "\t\t	 ________________________________________________________" << endl;
        cout << "\t\t	| Total Rental Amount is $:  | " << setw(25) << payment << " |" << endl;
        cout << "\t\t	 ________________________________________________________" << endl;
        cout << "\t\t	 # This is a computer-generated invoice and it does not" << endl;
        cout << "\t\t	 require an authorized signature #" << endl;
        cout << " " << endl;
        cout << "\t\t	///////////////////////////////////////////////////////////" << endl;
        cout << "\t\t	You are advised to pay up the amount before the due date." << endl;
        cout << "\t\t	Otherwise, a penalty fee will be applied" << endl;
        cout << "\t\t	///////////////////////////////////////////////////////////" << endl;
    }
    void calculate_payment()
    {
        double rental_fee = book_car->get_rental_fee();
        double driver_discount = driver->get_discount();

        distance_traveled = final_meter_rd - initial_meter_rd;
        payment = rental_fee * distance_traveled;
        double discount_amount = payment * (driver_discount / 100.0);

        int reservation_difference = reservation_duration - available_duration;

        if (reservation_difference > 0)
        {
            double fine_percentage = 0.05;
            fine = fine_percentage * reservation_difference;
            payment += fine;
        }

        payment -= discount_amount;
    }
};

void sign_up_menu(Driver *&newDriver, Driver **&registeredDrivers, int &numRegistered)
{
    system("cls");
    char driverType;

    cout << "Choose driver type:" << endl;
    cout << "1. Silver Driver" << endl;
    cout << "2. Platinum Driver" << endl;
    cout << "3. Gold Driver" << endl;
    cout << "Press ESCAPE to exit" << endl;

    while (true)
    {

        driverType = _getch();

        if (driverType == '1')
        {
            system("cls");
            newDriver = new SilverDriver();
            break;
        }
        else if (driverType == '2')
        {
            system("cls");
            newDriver = new PlatinumDriver();
            break;
        }
        else if (driverType == '3')
        {
            system("cls");
            newDriver = new GoldDriver();
            break;
        }
        else if (driverType == ESCAPE)
        {
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    if (newDriver != nullptr)
    {

        Driver **temp = new Driver *[numRegistered + 1];
        for (int i = 0; i < numRegistered; ++i)
        {
            temp[i] = registeredDrivers[i];
        }
        temp[numRegistered] = newDriver;

        delete[] registeredDrivers;

        registeredDrivers = temp;

        ++numRegistered;
        cout << "Driver registered successfully!" << endl;
        cout << "Number of registered drivers: " << numRegistered << endl;
        sleep(3);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
void displayCarDetails(const string &filename)
{
    ifstream carFile(filename);

    if (!carFile.is_open())
    {
        cerr << "Error opening " << filename << endl;
        return;
    }

    cout << "Car Details:" << endl;

    string line;
    while (getline(carFile, line))
    {
        cout << line << endl;
    }

    carFile.close();
}

void car_menu()
{
    system("cls");
    char key;

    cout << "Car Categories:" << endl;
    cout << "1. Economy Cars" << endl;
    cout << "2. Comfortable Cars" << endl;
    cout << "3. Luxury Cars" << endl;

    while (true)
    {

        key = _getch();

        if (key == '1')
        {
            displayCarDetails(ECONOMY_FILE);
            break;
        }
        else if (key == '2')
        {
            displayCarDetails(COMFORTABLE_FILE);
        }
        else if (key == '3')
        {
            displayCarDetails(LUXURY_FILE);
        }
        else if (key == ESCAPE)
        {
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

Driver *login(Driver *registeredDrivers[], int numRegistered)
{
    system("cls");

    string enteredUsername, enteredPassword;
    bool isValidCredentials = false;
    Driver *loggedInDriver = nullptr;

    cout << "Enter username: ";
    cin >> enteredUsername;
    cout << "Enter password: ";
    password_visual(enteredPassword);

    for (int i = 0; i < numRegistered; ++i)
    {
        if (registeredDrivers[i]->get_username() == enteredUsername &&
            registeredDrivers[i]->get_password() == enteredPassword)
        {
            cout << "Login successful!..." << endl;
            sleep(2);
            isValidCredentials = true;
            loggedInDriver = registeredDrivers[i];
            break;
        }
    }

    if (!isValidCredentials)
    {
        cout << "Invalid credentials! Please try again." << endl;
        exit(0);
    }

    return loggedInDriver;
}

Car *findCarById(Car *cars[], int totalCars, int carId)
{
    for (int i = 0; i < totalCars; ++i)
    {

        if (cars[i]->get_carId() == carId)
        {

            return cars[i];
        }
    }

    return nullptr;
}

Booking *findBookingById(Booking **allBookings, int numBookings, int bookingId)
{
    for (int i = 0; i < numBookings; ++i)
    {
        if (allBookings[i]->get_booking_id() == bookingId)
        {
            return allBookings[i];
        }
    }
    return nullptr;
}

void addBooking(Booking *&newBooking, Booking **&allBookings, int &numBookings)
{
    Booking **temp = new Booking *[numBookings + 1];
    for (int i = 0; i < numBookings; ++i)
    {
        temp[i] = allBookings[i];
    }
    temp[numBookings] = newBooking;

    delete[] allBookings;
    allBookings = temp;

    ++numBookings;
}

void mainMenu(Car *cars[], int totalCars, Driver *&newDriver, Driver **&registeredDrivers,
              int &numRegistered, Booking **&allBookings, int &numBookings)
{
    system("cls");
    int carId;
    int booking_id;
    char key;
    cout << "==============================" << endl;
    cout << "   Car Rental System" << endl;
    cout << "==============================" << endl;
    cout << "Press 1 to Sign Up" << endl;
    cout << "Press 2 to Login" << endl;
    cout << "Press Escape to Exit" << endl;

    while (true)
    {
        key = getch();

        if (key == '1')
        {
            sign_up_menu(newDriver, registeredDrivers, numRegistered);
            mainMenu(cars, totalCars, newDriver, registeredDrivers, numRegistered, allBookings, numBookings);
        }

        else if (key == '2')
        {
            Driver *loggedInDriver = login(registeredDrivers, numRegistered);
            car_menu();
            cout << "Enter car ID to book a car: ";
            cin >> carId;

            Car *foundCar = findCarById(cars, totalCars, carId);
            if (foundCar != nullptr)
            {
                Booking *newBooking = new Booking();

                newBooking->set_booking_info(foundCar, loggedInDriver);

                newBooking->calculate_payment();
                newBooking->billing();

                sleep(8);
                addBooking(newBooking, allBookings, numBookings);

                cout << "Enter booking id to Find: " << endl;
                cin >> booking_id;

                Booking *foundbooking = findBookingById(allBookings, numBookings, booking_id);
                foundbooking->display_booking_info();
                sleep(10);
            }
            else
            {
                cout << "Car ID not found." << endl;
            }
        }

        else if (key == ESCAPE)
        {
            cout << "Exiting program. Goodbye!" << endl;
            break;
        }
    }
}

int main()
{
    Driver *newDriver = nullptr;
    Driver **registeredDrivers = nullptr;
    int numRegistered = 0;

    Booking **allBookings = nullptr;
    int numBookings = 0;

    Car *cars[] = {
        new EconomyCar(452, "toyota", 2011, 1200, 1500, true, true, false, 2500, true, false, true, true, 1200.65),
        new EconomyCar(611, "Corolla", 2011, 1200, 1500, true, true, false, 2500, true, false, true, true, 350)};

    int totalCars = sizeof(cars) / sizeof(cars[0]);
    mainMenu(cars, totalCars, newDriver, registeredDrivers, numRegistered, allBookings, numBookings);
}