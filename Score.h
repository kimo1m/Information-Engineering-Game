#pragma once

// Score class definition
class Score {
private:
    int score;

public:
    Score();
    void increaseScore();
    int getScore() const;
    void increase_level();
};
