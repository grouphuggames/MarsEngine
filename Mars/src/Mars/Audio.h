#pragma once
#include "Core.h"
#include <xaudio2.h>

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'


namespace Mars
{
	class Audio
	{
	public:
		Audio() = default;

		void Init()
		{
			if (audio == nullptr)
			{
				HRESULT hr;
				hr = XAudio2Create(&audio, 0, XAUDIO2_DEFAULT_PROCESSOR);
				if (FAILED(hr))
				{
					MARS_CORE_ERROR("FAILED TO INIT AUDIO!");
				}

				hr = audio->CreateMasteringVoice(&master_voice);
				if (FAILED(hr))
				{
					MARS_CORE_ERROR("FAILED TO CREATE AUDIO MASTER VOICE!");
				}
			}
		}

		void PlayAudio(char* file_name)
		{
			AudioInfo info = {};
			info.file_name = file_name;

			OpenAudioFile(info);

			IXAudio2SourceVoice* source_voice;
			HRESULT hr = audio->CreateSourceVoice(&source_voice, (WAVEFORMATEX*)&info.wfx);
			if (FAILED(hr))
			{
				MARS_CORE_ERROR("FAILED TO CREATE SOURCE VOICE");
			}

			hr = source_voice->SubmitSourceBuffer(&info.buffer);
			if (FAILED(hr))
			{
				MARS_CORE_ERROR("FAILED TO SUBMIT SOURCE BUFFER");
			}

			hr = source_voice->Start(0);
			if (FAILED(hr))
			{
				MARS_CORE_ERROR("FAILED TO START AUDIO PLAYBACK");
			}
		}

	private:
		struct AudioInfo
		{
			WAVEFORMATEXTENSIBLE wfx = {};
			XAUDIO2_BUFFER buffer = {};
			char* file_name;
		};

		HRESULT FindChunk(HANDLE file, DWORD fourcc, DWORD& chunk_size, DWORD& chunk_data_position)
		{
			HRESULT hr = S_OK;
			if (SetFilePointer(file, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}

			DWORD chunk_type;
			DWORD chunk_data_size;
			DWORD RIFF_data_size = 0;
			DWORD file_type;
			DWORD bytes_read = 0;
			DWORD offset = 0;

			while (hr == S_OK)
			{
				DWORD read;
				if (ReadFile(file, &chunk_type, sizeof(DWORD), &read, nullptr) == 0)
				{
					hr = HRESULT_FROM_WIN32(GetLastError());
				}
				if (ReadFile(file, &chunk_data_size, sizeof(DWORD), &read, nullptr) == 0)
				{
					hr = HRESULT_FROM_WIN32(GetLastError());
				}

				switch (chunk_type)
				{
				case fourccRIFF:
					RIFF_data_size = chunk_data_size;
					chunk_data_size = 4;
					if (ReadFile(file, &file_type, sizeof(DWORD), &read, nullptr))
					{
						hr = HRESULT_FROM_WIN32(GetLastError());
					}
					break;

				default:
					if (SetFilePointer(file, chunk_data_size, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
					{
						return HRESULT_FROM_WIN32(GetLastError());
					}
				}

				offset += 2 * sizeof(DWORD);

				if (chunk_type == fourcc)
				{
					chunk_size = chunk_data_size;
					chunk_data_position = offset;
					return S_OK;
				}

				offset += chunk_data_size;

				if (bytes_read >= RIFF_data_size)
				{
					return S_FALSE;
				}
			}

			return S_OK;
		}

		HRESULT ReadChunkData(HANDLE file, void* buffer, DWORD buffer_size, DWORD buffer_offset)
		{
			HRESULT hr = S_OK;
			if (SetFilePointer(file, buffer_offset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}

			DWORD read;
			if (ReadFile(file, buffer, buffer_size, &read, nullptr) == 0)
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}

			return hr;
		}

		HRESULT OpenAudioFile(AudioInfo& info)
		{
			HANDLE file = CreateFile(info.file_name, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);

			if (file == INVALID_HANDLE_VALUE)
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}
			if (SetFilePointer(file, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}

			DWORD chunk_size;
			DWORD chunk_position;
			FindChunk(file, fourccRIFF, chunk_size, chunk_position);

			DWORD file_type;
			ReadChunkData(file, &file_type, sizeof(DWORD), chunk_position);
			if (file_type != fourccWAVE)
			{
				return S_FALSE;
			}

			FindChunk(file, fourccFMT, chunk_size, chunk_position);
			ReadChunkData(file, &info.wfx, chunk_size, chunk_position);

			FindChunk(file, fourccDATA, chunk_size, chunk_position);
			BYTE* data = new BYTE[chunk_size];
			ReadChunkData(file, data, chunk_size, chunk_position);

			info.buffer.AudioBytes = chunk_size;
			info.buffer.pAudioData = data;
			info.buffer.Flags = XAUDIO2_END_OF_STREAM;

			return S_OK;
		}

		IXAudio2* audio = nullptr;
		IXAudio2MasteringVoice* master_voice = nullptr;
	};
}
