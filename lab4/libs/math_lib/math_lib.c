#include "math_lib.h"

#include <math.h>

float cos_derivative_method1(float a, float dx) {
    return (cosf(a + dx) - cosf(a)) / dx;
}

float cos_derivative_method2(float a, float dx) {
    return (cosf(a + dx) - cosf(a - dx)) / (2 * dx);
}
