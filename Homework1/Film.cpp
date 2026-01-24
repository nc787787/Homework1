#include <iostream>
#include <iomanip>
#include <limits>
#include <fstream>
#include "Film.h"

using namespace std;

FilmTracker::FilmTracker() {
    count = 0; //initializes counter variable
}

//Private functions for user inputs
string FilmTracker::getFilmTitle() {
    string title;
    cout << "Enter film title: ";
    getline(cin, title);
    while (title.empty()) {
        cout << "Title cannot be blank\nEnter film title: ";
        getline(cin, title);
    }
    return title;
}

int FilmTracker::getFilmYear() {
    int year;
    cout << "Enter the year of the films release: ";
    while (!(cin >> year)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid year. Please enter a year only: ";
    }
    cin.ignore(1000, '\n');
    return year;
}

double FilmTracker::getFilmRating() {
    double rating;
    double minRating = 1;
    double maxRating = 10;
    cout << "Rate the film 1-10: ";
    while (!(cin >> rating) || rating < minRating || rating > maxRating) {
        cout << "Invalid input. Enter a number between 1-10: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(1000, '\n');
    return rating;
}

ViewingType FilmTracker::getViewingType() {
    int choice;

    cout << "How did you watch the film?\n";
    cout << "1. Theater\n";
    cout << "2. Streaming\n";
    cout << "3. Physical (DVD/Blu-ray)\n";
    cout << "Enter choice: ";

    while (!(cin >> choice) || choice < 1 || choice > 3) {
        cout << "Invalid choice. Enter 1, 2, or 3: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    cin.ignore(1000, '\n');

    switch (choice) {
    case 1: return THEATER;
    case 2: return STREAMING;
    case 3: return PHYSICAL;
    default: return STREAMING;
    }
}


//public function to call in main
void FilmTracker::getFilm() {
    if (count >= MAX_FILMS) {
        cout << "Film list is full.\n";
        return;
    }
    Film f;
    f.title = getFilmTitle();
    f.year = getFilmYear();
    f.rating = getFilmRating();
    f.viewingType = getViewingType();

    films[count] = f;
    count++;

    cout << f.title << " added." << endl;
}

//Prints film table in cmd
void FilmTracker::printTable() const {
    cout << "\n------------------- Your Films -----------------------------\n";
    cout << left << setw(25) << "Title"
        << right << setw(10) << "Year"
        << right << setw(10) << "Rating" 
        << right << setw(12) << "Viewed As" << endl;
    cout << "-----------------------------------------------------------\n";

    for (int i = 0; i < count; i++) {
        string viewText;

        switch (films[i].viewingType) {
        case THEATER: viewText = "Theater"; break;
        case STREAMING: viewText = "Streaming"; break;
        case PHYSICAL: viewText = "Physical"; break;
        }

        cout << left << setw(25) << films[i].title
            << right << setw(10) << films[i].year
            << right << setw(10) << fixed << setprecision(1) << films[i].rating
            << right << setw(12) << viewText
            << endl;
    }
}

void FilmTracker::saveTable(const string& filename) const {
    ofstream outFile(filename);
    if (!outFile) {
        cout << "Error opening File.\n";
        return;
    }

    outFile << "------------------ Your Films ------------------\n";
    outFile << left << setw(25) << "Title"
        << right << setw(10) << "Year"
        << right << setw(10) << "Rating" << endl;
    outFile << "------------------------------------------------\n";

    for (int i = 0; i < count; i++) {
        outFile << left << setw(25) << films[i].title
            << right << setw(10) << films[i].year
            << right << setw(10) << fixed << setprecision(1) << films[i].rating
            << endl;
    }

    outFile.close();
    cout << "\nTable saved to " << filename << endl;
}

void FilmTracker::printBanner() const {
    cout << "===============================" << endl;
    cout << "   Welcome to Film Tracker!  " << endl;
    cout << "===============================" << endl;
}

void FilmTracker::populateArray() {
    if (count >= MAX_FILMS) {
        cout << "Film List is full" << endl;
        return;
    }
    else {
        for (int i = count; i < MAX_FILMS; i++) {
            getFilm();
        }
    }
}

//I only did it this way with a function outside of the class because there was a requirement to pass an array and I couldnt fit it naturally.
double calculateAverageRating(Film arr[], int size) {
    if (size == 0) return 0;

    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i].rating;
    }
    return sum / size;
}

void FilmTracker::printAverageScore() {
    double avg = calculateAverageRating(films, count);
    cout << "Average rating: " << avg << endl;
}

void FilmTracker::showMenu() {
    int choice;

    do {
        cout << "\n===== Film Tracker Menu =====\n";
        cout << "1. Add a film\n";
        cout << "2. View Table\n";
        cout << "3. View Average Score\n";
        cout << "4. Save table to file\n";
        cout << "5. Quit\n";
        cout << "Enter your choice: ";

        while (!(cin >> choice)) {
            cout << "Invalid input. Enter a number: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cin.ignore(1000, '\n');

        switch (choice) {
        case 1:
            getFilm();
            break;
        case 2:
            printTable();
            break;
        case 3:
            printAverageScore();
            break;
        case 4:
            saveTable("savedTable.txt");
            break;
        case 5:
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Invalid menu option.\n";
        }

    } while (choice != 5);
}

bool FilmTracker::addFilm(const Film& f) {
    if (count >= MAX_FILMS) return false;
    films[count++] = f;
    return true;
}