// Name
// Section #
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include "person.cpp"
#include "book.cpp"


using namespace std;

void printMenu() {
    cout << "----------Library Book Rental System----------" << endl;
    cout << "1.  Book checkout" << endl;
    cout << "2.  Book return" << endl;
    cout << "3.  View all available books" << endl;
    cout << "4.  View all outstanding rentals" << endl;
    cout << "5.  View outstanding rentals for a cardholder" << endl;
    cout << "6.  Open new library card" << endl;
    cout << "7.  Close library card" << endl;
    cout << "8.  Exit system" << endl;
    cout << "Please enter a choice: ";
}

// You are not obligated to use these function declarations - they're just given as examples
void readBooks(vector<Book *> & myBooks) {
    int id;
    string auth, cat, bookName;
    Book * ptr;

    ifstream inData("books.txt");

    inData >> id;
    while(!inData.eof())
    {
      inData.get();
      getline(inData, bookName);
      getline(inData, auth);
      getline(inData, cat);

      ptr = new Book(id, bookName, auth, cat);
      myBooks.push_back(ptr);

      inData.get();
      inData >> id;
    }

    inData.close();
    ptr = NULL;
}

int readPersons(vector<Person *> & myCardholders) {
    string fName, lName;
    bool act;
    int cardNo;
    Person * ptr;

    ifstream inData("persons.txt");
    inData >> cardNo;

    while(!inData.eof())
    {
      inData >> act >> fName >> lName;
      ptr = new Person(cardNo, act, fName, lName);
      myCardholders.push_back(ptr);
      inData >> cardNo;
    }

    inData.close();

    ptr = NULL;
    return myCardholders.back()->getId() + 1;
}

void readRentals(vector<Book *> & myBooks, vector<Person *> myCardholders) {
    int bookID, cardNo;
    vector<Book *>::iterator book;
    vector<Person *>::iterator person;
    ifstream inData("rentals.txt");

    inData >> bookID;
    while(!inData.eof()) {
        inData >> cardNo;
        for(book = myBooks.begin(); book != myBooks.end(); book++) {
            if((*book)->getId() == bookID) {
                for(person = myCardholders.begin(); person != myCardholders.end(); person++) {
                    if((*person)->getId() == cardNo) {
                        (*book)->setPersonPtr((*person));
                    }
                }
            }
        }
        inData >> bookID;
    }
    inData.close();
}

void openCard(vector<Person *> & myCardholders, int nextID) {
    string fName, lName;

    cout << "Please enter the first name: ";
    cin >> fName;
    cout << "Please enter the last name: ";
    cin >> lName;

    Person * newCardholder = new Person(nextID, true, fName, lName);
    myCardholders.push_back(newCardholder);

    cout << "Card ID " << newCardholder->getId() << " active" << endl;
    cout << "Cardholder " << newCardholder->fullName() << endl;

    newCardholder = NULL;
}

Person * findPerson(vector<Person *> myCardholders, cardNo) {
    for(int i = 0; i < myCardholders.size(); i++) {
        if(myCardholders.at(i)->getId() == cardNo)
        return myCardholders.at(i);
    }

    cout << "Card ID not found" << endl;
    return nullptr;
}

Book * findBook(vector<Book *> myBooks, bookID) {
    for(int i = 0; i < myBooks.size(); i++) {
        if(myBooks.at(i)->getId() == bookID)
        return myBooks.at(i);
    }

    cout << "Book ID not found" << endl;
    return nullptr;
}

void bookCheckout(vector<Book *> & myBooks, vector<Person *> myCardholders){
    int cardNo, bookID;

    cout << "\nPlease enter the card ID: ";
    cin >> cardNo;
    for(int i = 0; i < myCardholders.size(); i++){
        if(myCardholders.at(i)->getId() == cardNo) {
            cout << myCardholders.at(i)->fullName() << endl;
            cout << "Please enter the book ID: ";
            cin >> bookID;

            for(int j = 0; j < myBooks.size(); j++) {
                if(myBooks.at(j)->getId() == bookID){
                    cout << myBooks.at(j)->getTitle() << endl;
                    myBooks.at(j)->setPersonPtr(myCardholders.at(i));
                    cout << "Rental completed" << endl;
                }
            }
        }
    }
}


int main()
{
    vector<Book *> books;
    vector<Person *> cardholders;
    int nextIdNo;

    nextIdNo = readPersons(cardholders);
    readBooks(books);
    readRentals(books, cardholders);

    int choice;
    do
    {
        // If you use cin anywhere, don't forget that you have to handle the <ENTER> key that
        // the user pressed when entering a menu option. This is still in the input stream.
        printMenu();
        cin >> choice;
        switch(choice)
        {
            case 1:
                // Book checkout
                bookCheckout(books, cardholders);
                break;

            case 2:
                // Book return
                break;

            case 3:
                // View all available books
                break;

            case 4:
                // View all outstanding rentals
                break;

            case 5:
                // View outstanding rentals for a cardholder
                break;

            case 6:
                // Open new library card
                break;

            case 7:
                // Close library card
                break;

            case 8:
                // Must update records in files here before exiting the program
                break;

            default:
                cout << "Invalid entry" << endl;
                break;
        }
        cout << endl;
   } while(choice != 8);
      return 0;
}
