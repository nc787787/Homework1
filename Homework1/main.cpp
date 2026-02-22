#include <iostream>
#include <iomanip>
#include <string>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace std;

enum ViewingType { THEATER = 1, STREAMING, PHYSICAL };

template <typename T>
void swapItems(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
class DynamicArray {
private:
    T* data;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        T* temp = new T[capacity];
        for (int i = 0; i < size; i++)
            temp[i] = data[i];
        delete[] data;
        data = temp;
    }

public:
    DynamicArray() : size(0), capacity(5) {
        data = new T[capacity];
    }

    ~DynamicArray() {
        delete[] data;  // If T is a pointer, actual object deletion happens in MediaTracker
    }

    bool add(const T& item) {
        if (size == capacity)
            resize();
        else if (size > capacity) return false;
        data[size++] = item;
        return true;
    }

    bool remove(int index) {
        if (index < 0 || index >= size)
            return false;
        for (int i = index; i < size - 1; i++)
            data[i] = data[i + 1];
        size--;
        return true;
    }

    T& operator[](int index) {
        if (index < 0 || index >= size) {
            cout << "Invalid index\n";
            static T dummy;  // safe fallback
            return dummy;
        }
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }

    int getSize() const { return size; }
};


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

    virtual void toStream(ostream& os) const {
        os << left << setw(20) << title
            << setw(8) << year
            << setw(12) << getType();
    }

    friend ostream& operator<<(ostream& os, const MediaItem& m) {
        m.toStream(os);
        return os;
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

    bool operator==(const Film& other) const {
        return title == other.title && getYear() == other.getYear() && getRating() == other.getRating();
    }

    void toStream(ostream& os) const override {
        MediaItem::toStream(os);
        os << setw(15) << fixed << setprecision(1) << rating.get();
        if (rating.isFavorite()) os << "(Fav)";
    }

    Film* clone() const {
        return new Film(*this);
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

    void toStream(ostream& os) const override {
        MediaItem::toStream(os);
        os << setw(15) << subject;
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
    DynamicArray<MediaItem*> items;

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
    }

    ~MediaTracker() {
        for (int i = 0; i < items.getSize(); i++)
            delete items[i];
    }

    MediaTracker& operator+=(MediaItem* m) {
        items.add(m);
        return *this;
    }

    MediaTracker& operator-=(int index) {
        if (index < 0 || index >= items.getSize()) return *this;

        delete items[index];
        items.remove(index);
        return *this;
    }

    MediaItem* get(int index) const {
        if (index < 0 || index >= items.getSize()) return nullptr;
        return items[index];
    }

    void printAll() const {
        cout << left << setw(20) << "Title"
             << setw(8) << "Year"
             << setw(12) << "Type"
             << setw(15) << "Rating/Subject" << endl;
        cout << string(55, '-') << endl;

        for (int i = 0; i < items.getSize(); i++) {
            cout << *(items[i]);
            cout << endl;
        }
            

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
                *this += new Film(t, y, v, r);
            }

            if (choice == 2) {
                string t, s; int y;
                cout << "Title: "; getline(cin, t);
                cout << "Year: "; cin >> y;
                cin.ignore();
                cout << "Subject: "; getline(cin, s);
                ViewingType v = askViewingType();
                *this += new Documentary(t, y, v, s);
            }

            if (choice == 3) printAll();

            if (choice == 4) {
                int index;
                printAll();
                cout << "Enter index to remove: ";
                cin >> index;
                *this -= index;
            }
        } while (choice != 9);
    }
};
/*
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

TEST_CASE("Add & remove dynamic via += / -=") {
    MediaTracker t;
    t += new Film("A", 2000, STREAMING, 5);
    t += new Documentary("B", 2001, THEATER, "Nature");

    CHECK(t.get(0) != nullptr);
    CHECK(t.get(1) != nullptr);
    t -= 0;
    CHECK(t.get(0) != nullptr);
    CHECK(t.get(1) == nullptr);
}

TEST_CASE("Film == overload test") {
    Film f1("Movie", 2020, STREAMING, 8);
    Film f2("Movie", 2020, STREAMING, 8);
    Film f3("Other", 2020, STREAMING, 5);

    CHECK(f1 == f2);
    CHECK(!(f1 == f3));
}

TEST_CASE("operator<<") {
    Film f("Movie", 2021, STREAMING, 9);
    Documentary d("Earth", 2020, THEATER, "Nature");
    ostringstream oss1, oss2;
    oss1 << f;
    oss2 << d;
    CHECK(oss1.str().find("Movie") != string::npos);
    CHECK(oss2.str().find("Earth") != string::npos);
}

//using int because its a class template
TEST_CASE("DynamicArray operator[]") {
    DynamicArray<int> arr;
    arr.add(10);
    arr.add(20);
    CHECK(arr[0] == 10);
    CHECK(arr[1] == 20);

    CHECK(arr[5] == 0);
}

TEST_CASE("swapItems with Film objects") {
    Film f1("Movie A", 2000, STREAMING, 7.5);
    Film f2("Movie B", 2010, THEATER, 8.2);

    swapItems(f1, f2);
    CHECK(f1.getTitle() == "Movie B");
    CHECK(f2.getTitle() == "Movie A");
}

TEST_CASE("DynamicArray auto-resize") {
    DynamicArray<int> arr;
    for (int i = 0; i < 10; i++)
        arr.add(i);

    CHECK(arr.getSize() == 10);
    for (int i = 0; i < 10; i++)
        CHECK(arr[i] == i);
}


#else*/
//main
int main() {
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    {
        MediaTracker tracker;
        tracker.showMenu();
    }
    //int* test = new int;
    //*test = 10;
    _CrtDumpMemoryLeaks();
    return 0;
}

//#endif