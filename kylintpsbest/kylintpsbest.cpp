#include <eosiolib/eosio.hpp>
using namespace eosio;

class kylintpsbest : public eosio::contract {
  public:
      using contract::contract;

      /// @abi action 
      void send(std::string msg) {}
};

EOSIO_ABI( kylintpsbest, (send) )