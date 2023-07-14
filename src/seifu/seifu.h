#ifndef seifu_h
#define seifu_h

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum SeifuStatus {
	SEIFU_OK = 0,
	SEIFU_WRAP = -1,
	SEIFU_DIV_BY_ZERO = -2
} SeifuStatus;

typedef struct SeifuDivResult {
	size_t quot;
	size_t rem;
} SeifuDivResult;

static inline bool seifu_add_would_wrap(size_t a, size_t b);
static inline bool seifu_sub_would_wrap(size_t a, size_t b);
static inline bool seifu_mult_would_wrap(size_t a, size_t b);

static inline SeifuStatus seifu_add(size_t a, size_t b, size_t *ret_res);
static inline size_t seifu_add_bounded(size_t a, size_t b);

static inline SeifuStatus seifu_sub(size_t a, size_t b, size_t *ret_res);
static inline size_t seifu_sub_bounded(size_t a, size_t b);

static inline SeifuStatus seifu_mult(size_t a, size_t b, size_t *ret_res);
static inline size_t seifu_mult_bounded(size_t a, size_t b);

static inline SeifuStatus seifu_div(size_t a, size_t b,
		SeifuDivResult *ret_res);
static inline SeifuDivResult seifu_div_bounded(size_t a, size_t b);

static inline SeifuStatus seifu_div_floor(size_t a, size_t b,
		size_t *ret_res);
static inline SeifuStatus seifu_div_ceil(size_t a, size_t b, size_t *ret_res);
static inline SeifuStatus seifu_div_round(size_t a, size_t b,
		size_t *ret_res);

static inline size_t seifu_div_ceil_unchecked(size_t a, size_t b);
static inline size_t seifu_div_round_unchecked(size_t a, size_t b);

static inline bool seifu_add_would_wrap(size_t a, size_t b)
{
	return a > SIZE_MAX - b;
}

static inline bool seifu_sub_would_wrap(size_t a, size_t b)
{
	return b > a;
}

static inline bool seifu_mult_would_wrap(size_t a, size_t b)
{
	// TODO implement without division
	return b != 0 && a > SIZE_MAX / b;
}

static inline SeifuStatus seifu_add(size_t a, size_t b, size_t *ret_res)
{
	if (seifu_add_would_wrap(a, b)) {
		return SEIFU_WRAP;
	}

	*ret_res = a + b;
	return SEIFU_OK;
}

static inline size_t seifu_add_bounded(size_t a, size_t b)
{
	if (seifu_add_would_wrap(a, b)) {
		return SIZE_MAX;
	}

	return a + b;
}

static inline SeifuStatus seifu_sub(size_t a, size_t b, size_t *ret_res)
{
	if (seifu_sub_would_wrap(a, b)) {
		return SEIFU_WRAP;
	}

	*ret_res = a - b;
	return SEIFU_OK;
}

static inline size_t seifu_sub_bounded(size_t a, size_t b)
{
	if (seifu_sub_would_wrap(a, b)) {
		return 0;
	}

	return a - b;
}

static inline SeifuStatus seifu_mult(size_t a, size_t b, size_t *ret_res)
{
	if (seifu_mult_would_wrap(a, b)) {
		return SEIFU_WRAP;
	}

	*ret_res = a * b;
	return SEIFU_OK;
}

static inline size_t seifu_mult_bounded(size_t a, size_t b)
{
	if (seifu_mult_would_wrap(a, b)) {
		return SIZE_MAX;
	}
}

static inline SeifuStatus seifu_div(size_t a, size_t b,
		SeifuDivResult *ret_res)
{
	if (b == 0) {
		return SEIFU_DIV_BY_ZERO;
	}

	// TODO check if this gets optimized
	size_t quot = a / b;
	size_t rem = a % b;

	*ret_res = (SeifuDivResult){
		.quot = quot,
		.rem = rem
	};
	return SEIFU_OK;
}

static inline SeifuDivResult seifu_div_bounded(size_t a, size_t b)
{
	if (b == 0) {
		return (SeifuDivResult){
			.quot = SIZE_MAX,
			.rem = 0
		};
	}

	// TODO check if this gets optimized
	size_t quot = a / b;
	size_t rem = a % b;

	return (SeifuDivResult){
		.quot = quot,
		.rem = rem
	};
}

static inline SeifuStatus seifu_div_floor(size_t a, size_t b, size_t *ret_res)
{
	if (b == 0) {
		return SEIFU_DIV_BY_ZERO;
	}

	*ret_res = a / b;
	return SEIFU_OK;
}

static inline SeifuStatus seifu_div_ceil(size_t a, size_t b, size_t *ret_res)
{
	if (b == 0) {
		return SEIFU_DIV_BY_ZERO;
	}

	*ret_res = seifu_div_ceil_unchecked(a, b);
	return SEIFU_OK;
}

static inline SeifuStatus seifu_div_round(size_t a, size_t b, size_t *ret_res)
{
	if (b == 0) {
		return SEIFU_DIV_BY_ZERO;
	}

	*ret_res = seifu_div_round_unchecked(a, b);
	return SEIFU_OK;
}

static inline size_t seifu_div_ceil_unchecked(size_t a, size_t b)
{
	size_t quot = a / b;
	size_t rem = a % b;

	return quot + (rem > 0);
}

static inline size_t seifu_div_round_unchecked(size_t a, size_t b)
{
	size_t quot = a / b;
	size_t rem = a % b;

	bool b_is_odd = b % 2 == 1;
	bool round_up = rem >= (b / 2 + b_is_odd);

	return quot + round_up;
}

#endif // seifu_h
