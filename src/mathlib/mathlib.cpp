#include "mathlib.h"


Effect::Effect()
{
}

Effect::~Effect()
{
}

void Effect::set_amps(std::vector<int> amps)
{
    this->amplitudes = amps;
}

std::vector<int> Effect::get_amps()
{
    return this->amplitudes;
}

void Effect::apply_effect()
{
}

Distortion::Distortion()
{
}

void Distortion::set_distort_val(int value)
{
    this->distortion_val = value;
}

int Distortion::get_distort_val()
{
    return this->distortion_val;
}

void Distortion::apply_effect()
{
    int len = amplitudes.size();
    for (int i = 0; i < len; ++i)
    {
        if (amplitudes[i] > distortion_val)
            amplitudes[i] = distortion_val;
        else if (amplitudes[i] < -distortion_val)
            amplitudes[i] = -distortion_val;
    }
}
