#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
// #include <iostream>
#include <cstring>

namespace gobang
{
using index_type = uint32_t;
using count_type = uint32_t;
using board_type = uint8_t;

static const account_name games_account = N(gobang_games);
static const account_name code_account = N(gobang);

struct game
{
    static const count_type BOARD_LEN = 20;
    static const count_type CONTINUOUS_CELLS = 5;
    static const account_name NO_WINNER = N(none);
    static const account_name NO_COUNT = NO_WINNER;

    board_type board[BOARD_LEN * BOARD_LEN];

    account_name challenger;
    account_name host;

    account_name turn;
    account_name winner;

    game()
        : challenger(NO_COUNT), host(NO_COUNT), turn(NO_COUNT), winner(NO_WINNER)
    {
        std::memset(board, 0, sizeof(board));
    }

    game(account_name challenger_account, account_name host_account)
        : challenger(challenger_account), host(host_account), turn(host_account), winner(NO_WINNER)
    {
        std::memset(board, 0, sizeof(board));
    }

    void reset_game()
    {
        std::memset(board, 0, sizeof(board));
        turn = host;
        winner = NO_WINNER;
    }

    auto primary_key() const { return challenger; }

    EOSLIB_SERIALIZE(game, (board)(host)(challenger)(turn)(winner))
};

struct create
{
    account_name challenger;
    account_name host;

    EOSLIB_SERIALIZE(create, (challenger)(host))
};

struct restart
{
    account_name challenger;
    account_name host;
    account_name by;

    EOSLIB_SERIALIZE(restart, (challenger)(host)(by))
};

struct getboard
{
    account_name challenger;
    account_name host;
    account_name by;

    EOSLIB_SERIALIZE(getboard, (challenger)(host)(by))
};

struct close
{
    account_name challenger;
    account_name host;

    EOSLIB_SERIALIZE(close, (challenger)(host))
};

struct movment
{
    index_type x;
    index_type y;

    account_name challenger;
    account_name host;
    account_name by;

    EOSLIB_SERIALIZE(movment, (x)(y)(challenger)(host)(by))
};

typedef eosio::multi_index<games_account, game> games;
};