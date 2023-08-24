//
// Created by arco on 2023/8/19.
//
#include <stdarg.h>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

enum json_type {
    json_type_null,
    json_type_object,
    json_type_array,
    json_type_string,
    json_type_int
};

typedef struct arco_json{
    enum json_type type;
    int depth;
    int child_num;
    char* key;
    void* value;
    struct arco_json* parent;
    struct arco_json* next;
}arco_json;

int g_json_char_num = 0;
char* g_json_str = NULL;

arco_json* new_json_object()
{
    arco_json* json = malloc(sizeof(arco_json));
    json->type = json_type_object;
    json->depth = 0;
    json->child_num = 0;
    json->key = NULL;
    json->value = NULL;
    json->next = NULL;
}

arco_json* new_json_array()
{
    arco_json* json = malloc(sizeof(arco_json));
    json->type = json_type_array;
    json->depth = 0;
    json->child_num = 0;
    json->key = NULL;
    json->value = NULL;
    json->next = NULL;
}

arco_json* new_json_string(char* value)
{
    // 分配内存
    arco_json* json = malloc(sizeof(arco_json));
    // 设置json类型
    json->type = json_type_string;
    json->depth = 0;
    json->child_num = 0;
    json->next = NULL;
    // 填充kv
    json->key = NULL;
    json->value = (char*) malloc(strlen(value) + 1);
    memcpy(json->value, value, strlen(value) + 1);
    return json;
}

arco_json* new_json_int(int value)
{
    // 分配内存
    arco_json* json = malloc(sizeof(arco_json));
    // 设置json类型
    json->type = json_type_int;
    json->depth = 0;
    json->child_num = 0;
    json->next = NULL;
    // 填充kv
    json->key = NULL;
    json->value = (int*) malloc(sizeof(int));
    *(int*) json->value = value;
    return json;
}

arco_json* new_json_empty()
{
    // 分配内存
    arco_json* json = malloc(sizeof(arco_json));
    // 设置json类型
    json->type = json_type_null;
    json->depth = 0;
    json->child_num = 0;
    json->next = NULL;
    // 填充kv
    json->key = NULL;
    json->value = NULL;
}

int get_json_type(arco_json* json)
{
    if (json != NULL) {
        return json->type;
    }
    else {
        return -1;
    }
}

int json_object_add(arco_json* json, char* key, arco_json* j_add)
{
    if (json->type != json_type_object) {
        printf("json type isn't object, can't add kv\n");
        return -1;
    }
    int i;
    // if cur json has none value
    if (json->value == NULL) {
        json->value = j_add;
        j_add->parent = json;
        j_add->key = malloc(strlen(key) + 1);
        memcpy(j_add->key, key, strlen(key) + 1);
        json->child_num++;
    }
    else {
        arco_json* arco = json->value;
        for (i = 0; i < json->child_num - 1; i++) {
            arco = arco->next;
        }
        j_add->key = malloc(strlen(key) + 1);
        memcpy(j_add->key, key, strlen(key) + 1);
        arco->next = j_add;
        j_add->parent = arco->parent;
        json->child_num++;
    }
    return 0;
}

int json_array_add(arco_json* json, arco_json* j_add)
{
    if (json->type != json_type_array) {
        printf("json type isn't array, can't add object\n");
        return -1;
    }
    int i;
    // if cur json has none value
    if (json->value == NULL) {
        json->value = j_add;
        json->child_num++;
    }
    else {
        arco_json* arco = json->value;
        for (i = 0; i < json->child_num - 1; i++) {
            arco = arco->next;
        }
        arco->next = j_add;
        j_add->parent = arco->parent;
        json->child_num++;
    }
    return 0;
}

typedef void (*deal_callback) (char*, ...);

void json_depth_expand(arco_json* json, int depth, deal_callback callback)
{
    if (get_json_type(json) == json_type_array) {
        if (json->key != NULL && depth > 0)
            callback("\"%s\":", json->key);
        callback("[");
        json_depth_expand(json->value, depth + 1, callback);
    }
    if (get_json_type(json) == json_type_object) {
        if (json->key != NULL && depth > 0)
            callback("\"%s\":", json->key);
        callback("{");
        json_depth_expand(json->value, depth + 1, callback);
    }
    if (json->type == json_type_string) {
        callback("\"%s\":", json->key);
        callback("\"%s\"", (char*) json->value);
        if (json->next != NULL) callback(",");
    }
    if (json->type == json_type_int) {
        callback("\"%s\":", json->key);
        callback("%d", *(int*) json->value);
        if (json->next != NULL) callback(",");
    }

    if (get_json_type(json) == json_type_array) {
        callback("]");
        if (json->next != NULL && depth > 0) callback(",");
    }
    if (get_json_type(json) == json_type_object) {
        callback("}");
        if (json->next != NULL && depth > 0) callback(",");
    }

    // 横向搜索
    if (json->next != NULL && depth > 0) {
        json_depth_expand(json->next, depth, callback);
    }
}

void print_callback(char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char str[64];
    vsprintf(str, fmt, args);
    printf("%s", str);
    va_end(args);
}

void calculate_callback(char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char str[64];
    vsprintf(str, fmt, args);
    g_json_char_num += (int) strlen(str);
    va_end(args);
}

void tostring_callback(char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char str[64];
    vsprintf(str, fmt, args);
    strcat(g_json_str, str);
    va_end(args);
}

void json_depth_expand_print(arco_json* json, int depth)
{
    if (get_json_type(json) == json_type_array) {
        if (json->key != NULL && depth > 0) printf("\"%s\":", json->key);
        printf("[");
        json_depth_expand_print(json->value, depth + 1);
    }
    if (get_json_type(json) == json_type_object) {
        if (json->key != NULL && depth > 0) printf("\"%s\":", json->key);
        printf("{");
        json_depth_expand_print(json->value, depth + 1);
    }
    if (json->type == json_type_string) {
        printf("\"%s\":", json->key);
        printf("\"%s\"", (char*) json->value);
        if (json->next != NULL) printf(",");
    }
    if (json->type == json_type_int) {
        printf("\"%s\":", json->key);
        printf("%d", *(int*) json->value);
        if (json->next != NULL) printf(",");
    }

    if (get_json_type(json) == json_type_array) {
        printf("]");
        if (json->next != NULL && depth > 0) printf(",");
    }
    if (get_json_type(json) == json_type_object) {
        printf("}");
        if (json->next != NULL && depth > 0) printf(",");
    }

    // 横向搜索
    if (json->next != NULL && depth > 0) {
        json_depth_expand_print(json->next, depth);
    }
}

int calculate_json_char_nums(arco_json* json, int depth)
{
    int char_nums = 0;
    if (get_json_type(json) == json_type_array) {
        if (json->key != NULL && depth > 0) {
            char_nums += ((int) strlen(json->key) + 3);
        }
        char_nums++;
        char_nums += calculate_json_char_nums(json->value, depth + 1);
    }
    if (get_json_type(json) == json_type_object) {
        if (json->key != NULL && depth > 0) {
            char_nums += ((int) strlen(json->key) + 3);
        }
        char_nums++;
        char_nums += calculate_json_char_nums(json->value, depth + 1);
    }
    if (json->type == json_type_string) {
        char_nums += ((int) strlen(json->key) + 3);
        char_nums += ((int) strlen(json->value) + 2);
        if (json->next != NULL) char_nums++;
    }
    if (json->type == json_type_int) {
        char_nums += ((int) strlen(json->key) + 3);
        int i, tmp = *(int*) json->value;
        if (tmp < 0) char_nums++;
        for (i = 0; tmp != 0; i++, char_nums++)  tmp /= 10;
        if (json->next != NULL) char_nums++;
    }

    if (get_json_type(json) == json_type_array) {
        char_nums++;
        if (json->next != NULL && depth > 0) char_nums++;
    }
    if (get_json_type(json) == json_type_object) {
        char_nums++;
        if (json->next != NULL && depth > 0) char_nums++;
    }

    // 横向搜索
    if (json->next != NULL && depth > 0) {
        char_nums += calculate_json_char_nums(json->next, depth);
    }
    return char_nums;
}

int calculate_json_str_length(arco_json* json)
{
    g_json_char_num = 0;
    json_depth_expand(json, 0, calculate_callback);
    return g_json_char_num;
}

char* json_to_string(arco_json* json)
{
    int size = calculate_json_str_length(json);
    g_json_str = malloc(size + 1);
    memset(g_json_str, '\0', size + 1);
    json_depth_expand(json, 0, tostring_callback);
    char* json_str = malloc(strlen(g_json_str) + 1);
    memcpy(json_str, g_json_str, strlen(g_json_str) + 1);
    free(g_json_str);
    g_json_str = NULL;
    return json_str;
}

char* str_get_here_to_there(char* str, int position, char c)
{
    int i, size = 1;
    char* dst = NULL;
    for (i = position; i < strlen(str); i++) {
        if (str[i] != c) size++;
        else break;
    }
    dst = malloc(sizeof(char) * size);
    for (i = position; i < strlen(str); i++) {
        if (str[i] != c) dst[i - position] = str[i];
        else {
            dst[i - position] = '\0';
            return dst;
        }
    }
    return NULL;
}

int string_to_json(char* str)
{
    int i, str_len = (int) strlen(str);
    int key_flag = 0, colon_flag = 0;
    char cur_key[64];
    arco_json* json = NULL;

    if (str[0] == '{') {
        json = new_json_object();
    }

    arco_json* p_json = json;
    for (i = 0; i < str_len; i++) {

        if (str[i] == '"') {
            if (++key_flag == 2) {
                key_flag = 0;
            }
            else {
                if (colon_flag == 0) {
                    p_json->key = str_get_here_to_there(str, i, '"');
                }
                else {
                    p_json->value = str_get_here_to_there(str, i, '"');
                }
            }
        }

        if (str[i] == ':') {
            colon_flag = 1;
        }

        if (str[i] == '{') {

        }

        if (str[i] == ',') {

        }

    }
}



// TODO test arcojson use
int main()
{
    arco_json* json_0 = new_json_object();
    arco_json* json_11 = new_json_object();
    arco_json* json_12 = new_json_object();
    arco_json* json_13 = new_json_object();
    arco_json* json_14 = new_json_array();

    json_object_add(json_11, "key11", new_json_string("value11"));
    json_object_add(json_11, "key12", new_json_string("value12"));

    json_object_add(json_12, "key21", new_json_string("value21"));
    json_object_add(json_12, "key22", new_json_int(100));
    json_object_add(json_12, "key23", new_json_int(-1));

    json_object_add(json_13, "key31", new_json_string("value31"));

    json_object_add(json_0, "key1", json_11);
    json_array_add(json_14, json_12);
    json_array_add(json_14, json_13);
    json_object_add(json_0, "key4", json_14);

    printf("%s\n", json_to_string(json_11));
    printf("%s\n", json_to_string(json_12));
    printf("%s\n", json_to_string(json_13));
    printf("%s\n", json_to_string(json_14));
    printf("%s\n", json_to_string(json_0));
}
