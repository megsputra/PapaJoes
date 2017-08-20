#ifndef EMPLOYEE_H_
#define EMPLOYEE_H_

#include <string>

using namespace std;

class Employee {
private:
	string userName;
	string password;

public:
	bool operator==(const Employee& customer) {
	    return (userName == customer.userName);
	}

	string getKey() {return userName;}
	Employee() : userName(""), password("") {}
	Employee(const string& userName, const string& password);

	/**access functions*/
	const string& getUserName() const;
	// returns the customer's user name

	const string& getPassword() const;
	// returns the customer's password

	/**manipulation procedures*/
	void setUserName(const string& userName);
	//set user name for a customer

	void setPassword(const string& password);
	//sets a customer;s password

	// TODO: implement the following functions.
	void viewProduct();
	//returns the full description of a product

	void addProduct();
	//adds product to database

	void deleteProduct();
	//remove product from database

	void modifyProduct();
	//modify the members of Product object

	void confirmDelivery();
	//ships?? what does this function do? Mega 6/18

	void viewAccount();
	//displays customer account information
};

Employee::Employee(const string& userName, const string& password) : userName(userName), password(password) {}

const string& Employee::getUserName() const {
	return userName;
}

void Employee::setUserName(const string& userName) {
	this->userName = userName;
}

const string& Employee::getPassword() const{
	return password;
}
void Employee::setPassword(const string& password){
	this->password = password;
}

#endif /* EMPLOYEE_H_ */
