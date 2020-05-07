#include "position.hh"

namespace board
{
    File handleFileOOB(int n) {
        if (n > (int)File::H || n < (int)File::A) {
            return File::OUTOFBOUNDS;
        }
        return (File)(n);
    }

    bool operator<(File f, File f1)
    {
        return (int)(f) < (int)f1;
    }

    bool operator>(File f, File f1)
    {
        return (int)(f) < (int)f1;
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

    bool operator<(Rank f, Rank f1)
    {
        return (int)(f) < (int)f1;
    }
    bool operator>(Rank f, Rank f1)
    {
        return (int)(f) < (int)f1;
    }
    Rank operator+(Rank r, int incr) {
        return handleRankOOB((int)r + incr);
    }

    Rank operator-(Rank r, int incr) {
        return handleRankOOB((int)r - incr);
    }

} // namespace