/*
 * Licensed under the terms of the MIT license.
 * See bottom of file for details.
 */

#ifndef seifu_h
#define seifu_h

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define SEIFU_VERSION_MAJOR 1
#define SEIFU_VERSION_MINOR 0
#define SEIFU_VERSION_PATCH 0

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

static inline SeifuStatus seifu_div_floor(size_t a, size_t b, size_t *ret_res);
static inline size_t seifu_div_floor_bounded(size_t a, size_t b);

static inline SeifuStatus seifu_div_ceil(size_t a, size_t b, size_t *ret_res);
static inline size_t seifu_div_ceil_bounded(size_t a, size_t b);

static inline SeifuStatus seifu_div_round(size_t a, size_t b, size_t *ret_res);
static inline size_t seifu_div_round_bounded(size_t a, size_t b);

static inline size_t unseifu_div_ceil(size_t a, size_t b);
static inline size_t unseifu_div_round(size_t a, size_t b);

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
	// this gets optimized by both gcc and clang
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

	return a * b;
}

static inline SeifuStatus seifu_div(size_t a, size_t b, SeifuDivResult *ret_res)
{
	if (b == 0) {
		return SEIFU_DIV_BY_ZERO;
	}

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

static inline size_t seifu_div_floor_bounded(size_t a, size_t b)
{
	if (b == 0) {
		return SIZE_MAX;
	}

	return a / b;
}

static inline SeifuStatus seifu_div_ceil(size_t a, size_t b, size_t *ret_res)
{
	if (b == 0) {
		return SEIFU_DIV_BY_ZERO;
	}

	*ret_res = unseifu_div_ceil(a, b);
	return SEIFU_OK;
}

static inline size_t seifu_div_ceil_bounded(size_t a, size_t b)
{
	if (b == 0) {
		return SIZE_MAX;
	}

	return unseifu_div_ceil(a, b);
}

static inline SeifuStatus seifu_div_round(size_t a, size_t b, size_t *ret_res)
{
	if (b == 0) {
		return SEIFU_DIV_BY_ZERO;
	}

	*ret_res = unseifu_div_round(a, b);
	return SEIFU_OK;
}

static inline size_t seifu_div_round_bounded(size_t a, size_t b)
{
	if (b == 0) {
		return SIZE_MAX;
	}

	return unseifu_div_round(a, b);
}

static inline size_t unseifu_div_ceil(size_t a, size_t b)
{
	size_t quot = a / b;
	size_t rem = a % b;

	return quot + (rem > 0);
}

static inline size_t unseifu_div_round(size_t a, size_t b)
{
	size_t quot = a / b;
	size_t rem = a % b;

	bool b_is_odd = b % 2 == 1;
	bool round_up = rem >= (b / 2 + b_is_odd);

	return quot + round_up;
}

/*
 * Copyright 2023 shan <writeitinc99@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the “Software”), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#endif // seifu_h
