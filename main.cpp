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

    return 0;
}
