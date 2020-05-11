#include <limits>
#include <cmath>

namespace ai {


    int evaluate() {

    }

    int negaMax(int depth) {
        if (depth == 0)
            return evaluate();
        int max = -INFINITY;
        for (/* all moves */) {
            int score = -negaMax(depth - 1);
            if (score > max)
                max = score;
        }
        return max;
    }
}