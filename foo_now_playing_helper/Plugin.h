/* 
 * This file is part of https://github.com/DeadSix27/foo_now_playing_helper
 * Copyright (C) 2019 DeadSix27
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FOONPH_PLUGIN_H_
#define FOONPH_PLUGIN_H_

#include "..\..\FoobarSDK\foobar2000\SDK\foobar2000.h"


class foo_nph :
	public initquit,
	public play_callback
{
public:
	foo_nph();
	~foo_nph();

	void on_init();
	void on_quit();

	void on_playback_starting(play_control::t_track_command command, bool paused);
	void on_playback_stop(play_control::t_stop_reason reason);
	void on_playback_pause(bool state);
	void on_playback_new_track(metadb_handle_ptr track);
	void on_playback_edited(metadb_handle_ptr track) { on_playback_new_track(track); }
	void on_playback_dynamic_info_track(const file_info& info);

	// Not implemented
	void on_playback_time(double time) {}
	void on_playback_seek(double time) {}
	void on_playback_dynamic_info(file_info const& info) {}
	void on_volume_change(float p_new_val) {}
};

#endif
