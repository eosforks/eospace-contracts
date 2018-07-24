# Fenda

## Deploy the contract

### Prepare

Make an cleos alias to this project:

```
alias cleos='docker-compose -f $PWD/docker-compose-local-init.yml exec keosd /opt/eosio/bin/cleos -u http://nodeosd:8888 --print-request'
```

Create keys:

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
```

Import keys:

```
cleos wallet import 5Jbahmm7s13GnKWgJgbK6u3eRaKXEpJvzscfYM7qUmMGZzZ6gWg -n fenda
cleos wallet import 5J55Jo38g5TsZ3kNS3RqYQCjMpmznGvqV9F9byJAvu6qvxNAQgp -n fenda
cleos wallet import 5K3CXpXhAP4TyZkyqu19kYRGtm3pNgbUQnjbmwhyXcLmKWSYz7G -n fenda
cleos wallet import 5JdSZywsWpyrTjSTnQzzCn6Q8dc7BReDBfKSpDC9gntcUcsq98A -n fenda
```

Create test accounts:

```
cleos create account eosio fenda EOS5tGvzWo7qvsYTEoU2sMQcSxze3vfAfmtvz5hHvwkXnTEVhGAsR EOS5tGvzWo7qvsYTEoU2sMQcSxze3vfAfmtvz5hHvwkXnTEVhGAsR
cleos create account fenda user1 EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW
cleos create account fenda user2 EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW
cleos create account fenda user3 EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW
```

### Issue Token

Issue a token:

```
cleos create account eosio eosio.token EOS8VBwVgM7M44P8r3Rxj31cRchdF3jxD9iGG1r3isXyDiyEaU7Qx EOS8VBwVgM7M44P8r3Rxj31cRchdF3jxD9iGG1r3isXyDiyEaU7Qx
cleos set contract eosio.token /contracts/eosio.token -p eosio.token
cleos push action eosio.token create '[ "eosio", "10000000000.0000 SEK", 0, 0, 0]' -p eosio.token
cleos push action eosio.token issue '["fenda", "10000000000.0000 SEK", "fenda"]' -p eosio
```

See the contract balance:

```
cleos get currency balance eosio.token fenda
# 1000000000.0000 SEK
```

### Deploy the contract

```
# deploy the contract
cleos set contract fenda /opt/eosio/bin/eos-contracts/fenda
```

```
# update auth
cleos push action eosio updateauth '{"account":"fenda","permission":"active","parent":"owner","auth":{"keys":[{"key":"EOS5tGvzWo7qvsYTEoU2sMQcSxze3vfAfmtvz5hHvwkXnTEVhGAsR", "weight":1}],"threshold":1,"accounts":[{"permission":{"actor":"fenda","permission":"eosio.code"},"weight":1}],"waits":[]}}' -p fenda
```

User register(User will get 100 SEK after register)

```
cleos push action fenda regist '["user1"]' -p user1
cleos push action fenda regist '["user2"]' -p user2
cleos push action fenda regist '["user3"]' -p user3
```

Get user currency:

```
# get user currency(User will get 100 SEK after register)
cleos get currency balance eosio.token user1
# 100.0000 SEK
cleos get currency balance eosio.token user2
# 100.0000 SEK
cleos get currency balance eosio.token user3
# 100.0000 SEK
```

Update User Auth:

```
cleos push action eosio updateauth '{"account":"user1","permission":"active","parent":"owner","auth":{"keys":[{"key":"EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW", "weight":1}],"threshold":1,"accounts":[{"permission":{"actor":"fenda","permission":"eosio.code"},"weight":1}],"waits":[]}}' -p user1
cleos push action eosio updateauth '{"account":"user2","permission":"active","parent":"owner","auth":{"keys":[{"key":"EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW", "weight":1}],"threshold":1,"accounts":[{"permission":{"actor":"fenda","permission":"eosio.code"},"weight":1}],"waits":[]}}' -p user2
cleos push action eosio updateauth '{"account":"user3","permission":"active","parent":"owner","auth":{"keys":[{"key":"EOS6BedgfJZoLDenM8RJgppxV7CR5833NHReLtGXyD6dcaB9pHirW", "weight":1}],"threshold":1,"accounts":[{"permission":{"actor":"fenda","permission":"eosio.code"},"weight":1}],"waits":[]}}' -p user3
```

### Ask question with the token

```
# fenda->ask, ask a question with 1SEK
(user1 ask user2 with 1SEK at a timestamp of 1528596443(2018-06-10 10:07:23))
cleos push action fenda ask '["user1","user2","hello","world",1528596443,"1.0000 SEK"]' -p user1
cleos get currency balance eosio.token user1
# We can see that user1 already consumed 1SEK
# 99.0000 SEK
cleos get currency balance eosio.token user2
# user2 still has 100SEK
# 100.0000 SEK
cleos get currency balance eosio.token fenda
# We can see that 1SEK asked by user1 has already been sent to the contract.
# 999999701.0000 SEK
# Ask another question
cleos push action fenda ask '["user1","user2","hello22222","world2222",1528596619, "1.0000 SEK"]' -p user1
cleos get currency balance eosio.token user1
# 98.0000 SEK
cleos get currency balance eosio.token user2
# 100.0000 SEK
cleos get currency balance eosio.token fenda
# 999999702.0000 SEK
```

### Get questions

```
cleos get table fenda fenda question
```

Results:

```
{
  "rows": [{
      "id": 1,
      "asker": "user1",
      "responder": "user2",
      "title": "hello",
      "brief": "world",
      "value": "1.0000 SEK",
      "answer": "",
      "ask_time": 1528596443,
      "answer_time": 0,
      "state": "created",
      "like": 0,
      "dislike": 0,
      "like_users": [],
      "dislike_users": []
    },{
      "id": 2,
      "asker": "user1",
      "responder": "user2",
      "title": "hello22222",
      "brief": "world2222",
      "value": "1.0000 SEK",
      "answer": "",
      "ask_time": 1528596619,
      "answer_time": 0,
      "state": "created",
      "like": 0,
      "dislike": 0,
      "like_users": [],
      "dislike_users": []
    }
  ],
  "more": false
}
```

```
cleos get table fenda fenda user
```

```
{
  "rows": [{
      "id": "user1",
      "description": "",
      "questions": [
        1,
        2
      ],
      "answers": [],
      "like": 0,
      "dislike": 0,
      "like_questions": [],
      "dislike_questions": []
    },{
      "id": "user2",
      "description": "",
      "questions": [],
      "answers": [
        1,
        2
      ],
      "like": 0,
      "dislike": 0,
      "like_questions": [],
      "dislike_questions": []
    },{
      "id": "user3",
      "description": "",
      "questions": [],
      "answers": [],
      "like": 0,
      "dislike": 0,
      "like_questions": [],
      "dislike_questions": []
    }
  ],
  "more": false
}
```

### Answer question and get paid

Before user2 answer user1, we can check the balance:

```
cleos get currency balance eosio.token user1
# 98.0000 SEK
cleos get currency balance eosio.token user2
# 100.0000 SEK
```

user2 answer the question raised by user1:

```
cleos push action fenda answer '["user2",1,"good",1528596781]' -p user2
```

Check the balance after user2 answered the question of user1:

```
cleos get currency balance eosio.token user1
# 98.0000 SEK
cleos get currency balance eosio.token user2
# 100.0000 SEK
```

We can see that since the question raised by user1 has not been confirmed by user1, user2 can't get paied.

#### Accept question

Now user1 can accept question and user2 can get paied.

```
cleos push action fenda comment '["user1",1,"accepted",1528597046]' -p user1
cleos get currency balance eosio.token user2
# 101.0000 SEK
cleos get currency balance eosio.token user1
# 98.0000 SEK
```

#### Reject question

user1 can also reject question, and user2 can not get paid.

```
# user2 anser first
cleos push action fenda answer '["user2",2,"bad question",1528597183]' -p user2
# user1 reject the question
cleos push action fenda comment '["user1",2,"rejected",1528597193]' -p user1
```

Then we can check the balance:

```
cleos get currency balance eosio.token user1
# 99.0000 SEK
cleos get currency balance eosio.token user2
# 101.0000 SEK
```

#### Like/Dislike

When a question has been answered, others can `like`  or ` dislike` the question, one person can only `like` or `dislike` one question one time.

```
# user1 'like' answer 2:
cleos push action fenda like '["user1",2]' -p user1
# user2 `dislike` answer 1:
cleos push action fenda dislike '["user2",1]' -p user2
```

Finally, let's check the table:

```
cleos get table fenda fenda question
```

Check the questions:

```
{
  "rows": [{
      "id": 1,
      "asker": "user1",
      "responder": "user2",
      "title": "hello",
      "brief": "world",
      "value": "0.0000 SEK",
      "answer": "good",
      "ask_time": 1528596443,
      "answer_time": 1528596781,
      "state": "accepted",
      "like": 0,
      "dislike": 1,
      "like_users": [],
      "dislike_users": [
        "user2"
      ]
    },{
      "id": 2,
      "asker": "user1",
      "responder": "user2",
      "title": "hello22222",
      "brief": "world2222",
      "value": "0.0000 SEK",
      "answer": "bad question",
      "ask_time": 1528596619,
      "answer_time": 1528597183,
      "state": "rejected",
      "like": 1,
      "dislike": 0,
      "like_users": [
        "user1"
      ],
      "dislike_users": []
    }
  ],
  "more": false
}
```

Check the user table:

```
cleos get table fenda fenda user
```

Users results:

```
{
  "rows": [{
      "id": "user1",
      "description": "",
      "questions": [
        1,
        2
      ],
      "answers": [],
      "like": 1,
      "dislike": 0,
      "like_questions": [
        2
      ],
      "dislike_questions": []
    },{
      "id": "user2",
      "description": "",
      "questions": [],
      "answers": [
        1,
        2
      ],
      "like": 0,
      "dislike": 1,
      "like_questions": [],
      "dislike_questions": [
        "15426360343441440768"
      ]
    },{
      "id": "user3",
      "description": "",
      "questions": [],
      "answers": [],
      "like": 0,
      "dislike": 0,
      "like_questions": [],
      "dislike_questions": []
    }
  ],
  "more": false
}
```

#### Cancel question

When a question has not been answered, asker can cancel this question:

Let's create some questions first:

```
cleos push action fenda ask '["user1","user2","cancel the question","cancel",1528597793,"1.0000 SEK"]' -p user1
```

Then user1 can cancel his question:

```
cleos push action fenda cancel '["user1",3]' -p user1
```

Let's check the table, we can see that question 3 has been canceled.

```
cleos get table fenda fenda question
```

```
{
  "rows": [{
      "id": 1,
      "asker": "user1",
      "responder": "user2",
      "title": "hello",
      "brief": "world",
      "value": "0.0000 SEK",
      "answer": "good",
      "ask_time": 1528596443,
      "answer_time": 1528596781,
      "state": "accepted",
      "like": 0,
      "dislike": 1,
      "like_users": [],
      "dislike_users": [
        "user2"
      ]
    },{
      "id": 2,
      "asker": "user1",
      "responder": "user2",
      "title": "hello22222",
      "brief": "world2222",
      "value": "0.0000 SEK",
      "answer": "bad question",
      "ask_time": 1528596619,
      "answer_time": 1528597183,
      "state": "rejected",
      "like": 1,
      "dislike": 0,
      "like_users": [
        "user1"
      ],
      "dislike_users": []
    },{
      "id": 3,
      "asker": "user1",
      "responder": "user2",
      "title": "cancel the question",
      "brief": "cancel",
      "value": "0.0000 SEK",
      "answer": "",
      "ask_time": 1528597793,
      "answer_time": 0,
      "state": "canceled",
      "like": 0,
      "dislike": 0,
      "like_users": [],
      "dislike_users": []
    }
  ],
  "more": false
}
```

#### Refuse to answer a question

User can also refuse to answer the question, let's create one more question:

```
cleos push action fenda ask '["user2","user1","refuse to answer","refuse",1528597850,"1.0000 SEK"]' -p user2
```

user1 can refuse to answer this question:

```
cleos push action fenda refuse '["user1",4]' -p user1
```

Finally let's check the table:

```
{
  "rows": [{
      "id": 1,
      "asker": "user1",
      "responder": "user2",
      "title": "hello",
      "brief": "world",
      "value": "0.0000 SEK",
      "answer": "good",
      "ask_time": 1528596443,
      "answer_time": 1528596781,
      "state": "accepted",
      "like": 0,
      "dislike": 1,
      "like_users": [],
      "dislike_users": [
        "user2"
      ]
    },{
      "id": 2,
      "asker": "user1",
      "responder": "user2",
      "title": "hello22222",
      "brief": "world2222",
      "value": "0.0000 SEK",
      "answer": "bad question",
      "ask_time": 1528596619,
      "answer_time": 1528597183,
      "state": "rejected",
      "like": 1,
      "dislike": 0,
      "like_users": [
        "user1"
      ],
      "dislike_users": []
    },{
      "id": 3,
      "asker": "user1",
      "responder": "user2",
      "title": "cancel the question",
      "brief": "cancel",
      "value": "0.0000 SEK",
      "answer": "",
      "ask_time": 1528597793,
      "answer_time": 0,
      "state": "canceled",
      "like": 0,
      "dislike": 0,
      "like_users": [],
      "dislike_users": []
    },{
      "id": 4,
      "asker": "user2",
      "responder": "user1",
      "title": "refuse to answer",
      "brief": "refuse",
      "value": "0.0000 SEK",
      "answer": "",
      "ask_time": 1528597850,
      "answer_time": 0,
      "state": "refused",
      "like": 0,
      "dislike": 0,
      "like_users": [],
      "dislike_users": []
    }
  ],
  "more": false
}
```

That's all of the contract.