#define PI 3.1415F
#define PI180 0.01745F
#define PI2 9.8696F
#define IPI 0.3183F
#define SINCSTM -0.1165401F
#define SINH2 1.57079F

#include <stdio.h>
#include <emscripten.h>

// -125 < n < 125
float sbnf(float x, int n)
{
    union
    {
        float f;
        uint32_t i;
    } u;
    u.i = (uint32_t)(0x7f - n) << 23;
    return x * u.f;
}

// max deviation <= 0.0013
float sinef(float x)
{
    if ((x > 0 && x < SINCSTM) || (x > PI - SINCSTM && x < PI))
    {
        return x;
    }
    if (x < 0 || x > PI)
    {
        return -sinef(x + PI * (x < 0 ? 1 : -1));
    }

    float y = x * (1.27324 - 0.405285 * fabs(x));
    return (0.225 * y + 0.775) * y;
}

float cosef(float x)
{
    return sinef(x + SINH2);
}

float gtor(float a)
{
    return a * PI180;
}

// ra[0] ra[1] dec[0] dec[1]
EMSCRIPTEN_KEEPALIVE
float calcCoords(float *a)
{
    float r = gtor(sbnf(-86400.0F * a[0] - a[1] + 1440.0F, 2));
    float d = gtor(a[2] + sbnf(a[3], 6));

    float coords[3];

    coords[0] = sbnf(cosef(d - r) + cosef(d + r), 1);
    coords[1] = sinef(d);
    coords[2] = cosef(d) * sinef(r);

    return ;
}
