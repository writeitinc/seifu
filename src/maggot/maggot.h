#ifndef maggot_h
#define maggot_h

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum MaggotStatus {
	MAGGOT_OK = 0,
	MAGGOT_WRAP = -1,
	MAGGOT_DIV_BY_ZERO = -2
} MaggotStatus;

typedef struct MaggotDivResult {
	size_t quot;
	size_t rem;
} MaggotDivResult;

static inline bool maggot_add_would_wrap(size_t a, size_t b);
static inline bool maggot_sub_would_wrap(size_t a, size_t b);
static inline bool maggot_mult_would_wrap(size_t a, size_t b);

static inline MaggotStatus maggot_add(size_t a, size_t b, size_t *ret_res);
static inline size_t maggot_add_bounded(size_t a, size_t b);

static inline MaggotStatus maggot_sub(size_t a, size_t b, size_t *ret_res);
static inline size_t maggot_sub_bounded(size_t a, size_t b);

static inline MaggotStatus maggot_mult(size_t a, size_t b, size_t *ret_res);
static inline size_t maggot_mult_bounded(size_t a, size_t b);

static inline MaggotStatus maggot_div(size_t a, size_t b,
		MaggotDivResult *ret_res);
static inline MaggotDivResult maggot_div_bounded(size_t a, size_t b);

static inline MaggotStatus maggot_div_floor(size_t a, size_t b,
		size_t *ret_res);
static inline MaggotStatus maggot_div_ceil(size_t a, size_t b, size_t *ret_res);
static inline MaggotStatus maggot_div_round(size_t a, size_t b,
		size_t *ret_res);

static inline size_t maggot_div_ceil_unchecked(size_t a, size_t b);
static inline size_t maggot_div_round_unchecked(size_t a, size_t b);

static inline bool maggot_add_would_wrap(size_t a, size_t b)
{
	return a > SIZE_MAX - b;
}

static inline bool maggot_sub_would_wrap(size_t a, size_t b)
{
	return b > a;
}

static inline bool maggot_mult_would_wrap(size_t a, size_t b)
{
	// TODO implement without division
	return b != 0 && a > SIZE_MAX / b;
}

static inline MaggotStatus maggot_add(size_t a, size_t b, size_t *ret_res)
{
	if (maggot_add_would_wrap(a, b)) {
		return MAGGOT_WRAP;
	}

	*ret_res = a + b;
	return MAGGOT_OK;
}

static inline size_t maggot_add_bounded(size_t a, size_t b)
{
	if (maggot_add_would_wrap(a, b)) {
		return SIZE_MAX;
	}

	return a + b;
}

static inline MaggotStatus maggot_sub(size_t a, size_t b, size_t *ret_res)
{
	if (maggot_sub_would_wrap(a, b)) {
		return MAGGOT_WRAP;
	}

	*ret_res = a - b;
	return MAGGOT_OK;
}

static inline size_t maggot_sub_bounded(size_t a, size_t b)
{
	if (maggot_sub_would_wrap(a, b)) {
		return 0;
	}

	return a - b;
}

static inline MaggotStatus maggot_mult(size_t a, size_t b, size_t *ret_res)
{
	if (maggot_mult_would_wrap(a, b)) {
		return MAGGOT_WRAP;
	}

	*ret_res = a * b;
	return MAGGOT_OK;
}

static inline size_t maggot_mult_bounded(size_t a, size_t b)
{
	if (maggot_mult_would_wrap(a, b)) {
		return SIZE_MAX;
	}

	return a * b;
}

static inline MaggotStatus maggot_div(size_t a, size_t b,
		MaggotDivResult *ret_res)
{
	if (b == 0) {
		return MAGGOT_DIV_BY_ZERO;
	}

	// TODO check if this gets optimized
	size_t quot = a / b;
	size_t rem = a % b;

	*ret_res = (MaggotDivResult){
		.quot = quot,
		.rem = rem
	};
	return MAGGOT_OK;
}

static inline MaggotDivResult maggot_div_bounded(size_t a, size_t b)
{
	if (b == 0) {
		return (MaggotDivResult){
			.quot = SIZE_MAX,
			.rem = 0
		};
	}

	// TODO check if this gets optimized
	size_t quot = a / b;
	size_t rem = a % b;

	return (MaggotDivResult){
		.quot = quot,
		.rem = rem
	};
}

static inline MaggotStatus maggot_div_floor(size_t a, size_t b, size_t *ret_res)
{
	if (b == 0) {
		return MAGGOT_DIV_BY_ZERO;
	}

	*ret_res = a / b;
	return MAGGOT_OK;
}

static inline MaggotStatus maggot_div_ceil(size_t a, size_t b, size_t *ret_res)
{
	if (b == 0) {
		return MAGGOT_DIV_BY_ZERO;
	}

	*ret_res = maggot_div_ceil_unchecked(a, b);
	return MAGGOT_OK;
}

static inline MaggotStatus maggot_div_round(size_t a, size_t b, size_t *ret_res)
{
	if (b == 0) {
		return MAGGOT_DIV_BY_ZERO;
	}

	*ret_res = maggot_div_round_unchecked(a, b);
	return MAGGOT_OK;
}

static inline size_t maggot_div_ceil_unchecked(size_t a, size_t b)
{
	size_t quot = a / b;
	size_t rem = a % b;

	return quot + (rem > 0);
}

static inline size_t maggot_div_round_unchecked(size_t a, size_t b)
{
	size_t quot = a / b;
	size_t rem = a % b;

	bool b_is_odd = b % 2 == 1;
	bool round_up = rem >= (b / 2 + b_is_odd);

	return quot + round_up;
}

#endif // maggot_h
