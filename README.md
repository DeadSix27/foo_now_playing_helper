

# foo_now_playing_helper

A foobar2000 component that announces events as well as track metadata via `OutputDebugString`, which allows for easy capture in other programs.


#### Announced events:

* `D6FNP_INIT`

	Sent when the plugin is loaded (e.g foobar2000 starts)
* `D6FNP_QUIT`

	Sent when the plugin is unloaded (e.g when foobar2000 exits)
* `"D6FNP_STATECHANGE_PLAY!"`

	Sent when playback is started.
* `D6FNP_STATECHANGE_PAUSE`; `D6FNP_PBPAUSE`

	Sent when playback is paused.
* `D6FNP_STATECHANGE_RESUME`

	Sent when playback is resumed.
* `D6FNP_STATECHANGE_SETTRACK`

	(I forgot when this happens, see: `play_control::track_command_settrack`, probably never occurs)
* `D6FNP_STATECHANGE_STOP`

	Sent when playback is stopped.
* `D6FNP_NEWTRACK`

	Sent when a new track is starting, e.g when a song ends or you skip to the next.
* `D6FNP_SONGINFO`

	Occurs at the same time as `D6FNP_NEWTRACK`, contains all the useful song information encoded as base64 and separated by `|`.

#### Structure of `D6FNP_SONGINFO`:
Notes:
- In this example we replaced the `|` separator by line breaks to make it readable.
- Every value is separately encoded in base64 to retain Unicode characters of different languages.
```
D6FNP_SONGINFO!
%title%
%artist%
%album artist%
%album%
%codec%
%bitrate%
%samplerate%
$info(bitspersample)
%filesize%
$year(%date%)
%discnumber%
%tracknumber%
%length_seconds%
%path%
$directory_path(%path%)
%rating%
```
Populated example:
```
D6FNP_SONGINFO!
Moonlight
Asterisk feat. 那由多
Lost Landscape
Lilium Records
FLAC
1045
44100
16
35396699
2018
1
1
269
G:\example_path\music\Lilium Records - Lost Landscape\01. Moonlight.flac
G:\example_path\music\Lilium Records - Lost Landscape\
8
```

### Tools
You can use `Dbgview.exe` to view the messages: https://docs.microsoft.com/en-us/sysinternals/downloads/debugview
