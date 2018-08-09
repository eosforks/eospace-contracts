# Deploy Contract On Kylin-Testnet

# 1. Deploy airdrop contract
cleos set contract myairdropooo /eospace-contracts/airdrop
# 2. Deploy token contract
cleos set contract myairdrop111 /contracts/eosio.token
# 2.1 Create the airdrop token
cleos push action myairdrop111 create '[ "myairdrop111", "10000000000.0000 AIRDROP", 0, 0, 0]' -p myairdrop111
# 2.2 Give airdrop contract some tokens
cleos push action myairdrop111 issue '["myairdropooo", "1000000000.0000 AIRDROP", "Give myairdropooo tokens for airdrop"]' -p myairdrop111
# 3. Update airdrop contract's permission
cleos push action eosio updateauth '{"account":"myairdropooo","permission":"active","parent":"owner","auth":{"keys":[{"key":"EOS8AsxJzm3fwAGwSK3Y5KQVRV3GSYYswhwmvjbdmc5c2PfMb9dwX","weight":1}],"threshold":1,"accounts":[{"permission":{"actor":"myairdropooo","permission":"eosio.code"},"weight":1}],"waits":[]}}' -p myairdropooo

# Check balance before airdrop
cleos get currency balance myairdrop111 myairdropooo
# 1000000000.0000 AIRDROP
cleos get currency balance eosio.token myairdrop112
# 60.0000 EOS
cleos get currency balance eosio.token myairdrop113
# 60.0000 EOS
cleos get currency balance myairdrop111 myairdrop112
# []
cleos get currency balance myairdrop111 myairdrop113
# []

# Users claim airdrop tokens
cleos push action myairdropooo update '["myairdrop112"]' -p myairdrop112
cleos push action myairdropooo update '["myairdrop113"]' -p myairdrop113

# Check balance
cleos get currency balance myairdrop111 myairdropooo
# 999999940.0000 AIRDROP
cleos get currency balance myairdrop111 myairdrop112
# 60.0000 AIRDROP
