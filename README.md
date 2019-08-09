# result
A simple type-safe Rust-like Result<T> type for C++11. It uses only strings for errors.

# Configuration
By default, the Result type will use assertions to check that values have been used (moved out) only once. 
To disable this feature `#define RESULT_MULTIPLE_MOVE`. 

# Licence
Public domain.
