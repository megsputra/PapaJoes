#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include "Order.h"
#include "Heap.h"
#include "List.h"
#include "Product.h"

#include <iostream>
#include <string>

using namespace std;

class Customer {
private:
    string customerID;
    string password;
    string firstName;
    string lastName;
    string eMail;
    string phoneNum;
    List<Product*> myCart;
    List<Order*> orderHistory;
    string street;
    string city;
    string state;
    string zip;

public:
    string getKey() {return customerID;}

    Customer(): customerID(""), password(""), firstName(""), lastName(""), eMail(""), phoneNum("") {}

    Customer(string customerID, string password, string first_name, string last_name,
             string e_mail, string phoneNum, string street, string city, string state, string zip)
    : customerID(customerID), password(password), firstName(first_name), lastName(last_name),
	  eMail(e_mail), phoneNum(phoneNum), street(street), city(city), state(state), zip(zip)
    {}

    const string& getCustomerID() const {
        return customerID;
    }

    void setCustomerID(const string& customerID) {
        this->customerID = customerID;
    }

    const string& getFirstName() const {
        return firstName;
    }

    void setFirstName(const string& firstName) {
        this->firstName = firstName;
    }

    const string& getLastName() const {
        return lastName;
    }

    void setLastName(const string& lastName) {
        this->lastName = lastName;
    }

    List<Product*> getCart() const {
            return myCart;
    }

    const string& getPassword() const {
        return password;
    }

    void setPassword(const string& password) {
        this->password = password;
    }

    const string& getEmail() const {
        return eMail;
    }

    void setEmail(const string& email) {
        this->eMail = email;
    }

    const string& getPhoneNum() const {
        return phoneNum;
    }

    void setPhoneNum(const string& phoneNum) {
        this->phoneNum = phoneNum;
    }

    const string& getStreet() const {
    	return street;
    }

    void setStreet(const string& str) {
        this->street = str;
    }

    const string& getCity() const {
    	return city;
    }

    void setCity(const string& city) {
    	this->city = city;
    }

    const string& getState() const {
    	return state;
    }

    void setState(const string& state) {
    	this->state = state;
    }

    const string& getZip() const {
    	return zip;
    }

    void setZip(const string& zip) {
    	this->zip = zip;
    }

    bool operator<(const Customer& customer);
    bool operator>(const Customer& customer);
    bool operator==(const Customer& customer);
    friend ostream& operator<<(ostream& os, Customer& customer);
    void addToCart(Product* product);
	void deleteItemFromCart(Product* product);
	void viewCart();
	void addToOrderHistory(Order* order);
	void displayAccountInfo();
	void editAccount();
	void printMyCart();
	void printOrderHistory();
    List<Order*> getOrderHistory();
};

List<Order*> Customer::getOrderHistory() {
	return orderHistory;
}
void Customer::addToOrderHistory(Order* order){
	orderHistory.insertEnd(order);
}

void Customer::printMyCart() {
	myCart.beginIterator();
	for(int i=0; i< myCart.getLength(); i++) {
		cout << **myCart.getIterator() << endl;
		myCart.advanceIterator();
	}
}

void Customer::printOrderHistory() {
	if (orderHistory.empty()) {
		cout << "EMPTY" << endl;
		return;
	}
	for (orderHistory.beginIterator(); !orderHistory.offEnd(); orderHistory.advanceIterator()) {
		cout << *orderHistory.getIterator() << endl;
	}
}

void Customer::addToCart(Product* product) {
	myCart.insertEnd(product);
}

void Customer::deleteItemFromCart(Product* product) {
	if (myCart.empty()) {
		cout << "Error: Couldn't delete any item, your cart is empty!!!\n";
		return;
	} else {
		int index = myCart.linearSearch(product);
		if (index == -1)
			cout << "Error: Delete item from cart - Item not found!!!\n";
		else {
			myCart.advanceToIndex(index);
			myCart.deleteIterator();
		}
	}
}

void Customer::displayAccountInfo() {
	cout << "\n***Account Information***\n";
	cout << "User name: " << customerID << endl;
	cout << "Password: " << password << endl;
	cout << "Name: " << firstName << " " << lastName << endl;
	cout << "E-mail: " << eMail << endl;
	cout << "Phone Number: " << phoneNum << endl;
	cout << "Address: " << street << ", " << city << ", " << state << " " << zip << endl;
}

bool Customer::operator<(const Customer &customer) {
    return customerID < customer.customerID;
}

bool Customer::operator>(const Customer &customer) {
    return customerID > customer.customerID;
}

bool Customer::operator==(const Customer& customer) {
    return (customerID == customer.customerID);
}

ostream& operator<<(ostream& os, Customer& customer) {
	os << "User name: " << customer.customerID << endl;
    os << "Name: " << customer.firstName << " " << customer.lastName << endl;
    os << "Email: " << customer.eMail << endl;
    os << "Phone number: " << customer.phoneNum << endl;
    os << "\nUser order history: " << endl;
    customer.printOrderHistory();
    return os;
}

#endif /* CUSTOMER_H_ */
