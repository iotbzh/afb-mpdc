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
static mpdcHandleT *mpdcLocalHandle;

// If no valid session return default mpcdHandle
STATIC mpdcHandleT* GetSessionHandle(json_object *queryJ) {
    mpdcHandleT *mpdcHandle;
    json_object *sessionJ;

    // if no session let's try default localhost one
    int done =json_object_object_get_ex(queryJ,"session", &sessionJ);
    if (!done) return mpdcLocalHandle;

    // extract session pointer and check for magicnumber
    const char* session= json_object_get_string(sessionJ);
    sscanf (session, "%p", &mpdcHandle);

    if (mpdcHandle->magic != MPDC_SESSION_MAGIC) {
      AFB_ERROR ("MPDC:GetSessionHandle (Hoops!!!) Invalid Session Magic");
      return NULL;
    }

    // everything looks fine
    return mpdcHandle;
}



STATIC void mpdcFlushConnect(mpdcHandleT *mpdcHandle) {

    if (mpdcHandle->mpd) mpd_response_finish(mpdcHandle->mpd);
}


PUBLIC void mpdcapi_ping(struct afb_req request) {
    json_object *query = afb_req_json(request);
    afb_req_success(request, query, NULL);
}


PUBLIC void mpdcapi_search(afb_req request) {
    const char *display=NULL;
    int exact=false, add=false;
    json_object *targetJ=NULL;
    json_object *queryJ=afb_req_json(request);

    // Retrieve mpdcHandle from session and assert connection
    mpdcHandleT *mpdcHandle=GetSessionHandle(queryJ);
    if (mpdcIfConnectFail(MPDC_CHANNEL_CMD, mpdcHandle, request)) goto OnErrorExit;

    //int err= wrap_json_unpack (queryJ, "{s:s,s?i,s?i,s?o !}"
    //    , "display", &display, "exact", &exact, "add", &add, "target", &targetJ);
    int err=0;
    err+= json_get_string (queryJ, "display", true,  &display);
    err+= json_get_int   (queryJ, "exact"  , false, &exact);
    err+= json_get_int   (queryJ, "add"    , false, &add);
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
        SearchAddConstraints (request, mpdcHandle, targetJ);
    } else {
        mpd_search_add_db_songs(mpdcHandle->mpd, exact);
        SearchAddConstraints (request, mpdcHandle, targetJ);
    }

    if (!mpd_search_commit(mpdcHandle->mpd)) {
        miscPostError(request, "MDCP:Search", mpdcHandle);
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
    mpdcFlushConnect(mpdcHandle);
    return;
}


// return Player Daemon Status
/* XXX - cleanup : replace by mpdapi_control type play
PUBLIC void mpdcapi_play(afb_req request) {
    json_object  *responseJ=NULL;
    int done;

    // Retrieve mpdcHandle from session and assert connection
    json_object *queryJ=afb_req_json(request);
    mpdcHandleT *mpdcHandle=GetSessionHandle(queryJ);
    if (mpdcIfConnectFail(MPDC_CHANNEL_CMD, mpdcHandle, request)) goto OnErrorExit;

    // retrieve optional song index
    int index=0;
    int current=0;
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
        miscPostError(request, "MPDC:Play Fail to Process Request", mpdcHandle);
        goto OnErrorExit;
    }

    // return status
    afb_req_success(request, responseJ, NULL);

OnErrorExit:
    mpdcFlushConnect(mpdcHandle);
    return;
}
*/

// return Player Daemon Status
PUBLIC void mpdcapi_status(afb_req request) {

    // Retrieve mpdcHandle from session and assert connection
    json_object *queryJ=afb_req_json(request);
    mpdcHandleT *mpdcHandle=GetSessionHandle(queryJ);
    if (mpdcIfConnectFail(MPDC_CHANNEL_CMD, mpdcHandle, request)) goto OnErrorExit;

    json_object *statusJ = StatusGetAll (request, mpdcHandle);
    if (!statusJ) goto OnErrorExit;

    // return status
    afb_req_success(request, statusJ, NULL);
    mpdcFlushConnect(mpdcHandle);

OnErrorExit:
    mpdcFlushConnect(mpdcHandle);
    return;
}

// Provide playlist Management
PUBLIC void mpdcapi_playlist(afb_req request) {
    json_object *responseJ=NULL;
    int error;

    // Retrieve mpdcHandle from session and assert connection
    json_object *queryJ=afb_req_json(request);
    mpdcHandleT *mpdcHandle=GetSessionHandle(queryJ);
    if (mpdcIfConnectFail(MPDC_CHANNEL_CMD, mpdcHandle, request)) goto OnErrorExit;

    // unpack json query object
    int current=false, clear=false, shuffle=false;
    char *name=NULL, *save=NULL, *load=NULL;
    json_object *moveJ=NULL;
    error=wrap_json_unpack(queryJ, "{s?b,s?b,s?b,s?s,s?s,s?s,s?o}"
        , "current", &current, "clear",&clear, "shuffle",&shuffle, "name",&name, "save",&save, "load",&load, "move",&moveJ);

    if (error) {
        afb_req_fail_f (request, "MPDC:playlist", "Command Syntax Error query=%s", json_object_get_string(queryJ));
        goto OnErrorExit;
    }

    if (shuffle) {
        error = !mpd_run_play(mpdcHandle->mpd);
        if (error) goto OnErrorExit;
    }

    if (name || current) {
        if (current) name=NULL; // current has precedence on name
        responseJ= ListPlayList (request, mpdcHandle, name);
        if (!responseJ) goto OnErrorExit;
    }

    // No grammatical control but at least clean will return an empty list
    if (clear) {
        error = !mpd_run_play(mpdcHandle->mpd);
        if (error) goto OnErrorExit;
    }

    afb_req_success(request, responseJ, NULL);
    mpdcFlushConnect(mpdcHandle);

OnErrorExit:
    mpdcFlushConnect(mpdcHandle);
    return;
}

// Provide playlist Management
PUBLIC void mpdcapi_listsong(afb_req request) {
    json_object *responseJ=NULL;

    // Retrieve mpdcHandle from session and assert connection
    json_object *queryJ=afb_req_json(request);
    mpdcHandleT *mpdcHandle=GetSessionHandle(queryJ);
    if (mpdcIfConnectFail(MPDC_CHANNEL_CMD, mpdcHandle, request)) goto OnErrorExit;

    responseJ = ListDirSong(request, mpdcHandle, queryJ);
    if (!responseJ) goto OnErrorExit;

    afb_req_success(request, responseJ, NULL);
    mpdcFlushConnect(mpdcHandle);
    return ;

OnErrorExit:
    afb_req_fail (request, "MPDC:Control", "Requested Control Fail (no control?)");
    mpdcFlushConnect(mpdcHandle);
    return;
}


// return list of configured output
PUBLIC void mpdcapi_output(afb_req request) {
    json_object *responseJ=NULL, *targetsJ=NULL;
    int only=false, list=true;

    // Retrieve mpdcHandle from session and assert connection
    // Retrieve mpdcHandle from session and assert connection
    json_object *queryJ=afb_req_json(request);
    mpdcHandleT *mpdcHandle=GetSessionHandle(queryJ);
    if (mpdcIfConnectFail(MPDC_CHANNEL_CMD, mpdcHandle, request)) goto OnErrorExit;

    int error=wrap_json_unpack(queryJ, "{s?b,s?b,s?o}"
        ,"list",&list, "only", &only, "target", &targetsJ);

    // for unknown reason queryJ=="null" when query is empty (José ???)
    if(error && (queryJ!=NULL && json_object_get_type(queryJ) == json_type_object)) {
       afb_req_fail_f(request, "MDCP:Output", "Invalid query input '%s'", json_object_get_string(queryJ));
       goto OnErrorExit;
    }

    // get response, send response and cleanup connection
    responseJ=OutputSetGet(request, mpdcHandle, list, only, targetsJ);
    if (responseJ) afb_req_success(request, responseJ, NULL);

OnErrorExit:
    mpdcFlushConnect(mpdcHandle);
    return;
}

PUBLIC void mpdcapi_control(afb_req request) {
    int error, count=0;
    int flag;
    int value;

    // Retrieve mpdcHandle from session and assert connection
    // Retrieve mpdcHandle from session and assert connection
    json_object *queryJ=afb_req_json(request);
    mpdcHandleT *mpdcHandle=GetSessionHandle(queryJ);
    if (mpdcIfConnectFail(MPDC_CHANNEL_CMD, mpdcHandle, request)) goto OnErrorExit;

    if (!json_get_int(queryJ, "pause", true, &flag)) {
       error =!mpd_send_pause(mpdcHandle->mpd, true);
       count++;
       goto OnDoneExit;
    }

    if (!json_get_int(queryJ, "resume", true, &flag)) {
       error = !mpd_run_play(mpdcHandle->mpd);
       count++;
       goto OnDoneExit;
    }

    if (!json_get_int(queryJ, "toggle", true, &flag)) {
        mpdStatusT *status = StatusRun(request, mpdcHandle);
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
    mpdcFlushConnect(mpdcHandle);
   return;
}


PUBLIC void mpdcapi_version(afb_req request) {

    // Retrieve mpdcHandle from session and assert connection
    // Retrieve mpdcHandle from session and assert connection
    json_object *queryJ=afb_req_json(request);
    mpdcHandleT *mpdcHandle=GetSessionHandle(queryJ);
    if (mpdcIfConnectFail(MPDC_CHANNEL_CMD, mpdcHandle, request)) goto OnErrorExit;

    json_object *responseJ= CtlGetversion(mpdcHandle, request);
    if (!responseJ) goto OnErrorExit;

    afb_req_success(request, responseJ, NULL);

OnErrorExit:
    mpdcFlushConnect(mpdcHandle);
    return;
}

// Connect create a new connection to a given server
PUBLIC void mpdcapi_connect(afb_req request) {
    char session[16];
    int subscribe=false;

    mpdcHandleT *mpdcHandle = (mpdcHandleT*)calloc (1, sizeof(mpdcHandleT));
    mpdcHandle->magic=MPDC_SESSION_MAGIC;

    // retrieve query arguments if not present use MPCD default
    json_object *queryJ= afb_req_json(request);
    if (json_get_string(queryJ, "label", true, &mpdcHandle->label)) {
        afb_req_fail_f (request, "MPDC:Connect", "Missing Label:xxxx in Query=%s", json_object_get_string(queryJ));
        goto OnErrorExit;
    }
    if (json_get_string(queryJ, "host", true, &mpdcHandle->hostname)) mpdcHandle->hostname=NULL;
    if (json_get_int(queryJ, "port", true, &mpdcHandle->port)) mpdcHandle->port=0;
    if (json_get_int(queryJ, "timeout", true, &mpdcHandle->timeout)) mpdcHandle->timeout=MPDC_DEFAULT_TIMEOUT;

    //  Check/Build Connection to MPD
    if (mpdcIfConnectFail(MPDC_CHANNEL_CMD, mpdcHandle, request)) goto OnErrorExit;

    if (json_get_int(queryJ, "subscribe", true, &subscribe)) subscribe=false;
    if (subscribe) {
        int error=EventCreate(mpdcHandle, request);
        if (error) goto OnErrorExit;
    }

    // return handle address as hexadecimal
    snprintf(session, sizeof(session), "%p", mpdcHandle);
    json_object *responseJ=json_object_new_object();
    json_object_object_add(responseJ, "session", json_object_new_string(session));
    json_object_object_add(responseJ, "version", CtlGetversion(mpdcHandle, request));
    json_object_object_add(responseJ,"output",OutputSetGet(request, mpdcHandle, true, false, NULL));

    afb_req_success(request, responseJ, NULL);

OnErrorExit:
    mpdcFlushConnect(mpdcHandle);
    return;
}

// Create a private connection for synchronous commands
PUBLIC int mpdcapi_init(const char *bindername) {

    mpdcLocalHandle = (mpdcHandleT*)calloc (1, sizeof(mpdcHandleT));
    mpdcLocalHandle->label="LocalMpdc";
    mpdcLocalHandle->magic=MPDC_SESSION_MAGIC;
    mpdcLocalHandle->timeout=MPDC_DEFAULT_TIMEOUT;
    int error=mpdcIfConnectFail(MPDC_CHANNEL_CMD, mpdcLocalHandle, NULL_AFBREQ);

    // failing to connect is not a fatal error
    if (error) {
        AFB_WARNING("MPDC:mpdcapi_init No Default Music Player Daemon (setenv MPDC_NODEF_CONNECT");
        free (mpdcLocalHandle);
        mpdcLocalHandle=NULL;
        goto OnErrorExit;
    }

    // best effort to subscribe to event
    error=EventCreate(mpdcLocalHandle, NULL_AFBREQ);
    if (error) AFB_WARNING("MPDC:mpdcapi_init Fail Create Event for default Music Player Daemon");

    mpdcFlushConnect(mpdcLocalHandle);
    return false;

 OnErrorExit:  // failing to open default MPD is not a fatal error;
    return false;
}
