#ifndef BIGINT_H
#define BIGINT_H

typedef char int8_t;
typedef unsigned char uint8_t;

typedef struct bigint
{
    /* 0 : most significative, 2 : less significative */
    int8_t digit_0;
    int8_t digit_1;
    int8_t digit_2;

    /* auxiliary: if 1 <=> value is +inf, if -1 <=> value is +inf, if 0 <=> value is finite */
    int8_t inf;
} bigint_t;

uint8_t less_than(bigint_t const a, bigint_t const b);

uint8_t greater_than(bigint_t const a, bigint_t const b);

uint8_t equals(bigint_t const a, bigint_t const b);

bigint_t from_digits(int8_t digit_0, int8_t digit_1, int8_t digit_2);

bigint_t positive_infinity();

bigint_t negative_infinity();

#endif /* BIGINT_H */