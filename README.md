# Spotify Popularity Analysis

A C++ data analysis project exploring whether a song's audio characteristics (energy, danceability, loudness, etc.) can predict its popularity on Spotify — using a dataset of 114,000 tracks across 114 genres.

## The Question

Spotify assigns every track a popularity score (0–100) using its own internal algorithm. This project asks: **can that score be predicted from a song's sound alone** — or does it depend on something else entirely?

**Hypothesis going in:** energy and loudness would be the strongest predictors of popularity, and explicit tracks would tend to score higher.

**Result:** the hypothesis was wrong. Energy showed almost zero correlation with popularity (r ≈ 0.001), and a regression model using energy alone explained only ~2% of the variance (R² ≈ 0.02) in popularity. Genre identity turned out to matter far more than any single audio feature — pop-film and k-pop topped average popularity, while high-energy genres like metal ranked near the bottom.

## Dataset

[Spotify Tracks Dataset](https://www.kaggle.com/datasets/maharshipandya/-spotify-tracks-dataset) — 114,000 tracks, 114 genres (1,000 tracks each), with audio features including danceability, energy, loudness, tempo, acousticness, valence, speechiness, instrumentalness, and liveness, plus popularity score and explicit content flag.

## Methods

- **Correlation analysis** — computed the Pearson r value between each of the 9 audio features and popularity, to see which features (if any) actually relate to popularity.
- **Linear regression** — modeled energy vs. popularity using least-squares to find the best-fit line, then compared predicted vs. actual popularity to test the model's real predictive power.

## Results

| Plot | Finding |
|---|---|
| Popularity distribution | Right-skewed with a spike at 0 (many obscure/unlisted tracks); most tracks cluster between 20–60, very few exceed 80 |
| Average popularity by genre | Pop-film and k-pop rank highest; black-metal and classical rank lowest — high energy does **not** predict high popularity |
| Correlation heatmap | All 9 audio features show weak correlation with popularity; instrumentalness has the strongest (negative) relationship at r ≈ -0.10; energy is essentially flat at r ≈ 0.001 |
| Energy vs. popularity regression | Slope ≈ 0.09 — a full swing from lowest to highest energy only shifts predicted popularity by ~0.09 points |
| Predicted vs. actual popularity | Model predicts nearly every track between 33–35 regardless of actual score (R² ≈ 0.02) — confirming energy alone has no real predictive power |

## Conclusion

Sonic properties alone can't explain what makes a song popular. Genre identity mattered more than any individual audio feature, suggesting popularity is driven more by factors outside this dataset — artist recognition, release recency, and cultural context — than by a track's raw sound.

## Tech Stack

- **C++** — data loading, parsing, and statistical computation (correlation, linear regression)
- **gnuplot** — chart generation from computed `.dat` files

## How to Run

```bash
g++ main.cpp -o main
./main
```

This reads `dataset.csv`, runs the analysis, and generates all plots in the `graphs/` folder via gnuplot.

## Project Structure

```
├── main.cpp                          # Core analysis: CSV parsing, correlation, regression
├── dataset.csv                       # Spotify tracks dataset
├── graphs/                           # Generated output charts
├── *.dat                             # Computed data files (correlation values, distributions, etc.)
└── *.gp                              # gnuplot scripts for each chart
```
