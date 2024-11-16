#!/bin/bash

VERSION=$(curl -s https://ddragon.leagueoflegends.com/api/versions.json | jq .[0] -r)

curl -s https://ddragon.leagueoflegends.com/cdn/${VERSION}/data/en_US/item.json > src/items.json
