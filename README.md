# arcojson
一个简单的c语言json库  

仅1个源文件,四百多行源码  

支持对象、数组、数值、字符串类型  

源文件:arcojson.c  

头文件:arcojson.h  

示例代码:example.c  

# 使用说明
## 创建json object
arco_json* json = new_json_object();

json_object_add(json, "key10", new_json_string("value10"));

会创建一个{"key0":"value0"}的json

## 解析json
示例json {"key10":"value10","key11":1234567}

### 获取字符串值
char* get_str = get_string_from_object(get_obj_by_key, "key10");

### 获取数值
long get_long = get_long_from_object(get_obj_by_key, "key11");
