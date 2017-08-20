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

#include "libclient-utils.h"


PUBLIC void miscPostError(afb_req request, const char* errlabel, mpdcHandleT *mpdHandle) {
    
     mpdConnectT *conn = mpdHandle->mpd;
    
	if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
            afb_req_fail_f (request, errlabel, "(hoops) Fail to Connect to Music Media Player Daemon ");
            return;
        }

	const char *message = mpd_connection_get_error_message(conn);
	if (mpd_connection_get_error(conn) == MPD_ERROR_SERVER)
		/* messages received from the server are UTF-8; the
		   rest is either US-ASCII or locale */
		message = charset_from_utf8(message);

        AFB_ERROR ("mpd error: %s", message);
	afb_req_fail_f (request, errlabel, "mpd error: %s", message);
	mpd_connection_free(conn);
}
