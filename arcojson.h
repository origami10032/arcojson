enum json_type{
    json_type_empty,
    json_type_object,
    json_type_array,
    json_type_string,
    json_type_long
};

typedef struct arco_json{
    enum json_type type;
    int child_num;
    int seq;
    char* key;
    void* value;
    struct arco_json* parent;
    struct arco_json* next;
}arco_json;


arco_json* new_json_object();
arco_json* new_json_array();
arco_json* new_json_string(char* value);
arco_json* new_json_long(long value);
int get_json_type(arco_json* json);
int json_object_add(arco_json* json, char* key, arco_json* j_add);
int json_array_add(arco_json* json, arco_json* j_add);
int calculate_json_str_length(arco_json* json);
char* json_to_string(arco_json* json);
arco_json* string_to_json(char* str);

