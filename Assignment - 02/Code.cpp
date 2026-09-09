#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

struct Movie
{
    string title;
    float rating;
    int year;
    long long popularity;
};

vector<Movie> movies;

int partition(vector<Movie> &arr, int low, int high, int choice)
{
    Movie pivot = arr[high];
    int i = low - 1;

    for(int j = low; j < high; j++)
    {
        bool condition = false;

        switch(choice)
        {
            case 1:
                condition = arr[j].rating > pivot.rating;
                break;

            case 2:
                condition = arr[j].year > pivot.year;
                break;

            case 3:
                condition = arr[j].popularity > pivot.popularity;
                break;
        }

        if(condition)
        {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i+1], arr[high]);

    return i+1;
}

void quickSort(vector<Movie> &arr, int low, int high, int choice)
{
    if(low < high)
    {
        int pi = partition(arr, low, high, choice);

        quickSort(arr, low, pi-1, choice);
        quickSort(arr, pi+1, high, choice);
    }
}

void loadCSV(string filename)
{
    ifstream file(filename);

    if(!file)
    {
        cout << "Error: Cannot open dataset!" << endl;
        return;
    }

    string line;

    getline(file, line);

    while(getline(file, line))
    {
        if(line.empty())
            continue;

        stringstream ss(line);

        Movie m;

        string rating;
        string year;
        string popularity;

        getline(ss, m.title, ',');
        getline(ss, rating, ',');
        getline(ss, year, ',');
        getline(ss, popularity, ',');

        if(m.title.empty() || rating.empty() || year.empty() || popularity.empty())
            continue;

        try
        {
            m.rating = stof(rating);
            m.year = stoi(year);
            m.popularity = stoll(popularity);

            movies.push_back(m);
        }
        catch(...)
        {

        }
    }

    file.close();
}

void displayMovies(int n)
{
    cout << left
         << setw(5) << "No."
         << setw(42) << "Movie Title"
         << setw(10) << "Rating"
         << setw(8) << "Year"
         << setw(15) << "Popularity"
         << endl;

    for(int i=0; i<n && i<movies.size(); i++)
    {
        cout << left
             << setw(5) << i+1
             << setw(42) << movies[i].title.substr(0,40)
             << setw(10) << fixed << setprecision(1) << movies[i].rating
             << setw(8) << movies[i].year
             << setw(15) << movies[i].popularity
             << endl;
    }
}

int main()
{
    loadCSV("movies_clean.csv");

    if(movies.empty())
    {
        cout << "No movie data found!" << endl;
        return 0;
    }

    int choice;
    int count;

    cout << "      STREAMFLIX MOVIE RECOMMENDATION\n\n";

    cout << "Choose Recommendation Parameter\n";
    cout << "1. IMDb Rating\n";
    cout << "2. Release Year\n";
    cout << "3. Popularity\n";

    cout << "\nEnter your choice: ";
    cin >> choice;

    if(choice < 1 || choice > 3)
    {
        cout << "Invalid Choice!" << endl;
        return 0;
    }

    cout << "How many movies do you want to display? ";
    cin >> count;

    quickSort(movies, 0, movies.size()-1, choice);

    displayMovies(count);
    return 0;
}
