#include <iostream>
#include <iomanip>
#include <string>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <vector>
#include <map>
#include <fstream>
#include "HttpClient.h"
#include "json.hpp"  // nlohmann/json library


using namespace std;
using json = nlohmann::json;

class APIClient : public HttpClient {
private:
    string response;

public:
    void StartOfData() override {
        response.clear();
    }

    void Data(const char* data, const unsigned int size) override {  // 🔥 EXACT MATCH
        response.append(data, size);
    }

    void EndOfData() override {}

    string GetResponse() const {
        return response;
    }
};

template <typename Type>
struct nodeType {
    Type info;
    nodeType* link;
};

template<typename Type>
class linkedListIterator {
private:
    nodeType<Type>* current;
public:
    linkedListIterator() {
        current = nullptr;
    }

    linkedListIterator(nodeType<Type>* ptr) {
        current = ptr;
    }

    Type operator*() {
        return current->info;
    }

    linkedListIterator<Type> operator++() {
        current = current->link;
        return *this;
    }

    bool isDone() const {
        return current == nullptr;
    }
};

template <typename Type>
class unorderedLinkedList {
private:
    int count;
    nodeType<Type>* first;
    nodeType<Type> *last;

public:
    unorderedLinkedList() {
        first = nullptr;
        last = nullptr;
        count = 0;
    }

    void print() const {
        nodeType<Type>* current;
        current = first;

        while (current != nullptr) {
            cout << current->info << " ";
            current = current->link;
        }
    }

    void destroyList() {
        nodeType<Type>* temp;

        while (first != nullptr) {
            temp = first;
            first = first->link;
            delete temp;
        }
        last = nullptr;
        count = 0;
    }

    bool search(const Type& searchItem) {
        bool found = false;
        nodeType<Type>* current;
        current = first;

        while (current != nullptr && !found)
            if (current->info == searchItem)
                return true;
            else
                current = current->link;
        return false;
    }

    void insertFirst(const Type& newItem) {
        nodeType<Type>* newNode;
        newNode = new nodeType<Type>;
        newNode->info = newItem;
        newNode->link = first;
        first = newNode;

        count++;

        if (last == nullptr)
            last = newNode;
    }

    void insertLast(const Type& newItem) {
        nodeType<Type>* newNode;
        newNode = new nodeType<Type>;

        newNode->info = newItem;
        newNode->link = nullptr;

        if (first == nullptr) {
            first = newNode;
            last = newNode;
            count++;
        }
        else {
            last->link = newNode;
            last = newNode;

            count++;
        }
    }

    void deleteNode(const Type& deleteItem) {
        nodeType<Type>* current;
        nodeType<Type>* trailCurrent;
        bool found;

        if (first == nullptr)
            cout << "Searched failed: empty list." << endl;
        else {
            if (first->info == deleteItem) {
                current = first;
                first = first->link;
                count--;
                if (first == nullptr)
                        last = nullptr;
                delete current;
            }
            else {
                found = false;
                trailCurrent = first;
                current = first->link;
                while (current != nullptr && !found) {
                    if (current->info != deleteItem) {
                        trailCurrent = current;
                        current = current->link;
                    }
                    else
                        found = true;
                }

                if (found) {
                    trailCurrent->link = current->link;
                    count--;
                    if (last == current)
                        last = trailCurrent;
                    delete current;
                }
                else
                    cout << "Search failed: item is not in list" << endl;
            }
        }
    }

    linkedListIterator<Type> begin() {
        linkedListIterator<Type> temp(first);
        return temp;
    }

    linkedListIterator<Type> end() {
        linkedListIterator<Type> temp(nullptr);
        return temp;
    }

    ~unorderedLinkedList() {
        destroyList();
    }
};

enum ViewingType { THEATER = 1, STREAMING, PHYSICAL };

class MediaException : public runtime_error {
    string fullMsg;
public:
    MediaException(const string& msg)
        : runtime_error(msg), fullMsg("\nMediaException: " + msg) {}

    const char* what() const noexcept override {
        return fullMsg.c_str();
    }
};

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
        delete[] data;
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
            throw MediaException("DynamicArray remove index out of bounds");
        for (int i = index; i < size - 1; i++)
            data[i] = data[i + 1];
        size--;
        return true;
    }

    T& operator[](int index) {
        if (index < 0 || index >= size) {
            throw MediaException("DynamicArray index out of bounds");
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw MediaException("DynamicArray index out of bounds");
        }
        return data[index];
    }

    int getSize() const { return size; }

    double highestRating(int first, int last) {

    }
};

template <typename T>
class ArrayStack {
private:
    T* data;
    int topIndex;
    int capacity;

public:
    explicit ArrayStack(int cap = 10)
        : data(nullptr), topIndex(-1), capacity(cap) {
        if (capacity <= 0)
            throw MediaException("ArrayStack invalid capacity");
        data = new T[capacity];
    }

    ~ArrayStack() {
        delete[] data;
    }

    bool isEmpty() const {
        return topIndex < 0;
    }

    bool isFull() const {
        return topIndex >= capacity - 1;
    }

    bool push(const T& item) {
        if (isFull())
            return false;
        data[++topIndex] = item;
        return true;
    }

    T pop() {
        if (isEmpty())
            throw MediaException("ArrayStack pop from empty stack");
        return data[topIndex--];
    }

    T top() const {
        if (isEmpty())
            throw MediaException("ArrayStack top from empty stack");
        return data[topIndex];
    }
};

template <typename T>
class ArrayQueue {
private:
    T* data;
    int frontIndex;
    int backIndex;
    int count;
    int capacity;

public:
    explicit ArrayQueue(int cap = 10)
        : data(nullptr), frontIndex(0), backIndex(0), count(0), capacity(cap) {
        if (capacity <= 0)
            throw MediaException("ArrayQueue invalid capacity");
        data = new T[capacity];
    }

    ~ArrayQueue() {
        delete[] data;
    }

    bool isEmpty() const {
        return count == 0;
    }

    bool isFull() const {
        return count == capacity;
    }

    bool enqueue(const T& item) {
        if (isFull())
            return false;

        data[backIndex] = item;
        backIndex = (backIndex + 1) % capacity;
        count++;
        return true;
    }

    T dequeue() {
        if (isEmpty())
            throw MediaException("ArrayQueue dequeue from empty queue");

        T item = data[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;
        return item;
    }

    T front() const {
        if (isEmpty())
            throw MediaException("ArrayQueue front from empty queue");

        return data[frontIndex];
    }
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
    vector<int> prices;
    ArrayStack<MediaItem*> undoRemoved;

    // We keep the DynamicArray for ordered storage, and use a map to
    // support direct title-based lookup instead of scanning every item.
    map<string, int> titleIndex;

    void rebuildTitleIndex() {
        titleIndex.clear();
        for (int i = 0; i < items.getSize(); i++) {
            titleIndex[items[i]->getTitle()] = i;
        }
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
    unorderedLinkedList<int> linkedPrices;

    MediaTracker() : undoRemoved(10) {
    }

    ~MediaTracker() {
        for (int i = 0; i < items.getSize(); i++)
            delete items[i];

        while (!undoRemoved.isEmpty())
            delete undoRemoved.pop();
    }

    MediaTracker& operator+=(MediaItem* m) {
        items.add(m);
        titleIndex[m->getTitle()] = items.getSize() - 1;
        return *this;
    }


    MediaTracker& operator-=(int index) {
        if (index < 0 || index >= items.getSize())
            throw MediaException("MediaTracker invalid index removal: " + to_string(index));

        MediaItem* removed = items[index];
        items.remove(index);

        if (!undoRemoved.push(removed)) {
            delete removed;
            throw MediaException("Undo stack full - cannot store removed item");
        }

        rebuildTitleIndex();
        return *this;
    }


    bool undoLastRemove() {
        if (undoRemoved.isEmpty())
            return false;

        MediaItem* restored = undoRemoved.pop();
        items.add(restored);
        titleIndex[restored->getTitle()] = items.getSize() - 1;
        return true;
    }

    MediaItem* get(int index) const {
        if (index < 0 || index >= items.getSize())
            throw MediaException("MediaTracker invalid index access: " + to_string(index));
        return items[index];
    }


    MediaItem* findByTitle(const string& title) const {
        auto it = titleIndex.find(title);
        if (it == titleIndex.end())
            return nullptr;

        return items[it->second];
    }

    bool removeByTitle(const string& title) {
        auto it = titleIndex.find(title);
        if (it == titleIndex.end())
            return false;

        *this -= it->second;
        return true;
    }

    void printTitleIndex() const {
        if (titleIndex.empty()) {
            cout << "Title map is empty.\n";
            return;
        }

        cout << "\nTitle lookup map:\n";
        for (const auto& entry : titleIndex) {
            cout << entry.first << " -> index " << entry.second << endl;
        }
    }


    int seqSearch(const vector<int>& list, int target)
    {
        for (int i = 0; i < list.size(); i++) {
            if (list[i] == target)
                return i;
        }
        return -1;
    }

    void bubbleSort(vector<int>& list) {
        int temp;

        for (int iteration = 1; iteration < list.size(); iteration++) {
            for (int index = 0; index < list.size() - iteration; index++) {
                if (list[index] > list[index + 1]) {
                    temp = list[index];
                    list[index] = list[index + 1];
                    list[index + 1] = temp;
                }
            }
        }
    }

    int binarySearch(const vector<int>& list, int target) {
        int first = 0;
        int last = list.size() - 1;
        int mid;
        bool found = false;

        while (first <= last && !found) {
            mid = (first + last) / 2;

            if (list[mid] == target)
                found = true;
            else if (list[mid] > target)
                last = mid - 1;
            else
                first = mid + 1;
        }
        if (found)
            return mid;
        else
            return -1;
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

    double highestRating(int& outIndex, int index = 0) const {

        //base case
        if (index >= items.getSize()) {
            outIndex = -1;
            return 0;
        }

        Film* f = dynamic_cast<Film*>(items[index]);
        double currentRating = 0;
        int currentIndex = -1;

        if (f != nullptr) {
            currentRating = f->getRating();
            currentIndex = index;
        }

        int restIndex;
        double maxRest = highestRating(restIndex, index + 1);

        if (currentRating > maxRest) {
            outIndex = currentIndex;
            return currentRating;
        }
        else {
            outIndex = restIndex;
            return maxRest;
        }
    }

    // JSON DATA LOADING FUNCTIONALITY
    // This method reads media items from a JSON file and populates the MediaTracker
    // The JSON file should contain an array of media objects with properties:
    // - type: "Film" or "Documentary"
    // - title, year, viewingType (1=Theater, 2=Streaming, 3=Physical)
    // - rating (for Film) or subject (for Documentary)
    bool loadFromJSON(const string& filename) {
        try {
            // Attempt to open the JSON file
            ifstream file(filename);
            if (!file.is_open()) {
                throw MediaException("JSON file not found: " + filename);
            }

            // Parse JSON from file
            json jsonData;
            file >> jsonData;
            file.close();

            // Validate that the JSON is an array
            if (!jsonData.is_array()) {
                throw MediaException("JSON root must be an array");
            }

            // Track how many items were loaded
            int loadedCount = 0;

            // Iterate over each JSON object in the array
            for (const auto& item : jsonData) {
                // Validate required fields
                if (!item.contains("type") || !item.contains("title") || 
                    !item.contains("year") || !item.contains("viewingType")) {
                    cout << "Warning: Skipping invalid JSON object (missing required fields)\n";
                    continue;
                }

                string type = item["type"];
                string title = item["title"];
                int year = item["year"];
                int viewingTypeInt = item["viewingType"];

                // Validate viewingType range
                if (viewingTypeInt < 1 || viewingTypeInt > 3) {
                    cout << "Warning: Skipping item with invalid viewingType: " << title << "\n";
                    continue;
                }

                ViewingType viewingType = static_cast<ViewingType>(viewingTypeInt);

                // Create appropriate MediaItem based on type
                if (type == "Film") {
                    if (!item.contains("rating")) {
                        cout << "Warning: Film missing rating field: " << title << "\n";
                        continue;
                    }
                    double rating = item["rating"];
                    *this += new Film(title, year, viewingType, rating);
                    loadedCount++;
                }
                else if (type == "Documentary") {
                    if (!item.contains("subject")) {
                        cout << "Warning: Documentary missing subject field: " << title << "\n";
                        continue;
                    }
                    string subject = item["subject"];
                    *this += new Documentary(title, year, viewingType, subject);
                    loadedCount++;
                }
                else {
                    cout << "Warning: Unknown media type '" << type << "' for: " << title << "\n";
                }
            }

            cout << "Successfully loaded " << loadedCount << " items from JSON file.\n";
            return true;
        }
        catch (const json::parse_error& e) {
            cout << "\nJSON Parse Error: " << e.what() << endl;
            cout << "The JSON file may be malformed.\n";
            return false;
        }
        catch (const MediaException& e) {
            cout << e.what() << endl;
            return false;
        }
        catch (const exception& e) {
            cout << "\nUnexpected error loading JSON: " << e.what() << endl;
            return false;
        }
    }

    void showMenu() {
        int choice;
        do {
            cout << "\n1.Add Film\n2.Add Documentary\n3.View\n4.Remove Item\n5.Print Highest Score\n6.Add price list\n7.Undo Remove\n8.Find By Title\n9.Load from JSON\n10.Load from API\n11.Quit\nChoice: ";
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
                try{ *this -= index; }
                catch (const MediaException& e) {
                    cout << "Error: " << e.what() << endl;
                }
            }

            if (choice == 5) {
                int maxIndex;
                double maxRating = highestRating(maxIndex);
                if (maxIndex == -1) {
                    cout << "No films found.\n";
                }
                else {
                    cout << "Highest rating: " << maxRating
                        << " for " << items[maxIndex]->getTitle();
                }
            }
            if (choice == 6) {
                int loop = 0;
                int temp = 0;
                while (loop == 0) {
                    cout << "\nEnter Price: ";
                    cin >> temp;
                    if (temp == -1)
                        break;
                    linkedPrices.insertFirst(temp);
                }
            }

            if (choice == 7) {
                if (!undoLastRemove())
                    cout << "Nothing to undo.\n";
            }

            if (choice == 8) {
                string searchTitle;
                cout << "Enter title to find: ";
                getline(cin, searchTitle);

                MediaItem* foundItem = findByTitle(searchTitle);
                if (foundItem == nullptr) {
                    cout << "Title not found.\n";
                }
                else {
                    cout << "Found item:\n";
                    cout << *foundItem << endl;
                }

                printTitleIndex();
            }

            if (choice == 9) {
                string filename;
                cout << "Enter JSON filename (or press Enter for 'media_data.json'): ";
                getline(cin, filename);
                if (filename.empty()) {
                    filename = "media_data.json";
                }
                loadFromJSON(filename);
            }

// REST API Integration:
// Performs a GET request to retrieve JSON data from an external API,
// parses the response using nlohmann/json, and inserts it into MediaTracker.
// Also demonstrates a POST request sending JSON data to the API.

            if (choice == 10) {
                APIClient client;

                // ---------------- GET REQUEST ----------------
                try {
                    client.Get("http://api.macomb.io/jokes?count=3");


                    string response = client.GetResponse();

                    json data = json::parse(response);

                    cout << "\nLoaded from API:\n";

                    for (auto& item : data) {
                        string title = item["setup"];
                        string subject = item["punchline"];

                        *this += new Documentary(title, 2024, STREAMING, subject);
                    }

                    cout << "Successfully loaded API data into MediaTracker.\n";

                    // 🔥 show integration
                    printAll();
                }
                catch (json::exception& e) {
                    cout << "JSON Error: " << e.what() << endl;
                }

                // ---------------- POST REQUEST ----------------
                try {
                    json postData;
                    postData["name"] = "New Media Item";
                    postData["value"] = 123;

                    string body = postData.dump();

                    client.Post("http://api.macomb.io/jokes", body);

                    cout << "\nPOST request sent successfully.\n";


                    string postResponse = client.GetResponse();

                    cout << "\nPOST Response:\n" << postResponse << endl;
                }
                catch (json::exception& e) {
                    cout << "POST JSON Error: " << e.what() << endl;
                }
            }

        } while (choice != 11);
    }
};

#ifdef RunTests
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

TEST_CASE("DynamicArray invalid index throws") {
    DynamicArray<int> arr;
    arr.add(10);
    CHECK_THROWS_AS(arr[5], MediaException);
    CHECK_THROWS_AS(arr[-1], MediaException);
}

TEST_CASE("DynamicArray remove invalid index throws") {
    DynamicArray<int> arr;
    arr.add(10);
    CHECK_THROWS_AS(arr.remove(1), MediaException);
}

TEST_CASE("Recursive function with films") {
    MediaTracker tracker;

    tracker += new Film("Movie A", 2000, STREAMING, 7.5);
    tracker += new Film("Movie B", 2010, THEATER, 8.2);
    tracker += new Film("Movie C", 2005, PHYSICAL, 6.8);

    tracker += new Documentary("Earth Doc", 2012, STREAMING, "Nature");

    int maxIndex;
    double maxRating = tracker.highestRating(maxIndex);

    CHECK(maxRating == 8.2);
    CHECK(tracker.get(maxIndex)->getTitle() == "Movie B");
}

TEST_CASE("Recursive function with no films") {
    MediaTracker tracker;

    tracker += new Documentary("Doc 1", 2000, STREAMING, "Science");
    tracker += new Documentary("Doc 2", 2005, THEATER, "Nature");

    int maxIndex;
    double maxRating = tracker.highestRating(maxIndex);

    CHECK(maxRating == 0);
    CHECK(maxIndex == -1);
}

TEST_CASE("MediaTracker search and sort functions") {
    MediaTracker tracker;
    vector<int> data = { 5, 3, 8, 1, 9 };

    // Test sequential search
    CHECK(tracker.seqSearch(data, 8) == 2);
    CHECK(tracker.seqSearch(data, 1) == 3);
    CHECK(tracker.seqSearch(data, 10) == -1);

    // Test bubble sort
    tracker.bubbleSort(data);
    CHECK(data == vector<int>{1, 3, 5, 8, 9});

    // Test binary search on sorted vector
    CHECK(tracker.binarySearch(data, 5) == 2);
    CHECK(tracker.binarySearch(data, 1) == 0);
    CHECK(tracker.binarySearch(data, 10) == -1);
}

TEST_CASE("Insert/Delete function of linked list") {
    unorderedLinkedList<int> list;

    list.insertFirst(10);
    CHECK(list.search(10));
    list.insertLast(20);
    CHECK(list.search(20));
    list.insertLast(30);
    CHECK(list.search(30));

    list.deleteNode(20);
    CHECK(!list.search(20));
    CHECK(list.search(10));    //10 maintains position
    CHECK(list.search(30));    //30 shifts on previous delete
}

TEST_CASE("Iterator traversal") {
    unorderedLinkedList<int> list;
    list.insertLast(1);
    list.insertLast(2);
    list.insertLast(3);

    linkedListIterator<int> it = list.begin();
    int expected = 1;
    while (!it.isDone()) {
        CHECK(*it == expected);
        ++it;
        ++expected;
    }
}

TEST_CASE("ArrayStack push/pop/top basic") {
    ArrayStack<int> s(3);
    CHECK(s.isEmpty());

    CHECK(s.push(10));
    CHECK(s.top() == 10);
    CHECK(!s.isEmpty());

    CHECK(s.push(20));
    CHECK(s.top() == 20);

    CHECK(s.pop() == 20);
    CHECK(s.top() == 10);
    CHECK(s.pop() == 10);
    CHECK(s.isEmpty());
}

TEST_CASE("ArrayStack push to full returns false") {
    ArrayStack<int> s(2);
    CHECK(s.push(1));
    CHECK(s.push(2));
    CHECK_FALSE(s.push(3));
}

TEST_CASE("ArrayStack pop/top on empty throws") {
    ArrayStack<int> s(2);
    CHECK_THROWS_AS(s.pop(), MediaException);
    CHECK_THROWS_AS(s.top(), MediaException);
}

TEST_CASE("MediaTracker undo remove restores last removed") {
    MediaTracker tracker;
    tracker += new Film("A", 2000, STREAMING, 7.0);
    tracker += new Film("B", 2001, STREAMING, 8.0);

    tracker -= 0; // removes "A" but stores it on undo stack

    CHECK(tracker.get(0)->getTitle() == "B");
    CHECK(tracker.undoLastRemove());
    CHECK(tracker.get(1)->getTitle() == "A");
}

TEST_CASE("MediaTracker undo remove when empty returns false") {
    MediaTracker tracker;
    CHECK_FALSE(tracker.undoLastRemove());
}

TEST_CASE("ArrayQueue enqueue/dequeue/front basic") {
    ArrayQueue<int> q(3);
    CHECK(q.isEmpty());

    CHECK(q.enqueue(10));
    CHECK(q.front() == 10);
    CHECK(!q.isEmpty());

    CHECK(q.enqueue(20));
    CHECK(q.front() == 10);

    CHECK(q.dequeue() == 10);
    CHECK(q.front() == 20);

    CHECK(q.dequeue() == 20);
    CHECK(q.isEmpty());
}

TEST_CASE("ArrayQueue enqueue to full returns false") {
    ArrayQueue<int> q(2);
    CHECK(q.enqueue(1));
    CHECK(q.enqueue(2));
    CHECK_FALSE(q.enqueue(3));
}

TEST_CASE("ArrayQueue dequeue/front on empty throws") {
    ArrayQueue<int> q(2);
    CHECK_THROWS_AS(q.dequeue(), MediaException);
    CHECK_THROWS_AS(q.front(), MediaException);
}

TEST_CASE("ArrayQueue circular behavior works") {
    ArrayQueue<int> q(3);

    CHECK(q.enqueue(1));
    CHECK(q.enqueue(2));
    CHECK(q.enqueue(3));
    CHECK_FALSE(q.enqueue(4)); // full

    CHECK(q.dequeue() == 1);
    CHECK(q.dequeue() == 2);

    CHECK(q.enqueue(4));
    CHECK(q.enqueue(5));

    CHECK(q.front() == 3);
    CHECK(q.dequeue() == 3);
    CHECK(q.dequeue() == 4);
    CHECK(q.dequeue() == 5);

    CHECK(q.isEmpty());
}

TEST_CASE("MediaTracker findByTitle returns matching item") {
    MediaTracker tracker;
    tracker += new Film("Inception", 2010, STREAMING, 9.0);
    tracker += new Documentary("Planet Earth", 2006, THEATER, "Nature");

    MediaItem* found = tracker.findByTitle("Inception");

    CHECK(found != nullptr);
    CHECK(found->getTitle() == "Inception");
}

TEST_CASE("MediaTracker findByTitle returns nullptr for missing title") {
    MediaTracker tracker;
    tracker += new Film("Interstellar", 2014, STREAMING, 8.5);

    CHECK(tracker.findByTitle("Missing Title") == nullptr);
}

TEST_CASE("MediaTracker removeByTitle removes an existing title") {
    MediaTracker tracker;
    tracker += new Film("Titanic", 1997, STREAMING, 8.0);
    tracker += new Film("Avatar", 2009, THEATER, 7.8);

    CHECK(tracker.removeByTitle("Titanic") == true);
    CHECK(tracker.findByTitle("Titanic") == nullptr);
    CHECK(tracker.get(0)->getTitle() == "Avatar");
}

TEST_CASE("MediaTracker removeByTitle returns false for missing title") {
    MediaTracker tracker;
    tracker += new Film("Gladiator", 2000, STREAMING, 8.4);

    CHECK(tracker.removeByTitle("Missing Title") == false);
    CHECK(tracker.get(0)->getTitle() == "Gladiator");
}

TEST_CASE("JSON loading - valid file with mixed media") {
    MediaTracker tracker;
    ofstream outFile("test_media.json");
    outFile << R"([{"type":"Film","title":"Test Film 1","year":2020,"viewingType":2,"rating":8.5},{"type":"Documentary","title":"Test Doc 1","year":2019,"viewingType":1,"subject":"Science"}])";
    outFile.close();
    CHECK(tracker.loadFromJSON("test_media.json") == true);
    MediaItem* item1 = tracker.findByTitle("Test Film 1");
    CHECK(item1 != nullptr);
    Film* film = dynamic_cast<Film*>(item1);
    CHECK(film->getRating() == 8.5);
    remove("test_media.json");
}

TEST_CASE("JSON loading - file not found") {
    MediaTracker tracker;
    CHECK(tracker.loadFromJSON("nonexistent.json") == false);
}

TEST_CASE("JSON loading - malformed JSON") {
    MediaTracker tracker;
    ofstream outFile("malformed.json");
    outFile << R"({ invalid json })";
    outFile.close();
    CHECK(tracker.loadFromJSON("malformed.json") == false);
    remove("malformed.json");
}

TEST_CASE("JSON loading - not array") {
    MediaTracker tracker;
    ofstream outFile("notarray.json");
    outFile << R"({"type":"Film","title":"Solo","year":2020,"viewingType":2,"rating":8.5})";
    outFile.close();
    CHECK(tracker.loadFromJSON("notarray.json") == false);
    remove("notarray.json");
}

TEST_CASE("JSON loading - missing fields") {
    MediaTracker tracker;
    ofstream outFile("missing.json");
    outFile << R"([{"type":"Film","year":2021,"viewingType":1,"rating":7.0}])";
    outFile.close();
    tracker.loadFromJSON("missing.json");
    remove("missing.json");
}

TEST_CASE("JSON loading - invalid viewingType") {
    MediaTracker tracker;
    ofstream outFile("invalid.json");
    outFile << R"([{"type":"Film","title":"Invalid","year":2021,"viewingType":99,"rating":7.0}])";
    outFile.close();
    tracker.loadFromJSON("invalid.json");
    CHECK(tracker.findByTitle("Invalid") == nullptr);
    remove("invalid.json");
}

TEST_CASE("APIClient stores response correctly") { //new assignment 14 doc test
    APIClient client;

    client.StartOfData();
    client.Data("hello", 5);
    client.EndOfData();

    CHECK(client.GetResponse() == "hello");
}

#else
int main() {
    {
        MediaTracker tracker;
        tracker.showMenu();
    }
    int leaks = _CrtDumpMemoryLeaks();
    if (!leaks) {
        cout << "No memory leaks detected.\n";
    }

    return 0;
}
#endif
