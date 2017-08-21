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

STATIC json_object* GetEntityList(afb_req request,mpdcHandleT *mpdcHandle, const char *path, enum mpd_entity_type filter_type) {
    
    json_object *elemJ, *responseJ= json_object_new_array();

    int done = mpd_send_list_meta(mpdcHandle->mpd,path);
    if (!done) {
        afb_req_fail_f(request, "MPDC:ListDirFile fail to send_list_meta Daemon=%s name=%s", mpdcHandle->label, path);
        goto OnErrorExit;
    }                    
    
    struct mpd_entity *entity;
    while ((entity = mpd_recv_entity(mpdcHandle->mpd)) != NULL) {
        const struct mpd_directory *dir;
        const struct mpd_song *song;
        const struct mpd_playlist *playlist;


        enum mpd_entity_type type = mpd_entity_get_type(entity);
        if (filter_type != MPD_ENTITY_TYPE_UNKNOWN && type != filter_type) type = MPD_ENTITY_TYPE_UNKNOWN;

        switch (type) {
            case MPD_ENTITY_TYPE_UNKNOWN:
                break;

            case MPD_ENTITY_TYPE_DIRECTORY:
                dir = mpd_entity_get_directory(entity);
                wrap_json_pack(&elemJ,"s:s","directory", charset_from_utf8(mpd_directory_get_path(dir)));
                json_object_array_add(responseJ, elemJ);
                break;

            case MPD_ENTITY_TYPE_SONG:
                song = mpd_entity_get_song(entity);
                json_object_array_add(responseJ, CtlPlayCurrentSong(song));
                break;

            case MPD_ENTITY_TYPE_PLAYLIST:
                playlist = mpd_entity_get_playlist(entity);
                wrap_json_pack(&elemJ,"s:s","playlist", charset_from_utf8(mpd_playlist_get_path(playlist)));
                json_object_array_add(responseJ, elemJ);
                break;
        }
        if (type !=  MPD_ENTITY_TYPE_SONG)  mpd_entity_free(entity);
    }
    
    if (!mpd_response_finish(mpdcHandle->mpd)) {
        miscPostError(request,"MPDC:GetEntityList", mpdcHandle);
        goto OnErrorExit;
    }
    
    return responseJ;

OnErrorExit:    
    json_object_put(responseJ);
    return NULL;
    
}


PUBLIC json_object* ListDirSong (afb_req request, mpdcHandleT *mpdcHandle, json_object *pathsJ) {
    json_object *responseJ, *pathJ, *entityJ;
    const char *path;
       
    switch (json_object_get_type (pathsJ)) {
        
        case json_type_array:
            
            responseJ= json_object_new_array();
            
            for (int idx=0; idx < json_object_array_length(pathsJ); idx ++) {
                pathJ = json_object_array_get_idx(pathsJ, idx);
                path= json_object_get_string(pathJ);
                if (!pathJ) {
                    afb_req_fail (request, "MPDC:ListDirSong path not an array of string path=%s", path);
                    goto OnErrorExit;                    
                }
                entityJ=GetEntityList (request, mpdcHandle, charset_to_utf8(path), MPD_ENTITY_TYPE_UNKNOWN);
                if (entityJ) json_object_array_add(responseJ, entityJ); 
            }
       
        break;
        
        case json_type_string:
            path= json_object_get_string(pathsJ);
            entityJ=GetEntityList (request, mpdcHandle, charset_to_utf8(path), MPD_ENTITY_TYPE_UNKNOWN);
            break;
            
        case json_type_object:
        case json_type_null:
            responseJ=GetEntityList (request, mpdcHandle, NULL, MPD_ENTITY_TYPE_UNKNOWN);
            break;
            
        default:
            afb_req_fail_f(request, "MPDC:ListDirSong path should be a string a an array of string path=%s", json_object_get_string(pathsJ));
            goto OnErrorExit;
    }

    return responseJ;
    
OnErrorExit:
    return NULL;
}


PUBLIC json_object *ListPlayList (afb_req request, mpdcHandleT *mpdcHandle, const char *name) {

    json_object* responseJ = json_object_new_array();
    int count=0;

    // if name defined return name playlist content
    if (name) {
        int done = mpd_send_list_playlist_meta(mpdcHandle->mpd, name);
        if (!done) {
            afb_req_fail(request, "MPDC:PlaylistPrint Fail to retrieve content playlist=%s", name);
            goto OnErrorExit;
        }
    } else { // no name return current queue list
        mpd_send_list_queue_meta(mpdcHandle->mpd);
    }
    
    struct mpd_song *song;
    while ((song = mpd_recv_song(mpdcHandle->mpd)) != NULL) {
        json_object *songJ = CtlPlayCurrentSong(song);
        json_object_array_add(responseJ, songJ);
        count ++;
    }
 
    if (count == 0) {
        if (!name) name="Current/Active";
        afb_req_fail(request, "MPDC:PlaylistPrint List Empty playlist=%s", name);
        goto OnErrorExit;        
    }
    
    return responseJ;
    
OnErrorExit:
    json_object_put(responseJ);
    return NULL;
}
