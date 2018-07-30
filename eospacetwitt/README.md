# EOSpace-Twitt

Simple yet powerful contract to record information in EOS Blockchain.

## Deploy

This simple contract has already been deployed in https://www.cryptokylin.io , you can also deploy the contract yourself just by running:

```
cleos set contract eospacetwitt /path/to/eospacetwitt -p eospacetwitt@active
```

`/path/to/eospacetwitt` should be replaced by own directory.

## How to run

```
cleos -u https://api-kylin.eosasia.one push action eospacetwitt send '[ "eospacetwitt", "We love EOS" ]' -p eospacetwitt@active
```

Have fun with it!

## About

All the data is on chain, and it can be found here: https://www.cryptokylin.io/#/tx/eospacetwitt