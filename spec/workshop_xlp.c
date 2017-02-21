#include <stdio.h>

/***********************************************************
 * Load configuration specific to PLP
 *
 * Edit these defines to change the results of the
 * partitioning.
 *
 * */
#define PI_ACCURACY_PLP         1000
#define TRIGO_ACCURACY_PLP        10
#define WORKER_LOAD_PLP           20
#define MAIN_LOAD                400    /* DLP or PLP */

/***********************************************************
 * Load configuration specific to DLP
 *
 * Edit these defines to change the results of the
 * partitioning.
 *
 * */
#define PI_ACCURACY_DLP          200
#define LOAD_DLP                 400

float calculate_pi()
{
    float pi = 1.0;

    float sign = -1;

    for (int i = 1; i < PI_ACCURACY_PLP; i++)
    {
        pi += sign * (1.0 / (1.0 + 2.0 * i));
        sign *= -1;

    }
    pi = 4 * pi;

    return pi;
}

float maps_sin(float x)
{
    float acc_fact = 1;

    float acc_x_pow = x;

    float acc_sin = x;

    float sign = -1;

    for (int i = 1; i < TRIGO_ACCURACY_PLP; i++)
    {
        acc_fact *= (2 * i + 1) * (2 * i);
        acc_x_pow *= x * x;
        acc_sin += sign * acc_x_pow / acc_fact;
        sign *= -1;
    }

    return acc_sin;
}

float maps_cos(float x)
{
    float acc_fact = 1;

    float acc_x_pow = 1;

    float acc_cos = 1;

    float sign = -1;

    for (int i = 1; i < TRIGO_ACCURACY_PLP; i++)
    {
        acc_fact *= (2 * i) * (2 * i - 1);
        acc_x_pow *= x * x;
        acc_cos += sign * acc_x_pow / acc_fact;
        sign *= -1;
    }

    return acc_cos;
}

float doSomeWork(float x)
{
    float tmp = 0.0;

    for (int i = 0; i < WORKER_LOAD_PLP; i++)
    {
        tmp += maps_cos(x) / (i + 1) + maps_sin(x) / (i + 1);
    }

    return tmp;
}

float doSomeMoreWork(float x)
{
    float tmp = 0.0;

    for (int i = 0; i < (3 * WORKER_LOAD_PLP / 2); i++)
    {
        tmp += maps_sin(x) / (i + 1);
    }

    return tmp;
}

int main(void)
{
    float x = 0.0;

    float y = 0.0;

    float z = 0.0;

    float a = 9.2;

    for (int i = 0; i < MAIN_LOAD; i++)
    {
        x = calculate_pi();
        y = doSomeWork(x / 2);
        y = y + a;
        z = doSomeMoreWork(y / 4);
    }

    printf("z=%f\n", z);

    float A[LOAD_DLP] = { 0.0 };

    for (int i = 0; i < LOAD_DLP; i++)
    {
        A[i] = calculate_pi();
    }

    float acc = 0;

    for (int i = 0; i < LOAD_DLP; i++)
    {
        acc += A[i];
    }
    printf("Acc: %f\n", acc);

    return 0;
}
