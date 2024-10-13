#define _CRT_SECURE_NO_WARNINGS
#include "glib/graphics/video/VideoPlayer.h"

#include <iostream>
#include <glad/glad.h>
#include <thread>
#include <chrono>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <GLFW/glfw3.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

namespace chrono = std::chrono;
using namespace glib;

#define FRAME_BUF_SIZE 16
#define AUDIO_FRAME_BUF_SIZE 8

extern ALCcontext* ALCCONTEXT;

namespace glib
{
	struct StreamInfo
	{
		AVStream* stream;
		int index;
	};

	struct Frame
	{
		unsigned int texture = 0;
		unsigned int buffer = 0;
		double time = 0.0;
		double duration = 0.0;
		bool used = false;
	};

	class Supplier
	{
	protected:
		AVFormatContext* m_FmtCtx;
		AVCodecContext* m_CodecCtx;
		StreamInfo m_StreamInfo;
	public:
		Supplier(AVFormatContext* fmtCtx, AVCodecContext* codecCtx, StreamInfo streamInfo) : m_FmtCtx(fmtCtx), m_CodecCtx(codecCtx), m_StreamInfo(streamInfo) {}
		virtual ~Supplier() {}

		virtual void Update(float delta) = 0;
	};

	class FrameSupplier : Supplier
	{
	private:
		AVPacket* m_Packet = nullptr;

		// Buffers
		AVFrame* m_AVFrameBuf1[FRAME_BUF_SIZE];
		AVFrame* m_AVFrameBuf2[FRAME_BUF_SIZE];
		Frame m_FrameBuf1[FRAME_BUF_SIZE];
		Frame m_FrameBuf2[FRAME_BUF_SIZE];

		// Active / Inactive buffers
		AVFrame** m_ActiveAVFrameBuf = nullptr;
		AVFrame** m_InactiveAVFrameBuf = nullptr;
	public:
		Frame* m_ActiveFrameBuf = nullptr;
		bool m_HasProcessedFrames = false;
	private:
		Frame* m_InactiveFrameBuf = nullptr;

		// SWS
		SwsContext* m_SWSCtx = nullptr;
		AVFrame* m_ConvertedFrame = nullptr;

		// Reading Thread
		bool m_Running = true;
		bool m_ShouldRead = true;
		bool m_FinishedReading = false;
		std::thread m_ReadingThread;

		bool m_Swapped = false;
	public:
		FrameSupplier(AVFormatContext* fmtCtx, AVCodecContext* codecCtx, StreamInfo streamInfo) : Supplier(fmtCtx, codecCtx, streamInfo)
		{
			Setup();
		}

		~FrameSupplier() override
		{
			Clean();
		}

		void Setup()
		{
			m_Packet = av_packet_alloc();
			m_ConvertedFrame = av_frame_alloc();

			FillAVFrameBuffer(m_AVFrameBuf1);
			FillAVFrameBuffer(m_AVFrameBuf2);

			FillFrameBuffer(m_FrameBuf1);
			FillFrameBuffer(m_FrameBuf2);

			m_InactiveAVFrameBuf = m_AVFrameBuf1;
			m_InactiveFrameBuf = m_FrameBuf1;

			m_ActiveAVFrameBuf = m_AVFrameBuf2;
			m_ActiveFrameBuf = m_FrameBuf2;

			m_ReadingThread = std::thread([&]() {
				RunReadingThread();
			});

			while (m_ShouldRead) std::this_thread::yield();
			while (m_FinishedReading) std::this_thread::yield();

			Swap();
		}

		void FillAVFrameBuffer(AVFrame** buf)
		{
			for (int i = 0; i < FRAME_BUF_SIZE; i++)
			{
				buf[i] = av_frame_alloc();
			}
		}

		void FillFrameBuffer(Frame* buf)
		{
			for (int i = 0; i < FRAME_BUF_SIZE; i++)
			{
				buf[i] = {};
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glGenTextures(1, &buf[i].texture);
			}
		}

		void FreeAVFrameBuffer(AVFrame** buf)
		{
			for (int i = 0; i < FRAME_BUF_SIZE; i++)
			{
				av_frame_free(&buf[i]);
			}
		}

		void FreeFrameBuffer(Frame* buf)
		{
			for (int i = 0; i < FRAME_BUF_SIZE; i++)
			{
				glDeleteTextures(1, &buf[i].texture);
			}
		}

		void Clean()
		{
			sws_freeContext(m_SWSCtx);
			m_Running = false;
			m_ReadingThread.join();

			FreeAVFrameBuffer(m_AVFrameBuf1);
			FreeAVFrameBuffer(m_AVFrameBuf2);

			FreeFrameBuffer(m_FrameBuf1);
			FreeFrameBuffer(m_FrameBuf2);

			av_packet_free(&m_Packet);
			av_frame_free(&m_ConvertedFrame);
		}

		bool ReadAVFrame(AVFrame* frame)
		{
			if (av_read_frame(m_FmtCtx, m_Packet) != 0) return false;
			if (m_Packet->stream_index != m_StreamInfo.index) return false;

			int response = avcodec_send_packet(m_CodecCtx, m_Packet);

			response = avcodec_receive_frame(m_CodecCtx, frame);
			if (response == AVERROR(EAGAIN)) {
				return ReadAVFrame(frame);
			}

			return response >= 0;
		}

		void RunReadingThread()
		{
			while (m_Running)
			{
				std::this_thread::sleep_for(chrono::microseconds(1));
				if (!m_ShouldRead || m_FinishedReading) continue;
				m_ShouldRead = false;
				m_HasProcessedFrames = true;

				int i = 0;
				int runs = 0;
				while (i < FRAME_BUF_SIZE && m_Running)
				{
					AVFrame* frame = m_InactiveAVFrameBuf[i];
					if (ReadAVFrame(frame))
					{
						if (m_SWSCtx == nullptr)
						{
							m_SWSCtx = sws_getContext(frame->width, frame->height,
								(AVPixelFormat)frame->format, frame->width, frame->height,
								AV_PIX_FMT_RGB24, SWS_FAST_BILINEAR, 0, 0, 0);
						}
						i++;
					}
					runs++;
					if (runs >= FRAME_BUF_SIZE * 10)
					{
						break;
					}
				}

				m_FinishedReading = true;
			}
		}

		void Update(float delta) override
		{
			if (m_FinishedReading)
			{
				for (int i = 0; i < FRAME_BUF_SIZE; i++)
				{
					Frame& frame = m_InactiveFrameBuf[i];
					AVFrame* avFrame = m_InactiveAVFrameBuf[i];

					frame.used = false;
					
					AVRational time_base = m_StreamInfo.stream->time_base;
					int64_t timestamp_ms = av_rescale_q(avFrame->pts, time_base, { 1, 1000 });
					frame.time = timestamp_ms;

					glBindTexture(GL_TEXTURE_2D, frame.texture);

					sws_scale_frame(m_SWSCtx, m_ConvertedFrame, avFrame);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

					if (GL_EXT_texture_filter_anisotropic) {
						GLfloat largest;
						glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
					}

					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_ConvertedFrame->width, m_ConvertedFrame->height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_ConvertedFrame->data[0]);

					glGenerateMipmap(GL_TEXTURE_2D);
				}

				glBindTexture(GL_TEXTURE_2D, 0);
				m_FinishedReading = false;
			}
		}

		void Swap()
		{
			if (m_Swapped)
			{
				m_ActiveAVFrameBuf = m_AVFrameBuf1;
				m_ActiveFrameBuf = m_FrameBuf1;
				m_InactiveAVFrameBuf = m_AVFrameBuf2;
				m_InactiveFrameBuf = m_FrameBuf2;
			}
			else
			{
				m_ActiveAVFrameBuf = m_AVFrameBuf2;
				m_ActiveFrameBuf = m_FrameBuf2;
				m_InactiveAVFrameBuf = m_AVFrameBuf1;
				m_InactiveFrameBuf = m_FrameBuf1;
			}
			m_Swapped = !m_Swapped;
		}

		void RequestMoreFrames()
		{
			if (m_ShouldRead) return;
			m_ShouldRead = true;
			m_HasProcessedFrames = false;
		}
	};

	class AudioSupplier : public Supplier
	{
	public:
		unsigned int m_Source = 0;
	private:
		AVPacket* m_Packet = nullptr;

		// Buffers
		AVFrame* m_AVFrameBuf1[AUDIO_FRAME_BUF_SIZE];
		AVFrame* m_AVFrameBuf2[AUDIO_FRAME_BUF_SIZE];
		unsigned int m_Bufs[AUDIO_FRAME_BUF_SIZE];

		// Active / Inactive buffers
		AVFrame** m_ActiveAVFrameBuf = nullptr;
		AVFrame** m_InactiveAVFrameBuf = nullptr;

		// SWR
		SwrContext* m_SWRCtx = nullptr;
		AVFrame* m_ConvertedFrame = nullptr;

		// Reading Thread
		bool m_Running = true;
		bool m_ShouldRead = true;
		bool m_FinishedReading = false;
		std::thread m_ReadingThread;

		bool m_Swapped = true;

		int m_FrameBufIndex = 0;
	public:
		AudioSupplier(AVFormatContext* fmtCtx, AVCodecContext* codecCtx, StreamInfo streamInfo) : Supplier(fmtCtx, codecCtx, streamInfo)
		{
			Setup();
		}

		~AudioSupplier() override
		{
			Clean();
		}

		void Setup()
		{
			m_Packet = av_packet_alloc();
			m_ConvertedFrame = av_frame_alloc();

			FillAVFrameBuffer(m_AVFrameBuf1);
			FillAVFrameBuffer(m_AVFrameBuf2);

			FillFrameBuffer(m_Bufs);

			alGenSources(1, &m_Source);

			m_InactiveAVFrameBuf = m_AVFrameBuf1;
			m_ActiveAVFrameBuf = m_AVFrameBuf2;

			m_ReadingThread = std::thread([&]() {
				RunReadingThread();
			});

			while (m_ShouldRead) std::this_thread::sleep_for(chrono::microseconds(1));

			Swap();
			m_ShouldRead = true;

			for (int i = 0; i < AUDIO_FRAME_BUF_SIZE; i++)
			{
				RefillBuffer(m_Bufs[i]);
			}

			alSourceQueueBuffers(m_Source, AUDIO_FRAME_BUF_SIZE, m_Bufs);
			alSourcePlay(m_Source);
		}

		void FillAVFrameBuffer(AVFrame** buf)
		{
			for (int i = 0; i < AUDIO_FRAME_BUF_SIZE; i++)
			{
				buf[i] = av_frame_alloc();
			}
		}

		void FillFrameBuffer(unsigned int* buf)
		{
			for (int i = 0; i < AUDIO_FRAME_BUF_SIZE; i++)
			{
				alGenBuffers(1, &buf[i]);
			}
		}

		void FreeAVFrameBuffer(AVFrame** buf)
		{
			for (int i = 0; i < AUDIO_FRAME_BUF_SIZE; i++)
			{
				av_frame_free(&buf[i]);
			}
		}

		void FreeFrameBuffer(unsigned int* buf)
		{
			for (int i = 0; i < AUDIO_FRAME_BUF_SIZE; i++)
			{
				alDeleteBuffers(1, &buf[i]);
			}
		}

		void Clean()
		{
			swr_free(&m_SWRCtx);
			m_Running = false;
			m_ReadingThread.join();

			alDeleteSources(1, &m_Source);

			FreeAVFrameBuffer(m_AVFrameBuf1);
			FreeAVFrameBuffer(m_AVFrameBuf2);

			FreeFrameBuffer(m_Bufs);

			av_packet_free(&m_Packet);
			av_frame_free(&m_ConvertedFrame);
		}

		bool ReadAVFrame(AVFrame* frame)
		{
			if (av_read_frame(m_FmtCtx, m_Packet) != 0) return false;
			if (m_Packet->stream_index != m_StreamInfo.index) return false;

			int response = avcodec_send_packet(m_CodecCtx, m_Packet);

			response = avcodec_receive_frame(m_CodecCtx, frame);
			if (response == AVERROR(EAGAIN)) {
				ReadAVFrame(frame);
				return false;
			}
			if (response >= 0)
			{
				return true;
			}

			return false;
		}

		void RunReadingThread()
		{
			while (m_Running)
			{
				std::this_thread::sleep_for(chrono::microseconds(1));
				if (!m_ShouldRead) continue;

				int i = 0;
				while (i < AUDIO_FRAME_BUF_SIZE && m_Running)
				{
					AVFrame* frame = m_InactiveAVFrameBuf[i];

					if (ReadAVFrame(frame))
					{
						if (m_SWRCtx == nullptr)
						{
							m_SWRCtx = swr_alloc();

							av_opt_set_chlayout(m_SWRCtx, "in_chlayout", &frame->ch_layout, 0);
							av_opt_set_int(m_SWRCtx, "in_sample_rate", frame->sample_rate, 0);
							av_opt_set_sample_fmt(m_SWRCtx, "in_sample_fmt", (AVSampleFormat)frame->format, 0);

							av_opt_set_chlayout(m_SWRCtx, "out_chlayout", &m_CodecCtx->ch_layout, 0);
							av_opt_set_int(m_SWRCtx, "out_sample_rate", m_CodecCtx->sample_rate, 0);
							av_opt_set_sample_fmt(m_SWRCtx, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);

							swr_init(m_SWRCtx);
						}
						i++;
					}
				}

				m_ShouldRead = false;
			}
		}

		void RefillBuffer(unsigned int buf)
		{
			if (m_FrameBufIndex >= AUDIO_FRAME_BUF_SIZE) return;

			AVFrame* frame = m_ActiveAVFrameBuf[m_FrameBufIndex];
			m_ConvertedFrame->ch_layout = m_CodecCtx->ch_layout;
			m_ConvertedFrame->sample_rate = m_CodecCtx->sample_rate;
			m_ConvertedFrame->format = AV_SAMPLE_FMT_S16;

			swr_convert_frame(m_SWRCtx, m_ConvertedFrame, frame);

			int bufferSize = av_samples_get_buffer_size(NULL, m_ConvertedFrame->ch_layout.nb_channels,
				m_ConvertedFrame->nb_samples,
				(AVSampleFormat)m_ConvertedFrame->format, 1);

			alBufferData(buf, AL_FORMAT_STEREO16, m_ConvertedFrame->data[0], bufferSize, m_CodecCtx->sample_rate);

			m_FrameBufIndex++;
		}

		void Update(float delta) override
		{
			ALint processed;
			alGetSourcei(m_Source, AL_BUFFERS_PROCESSED, &processed);

			while (processed > 0) {
				ALuint buffer;
				alSourceUnqueueBuffers(m_Source, 1, &buffer);
				if (m_FrameBufIndex >= AUDIO_FRAME_BUF_SIZE)
				{
					Swap();
					m_ShouldRead = true;
					m_FrameBufIndex = 0;
				}

				RefillBuffer(buffer);
				alSourceQueueBuffers(m_Source, 1, &buffer);
				processed--;
			}

			ALint state;
			alGetSourcei(m_Source, AL_SOURCE_STATE, &state);
			if (state != AL_PLAYING) {
				alSourcePlay(m_Source);
			}
		}

		void Swap()
		{
			if (m_Swapped)
			{
				m_ActiveAVFrameBuf = m_AVFrameBuf1;
				m_InactiveAVFrameBuf = m_AVFrameBuf2;
			}
			else
			{
				m_ActiveAVFrameBuf = m_AVFrameBuf2;
				m_InactiveAVFrameBuf = m_AVFrameBuf1;
			}
			m_Swapped = !m_Swapped;
		}

		void RequestMoreFrames()
		{
			if (m_ShouldRead) return;
			m_ShouldRead = true;
		}
	};

	using namespace std::chrono;

	class VideoPlayerImpl
	{
	private:
		std::string m_Path;

		FrameSupplier* m_FrameSupplier = nullptr;
		AudioSupplier* m_AudioSupplier = nullptr;

		float m_FPS = 0.0f;
		bool m_Opened = false;
		double m_Duration = 0.0f;
		double m_Elapsed = 0.0f;
		bool m_Finished = false;
		float m_LoadTime = 0.0f;

		AVCodecContext* m_VideoCtx = nullptr;
		AVStream* m_VideoStream = nullptr;
		AVCodecContext* m_AudioCtx = nullptr;
		AVStream* m_AudioStream = nullptr;
		AVFormatContext* m_VCtx = nullptr;
		AVFormatContext* m_ACtx = nullptr;
		int m_VideoStreamIdx = -1;
		int m_AudioStreamIdx = -1;
	public:
		unsigned int m_CurrentTexture = 0;
	public:
		VideoPlayerImpl(const std::string& path) : m_Path(path)
		{
		}

		~VideoPlayerImpl()
		{
			if (m_Opened)
			{
				CloseFile();
			}
		}

		void OpenFile()
		{
			milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

			m_VCtx = avformat_alloc_context();
			avformat_open_input(&m_VCtx, m_Path.c_str(), NULL, NULL);
			avformat_find_stream_info(m_VCtx, NULL);

			FindCodec(m_VCtx, true);

			m_ACtx = avformat_alloc_context();
			avformat_open_input(&m_ACtx, m_Path.c_str(), NULL, NULL);
			avformat_find_stream_info(m_ACtx, NULL);

			FindCodec(m_ACtx, false);

			m_Duration = m_VCtx->duration / (AV_TIME_BASE / 1000.0);

			m_FrameSupplier = new FrameSupplier(m_VCtx, m_VideoCtx, { m_VideoStream, m_VideoStreamIdx });
			m_AudioSupplier = new AudioSupplier(m_ACtx, m_AudioCtx, { m_AudioStream, m_AudioStreamIdx });

			milliseconds end = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
			m_LoadTime = end.count() - start.count();
			m_Elapsed = -m_LoadTime;
			m_Opened = true;
		}

		void CloseFile()
		{
			m_Opened = false;

			delete m_FrameSupplier;
			m_FrameSupplier = nullptr;
			avformat_close_input(&m_VCtx);
			avcodec_free_context(&m_VideoCtx);

			delete m_AudioSupplier;
			m_AudioSupplier = nullptr;
			avformat_close_input(&m_ACtx);
			avcodec_free_context(&m_AudioCtx);
		}

		void FindCodec(AVFormatContext* avFmtCtx, bool videoCtx)
		{
			for (int i = 0; i < avFmtCtx->nb_streams; i++)
			{
				AVStream* stream = avFmtCtx->streams[i];
				AVCodecParameters* codecParams = stream->codecpar;
				const AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);

				if (codecParams->codec_type == AVMEDIA_TYPE_VIDEO && videoCtx) {
					m_VideoStreamIdx = i;
					m_VideoStream = stream;

					m_VideoCtx = avcodec_alloc_context3(codec);
					avcodec_parameters_to_context(m_VideoCtx, codecParams);
					avcodec_open2(m_VideoCtx, codec, NULL);
					return;
				}
				else if (codecParams->codec_type == AVMEDIA_TYPE_AUDIO && !videoCtx) {
					m_AudioStreamIdx = i;
					m_AudioStream = stream;

					m_AudioCtx = avcodec_alloc_context3(codec);
					avcodec_parameters_to_context(m_AudioCtx, codecParams);
					avcodec_open2(m_AudioCtx, codec, NULL);
					return;
				}
			}
		}

		void Play()
		{
			m_Finished = false;
			m_Elapsed = 0.0;
			OpenFile();
		}

		void Pause()
		{

		}

		void Resume()
		{

		}

		void Stop()
		{
			CloseFile();
		}

		void Update(float delta)
		{
			if (!m_Opened) return;

			m_FrameSupplier->Update(delta);
			m_AudioSupplier->Update(delta);

			if (m_FrameSupplier->m_HasProcessedFrames)
			{
				int usedInUpdate = 0;
				for (int i = 0; i < FRAME_BUF_SIZE; i++)
				{
					Frame& frame = m_FrameSupplier->m_ActiveFrameBuf[i];
					if (m_Elapsed >= frame.time && !frame.used)
					{
						frame.used = true;
						m_CurrentTexture = frame.texture;
					}
					else if (frame.used)
					{
						usedInUpdate++;
					}
				}

				if (usedInUpdate >= FRAME_BUF_SIZE)
				{
					m_FrameSupplier->Swap();
					m_FrameSupplier->RequestMoreFrames();
				}
			}

			m_Elapsed += delta;

			if (m_Elapsed >= m_Duration)
			{
				m_Finished = true;
				Stop();
				return;
			}
		}

		bool IsFinished()
		{
			return m_Finished;
		}

		void SetVolume(float volume)
		{
			alSourcef(m_AudioSupplier->m_Source, AL_GAIN, volume);
		}

		float GetTimePosition()
		{
			return m_Elapsed;
		}
	};
}

glib::VideoPlayer::VideoPlayer(const std::string& path)
	: pos(Vec2(0.0f, 0.0f)), size(Vec2(0.0f, 0.0f)), scale(Vec2(1.0f, 1.0f)), rotation(0.0f)
{
	visible = true;
	impl = new VideoPlayerImpl(path);
}

glib::VideoPlayer::~VideoPlayer()
{
	delete impl;
}

void glib::VideoPlayer::Play()
{
	impl->Play();
}

void glib::VideoPlayer::Pause()
{
	impl->Pause();
}

void glib::VideoPlayer::Resume()
{
	impl->Resume();
}

void glib::VideoPlayer::Stop()
{
	impl->Stop();
}

void glib::VideoPlayer::Draw()
{
	glBindTexture(GL_TEXTURE_2D, impl->m_CurrentTexture);
}

void glib::VideoPlayer::Update(float delta)
{
	impl->Update(delta);
}

bool glib::VideoPlayer::IsFinished()
{
	return impl->IsFinished();
}

void glib::VideoPlayer::SetVolume(float volume)
{
	impl->SetVolume(volume);
}

float glib::VideoPlayer::GetTimePosition()
{
	return impl->GetTimePosition();
}
