#include "bigint.h"

uint8_t less_than(bigint_t const a, bigint_t const b)
{
    /* if both are infinite */
    if (a.inf != 0 && b.inf != 0)
        return a.inf < b.inf;

    /* if a is infinite but b is finite */
    else if (a.inf != 0 && b.inf == 0)
        return a.inf < 0;

    /* if a is finite but b is infinite */
    else if (a.inf == 0 && b.inf != 0)
        return b.inf > 0;

    /* if both are finite compare the whole number */
    else
        return a.digit_0 < b.digit_0 ||
               a.digit_0 == b.digit_0 && a.digit_1 < b.digit_1 ||
               a.digit_0 == b.digit_0 && a.digit_1 == b.digit_1 && a.digit_2 < b.digit_2;
}

uint8_t greater_than(bigint_t const a, bigint_t const b)
{
    /* if both are infinite */
    if (a.inf != 0 && b.inf != 0)
        return a.inf > b.inf;

    /* if a is infinite but b is finite */
    else if (a.inf != 0 && b.inf == 0)
        return a.inf > 0;

    /* if a is finite but b is infinite */
    else if (a.inf == 0 && b.inf != 0)
        return b.inf < 0;

    /* if both are finite compare the whole number */
    else
        return a.digit_0 > b.digit_0 ||
               a.digit_0 == b.digit_0 && a.digit_1 > b.digit_1 ||
               a.digit_0 == b.digit_0 && a.digit_1 == b.digit_1 && a.digit_2 > b.digit_2;
}

uint8_t equals(bigint_t const a, bigint_t const b)
{
    /* if both are infinite */
    if (a.inf != 0 && b.inf != 0)
        return a.inf == b.inf;

    /* else compare the whole number */
    else
        return a.inf == b.inf &&
               a.digit_0 == b.digit_0 &&
               a.digit_1 == b.digit_1 &&
               a.digit_2 == b.digit_2;
}

bigint_t from_digits(int8_t digit_0, int8_t digit_1, int8_t digit_2)
{
    bigint_t i = {0, 0, 0, 0};
    i.digit_0 = digit_0;
    i.digit_1 = digit_1;
    i.digit_2 = digit_2;
    return i;
}

bigint_t positive_infinity()
{
    bigint_t i = {0, 0, 0, 1};
    return i;
}

bigint_t negative_infinity()
{
    bigint_t i = {0, 0, 0, -1};
    return i;
}