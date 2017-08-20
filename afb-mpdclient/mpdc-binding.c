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
 * references:
 *  
 */

#define _GNU_SOURCE

#include <sys/prctl.h>
#include <stdio.h>

#include "mpdc-binding.h"

// Include Binding Stub generated from Json OpenAPI
STATIC int mpdcBindingInit(void);
STATIC void mpdcDispatchEvent(const char *evtLabel, json_object *eventJ);
#include "mpdc-apidef.h"


PUBLIC  bool mpdcFailConnect(MpdcHandleT *mpdcHandle, afb_req request) {
    bool forceConnect= false; 
    
    // when trying to reconnect from main loop request is not valid
    if (!afb_req_is_valid(request) || !mpdcHandle->mpd) forceConnect=true;
    
    // if not already connected let's try to connect
    if (forceConnect) {
        // connect to MPD daemon NULL=localhost, 0=default port, 30000 timeout/ms
        mpdcHandle->mpd = mpd_connection_new(NULL, 0, 30000);
        if (mpdcHandle->mpd == NULL) {
            if (afb_req_is_valid(request)) afb_req_fail (request, "MDCP:Create", "No More Memory");
            goto OnErrorExit;
        }        
    }

    if (mpd_connection_get_error(mpdcHandle->mpd) != MPD_ERROR_SUCCESS) {
            if (afb_req_is_valid(request)) afb_req_fail (request, "MDPC:Connect", mpd_connection_get_error_message(mpdcHandle->mpd));
            else AFB_ERROR("MDPC:Connect error=%s",  mpd_connection_get_error_message(mpdcHandle->mpd));
            mpd_connection_free(mpdcHandle->mpd);
            mpdcHandle->mpd = NULL;
            goto OnErrorExit;
    }    
    return false;
    
 OnErrorExit:
    return true;
}


// Call when ever en event reach Mpdc Binding
STATIC void mpdcDispatchEvent(const char *evtLabel, json_object *eventJ) {
    
}

// Call at Init time (place here any runtime dependencies)
STATIC int mpdcBindingInit(void) {

    
    int rc= prctl(PR_SET_NAME, "afb-mpdc-agent",NULL,NULL,NULL);
    if (rc) AFB_ERROR("ERROR: AlsaCore fail to rename process");
    
    // create a global event to send MPDC events
    const char*binderName = GetBinderName();
    
    // Initialise Needed Components
    rc+=mpdcapi_init(binderName);
    rc+=EventInit(binderName);
    
    return rc;
}


