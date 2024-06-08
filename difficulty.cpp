#include "difficulty.h"

Difficulty::Difficulty() : mCurrentLevel(1) {}

void Difficulty::increaseDifficulty(int score) {
    mCurrentLevel = score / 100 + 1;
    // Adjust game parameters based on mCurrentLevel
}
