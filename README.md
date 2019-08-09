# result-option
A simple type-safe Result<T> and Option<T> types for C++11.

Result currently allows only for strings for errors.

## Configuration
By default, the types type will use assertions to check that values have been used (moved out) only once. 
To disable this feature `#define RESULT_OPTION_MULTIPLE_MOVE`. 

## Licence
Public domain.
