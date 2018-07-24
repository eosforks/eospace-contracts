# Fenda

```
alias cleos='docker-compose -f $PWD/docker-compose-local-init.yml exec keosd /opt/eosio/bin/cleos -u http://nodeosd:8888'
```

```
cleos wallet create -n fenda
Creating wallet: fenda
Save password to use in the future to unlock this wallet.
Without password imported keys will not be retrievable.
"PW5KD113hVZTb72QqwUj4oTkwGV6YzgVRC1ePxbQrK6m2cSZAHkow"

cleos wallet open -n fenda && cleos wallet unlock -n fenda

cleos create key
Private key: 5Jbahmm7s13GnKWgJgbK6u3eRaKXEpJvzscfYM7qUmMGZzZ6gWg
Public key: EOS5tGvzWo7qvsYTEoU2sMQcSxze3vfAfmtvz5hHvwkXnTEVhGAsR

cleos create key
Private key: 5J55Jo38g5TsZ3kNS3RqYQCjMpmznGvqV9F9byJAvu6qvxNAQgp
Public key: EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW

cleos create key
Private key: 5K3CXpXhAP4TyZkyqu19kYRGtm3pNgbUQnjbmwhyXcLmKWSYz7G
Public key: EOS7hQNVa1FEEqKLq9a7vcrUN558Mm46EpeERy31Li4WEVUe8LoEb

cleos create key
Private key: 5JdSZywsWpyrTjSTnQzzCn6Q8dc7BReDBfKSpDC9gntcUcsq98A
Public key: EOS8VBwVgM7M44P8r3Rxj31cRchdF3jxD9iGG1r3isXyDiyEaU7Qx

# import key
cleos wallet import 5Jbahmm7s13GnKWgJgbK6u3eRaKXEpJvzscfYM7qUmMGZzZ6gWg -n fenda
cleos wallet import 5J55Jo38g5TsZ3kNS3RqYQCjMpmznGvqV9F9byJAvu6qvxNAQgp -n fenda
cleos wallet import 5K3CXpXhAP4TyZkyqu19kYRGtm3pNgbUQnjbmwhyXcLmKWSYz7G -n fenda
cleos wallet import 5JdSZywsWpyrTjSTnQzzCn6Q8dc7BReDBfKSpDC9gntcUcsq98A -n fenda

# create accounts
cleos create account eosio fenda EOS5tGvzWo7qvsYTEoU2sMQcSxze3vfAfmtvz5hHvwkXnTEVhGAsR EOS5tGvzWo7qvsYTEoU2sMQcSxze3vfAfmtvz5hHvwkXnTEVhGAsR
cleos create account fenda user1 EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW
cleos create account fenda user2 EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW
cleos create account fenda user3 EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW

# issue a token
cleos create account eosio eosio.token EOS8VBwVgM7M44P8r3Rxj31cRchdF3jxD9iGG1r3isXyDiyEaU7Qx EOS8VBwVgM7M44P8r3Rxj31cRchdF3jxD9iGG1r3isXyDiyEaU7Qx
cleos set contract eosio.token /contracts/eosio.token -p eosio.token
cleos push action eosio.token create '[ "eosio", "1000000000.0000 SEK", 0, 0, 0]' -p eosio.token
cleos push action eosio.token issue '["fenda", "1000000000.0000 SEK", "fenda"]' -p eosio

# deploy the contract
cleos set contract fenda /opt/eosio/bin/eos-contracts/fenda

# update auth
cleos push action eosio updateauth '{"account":"fenda","permission":"active","parent":"owner","auth":{"keys":[{"key":"EOS5tGvzWo7qvsYTEoU2sMQcSxze3vfAfmtvz5hHvwkXnTEVhGAsR", "weight":1}],"threshold":1,"accounts":[{"permission":{"actor":"fenda","permission":"eosio.code"},"weight":1}],"waits":[]}}' -p fenda

# user register
cleos push action fenda regist '["user1"]' -p user1
cleos push action fenda regist '["user2"]' -p user2
cleos push action fenda regist '["user3"]' -p user3

# get user currency(User will get 100 SEK after register)
cleos get currency balance eosio.token user1
# 100.0000 SEK
cleos get currency balance eosio.token user2
# 100.0000 SEK
cleos get currency balance eosio.token user3
# 100.0000 SEK

# fenda -> token
cleos push action eosio updateauth '{"account":"user1","permission":"active","parent":"owner","auth":{"keys":[{"key":"EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW", "weight":1}],"threshold":1,"accounts":[{"permission":{"actor":"fenda","permission":"eosio.code"},"weight":1}],"waits":[]}}' -p user1

# fenda->ask, ask a question with 1SEK
cleos push action fenda ask '["user1","user2","hello","world",123,"1.0000 SEK"]' -p user1
cleos get currency balance eosio.token user1
# 99.0000 SEK
cleos get currency balance eosio.token user2
# 100.0000 SEK
# Ask another question
cleos push action fenda ask '["user1","user2","hello22222","world2222",123, "1.0000 SEK"]' -p user1
cleos get currency balance eosio.token user1
# 98.0000 SEK
cleos get currency balance eosio.token user2
# 100.0000 SEK

# get question
cleos get table fenda fenda question
cleos get table fenda fenda user

# fenda->answer
cleos push action fenda answer '["user2",1,"good",1528596781]' -p user2
cleos get currency balance eosio.token user2

# fenda->comment.accepted
cleos push action fenda comment '["user1",1,"accepted",1528597046]' -p user1
cleos get currency balance eosio.token user2
# 101.0000 SEK
cleos get currency balance eosio.token user1
# 98.0000 SEK

# fenda->comment.rejected
cleos push action fenda answer '["user2",2,"bad question",1528597183]' -p user2
cleos push action fenda comment '["user1",2,"rejected",1528597193]' -p user1
cleos get currency balance eosio.token user1
# 99.0000 SEK
cleos get currency balance eosio.token user2
# 101.0000 SEK

# fenda->like
cleos push action fenda like '["user1",2]' -p user1
# fenda->like_questions
cleos push action fenda dislike '["user2",1]' -p user2

cleos get table fenda fenda question
cleos get table fenda fenda user

# fenda->cancel
cleos push action fenda ask '["user1","user2","cancel the question","cancel",1528597793,"1.0000 SEK"]' -p user1
cleos push action fenda cancel '["user1",3]' -p user1

# fenda->refuse
cleos push action fenda ask '["user2","user1","refuse to answer","refuse",1528597850,"1.0000 SEK"]' -p user1
cleos push action fenda refuse '["user1",4]' -p user1
```






