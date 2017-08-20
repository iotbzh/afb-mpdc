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
#include "json-setget.h"

PUBLIC json_object *CtlPlayCurrentSong(struct mpd_song *song) {
    
    if (!song)  goto OnErrorExit;
    

    // Extract Song Informations
    const char *uri= mpd_song_get_uri(song);
    json_object *artist= StatusSongTag(song, MPD_TAG_ARTIST);
    json_object *album = StatusSongTag(song, MPD_TAG_ALBUM);
    json_object *title = StatusSongTag(song, MPD_TAG_ALBUM);
    json_object *track = StatusSongTag(song, MPD_TAG_TRACK);
    json_object *name  = StatusSongTag(song, MPD_TAG_NAME);
    json_object *date  = StatusSongTag(song, MPD_TAG_DATE);
    
    // build response 
    //int error= wrap_json_pack(&responseJ, "{s:s*, s:s*, s:s*, s:s*, s:s*, s:s*, s:s*}"
    //    , "uri",uri, "artist",artist, "album", album, "title", title, "track", track, "name", name, "date", date);
    //    if (error) {
    //       afb_req_fail(request, "MDCP:CtlPlayCurrentSong", "Fail to process song tags"); 
    //       goto OnErrorExit;        
    //    }

    json_object *responseJ=json_object_new_object();
    if(artist) json_object_object_add(responseJ, "artist", artist);
    if(album) json_object_object_add(responseJ, "album", album);
    if(album) json_object_object_add(responseJ, "title", title);
    if(track) json_object_object_add(responseJ, "track", track);
    if(name) json_object_object_add(responseJ, "name", name);
    if(date) json_object_object_add(responseJ, "date", date);
    if(uri) json_object_object_add(responseJ, "uri", json_object_new_string(uri));
    
        
    // free song resources
    mpd_song_free(song);    
    
    return responseJ;

OnErrorExit:
    return NULL;
}


PUBLIC json_object *CtlGetversion(mpdcHandleT *mpdcHandle, afb_req request) {
    json_object *responseJ=NULL;
    const unsigned* version;

    // get MPD version
    version=mpd_connection_get_server_version(mpdcHandle->mpd);
    int major=version[0]; 
    int minor=version[1];
    int patch=version[2];
    
    // return as a json_object
    wrap_json_pack(&responseJ, "{si,si,si}", "major", major, "minor",minor, "patch",patch);
    
    return (responseJ);

}
