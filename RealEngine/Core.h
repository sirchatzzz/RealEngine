#pragma once
class ViewPort {
public:

	int x;
	int y;
	int width;
	int height;

	inline float GetAspectRatio() {
		return static_cast<float>(width) / static_cast<float>(height);
	}
	inline operator const int* () const {
		return static_cast<const int*>(&x);
	}
	inline operator int* () {
		return static_cast<int*>(&x);
	}
};

