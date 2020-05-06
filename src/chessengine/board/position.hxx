namespace board
{
    inline Position::Position(File file, Rank rank)
        : file_(file)
        , rank_(rank)
    {}

    inline bool Position::operator==(const Position& pos) const
    {
        return file_get() == pos.file_get() && rank_get() == pos.rank_get();
    }

    inline bool Position::operator!=(const Position& pos) const
    {
        return !(*this == pos);
    }

    inline File Position::file_get() const
    {
        return file_;
    }

    inline Rank Position::rank_get() const
    {
        return rank_;
    }


    static std::string rank_to_string(const Rank& rank) {
        switch (rank) {
            case Rank::ONE:
                return "1";
            case Rank::TWO:
                return "2";
            case Rank::THREE:
                return "3";
            case Rank::FOUR:
                return "4";
            case Rank::FIVE:
                return "5";
            case Rank::SIX:
                return "6";
            case Rank::SEVEN:
                return "7";
            case Rank::EIGHT:
                return "8";
            default:
                return "UNKNOWN RANK";
        };
    }

    static std::string file_to_string(const File& file) {
        switch (file) {
            case File::A:
                return "A";
            case File::B:
                return "B";
            case File::C:
                return "C";
            case File::D:
                return "D";
            case File::E:
                return "E";
            case File::F:
                return "F";
            case File::G:
                return "G";
            case File::H:
                return "H";
            default:
                return "UNKNOWN FILE";
        };
    }

    inline std::string Position::to_string() const
    {
        return file_to_string(file_get()) + rank_to_string(rank_get());
    }

} // namespace board
