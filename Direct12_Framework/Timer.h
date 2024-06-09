#pragma once

const ULONG kMaxSampleCount = 50; // Maximum frame time sample count

// �ð��� �����ϰ� �����ϴ� Ŭ����
class Timer
{
public:
	Timer();
	~Timer() {}

	//GameFramework�� FrameAdvence�Լ��� �� ó�� ȣ�� �Ǹ� �Ű������� fps�� ������ �� �ִ�. \
	fps ������ ���ַ��� 0������ ���� ������ �ȴ�.
	void Tick(float frame_per_second);

private:
	// ī���� �� * time_scale = x ��
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

