#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

int last_err_code = 0;
// last_err_code = 0 when there is no error
//
// last_err_code = 1 when there is invalid characters, the valid characters
//                   are 0~9, '.', '+-'
//
// last_err_code = 2 when there is no integer before '.' or there is no integer
//                   after '.'
//                   e.g. ".123", "123.", "."
//
// last_err_code = 3 when there is more than one '.'
//                   e.g. "123..456", "123.4.56"
//
// last_err_code = 4 when the integer is bigger than FLOAT_MAX
//                   e.g. "1111111111111111111111111111111111111.23"
//

// Clear the left and right whitespace
// e.g. "  123 456  " -> "123 456"
char *trim(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;

	char *start = str;

	if (!(*str))
		return str;

	char *end = str;

	while (*str) {
		if (*str != ' ' && *str != '\t')
			end = str;
		str++;
	}

	*(end + 1) = 0;

	return start;
}

// String to Float
float str_to_float(const char *pstr)
{
	char *pstr_copy, *str;
	// The sign of float, set -1 when the first character is '-'
	int sign = 1;
	// The value of integers
	long long integer = 0;
	// The value of decimals
	double decimal = 0;
	// The multiple of next decimal
	double dec_num = 0.1;
	// Has found dot '.'
	bool has_dot = 0;
	// Has integer
	bool has_integer = 0;

	if (pstr == NULL)
		return 0;

	pstr_copy = strdup(pstr);
	str = trim(pstr_copy);

	if (!(*str)) {
		// "   "
		return 0;
	}

	if (*str == '+' || *str == '-') {
		if (*str == '-')
			sign = -1;
		str++;
	}

	while (*str) {

		if (*str >= '0' && *str <= '9') {

			if (!has_dot) {
				// "123"
				if (!has_integer)
					has_integer = 1;

				integer *= 10;
				integer += *str - '0';

				if (integer > (long long)INT_MAX) {
					// e.g. "111111111111111111111111111111111.22"
					last_err_code = 4;
					return 0;
				}

			} else if (!has_integer) {
				// ".123"
				last_err_code = 2;
				return 0;
			} else {
				// "123.456"
				if (dec_num < (double)1e-10) {
					// There are too many decimals, ignore the following decimals
				} else {
					decimal += (*str - '0') * dec_num;
					dec_num *= 0.1;
				}
			}

		} else if (*str == '.') {

			if (has_dot) {
				// e.g. "123...456"
				last_err_code = 3;
				return 0;
			}
			has_dot = 1;

		} else {
			// e.g. "123fgh?.456"
			last_err_code = 1;
			return 0;
		}

		str++;
	}

	if (has_dot && (!has_integer || dec_num == 0.1)) {
		// e.g. ".123" or "123." or "."
		last_err_code = 2;
		return 0;
	}

	free(pstr_copy);

	float ret = (float) integer + (float)decimal;
	return ret * sign;
}

int main()
{
	const struct TestCase {
		const char *str;
		const float ret;
		int last_err_code;
	} test_cases[] = {
		// last_err_code != 0
		{ "abc", 0, 1 },
		{ "123+.456", 0, 1 },
		{ "++123.456", 0, 1 },

		{ ".", 0, 2 },
		{ ".123", 0, 2 },
		{ "123.", 0, 2 },

		{ "123..456", 0, 3 },
		{ "123.4.56", 0, 3 },

		// Case #8:
		{ "1111111111111111111111111111111.456", 0, 4 },

		// last_err_code == 0
		{ "", 0, 0 },
		{ "123.456", 123.456, 0 },
		// There are too many decimals
		{ "1.12345678901234567890", 1.12345678, 0 },
	};

	int errors = 0;

	for (int iii = 0; iii < sizeof(test_cases) / sizeof(TestCase); iii++) {
		const TestCase &tc = test_cases[iii];
		// Clear last_err_code
		last_err_code = 0;
		const float actual_ret = str_to_float(tc.str);

		if (tc.ret != actual_ret || last_err_code != tc.last_err_code) {

			errors++;

			cout << "Case #" << iii << ": FAILED" <<  endl;
			cout << "\tExpected ret=" << tc.ret << endl;
			cout << "\tAcutal   ret=" << actual_ret << endl;
			cout << "\tExpected last_err_code=" << tc.last_err_code << endl;
			cout << "\tAcutal   last_err_code=" << last_err_code << endl;
		}
	}

	if (errors == 0)
		cout << "All test passed!" << endl;
	else
		cout << "There are " << errors << " cases failed." << endl;

	return 0;
}

