#include <eosiolib/eosio.hpp>
using namespace eosio;

class eospacetwitt : public eosio::contract {
  public:
      using contract::contract;

      /// @abi action 
      void send(std::string sender, std::string msg) {}
};

EOSIO_ABI( eospacetwitt, (send) )