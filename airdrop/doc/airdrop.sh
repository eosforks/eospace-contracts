# Prepare
cleos create account eosio airdrop EOS5nWRCg3MEHH1iUS2rnkGi3fTPHZrSJCauRMssCmZ1UqU2Ev4v3 EOS5nWRCg3MEHH1iUS2rnkGi3fTPHZrSJCauRMssCmZ1UqU2Ev4v3
cleos set contract airdrop /eoscontracts/airdrop

# Create Accounts
cleos create account eosio user1 EOS7JHm18xBFWKL9TZGmn5zycD13opgFFJSJsWMb7QMBQkFUzSurq EOS7JHm18xBFWKL9TZGmn5zycD13opgFFJSJsWMb7QMBQkFUzSurq
cleos create account eosio user2 EOS7gpeednuaa4iRf3JxwxfxKHgWZ9jGxwbeeemdVSWBwfPJixBqF EOS7gpeednuaa4iRf3JxwxfxKHgWZ9jGxwbeeemdVSWBwfPJixBqF
cleos create account eosio user3 EOS7vhqZ1hSur76hJB6FTVXfbQE1EGxKhrMiF9bnC8sbsFj5YMFyt EOS7vhqZ1hSur76hJB6FTVXfbQE1EGxKhrMiF9bnC8sbsFj5YMFyt
cleos create account eosio eosio.token EOS5dCtDJ1kU4wCSNX32AhiAyka5wE1CZsBTjvbb3UWU8bYwzak5Z EOS5dCtDJ1kU4wCSNX32AhiAyka5wE1CZsBTjvbb3UWU8bYwzak5Z
cleos set contract eosio.token /opt/eosio/bin/eos-contracts/contracts/eosio.token

cleos push action eosio.token create '[ "eosio", "1000000000.0000 EOS", 0, 0, 0]' -p eosio.token
cleos push action eosio.token issue '["airdrop", "1000000000.0000 EOS", "airdrop"]' -p eosio
cleos push action eosio.token transfer '["airdrop","user1","1000.0000 EOS",""]' -p airdrop
cleos push action eosio.token transfer '["airdrop","user2","1000.0000 EOS",""]' -p airdrop
cleos push action eosio.token transfer '["airdrop","user3","1000.0000 EOS",""]' -p airdrop

# Issue Token
cleos create account eosio sekio.token EOS7vhqZ1hSur76hJB6FTVXfbQE1EGxKhrMiF9bnC8sbsFj5YMFyt EOS7vhqZ1hSur76hJB6FTVXfbQE1EGxKhrMiF9bnC8sbsFj5YMFyt
cleos set contract sekio.token /opt/eosio/bin/eos-contracts/contracts/eosio.token
cleos push action sekio.token create '[ "eosio", "1000000000.0000 SEK", 0, 0, 0]' -p sekio.token
cleos push action sekio.token issue '["airdrop", "1000000000.0000 SEK", "airdrop"]' -p eosio

# Update Auth
cleos push action eosio updateauth '{"account":"airdrop","permission":"active","parent":"owner","auth":{"keys":[{"key":"EOS5nWRCg3MEHH1iUS2rnkGi3fTPHZrSJCauRMssCmZ1UqU2Ev4v3", "weight":1}],"threshold":1,"accounts":[{"permission":{"actor":"airdrop","permission":"eosio.code"},"weight":1}],"waits":[]}}' -p airdrop

# Claim Tokens
cleos push action airdrop update '["user1"]' -p user1

# Query
cleos get currency balance eosio.token user1
cleos get currency balance sekio.token user1