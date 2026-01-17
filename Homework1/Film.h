#pragma once
#include <string>

enum ViewingType {
	THEATER = 1,
	STREAMING,
	PHYSICAL
};

struct Film {
	std::string title;
	int year;			//release year
	double rating;		//rating including decimals 1-10
	ViewingType viewingType;
};

class FilmTracker {
private:
	static const int MAX_FILMS = 3;
	Film films[MAX_FILMS];
	int count;	//current amount of films entered

	//used by public function getFilm()
	std::string getFilmTitle();
	int getFilmYear();
	double getFilmRating();
	ViewingType getViewingType();

public:
	FilmTracker();
	void getFilm();					//prompts user to enter film data
	void printTable() const;		//prints table of film data
	void saveTable(const std::string& filename) const;	//saves film data to file
	void printBanner() const;
	void populateArray();			//Prompts the user to fill the array of films
	void printAverageScore();
	void showMenu();		//Main function that calls many other functions
};