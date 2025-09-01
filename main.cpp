#include <iostream>
#include <string>
using namespace std;

class BigInt
{
    string number;   // Stores the number as a string
    bool isNegative; // True if number is negative

    // Remove unnecessary leading zeros from the number string
    void removeLeadingZeros(string &s)
    {
        while (number.length() > 1 && number[0] == '0')
        {
            number.erase(0, 1);
        }
    }

    // Compare absolute values of two BigInts (ignore signs)
    // Returns: 1 if |this| > |other|, 0 if equal, -1 if |this| < |other|
    int compareMagnitude(const BigInt &other) const
    {
        if (number.length() > other.number.length()) return 1;
        if (number.length() < other.number.length()) return -1;
        if (number > other.number) return 1;
        if (number < other.number) return -1;

        return 0;
    }
    string digits; // string used to convert int64_t value to string

public:
    // Default constructor - initialize to zero
    BigInt()
    { // Hafez
      // TODO: Implement this constructor
    }

    // Constructor from 64-bit integer
    BigInt(int64_t value)
    { // Fares
        digits = to_string(value);
    }

    // Constructor from string representation
    BigInt(const string &str)
    { // abdelrahman
    // empty?
    if (str.empty()) {
        number = "0";
        isNegative = false;
        return;
    }

    //negative?
    size_t start = 0;
    if (str[0] == '-') {
        isNegative = true;
        start = 1;
    }
    else if (str[0] == '+') {
        isNegative = false;
        start = 1;
    }
    else
        isNegative = false;

    number = str.substr(start);
    removeLeadingZeros(number);
    if (number == "0")
        isNegative = false;
}

    // Copy constructor
    BigInt(const BigInt &other)
    { // menna
      this->number = other.number;
      this->isNegative = other.isNegative;

    }

    // Destructor
    ~BigInt()
    { // Ahmed
      // TODO: Implement if needed
    }

    // Assignment operator
    BigInt &operator=(const BigInt &other)
    { // hafez
        // TODO: Implement this operator
        return *this;
    }

    // Unary negation operator (-x)
    BigInt operator-() const
    { // fares
        BigInt result = *this;
        if (result.number == "0")
        {
            return result;
        }
        else
        {
            result.isNegative = !result.isNegative;
        }

        return result;
    }

    // Unary plus operator (+x)
    BigInt operator+() const
    { // ahmed
        BigInt result;
        // TODO: Implement this operator
        return result;
    }

    // Addition assignment operator (x += y)
    BigInt &operator+=(const BigInt &other)
    { // hafez
        // TODO: Implement this operator
        return *this;
    }

    // Subtraction assignment operator (x -= y)
    BigInt &operator-=(const BigInt &other)
    { // fares
        int n = number.size();
        int o = other.number.size();

        string y = string(n - o, '0') + other.number; // y will represent the other string and if n>m the gap will be completed with zeros
        if (number < y)
        {
            swap(number, y);
            isNegative = !isNegative; // to handle the sign after swapping to subtract always smaller from larger
        }

        for (int i = n - 1; i >= 0; i--)
        {
            int numx = number[i] - '0';
            int numy = y[i] - '0';
            if (numx < numy)
            {
                numx += 10;
                if (i > 0)
                {                                                    // avoid out-of-bounds
                    number[i - 1] = (number[i - 1] - '0' - 1) + '0'; // take the borrow from the preceding digit and change its value then return it to char again
                }
            }
            int resultsubtract = numx - numy;
            number[i] = resultsubtract + '0';
        }

        removeLeadingZeros(number); // remove the leading zeros from final number
        return *this;               // refers to the current number we have
    }

    // Multiplication assignment operator (x *= y)
    BigInt &operator*=(const BigInt &other)
    { // abdelrahman
        if (number == "0" || other.number == "0") {
        number = "0";
        isNegative = false;
        return *this;
    }

    string num1 = number;
    string num2 = other.number;
    int n = num1.size();
    int m = num2.size();
    bool resultNegative = (isNegative != other.isNegative);
    vector<int> result(n + m, 0);

    // multiply num1 with num2
    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            int digit1 = num1[i] - '0';
            int digit2 = num2[j] - '0';
            int product = digit1 * digit2;
            int sum = product + result[i + j + 1];
            result[i + j + 1] = sum % 10;
            result[i + j] += sum / 10;
        }
    }

    string strResult;
    for (int digit : result) {
        if (!(strResult.empty() && digit == 0)) {
            strResult += to_string(digit);
        }
    }
    if (strResult.empty()) {
        strResult = "0";
        resultNegative = false;
    }

    number = strResult;
    isNegative = resultNegative;
    return *this;
}

    // Division assignment operator (x /= y)
    BigInt &operator/=(const BigInt &other)
    { // menna


    if (other.number == "0")
    {
        throw runtime_error("Division by zero");
    }

    // Determine the sign of the result
    bool resultNegative = (this->isNegative != other.isNegative);

    string dividend = this->number;
    string divisor = other.number;

    // Final result string
    string result = "";

    // Temporary remainder while dividing
    string current = "";

    for (size_t i = 0; i < dividend.size(); i++)
    {
        // Bring down the next digit
        current += dividend[i];

        // Remove leading zeros from the current value
        while (current.size() > 1 && current[0] == '0')
            current.erase(0, 1);

        int count = 0;

        // Subtract divisor from current until current < divisor
        BigInt cur(current);
        while (cur.compareMagnitude(other) >= 0)
        {
            cur = cur - other; // Requires operator- to be implemented
            count++;
        }

        // Update the remainder
        current = cur.number;

        // Append the digit to the result
        result += char('0' + count);
    }

    // Remove leading zeros from the result
    while (result.size() > 1 && result[0] == '0')
        result.erase(0, 1);

    this->number = result;
    this->isNegative = (result != "0") && resultNegative;

    return *this;
}



    // Modulus assignment operator (x %= y)
    BigInt &operator%=(const BigInt &other)
    { // lasheen
        // TODO: Implement this operator
        return *this;
    }

    // Pre-increment operator (++x)
    BigInt &operator++()
    {
        int n = number.size();

        for (int i = n - 1; i >= 0; --i)
        {
            if (number[i] < '9')
            {
                number[i] += 1;
                return *this; // no need to continue looping
            }
            else
            {
                number[i] = '0'; // set to 0, carry to next digit
            }
        }

        number.insert(number.begin(), '1'); // we need to insert 1 at the beginning if all digits were '9'
        return *this;
    }

    // Post-increment operator (x++)
    BigInt operator++(int)
    { // abdelrahman
        BigInt temp = *this;
        ++(*this);
        return temp;
    }

    // Pre-decrement operator (--x)
    BigInt &operator--()
    { // menna


    if (number == "0")
    {
        number = "1";
        isNegative = true;
        return *this;
    }

    if (!isNegative)
    {
        BigInt one("1");
        *this = *this - one;
    }
    else
    {
        BigInt one("1");
        *this = *this + one;
    }


        return *this;
    }

    // Post-decrement operator (x--)
    BigInt operator--(int)
    { // lasheen
        BigInt temp;
        // TODO: Implement this operator
        return temp;
    }

    // Convert BigInt to string representation
    string toString() const
    { // hafez
        // TODO: Implement this function
        return "";
    }

    // Output stream operator (for printing)
    friend ostream &operator<<(ostream &os, const BigInt &num)
    { // fares

        {
            if (num.isNegative && num.number != "0")
            {
                os << '-'; // after searching i found out that ostream is similar to stack push or queue as well
            }
            os << num.number;
            return os;
        }
        return os;
    }

    // Input stream operator (for reading from input)
    friend istream &operator>>(istream &is, BigInt &num)
    { // abdelrahman
        string input;
        is >> input;

        try {
            num = BigInt(input);
        } catch (const invalid_argument& e) {
            is.setstate(ios::failbit);
        }

        return is;
    }

    // Friend declarations for comparison operators
    friend bool operator==(const BigInt &lhs, const BigInt &rhs); // menna
    friend bool operator<(const BigInt &lhs, const BigInt &rhs);  // lasheen
    friend BigInt operator+(BigInt lhs, const BigInt &rhs);       // fares
    friend bool operator<=(const BigInt &lhs, const BigInt &rhs); // fares
};
void removeLeadingZeros(string &s)
{
    while (s.length() > 1 && s[0] == '0')
    {
        s.erase(0, 1);
    }
}

// Binary addition operator (x + y)
BigInt operator+(BigInt lhs, const BigInt &rhs)
{ // fares
    BigInt result;
    string x = lhs.number;
    string y = rhs.number;
    int n = x.size();
    int m = y.size();

    if (n < m)
        x = string(m - n, '0') + x;
    if (m < n)
        y = string(n - m, '0') + y;
    int len = max(n, m);
    string sum(len + 1, '0'); // +1 for possible carry like 1+999=1000
    int carry = 0;
    for (int i = len - 1; i >= 0; --i)
    {
        int digitSum = (x[i] - '0') + (y[i] - '0') + carry;
        sum[i + 1] = (digitSum % 10) + '0';
        carry = digitSum / 10;
    }
    sum[0] = carry + '0';
    // Remove leading zero if not needed
    if (sum[0] == '0')
        sum.erase(0, 1);
    result.number = sum;
    return result;
}

// Binary subtraction operator (x - y)
BigInt operator-(BigInt lhs, const BigInt &rhs)
{ // abdelrahman
    // different signs then convert to addition
    if (lhs.isNegative != rhs.isNegative) {
        BigInt absRhs = rhs;
        absRhs.isNegative = !absRhs.isNegative;
        return lhs + absRhs;
    }

    // compare magnitudes of same sign
    int comp = lhs.compareMagnitude(rhs);
    if (comp == 0) {

        return BigInt(0);
    }

    BigInt result;
    string num1, num2;
    bool resultNegative;

    if (comp > 0) {// lhs > rhs
        num1 = lhs.number;
        num2 = rhs.number;
        resultNegative = lhs.isNegative;
    }
    else {// lhs < rhs
        num1 = rhs.number;
        num2 = lhs.number;
        resultNegative = !lhs.isNegative;
    }

    // pad small number with zeros
    if (num1.length() < num2.length()) {
        num1 = string(num2.length() - num1.length(), '0') + num1;
    }
    else if (num2.length() < num1.length()) {
        num2 = string(num1.length() - num2.length(), '0') + num2;
    }

    string difference;
    int borrow = 0;
    for (int i = num1.length() - 1; i >= 0; i--) { // subtraction
        int digit1 = num1[i] - '0' - borrow;
        int digit2 = num2[i] - '0';

        if (digit1 < digit2) {
            digit1 += 10;
            borrow = 1;
        }
        else
            borrow = 0;

        int diff = digit1 - digit2;
        difference = to_string(diff) + difference;
    }

    removeLeadingZeros(difference);
    result.number = difference;
    result.isNegative = resultNegative;
    return result;
}

// Binary multiplication operator (x * y)
BigInt operator*(BigInt lhs, const BigInt &rhs)
{ // menna
    BigInt result;

}



// Binary division operator (x / y)
BigInt operator/(BigInt lhs, const BigInt &rhs)
{ // lasheen
    BigInt result;
    // TODO: Implement this operator
    return result;
}

// Binary modulus operator (x % y)
BigInt operator%(BigInt lhs, const BigInt &rhs)
{ // hafez
    BigInt result;
    // TODO: Implement this operator
    return result;
}

// Equality comparison operator (x == y)
bool operator==(const BigInt &lhs, const BigInt &rhs)
{ // lasheen
    // TODO: Implement this operator
    return false;
}

// Inequality comparison operator (x != y)
bool operator!=(const BigInt &lhs, const BigInt &rhs)
{ // hafez
    // TODO: Implement this operator
    return false;
}

// Less-than comparison operator (x < y)
bool operator<(const BigInt &lhs, const BigInt &rhs)
{ // abdelrahman
    // different sign
    if (lhs.isNegative && !rhs.isNegative) return true;
    if (!lhs.isNegative && rhs.isNegative) return false;

    //compare magnitudes of same sign
    int comp = lhs.compareMagnitude(rhs);
    // both negative
    if (lhs.isNegative) {
        return comp > 0;
    }
    // both positive
    else {
        return comp < 0;
    }    return false;
}

// Less-than-or-equal comparison operator (x <= y)
bool operator<=(const BigInt &lhs, const BigInt &rhs)
{ // fares
    string x = lhs.number;
    string y = rhs.number;
    removeLeadingZeros(x);
    removeLeadingZeros(y);
    int n = x.size();
    int m = y.size();
    // both of differentt signs
    if (lhs.isNegative && !rhs.isNegative)
        return true;
    if (!lhs.isNegative && rhs.isNegative)
        return false;
    // Both same sign
    if (n < m)
        return lhs.isNegative ? false : true;
    if (n > m)
        return lhs.isNegative ? true : false;
    // n == m
    for (int i = 0; i < n; i++)
    {
        if (x[i] < y[i])
            return lhs.isNegative ? false : true;
        if (x[i] > y[i])
            return lhs.isNegative ? true : false;
    }
    // exiting loop means all digits are equal so return true

    return true;
}



// Greater-than comparison operator (x > y)

    friend bool operator>(const BigInt &lhs, const BigInt &rhs)
//menna

    {
        if (lhs.isNegative != rhs.isNegative)
            return rhs.isNegative;

        int cmp = lhs.compareMagnitude(rhs);

        if (!lhs.isNegative)
            return cmp > 0;
        else
            return cmp < 0;
    }

// Greater-than-or-equal comparison operator (x >= y)
bool operator>=(const BigInt &lhs, const BigInt &rhs)
{ // lasheen
    // TODO: Implement this operator
    return false;
}

int main()
{
    cout << "=== BigInt Class Test Program ===" << endl
         << endl;
    cout << "NOTE: All functions are currently empty." << endl;
    cout << "Your task is to implement ALL the functions above." << endl;
    cout << "The tests below will work once you implement them correctly." << endl
         << endl;

    /*
    // Test 1: Constructors and basic output
    cout << "1. Constructors and output:" << endl;
    BigInt a(12345);              // Should create BigInt from integer
    BigInt b("-67890");           // Should create BigInt from string
    BigInt c("0");                // Should handle zero correctly
    BigInt d = a;                 // Should use copy constructor
    cout << "a (from int): " << a << endl;        // Should print "12345"
    cout << "b (from string): " << b << endl;     // Should print "-67890"
    cout << "c (zero): " << c << endl;            // Should print "0"
    cout << "d (copy of a): " << d << endl << endl; // Should print "12345"

    // Test 2: Arithmetic operations
    cout << "2. Arithmetic operations:" << endl;
    cout << "a + b = " << a + b << endl;          // Should calculate 12345 + (-67890)
    cout << "a - b = " << a - b << endl;          // Should calculate 12345 - (-67890)
    cout << "a * b = " << a * b << endl;          // Should calculate 12345 * (-67890)
    cout << "b / a = " << b / a << endl;          // Should calculate (-67890) / 12345
    cout << "a % 100 = " << a % BigInt(100) << endl << endl; // Should calculate 12345 % 100

    // Test 3: Relational operators
    cout << "3. Relational operators:" << endl;
    cout << "a == d: " << (a == d) << endl;       // Should be true (12345 == 12345)
    cout << "a != b: " << (a != b) << endl;       // Should be true (12345 != -67890)
    cout << "a < b: " << (a < b) << endl;         // Should be false (12345 < -67890)
    cout << "a > b: " << (a > b) << endl;         // Should be true (12345 > -67890)
    cout << "c == 0: " << (c == BigInt(0)) << endl << endl; // Should be true (0 == 0)

    // Test 4: Unary operators and increments
    cout << "4. Unary operators and increments:" << endl;
    cout << "-a: " << -a << endl;                 // Should print "-12345"
    cout << "++a: " << ++a << endl;               // Should increment and print "12346"
    cout << "a--: " << a-- << endl;               // Should print "12346" then decrement
    cout << "a after decrement: " << a << endl << endl; // Should print "12345"

    // Test 5: Large number operations
    cout << "5. Large number operations:" << endl;
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    cout << "Very large addition: " << num1 + num2 << endl;
    cout << "Very large multiplication: " << num1 * num2 << endl << endl;

    // Test 6: Edge cases and error handling
    cout << "6. Edge cases:" << endl;
    BigInt zero(0);
    BigInt one(1);
    try {
        BigInt result = one / zero;               // Should throw division by zero error
        cout << "Division by zero succeeded (unexpected)" << endl;
    } catch (const runtime_error& e) {
        cout << "Division by zero correctly threw error: " << e.what() << endl;
    }
    cout << "Multiplication by zero: " << one * zero << endl;        // Should be "0"
    cout << "Negative multiplication: " << BigInt(-5) * BigInt(3) << endl;  // Should be "-15"
    cout << "Negative division: " << BigInt(-10) / BigInt(3) << endl;       // Should be "-3"
    cout << "Negative modulus: " << BigInt(-10) % BigInt(3) << endl;        // Should be "-1"
    */

    return 0;
}
