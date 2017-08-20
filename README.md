------------------------------------------------------------------------
                Audio Client Binding for Music Daemon Player
------------------------------------------------------------------------

# Cloning Music Player Binding from Git
-------------------------------------------------------

```
# Initial clone with submodules
git clone --recurse-submodules https://github.com/iotbzh/mpdc-binding

# Do not forget submodules with pulling
git pull --recurse-submodules https://github.com/iotbzh/mpdc-binding
```

### Dependencies

 * OpenSuse: See instruction for AGL Linux native dev packages at https://en.opensuse.org/LinuxAutomotive
    * sudo zypper in agl-app-framework-binder
    * sudo zypper in libmpdclient-devel mpd


### Quick start

 * Compile mpdc-binding
   * cd  mpdc-binding
   * cmake ..
   * make
 * Create an Alsa Config for Music Player Daemon
   * Check conf.d/project/mpd.d/mpd-fulup-home.conf
   * Place config file somewhere (default mpd search location $HOME/.config/mpd/mpd.conf or /etc/mpd.conf)
   * Edit mpd.conf to reflect your configuration
 * Start MPD in debug mode
   * mpd --no-daemon path-to-mpd.conf
 * Start mpdc-binding
   * afb-daemon 

 * Note: For you initial test, you may want to use a richer graphical frontend like cantata(Qt).
    ** OpenSuse Binary Package https://software.opensuse.org/package/cantata

### API

 * Search search the database for song,categories and optionally add them to play list
    * display(mandatory): fiel to be return by command eg: artist, title, filename, ...
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
 
 * Play a song from a playlist
    * current(default:true): play current song from current playlist
    * index(default:true) song index in play list
    * playlist(optional): chosen playlist name
    * example:  {api:'mpdc',verb:'play', query={paylist:'Fulup', index:10}}

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
    * list  "List currently available playlists"
    * print "[<playlist>]", "Print <playlist>"
    * clear "Clear the current playlist"
    * shuffle "Shuffle the current playlist"
    * move "<from> <to>", "Move song in playlist"
    * insert "<uri>", "Insert a song to the current playlist after the current track"
    * save "<file>", "Save a playlist as <file>"
    * load "<file>", "Load <file> as a playlist"
    * remove "<file>", "Remove a playlist"

  * Manage few administrative commands
    * update "[<path>]", "Scan music directory for updates"
    * stat "Display statistics about MPD"
    * version "Report version of MPD"
    



 

 