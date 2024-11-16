#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cjson/cJSON.h"
#include "../include/duckdb.h"

// Function to create the database schema
void create_tables(duckdb_connection connection) {
    // Main items table
    duckdb_state state = duckdb_query(connection,
        "CREATE TABLE IF NOT EXISTS items ("
        "    item_id VARCHAR PRIMARY KEY,"
        "    name VARCHAR,"
        "    description TEXT,"
        "    colloq VARCHAR,"
        "    plaintext TEXT,"
        "    stacks INTEGER,"
        "    consumed BOOLEAN,"
        "    in_store BOOLEAN,"
        "    hide_from_all BOOLEAN,"
        "    depth INTEGER"
        ");", NULL);

    // Image details table
    duckdb_query(connection,
        "CREATE TABLE IF NOT EXISTS item_images ("
        "    item_id VARCHAR PRIMARY KEY,"
        "    full_image VARCHAR,"
        "    sprite VARCHAR,"
        "    image_group VARCHAR,"
        "    x INTEGER,"
        "    y INTEGER,"
        "    w INTEGER,"
        "    h INTEGER,"
        "    FOREIGN KEY (item_id) REFERENCES items(item_id)"
        ");", NULL);

    // Gold details table
    duckdb_query(connection,
        "CREATE TABLE IF NOT EXISTS item_gold ("
        "    item_id VARCHAR PRIMARY KEY,"
        "    base INTEGER,"
        "    purchasable BOOLEAN,"
        "    total INTEGER,"
        "    sell INTEGER,"
        "    FOREIGN KEY (item_id) REFERENCES items(item_id)"
        ");", NULL);

    // Tags table (many-to-many)
    duckdb_query(connection,
        "CREATE TABLE IF NOT EXISTS item_tags ("
        "    item_id VARCHAR,"
        "    tag VARCHAR,"
        "    PRIMARY KEY (item_id, tag),"
        "    FOREIGN KEY (item_id) REFERENCES items(item_id)"
        ");", NULL);

    // Maps table (many-to-many)
    duckdb_query(connection,
        "CREATE TABLE IF NOT EXISTS item_maps ("
        "    item_id VARCHAR,"
        "    map_id VARCHAR,"
        "    enabled BOOLEAN,"
        "    PRIMARY KEY (item_id, map_id),"
        "    FOREIGN KEY (item_id) REFERENCES items(item_id)"
        ");", NULL);

    // Stats table
    duckdb_query(connection,
        "CREATE TABLE IF NOT EXISTS item_stats ("
        "    item_id VARCHAR,"
        "    stat_name VARCHAR,"
        "    stat_value DOUBLE,"
        "    PRIMARY KEY (item_id, stat_name),"
        "    FOREIGN KEY (item_id) REFERENCES items(item_id)"
        ");", NULL);
}

// Function to insert item data
void insert_item(duckdb_connection connection, const char* item_id, cJSON* item) {
    char query[4096];
    
    // Insert main item data
    snprintf(query, sizeof(query),
        "INSERT INTO items (item_id, name, description, colloq, plaintext, stacks, "
        "consumed, in_store, hide_from_all, depth) VALUES ('%s', '%s', '%s', '%s', '%s', %d, %d, %d, %d, %d);",
        item_id,
        cJSON_GetObjectItem(item, "name")->valuestring,
        cJSON_GetObjectItem(item, "description")->valuestring,
        cJSON_GetObjectItem(item, "colloq")->valuestring,
        cJSON_GetObjectItem(item, "plaintext")->valuestring,
        cJSON_GetObjectItem(item, "stacks") ? cJSON_GetObjectItem(item, "stacks")->valueint : 0,
        cJSON_GetObjectItem(item, "consumed") ? cJSON_GetObjectItem(item, "consumed")->valueint : 0,
        cJSON_GetObjectItem(item, "inStore") ? cJSON_GetObjectItem(item, "inStore")->valueint : 0,
        cJSON_GetObjectItem(item, "hideFromAll") ? cJSON_GetObjectItem(item, "hideFromAll")->valueint : 0,
        cJSON_GetObjectItem(item, "depth") ? cJSON_GetObjectItem(item, "depth")->valueint : 0
    );
    duckdb_query(connection, query, NULL);

    // Insert image data
    cJSON* image = cJSON_GetObjectItem(item, "image");
    if (image) {
        snprintf(query, sizeof(query),
            "INSERT INTO item_images VALUES ('%s', '%s', '%s', '%s', %d, %d, %d, %d);",
            item_id,
            cJSON_GetObjectItem(image, "full")->valuestring,
            cJSON_GetObjectItem(image, "sprite")->valuestring,
            cJSON_GetObjectItem(image, "group")->valuestring,
            cJSON_GetObjectItem(image, "x")->valueint,
            cJSON_GetObjectItem(image, "y")->valueint,
            cJSON_GetObjectItem(image, "w")->valueint,
            cJSON_GetObjectItem(image, "h")->valueint
        );
        duckdb_query(connection, query, NULL);
    }

    // Insert gold data
    cJSON* gold = cJSON_GetObjectItem(item, "gold");
    if (gold) {
        snprintf(query, sizeof(query),
            "INSERT INTO item_gold VALUES ('%s', %d, %d, %d, %d);",
            item_id,
            cJSON_GetObjectItem(gold, "base")->valueint,
            cJSON_GetObjectItem(gold, "purchasable")->valueint,
            cJSON_GetObjectItem(gold, "total")->valueint,
            cJSON_GetObjectItem(gold, "sell")->valueint
        );
        duckdb_query(connection, query, NULL);
    }

    // Insert tags
    cJSON* tags = cJSON_GetObjectItem(item, "tags");
    if (tags) {
        cJSON* tag;
        cJSON_ArrayForEach(tag, tags) {
            snprintf(query, sizeof(query),
                "INSERT INTO item_tags VALUES ('%s', '%s');",
                item_id, tag->valuestring
            );
            duckdb_query(connection, query, NULL);
        }
    }

    // Insert maps
    cJSON* maps = cJSON_GetObjectItem(item, "maps");
    if (maps) {
        cJSON* map;
        cJSON_ArrayForEach(map, maps) {
            snprintf(query, sizeof(query),
                "INSERT INTO item_maps VALUES ('%s', '%s', %d);",
                item_id, map->string, map->valueint
            );
            duckdb_query(connection, query, NULL);
        }
    }

    // Insert stats
    cJSON* stats = cJSON_GetObjectItem(item, "stats");
    if (stats) {
        cJSON* stat;
        cJSON_ArrayForEach(stat, stats) {
            snprintf(query, sizeof(query),
                "INSERT INTO item_stats VALUES ('%s', '%s', %f);",
                item_id, stat->string, stat->valuedouble
            );
            duckdb_query(connection, query, NULL);
        }
    }
}

int main() {
    // Initialize DuckDB
    duckdb_database db;
    duckdb_connection connection;
   
	const char* db_path = "items.db";

    if (duckdb_open(db_path, &db) != DuckDBSuccess) {
        fprintf(stderr, "Failed to open database\n");
        return 1;
    }
    
    if (duckdb_connect(db, &connection) != DuckDBSuccess) {
        fprintf(stderr, "Failed to connect to database\n");
        duckdb_close(&db);
        return 1;
    }

    // Create tables
    create_tables(connection);

    // Read and parse JSON file
    FILE* fp = fopen("src/items.json", "r");
    if (!fp) {
        fprintf(stderr, "Failed to open JSON file\n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long length = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* json_str = malloc(length + 1);
    fread(json_str, 1, length, fp);
    json_str[length] = '\0';
    fclose(fp);

    // Parse JSON
    cJSON* root = cJSON_Parse(json_str);
    if (!root) {
        fprintf(stderr, "Failed to parse JSON\n");
        free(json_str);
        return 1;
    }

    // Get data object
    cJSON* data = cJSON_GetObjectItem(root, "data");
    
    // Iterate through items
    cJSON* item;
    cJSON_ArrayForEach(item, data) {
        insert_item(connection, item->string, item);
    }

    // Cleanup
    cJSON_Delete(root);
    free(json_str);
    duckdb_disconnect(&connection);
    duckdb_close(&db);

    return 0;
}
