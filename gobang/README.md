# EOS五子棋合约: gobang

一个五子棋的简单合约，**参考**[Tic-Tac-Toe](https://github.com/EOSIO/eos/wiki/Tutorial-Tic-Tac-Toe)，以下实验基于**eosio/eos:20180521**这个版本实现。

## 工作部分

* 一些小修改

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
  void on(const getboard &p);
  ```

* 修改棋盘

  ```
  board_type board[BOARD_LEN * BOARD_LEN];
  ```

* 修改游戏逻辑

  ```
  bool is_index_inrange(index_type x, index_type y);
  bool is_valid_movement(const movment &mvt, const game &gm);
  bool check_win(index_type x, index_type y, const game &gm);
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

## 创建Key

首先我们给这个游戏单独创建一个key:

```
cleos create key
```

```
Private key: ***************************
Public key: EOS7VXqbbkqUxyhDYsJiwSuxzMPsV6JAMbezoJmU7hQ45VuBc5y3F
```

## 创建钱包

然后我们单独为这个游戏创建一个钱包:

```
cleos wallet create -n gobang
```

得到结果:

```
Creating wallet: gobang
Save password to use in the future to unlock this wallet.
Without password imported keys will not be retrievable.
"***************************"
```

## 导入key

这里需要导入在`cleos create key`中创建的 **private key** :

```
cleos wallet import -n gobang ******************************
```

## 创建账号

我们单独为这个游戏创建三个账号:

```
cleos create account eosio gobang EOS7VXqbbkqUxyhDYsJiwSuxzMPsV6JAMbezoJmU7hQ45VuBc5y3F EOS7VXqbbkqUxyhDYsJiwSuxzMPsV6JAMbezoJmU7hQ45VuBc5y3F
cleos create account eosio player1 EOS7VXqbbkqUxyhDYsJiwSuxzMPsV6JAMbezoJmU7hQ45VuBc5y3F EOS7VXqbbkqUxyhDYsJiwSuxzMPsV6JAMbezoJmU7hQ45VuBc5y3F
cleos create account eosio player2 EOS7VXqbbkqUxyhDYsJiwSuxzMPsV6JAMbezoJmU7hQ45VuBc5y3F EOS7VXqbbkqUxyhDYsJiwSuxzMPsV6JAMbezoJmU7hQ45VuBc5y3F
```

得到结果:

```
executed transaction: 2f7471a89e502ec549181211721a4a872fde37375c1ba59f76921a76db1c9595  288 bytes  248 us
#         eosio <= eosio::newaccount            {"creator":"eosio","name":"gobang","owner":{"threshold":1,"keys":[{"key":"EOS7VXqbbkqUxyhDYsJiwSuxzM...
warning: transaction executed locally, but may not be confirmed by the network yet
executed transaction: e6be0000f8b975d8e7aa18afa22ea1bb6b55528f4ba6c6ef76e4fb658b401d35  288 bytes  230 us
#         eosio <= eosio::newaccount            {"creator":"eosio","name":"player1","owner":{"threshold":1,"keys":[{"key":"EOS7VXqbbkqUxyhDYsJiwSuxz...
warning: transaction executed locally, but may not be confirmed by the network yet
executed transaction: f3b9590b76804dcd1759e256b82a9a3fbb22c512d2f545701f7a19cef56558de  288 bytes  262 us
#         eosio <= eosio::newaccount            {"creator":"eosio","name":"player2","owner":{"threshold":1,"keys":[{"key":"EOS7VXqbbkqUxyhDYsJiwSuxz...
warning: transaction executed locally, but may not be confirmed by the network yet
```

## 部署合约

请将下面的合约地址换成自己的合约地址：

```
cleos set contract gobang /opt/eosio/bin/eos-contracts/gobang
```

得到结果：

```
Reading WAST/WASM from /opt/eosio/bin/eos-contracts/gobang/gobang.wast...
Assembling WASM...
Publishing contract...
executed transaction: b7579d68f73bf62ea24ef0313f316032501fb3e1d2fe4d88394fa9daea558d99  4976 bytes  3013 us
#         eosio <= eosio::setcode               {"account":"gobang","vmtype":0,"vmversion":0,"code":"0061736d01000000016c1260000060027e7e0060017e006...
#         eosio <= eosio::setabi                {"account":"gobang","abi":"0000060467616d65000505626f6172640775696e74385b5d0a6368616c6c656e6765720c6...
warning: transaction executed locally, but may not be confirmed by the network yet
```

## 创建游戏





## 测试指令

**预准备**：创建三个账号`gobang`,`player1`,`player2` ，并设置gobang账号contracts为本五子棋合约

* 见[Tic-Tac-Toe](https://github.com/EOSIO/eos/wiki/Tutorial-Tic-Tac-Toe)

  ```
  cleos create account ${creator_name} gobang ${contract_pub_owner_key} ${contract_pub_active_key} --permission ${creator_name}@active

  cleos create account ${creator_name} player1 ${contract_pub_owner_key} ${contract_pub_active_key} --permission ${creator_name}@active

  cleos create account ${creator_name} player2 ${contract_pub_owner_key} ${contract_pub_active_key} --permission ${creator_name}@active

  cleos set contract gobang ${gobang_folder}
  ```


_创建游戏_  `成功`
```
cleos push action gobang create '{"challenger":"player2","host":"player1"}' --permission player1@active  
```
_获取棋盘状态_ `成功`
```
cleos push action gobang getboard '{"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active  
```
_玩家2在（0，0）下子_  `失败：不是玩家2回合`
```
cleos push action gobang movment '{"x":0,"y":0,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active
```
_玩家1在（0，0）下子_ `成功`
```
cleos push action gobang movment '{"x":0,"y":0,"challenger":"player2","host":"player1","by":"player2"}' --permission player1@active
```
_玩家1在（0，0）下子_ `失败：不是玩家1回合`
```
 cleos push action gobang movment '{"x":0,"y":0,"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active 
```

_玩家2在（0，0）下子_ `失败：非法位置（已经有子）`

```
 cleos push action gobang movment '{"x":0,"y":0,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active
```

_玩家2在（0，1）下子_ `成功`

```
cleos push action gobang movment '{"x":0,"y":1,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active
```

_获取棋盘状态_ `成功`

```
cleos push action gobang getboard '{"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active
```

_连下7子，玩家1达成五子条件_  `成功`

```
cleos push action gobang movment '{"x":1,"y":0,"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active

cleos push action gobang movment '{"x":1,"y":1,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active

cleos push action gobang movment '{"x":2,"y":0,"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active

cleos push action gobang movment '{"x":2,"y":1,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active

cleos push action gobang movment '{"x":3,"y":0,"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active

cleos push action gobang movment '{"x":3,"y":1,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active 

cleos push action gobang movment '{"x":4,"y":0,"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active
```

_玩家2在（4，1）下子_ `失败：游戏已结束`

```
cleos push action gobang movment '{"x":4,"y":1,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active
```

_获取棋盘信息_ `成功`

```
cleos push action gobang getboard '{"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active
```

_重置棋盘_ `成功`

```
cleos push action gobang restart '{"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active
```

_获取棋盘信息_ `成功，此时棋盘为空`

```
cleos push action gobang getboard '{"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active
```

_玩家2在（4，1）下子_  `失败：不是玩家2回合`

```
cleos push action gobang movment '{"x":4,"y":1,"challenger":"player2","host":"player1","by":"player2"}' --permission player2@active
```

_玩家1在（4，0）下子_  `成功`

```
cleos push action gobang movment '{"x":4,"y":0,"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active
```

_结束游戏_ `成功`

```
cleos push action gobang close '{"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active
```

_玩家1在（4，0）下子_ `失败：无该游戏`

```
cleos push action gobang movment '{"x":4,"y":0,"challenger":"player2","host":"player1","by":"player1"}' --permission player1@active
```

## 测试输出

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

