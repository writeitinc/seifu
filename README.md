# seifu

`seifu` is a library for safe `size_t` arithmetic.

## Features

Check if an operation would wrap:
- `seifu_add_would_wrap()`
- `seifu_sub_would_wrap()`
- `seifu_mult_would_wrap()`

Wrap-checked arithmetic:
- `seifu_add()`
- `seifu_sub()`
- `seifu_mult()`

Check for divide-by-zero:
- `seifu_div()`

Quotient-remainder return type:
```c
typedef struct SeifuDivResult {
	size_t quot;
	size_t rem;
} SeifuDivResult;
```

Floor, ceiling, and rounded division:
- `seifu_div_floor()`
- `seifu_div_ceil()`
- `seifu_div_rounded()`

Instead of erroring, use intuitive defaults:
- `seifu_add_bounded()` - returns `SIZE_MAX` if operation would wrap over
- `seifu_sub_bounded()` - returns `0` if operation would wrap under
- `seifu_mult_bounded()` - returns `SIZE_MAX` if operation would wrap over
- `seifu_div_bounded()` - returns `SIZE_MAX` if attempting to divide by `0`
- `seifu_div_floor_bounded()` - returns `SIZE_MAX` if attempting to divide by `0`
- `seifu_div_ceil_bounded()` - returns `SIZE_MAX` if attempting to divide by `0`
- `seifu_div_rounded_bounded()` - returns `SIZE_MAX` attempting to divide by `0`

...And a little bit of unsafe math:
- `unseifu_div_ceil()`
- `unseifu_div_round()`
