#include "HashTable.h"
#include "Customer.h"
#include "Employee.h"
#include "BST.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

struct Context;

void readDataFromFile(Context& context);
void writeDataToFile(Context& context);
void parseData(const string& buffer, int numColumns, vector<string>& tokens);
void welcomeScreen(Context* context);
void guestScreen(Context* context);
void customerScreen(Context* context);
void employeeScreen(Context* context);
void registerScreen(Context* context, bool wasGuest = false);
void loginScreen(Context* context, bool isCustomer, bool wasGuest = false);
void browseScreen(Context* context);
void searchScreen(Context* context);
void viewProduct(Context* context);
void viewCart(Context* context);
void viewPurchase(Context* context);
void viewAccount(Context* context);
void manageProduct(Context* context);
void manageOrder(Context* context);
void manageUser(Context* context);
string getPassword();
void addCart(Context* context);
void recommendation(Context* context);
void delay(int secs);

struct Context {
	BST inventoryPrimary;
	BST inventorySecondary;

	HashTable<Customer> customers;
	HashTable<Employee> emps;
	vector<Order*> orders;
	PriorityQueue PQ;
	Customer* currentCustomer;
	Employee* currentEmployee;

	Context() : inventoryPrimary(true), inventorySecondary(false),
			currentCustomer(NULL), currentEmployee(NULL) {}
};

int main()
{
	cout << "============================================================" << endl;
    cout << "      _=====_            WELCOME TO          _=====_" <<endl;
    cout << "     / _____ \\         PAPAJOE'S SHOP       / _____ \\" "" <<endl;
    cout << "    +.-'_____'-.---------------------------.-'_____'-.+" <<endl;
    cout << "   /   |     |  '.         ©KMTT          .' |  _  |   \\" "" <<endl;
    cout << "  / ___| /|\\ |___ \\         2017        /____| /_\\ |___ \\" "" <<endl;
    cout << " / |      |      | ;  __           _   ; | _         _ | ;" <<endl;
    cout << " | | <---   ---> | | |__|         |_:> | ||_|       (_)| |" <<endl;
    cout << " | |___   |   ___| ;SELECT       START ; |___       ___| ;" <<endl;
    cout << " |\\    | \\|/ |    /  _     ___      _   \\    | (X) |    /" <<endl;
    cout << " | \\   |_____|  .','\" \"', |___|  ,'" "', '. \\   |_____| .' |" <<endl;
    cout << " |  '-.______.-' /       \\ANALOG/       \\  '-.____.-'   |" <<endl;
    cout << " |               |       |------|       |               |" <<endl;
    cout << " |              /\\       /      \\       /\\              |" <<endl;
    cout << " |             /  '.___.'        '.___.'  \\             |" <<endl;
    cout << " |            /                            \\            |" <<endl;
    cout << "  \\         /                               \\          /" <<endl;
    cout << "   \\_______/                                 \\________/" <<endl;
	cout << "============================================================" << endl;
	cout << endl;

    Context context;
    readDataFromFile(context);
    welcomeScreen(&context);
    writeDataToFile(context);
    return 0;
}

void delay(int secs)
{
    for (long i=0; i<60000000*secs; i++ ) ;
}

bool GetLine(istream& input, string& line) {
	bool ret = getline(input, line);
	while (line[line.size() - 1] == '\r') line.resize(line.size() - 1);
	return ret;
}

void printList(List<Product*> l) {
	for (l.beginIterator(); !l.offEnd(); l.advanceIterator()) {
		cout << **l.getIterator() << endl;
	}
}

void openInfile(const string& fileName, ifstream& fin)
{
	fin.open(fileName.c_str());
	if (!fin) {
		cerr << "Unable to open source file: " << fileName << endl;
		exit(-1);
	}
}

void openOutfile(const string& fileName, ofstream& fout)
{
	fout.open(fileName.c_str());
	if (!fout) {
		cerr << "Unable to open destination file: " << fileName << endl;
		exit(-2);
	}
}

void readDataFromFile(Context& context)
{
	ifstream fin;
	string line;

	openInfile("productData.tsv", fin);
	GetLine(fin, line);
    while (GetLine(fin, line)) {
    	vector<string> tokens;
    	parseData(line, 10, tokens);
    	Product* newProduct = new Product(tokens[0], tokens[1], atof(tokens[2].c_str()), tokens[3], tokens[4],
    			                          atoi(tokens[5].c_str()), atof(tokens[6].c_str()), atoi(tokens[7].c_str()), tokens[8]);
    	context.inventorySecondary.insertData(newProduct);
    	context.inventoryPrimary.insertData(newProduct);
    }
    fin.close();

    openInfile("customerData.tsv", fin);
    GetLine(fin, line);
	while (GetLine(fin, line)) {
		vector<string> tokens;
		parseData(line, 10, tokens);
		context.customers.insertData(
				Customer(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4],
						 tokens[5], tokens[6], tokens[7], tokens[8], tokens[9]));
	}
    fin.close();

    openInfile("employeeData.tsv", fin);
    GetLine(fin, line);
	while (GetLine(fin, line)) {
		vector<string> tokens;
		parseData(line, 2, tokens);
		context.emps.insertData(Employee(tokens[0], tokens[1]));
	}
    fin.close();

    openInfile("orderData.tsv", fin);
    GetLine(fin, line);
	while (GetLine(fin, line)) {
		vector<string> tokens;
		parseData(line, 6, tokens);
		context.orders.push_back(new Order(tokens[0], tokens[1], atof(tokens[2].c_str()),
								 static_cast<time_t>(atol(tokens[3].c_str())),
								 static_cast<SHIPPING_TYPE>(atoi(tokens[5].c_str()))));
		Order* order = context.orders.back();
		if (atoi(tokens[4].c_str())) {
			order->setToShipped();
		} else {
			context.PQ.heapInsert(order);
		}
		Customer tmp;
		tmp.setCustomerID(order->getCustomerName());
		Customer* customer = context.customers.searchData(tmp);
		assert(customer != NULL);
		customer->addToOrderHistory(order);
	}
    fin.close();

    openInfile("cartData.tsv", fin);
    GetLine(fin, line);
	while (GetLine(fin, line)) {
		vector<string> tokens;
		parseData(line, 2, tokens);
		Customer tmp;
		tmp.setCustomerID(tokens[0]);
		Customer* customer = context.customers.searchData(tmp);
		assert(customer != NULL);
		Product tmpProduct(tokens[1]);
		Product* product = context.inventoryPrimary.search(&tmpProduct);
		assert(product != NULL);
		customer->addToCart(product);
	}
    fin.close();
}

void writeDataToFile(Context& context) {
	ofstream fout, cartFout;

	openOutfile("customerData.tsv", fout);
	openOutfile("cartData.tsv", cartFout);
	fout << "CustomerID\tPassword\tFirstName\tLastName\tEmail\tPhone\tAddress\tCity\tState\tZip Code" << endl;;
	cartFout << "CustomerID\tProductID" << endl;
	for (int i = 0; i < HashTable<Customer>::SIZE; i++) {
		List<Customer> customers = context.customers.getBucket(i);
		for (customers.beginIterator(); !customers.offEnd(); customers.advanceIterator()) {
			Customer* customer = customers.getIterator();
			fout << customer->getCustomerID() << "\t" << customer->getPassword()
				 << "\t" << customer->getFirstName() << "\t" << customer->getLastName()
				 << "\t" << customer->getEmail() << "\t" << customer->getPhoneNum()
				 << "\t" << customer->getStreet() << "\t" << customer->getCity()
				 << "\t" << customer->getState() << "\t" << customer->getZip() << endl;

			List<Product*> cart = customer->getCart();
			for (cart.beginIterator(); !cart.offEnd(); cart.advanceIterator()) {
				Product** p = cart.getIterator();
				cartFout << customer->getCustomerID() << "\t" << (*p)->getProductID() << endl;
			}
		}
	}
	cartFout.close();
	fout.close();

	// Now can delete orders because customer data have been written.
	openOutfile("orderData.tsv", fout);
	fout << "CustomerID\tProductName\tPrice\tDate\tIsDelivered\tShippingType" << endl;
	for (int i = 0; i < context.orders.size(); i++) {
		Order* order = context.orders[i];
		fout << order->getCustomerName() << "\t" << order->getProductName()
				<< "\t" << order->getPrice() << "\t" << order->getDateOfPurchase()
				<< "\t" << order->isShipped() << "\t" << order->getShippingType() << endl;
		// Release the Order data allocated on heap.
		delete order;
	}
	fout.close();

	openOutfile("productData.tsv", fout);
	fout << "Product\tID\tName\tPrice\tDeveloper\tGenres\tNumRating\tTotalRating\tStock\tDescription" << endl;
	context.inventoryPrimary.inOrderToFile(fout);
	fout.close();
	// Release the Product data allocated on heap.
	context.inventoryPrimary.inOrderReleaseData();

	openOutfile("employeeData.tsv", fout);
	fout << "User name\tPassword" << endl;;
	for (int i = 0; i < HashTable<Employee>::SIZE; i++) {
		List<Employee> employees = context.emps.getBucket(i);
		for (employees.beginIterator(); !employees.offEnd(); employees.advanceIterator()) {
			Employee* employee = employees.getIterator();
			fout << employee->getUserName() << "\t" << employee->getPassword() << endl;
		}
	}
	fout.close();
}

void parseData(const string& buffer, int numColumns, vector<string>& tokens)
{
	static const char SEPARATOR = '\t';
	size_t s1 = 0, s2 = 0;

	for (int i = 0; i < numColumns; i++) {
		s2 = i > 0 ? s1 + 1 : s1;
		s1 = buffer.find(SEPARATOR, s2);
		tokens.push_back(buffer.substr(s2, s1 - s2));
	}
}

void welcomeScreen(Context* context)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	char input = 'q';
	do {
		cout << "\nPlease select a role OR quit the program: " << endl
			 << "\t- 'g': guest" << endl
			 << "\t- 'c': customer" << endl
			 << "\t- 'e': employee" << endl
			 << "\t- 'q': quit" << endl
			 << "Your choice: ";
		cin >> input;
		cin.ignore(80, '\n');
		switch(input) {
		case 'g':
		case 'G':
			guestScreen(context);
			break;
		case 'c':
		case 'C':
			loginScreen(context, /*isCustomer=*/true);
			break;
		case 'e':
		case 'E':
			loginScreen(context, /*isCustomer=*/false);
			break;
		}
	} while (input != 'q' && input != 'Q');
	cout << endl;

	cout << "Saving all data into file & closing program......" << endl << endl;
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
    cout << "\n\n~*~*~ Thanks for visiting Papa Joe's Online Shop! ~*~*~" << endl
    	 <<	"~*~*~ Hope you enjoy your shopping. Bye! ~*~*~" << endl << endl;
}

void guestScreen(Context* context)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	char input = 'w';
	do {
		if (context->currentCustomer) {
			customerScreen(context);
			return;
		}
		cout << "\nWelcome to guest screen!" << endl
			 << "Please enter your choice: " << endl
			 << "\t- 'b': browse product" << endl
			 << "\t- 's': search product" << endl
			 << "\t- 'r': register" << endl
			 << "\t- 'w': back to welcome screen" << endl
			 << "Your choice: ";
		cin >> input;
		cin.ignore(80, '\n');
		switch(input) {
		case 'b':
		case 'B':
			browseScreen(context);
			break;
		case 's':
		case 'S':
			searchScreen(context);
			break;
		case 'r':
		case 'R':
			registerScreen(context);
			break;
		}
		// Guest has registered and logged in.
		if (context->currentCustomer != NULL) {
			customerScreen(context);
			return;
		}
	} while (input != 'w' && input != 'W');
}

void customerScreen(Context* context)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	char input = 'o';
	do {
		cout << "\nWelcome to customer screen, " << context->currentCustomer->getCustomerID() << "! "<< endl
			 << "Please enter your choice: " << endl
			 << "\t- 'b': browse" << endl
			 << "\t- 's': search" << endl
			 << "\t- 'r': get a game recommendation" << endl
			 << "\t- 'c': view cart" << endl
			 << "\t- 'p': purchase history" << endl
			 << "\t- 'a': view account" << endl
			 << "\t- 'o': logout" << endl
			 << "Your choice: ";
		cin >> input;
		cin.ignore(80, '\n');
		switch(input) {
		case 'b':
		case 'B':
			browseScreen(context);
			break;
		case 's':
		case 'S':
			searchScreen(context);
			break;
		case 'r':
		case 'R':
			recommendation(context);
			break;
		case 'c':
		case 'C':
			viewCart(context);
			break;
		case 'p':
		case 'P':
			viewPurchase(context);
			break;
		case 'a':
		case 'A':
			viewAccount(context);
			break;
		}
	} while (input != 'o' && input != 'O');
	context->currentCustomer = NULL;
}

void recommendation(Context* context)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	cout << "\nIn this screen you can get recommendation for buying products."<< endl;
	List<Order*> orders = context->currentCustomer->getOrderHistory();
	orders.beginIterator();
	if (orders.getLength() == 0) {
		printList(context->inventoryPrimary.browse());
		return;
	}

	List<string> pNames;
	srand (time(NULL));
	int random = rand() % orders.getLength() +1;
	for(int i=0; i < orders.getLength(); i++) {
		pNames.insertBegin((*orders.getIterator())->getProductName());
		orders.advanceIterator();
	}
	pNames.advanceToIndex(random);
	Product max(*pNames.getIterator(), false);
	max = *context->inventorySecondary.search(&max);
	List<Product*> games = context->inventoryPrimary.searchString('g', max.getGenre());
	games.beginIterator();
	int i=0;
	do {
		max = **games.getIterator();
		games.advanceIterator();
		i++;
	} while (pNames.linearSearch(max.getName()) != -1);
	for(; i < games.getLength(); i++) {
		if(max.getRating() < (*games.getIterator())->getRating() && pNames.linearSearch((*games.getIterator())->getName()) == -1)
			max = **games.getIterator();
		games.advanceIterator();
	}
	cout << "Base on order history, we think you might be interested in buying: " << endl;
	cout << max << endl;
}

void employeeScreen(Context* context)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	char input = 'o';
	do {
		cout << "\nWelcome to employee screen, " << context->currentEmployee->getUserName() << "! "<< endl
			 << "Please enter your choice: " << endl
			 << "\t- 's': search product" << endl
			 << "\t- 'p': product management" << endl
			 << "\t- 'm': order management" << endl
			 << "\t- 'u': user management" << endl
			 << "\t- 'o': logout" << endl
			 << "Your choice: ";
		cin >> input;
		cin.ignore(80, '\n');
		switch(input) {
		case 's':
		case 'S':
			searchScreen(context);
			break;
		case 'p':
		case 'P':
			manageProduct(context);
			break;
		case 'm':
		case 'M':
			manageOrder(context);
			break;
		case 'u':
		case 'U':
			manageUser(context);
			break;
		}
	} while (input != 'o' && input != 'O');
	context->currentEmployee = NULL;
}

void registerScreen(Context* context, bool wasGuest)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	cout << "\nWelcome to registration!" << endl;
	cout << "Please fill in following informations to sign up." << endl;
	string customerID;
	string password;
	string firstName;
	string lastName;
	string email;
	string phoneNum;
	string address;
	string city;
	string state;
	string zip;

	bool registered = false;
	do {
		cout << "\nPlease enter user name: ";
		cin >> customerID;
		cin.ignore(80, '\n');
		Customer tmp;
		tmp.setCustomerID(customerID);
		Customer* customer = context->customers.searchData(tmp);
		if (customer != NULL) {
			cout << "\nError: User name - \""<< customerID << "\" is already taken!" << endl;
			continue;
		}
		cout << "Password: ";
		GetLine(cin, password);
		cout << "First name: ";
		GetLine(cin, firstName);
		cout << "Last name: ";
		GetLine(cin, lastName);
		cout << "E-mail: ";
		GetLine(cin, email);
		cout << "Phone number: ";
		GetLine(cin, phoneNum);
		cout << "Street address: ";
		GetLine(cin, address);
		cout << "City: ";
		GetLine(cin, city);
		cout << "State: ";
		GetLine(cin, state);
		cout << "Zip code: ";
		GetLine(cin, zip);
		registered = true;
	} while (!registered);
	context->customers.insertData(Customer(customerID, password, firstName, lastName, email, phoneNum, address, city, state, zip));
	cout << "\nAccount successfully created!" << endl;

	loginScreen(context, /*isCustomer=*/true, wasGuest);
}

void loginScreen(Context* context, bool isCustomer, bool wasGuest)
{
	do {
		for (int i = 0; i < 12; i++) {
			cout << ">>>>>" << std::flush;
			delay(1);
		}
		cout << "\nHi " << (isCustomer ? "Customer" : "Employee")
			 << "! " << endl
			 << "Please enter User name and password to log in." << endl;
		string userID = "";
		cout << "User name: ";
		cin >> userID;
		cin.ignore(80, '\n');
		cout << "Password: ";
		string password = getPassword();
		if (isCustomer) {
			Customer tmp;
			tmp.setCustomerID(userID);
			context->currentCustomer = context->customers.searchData(tmp);
			if (!context->currentCustomer || context->currentCustomer->getPassword() != password) {
				cout << "\nError: Customer login failed, please try again!" << endl;
				continue;
			}
			break;
		} else {
			Employee tmp;
			tmp.setUserName(userID);
			context->currentEmployee = context->emps.searchData(tmp);
			if(!context->currentEmployee || context->currentEmployee->getPassword() != password) {
				cout << "\nError: Employee login failed, please try again!" << endl;
				continue;
			}
			break;
		}
	} while (true);
	if (wasGuest) return;
	isCustomer ? customerScreen(context) : employeeScreen(context);
}

//This is a helper function for turning the entered password into asterisks.
//It can be compiled here, but not correctly shown in eclipse, will be shown in terminal later.
//Reference: http://www.cplusplus.com/articles/E6vU7k9E/#UNIX-e1
int getch() {
    int ch;
    struct termios t_old, t_new;

    tcgetattr(STDIN_FILENO, &t_old);
    t_new = t_old;
    t_new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
    return ch;
}

//This is a helper function for turning the entered password into asterisks.
//It can be compiled here, but not correctly shown in eclipse, will be shown in terminal later.
string getPassword() {
  const char BACKSPACE = 127;
  const char RETURN = 10;

  string password;
  unsigned char ch = 0;

  while( (ch = getch()) != RETURN) {
	  if (ch == BACKSPACE) {
		  if (password.length() != 0) {
			  cout <<"\b \b";
			  password.resize(password.length() - 1);
		  }
	  } else {
		  password += ch;
		  cout << '*';
	  }
  }
  cout << endl;
  return password;
}

void browseScreen(Context* context)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	printList(context->inventoryPrimary.browse());
	if (context->currentEmployee == NULL) {
		addCart(context);
	}
}

void addCart(Context* context)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	string input;
	do {
		cout << "\nTo add product to cart, please enter one product ID OR -'b' for back:" << endl;
		cout << "Your choice: ";
		cin >> input;
		cin.ignore(80, '\n');
		if (input == "b" || input == "B")
			return;
		else {
			if (!context->currentCustomer){
				cout << "Sorry, you must be registered as a customer before adding your product into cart." << endl;
				registerScreen(context, true);
			}
			Product tmpProduct(input);
			Product* product = context->inventoryPrimary.search(&tmpProduct);
			if (product != NULL) {
				context->currentCustomer->addToCart(product);
				cout << "\nProduct - " << product->getName() << " has been added to cart!" << endl;
				return;
			} else
				cout << "\nError: Product is not found!!!" << endl;
		}
	} while(true);
}

void searchScreen(Context* context)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	char input = 'b';
	string inputS;
	double low, high;
	do {
		bool foundProduct = false;
		cout << "\nIn this screen you can search the products."<< endl
			 << "Please select searching method:"<< endl
			 << "\t- 'i': search by product id" << endl
			 << "\t- 'n': search by game name" << endl
			 << "\t- 'd': show all games made by a certain developer" << endl
			 << "\t- 'r': show all games with a certain rating" << endl
			 << "\t- 'g': show all games in certain genre" << endl
			 << "\t- 'p': show all games in a certain price range" << endl
			 << "\t- 'b': go back to previous screen" << endl
			 << "Your choice: ";
		cin >> input;
		cin.ignore(80, '\n');
		switch(input) {
			case 'i':
			case 'I': {
				cout << "\n~~~~~~The rule of product ID (5 digits)~~~~~~" << endl
					 << "Genre:   1 - Action" << endl
					 << "\t 2 - Adventure" << endl
					 << "\t 3 - RPG" << endl
					 << "\t 4 - Simulation" << endl
					 << "\t 5 - Strategy" << endl
					 << "\t 6 - Sports" << endl
					 << "\t 7 - Others" << endl
					 << "Number: 01 - 25" << endl
					 << "Release year: 17 for 2017" << endl;
				cout << "\nPlease enter product ID (genre + seriesNumber + releaseYear): ";
				cin >> inputS;
				cin.ignore(80, '\n');
				Product tmpProduct(inputS);
				Product* product = context->inventoryPrimary.search(&tmpProduct);
				if (product != NULL) {
					cout << *product << endl;
					foundProduct = true;
				}
				else cout << "\nError: Product is not found!!!" << endl;
				break;}
			case 'N':
			case 'n': {
				cout << "\nPlease enter product name (e.g. Overwatch/NBA 2K17): ";
				GetLine(cin, inputS);
				Product tmp(inputS, /*isId=*/false);
				Product* product = context->inventorySecondary.search(&tmp);
				if (product != NULL) {
					cout << *product << endl;
					foundProduct = true;
				}
				else cout << "\nError: Product is not found!!!" << endl;
				break;}
			case 'D':
			case 'd':
			case 'g':
			case 'G': {
				cout << "\nGenre: 'Action'/'Adventure'/'RPG'/'Simulation'/'Strategy'/'Sports'/'Others';" << endl
					 << "Developer: e.g.'Blizzard Entertainment';" << endl;
				cout << "\nPlease enter your input:" << endl;
				GetLine(cin, inputS);
				List<Product*> products = context->inventoryPrimary.searchString(input, inputS);
				if (!products.empty()) {
					printList(products);
					foundProduct = true;
				}
				else cout << "\nError: No product found!!!" << endl;
				break;}
			case 'R':
			case 'r':
			case 'P':
			case 'p': {
				cout << "\nPlease enter the low boundary (0.0 <= Price) OR (0.0 <= Rating <= 5.0): ";
				cin >> low;
				cin.ignore(80, '\n');
				cout << "Please enter the high boundary (0.0 <= Price) OR (0.0 <= Rating <= 5.0): ";
				cin >> high;
				cin.ignore(80, '\n');
				List<Product*> products = context->inventoryPrimary.searchNum(input, low, high);
				if (!products.empty()) {
					printList(products);
					foundProduct = true;
				}
				else cout << "\nError: No product found!!!" << endl;
				break;}
			case 'B':
			case 'b':
				return;
		}
		if (context->currentEmployee == NULL && foundProduct) {
			addCart(context);
		}
	} while (input != 'b' && input != 'B');
}

void viewProduct(Context* context)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	char choice;
	cout << "\nIn this screen you can view the products by primary key OR secondary key."<< endl
		 << "Please enter ('p' to view by primary key) " << endl
		 << "OR enter ('s' to view by secondary key)" << endl
		 << "Your choice: ";
	cin >> choice;
	cin.ignore(80, '\n');
	if (choice == 'p' || choice == 'P')
		context->inventoryPrimary.inOrderPrint(cout);
	else if (choice == 's' || choice == 's')
		context->inventorySecondary.inOrderPrint(cout);
}

Product* checkCart(Product* tmp, Context* context) {
	List<Product*> cart = context->currentCustomer->getCart();
		cart.beginIterator();
	for (int i = 0; i < cart.getLength(); i++) {
		if(**cart.getIterator() == *tmp)
			return *cart.getIterator();
		cart.advanceIterator();
	}
	return NULL;
}

void viewCart(Context* context)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	string input;
	char chosenShippingType;
	do {
		cout << "\nHere is your shopping cart: ";
		if (context->currentCustomer->getCart().empty()) {
			cout << "EMPTY" << endl;
			return;
		}
		cout << endl;
		context->currentCustomer->printMyCart();
		cout << "To select one of your item, please enter ('productID' OR 'b' - back):" << endl;
		cout << "Your choice: ";
		cin >> input;
		cin.ignore(80, '\n');
		if (input == "b" || input == "B" )
			break;
		else {
			char choice;
			Product tmpProduct(input);
			Product* tmp = checkCart(&tmpProduct, context);
			if (tmp == NULL) {
				cout << "\nError: Product is not in cart!!!" << endl;
				continue;
			}
			cout << "\nPlease choose ('p' - purchase OR 'd' - delete) for this item: " << endl;
			cout << "Your choice: ";
			cin >> choice;
			cin.ignore(80, '\n');
			if (choice == 'd' || choice == 'D') {
				context->currentCustomer->deleteItemFromCart(tmp);
				cout << "Product - " << tmp->getName() << " has been successfully deleted from your cart!" << endl;
				continue;
			}
			if (tmp->getStock() < 1) {
				cout << "Sorry, the chosen product is currently out of stock!" << endl;
				return;
			}
			tmp->setStock(tmp->getStock() - 1);
			cout << "\nPlease choose a shipping type for your order:" << endl
				 << "\t- 'o': Overnight" << endl
				 << "\t- 'r': Rush" << endl
				 << "\t- 's': Standard" << endl
				 << "Your choice: ";
			cin >> chosenShippingType;
			cin.ignore(80, '\n');
			SHIPPING_TYPE shippingType = STANDARD;
			switch (chosenShippingType) {
				 case 'o':
				 case 'O': {
					 cout << "Overnight shipping has been selected." << endl;
					 shippingType = OVERNIGHT;
					 break;
				 }
				 case 'r':
				 case 'R': {
					 cout << "Rush shipping has been selected." << endl;
					 shippingType = RUSH;
					 break;
				 }
				 default: {
					 cout << "Standard shipping has been selected." << endl;
					 break;
				 }
			 }
			time_t now;
			time(&now);
			context->orders.push_back(new Order(context->currentCustomer->getCustomerID(),
					tmp->getName(), tmp->getPrice(), now, shippingType));
			Order* order = context->orders.back();
			context->PQ.heapInsert(order);
			context->currentCustomer->deleteItemFromCart(tmp);
			context->currentCustomer->addToOrderHistory(order);
			cout << "Order - " << tmp->getName() << " has been placed!" << endl;
		}
	} while (true);
}

void viewPurchase(Context* context)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	cout << "\nHere is your purchase history:" << endl;
	context->currentCustomer->printOrderHistory();

	List<Order*> orders = context->currentCustomer->getOrderHistory();
	orders.beginIterator();
	if (orders.getLength() != 0) {
		char choice;
		cout << "\nWould you like to rate your purchase? (-'y': yes OR -'n': no) " << endl;
		cout << "Your choice: ";
		cin >> choice;
		cin.ignore(80, '\n');
		while (choice != 'n' && choice != 'n') {
			string productName;
			cout << "Please enter product name: ";
			GetLine(cin, productName);
			Product tmp(productName, /*isId=*/false);
			Product* product = context->inventorySecondary.search(&tmp);
			if (product != NULL) {
				double rating;
				cout << "Please enter your rating (0.0 - 5.0): ";
				cin >> rating;
				cin.ignore(80, '\n');
				product->rate(rating);
				cout << "Thank you for your rating!" << endl;
				choice = 'n';
			} else {
				cout << "Error: Rate product - Product not found!!!" << endl << endl;
			}
		}
	}
}

void viewAccount(Context* context)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	cout << "\nHere is your account information:" << endl;
	context->currentCustomer->displayAccountInfo();
	cout << endl;

	char choice = 'b';
	cout << "\nWould you like to ('e' - edit account OR 'b' - back to previous screen)? " << endl;
	cout << "Your choice: ";
	cin >> choice;
	cin.ignore(80, '\n');
	if (choice == 'e' || choice == 'E') {
    	char choice;
		string input;
		cout << "\nPlease select a field to edit OR 'b' - back to previous screen. " << endl
			 << "\t- 'f': first name - " << context->currentCustomer->getFirstName() << endl
			 << "\t- 'l': last name - " << context->currentCustomer->getLastName() << endl
			 << "\t- 'p': password - " << context->currentCustomer->getPassword() << endl
			 << "\t- 'e': e-mail - " << context->currentCustomer->getEmail() << endl
			 << "\t- 'n': phone number - " << context->currentCustomer->getPhoneNum() << endl
			 << "\t- 'a': address - " << context->currentCustomer->getStreet() << ", " << context->currentCustomer->getCity() << ", " << context->currentCustomer->getState() << " " << context->currentCustomer->getZip() << endl
			 << "\t- 'b': back to customer screen" << endl
			 << "Your choice: ";
		cin >> choice;
		cin.ignore(80, '\n');

		if (choice != 'b' && choice != 'B') {
			switch(choice)
			{
				case 'f':
				case 'F':
					cout << "\nType new first name: ";
					GetLine(cin, input);
					context->currentCustomer->setFirstName(input);
					break;
				case 'l':
				case 'L':
					cout << "\nType new last name: ";
					GetLine(cin, input);
					context->currentCustomer->setLastName(input);
					break;
				case 'p':
				case 'P':
					cout << "\nType new password: ";
					GetLine(cin, input);
					context->currentCustomer->setPassword(input);
					break;
				case 'e':
				case 'E':
					cout << "\nType new e-mail: ";
					GetLine(cin, input);
					context->currentCustomer->setEmail(input);
					break;
				case 'n':
				case 'N':
					cout << "\nType new phone number: ";
					GetLine(cin, input);
					context->currentCustomer->setPhoneNum(input);
					break;
				case 'a':
				case 'A':
					cout << "\nType new street address: ";
					GetLine(cin, input);
					context->currentCustomer->setStreet(input);

					cout << "Type new city: ";
					GetLine(cin, input);
					context->currentCustomer->setCity(input);

					cout << "Type new state: ";
					GetLine(cin, input);
					context->currentCustomer->setState(input);

					cout << "Type new zip code: ";
					GetLine(cin, input);
					context->currentCustomer->setZip(input);
					break;
			}
			cout << "\nYour account information(s) has been successfully updated!\n";
		}
    }
}

void manageProduct(Context* context)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	char input ='b';
	do {
		cout << "\nIn this screen you can manage the PRODUCT inventory"<< endl
			 << "Please enter your choice: " << endl
			 << "\t- 'v': view all product" << endl
			 << "\t- 'a': add a new product" << endl
			 << "\t- 'd': delete product" << endl
			 << "\t- 'c': change stock" << endl
			 << "\t- 'b': go back to previous screen" << endl
			 << "Your choice: ";
		cin >> input;
		cin.ignore(80, '\n');
		switch(input) {
			case 'v':
			case 'V':
				viewProduct(context);
				break;
			case 'a':
			case 'A': {
				string id, name, developer, discription, genre;
				double price;
				int stock;
				cout << "\nPlease enter product ID (e.g.['1-7' - Genre]+['26' - Number]+['17' - ReleaseYear]): ";
				cin >> id;
				cin.ignore(80, '\n');
				cout << "Please enter product name: ";
				GetLine(cin, name);
				cout << "Please enter product developer: ";
				GetLine(cin, developer);
				cout << "Please enter product description (in one line): ";
				GetLine(cin, discription);
				cout << "Genre: 'Action'/'Adventure'/'RPG'/'Simulation'/'Strategy'/'Sports'/'Others'" << endl;
				cout << "Please enter product genre: ";
				cin >> genre;
				cin.ignore(80, '\n');
				cout << "Please enter product price: ";
				cin >> price;
				cin.ignore(80, '\n');
				cout << "Please enter product stock: ";
				cin >> stock;
				cin.ignore(80, '\n');

				Product* newProduct = new Product(id, name, price, developer,
						genre, 0, 0.0, stock, discription);
				context->inventorySecondary.insertData(newProduct);
				context->inventoryPrimary.insertData(newProduct);
			} break;
			case 'd':
			case 'D': {
				string id;
				cout << "\nPlease enter product ID (delete item): ";
				cin >> id;
				cin.ignore(80, '\n');
				Product tmp(id);
				Product* newProduct = context->inventoryPrimary.search(&tmp);

				if (newProduct != NULL) {
					context->inventorySecondary.removeData(newProduct);
					context->inventoryPrimary.removeData(newProduct);
					cout << "Product " << id << " has been successfully deleted!" << endl;
				} else
					cout << "Error: Delete item - Product is not found!" << endl;
				break;
			}
			case 'c':
			case 'C': {
				string productID;
				int stock;
				cout << "\nPlease enter product ID (change stock): ";
				cin >> productID;
				cin.ignore(80, '\n');

				Product* newProduct = new Product(productID);
				newProduct = context->inventoryPrimary.search(newProduct);
				if (newProduct) {
					cout << "Please enter a positive OR negative number to change stock by: ";
					cin >> stock;
					cin.ignore(80, '\n');
					newProduct->setStock(newProduct->getStock() + stock);
					cout << "Product - " << newProduct->getName() << "\'s stock has been changed!" << endl;
				} else
					cout << "\nError: Change stock - Product not found!!!" << endl;
				break;
			}
		}
	} while (input != 'b' && input != 'B');
}

void manageOrder(Context* context)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	char input = 'b';
	do {
			cout << "\nIn this screen you can manage the ORDER inventory."<< endl
				 << "Please select one of the following options:"<< endl
				 << "\t- 'v': view all orders" << endl
				 << "\t- 's': ship an order" << endl
				 << "\t- 'b': back to employee screen" << endl
				 << "Your choice: ";
			cin >> input;
			cin.ignore(80, '\n');
			switch (input) {
				case 'v':
				case 'V': {
					context->PQ.showPriorityQueue(cout);
					break;
				}
				case 's':
				case 'S': {
					context->PQ.shipProduct();
					break;
				}
			}
		} while (input != 'b' && input != 'B');
}

void manageUser(Context* context)
{
	for (int i = 0; i < 12; i++) {
		cout << ">>>>>" << std::flush;
		delay(1);
	}
	string choice = "";
	cout << "\nIn this screen you can manage the user inventory."<< endl
		 << "Please enter 'all' to view the all users information" << endl
		 << "OR enter 'one' to view one specific user information" << endl
		 << "Your choice: ";
	cin >> choice;
	cin.ignore(80, '\n');
	if (choice == "all" || choice == "ALL") {
		context->customers.printTable();
	}
	else if (choice == "one" || choice == "ONE") {
		string customerID = "";
		cout << "\nPlease enter user name: ";
		getline(cin, customerID);

		Customer tmp;
		tmp.setCustomerID(customerID);
		Customer* customer = context->customers.searchData(tmp);
		if (customer == NULL) {
			cout << "\nError: User does NOT exist!" << endl;
		} else {
			customer->displayAccountInfo();
			cout << "\nUser's order history:" << endl;
			customer->printOrderHistory();
		}
	}
}
