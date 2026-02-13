#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

enum ViewingType { THEATER = 1, STREAMING, PHYSICAL };

//used as composition
class Rating {
private:
    double value;

public:
    Rating()
    {
        value = 0;
    }

    Rating(double v) { set(v); }

    void set(double v) {
        if (v >= 1 && v <= 10)
            value = v;
    }

    double get() const { return value; }

    bool isFavorite() const {
        return value >= 8.0;
    }
};

//Abstract base class
class MediaItem {
protected:
    string title;     

private:
    int year;
    ViewingType viewingType;

public:
    MediaItem(){
        title = "";
        year = 0;
        viewingType = STREAMING;
    }
    MediaItem(string t, int y, ViewingType v)
    {
        title = t;
        year = y;
        viewingType = v;
    }

    void setTitle(string t) { title = t; }
    void setYear(int y) { if (y > 0) year = y; }
    void setViewingType(ViewingType v) { viewingType = v; }
    string getTitle() const { return title; }
    int getYear() const { return year; }
    ViewingType getViewingType() const { return viewingType; }

    //virtual functions
    virtual string getType() const = 0; //pure virtual

    virtual void print() const {
        cout << left << setw(20) << title
            << setw(8) << year;
    }

    virtual ~MediaItem() {};
};

//derived class
class Film : public MediaItem {
private:
    Rating rating;   // composition

public:
    Film() : MediaItem() {
        rating = 0;
    }
    Film(string t, int y, ViewingType v, double r)
        : MediaItem(t, y, v) {
        rating = r;
    }

    void setRating(double r) { rating.set(r); }
    double getRating() const { return rating.get(); }


    //overrides
    string getType() const override {
        return "Film";
    }

    void print() const override {
        MediaItem::print();   
        cout << setw(12) << "Film"          
            << setw(15) << fixed << setprecision(1) << rating.get(); 
        if (rating.isFavorite()) cout << "(Fav)";
        cout << endl;
    }
};

//derived class
class Documentary : public MediaItem {
private:
    string subject;

public:
    Documentary() : MediaItem() { 
        subject = "";
    }

    Documentary(string t, int y, ViewingType v, string s) : MediaItem(t, y, v) {
        subject = s;
    }

    void setSubject(string s) { subject = s; }
    string getSubject() const { return subject; }

    //overrides
    string getType() const override {
        return "Documentary";
    }

    void print() const  override{
        MediaItem::print();   
        cout << setw(12) << "Doc"           
            << setw(15) << subject;        
        cout << endl;
    }
};

//manager class
class MediaTracker {
private:
    MediaItem** items;
    int size;
    int capacity;

    void resizeArray() {
        capacity *= 2;
        MediaItem** temp = new MediaItem * [capacity];

        for (int i = 0; i < size; i++)
            temp[i] = items[i];

        delete[] items;
        items = temp;
    }

    ViewingType askViewingType() {
        int choice;
        cout << "1.Theater 2.Streaming 3.Physical: ";
        while (!(cin >> choice) || choice < 1 || choice > 3) {
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cin.ignore(1000, '\n');
        return static_cast<ViewingType>(choice);
    }

public:
    MediaTracker() {
        capacity = 5;
        size = 0;
        items = new MediaItem * [capacity];
    }

    ~MediaTracker() {
        for (int i = 0; i < size; i++)
            delete items[i];      

        delete[] items;  
    }

    bool add(MediaItem* m) {
        if (size == capacity)
            resizeArray();
        else if (size > capacity) return false;
        items[size++] = m;
        return true;
    }

    bool remove(int index) {
        if (index < 0 || index >= size)
            return false;

        delete items[index];

        for (int i = index; i < size - 1; i++)
            items[i] = items[i + 1];

        size--;
        return true;
    }

    void printAll() const {
        cout << left << setw(20) << "Title"
            << setw(8) << "Year"
            << setw(12) << "Type"
            << setw(15) << "Rating/Subject" << endl;
        cout << string(55, '-') << endl;

        for (int i = 0; i < size; i++)
            items[i]->print();   // polymorphism
        cout << endl;
    }

    void showMenu() {
        int choice;
        do {
            cout << "\n1.Add Film\n2.Add Documentary\n3.View\n4.Remove Item\n9.Quit\nChoice: ";
            cin >> choice;
            cout << endl;
            cin.ignore(1000, '\n');

            if (choice == 1) {
                string t; int y; double r;
                cout << "Title: "; getline(cin, t);
                cout << "Year: "; cin >> y;
                cout << "Rating: "; cin >> r;
                ViewingType v = askViewingType();
                add(new Film(t, y, v, r));
            }

            if (choice == 2) {
                string t, s; int y;
                cout << "Title: "; getline(cin, t);
                cout << "Year: "; cin >> y;
                cin.ignore();
                cout << "Subject: "; getline(cin, s);
                ViewingType v = askViewingType();
                Documentary d(t, y, v, s);
                add(new Documentary(t, y, v, s));
            }
            if (choice == 3) printAll();

            if (choice == 4) {
                int index;
                printAll();
                cout << "Enter index to remove: ";
                cin >> index;
                remove(index);
            }
        } while (choice != 9);
    }
};

#ifdef _DEBUG
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("Film constructor sets fields") {
    Film f("Test", 2020, STREAMING, 8.5);
    CHECK(f.getTitle() == "Test");
    CHECK(f.getRating() == 8.5); //tests composition
}

TEST_CASE("Rating helper") {
    Rating r(9);
    CHECK(r.isFavorite() == true);
}

TEST_CASE("Documentary subject") {
    Documentary d("Earth", 2019, THEATER, "Nature");
    CHECK(d.getSubject() == "Nature");
}

TEST_CASE("Derived calls base via print") {
    Film f("Movie", 2021, STREAMING, 7);
    CHECK(f.getTitle() == "Movie");
}

TEST_CASE("Setter updates") {
    Film f;
    f.setRating(6);
    CHECK(f.getRating() == 6);
}

TEST_CASE("Polymorphic virtual function test") {
    MediaItem* m = new Film("Test", 2020, STREAMING, 9);
    CHECK(m->getType() == "Film");
    delete m;
}

TEST_CASE("Add & remove dynamic") {
    MediaTracker t;
    t.add(new Film("A", 2000, STREAMING, 5));
    t.add(new Documentary("B", 2001, THEATER, "Nature"));
    CHECK(t.remove(0) == true);
}

#else

int main() {
    MediaTracker tracker;
    tracker.showMenu();
    return 0;
}

#endif