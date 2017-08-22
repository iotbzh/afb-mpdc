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

#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/song.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "libclient-utils.h"


// should be more than enough
#ifndef MAX_CONFIGURE_OUTPUT
#define MAX_CONFIGURE_OUTPUT 20
#endif

#include <json-setget.h>

typedef struct {
    const char* name;
    unsigned id;
    bool enable;
} outputElemT;

STATIC bool OutputOneSetGet(afb_req request, mpdConnectT *mpdConnect, bool only, outputElemT *outputs, json_object *targetJ) {
    const char *name=NULL;
    int id=0, enable=true, all=false;

    int err= wrap_json_unpack(targetJ, "{s?b,s?s,s?i,s?b !}",  "enable", &enable, "name", &name, "id", &id, "all", &all);
    if (err) {
        afb_req_fail_f(request, "MPDC:OutputOneSetGet", "Invalid Ouput target='%s'", json_object_get_string(targetJ));
        goto OnErrorExit;
    }
    
    if (!name && !id && !all) {
        afb_req_fail_f(request, "MPDC:OutputOneSetGet", "Requirer either 'name'|'id'|'all' target='%s'", json_object_get_string(targetJ));
        goto OnErrorExit;        
    }

    // search for output within config
    for (int idx = 0; outputs[idx].id!=0; idx++) {

        // if output match then let's update output status
        if (id == outputs[idx].id || (name && !strcasecmp(name, outputs[idx].name)) || all==true) {
            if (enable) {
                mpd_send_enable_output(mpdConnect, outputs[idx].id - 1);
                outputs[idx].enable=true;
            } else {
                mpd_send_disable_output(mpdConnect, outputs[idx].id - 1);
                outputs[idx].enable= false;
            }
        } else if (only) {
            if (!enable) {
                mpd_send_enable_output(mpdConnect, outputs[idx].id - 1);
                outputs[idx].enable=true;
            } else {
                mpd_send_disable_output(mpdConnect, outputs[idx].id - 1);
                outputs[idx].enable= false;
            }
        }
    }
    
    return false;
    
OnErrorExit:
    return true;
}

PUBLIC json_object* OutputSetGet(afb_req request, mpdcHandleT *mpdHandle, bool list, bool only, json_object *targetsJ) {
    unsigned count;
    json_object*responseJ=NULL;
    mpdConnectT *mpdConnect = mpdHandle->mpd;
    
    // list request
    if (list) responseJ = json_object_new_array();
    
    // send output command to MPD
    mpd_send_outputs(mpdConnect);
    
    // allow on a stack a space to keep output config
    outputElemT* outputs= alloca(sizeof(outputElemT) * MAX_CONFIGURE_OUTPUT);

    // build the list of configure output
    struct mpd_output *output;
    for (count=0;(output = mpd_recv_output(mpdConnect)) != NULL; count++) {
        
        /* We increment by 1 to make it natural to the user  */
        outputs[count].name= strdup(mpd_output_get_name(output));
        outputs[count].id  = mpd_output_get_id(output)+1;
        outputs[count].enable=mpd_output_get_enabled(output);
        
        if (count == MAX_CONFIGURE_OUTPUT) {
            afb_req_fail_f (request, "MPDC:SetGetOutput", "Too many output configure MAX_CONFIGURE_OUTPUT=%d", MAX_CONFIGURE_OUTPUT);
            goto OnErrorExit;                    
        }
        
        mpd_output_free(output);
    }
    // close output array
    outputs[count].id=0;
    outputs[count].name=NULL;

   
    if (targetsJ != NULL) {  // if targets defined apply control 
        
        // Start packing command
        if (!mpd_command_list_begin(mpdConnect, false)) {
            afb_req_fail(request, "MPDC:SetGetOutput", "Fail to start command list");
            goto OnErrorExit;
        }

        if (json_object_get_type(targetsJ) != json_type_array) {
            int error = OutputOneSetGet(request, mpdConnect, only, outputs, targetsJ);
            if (error) goto OnErrorExit;
        } else {

            // apply output targetsJ
            for (int idx = 0; idx < json_object_array_length(targetsJ); idx++) {

                int error = OutputOneSetGet(request, mpdConnect, only, outputs, json_object_array_get_idx(targetsJ, idx));
                if (error) goto OnErrorExit;
            }

        }
        
        // End command packing
        if (!mpd_command_list_end(mpdConnect)) {
            miscPostError(request, "MPDC:SetGetOutput", mpdHandle);
            goto OnErrorExit;
        }
    }
       
    // We need to wait for target control to be done before creating the update output list
    for (int idx = 0; outputs[idx].id!=0; idx++) {
        json_object *outputJ;
        if (list) {
            wrap_json_pack(&outputJ, "{ss,si,sb}", "name", outputs[idx].name, "id", outputs[idx].id, "enable", outputs[idx].enable);
            json_object_array_add(responseJ, outputJ);
        }
        free((void*)outputs[idx].name);
    }

    return responseJ;
    
OnErrorExit:
            return NULL;
}