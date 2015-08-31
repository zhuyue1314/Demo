/*
 * Author   : Kai Zhao
 * Email    : loverszhao@gmail.com
 * Reference:
 */

/*
 * Description: Sort from 1 to N by dictionary,
 * e.g. N = 11, sorted: 1, 10, 11, 2, 3, 4, 5, 6, 7, 8, 9
 */

#include <cassert>
#include <cmath>

#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;

// 1  10  11  12  13  14  15  16  17  18  19  100  101  102
// 2  20  21  22  23  24  25  26  27  28  29  200  201  202
// 3  ...
//
// 1  return 10
// 10 return 11
// 3  return 30
//

#define MAX_SIZE 10000

int ans[MAX_SIZE];

void run(const int N)
{
	int kkk, mmm;
	int index = 0;

	for (int iii = 1; iii <= 9; iii++) {
		for (int jjj = 0; jjj <= 9; jjj++) {
			kkk = iii * pow(10.0, jjj);       // 10  100   20
			mmm = kkk + pow(10.0, jjj) - 1;   // 19  199   29
			while (kkk <= N && kkk <= mmm)
				ans[index++] = kkk++;
			if (kkk > N)
				break;
		}
	}
}

int main()
{
	cout << "===================================" << endl;
	cout << "\tThis is a demo project" << endl;
	cout << "===================================" << endl;

	const struct TestCase {
		int N;
		int expected_arr[MAX_SIZE];
	} test_cases [] = {
		{ 11, 
			{ 1, 10, 11, 2, 3, 4, 5, 6, 7, 8, 9 }
		},
		{ 23, 
			{ 1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
			  2, 20, 21, 22, 23, 3, 4, 5, 6, 7, 8, 9 }
		},
		{ 103, 
			{ 1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 100, 101, 102, 103,
			  2, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			  3, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
			  4, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
			  5, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
			  6, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
			  7, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
			  8, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
			  9, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99 },
		},
	};

	for (int iii = 0; iii < sizeof(test_cases) / sizeof(TestCase); iii++) {
		run(test_cases[iii].N);
		assert(std::equal(ans, ans + test_cases[iii].N, test_cases[iii].expected_arr));
	}
	cout << "success!" << endl;

	return 0;
}
