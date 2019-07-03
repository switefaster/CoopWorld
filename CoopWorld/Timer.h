#pragma once

#ifndef _TIMER_H_
#define _TIMER_H_

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

#endif
