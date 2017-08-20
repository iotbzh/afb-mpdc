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
 *  https://www.musicpd.org/doc/libmpdclient/song_8h.html
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


#include "mpdc-binding.h"

static MpdcHandleT* mpdcHandle=NULL;

// Jose this is really scrap !!!
static afb_req NULL_AFBREQ = {};

typedef struct {
    const char *name;
    int  count;
    afb_event event;
} afbEventT;
static afbEventT *afbEvent=NULL;


PUBLIC bool EventPush ( json_object *ctlEventJ) {
    afbEvent->count++;
 
    if (!afbEvent && !afb_event_is_valid(afbEvent->event)) {
        AFB_ERROR ("MPDC:EventPush Hoop Event Structure is not valid");
        goto OnErrorExit;
    }
    
    int done = afb_event_push(afbEvent->event, ctlEventJ);
    if (!done) {
        AFB_ERROR ("MPDC:EventPush (no client) event=%s count=%d", afbEvent->name, afbEvent->count);
        goto OnErrorExit;
    }
    
    return false;
    
OnErrorExit:
    return true;
}

// Subscribe Client to Event
PUBLIC bool EventSubscribe (afb_req request) {
    if (!afb_req_is_valid(request)) return true;
    
    if (!afbEvent) {
        afb_req_fail (request, "MPDC:EventSubscribe", "Event should be created before subscription");
        return true;
    }
    
    int err = afb_req_subscribe(request, afbEvent->event);
    if (err) {
        afb_req_fail (request, "MPDC:EventSubscribe", "Client Protocol is not compatible with event subcription");        
    }
    
    return false;
}


// Entry point for client to register to MPDC binding events
PUBLIC void mpdcapi_subscribe(afb_req request) {


    int error= EventSubscribe(request);    
    if (error) goto OnErrorExit;        
    
    afb_req_success(request, NULL, NULL);
    return;
    
 OnErrorExit:
    return;    
}

// Create MPDC binder event and open a new socket to MPD for event only
PUBLIC bool EventInit(const char *binderName) {
    bool error;
    
    // Free any previous event structure
    if (afbEvent) {
        if (afb_event_is_valid(afbEvent->event)) return false;
        else free(afbEvent);  // Fulup->Jose: Question how binder known about this ???
    }
    
    afbEvent = calloc (1, sizeof(afbEventT));
    afbEvent->name=binderName;
    afbEvent->event = afb_daemon_make_event(binderName);
    
    // Prepare a dedicated connection for MPD for events
    mpdcHandle = (MpdcHandleT*)calloc (1, sizeof(MpdcHandleT));
    mpdcHandle->label="EvtChannel";
    error=mpdcFailConnect(mpdcHandle, NULL_AFBREQ);
    if (error) goto OnErrorExit;
    
    // Connect Event Channel to Mainloop
    error=MainLoopAddMpdc(mpdcHandle);
    if (error) goto OnErrorExit;
    
    return true;
    
OnErrorExit:
    return true;    
}
