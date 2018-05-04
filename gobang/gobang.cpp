#include "gobang.hpp"

namespace gobang
{
struct game_impl
{
    static const board_type EMPTY_CELL = 0;
    static const board_type HOST_CELL = 1;
    static const board_type CHALLENGER_CELL = 2;

    index_type get_board_index(index_type x, index_type y)
    {
        return x * game::BOARD_LEN + y;
    }

    bool is_index_inrange(index_type x, index_type y)
    {
        // <0 is not needed in uint
        return !(x < 0 || x >= game::BOARD_LEN || y < 0 || y >= game::BOARD_LEN);
    }

    bool is_valid_movement(const movment &mvt, const game &gm)
    {
        if (!is_index_inrange(mvt.x, mvt.y))
        {
            return false;
        }

        auto m_index = get_board_index(mvt.x, mvt.y);
        return gm.board[m_index] == EMPTY_CELL;
    }

    bool check_win(index_type x, index_type y, const game &gm)
    {
        auto check_win_dir = [&](int dir_x, int dir_y) -> bool {

            const auto &board = gm.board;
            auto m_index = get_board_index(x, y);
            const auto CURR_CELL = gm.board[m_index];
            count_type m_count = 1;

            //positive dir
            for (auto i = x + dir_x, j = y + dir_y;
                 is_index_inrange(i, j) && gm.board[get_board_index(i, j)] == CURR_CELL;
                 i += dir_x, j += dir_y)
            {
                m_count++;
            };
            //negative dir
            for (auto i = x - dir_x, j = y - dir_y;
                 is_index_inrange(i, j) && gm.board[get_board_index(i, j)] == CURR_CELL;
                 i -= dir_x, j -= dir_y)
            {
                m_count++;
            };

            return m_count >= game::CONTINUOUS_CELLS;
        };
        return check_win_dir(1, 1) || check_win_dir(0, 1) || check_win_dir(-1, 1) || check_win_dir(1, 0);
    }

    void on(const create &c)
    {
        eosio::require_auth(c.host);
        eosio_assert(c.challenger != c.host,
                     "challenger shouldn't be the same as host");

        games existing_host_games(code_account, c.host);
        auto iter = existing_host_games.find(c.challenger);
        eosio_assert(iter == existing_host_games.end(),
                     "game already exists");

        existing_host_games.emplace(c.host, [&](auto &gm) {
            gm.challenger = c.challenger;
            gm.host = c.host;
            gm.reset_game();
        });
    }

    void on(const restart &r)
    {
        eosio::require_auth(r.by);

        games existing_host_games(code_account, r.host);
        auto iter = existing_host_games.find(r.challenger);
        eosio_assert(iter != existing_host_games.end(),
                     "game doesn't exists");
        eosio_assert(r.by == iter->host || r.by == iter->challenger,
                     "this is not your game!");

        existing_host_games.modify(iter, iter->host, [](auto &gm) {
            gm.reset_game();
        });
    }

    void on(const getboard &p)
    {
        eosio::require_auth(p.by);

        games existing_host_games(code_account, p.host);
        auto iter = existing_host_games.find(p.challenger);
        eosio_assert(iter != existing_host_games.end(),
                     "game doesn't exists");
        eosio_assert(p.by == iter->host || p.by == iter->challenger,
                     "this is not your game!");

        for (index_type i = 0, index = 0; i < game::BOARD_LEN * game::BOARD_LEN; ++i)
        {
            printi(iter->board[index++]);
        }
    }

    void on(const close &c)
    {
        eosio::require_auth(c.host);

        games existing_host_games(code_account, c.host);
        auto iter = existing_host_games.find(c.challenger);

        eosio_assert(iter != existing_host_games.end(), "game not exist.");
        existing_host_games.erase(iter);
    }

    void on(const movment &m)
    {
        eosio::require_auth(m.by);

        games existing_host_games(code_account, m.host);
        auto iter = existing_host_games.find(m.challenger);
        eosio_assert(iter != existing_host_games.end(), "game not exist.");

        eosio_assert(m.by == iter->host || m.by == iter->challenger, "this is not your game!");

        eosio_assert(iter->winner == game::NO_WINNER, "game has ended!.");

        eosio_assert(iter->turn == m.by, "not your turn!");

        eosio_assert(is_valid_movement(m, *iter), "not a valid movement!");

        const auto m_index = get_board_index(m.x, m.y);

        const auto cell_val = iter->turn == iter->host ? HOST_CELL : CHALLENGER_CELL;

        existing_host_games.modify(iter, iter->host, [&](auto &gm) {
            gm.board[m_index] = cell_val;
            if (check_win(m.x, m.y, gm))
            {
                gm.winner = gm.turn;
                return;
            }
            gm.turn = gm.turn == gm.host ? gm.challenger : gm.host;
        });
    }
    void apply(uint64_t /*receiver*/, uint64_t code, uint64_t action)
    {

        if (code == code_account)
        {
            if (action == N(create))
            {
                game_impl::on(eosio::unpack_action_data<gobang::create>());
            }
            else if (action == N(restart))
            {
                game_impl::on(eosio::unpack_action_data<gobang::restart>());
            }
            else if (action == N(getboard))
            {
                game_impl::on(eosio::unpack_action_data<gobang::getboard>());
            }
            else if (action == N(close))
            {
                game_impl::on(eosio::unpack_action_data<gobang::close>());
            }
            else if (action == N(movment))
            {
                game_impl::on(eosio::unpack_action_data<gobang::movment>());
            }
        }
    }
};
}

/**
*  The apply() methods must have C calling convention so that the blockchain can lookup and
*  call these methods.
*/
extern "C" {

using namespace gobang;
/// The apply method implements the dispatch of events to this contract
void apply(uint64_t receiver, uint64_t code, uint64_t action)
{
    game_impl().apply(receiver, code, action);
}

} // extern "C"
