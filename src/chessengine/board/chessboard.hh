#pragma once

#include <boost/numeric/ublas/matrix.hpp>
#include <map>
#include <optional>
#include <vector>


#include "color.hh"

#include "move.hh"
#include "piece-move-builder.hh"

#include "piece.hh"
#include "piece-type.hh"
#include "position.hh"

namespace board {

    struct hash_pair {
        template <class T1, class T2>
        size_t operator()(const std::pair<T1, T2>& p) const
        {
            auto hash1 = std::hash<T1>{}(p.first);
            auto hash2 = std::hash<T2>{}(p.second);
            return hash1 ^ hash2;
        }
    };


    /*
    ** Implementation of Chessboard using a matrix and a list of pieces
    */
    class Chessboard {

    private:

        //matrix representation of the board [FILE][RANK]
        //squares hold a nullptr if empty, a pointer to a piece in the piece list otherwise
        boost::numeric::ublas::matrix<Piece**> board_ = boost::numeric::ublas::matrix<Piece**>(
                8, 8, nullptr/*(Piece**)malloc(sizeof(void*))*/);
        //all our pieces
        std::unordered_map<std::pair<PieceType, Color>, std::vector<Piece*>, hash_pair> pieces_ = {
                { { PieceType::PAWN, Color::BLACK }, std::vector<Piece*>() },
                { { PieceType::KNIGHT, Color::BLACK }, std::vector<Piece*>() },
                { { PieceType::BISHOP, Color::BLACK }, std::vector<Piece*>() },
                { { PieceType::QUEEN, Color::BLACK }, std::vector<Piece*>() },
                { { PieceType::KING, Color::BLACK }, std::vector<Piece*>() },
                { { PieceType::PAWN, Color::WHITE }, std::vector<Piece*>() },
                { { PieceType::KNIGHT, Color::WHITE }, std::vector<Piece*>() },
                { { PieceType::BISHOP, Color::WHITE }, std::vector<Piece*>() },
                { { PieceType::QUEEN, Color::WHITE }, std::vector<Piece*>() },
                { { PieceType::KING, Color::WHITE }, std::vector<Piece*>() },
        };

        //whose turn is it ?
        bool is_white_turn_ = true;

        //keep track of some events to assess legality of some moves
        bool did_white_king_castling_ = false;
        bool did_white_queen_castling_ = false;
        bool did_black_king_castling_ = false;
        bool did_black_queen_castling_ = false;

        //turns elapsed since start
        unsigned int current_turn_ = 0;

        //turns elapsed since a piece was last taken or a pawn moved
        std::vector<int> past_moves_halfmove_clocks_ = {0};
        std::vector<std::optional<Position>> past_moves_en_passant_target_squares_ = {std::nullopt};
        std::vector<Piece*> last_pieces_captured_;

        //used by constructors
        void put_piece(Piece* p);

        //these 3 methods are used by do_move
        void remove_piece(Piece *p);
        void move_piece(Piece *p, const Position& new_position);
        void promote_piece(Piece *p, const PieceType& type);

        //theses methods are used by undo_move
        void undo_remove_piece(Piece *p);
        void undo_move_piece(Piece *p, const Position& old_position);
        void undo_promote_piece(Piece *p, const PieceType& type);


    public:

        // constructors
        Chessboard() {

            //std::cerr << "initialising matrix\n";
            for (auto i = 0; i < 8; i++) {
                for (auto j = 0; j < 8; j++) {
                    board_(i,j) = (Piece**)(malloc(sizeof(void*)));
                    *board_(i,j) = nullptr;
                }
            }
            //std::cerr << "matrix of pointers initialised\n";

            for (auto position_per_fen_char : Chessboard::initial_positions) {
                auto attributes = Piece::piecetype_and_color_from_fen(position_per_fen_char.first);
                for (auto position : position_per_fen_char.second) {
                    put_piece(new Piece(position, attributes.second, attributes.first));
                }
            }
            //std::cout << to_string() << "\n";
        }

        //no chessboard copy thx
        Chessboard(const Chessboard &other) = delete; //TODO: make sure this does not break everything


        bool occupied_by(board::Color myColor, board::PieceType myType, board::Position myPos);

        //FEN string based constructor
        Chessboard(std::string fen_string);

        //destructor
        ~Chessboard() {
            //std::cerr << "entered chessboard destructor\n";
            for (auto it = board_.begin1(); it < board_.end1(); it++) {
                if (*(*it)) {
                    delete(*(*it));
                }
                free(*it);
            }
            for (auto it = last_pieces_captured_.begin(); it < last_pieces_captured_.end(); it++) {
                delete(*it);
            }
            //std::cerr << "destructed chessbaord\n";
        }

        bool operator==(Chessboard b);

        Move parse_uci_move(std::string uci_move);

        static std::shared_ptr<Chessboard> parse_uci(std::string uci_position);

        //main methods
        void do_move(const Move&, bool change_turn = true);

        bool is_move_legal(Move&, bool check_self_check = true);

        std::list<Move> generate_legal_moves(bool check_self_check = true);

        bool is_check();

        bool is_checkmate();

        bool is_draw();

        void undo_move(const Move&, bool change_turn = true);

        inline Piece** operator[](Position pos) {
            return board_((int) pos.file_get(), (int) pos.rank_get());
        }


        //Getters
        const std::unordered_map<std::pair<PieceType, Color>, std::vector<Piece*>, hash_pair>& get_pieces() {
            return pieces_;
        }

        //utils
        std::string to_string() const;

        inline Color whose_turn_is_it() const { return is_white_turn_ ? Color::WHITE : Color::BLACK; }

        inline const Piece* read(Position pos) const {//same as operator[], but read-only
            if (pos.file_get() == File::OUTOFBOUNDS ||
                pos.rank_get() == Rank::OUTOFBOUNDS)
                return nullptr;
            return *board_((int) pos.file_get(), (int) pos.rank_get());
        }


        void change_turn() { is_white_turn_ = !is_white_turn_; }

        friend class MoveLegalityChecker;

        //this is used to initialise initial_positions attribute, which represents the expected initial configuration of a chessboard
        static std::unordered_map<char, std::vector<Position>> initial_positions;
        static void initialise_chessboard_static_attributes() {
            initial_positions.insert({'R', { Position(File::A, Rank::ONE), Position(File::H, Rank::ONE)}});
            initial_positions.insert({'K', { Position(File::E, Rank::ONE)}});
            initial_positions.insert({'Q', { Position(File::D, Rank::ONE)}});
            initial_positions.insert({'N', { Position(File::B, Rank::ONE), Position(File::G, Rank::ONE)}});
            initial_positions.insert({'B', { Position(File::C, Rank::ONE), Position(File::F, Rank::ONE)}});
            initial_positions.insert({'k', { Position(File::E, Rank::EIGHT)}});
            initial_positions.insert({'q', { Position(File::D, Rank::EIGHT)}});
            initial_positions.insert({'r', { Position(File::A, Rank::EIGHT), Position(File::H, Rank::EIGHT)}});
            initial_positions.insert({'n', { Position(File::B, Rank::EIGHT), Position(File::G, Rank::EIGHT)}});
            initial_positions.insert({'b', { Position(File::C, Rank::EIGHT), Position(File::F, Rank::EIGHT)}});
            auto initial_black_pawn_positions = std::vector<Position>();
            auto initial_white_pawn_positions = std::vector<Position>();
            for (File f = File::A; f != File::OUTOFBOUNDS; f = f + 1) {
                initial_black_pawn_positions.emplace_back(Position(f, Rank::SEVEN));
                initial_white_pawn_positions.emplace_back(Position(f, Rank::TWO));
            }
            initial_positions.insert(std::make_pair('P', initial_white_pawn_positions));
            initial_positions.insert(std::make_pair('p', initial_black_pawn_positions));
        }
    };

    class MoveLegalityChecker {
        public:
        static bool is_move_legal_QUEEN(const Chessboard& b, const Move& move);
        static bool is_move_legal_KING(const Chessboard& b, const Move& move);
        static bool is_move_legal_ROOK(const Chessboard& b, const Move& move);
        static bool is_move_legal_KNIGHT(const Chessboard& b, const Move& move);
        static bool is_move_legal_BISHOP(const Chessboard& b, const Move& move);
        static bool is_move_legal_PAWN(const Chessboard& b, const Move& move);
        static bool is_move_legal(const Chessboard& b, const Move& move);
    };

}
