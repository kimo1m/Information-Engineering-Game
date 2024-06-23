#include "Score.h"

// Globals for adjusting villain behavior
extern float VILLAIN_GENERATION_INTERVAL;
extern float VILLAIN_SPEED;

Score::Score() : score(0) {}

void Score::increaseScore() {
    score++;
}

int Score::getScore() const {
    return score;
}

void Score::increase_level() {
    if (score >= 15) {
        VILLAIN_GENERATION_INTERVAL = 0.70f;
        VILLAIN_SPEED = 0.3f;
    }
    if (score > 30) {
        VILLAIN_GENERATION_INTERVAL = 0.50f;
        VILLAIN_SPEED = 0.5f;
    }
}
