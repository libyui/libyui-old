
# Unit Tests

This directory contains unit tests.

The unit tests are enabled by default, if you want to disable them (not
recommended!) then use the `-DENABLE_TESTS=OFF` cmake option.


## Writing Tests

- The test files should end with `_test.cc` suffix.
- Use the boost test framework, see the [documentation](
https://www.boost.org/doc/libs/release/libs/test/doc/html/index.html).


## Running the Tests

Run `make test`. If some some test fails and you need to get more details
then directly run the test binary from `build/tests` directory, it will print
the details on the console.


## Code Coverage

The code coverage reporting can be enabled using the `-DENABLE_CODE_COVERAGE=ON`
cmake option or using the `make -f Makefile.cvs COVERAGE=1` initial call.
The `lcov` package needs to be installed.

To print the code coverage run `make`, `make test` and `make coverage`.
There is an additional all-in-one target which build the sources, runs the tests
and prints the code coverage - simply run `make tests`.

The code coverage summary will be displayed on the console. If you need more
details (e.g. which lines are covered) then open the `coverage/index.html`
file in a web browser.
