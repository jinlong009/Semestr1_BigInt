#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

class BigInt {
    public:
        BigInt();
        BigInt(std::string string);
        BigInt(signed int number);
        BigInt(unsigned int number);
        BigInt(signed long number);
        BigInt(unsigned long number);
        BigInt(signed long long number);
        BigInt(unsigned long long number);
        static std::string to_string(BigInt number);
        friend std::ostream& operator <<(std::ostream& ostream, const BigInt& number);
        friend bool operator ==(BigInt number1, BigInt number2);
        friend bool operator !=(BigInt number1, BigInt number2);
        friend bool operator <(BigInt number1, BigInt number2);
        friend bool operator >(const BigInt& number1, const BigInt& number2);
        friend bool operator >=(const BigInt& number1, const BigInt& number2);
        friend bool operator <=(const BigInt& number1, const BigInt& number2);
        friend BigInt operator +(BigInt number1, BigInt number2);
        BigInt operator +=(BigInt number);
        BigInt operator ++();
        BigInt operator ++(int);
        friend BigInt operator -(BigInt number1, BigInt number2);
        BigInt operator -=(BigInt number);
        BigInt operator --();
        BigInt operator --(int);
        friend BigInt operator *(const BigInt& number1, const BigInt& number2);
        BigInt operator *=(const BigInt& number);
        friend BigInt operator /(BigInt number1, BigInt number2);
        BigInt operator /=(BigInt number);
        friend BigInt operator %(BigInt number1, BigInt number2);
        BigInt operator %=(BigInt number);

    private:
        std::vector<int> digits;
        bool sign;
        static const int base = 1000000000;
        static const int base_length = 9;
        static const int max_def_mul = 256;
        static std::vector<int> ConvertStringToVector(const std::string& string);
        static BigInt RemoveLeadingZeros(BigInt number);
        static BigInt ShiftRight(BigInt number, long long power);
        static BigInt ShiftLeft(BigInt number, long long power);
        static BigInt multiply(BigInt number1, BigInt number2, bool iteration);
};

std::vector<int> BigInt::ConvertStringToVector(const std::string& string) {
    std::vector<int> result;
    if (string.size() % base_length == 0) {
        result.resize(string.size() / base_length);
    } else {
        result.resize(string.size() / base_length + 1);
    }

    for (long long str_pos = string.size() - 1, res_pos = result.size() - 1; str_pos >= 0; str_pos -= base_length, --res_pos) {
        if (str_pos + 1 - base_length <= 0) {
            result[res_pos] = std::stoi(string.substr(0, str_pos + 1));
        } else {
            result[res_pos] = std::stoi(string.substr(str_pos - base_length + 1, base_length));
        }
    }

    return result;
}

BigInt::BigInt() {
    digits.resize(1);
    sign = true;
}

BigInt::BigInt(std::string string) {
    assert(!(string.empty() || (string.size() == 1 && string[0] == '-')));

    if (string.front() == '-') {
        string.erase(string.begin());
        sign = false;
    } else {
        sign = true;
    }

    for (long long i = 0; i < string.size(); ++i) {
        assert(!(string[i] < 48 || string[i] > 57));
    }
    while (string.size() != 1 && string.front() == '0') {
        string.erase(string.begin());
    }

    digits = BigInt::ConvertStringToVector(string);
}

BigInt::BigInt(signed int number) {
    std::string string = std::to_string(number);
    if (string.front() == '-') {
        string.erase(string.begin());
        sign = false;
    } else {
        sign = true;
    }

    digits = BigInt::ConvertStringToVector(string);
}

BigInt::BigInt(unsigned int number) {
    sign = true;
    digits = BigInt::ConvertStringToVector(std::to_string(number));
}

BigInt::BigInt(signed long number) {
    std::string string = std::to_string(number);
    if (string.front() == '-') {
        string.erase(string.begin());
        sign = false;
    } else {
        sign = true;
    }

    digits = BigInt::ConvertStringToVector(string);
}

BigInt::BigInt(unsigned long number) {
    sign = true;
    digits = BigInt::ConvertStringToVector(std::to_string(number));
}

BigInt::BigInt(signed long long number) {
    std::string string = std::to_string(number);
    if (string.front() == '-') {
        string.erase(string.begin());
        sign = false;
    } else {
        sign = true;
    }

    digits = BigInt::ConvertStringToVector(string);
}

BigInt::BigInt(unsigned long long number) {
    sign = true;
    digits = BigInt::ConvertStringToVector(std::to_string(number));
}

std::string BigInt::to_string(BigInt number) {
    if (number.digits.size() == 1 && number.digits.front() == 0) {
        return "0";
    }
    std::string result;
    if (!number.sign) {
        result.append("-");
    }

    result.reserve(number.digits.size() * (base_length - 1));
    std::string tmp;
    result.append(std::to_string(number.digits[0]));
    for (long long i = 1; i < number.digits.size(); ++i) {
        tmp = std::to_string(number.digits[i]);
        tmp.reserve(base_length - tmp.size());
        while (tmp.size() < base_length) {
            tmp.insert(tmp.begin(), '0');
        }
        result.append(tmp);
    }

    return result;
}

std::ostream& operator <<(std::ostream& ostream, const BigInt& number) {
    std::string string = BigInt::to_string(number);
    for (long long i = 0; i < string.size(); ++i) {
        ostream.put(string[i]);
    }

    return ostream;
}

bool operator ==(BigInt number1, BigInt number2) {
    if (number1.sign != number2.sign) {
        return false;
    }
    if (number1.digits.size() != number2.digits.size()) {
        return false;
    }
    for (long long i = 0; i < number1.digits.size(); ++i) {
        if (number1.digits[i] != number2.digits[i]) {
            return false;
        }
    }
    return true;
}

bool operator !=(BigInt number1, BigInt number2) {
    return !(number1 == number2);
}

bool operator <(BigInt number1, BigInt number2) {
    if (number1 == number2) {
        return false;
    }
    if (!number1.sign && number2.sign) {
        return true;
    }
    if (number1.sign && !number2.sign) {
        return false;
    }
    if (!number1.sign && !number2.sign) {
        number1.sign = true;
        number2.sign = true;
        return !(number1 < number2);
    }
    if (number1.digits.size() < number2.digits.size()) {
        return true;
    }
    if (number1.digits.size() > number2.digits.size()) {
        return false;
    }
    return std::lexicographical_compare(number1.digits.begin(), number1.digits.end(), number2.digits.begin(), number2.digits.end());
}

bool operator >(const BigInt& number1, const BigInt& number2) {
    if (number1 != number2 && !(number1 < number2)) {
        return true;
    }
    return false;
}

bool operator <=(const BigInt& number1, const BigInt& number2) {
    if (number1 < number2 || number1 == number2) {
        return true;
    }
    return false;
}

bool operator >=(const BigInt& number1, const BigInt& number2) {
    if (number1 > number2 || number1 == number2) {
        return true;
    }
    return false;
}

BigInt BigInt::RemoveLeadingZeros(BigInt number) {
    long long border = number.digits.size() - 1;
    for (long long i = 0; i < number.digits.size() - 1; ++i) {
        if (number.digits[i] != 0) {
            border = i;
            break;
        }
    }
    number.digits.erase(number.digits.begin(), number.digits.begin() + border);

    return number;
}

BigInt BigInt::ShiftRight(BigInt number, long long power) {
    number.digits.reserve(power);
    for (long long i = 0; i < power; ++i) {
        number.digits.insert(number.digits.begin(), 0);
    }
    return number;
}

BigInt BigInt::ShiftLeft(BigInt number, long long power) {
    if (number == 0) {
        return number;
    }
    number.digits.resize(number.digits.size() + power);
    return number;
}

BigInt operator +(BigInt number1, BigInt number2) {
    if (number1.sign && !number2.sign) {
        number2.sign = true;
        return number1 - number2;
    }
    if (!number1.sign && number2.sign) {
        number1.sign = true;
        return number2 - number1;
    }
    if (!number1.sign && !number2.sign) {
        number2.sign = true;
    }
    if (number1.digits.size() > number2.digits.size()) {
        number2 = BigInt::ShiftRight(number2, number1.digits.size() - number2.digits.size());
    } else {
        number1 = BigInt::ShiftRight(number1, number2.digits.size() - number1.digits.size());
    }

    int sum;
    int in_mind = 0;
    for (long long i = number1.digits.size() - 1; i >= 0; --i) {
        sum = number1.digits[i] + number2.digits[i] + in_mind;
        in_mind = sum / BigInt::base;
        number1.digits[i] = sum % BigInt::base;
    }
    if (in_mind != 0) {
        number1.digits.insert(number1.digits.begin(), in_mind);
    }

    return number1;
}

BigInt BigInt::operator +=(BigInt number) {
    return *this = *this + number;
}

BigInt BigInt::operator ++() {
    return *this = *this + 1;
}

BigInt BigInt::operator ++(int) {
    *this = *this + 1;
    return *this = *this - 1;
}

BigInt operator -(BigInt number1, BigInt number2) {
    if (number1.sign && !number2.sign) {
        number2.sign = true;
        return number1 + number2;
    }
    if (!number1.sign && number2.sign) {
        number1.sign = true;
        BigInt tmp = number1 + number2;
        tmp.sign = false;
        return tmp;
    }
    if (!number1.sign && !number2.sign) {
        number1.sign = true;
        number2.sign = true;
        BigInt tmp;
        tmp = number1;
        number1 = number2;
        number2 = tmp;
    }
    if (number1 < number2) {
        BigInt tmp = number1;
        number1 = number2;
        number2 = tmp;
        number1.sign = false;
    }

    number2 = BigInt::ShiftRight(number2, number1.digits.size() - number2.digits.size());
    int different;
    for (long long i = number1.digits.size() - 1; i >= 0; --i) {
        different = number1.digits[i] - number2.digits[i];
        if (different >= 0) {
            number1.digits[i] = different;
        } else {
            number1.digits[i] = different + BigInt::base;
            for (long long j = i - 1; true; --j) {
                if (number1.digits[j] == 0) {
                    number1.digits[j] = BigInt::base - 1;
                } else {
                    number1.digits[j] = number1.digits[j] - 1;
                    break;
                }
            }
        }
    }

    return BigInt::RemoveLeadingZeros(number1);
}

BigInt BigInt::operator -=(BigInt number) {
    return *this = *this - number;
}

BigInt BigInt::operator --() {
    return *this = *this - 1;
}

BigInt BigInt::operator --(int) {
    *this = *this - 1;
    return *this = *this + 1;
}

BigInt BigInt::multiply(BigInt number1, BigInt number2, bool iteration) {
    if (std::min(number1.digits.size(), number2.digits.size()) <= max_def_mul) {
        number1 = BigInt::RemoveLeadingZeros(number1);
        number2 = BigInt::RemoveLeadingZeros(number2);
        BigInt result;
        result.digits.resize(number1.digits.size() + number2.digits.size());
        long long composition;

        for (long long i = number1.digits.size() - 1; i >= 0; --i) {
            for (long long j = number2.digits.size() - 1; j >= 0; --j) {
                composition = static_cast<long long>(number1.digits[i]) * static_cast<long long>(number2.digits[j]) + result.digits[i + j + 1];
                result.digits[i + j + 1] = composition % BigInt::base;
                result.digits[i + j] = result.digits[i + j] + (composition / BigInt::base);
            }
        }

        return BigInt::RemoveLeadingZeros(result);
    }

    if (number1.digits.size() % 2 != 0) {
        number1.digits.insert(number1.digits.begin(), 0);
    }
    if (number2.digits.size() % 2 != 0) {
        number2.digits.insert(number2.digits.begin(), 0);
    }
    if (number1.digits.size() > number2.digits.size()) {
        number2 = BigInt::ShiftRight(number2, number1.digits.size() - number2.digits.size());
    } else {
        number1 = BigInt::ShiftRight(number1, number2.digits.size() - number1.digits.size());
    }

    long long numbers_size = number1.digits.size();
    long long numbers_part_size = numbers_size / 2;
    BigInt number1_left_part;
    BigInt number1_right_part;
    BigInt number2_left_part;
    BigInt number2_right_part;
    number1_left_part.digits.resize(numbers_part_size);
    number1_right_part.digits.resize(numbers_part_size);
    number2_left_part.digits.resize(numbers_part_size);
    number2_right_part.digits.resize(numbers_part_size);
    std::copy(number1.digits.begin(), number1.digits.begin() + numbers_part_size, number1_left_part.digits.begin());
    std::copy(number2.digits.begin(), number2.digits.begin() + numbers_part_size, number2_left_part.digits.begin());
    std::copy(number1.digits.begin() + numbers_part_size, number1.digits.begin() + numbers_size, number1_right_part.digits.begin());
    std::copy(number2.digits.begin() + numbers_part_size, number2.digits.begin() + numbers_size, number2_right_part.digits.begin());

    BigInt product_first;
    BigInt product_second;
    BigInt product_third;
    product_first = BigInt::multiply(number1_left_part, number2_left_part, false);
    product_second = BigInt::multiply(number1_right_part, number2_right_part, false);
    product_third = BigInt::multiply(BigInt::RemoveLeadingZeros(number1_left_part) + BigInt::RemoveLeadingZeros(number1_right_part), BigInt::RemoveLeadingZeros(number2_left_part) + BigInt::RemoveLeadingZeros(number2_right_part), false);

    return BigInt::ShiftLeft(product_first, numbers_size) + BigInt::ShiftLeft(product_third - (product_first + product_second), numbers_part_size) + product_second;
}

BigInt operator *(const BigInt& number1, const BigInt& number2) {
    BigInt result = BigInt::multiply(number1, number2, true);
    result.sign = (number1.sign == number2.sign);
    return result;
}

BigInt BigInt::operator *=(const BigInt& number) {
    return *this = *this * number;
}

BigInt operator /(BigInt number1, BigInt number2) {
    assert(number2 != 0);

    if (number2.digits.size() == 1) {
        int number2_integer = number2.digits.front();
        int in_mind = 0;
        long long composition;
        for (long long i = 0; i < number1.digits.size(); ++i) {
            composition = static_cast<long long>(number1.digits[i]) + static_cast<long long>(in_mind) * static_cast<long long>(BigInt::base);
            number1.digits[i] = composition / number2_integer;
            in_mind = composition % number2_integer;
        }
        number1.sign = (number1.sign == number2.sign);
        return BigInt::RemoveLeadingZeros(number1);
    }

    BigInt result;
    result.sign = (number1.sign == number2.sign);
    BigInt number1_part;
    number1.sign = true;
    number2.sign = true;
    if (number1 < number2) {
        return 0;
    }

    result.digits.resize(0);
    number1_part.digits.resize(0);
    int quotient;
    int left;
    int middle;
    int right;
    BigInt tmp;
    for (long long i = 0; i < number1.digits.size(); ++i) {
        number1_part.digits.push_back(number1.digits[i]);
        quotient = 0;
        left = 0;
        right = BigInt::base;
        while (left <= right) {
            middle = (left + right) / 2;
            tmp = number2 * middle;
            if (tmp <= number1_part) {
                quotient = middle;
                left = middle + 1;
            } else {
                right = middle - 1;
            }
        }

        number1_part = number1_part - (number2 * quotient);
        if (!result.digits.empty() || quotient != 0) {
            result.digits.push_back(quotient);
        }
        if (number1_part == 0) {
            number1_part.digits.resize(0);
        }
    }

    return result;
}

BigInt BigInt::operator /=(BigInt number) {
    return *this = *this / number;
}

BigInt operator %(BigInt number1, BigInt number2) {
    assert(number2 != 0);
    
    if (number2.digits.size() == 1) {
        int number2_integer = number2.digits.front();
        int in_mind = 0;
        long long composition;
        for (long long i = 0; i < number1.digits.size(); ++i) {
            composition = static_cast<long long>(number1.digits[i]) + static_cast<long long>(in_mind) * static_cast<long long>(BigInt::base);
            number1.digits[i] = composition / number2_integer;
            in_mind = composition % number2_integer;
        }

        return in_mind;
    }

    BigInt number1_part;
    number1.sign = true;
    number2.sign = true;
    if (number1 < number2) {
        return number1;
    }

    number1_part.digits.resize(0);
    int quotient;
    int left;
    int middle;
    int right;
    BigInt tmp;
    for (long long i = 0; i < number1.digits.size(); ++i) {
        number1_part.digits.push_back(number1.digits[i]);
        quotient = 0;
        left = 0;
        right = BigInt::base;
        while (left <= right) {
            middle = (left + right) / 2;
            tmp = number2 * middle;
            if (tmp <= number1_part) {
                quotient = middle;
                left = middle + 1;
            } else {
                right = middle - 1;
            }
        }
        number1_part = number1_part - (number2 * quotient);
        if (number1_part == 0) {
            number1_part.digits.resize(0);
        }
    }
    if (number1_part.digits.empty()) {
        return 0;
    }

    return number1_part;
}

BigInt BigInt::operator %=(BigInt number) {
    return *this = *this % number;
}
