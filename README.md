------------------------------------------------------------------------
#                Audio Client Binding for Music Daemon Player
------------------------------------------------------------------------

## Cloning Music Player Binding from Git
-------------------------------------------------------

```
# Initial clone with submodules
git clone --recurse-submodules https://github.com/iotbzh/mpdc-binding

# Do not forget submodules with pulling
git pull --recurse-submodules https://github.com/iotbzh/mpdc-binding
```

## Dependencies

See instructions for AGL Linux native dev packages at https://en.opensuse.org/LinuxAutomotive#Installation_AGL_Application_Framework

 * OpenSuse:
    * sudo zypper in agl-app-framework-binder
    * sudo zypper in libmpdclient-devel mpd

* Ubuntu 16.04LTS
    * sudo apt install agl-app-framework-binder
    * sudo apt install libmpdclient-dev mpd

## Quick start

 * Compile mpdc-binding
   * cd mpdc-binding
   * mkdir build && cd build
   * cmake ..
   * make
 * Create an Alsa Config for Music Player Daemon
   * Check conf.d/project/mpd.d/mpd-fulup-home.conf
   * Place config file somewhere (default mpd search location $HOME/.config/mpd/mpd.conf or /etc/mpd.conf)
   * Edit mpd.conf to reflect your configuration
 * Start MPD in debug mode
   * mpd --no-daemon --stderr -v path-to-your-mpd.conf
 * Start mpdc-binding
   * afb-daemon

 * Verify your Alsa Config
   * Make sure pulse does not preempt your sound card (pavucontrol/configuration/soundcard/off)
   * Install you asoundrc file (see sample in conf.d/project/alsa.d)
   * Assert that your Alsa/PCM is active example:speaker-test -DNavPCM -c2 -twav
   ```
     grep pcm ~/.asoundrc  # check your configure PCM
     speaker-test -Dhw:v1340 -c2 -twav        # check sound card
     speaker-test -DMixerPCM -c2 -twav        # check Alsa Mixer
     speaker-test -DNavSoftvolPCM -c2 -twav   # check Navigation Softvol
     speaker-test -DNavigation -c2 -twav      # WARNING: AAAA/control should be ready to respond

     Note: no need to reload Alsa for those test. ~.asoundrc is read for each new request
   ```

 * Note: For you initial test, you may want to use a richer graphical frontend like cantata(Qt).
    ** OpenSuse Binary Package https://software.opensuse.org/package/cantata

 * Check it works (list music repository)
   * mpc ls
   * mpc findadd filename 'music-filename.mp3'  # warning should be fullname
   * mpc playlist # check your playlist contend
   * mpc play
   ```
   # Do not set MPD_PORT in environement to advoid further conflict
   export NAV_MPD=6002   # My Music Player Deamon Port
   - MPD_PORT=$NAV_MPD mpc ls   # List all files from my Music library
   - MPD_PORT=$NAV_MPD mpc output # list audio output

   - MPD_PORT=$NAV_MPD mpc findadd filename 'message-1.mp3' # add file to playlist
   - MPD_PORT=$NAV_MPD mpc playlist # list song in default playlist
   - MPD_PORT=$NAV_MPD mpc clear # clear all playlist

   - MPD_PORT=$NAV_MPD mpc play
   - MPD_PORT=$NAV_MPD mpc pause
   - MPD_PORT=$NAV_MPD mpc toggle


   ```

## API

 * Search search the database for song,categories and optionally add them to play list
    * display(mandatory): field to be return by command eg: artist, title, filename, ...
    * query(optional): artist_name (should match with type)
    * target(optional) and array of [{type:xxx, content:yyy}]
    * add(optional): boolean (search and add to play list)
    * exact(optional): boolean (search substring or not)
    * example:  {api:'mpdc',verb:'search', query={display:'artist', target={type:'genre', content:'folk'}}

 * Output select or return the output list from configuration. Take list:true as default
    * list(default:true) return the list of output with the enable/disable flag.
    * only(default:false) revers action for every non listed output
    * target:(default:none) json array of targeted outputs [{name:xxxx|id:??|all:true, enable:true/false}]
    * example: {api:'mpdc',verb:'output', only:true, target:[{name='zone-front',enable:true(default)}, ...], ...}

 * Control send control action to music daemon (except when specified control are simple boolean)
    * pause
    * resume
    * toggle "Toggles Play/Pause, plays if stopped"
    * next "Play the next song in the current playlist"
    * prev "Play the previous song in the current playlist"
    * play "[<position>]", "Start playing at <position>"
    * stop "Stop the currently playing playlists"
    * seek "[+-][HH:MM:SS]|<0-100>%", "Seeks to the specified position"

  * Playlist control playlist
    * current  "Print current Playlist Queue"
    * name "[<playlist>]", "Print <playlist> content, multiple list name might be given as an array"
    * clear "Clear the current playlist"
    * shuffle "Shuffle the current playlist"
    * move "<from> <to>", "Move song in playlist"
    * save "<file>", "Save a playlist as <file>"
    * load "<file>", "Load <file> as a playlist"
    * remove "<file>", "Remove a playlist"

  * File Management
    * listsong [path] list all song from Daemon at given path (no path == list all)

  * Manage few administrative commands
    * update "[<path>]", "Scan music directory for updates"
    * stat "Display statistics about MPD"
    * version "Report version of MPD"

## Environement Variables

* MPDC_NODEF_CONNECT by default MPDC will try to connect to Music Player Daemon default port on localhost. When MPDC_NODEF_CONNECT
  is et no connection is try at initialisation time and MPDC will wait for effective connection request. Note that MPDC support
  simultaneous connections to as many MPD you need (in demo sample config: Multimedia, Navigation, Emergency, ...).




