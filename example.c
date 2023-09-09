//
// Created by arco on 2023/9/3.
//
#include <string.h>
#include "stdio.h"
#include "arcojson.h"

/**
 * test arco json usage
 */
void create_json_object_test()
{
    arco_json* json = new_json_object();
    json_object_add(json, "key0", new_json_string("value0"));
    arco_json* json1 = new_json_object();
    json_object_add(json1, "key1.0", new_json_string("value1.0"));
    json_object_add(json, "key1", json1);
    arco_json* json2 = new_json_object();
    arco_json* json20 = new_json_object();
    json_object_add(json20, "key2.0.1", new_json_string("value2.0.1"));
    json_object_add(json2, "key2.0", json20);
    json_object_add(json, "key2", json2);

    printf("create_json_obj:%s\n", json_to_string(json));
}

void create_json_object_test_long()
{
    arco_json* json = new_json_object();
    json_object_add(json, "key0", new_json_long(100));
    arco_json* json1 = new_json_object();
    json_object_add(json1, "key1.0", new_json_long(-1));
    json_object_add(json, "key1", json1);
    arco_json* json2 = new_json_object();
    arco_json* json20 = new_json_object();
    json_object_add(json20, "key2.0.1", new_json_long(-1234567));
    json_object_add(json20, "key2.0.2", new_json_string("value2.0.2"));
    json_object_add(json2, "key2.0", json20);
    json_object_add(json, "key2", json2);

    printf("create_json_obj_num:%s\n", json_to_string(json));
}

void create_json_array_test()
{
    arco_json* json = new_json_array();
    arco_json* json0 = new_json_object();
    json_object_add(json0, "key0", new_json_string("value0"));
    arco_json* json1 = new_json_object();
    json_object_add(json1, "key1", new_json_string("value1"));
    arco_json* json2 = new_json_object();
    arco_json* json20 = new_json_object();
    json_object_add(json20, "key2.0", new_json_string("value2.0"));
    json_object_add(json2, "key2", json20);
    json_array_add(json, json0);
    json_array_add(json, json1);
    json_array_add(json, json2);
    printf("create_json_arr:%s\n", json_to_string(json));
}


void create_json_mixed_test()
{
    arco_json* json = new_json_object();
    arco_json* j_o0 = new_json_object();
    json_object_add(j_o0, "ok0", new_json_string("oval0"));

    arco_json* j_a1 = new_json_array();
    arco_json* j_o10 = new_json_object();
    json_object_add(j_o10, "ok10", new_json_string("oval10"));
    arco_json* j_o11 = new_json_object();
    json_object_add(j_o11, "ok11", new_json_string("oval11"));
    json_object_add(j_o11, "ok12", new_json_string("oval12"));
    json_array_add(j_a1, j_o10);
    json_array_add(j_a1, j_o11);

    arco_json* j_o2 = new_json_object();
    arco_json* j_o20 = new_json_object();
    json_object_add(j_o20, "ok20", new_json_string("oval20"));
    json_object_add(j_o20, "ok21", new_json_string("oval21"));
    json_object_add(j_o20, "ok22", new_json_string("oval22"));
    json_object_add(j_o2, "ok2", j_o20);

    json_object_add(json, "obj_1", j_o0);
    json_object_add(json, "arr_1", j_a1);
    json_object_add(json, "obj_2", j_o2);

    printf("create_json_mix:%s\n", json_to_string(json));
}


void create_json_null_test()
{
    arco_json* json_o = new_json_object();
    arco_json* json_a = new_json_array();
    printf("create_json_nul:%s  %s\n", json_to_string(json_o), json_to_string(json_a));
}

void str_to_json_object_test()
{
    char str[] = "{\"key0\":\"value0\",\"key1\":{\"key1.0\":\"value1.0\"},\"key2\":{\"key2.0\":{\"key2.0.1\":\"value2.0.1\"}}}";
    arco_json* json = string_to_json(str);
    printf("str_to_json_obj:%s\n", json_to_string(json));
}

void str_to_json_object_test_long()
{
    char str[] = "{\"key0\":100,\"key1\":{\"key1.0\":-1},\"key2\":{\"key2.0\":{\"key2.0.1\":-1234567,\"key2.0.2\":\"value2.0.2\"}}}";
    arco_json* json = string_to_json(str);
    printf("str_to_json_obj_num:%s\n", json_to_string(json));
}

void str_to_json_array_test()
{
    char str[] = "[{\"key0\":\"value0\"},{\"key1\":\"value1\"},{\"key2\":{\"key2.0\":\"value2.0\"}}]";
    arco_json* json = string_to_json(str);
    printf("str_to_json_arr:%s\n", json_to_string(json));
}

void str_to_json_mixed_test()
{
    char str[] = "{\"obj_1\":{\"ok0\":\"oval0\"},\"arr_1\":[{\"ok10\":\"oval10\"},{\"ok11\":\"oval11\",\"ok12\":\"oval12\"}],\"obj_2\":{\"ok2\":{\"ok20\":\"oval20\",\"ok21\":\"oval21\",\"ok22\":\"oval22\"}}}";
    arco_json* json = string_to_json(str);
    printf("str_to_json_mix:%s\n", json_to_string(json));
}

void str_to_json_null_test()
{
    char str[] = "{}";
    arco_json* json = string_to_json(str);
    char str2[] = "[]";
    arco_json* json2 = string_to_json(str2);
    printf("str_to_json_null:%s  %s\n", json_to_string(json), json_to_string(json2));
}

void json_depth_expand_print(arco_json* json, int depth)
{
//    printf("depth=%d\n", depth);
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
    if (json->type == json_type_long) {
        printf("\"%s\":", json->key);
        printf("%d", *(int*) json->value);
        if (json->next != NULL) printf(",");
    }
    if (json->type == json_type_empty) {
        printf("tmd empty\n");
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

void get_json_value_test()
{
    arco_json* json = new_json_array();
    arco_json* json0 = new_json_object();
    json_object_add(json0, "key00", new_json_long(123));
    json_object_add(json0, "key01", new_json_string("value01"));
    json_object_add(json0, "key02", new_json_string("value02"));
    arco_json* json1 = new_json_object();
    arco_json* json10 = new_json_object();
    json_object_add(json10, "key10", new_json_string("value10"));
    json_object_add(json10, "key11", new_json_long(1234567));
    json_object_add(json1, "key1", json10);
    json_array_add(json, json0);
    json_array_add(json, json1);

    printf("get_json_value_test:%s\n", json_to_string(json));

    arco_json* get_obj_by_idx = get_object_from_array(json, 1);
    printf("get_obj_by_idx:%s\n", json_to_string(get_obj_by_idx));

    arco_json* get_obj_by_key = get_object_from_object(get_obj_by_idx, "key1");
    printf("get_obj_by_key:%s\n", json_to_string(get_obj_by_key));

    char* get_str = get_string_from_object(get_obj_by_key, "key10");
    printf("get_str_value:%s\n", get_str);

    long get_long = get_long_from_object(get_obj_by_key, "key11");
    printf("get_str_value:%ld\n", get_long);

}

int main()
{
    // 创建json对象示例
    create_json_object_test();
    str_to_json_object_test();
    printf("\n");
    // 创建带数值的json对象示例
    create_json_object_test_long();
    str_to_json_object_test_long();
    printf("\n");
    // 创建json数组示例
    create_json_array_test();
    str_to_json_array_test();
    printf("\n");
    // 对象和数组混合示例
    create_json_mixed_test();
    str_to_json_mixed_test();
    printf("\n");
    // null情况示例
    create_json_null_test();
    str_to_json_null_test();
    printf("\n");
    // json对象获取值示例(数组 对象 字符串 数值
    get_json_value_test();

    return 0;
}

