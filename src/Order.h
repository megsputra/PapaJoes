#ifndef ORDER_H_
#define ORDER_H_

#include "Product.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

enum SHIPPING_TYPE { OVERNIGHT = 3, RUSH = 2, STANDARD = 1 };  //standard, overnight, rush
class Order {
private:
    string customerName;//indicate who bought it
    string productName;
    double price;
    time_t dateOfPurchase;
    bool isDelivered; // already shipped
    SHIPPING_TYPE shippingType;
    // List<Product> shoppingBag; //add the products to the "shoppingBag"list.

public:
    Order(const string& customerName, const string& productName, double price, time_t dateOfPurchase,
    		SHIPPING_TYPE shippingType);
    // instantiates new Order

    /**access functions*/
    int getQuantity() const;
    // returns the number (quantity) of the product

    const string& getCustomerName() const;

    const string& getProductName() const;
    // returns the name of the product

    void setToShipped();
    // set whether the order is already shipped

    SHIPPING_TYPE getShippingType() const;
    // returns the shipping type. Overnight, Rush, Standard

    bool isShipped() const;
    // returns whether the order is already shipped

    time_t getDateOfPurchase() const;
    string getDateOfPurchaseString() const;
    // returns the date customer purchased the product

    double getPrice() const {
    	return price;
    }

    /**manipulation procedures*/
    void setShippingType(SHIPPING_TYPE shippingType);
    // set the shipping type. Overnight, rush, standard

    void printOrder() const;
    // prints the information about a customer’s order

    void printPurchaseTime(ostream&);
    // prints the time of purchase

    bool operator<(const Order &order);
    bool operator>(Order* order);
    bool operator==(const Order &order);
    friend ostream& operator<<(ostream& os, const Order& order);
};

Order::Order(const string& customerName, const string& productName, double price, time_t dateOfPurchase,
		SHIPPING_TYPE shippingType)
	: customerName(customerName), productName(productName), price(price),
	  dateOfPurchase(dateOfPurchase), isDelivered(false), shippingType(shippingType) {}

time_t Order::getDateOfPurchase() const {
	return dateOfPurchase;
}

string Order::getDateOfPurchaseString() const {
    string ret(ctime(&dateOfPurchase));
    while (ret[ret.size() - 1] == '\n') ret.resize(ret.size() - 1);
    return ret;
}

const string& Order::getCustomerName() const {
	return customerName;
}

const string& Order::getProductName() const {
    return productName;
}

bool Order::isShipped() const {
    return isDelivered;
}

void Order::setToShipped() {
    this->isDelivered = true;
}

SHIPPING_TYPE Order::getShippingType() const {
    return shippingType;
}

void Order::setShippingType(SHIPPING_TYPE shippingType) {
    this->shippingType = shippingType;
}

void Order::printOrder() const
{
    cout << productName << " was purchased on "
    		<< getDateOfPurchaseString() << endl;
    cout << "Total Price: $" << price << endl;

}

bool Order::operator<(const Order &order)
{
    if (shippingType < order.shippingType)
        return true;
    else if (shippingType == order.shippingType && dateOfPurchase < order.dateOfPurchase)
        return true;
    else if (shippingType == order.shippingType && dateOfPurchase == order.dateOfPurchase)
        return false;
    else
        return false;
}

bool Order::operator>(Order* order)
{
    if (shippingType > order->shippingType)
        return true;
    else if (shippingType == order->shippingType)
    {
    	if( dateOfPurchase > order->dateOfPurchase)
    		return true;
    	else
    		return false;
    }
    else
        return false;
}

bool Order::operator==(const Order& order) {
    return (shippingType == order.shippingType && dateOfPurchase == order.dateOfPurchase);
}

ostream& operator<<(ostream& os, const Order* order)
{
	os << "Product Name: "<< order->getProductName() << endl;
	os << "Purchased on: " << order->getDateOfPurchaseString() << endl;
	os << "Shipping Priority: ";
	switch(order->getShippingType())
	{
		case STANDARD : os << "STANDARD" << endl;
		break;
		case OVERNIGHT : os << "OVERNIGHT" << endl;
		break;
		case RUSH : os << "RUSH" << endl;
		break;
	}
	if (order->isShipped())
	{
		os << "Status: "<< "Shipped" << endl;
	}
	else
	{
		os << "Status: "<< "Not Shipped" << endl;
	}

    return os;
}

#endif /* ORDER_H_ */
