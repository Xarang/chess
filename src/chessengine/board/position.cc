#include "position.hh"

namespace board
{
    File handleFileOOB(int n) {
        if (n > (int)File::H || n < (int)File::A) {
            return File::OUTOFBOUNDS;
        }
        return (File)(n);
    }

    File operator+(File f, int incr) {
        return handleFileOOB((int)f + incr);
    }

    File operator-(File f, int incr) {
        return handleFileOOB((int)f - incr);
    }

    Rank handleRankOOB(int n) {
        if (n > (int)Rank::EIGHT || n < (int)Rank::ONE) {
            return Rank::OUTOFBOUNDS;
        }
        return (Rank)(n);
    }

    Rank operator+(Rank r, int incr) {
        return handleRankOOB((int)r + incr);
    }

    Rank operator-(Rank r, int incr) {
        return handleRankOOB((int)r - incr);
    }

} // namespace