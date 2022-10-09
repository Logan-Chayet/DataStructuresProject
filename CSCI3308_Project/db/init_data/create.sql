CREATE TABLE reviews (
    id SERIAL PRIMARY KEY,
    movie_name TEXT NOT NULL,
    movie_review TEXT NOT NULL,
    review_date TEXT NOT NULL
);