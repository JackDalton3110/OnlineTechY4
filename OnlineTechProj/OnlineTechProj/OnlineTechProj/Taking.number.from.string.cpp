#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

 
struct not_digit {
    bool operator()(const char c)
    {
        return c != ' ' && !isdigit(c);
    }
};
 
