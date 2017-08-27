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
 *  https://github.com/DaveDavenport/mpccmd/blob/master/mpcmd.c
 *  https://searchcode.com/codesearch/view/40096054/
 */

#define _GNU_SOURCE

#include <sys/prctl.h>
#include <stdio.h>
#include <systemd/sd-event.h>

#include "mpdc-binding.h"

// Jose this is really scrap !!!
static afb_req NULL_AFBREQ = {};

STATIC int MainLoopCallback(sd_event_source* src, int fd, uint32_t revents, void* handle) {
    mpdcHandleT *mpdcHandle = (mpdcHandleT*)handle;
    
    // Looks like that at this point we should not put any filter on mpd_recv_idle (Fulup why???)
    mpd_recv_idle(mpdcHandle->mpdEvt, true);
    if(mpd_connection_get_error(mpdcHandle->mpdEvt)){
        AFB_ERROR ("MPDC:MainLoopCallback Got Error=%s", mpd_connection_get_error_message(mpdcHandle->mpdEvt));
        
        // try to recover from error if needed close and reopen connection after timeout
        int success = mpd_connection_clear_error(mpdcHandle->mpdEvt);
        if (!success) {
            mpdcHandle->mpdEvt=NULL; // Force reconnection to server
            int error= mpdcIfConnectFail(MPDC_CHANNEL_EVT, mpdcHandle, NULL_AFBREQ); 
            if (!error) MainLoopAddMpdc(mpdcHandle);
            return false;
        }
        goto OnErrorExit;
    }
    
    // event are empty we need to query back MPD
    mpd_command_list_begin(mpdcHandle->mpdEvt, true);
    mpd_send_status(mpdcHandle->mpdEvt);
    mpd_send_current_song(mpdcHandle->mpdEvt);
    mpd_command_list_end(mpdcHandle->mpdEvt);
    
    // now that got status we may parse it to generate some form of usefull events
    enum mpd_state state= mpd_status_get_state(mpd_recv_status(mpdcHandle->mpdEvt));
    char*stateName;
    switch (state) {
        case MPD_STATE_STOP:
            stateName="MPD_STATE_STOP";
            break;
        case MPD_STATE_PLAY:
            stateName="MPD_STATE_PLAY";
            break;
        case MPD_STATE_PAUSE:
            stateName="MPD_STATE_PAUSE";
            break;
        case MPD_STATE_UNKNOWN:
        default:
            stateName="MPD_STATE_UNKNOWN";
    }
        
    // We are now ready to push MPD event to every subscribers.
    json_object *ctlEventJ= json_object_new_object();
    json_object_object_add(ctlEventJ,"state", json_object_new_string(stateName));
    if (state == MPD_STATE_PLAY) {
        mpd_response_next(mpdcHandle->mpdEvt);
        struct mpd_song *song;
        song = mpd_recv_song(mpdcHandle->mpdEvt);
        json_object *songJ= CtlPlayCurrentSong(song);
        json_object_object_add(ctlEventJ,"song", songJ);
    }
     
   
    int error=EventPush (mpdcHandle, ctlEventJ);
    if (error) AFB_NOTICE ("MPDC received MPD event=%s (no subscriber)", stateName);
    mpd_response_finish(mpdcHandle->mpdEvt);
    mpd_send_idle_mask(mpdcHandle->mpdEvt, MPD_IDLE_PLAYER); 
    return 0;

OnErrorExit:
    AFB_WARNING("MPDC:MainLoopCallback: Connect in Error Try Reset");
    return 0;
}

PUBLIC int MainLoopAddMpdc(mpdcHandleT *mpdcHandle) {
    int err;
    sd_event_source *source;
    
    // Listen only to Player Event
    //mpd_connection_set_timeout(mpdcHandle->mpdEvt,1000); // set timeout to 1s
    mpd_send_idle_mask(mpdcHandle->mpdEvt, MPD_IDLE_PLAYER); 

    // retrieve socket handle
    int sockfd=mpd_connection_get_fd(mpdcHandle->mpdEvt);
    if (sockfd <= 0) {
        AFB_ERROR("MPDC:MainLoopCallback Fail to retrieve MPDC connection FD");
        goto OnErrorExit;        
    }
    
    // add socket file descriptor to binder libsystemd mainloop
    err = sd_event_add_io(afb_daemon_get_event_loop(), &source, sockfd, EPOLLIN, MainLoopCallback, mpdcHandle);
    if (err < 0) {
        AFB_ERROR("MPDC:MainLoopCallback Cannot hook MPDC to Binder");
        goto OnErrorExit;
    }
    
    return false;

OnErrorExit:
    return true;
}


