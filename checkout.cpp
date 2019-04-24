//Ray Palo
// Section #2
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

void displayBookInfo(Book * book) {
    cout << "Book ID: " << book->getId() << endl;
    cout << "Title: " << book->getTitle() << endl;
    cout << "Author: " << book->getAuthor() << endl;
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

Person * findPerson(vector<Person *> myCardholders, int cardNo) {
    for(int i = 0; i < myCardholders.size(); i++) {
        if(myCardholders.at(i)->getId() == cardNo)
        return myCardholders.at(i);
    }

    cout << "Card ID not found" << endl;
    return nullptr;
}

Book * findBook(vector<Book *> myBooks, int bookID) {
    for(int i = 0; i < myBooks.size(); i++) {
        if(myBooks.at(i)->getId() == bookID)
        return myBooks.at(i);
    }

    cout << "Book ID not found" << endl;
    return nullptr;
}

bool bookIsRented(Book * book) {
    if(book->getPersonPtr() != nullptr) {
        return true;
    }
    else {
        return false;
    }
}

void bookCheckout(vector<Book *> & myBooks, vector<Person *> myCardholders){
    int cardNo, bookID;
    Person * cardholder;
    Book * book;
    bool rented;

    cout << "\nPlease enter the card ID: ";
    cin >> cardNo;
    cardholder = findPerson(myCardholders, cardNo);

    if(cardholder != nullptr) {
        cout << "Cardholder: " << cardholder->fullName() << endl;
        cout << "Please enter the book ID: ";
        cin >> bookID;
        book = findBook(myBooks, bookID);

        if(book != nullptr) {
          rented = bookIsRented(book);
          if(rented == false) {
              cout << book->getTitle() << endl;
              book->setPersonPtr(cardholder);
              cout << "Rental Completed" << endl;
          }
          else
          cout << "Book already checked out" << endl;
        }
    }
}

void bookReturn(vector<Book *> & myBooks) {
    int bookID;
    Book * book;
    cout << "\nPlease enter the book ID to return: ";
    cin >> bookID;
    book = findBook(myBooks, bookID);

    if(book != nullptr) {
        cout << book->getTitle() << endl;
        book->setPersonPtr(nullptr);
        cout << "Return Completed" << endl;
    }

}

void viewAvailableBooks(vector<Book *> myBooks) {
    bool noBooks = false;

    cout << endl;
    for(int i = 0; i < myBooks.size(); i++) {
        if(bookIsRented(myBooks.at(i)) == false) {
            displayBookInfo(myBooks.at(i));
            cout << "Category: " << myBooks.at(i)->getCategory() << endl;
            noBooks = true;
            cout << endl;
        }
    }
    if(noBooks = false)
    cout << "No books available" << endl;
}

void viewOutstandingRentals(vector<Book *> myBooks) {
  bool rentals = false;

  cout << endl;
  for(int i = 0; i < myBooks.size(); i++) {
      if(bookIsRented(myBooks.at(i)) == true) {
          displayBookInfo(myBooks.at(i));
          cout << "Cardholder: " << myBooks.at(i)->getPersonPtr()->fullName() << endl;
          cout << "Card ID: " << myBooks.at(i)->getPersonPtr()->getId() << endl;
          rentals = true;
          cout << endl;
      }
  }
  if(rentals == false)
  cout << "No outstanding rentals" << endl;
}

void viewCardholderRentals(vector<Book *> myBooks,
    vector<Person *> myCardholders) {
        int cardNo;
        bool hasRentals = false;
        Person * cardholder;

        cout << "\nPlease enter the card ID: ";
        cin >> cardNo;
        cardholder = findPerson(myCardholders, cardNo);

        if(cardholder != nullptr) {
            cout << "Cardholder: " << cardholder->fullName() << endl;
            cout << endl;
            for(int i = 0; i < myBooks.size(); i++) {
                if(myBooks.at(i)->getPersonPtr() == cardholder) {
                    if(hasRentals == false)
                    hasRentals = true;
                    displayBookInfo(myBooks.at(i));
                    cout << endl;
                }
            }
        }
        if(hasRentals == false)
        cout << "No books currently checked out" << endl;
    }

void closeCard(vector<Person *> & myCardholders) {
    int cardNo;
    char choice;
    Person * cardholder;

    cout << "\nPlease enter the card ID: ";
    cin >> cardNo;
    cardholder = findPerson(myCardholders, cardNo);

    if(cardholder != nullptr) {
        cout << "Cardholder: " << cardholder->fullName() << endl;
        if(cardholder->isActive() == false) {
          cout << "Card ID is already inactive" << endl;
        }
        else {
          cout << "Are you sure you want to deactivate card\n(Y/n)? ";
          cin >> choice;

          if(choice == 'y' || choice == 'Y') {
              cardholder->setActive(false);
              cout << "Card ID deactivated" << endl;
          }
          else if(choice == 'n' || choice == 'N')
          return;
        }
    }

}

void updateRecords(vector<Book *> myBooks, vector<Person*> myCardholders) {
    ofstream outData;

    outData.open("persons.txt");

    for(int i = 0; i < myCardholders.size(); i++) {
        outData << myCardholders.at(i)->getId() << " "
          << myCardholders.at(i)->isActive() << " "
            << myCardholders.at(i)->fullName() << endl;
    }

    outData.close();

    outData.open("rentals.txt");

    for(int i = 0; i < myBooks.size(); i++) {
        if(myBooks.at(i)->getPersonPtr() != nullptr) {
            outData << myBooks.at(i)->getId() << " "
              << myBooks.at(i)->getPersonPtr()->getId() << endl;
        }
    }

    for(int i = 0; i < myCardholders.size(); i++) {
        delete myCardholders.at(i);
        myCardholders.at(i) = nullptr;
    }

    for(int i = 0; i < myBooks.size(); i++) {
        delete myBooks.at(i);
        myBooks.at(i) = nullptr;
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
                bookReturn(books);
                break;

            case 3:
                // View all available books
                viewAvailableBooks(books);
                break;

            case 4:
                // View all outstanding rentals
                viewOutstandingRentals(books);
                break;

            case 5:
                // View outstanding rentals for a cardholder
                viewCardholderRentals(books, cardholders);
                break;

            case 6:
                // Open new library card
                openCard(cardholders, nextIdNo);
                nextIdNo++;
                break;

            case 7:
                // Close library card
                closeCard(cardholders);
                break;

            case 8:
                // Must update records in files here before exiting the program
                updateRecords(books, cardholders);
                break;

            default:
                cout << "Invalid entry" << endl;
                break;
        }
        cout << endl;
   } while(choice != 8);
      return 0;
}
