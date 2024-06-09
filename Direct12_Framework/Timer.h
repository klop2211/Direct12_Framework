#pragma once

const ULONG kMaxSampleCount = 50; // Maximum frame time sample count

// 시간을 측정하고 관리하는 클래스
class Timer
{
public:
	Timer();
	~Timer() {}

	int current_frame_rate() const { return current_frame_rate_; }
	float elapsed_time() const { return elapsed_time_; }

	//GameFramework의 FrameAdvence함수에 맨 처음 호출 되며 매개변수로 fps를 설정할 수 있다. \
	fps 상한을 없애려면 0이하의 값을 넣으면 된다.
	void Tick(float frame_per_second = 0.f);

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
				
	std::array<float, kMaxSampleCount> frame_time_;
	ULONG sample_count_;

	unsigned long current_frame_rate_;
	unsigned long frame_per_second_;
	float fps_elapsed_time_;
	
	bool is_stop_;

};

