#ifndef MATHLIB_H
#define MATHLIB_H

#include "mathlib_global.h"
#include <vector>

class MATHLIBSHARED_EXPORT Effect
{
public:
    Effect();
    void set_amps(std::vector<int> amps);
    virtual ~Effect() = 0;
    std::vector<int> get_amps();
    virtual void apply_effect() = 0;
    std::vector<int> amplitudes;
};

class MATHLIBSHARED_EXPORT Distortion : public Effect
{
public:
    Distortion();
    void set_distort_val(int value);
    int get_distort_val();
    void apply_effect();
private:
    int distortion_val;
};

#endif // MATHLIB_H
