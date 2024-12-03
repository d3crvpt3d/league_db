#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "../include/jobject.h"
#include "../include/duckdb.h"


int main(int argc, char **argv){

	duckdb_database db;
	duckdb_connection conn;
	duckdb_result result;

	if(duckdb_open("../data/items.db", &db) == DuckDBError){
		fprintf(stderr, "Cant open data/items.db\n");
		return -1;
	}

	if(duckdb_connect(db, &conn) == DuckDBError){
		fprintf(stderr, "Cant open data/items.db\n");
		return -2;
	}
	
	if( duckdb_query(conn, "CREATE TABLE", duckdb_result *out_result) == DuckDBError){
		fprintf(stderr, "Query error");
		return -3;
	}

	duckdb_disconnect(&conn);
	duckdb_close(&db);

	return 0;
}
