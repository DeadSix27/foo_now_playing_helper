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

#include <cmath>
#include "Plugin.h"
#include "base64.h"
#include <iostream>


DECLARE_COMPONENT_VERSION (
	"Now Playing Helper",
	"1.0.1",
	"Copyright (C) 2019 DeadSix (https://github.com/DeadSix27)\n"
	"Helper component to finally have a useful way of capturing foobar events & title information.\n"
	"\n"
);

static initquit_factory_t<foo_nph> foo_interface;

foo_nph::foo_nph()
{
}

foo_nph::~foo_nph()
{
}


void foo_nph::on_init()
{
	OutputDebugString(_T("D6FNP_INIT!"));
	static_api_ptr_t<play_callback_manager> pcm;
	pcm->register_callback(
		this,
		play_callback::flag_on_playback_starting |
		play_callback::flag_on_playback_stop |
		play_callback::flag_on_playback_pause |
		play_callback::flag_on_playback_new_track |
		play_callback::flag_on_playback_edited |
		play_callback::flag_on_playback_dynamic_info_track,
		false);
}

void foo_nph::on_quit()
{
	OutputDebugString(_T("D6FNP_QUIT!"));
}

void foo_nph::on_playback_starting(play_control::t_track_command command, bool paused)
{
	if (paused)
	{
		
		OutputDebugString(_T("D6FNP_STATECHANGE_PAUSE!"));
	}
	else
	{
		switch (command)
		{
		case play_control::track_command_play:
			OutputDebugString(_T("D6FNP_STATECHANGE_PLAY!"));
			break;
		case play_control::track_command_resume:
			OutputDebugString(_T("D6FNP_STATECHANGE_RESUME!"));
			break;
		case play_control::track_command_settrack:
			OutputDebugString(_T("D6FNP_STATECHANGE_SETTRACK!"));
			break;
		}
	}
}

void foo_nph::on_playback_stop(play_control::t_stop_reason reason)
{
	switch (reason)
	{
	case play_control::stop_reason_user:
	case play_control::stop_reason_eof:
	case play_control::stop_reason_shutting_down:
		OutputDebugString(_T("D6FNP_STATECHANGE_STOP!"));
		break;
	}
}

void foo_nph::on_playback_pause(bool state)
{
	OutputDebugString(_T("D6FNP_PBPAUSE!"));
}

void foo_nph::on_playback_new_track(metadb_handle_ptr track)
{
	OutputDebugString(_T("D6FNP_NEWTRACK!"));

	service_ptr_t<titleformat_object> script;
	//pfc::string8 format = "[%title%]\t[%artist%]\t[%album%]\t$year(%date%)\t$num(%tracknumber%,0)\t%length_seconds%\t%path%\t$mul($min($max(0,%rating%),5),2)";
	pfc::string8 format = "$if(%title%,%title%,-)\t$if(%artist%,%artist%,-)\t$if(%album%,%album%,-)\t$if(%album artist%,%album artist%,-)\t$if(%codec%,%codec%,-)\t$if(%bitrate%,%bitrate%,-)\t$if(%samplerate%,%samplerate%,-)\t$if($info(bitspersample),$info(bitspersample),-)\t$if(%filesize%,%filesize%,-)\t$if($year(%date%),$year(%date%),-)\t$num(%discnumber%,0)\t$num(%tracknumber%,0)\t$if(%length_seconds%,%length_seconds%,-)\t$if(%path%,%path%,-)\t$if(%path%,$directory_path(%path%),-)\t$mul($min($max(0,%rating%),5),2)";

	if (static_api_ptr_t<titleformat_compiler>()->compile(script, format))
	{
		static_api_ptr_t<playback_control> pbc;

		pbc->playback_format_title_ex(
			track,
			nullptr,
			format,
			script,
			nullptr,
			playback_control::display_level_titles);
		std::string out1; 
		std::string raw = format.get_ptr();
		std::string delimiter = "\t";
		size_t pos = 0;
		std::string token;
		while ((pos = raw.find(delimiter)) != std::string::npos) {
			token = raw.substr(0, pos);

			const unsigned char * tokenC = reinterpret_cast<const unsigned char *>(token.c_str());
			out1 += "|" + base64_encode(tokenC,token.length());

			raw.erase(0, pos + delimiter.length());
		}
		const unsigned char * tokenC = reinterpret_cast<const unsigned char *>(raw.c_str());
		out1 += "|" + base64_encode(tokenC,raw.length());

		//std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(format.get_ptr()), format.get_length());
		out1 = "D6FNP_SONGINFO!" + out1.substr(1);
		std::wstring stemp = std::wstring(out1.begin(), out1.end());
		LPCWSTR result = stemp.c_str();
		OutputDebugString(result);
	}
}
void foo_nph::on_playback_dynamic_info_track(const file_info& info)
{
	metadb_handle_ptr track;
	static_api_ptr_t<playback_control> pbc;
	if (pbc->get_now_playing(track))
	{
		on_playback_new_track(track);
	}
}
