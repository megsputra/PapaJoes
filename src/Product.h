#ifndef PRODUCT_H_
#define PRODUCT_H_

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Product {
private:
	string productID; ///Added by Ting
    string name;
    double price;
    string developer;
    string genre;
    int numRating;
    double totalRating;
    int stock;
    string description;


public:
    void rate (double rating) {
    	totalRating += rating;
    	numRating++;
    }

    Product(string productID, string name, double price, string developer, string genre, int numRating, double totalRating, int stock, string describtion); //Added one string by Ting
    //create a product based on input file

    Product(const string& key, bool isId=true);
    //look for a product by ID or by name.

    /**access functions*/
    double getPrice() const;
    //returns the price of a product

    const string& getGenre() const;
    //returns the genre of a product

    int getStock() const;
    //returns the stock of a product

    double getRating() const;
    //returns the rating of a product

    const string& getDescription() const;
    //returns the description of a product

    const string& getDeveloper() const;
    //returns the developer of a product

    const string& getName() const;
    //returns the name of a product

    string getFieldS(char) const;
    //return all string info of a product

    double getFieldN(char) const;
    //return all number info of a product

    const string& getProductID() const;
    //returns the product ID of a product

    /**manipulation procedures*/
    void setPrice(double price);
    //initialize a value for the product’s price

    void setGenre(const string& genre);
    //initialize a value for the product’s genre

    void setStock(double stock);
    //initialize a value for the product’s stock

    void addRating(double rating);
    //pre 0 <= rating <= 5
    //initialize a value for the product’s rating

    void setDescription(const string& description);
    //initialize a value for the product’s description

    void setDeveloper(const string& dev);
    //initialize a value for the product’s developer

    void setName(const string& name);
    //initialize a value for the product’s name

    void setProductID(const string& id);
    //initialize a value for the product’s ID

    /**additional functions*/
    void printProduct() const;
    //prints product information

    bool operator<(const Product &pdt);
    bool operator>(const Product &pdt);
    bool operator==(const Product &pdt);
    void toFile(ofstream& of);

    friend ostream& operator<<(ostream& os, const Product& pdt);
};

Product::Product(string id, string name, double price, string developer,
		string genre, int nRating, double tRating, int stock, string description)
: productID(id), name(name), price(price), developer(developer),
  genre(genre), numRating(nRating), totalRating(tRating), stock(stock), description(description){}

Product::Product(const string& key, bool isId)
	: productID(isId ? key : ""), name(isId ? "" : key), price(0.0), developer(""),
	  genre(""), numRating(0), totalRating(0.0), stock(0), description("") {}

string Product::getFieldS(char c) const {
	switch(c){
			case 'D':
			case 'd':
				return developer;
			case 'G':
			case 'g':
				return genre;
	}
	return "Error: FieldS is not found!!!";
}

double Product::getFieldN(char c) const {
	switch(c){
			case 'R':
			case 'r':
				return (numRating == 0) ? 0 : totalRating / numRating;
			case 'C':
			case 'c':
				return price;
	}
	return -1;
}

const string& Product::getProductID() const {
	return productID;
}

void Product::setProductID(const string& id) {
	this->productID = id;
}

double Product::getPrice() const {
    return price;
}

void Product::setPrice(double price) {
    this->price = price;
}

const string& Product::getGenre() const {
    return genre;
}

void Product::setGenre(const string& genre) {
    this->developer = genre;
}
int Product::getStock() const {
    return stock;
}

void Product::setStock(double stock) {
    this->stock = stock;
}

double Product::getRating() const {
    return (numRating == 0) ? 0 : totalRating/numRating;
}

void Product::addRating(double rating) {
	numRating++;
	totalRating += rating;
}

const string& Product::getDescription() const {
    return description;
}

void Product::setDescription(const string& description) {
    this->description = description;
}

const string& Product::getDeveloper() const {
    return developer;
}

void Product::setDeveloper(const string& dev) {
    this->developer = dev;
}

const string& Product::getName() const {
    return name;
}

void Product::setName(const string& name) {
    this->name = name;
}

/*void Product::printProduct() const
{
    cout << "Game: " << name
   		 << "\nPrice: " << price
    	 << "\nDeveloper: " << developer
		 << "\nGenre: " << genre
		 << "\nStock: " << stock
		 << "\nRating: " << totalRating
		 << "\nDescription: " << description << endl;
}
*/

bool Product::operator<(const Product &pdt)
{
	return productID < pdt.productID;
}

bool Product::operator>(const Product &pdt)
{
	return productID > pdt.productID;
}

bool Product::operator==(const Product& pdt) {
    return (productID == pdt.productID);
}

void Product::toFile(ofstream& of)
{
    of << productID << "\t" << name << "\t" << price << "\t" << developer << "\t" << genre
       << "\t"  << numRating << "\t" << totalRating << "\t" << stock << "\t"  << description
	   << endl;
}

ostream& operator<<(ostream& os, const Product& pdt)
{
    os << "\n~~~GAME INFO~~~"
	   << "\nProductID: " << pdt.productID
       << "\nProductName: " << pdt.name
	   << "\nPrice: " << pdt.price
       << "\nDeveloper: " << pdt.developer
	   << "\nGenre: " << pdt.genre
	   << "\nRating: " << pdt.getRating()
	   << "\nStock: " << pdt.stock
	   << "\nDescription: " << pdt.description << endl;
    return os;
}

#endif /* PRODUCT_H_ */
