# Usage:
1. download duckdb Zip file (https://duckdb.org/docs/installation/)
2. Extract
3. put duckdb.h in include/
4. put libduckdb.so and libduck_static.a into lib/
5. `make`
6. run ./update_items.sh
7. run ./parser
# for lazy ppl
Linux x86_64:
in `league_db/` with "upzip":
```
curl -s -o ./libduckdb-linux-amd64.zip https://github.com/duckdb/duckdb/releases/download/v1.1.3/libduckdb-linux-amd64.zip && unzip libduckdb-linux-amd64.zip && rm duckdb.hpp && mv duckdb.h include/ && mv libduck* lib/ && make && ./update_items.sh && ./parser && echo DONE..
```
