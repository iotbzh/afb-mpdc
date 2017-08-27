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


PUBLIC int EventPush (mpdcHandleT *mpdcHandle, json_object *ctlEventJ) {
 
    if (!mpdcHandle || !afb_event_is_valid(mpdcHandle->event)) {
        AFB_ERROR ("MPDC:EventPush (Hoop) Event Structure is not valid");
        goto OnErrorExit;
    }
    
    mpdcHandle->count++;
    
    int done = afb_event_push(mpdcHandle->event, ctlEventJ);
    if (!done) {
        AFB_ERROR ("MPDC:EventPush (no client) event=%s count=%d", mpdcHandle->label, mpdcHandle->count);
        goto OnErrorExit;
    }
    
    return false;
    
OnErrorExit:
    return true;
}

// Subscribe Client to Event
PUBLIC int EventSubscribe (afb_req request, mpdcHandleT *mpdcHandle) {
    if (!afb_req_is_valid(request)) return true;
    
    if (!afb_event_is_valid(mpdcHandle->event)) {
        afb_req_fail (request, "MPDC:EventSubscribe", "Event should be created before subscription");
        return true;
    }
    
    int err = afb_req_subscribe(request, mpdcHandle->event);
    if (err) {
        afb_req_fail (request, "MPDC:EventSubscribe", "Client Protocol is not compatible with event subcription"); 
        return true;
    }
    
    return false;
}



// Create MPDC binder event and open a new socket to MPD for event only
PUBLIC int EventMpdSubscribe(mpdcHandleT *mpdcHandle, afb_req request) {
    int error;
    
    error=mpdcIfConnectFail(MPDC_CHANNEL_EVT, mpdcHandle, request);
    if (error) goto OnErrorExit;
    
    // create the event with label name
    
    // create binder event to send test pause/resume
    mpdcHandle->event = afb_daemon_make_event(mpdcHandle->label);
    if (!afb_event_is_valid(mpdcHandle->event)) {
        AFB_ERROR ("MPDC:EventMpdSubscribe Cannot create event label=%s",mpdcHandle->label);
        return 1;
    }
    
    int err = afb_req_subscribe(request, mpdcHandle->event);
    if (err) {
        AFB_ERROR ("MPDC:EventMpdSubscribe Fail Client Event Subscribe label=%s",mpdcHandle->label);
        goto OnErrorExit;
    }
    
    // Connect Event Channel to Mainloop
    error=MainLoopAddMpdc(mpdcHandle);
    if (error) goto OnErrorExit;
    
    return false;
    
OnErrorExit:
    return true;    
}
