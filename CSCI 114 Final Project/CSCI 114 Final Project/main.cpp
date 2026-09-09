//
//  main.cpp
//  CSCI 114 Final Project
//
//  Created by Cindy on 2026/5/12.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <cstdlib>

using namespace std;

struct Track {
    int index;
    string track_id;
    string artists;
    string album_name;
    string track_name;
    int popularity;
    long duration_ms;
    bool explicit_content;
    double danceability;
    double energy;
    int key;
    double loudness;
    int mode;
    double speechiness;
    double acousticness;
    double instrumentalness;
    double liveness;
    double valence;
    double tempo;
    int time_signature;
    string track_genre;
};

vector<string> parseCSVLine(const string& line) {
    vector<string> fields;
    string field;
    bool inQuotes = false;

    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(field);
    return fields;
}

vector<Track> loadCSV(const string& filename) {
    vector<Track> tracks;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: could not open file " << filename << endl;
        return tracks;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        auto f = parseCSVLine(line);
        if (f.size() < 21) continue;

        Track t;
        t.index              = stoi(f[0]);
        t.track_id           = f[1];
        t.artists            = f[2];
        t.album_name         = f[3];
        t.track_name         = f[4];
        t.popularity         = stoi(f[5]);
        t.duration_ms        = stol(f[6]);
        t.explicit_content   = (f[7] == "True");
        t.danceability       = stod(f[8]);
        t.energy             = stod(f[9]);
        t.key                = stoi(f[10]);
        t.loudness           = stod(f[11]);
        t.mode               = stoi(f[12]);
        t.speechiness        = stod(f[13]);
        t.acousticness       = stod(f[14]);
        t.instrumentalness   = stod(f[15]);
        t.liveness           = stod(f[16]);
        t.valence            = stod(f[17]);
        t.tempo              = stod(f[18]);
        t.time_signature     = stoi(f[19]);
        t.track_genre        = f[20];

        tracks.push_back(t);
    }

    return tracks;
}

void writePopularityDistribution(const vector<Track>& tracks) {
    ofstream out("popularity_distribution.dat");

    map<int, int> counts;
    for (const auto& t : tracks) {
        counts[t.popularity]++;
    }
    for (const auto& p : counts) {
        out << p.first << " " << p.second << "\n";
    }
    out.close();

    ofstream gp("popularity_distribution.gp");
    gp << "set terminal png size 900,600\n";
    gp << "set output 'popularity_distribution.png'\n";
    gp << "set title 'Distribution of Popularity Scores'\n";
    gp << "set xlabel 'Popularity'\n";
    gp << "set ylabel 'Number of Tracks'\n";
    gp << "set boxwidth 0.8\n";
    gp << "set style fill solid\n";
    gp << "plot 'popularity_distribution.dat' using 1:2 with boxes title 'Tracks'\n";
    gp.close();

    system("/opt/homebrew/bin/gnuplot popularity_distribution.gp");
}

void writeAveragePopularityByGenre(const vector<Track>& tracks) {
    map<string, pair<double, int>> genreStats;

    for (const auto& t : tracks) {
        genreStats[t.track_genre].first += t.popularity;
        genreStats[t.track_genre].second++;
    }

    ofstream out("avg_popularity_by_genre.dat");
    for (const auto& p : genreStats) {
        double avg = p.second.first / p.second.second;
        out << "\"" << p.first << "\" " << avg << "\n";
    }
    out.close();

    ofstream gp("avg_popularity_by_genre.gp");
    gp << "set terminal png size 1400,700\n";
    gp << "set output 'avg_popularity_by_genre.png'\n";
    gp << "set title 'Average Popularity by Genre'\n";
    gp << "set xlabel 'Genre'\n";
    gp << "set ylabel 'Average Popularity'\n";
    gp << "set style data histograms\n";
    gp << "set style fill solid\n";
    gp << "set xtics rotate by -60\n";
    gp << "set xtics font ',8'\n";
    gp << "set yrange [0:70]\n";
    gp << "plot 'avg_popularity_by_genre.dat' using 2:xtic(1) title 'Average Popularity'\n";
    gp.close();

    system("/opt/homebrew/bin/gnuplot avg_popularity_by_genre.gp");
}

double correlation(const vector<double>& x, const vector<double>& y) {
    int n = x.size();
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0, sumY2 = 0;

    for (int i = 0; i < n; i++) {
        sumX  += x[i];
        sumY  += y[i];
        sumXY += x[i] * y[i];
        sumX2 += x[i] * x[i];
        sumY2 += y[i] * y[i];
    }

    double numerator   = n * sumXY - sumX * sumY;
    double denominator = sqrt((n * sumX2 - sumX * sumX) *
                              (n * sumY2 - sumY * sumY));

    if (denominator == 0) return 0;
    return numerator / denominator;
}

void writeCorrelationWithPopularity(const vector<Track>& tracks) {
    vector<double> popularity, danceability, energy, loudness, speechiness;
    vector<double> acousticness, instrumentalness, liveness, valence, tempo;

    for (const auto& t : tracks) {
        popularity.push_back(t.popularity);
        danceability.push_back(t.danceability);
        energy.push_back(t.energy);
        loudness.push_back(t.loudness);
        speechiness.push_back(t.speechiness);
        acousticness.push_back(t.acousticness);
        instrumentalness.push_back(t.instrumentalness);
        liveness.push_back(t.liveness);
        valence.push_back(t.valence);
        tempo.push_back(t.tempo);
    }

    ofstream out("correlation_with_popularity.dat");
    out << "\"danceability\" "     << correlation(danceability,     popularity) << "\n";
    out << "\"energy\" "           << correlation(energy,           popularity) << "\n";
    out << "\"loudness\" "         << correlation(loudness,         popularity) << "\n";
    out << "\"speechiness\" "      << correlation(speechiness,      popularity) << "\n";
    out << "\"acousticness\" "     << correlation(acousticness,     popularity) << "\n";
    out << "\"instrumentalness\" " << correlation(instrumentalness, popularity) << "\n";
    out << "\"liveness\" "         << correlation(liveness,         popularity) << "\n";
    out << "\"valence\" "          << correlation(valence,          popularity) << "\n";
    out << "\"tempo\" "            << correlation(tempo,            popularity) << "\n";
    out.close();

    ofstream gp("correlation_with_popularity.gp");
    gp << "set terminal png size 1000,600\n";
    gp << "set output 'correlation_with_popularity.png'\n";
    gp << "set title 'Correlation of Audio Features with Popularity'\n";
    gp << "set xlabel 'Feature'\n";
    gp << "set ylabel 'Correlation'\n";
    gp << "set style data histograms\n";
    gp << "set style fill solid\n";
    gp << "set xtics rotate by -30\n";
    gp << "set yrange [-0.15:0.1]\n";
    gp << "plot 'correlation_with_popularity.dat' using 2:xtic(1) title 'Correlation'\n";
    gp.close();

    system("/opt/homebrew/bin/gnuplot correlation_with_popularity.gp");
}

void writeEnergyPopularityRegression(const vector<Track>& tracks) {
    vector<double> x, y;

    for (const auto& t : tracks) {
        x.push_back(t.energy);
        y.push_back(t.popularity);
    }

    int n = x.size();
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

    for (int i = 0; i < n; i++) {
        sumX  += x[i];
        sumY  += y[i];
        sumXY += x[i] * y[i];
        sumX2 += x[i] * x[i];
    }

    double slope     = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    double intercept = (sumY - slope * sumX) / n;

    ofstream out("energy_vs_popularity.dat");
    ofstream pred("predicted_vs_actual.dat");

    for (int i = 0; i < n; i++) {
        double predicted = intercept + slope * x[i];
        out  << x[i] << " " << y[i]      << "\n";
        pred << y[i] << " " << predicted  << "\n";
    }
    out.close();
    pred.close();

    ofstream gp1("energy_vs_popularity.gp");
    gp1 << "set terminal png size 900,600\n";
    gp1 << "set output 'energy_vs_popularity.png'\n";
    gp1 << "set title 'Energy vs Popularity with Regression Line'\n";
    gp1 << "set xlabel 'Energy'\n";
    gp1 << "set ylabel 'Popularity'\n";
    gp1 << "set xrange [0:1]\n";
    gp1 << "set yrange [-5:105]\n";
    gp1 << "f(x) = " << intercept << " + " << slope << " * x\n";
    gp1 << "plot 'energy_vs_popularity.dat' using 1:2 with points pointtype 7 pointsize 0.3 title 'Tracks', ";
    gp1 << "f(x) with lines lw 2 title 'Regression Line'\n";
    gp1.close();

    system("/opt/homebrew/bin/gnuplot energy_vs_popularity.gp");

    ofstream gp2("predicted_vs_actual.gp");
    gp2 << "set terminal png size 900,600\n";
    gp2 << "set output 'predicted_vs_actual.png'\n";
    gp2 << "set title 'Predicted vs Actual Popularity'\n";
    gp2 << "set xlabel 'Actual Popularity'\n";
    gp2 << "set ylabel 'Predicted Popularity'\n";
    gp2 << "set xrange [-5:105]\n";
    gp2 << "set yrange [25:45]\n";
    gp2 << "plot 'predicted_vs_actual.dat' using 1:2 with points pointtype 7 pointsize 0.3 title 'Predictions', ";
    gp2 << "x with lines lw 2 title 'Perfect Prediction'\n";
    gp2.close();

    system("/opt/homebrew/bin/gnuplot predicted_vs_actual.gp");
}

int main(int argc, const char * argv[]) {
    vector<Track> tracks = loadCSV("dataset.csv");

    cout << "Loaded " << tracks.size() << " tracks." << endl;

    for (int i = 0; i < 3 && i < (int)tracks.size(); i++) {
        cout << tracks[i].track_name << " by " << tracks[i].artists
             << " | popularity: " << tracks[i].popularity
             << " | genre: " << tracks[i].track_genre << endl;
    }

    writePopularityDistribution(tracks);
    writeAveragePopularityByGenre(tracks);
    writeCorrelationWithPopularity(tracks);
    writeEnergyPopularityRegression(tracks);

    cout << "Graphs generated successfully." << endl;

    return 0;
}
