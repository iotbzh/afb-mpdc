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
 * References:
 *  Utilities rebuilt from Media Daemon Command Line Interface
 *  https://github.com/MusicPlayerDaemon/libmpdclient
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "libclient-utils.h"

enum mpd_tag_type SearchTypeTag(const char *name) {

    if (strcasecmp(name, "any") == 0) return SEARCH_TAG_ANY;
    if (strcasecmp(name, "filename") == 0) return SEARCH_TAG_URI;
    if (strcasecmp(name, "base") == 0) return SEARCH_TAG_BASE;

    enum mpd_tag_type type = mpd_tag_name_iparse(name);
    if (type != MPD_TAG_UNKNOWN) return type;

    AFB_ERROR("MDCP:SEARCH:TYPE '%s' is not a valid search type", name);
    return MPD_TAG_UNKNOWN;
}

bool SearchAddOneConstraint(afb_req request, mpdcHandleT *mpdcHandle, json_object *contraintJ) {

    const char *type, *content;
    mpdConnectT *conn= mpdcHandle->mpd;
            
    int err = wrap_json_unpack(contraintJ, "{s:s, s:s !}", "type", &type, "content", &content);
    if (err) {
        afb_req_fail_f(request, "SearchAddOneConstraint", "Invalid Search Filter =%s", json_object_get_string(contraintJ));
        return true;
    }

    enum mpd_tag_type tag = SearchTypeTag(type);

    if (tag == (enum mpd_tag_type)SEARCH_TAG_ANY)
        mpd_search_add_any_tag_constraint(conn, MPD_OPERATOR_DEFAULT, charset_to_utf8(content));
    else if (tag == (enum mpd_tag_type)SEARCH_TAG_URI)
        mpd_search_add_uri_constraint(conn, MPD_OPERATOR_DEFAULT, charset_to_utf8(content));
    else if (tag == (enum mpd_tag_type)SEARCH_TAG_BASE)
        mpd_search_add_base_constraint(conn, MPD_OPERATOR_DEFAULT, charset_to_utf8(content));
    else
        mpd_search_add_tag_constraint(conn, MPD_OPERATOR_DEFAULT, tag, charset_to_utf8(content));
    return false;
}

bool SearchAddConstraints(afb_req request,mpdcHandleT *mpdcHandle, json_object *constraintsJ) {

    // make sure constrains is a valid array
    if (json_object_get_type(constraintsJ) != json_type_array) {
        int err = SearchAddOneConstraint(request, mpdcHandle, constraintsJ);
        if (err) return true;
        
    } else
        for (int idx = 0; idx < json_object_array_length(constraintsJ); idx++) {
        int err = SearchAddOneConstraint(request, mpdcHandle, json_object_array_get_idx(constraintsJ, idx));
        if (err) return true;
    }

    // free object
    json_object_put(constraintsJ);
    return false;
}
