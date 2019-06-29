#pragma once

class Timer {
    public:
        Timer();

        float Elapsed() const;
        float Delta() const;
		float CurrentTime() const;

        void Reset();
        void Resume();
        void Pause();
        void Tick();

    private:
        double mSecondPerCount;
        double mDeltaTime;

        __int64 mBaseTime;
        __int64 mPausedTime;
        __int64 mStopTime;
        __int64 mPrevTime;
        __int64 mCurrTime;

        bool mPaused;
};

