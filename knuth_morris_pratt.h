/* Copyright (c) 2012 Kevin L. Stern
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef KNUTH_MORRIS_PRATT_H_
#define KNUTH_MORRIS_PRATT_H_

#include <string>
#include <limits>

/**
 * An implementation of the Knuth Morris Pratt substring search algorithm. An
 * instance of the algorithm is constructed around a needle string of length m,
 * a process which consumes O(m) time as well as O(m) space. Once an instance is
 * constructed, it is capable of searching for the needle string in any number
 * of haystack strings. The search process consumes O(n) time in a haystack
 * string of length n.
 *
 * @author Kevin L. Stern
 */
class KnuthMorrisPratt {
public:
	static const uint32_t NOT_FOUND;
	/**
	 * Constructor.
	 *
	 * @param needle
	 *            the search string for which the instance will be constructed.
	 */
	KnuthMorrisPratt(const std::string &needle) :
			needle(needle), state_transition_table(new uint32_t[needle.size()]) {
		state_transition_table[0] = NOT_FOUND;
		uint32_t state = 0;
		for (uint32_t i = 1; i < needle.length(); i++) {
			uint32_t transition = state;
			if (needle[transition] == needle[i]) {
				transition = state_transition_table[transition];
			}
			state_transition_table[i] = transition;
			if (needle[i] == needle[state]) {
				state += 1;
			} else {
				state = 0;
			}
		}
	}

	~KnuthMorrisPratt() {
		delete[] state_transition_table;
	}

	/**
	 * Execute the search algorithm.
	 *
	 * @param haystack
	 *            the string in which to search for the needle specified at
	 *            construction time.
	 * @return the index of the first occurrence of the needle string within the
	 *         specified haystack string, -1 if none.
	 */
	uint32_t execute(std::string haystack) {
		return execute(haystack, 0);
	}

	/**
	 * Execute the search algorithm.
	 *
	 * @param haystack
	 *            the string in which to search for the needle specified at
	 *            construction time.
	 * @param index
	 *            the index at which to begin the search within the haystack
	 *            string.
	 * @return the index of the first occurrence of the needle string within the
	 *         specified portion of the haystack string, -1 if none.
	 */
	uint32_t execute(std::string haystack, uint32_t index) {
		uint32_t state = 0;
		for (uint32_t i = index; i < haystack.length(); i++) {
			if (haystack[i] == needle[state]) {
				++state;
				if (state == needle.length()) {
					return i - needle.length() + 1;
				}
			} else {
				do {
					state = state_transition_table[state];
				} while (state != NOT_FOUND && haystack[i] != needle[state]);
				++state;
			}
		}
		return NOT_FOUND;
	}
private:
	std::string needle;
	uint32_t *state_transition_table;
};

const uint32_t KnuthMorrisPratt::NOT_FOUND =
		std::numeric_limits<uint32_t>::max();

#endif /* KNUTH_MORRIS_PRATT_H_ */
