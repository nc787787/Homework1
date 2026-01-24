#include <iostream>
#include "Film.h"

using namespace std;

void setYear(Film& f, int y) {
	if (y > 0) {
		f.year = y;
	}
}
void setRating(Film& f, double r) {
	if (r >= 1.0 && r <= 10.0) {
		f.rating = r;
	}
}

string viewingTypeToString(ViewingType v) {
	switch (v) {
	case THEATER:   return "Theater";
	case STREAMING: return "Streaming";
	case PHYSICAL:  return "Physical";
	default:        return "Unknown";
	}
}

#ifdef _DEBUG
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
double calculateAverageRating(Film arr[], int size); //exposing non-class functions in Film.cpp since im trying to stop using .h as per your instructions
#endif

#ifdef _DEBUG
//debug code

TEST_CASE("Add one film and increase count") {
	FilmTracker tracker;

	Film f;
	f.title = "test film";
	f.year = 2026;
	f.rating = 8.5;
	f.viewingType = STREAMING;

	CHECK(tracker.addFilm(f) == true);
	f.rating = 2.2;
	CHECK(tracker.addFilm(f) == true);
	f.rating = 3.3;
	CHECK(tracker.addFilm(f) == true);
	f.rating = 4.4;
	CHECK(tracker.addFilm(f) == false); //will be INVALID as true! exceeds max capacity 
}

TEST_CASE("Average rating calculation") {
	Film films[3];

	films[0] = { "Movie A", 2020, 8.0, STREAMING };
	films[1] = { "Movie B", 2021, 6.0, THEATER };
	films[2] = { "Movie C", 2022, 10.0, PHYSICAL };

	CHECK(calculateAverageRating(films, 3) == doctest::Approx(8.0));
	setRating(films[0], 1.0);
	setRating(films[1], 1.0);
	setRating(films[2], 1.0);
	CHECK(calculateAverageRating(films, 3) == doctest::Approx(1.0)); //edge case
}

TEST_CASE("Return enum viewing type test") {
	CHECK(viewingTypeToString(THEATER) == "Theater");
	CHECK(viewingTypeToString(STREAMING) == "Streaming");
	CHECK(viewingTypeToString(PHYSICAL) == "Physical");
	CHECK(viewingTypeToString(static_cast<ViewingType>(999)) == "Unknown");
}

#else
int main() {
	FilmTracker tracker;
	tracker.showMenu();

	return 0;
}
#endif