# utils/stringutils



## Description

String utilities

## Functions

- `bool iswhitespace(std::string s)` = If the string is entirely whitespace.
- `std::vector<std::string> split(std::string s)` = Split a string by whitespace.
- `int find(std::string s, char t, int x)` = Find the first instancE of `t` in `s` starting from index `x` (-1 if none).
- `std::string substr(std::string s, int a, int b)` = Substring from `a` up to not including `b`.