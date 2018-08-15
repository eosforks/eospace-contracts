#!/bin/bash
# chose one API endpoint
alias cleos='docker-compose -f $PWD/docker-compose-local-init.yml exec keosd /opt/eosio/bin/cleos -u https://api.kylin-testnet.eospace.io --print-request --print-response'
alias cleos='docker-compose -f $PWD/docker-compose-local-init.yml exec keosd /opt/eosio/bin/cleos -u https://api-kylin.eosasia.one --print-request --print-response'
alias cleos='docker-compose -f $PWD/docker-compose-local-init.yml exec keosd /opt/eosio/bin/cleos -u https://api-kylin.eoslaomao.com --print-request --print-response'
alias cleos='docker-compose -f $PWD/docker-compose-local-init.yml exec keosd /opt/eosio/bin/cleos -u https://api-kylin.eosasia.one --print-request'

cleos set contract kylintpsbest /eospace-contracts/kylintpsbest -p kylintpsbest@active
cleos push action kylintpsbest send '[ "KylinTestnet TPS Best!" ]' -p kylintpsbest@kylintps
