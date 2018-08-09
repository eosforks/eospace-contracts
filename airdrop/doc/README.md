# Airdrop contract

The EOS user can send 0EOS to the contract address to get the contract token that matches the user's balance.

## Before you run

Modify these by your own needs.
```
/* Raw token contract */
#define OLD_TOKEN_ACCOUNT N(eosio.token)
/* Raw token type */
#define OLD_TOKEN_TYPE (eosio::symbol_type(eosio::string_to_symbol(4, "EOS")))

/* Your token contract */
#define NEW_TOKEN_ACCOUNT N(myairdrop111)
/* Your token type */
#define NEW_TOKEN_TYPE eosio::string_to_symbol(4, "AIRDROP")
```

## Usage
- First ensure that the number of NEW_TOKEN_TYPE virtual tokens of the contract is sufficient.
- Then claim the token like this: `cleos push action airdrop update '["user1"]' -p user1`, where `user1` is the account name of your EOS account.