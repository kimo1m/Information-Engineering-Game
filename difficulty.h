#pragma once

class Difficulty {
public:
    Difficulty();
    void increaseDifficulty(int score);

private:
    int mCurrentLevel;
};
