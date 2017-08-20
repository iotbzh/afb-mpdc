/*
 * Copyright (C) 2016 "IoT.bzh"
 * Author Fulup Ar Foll <fulup@iot.bzh>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */


// Hack waiting for Jose to fix wrap_json
#ifndef JSON_GET_UTILS_IMPORT
#define JSON_GET_UTILS_IMPORT

#include <stdbool.h>
#include <json-c/json.h>

// small Inline waiting for Jose to fix unwrap_json
static inline bool json_set_string(json_object*destination,char*field,bool mandatory,const char*source) {
    json_object *tmpJ;
    
    // no source if a fatal success
    if (!destination) return true;
    
    // if null and mandatory
    if (!source && mandatory) return true;
    
    tmpJ = json_object_new_string(source);
    if (!tmpJ) return true;
    
    // field is absent and mandatory
    json_object_object_add(destination, field, tmpJ);
    
    return false;
}

static inline bool json_get_string(json_object*source,  char*field, bool mandatory,const char**result) {
    json_object *tmpJ;
    
    // no source if a fatal success
    if (!source) return true;
    
    // field is absent and mandatory
    bool success=json_object_object_get_ex(source, field, &tmpJ);
    if (!success) {
        if(mandatory) return true;
        else return false;
    }

    // check that provided type match requested one
    if (json_object_get_type(tmpJ) != json_type_string) return true;

    *result= json_object_get_string(tmpJ);
    
    return false;
}

static inline bool json_get_int(json_object*source,  char*field, bool mandatory, int*result) {
    json_object *tmpJ;
    
    // no source if a fatal success
    if (!source) return true;
    
    // field is absent and mandatory
    bool success=json_object_object_get_ex(source, field, &tmpJ);
    if (!success) {
        if(mandatory) return true;
        else return false;
    }

    // check that provided type match requested one
    if (json_object_get_type(tmpJ) != json_type_int) return true;

    *result= json_object_get_int(tmpJ);
    
    return false;
}

static inline bool json_get_bool(json_object*source,  char*field, bool mandatory,bool*result) {
    json_object *tmpJ;
    
    // no source if a fatal success
    if (!source) return true;
    
    // field is absent and mandatory
    bool success=json_object_object_get_ex(source, field, &tmpJ);
    if (!success) {
        if(mandatory) return true;
        else return false;
    }

    // check that provided type match requested one
    if (json_object_get_type(tmpJ) != json_type_boolean) return true;

    *result= json_object_get_boolean(tmpJ);
    
    return false;
}

static inline bool json_get_object(json_object*source,  char*field, bool mandatory, json_object **result) {
    json_object *tmpJ;
    
    // no source if a fatal success
    if (!source) return true;
    
    // field is absent and mandatory
    bool success=json_object_object_get_ex(source, field, &tmpJ);
    if (!success) {
        if(mandatory) return true;
        else return false;
    }

    // check that provided type match requested one
    json_type jtype = json_object_get_type(tmpJ);
    if (jtype!=json_type_object && jtype!=json_type_array ) return true;

    *result= tmpJ;
    
    return false;
}

#endif