# gobang

一个五子棋的简单合约，参考[Tic-Tac-Toe](https://github.com/EOSIO/eos/wiki/Tutorial-Tic-Tac-Toe)

* 一些简单的修改

* 添加了一个显示棋盘一维数组的请求接口

  `getboard`:

  ```
  struct getboard
  {
      account_name challenger;
      account_name host;
      account_name by;
  
      EOSLIB_SERIALIZE(getboard, (challenger)(host)(by))
  };
  ```

  ```
  
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
  ```

* Tic-Tac-Toe中的movement与move在gobang中合并为movment:

  `movement`与`move`:

  ```
     struct movement {
        uint32_t    row;
        uint32_t    column;
  
        EOSLIB_SERIALIZE( movement, (row)(column) )
     };
  
     struct move {
        account_name   challenger;
        account_name   host;
        account_name   by; // the account who wants to make the move
        movement       mvt;
  
        EOSLIB_SERIALIZE( move, (challenger)(host)(by)(mvt) )
     };
  ```

  `movment`:

  ```
  struct movment
  {
      index_type x;
      index_type y;
  
      account_name challenger;
      account_name host;
      account_name by;
  
      EOSLIB_SERIALIZE(movment, (x)(y)(challenger)(host)(by))
  };
  ```

##测试输出:

```
± % cleos push action gobang create '{"challenger":"player2","host":"player1"}' --permission player1@active                                                                                                                                          !4327
executed transaction: db56626db08c243bd1bddccf60e8c59cac55711a15e66d2886ea95aa7ba30b09  240 bytes  103424 cycles
#        gobang <= gobang::create               {"challenger":"player2","host":"player1"}


% cleos push action gobang getboard '{"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active                                                                                                                          !4356
executed transaction: af723893f6ba1db1c35a93eba7e57d685601b540472dc5e262664ae08881848b  248 bytes  112640 cycles
#        gobang <= gobang::getboard             {"challenger":"player2","host":"player1","by":"player1"}
>> 0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000


± % cleos push action gobang movment '{"x":0,"y":0,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active                                                                                                              !4330
Error 3030000: transaction validation exception
Ensure that your transaction satisfy the contract's constraint!
Error Details:
condition: assertion failed: not your turn!


± % cleos push action gobang movment '{"x":0,"y":0,"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active                                                                                                              !4332
executed transaction: 4d23ab4249e555d478c4d19d0f62cb0e8f9ff4608f57541fd9e9af0b2177fd9c  256 bytes  105472 cycles
#        gobang <= gobang::movment              {"x":0,"y":0,"challenger":"player2","host":"player1","by":"player1"}


± % cleos push action gobang movment '{"x":0,"y":0,"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active                                                                                                              !4334
Error 3030000: transaction validation exception
Ensure that your transaction satisfy the contract's constraint!
Error Details:
condition: assertion failed: not your turn!


± % cleos push action gobang movment '{"x":0,"y":0,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active                                                                                                              !4335
Error 3030000: transaction validation exception
Ensure that your transaction satisfy the contract's constraint!
Error Details:
condition: assertion failed: not a valid movement!


± % cleos push action gobang movment '{"x":0,"y":1,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active                                                                                                              !4336
executed transaction: 172d6b4706576265fc168205d5f26bff673233dea4fb86dbfc2362f48748d677  256 bytes  105472 cycles
#        gobang <= gobang::movment              {"x":0,"y":1,"challenger":"player2","host":"player1","by":"player2"}

% cleos push action gobang getboard '{"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active                                                                                                                          !4363
executed transaction: af62b30ca6fcb3ddc4f39be612b9a5fdfcfa414566a775ac04a4df8ab536124c  248 bytes  112640 cycles
#        gobang <= gobang::getboard             {"challenger":"player2","host":"player1","by":"player1"}
>> 1200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000

± % cleos push action gobang movment '{"x":1,"y":0,"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active                                                                                                              !4337
executed transaction: 7cca43bdda936915941c0c681ad7e11713bab17c7e304df998566c933a0b1736  256 bytes  105472 cycles
#        gobang <= gobang::movment              {"x":1,"y":0,"challenger":"player2","host":"player1","by":"player1"}

± % cleos push action gobang movment '{"x":1,"y":1,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active                                                                                                              !4338
executed transaction: e64176549d341ff6d0e23e370a2a835bbb1413243f062ef39be965d5318f6ca1  256 bytes  106496 cycles
#        gobang <= gobang::movment              {"x":1,"y":1,"challenger":"player2","host":"player1","by":"player2"}

± % cleos push action gobang movment '{"x":2,"y":0,"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active                                                                                                              !4339
executed transaction: 62f0465be0f69af280fe45c7e85c545bd91763b2f7dfd272d1230399e32b9b2e  256 bytes  105472 cycles
#        gobang <= gobang::movment              {"x":2,"y":0,"challenger":"player2","host":"player1","by":"player1"}

± % cleos push action gobang movment '{"x":2,"y":1,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active                                                                                                              !4340
executed transaction: 9f028bb8fcc225beba702a81cd0070057ee96735453d6edf441b9f5332c49be3  256 bytes  106496 cycles
#        gobang <= gobang::movment              {"x":2,"y":1,"challenger":"player2","host":"player1","by":"player2"}

± % cleos push action gobang movment '{"x":3,"y":0,"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active                                                                                                              !4341
executed transaction: 241b79e8a5bd75ab8f13b672271916d63c7f67a6a3ff223c2d2597a694ce3777  256 bytes  105472 cycles
#        gobang <= gobang::movment              {"x":3,"y":0,"challenger":"player2","host":"player1","by":"player1"}

± % cleos push action gobang movment '{"x":3,"y":1,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active                                                                                                              !4342
executed transaction: 6848dc22188e0c2e61f264973dcb365df618fe190f10543edb3aed02a35cb193  256 bytes  106496 cycles
#        gobang <= gobang::movment              {"x":3,"y":1,"challenger":"player2","host":"player1","by":"player2"}

± % cleos push action gobang movment '{"x":4,"y":0,"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active                                                                                                              !4343
executed transaction: b8d7e2e8d49713f282ac4eb668f7979219847dcc2bb133beeb58d8b8a609d53d  256 bytes  105472 cycles
#        gobang <= gobang::movment              {"x":4,"y":0,"challenger":"player2","host":"player1","by":"player1"}

± % cleos push action gobang movment '{"x":4,"y":1,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active                                                                                                              !4344
Error 3030000: transaction validation exception
Ensure that your transaction satisfy the contract's constraint!
Error Details:
condition: assertion failed: game has ended!.


% cleos push action gobang getboard '{"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active                                                                                                                          !4372
executed transaction: 224259b5abf221eabdf3ac6809be3cacf9909b2861a0611ec278b0e15f3be780  248 bytes  112640 cycles
#        gobang <= gobang::getboard             {"challenger":"player2","host":"player1","by":"player1"}
>> 1200000000000000000012000000000000000000120000000000000000001200000000000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000


± % cleos push action gobang restart '{"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active                                                                                                                          !4346
executed transaction: 494c1938d0e3ca680eeaabc6b8cf9cd2d5767481d6db1f4d16f834b652284a6c  248 bytes  104448 cycles
#        gobang <= gobang::restart              {"challenger":"player2","host":"player1","by":"player1"}


cleos push action gobang getboard '{"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active                                                                                                                          !4375
executed transaction: 8c9ce8aa249cb9aec5279e395ac8dd2084c95ad5376f96219aadabed2e17cbbf  248 bytes  112640 cycles
#        gobang <= gobang::getboard             {"challenger":"player2","host":"player1","by":"player1"}
>> 0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000


± % cleos push action gobang movment '{"x":4,"y":1,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active                                                                                                              !4347
Error 3030000: transaction validation exception
Ensure that your transaction satisfy the contract's constraint!
Error Details:
condition: assertion failed: not your turn!

± % cleos push action gobang movment '{"x":4,"y":0,"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active                                                                                                              !4348
executed transaction: d8d846a7f94e81fb4e7396ca5ee1a761c1ca2757d7103c6d299612bff083e614  256 bytes  105472 cycles
#        gobang <= gobang::movment              {"x":4,"y":0,"challenger":"player2","host":"player1","by":"player1"}

± % cleos push action gobang close '{"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active                                                                                                                            !4349
executed transaction: 6d579a5aa11b21ea8d521edbad6bad436592ba6e21a75c2a8cd4fb0025df4628  240 bytes  105472 cycles
#        gobang <= gobang::close                {"challenger":"player2","host":"player1"}

± % cleos push action gobang movment '{"x":4,"y":0,"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active                                                                                                              !4350
Error 3030000: transaction validation exception
Ensure that your transaction satisfy the contract's constraint!
Error Details:
condition: assertion failed: game not exist.

```

