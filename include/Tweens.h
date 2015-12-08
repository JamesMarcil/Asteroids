#ifndef TWEENS_H
#define TWEENS_H

class Tweens
{
private:
    // Private/deleted to prevent instantiation.
    Tweens(void) = delete;
    ~Tweens(void) = delete;
    Tweens(const Tweens& rhs) = delete;
    Tweens& operator=(const Tweens& rhs) = delete;
    Tweens(Tweens&& rhs) = delete;
    Tweens& operator=(Tweens&& rhs) = delete;

public:
	static float MapRange(float value, float srcMin, float srcMax, float outMin, float outMax)
	{
		return (value - srcMin) / (srcMax - srcMin) * (outMax - outMin) + outMin;
	}

	static float EaseInEaseOut(float t)
	{
		if (t < 0.5)
		{
			return pow(t, 2);
		}
		else
		{
			return pow(1-t,2);
		}
	}

	static float EaseIn(float t)
	{
		return pow(t, 2);
	}

	static float EaseOut(float t)
	{
		return pow(1 - t, 2);
	}
};
#endif
