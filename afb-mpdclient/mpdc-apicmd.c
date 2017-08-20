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
 *  https://www.musicpd.org/doc/libmpdclient/files.html
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

#include "mpdc-binding.h"
#include "json-setget.h"

        
// Jose this is really scrap !!!
static afb_req NULL_AFBREQ = {};

static MpdcHandleT *mpdcHandle;

PUBLIC void mpdcapi_ping(struct afb_req request) {
    json_object *query = afb_req_json(request);
    afb_req_success(request, query, NULL);
}


PUBLIC void mpdcapi_search(afb_req request) {
    const char *display=NULL;
    bool exact=false, add=false;
    json_object *targetJ=NULL;
    json_object *queryJ=afb_req_json(request);
    
    //  Assert Connection to MPD
    if (mpdcFailConnect(mpdcHandle, request)) goto OnErrorExit;

    //int err= wrap_json_unpack (queryJ, "{s:s,s?i,s?i,s?o !}"
    //    , "display", &display, "exact", &exact, "add", &add, "target", &targetJ);
    int err=0;
    err+= json_get_string (queryJ, "display", true,  &display);
    err+= json_get_bool   (queryJ, "exact"  , false, &exact);
    err+= json_get_bool   (queryJ, "add"    , false, &add);
    err+= json_get_object (queryJ, "target" , false, &targetJ);
    if (err) {
        afb_req_fail_f (request, "MDCP:Search","Search 'display' field not found in '%s'", json_object_get_string(queryJ));
        goto OnErrorExit;
    }
    
    enum mpd_tag_type tag= SearchTypeTag(display);
    if (tag == MPD_TAG_UNKNOWN) {
        afb_req_fail_f (request, "MDCP:Search","music Unknown type=%s", display);
        goto OnErrorExit;
    }
     
    // if query present then search for song otherwise search for tag
    if (!targetJ) 
        mpd_search_db_tags(mpdcHandle->mpd, tag);                
    else if (!add) {
        mpd_search_db_songs(mpdcHandle->mpd, exact);
        SearchAddConstraints (request, mpdcHandle->mpd, targetJ);
    } else {
        mpd_search_add_db_songs(mpdcHandle->mpd, exact);
        SearchAddConstraints (request, mpdcHandle->mpd, targetJ);        
    }

    if (!mpd_search_commit(mpdcHandle->mpd)) {
        miscPostError(request, "MDCP:Search", mpdcHandle->mpd);
        goto OnErrorExit;
    }

    // create response and clean double
    json_object *listJ= json_object_new_array();
    struct mpd_pair *pair;
    json_object *cleanupJ=json_object_new_object(), *tmpJ, *dummyJ= json_object_new_boolean(true);
    while ((pair = mpd_recv_pair_tag(mpdcHandle->mpd, tag)) != NULL) {
        const char*value=pair->value;
        if (!json_object_object_get_ex(cleanupJ,value,&tmpJ)) {
            json_object_array_add(listJ, json_object_new_string(charset_from_utf8(value)));
            json_object_object_add(cleanupJ,value, dummyJ);
        }
        mpd_return_pair(mpdcHandle->mpd, pair);
    }

    // cleanup object
    json_object_put(cleanupJ);
    json_object_put(dummyJ);
    
    // return status
    afb_req_success(request, listJ, NULL);
    
OnErrorExit:
    mpd_response_finish(mpdcHandle->mpd);
    return;    
}


// return Player Daemon Status
PUBLIC void mpdcapi_play(afb_req request) {
    json_object *responseJ=NULL;
    int done;
    
    //  Assert Connection to MPD
    if (mpdcFailConnect(mpdcHandle, request)) goto OnErrorExit;

    // retrieve optional song index
    json_object *queryJ=afb_req_json(request); 
    int index=0; 
    bool current=0;
    wrap_json_unpack(queryJ, "{s?i, s?b s?b!}", "index", &index, "current", &current);
    
    if (current) {
        struct mpd_song *song;
        
        // try to run current song
        song = mpd_run_current_song(mpdcHandle->mpd);
        if (song == NULL) {
            afb_req_fail(request, "MDCP:CtlPlayCurrentSong", "No Current Song Selected"); 
            goto OnErrorExit;
        }
        responseJ=CtlPlayCurrentSong(song); 
        if (!responseJ) goto OnErrorExit;
    }
    else if (index)
        done=mpd_run_play_pos(mpdcHandle->mpd, index);
    else
        done=mpd_run_play(mpdcHandle->mpd);
    
    if(!done) {
        miscPostError(request, "MPDC:Play Fail to Process Request", mpdcHandle->mpd);
        goto OnErrorExit;
    }
    
    // return status
    afb_req_success(request, responseJ, NULL);
        
OnErrorExit:
    mpd_response_finish(mpdcHandle->mpd);
    return;
}

// return Player Daemon Status
PUBLIC void mpdcapi_status(afb_req request) {
    
    //  Assert Connection to MPD
    if (mpdcFailConnect(mpdcHandle, request)) goto OnErrorExit;

    json_object *statusJ = StatusGetAll (request, mpdcHandle->mpd);
    if (!statusJ) goto OnErrorExit;
    
    // return status
    afb_req_success(request, statusJ, NULL);    
    
OnErrorExit:
    mpd_response_finish(mpdcHandle->mpd);
    return; 
}

// return list of configured output
PUBLIC void mpdcapi_output(afb_req request) {
    json_object *responseJ=NULL;
    json_object *targetsJ=NULL, *queryJ=afb_req_json(request); 
    bool only=false, list=true;

    //  Assert Connection to MPD
    if (mpdcFailConnect(mpdcHandle, request)) goto OnErrorExit;

    bool error=wrap_json_unpack(queryJ, "{s?b,s?b,s?o !}"
        ,"list",&list, "only", &only, "target", &targetsJ);

    // for unknown reason queryJ=="null" when query is empty (José ???)
    if(error && (queryJ!=NULL && json_object_get_type(queryJ) == json_type_object)) {
       afb_req_fail_f(request, "MDCP:Output", "Invalid query input '%s'", json_object_get_string(queryJ)); 
       goto OnErrorExit;                
    }
       
    // get response, send response and cleanup connection
    responseJ=OutputSetGet(request, mpdcHandle->mpd, list, only, targetsJ);
    if (responseJ) afb_req_success(request, responseJ, NULL);
        
OnErrorExit:
    mpd_response_finish(mpdcHandle->mpd);
    return;
}


PUBLIC void mpdcapi_version(afb_req request) {
    json_object *responseJ=NULL;
    const unsigned* version;
    
    //  Assert Connection to MPD
    if (mpdcFailConnect(mpdcHandle, request)) goto OnErrorExit;

    // get MPD version
    version=mpd_connection_get_server_version(mpdcHandle->mpd);
    int major=version[0]; 
    int minor=version[1];
    int patch=version[2];
    
    // return as a json_object
    wrap_json_pack(&responseJ, "{si,si,si}", "major", major, "minor",minor, "patch",patch);
    
    afb_req_success(request, responseJ, NULL);

OnErrorExit:
    mpd_response_finish(mpdcHandle->mpd);
    return;
}


PUBLIC void mpdcapi_control(afb_req request) {
    int error, count=0;
    bool flag;
    int value;
    
    //  Assert Connection to MPD
    if (mpdcFailConnect(mpdcHandle, request)) goto OnErrorExit;

    json_object *queryJ=afb_req_json(request); 

    if (!json_get_bool(queryJ, "pause", true, &flag)) {
       error =!mpd_send_pause(mpdcHandle->mpd, true);
       count++;
       goto OnDoneExit;
    }
    
    if (!json_get_bool(queryJ, "resume", true, &flag)) {
       error = !mpd_run_play(mpdcHandle->mpd);
       count++;
       goto OnDoneExit;
    }
    
    if (!json_get_bool(queryJ, "toggle", true, &flag)) {
        struct mpd_status *status = StatusRun(request, mpdcHandle->mpd);
        count++;
        
 	if (mpd_status_get_state(status) == MPD_STATE_PLAY) {
            error = !mpd_send_pause(mpdcHandle->mpd, true);
	} else {
            error = !mpd_run_play(mpdcHandle->mpd);
	}
        goto OnDoneExit;
    }
    
    if (!json_get_int(queryJ, "play", true, &value)) {
        error = !mpd_run_play_pos(mpdcHandle->mpd, value);
        count++;
        goto OnDoneExit;
    }
    
    if (!json_get_int(queryJ, "prev", true, &value)) {
        error = !mpd_run_previous(mpdcHandle->mpd);
        count++;
        goto OnDoneExit;
    }
    
    if (!json_get_int(queryJ, "next", true, &value)) {
        error = !mpd_run_next(mpdcHandle->mpd);
        count++;
        goto OnDoneExit;
    }
   
OnDoneExit:
    if (error || count==0) 
        afb_req_fail (request, "MPDC:Control", "Requested Control Fail (no control?)");
    else     
        afb_req_success(request, NULL, NULL);
    
OnErrorExit:
    mpd_response_finish(mpdcHandle->mpd);
   return;    
}


// In Most case Connection to MPD should be automatic
PUBLIC void mpdcapi_connect(afb_req request) {
    
    //  Assert Connection to MPD
    if (mpdcFailConnect(mpdcHandle, request)) goto OnErrorExit;

    afb_req_success(request, NULL, NULL);
    
OnErrorExit:
    mpd_response_finish(mpdcHandle->mpd);
    return;    
}

// Create a private connection for synchronous commands
PUBLIC bool mpdcapi_init(const char *bindername) {

    mpdcHandle = (MpdcHandleT*)calloc (1, sizeof(MpdcHandleT));
    mpdcHandle->label="CmdChannel";
    bool error=mpdcFailConnect(mpdcHandle, NULL_AFBREQ);
    
    return error;
}