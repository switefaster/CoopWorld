#include "Timer.h"

#include <Windows.h>

Timer::Timer() :
    mSecondPerCount( 0.0 ),
    mDeltaTime( -1.0 ),
    mBaseTime( 0 ),
    mPausedTime( 0 ),
    mStopTime( 0 ),
    mPrevTime( 0 ),
    mCurrTime( 0 ),
    mPaused( false )
{
    __int64 countsPerSec;
    QueryPerformanceFrequency( ( LARGE_INTEGER* )&countsPerSec );
    mSecondPerCount = 1.0 / ( double )countsPerSec;
}

float Timer::Elapsed() const
{
    if ( mPaused )
    {
        return ( float )( ( ( mStopTime - mPausedTime ) - mBaseTime ) * mSecondPerCount );
    }

    return ( float )( ( ( mCurrTime - mPaused ) - mBaseTime ) * mSecondPerCount );
}

float Timer::Delta() const
{
    return ( float )mDeltaTime;
}

void Timer::Reset()
{
    __int64 currTime;
    QueryPerformanceCounter( ( LARGE_INTEGER* )&currTime );
    mBaseTime = currTime;
    mPrevTime = currTime;
    mStopTime = 0;
    mPaused = false;
}

void Timer::Resume()
{
    __int64 startTime;
    QueryPerformanceCounter( ( LARGE_INTEGER* )&startTime );

    if ( mPaused )
    {
        mPausedTime += ( startTime - mStopTime );
        mPrevTime = startTime;
        mStopTime = 0;
        mPaused = false;
    }
}

void Timer::Pause()
{
    if ( !mPaused )
    {
        __int64 currTime;
        QueryPerformanceCounter( ( LARGE_INTEGER* )&currTime );
        mStopTime = currTime;
        mPaused = true;
    }
}

void Timer::Tick()
{
    if ( mPaused )
    {
        mDeltaTime = 0.0;
        return;
    }

    __int64 currTime;
    QueryPerformanceCounter( ( LARGE_INTEGER* )&currTime );
    mCurrTime = currTime;
    mDeltaTime = ( mCurrTime - mPrevTime ) * mSecondPerCount;
    mPrevTime = mCurrTime;

    if ( mDeltaTime < 0.0 )
    {
        mDeltaTime = 0.0;
    }
}

