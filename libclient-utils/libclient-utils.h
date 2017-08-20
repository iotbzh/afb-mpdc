/*
 * AlsaLibMapping -- provide low level interface with ALSA lib (extracted from alsa-json-gateway code)
 * Copyright (C) 2015,2016,2017, Fulup Ar Foll fulup@iot.bzh
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
 */

 
#ifndef MPCLIBUTILS_H
#define MPCLIBUTILS_H


#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/song.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>

#include <common-utils.h>

#ifndef PUBLIC
  #define PUBLIC
#endif
#define STATIC static

// imported from LibClient Music Player Daemon CLI
#include "charset.h"

// make our life easier
typedef struct mpd_connection mpdConnectT;
typedef struct mpd_output mpdOutputT;
typedef struct mpd_status mpdStatusT;


typedef enum {
    MPDC_CHANNEL_CMD,
    MPDC_CHANNEL_EVT,
} mpdcChannelEnumT;

typedef struct {
    long magic;
    const char *label;
    const char *hostname;
    int port;
    int timeout; // ms
    int count;
    mpdConnectT *mpd;
    mpdConnectT *mpdEvt;
    afb_event event;
} mpdcHandleT;

// output.c
PUBLIC json_object* OutputSetGet(afb_req request, mpdcHandleT *mpdHandle, bool list, bool only, json_object *targetsJ);

// misc.c
PUBLIC void miscPostError(afb_req request, const char* errlabel, mpdcHandleT *mpdHandle);

// search.c add few type TAG ???
enum {
	SEARCH_TAG_ANY = MPD_TAG_COUNT + 1,
	SEARCH_TAG_URI = MPD_TAG_COUNT + 2,
	SEARCH_TAG_BASE,
};

enum mpd_tag_type SearchTypeTag(const char *name);
bool SearchAddConstraints(afb_req request,mpdcHandleT *mpdHandle, json_object *constraintsJ);
bool SearchAddOneConstraint(afb_req request, mpdcHandleT *mpdHandle, json_object *contraintJ);

// status.c
PUBLIC json_object *StatusSongTag(const struct mpd_song *song, enum mpd_tag_type type);
PUBLIC json_object *StatusGetAll(afb_req request,  mpdcHandleT *mpdHandle);
PUBLIC mpdStatusT *StatusRun(afb_req request, mpdcHandleT *mpdHandle);

// control.c
PUBLIC json_object *CtlPlayCurrentSong(struct mpd_song *song);
PUBLIC json_object *CtlGetversion(mpdcHandleT *mpdcHandle, afb_req request);

#endif /* MPCLIBUTILS_H */
