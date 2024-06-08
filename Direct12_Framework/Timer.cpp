#include "stdafx.h"
#include "Timer.h"

Timer::Timer()
{
	//퍼포먼스 카운터 빈도
	::QueryPerformanceFrequency((LARGE_INTEGER*)&performance_frequency_per_second_);
	//현재 카운터 수
	::QueryPerformanceCounter((LARGE_INTEGER*)&last_performance_counter_);
	time_scale_ = 1.0 / (double)performance_frequency_per_second_;

	base_performance_counter_ = last_performance_counter_;
	paused_performance_counter_ = 0;
	stop_performance_counter_ = 0;

	sample_count_ = 0;
	current_frame_rate_ = 0;
	frame_per_second = 0;
	fps_elapsed_time_ = 0.0f;

}
