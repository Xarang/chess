#pragma once

#include <utility>

#include <string>
#include "../../utype.hh"

namespace board
{
    /* The file enum represent the colomns
     * on the board */
    enum class File
    {
        OUTOFBOUNDS = -1,
        A = 0,
        B = 1,
        C = 2,
        D = 3,
        E = 4,
        F = 5,
        G = 6,
        H = 7
    };

    File operator+(File f, int incr);
    File operator-(File f, int incr);

    /* The rank enum represent the lines
     * on the board
     * 
     * the value is the index in the matrix which is why it is value - 1
     *  */
    enum class Rank
    {
        OUTOFBOUNDS = -1,
        ONE = 0,
        TWO = 1,
        THREE = 2,
        FOUR = 3,
        FIVE = 4,
        SIX = 5,
        SEVEN = 6,
        EIGHT = 7
    };
    
    Rank operator+(Rank f, int incr);
    Rank operator-(Rank f, int incr);
    

    /* Position represent a coordinate on the board */
    class Position final
    {
    public:
        Position() : file_(File::A), rank_(Rank::ONE) {};
        Position(File file, Rank rank);

        File file_get() const;
        Rank rank_get() const;

        bool operator==(const Position& pos) const;
        bool operator!=(const Position& pos) const;

        std::string to_string() const;
        
    private:
        File file_;
        Rank rank_;
    };

} // namespace board

#include "position.hxx"
