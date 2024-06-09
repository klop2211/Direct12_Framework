#include "stdafx.h"
#include "Timer.h"

Timer::Timer()
{
	//�����ս� ī���� ��
	::QueryPerformanceFrequency((LARGE_INTEGER*)&performance_frequency_per_second_);
	//���� ī���� ��
	::QueryPerformanceCounter((LARGE_INTEGER*)&last_performance_counter_);
	time_scale_ = 1.0 / (double)performance_frequency_per_second_;

	base_performance_counter_ = last_performance_counter_;
	paused_performance_counter_ = 0;
	stop_performance_counter_ = 0;

	sample_count_ = 0;
	current_frame_rate_ = 0;
	frame_per_second_ = 0;
	fps_elapsed_time_ = 0.0f;

}

void Timer::Tick(float frame_per_second)
{
	if (is_stop_)
	{
		elapsed_time_ = 0.f;
		return;
	}

	// �����Ӱ� �ð���
	float elapsed_time;

	::QueryPerformanceCounter((LARGE_INTEGER*)&current_performance_counter_);
	elapsed_time = static_cast<float>((current_performance_counter_ - last_performance_counter_) * time_scale_);

	// ���� fps�� �°� ���ư��Բ� ���
	if (frame_per_second > 0.f)
	{
		while (elapsed_time < (1.f / frame_per_second))
		{
			::QueryPerformanceCounter((LARGE_INTEGER*)&current_performance_counter_);
			elapsed_time = static_cast<float>((current_performance_counter_ - last_performance_counter_) * time_scale_);
		}
	}

	last_performance_counter_ = current_performance_counter_;

	// ��� frame_time�� ���ϱ� ���� ���� ����
	if (fabsf(elapsed_time - elapsed_time_) < 1.f)
	{
		::memmove(&frame_time_[1], frame_time_.data(), (kMaxSampleCount - 1) * sizeof(float));
		frame_time_[0] = elapsed_time;
		if (sample_count_ < kMaxSampleCount)
		{
			sample_count_++;
		}
	}

	frame_per_second_++;
	fps_elapsed_time_ += elapsed_time;
	if (fps_elapsed_time_ > 1.f)
	{
		current_frame_rate_ = frame_per_second_;

	}
}
