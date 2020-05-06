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
        OUTOFBOUNDS,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
    };

    File operator+(File f, int incr);
    File operator-(File f, int incr);

    /* The rank enum represent the lines
     * on the board */
    enum class Rank
    {
        OUTOFBOUNDS,
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
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
