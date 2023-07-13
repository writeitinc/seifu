#ifndef maggot_h
#define maggot_h

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

static inline MaggotStatus maggot_add(size_t a, size_t b, size_t *ret_res);
static inline MaggotStatus maggot_sub(size_t a, size_t b, size_t *ret_res);
static inline MaggotStatus maggot_mult(size_t a, size_t b, size_t *ret_res);
static inline MaggotStatus maggot_div(size_t a, size_t b,
		MaggotDivResult *ret_res);

static inline MaggotStatus maggot_add(size_t a, size_t b, size_t *ret_res)
{
	if (a > SIZE_MAX - b) {
		return MAGGOT_WRAP;
	}

	*ret_res = a + b;
	return MAGGOT_OK;
}

static inline MaggotStatus maggot_sub(size_t a, size_t b, size_t *ret_res)
{
	if (b > a) {
		return MAGGOT_WRAP;
	}

	*ret_res = a - b;
	return MAGGOT_OK;
}

static inline MaggotStatus maggot_mult(size_t a, size_t b, size_t *ret_res)
{
	// TODO implement without division
	if (b != 0 && a > SIZE_MAX / b) {
		return MAGGOT_WRAP;
	}

	*ret_res = a * b;
	return MAGGOT_OK;
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

#endif // maggot_h
