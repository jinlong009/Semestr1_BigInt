#include <iostream>
#include "BigInt.h"


int main() {
    std::string num1 = "6541654885416548";
    std::string num2 = "486486584756989";
    BigInt number1 = num1;
    BigInt number2 = num2;
    
    std::cout << number1 + number2 << std::endl;
    std::cout << number1 - number2 << std::endl;
    std::cout << number1 * number2 << std::endl;
    std::cout << number1 / number2 << std::endl;
    std::cout << number1 % number2 << std::endl;
    std::cout << (number1 < number2) << std::endl;
    std::cout << (number1 > number2) << std::endl;

    BigInt n = 100000000000000;
    ++n;
    std::cout << n << std::endl;
    --n;
    std::cout << n << std::endl;

    return 0;
}
