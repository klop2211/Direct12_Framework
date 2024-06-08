#pragma once

const ULONG MAX_SAMPLE_COUNT = 50; // Maximum frame time sample count

class Timer
{
public:
	Timer();
	~Timer() {}

private:
	// 카운터 수 * time_scale = x 초
	double time_scale_;

	float elapsed_time_;

	__int64 base_performance_counter_;
	__int64 paused_performance_counter_;
	__int64	stop_performance_counter_;
	__int64	current_performance_counter_;
	__int64	last_performance_counter_;
				
	__int64	performance_frequency_per_second_;
				
	float frame_time_[MAX_SAMPLE_COUNT];
	ULONG sample_count_;
				
	unsigned long current_frame_rate_;
	unsigned long frame_per_second;
	float fps_elapsed_time_;
				
	bool is_stop_;

};

