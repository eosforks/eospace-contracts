#include <eosiolib/eosio.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/currency.hpp>
#include <eosiolib/multi_index.hpp>
#include <vector>

/* Raw token contract */
#define OLD_TOKEN_ACCOUNT N(eosio.token)
/* Raw token type */
#define OLD_TOKEN_TYPE (eosio::symbol_type(eosio::string_to_symbol(4, "EOS")))

/* Your token contract */
#define NEW_TOKEN_ACCOUNT N(myairdrop111)
/* Your token type */
#define NEW_TOKEN_TYPE eosio::string_to_symbol(4, "AIRDROP")

class airdrop : public eosio::contract
{
  public:
    airdrop(account_name self) : eosio::contract(self), logs(_self, _self){};

    // @abi action
    void update(account_name owner)
    {
        accounts user_acnts(OLD_TOKEN_ACCOUNT, owner);
        const auto &user = user_acnts.get(OLD_TOKEN_TYPE.name(), "no balance object found");

        auto log_iter = logs.find(owner);
        if (log_iter == logs.end())
        {
            logs.emplace(_self, [&](auto &p) {
                p.id = owner;
            });
            airdrop_(owner, eosio::asset(user.balance.amount, NEW_TOKEN_TYPE));
        }
        else
        {
            //do nothing
        }
    };

  private:
    struct account
    {
        eosio::asset balance;

        uint64_t primary_key() const { return balance.symbol.name(); }
    };
    typedef eosio::multi_index<N(accounts), account> accounts;

    struct log
    {
        account_name id;

        EOSLIB_SERIALIZE(log, (id))
        uint64_t primary_key() const { return id; }
    };
    typedef eosio::multi_index<N(log), log> log_list;
    log_list logs;

    inline void airdrop_(account_name to, const eosio::asset &offer) const
    {
        eosio::action(
            eosio::permission_level{_self, N(active)},
            NEW_TOKEN_ACCOUNT, N(transfer),
            std::make_tuple(_self, to, offer, std::string("airdrop")))
            .send();
    }
};

EOSIO_ABI(airdrop, (update))