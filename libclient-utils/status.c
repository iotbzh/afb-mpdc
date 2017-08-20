/*
 * music player command (mpc)
 * Copyright (C) 2003-2015 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "charset.h"

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <locale.h>
#include <langinfo.h>
#include <iconv.h>

#include "libclient-utils.h"

// Extract Song Data Tag and return them as a json array of strings
PUBLIC json_object *StatusSongTag(const struct mpd_song *song, enum mpd_tag_type type) {
    json_object *responseJ = json_object_new_array();
    int  index;
    const char *value;
    
    for (index=0; (value=mpd_song_get_tag(song,type, index)) != NULL; index++) {
        json_object_array_add(responseJ, json_object_new_string(value));
    }
    
    // Label not found
    if (index==0) goto OnErrorExit;
    
    return responseJ;
    
OnErrorExit:
    // No response free json array
    json_object_put(responseJ);
    return NULL;
    
}    

PUBLIC json_object *StatusGetAll(afb_req request,  mpdConnectT *mpdConnect) {
    struct mpd_status *status;
    const struct mpd_audio_format *audio_format;
    json_object *daemonStatus=NULL, *songStatus=NULL, *formatStatus=NULL;
    int err=0;

    if (!mpd_command_list_begin(mpdConnect, true) || !mpd_send_status(mpdConnect) ||
	!mpd_send_current_song(mpdConnect) ||  !mpd_command_list_end(mpdConnect)) {
        
        afb_req_fail_f (request, "MDCP:StatusGetAll","Sending request status fail err=%s", mpd_connection_get_error_message(mpdConnect));
        goto OnErrorExit;
        
    }
    
    status = mpd_recv_status(mpdConnect);
    if (!status) {
        afb_req_fail_f (request, "MDCP:StatusGetAll", "Receiving status fail err=%s", mpd_connection_get_error_message(mpdConnect));
        goto OnErrorExit;
    }
       
    if (mpd_status_get_error(status)) {
        AFB_WARNING("MDCP:StatusGetAll Get an Error Message: %s", mpd_status_get_error(status));
    }

    int volume= mpd_status_get_volume(status);
    int repeat= mpd_status_get_repeat(status);
    int single= mpd_status_get_single(status);
    int consum= mpd_status_get_consume(status);
    int random= mpd_status_get_random(status);
    int qlength = mpd_status_get_queue_length(status);
    int qversion= (int)mpd_status_get_queue_version(status);
    
    err+= wrap_json_pack(&daemonStatus, "{si,si,si,si,si,si,si}"
        , "volume", volume, "repeat",repeat, "single",single, "consum", consum, "random",random, "qlength",qlength, "qversion", qversion);


    if (mpd_status_get_state(status) == MPD_STATE_PLAY || mpd_status_get_state(status) == MPD_STATE_PAUSE) {
        
        int song   = mpd_status_get_song_pos(status);
        int elasped= mpd_status_get_elapsed_time(status);
        int total  = mpd_status_get_total_time(status);
        int rate   =mpd_status_get_kbit_rate(status);
        
        err+= wrap_json_pack(&songStatus, "{si,si,si,si}"
        , "song", song, "elasped",elasped, "total",total, "rate", rate);
        
    }

    audio_format = mpd_status_get_audio_format(status);
    if (audio_format != NULL) {
        int rate= audio_format->sample_rate;
        int bits= audio_format->bits;
        int channels= audio_format->channels;

        err+= wrap_json_pack(&formatStatus, "{si,si,si}"
        , "rate", rate, "bits",bits, "channels",channels);
    }

    json_object *responseJ;
    err+= wrap_json_pack (&responseJ, "{so, so*, so*}", "daemon", daemonStatus, "song", songStatus, "format", formatStatus);
    if (err) { // something went 
        afb_req_fail_f(request, "MDCP:StatusGetAll fail to encode status daemonStatus=%s songStatus=%s formatStatus=%s"
            , json_object_get_string(daemonStatus), json_object_get_string(songStatus), json_object_get_string(formatStatus));
        
        json_object_put(responseJ); // free now useless responseJ
        goto OnErrorExit;
    }
    
    return responseJ;
OnErrorExit:
    return NULL;
}


PUBLIC struct mpd_status *StatusRun(afb_req request, mpdConnectT*conn) {
	struct mpd_status *ret = mpd_run_status(conn);
	if (ret == NULL)
		miscPostError(request, "StatusRun", conn);

	return ret;
}