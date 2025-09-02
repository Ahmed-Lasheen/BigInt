#include <iostream>
#include <string>
#include <vector>
using namespace std;

class BigInt
{
    private:
    string number;   // Stores the number as a string
    bool isNegative; // True if number is negative

    // Remove unnecessary leading zeros from the number string
    void removeLeadingZeros() { 
        // TODO: Implement this function
        while(number.length()>1&&number[0]=='0')
        {
            number.erase(0,1);
        }
        if (number == "0")
        {
            isNegative = false; 
        }
    }    

    // Compare absolute values of two BigInts (ignore signs)
    // Returns: 1 if |this| > |other|, 0 if equal, -1 if |this| < |other|
    int compareMagnitude(const BigInt& other) const {
        // TODO: Implement this function
        if(this->number.length()>other.number.length())
            return 1;
        else if(this->number.length()<other.number.length())
            return -1;
        else
        {
            int i=0;
            while(i<this->number.length())
            {
                if(this->number[i]>other.number[i])
                {
                    return 1;
                }
                else if(this->number[i]<other.number[i])
                {
                    return -1;
                }
                i++;
            }
        }
        return 0;
    }
    //string digits; // string used to convert int64_t value to string

public:
    // Default constructor - initialize to zero
    BigInt()
    { // Hafez
      // TODO: Implement this constructor
      number="0";
    }

    // Constructor from 64-bit integer
    BigInt(int64_t value)
    { // Fares
        this->number = to_string(value);
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
    removeLeadingZeros();
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
        result=*this;
        return result;
    }

    // Addition assignment operator (x += y)
    BigInt &operator+=(const BigInt &other)
    { // hafez
        // TODO: Implement this operator
        *this=*this+other;
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

        removeLeadingZeros(); // remove the leading zeros from final number
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
        *this=(*this)%other;
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
    if (number == "0") {
        number = "1";
        isNegative = true;
    } else if (!isNegative) {
        BigInt one("1");
        *this = *this - one;
    } else {
        BigInt one("1");
        *this = *this + one;
    }
    removeLeadingZeros();
    return *this;
    }

    // Post-decrement operator (x--)
    BigInt operator--(int)
    { // lasheen
        BigInt temp;
        // TODO: Implement this operator
        temp=*this;
        --(*this);
        return temp;
    }

    // Convert BigInt to string representation
    string toString() const
    { // hafez
        if (isNegative && number != "0")
        return "-" + number;
        return number;
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
    friend BigInt operator%(BigInt lhs, const BigInt &rhs);
    friend bool operator==(const BigInt &lhs, const BigInt &rhs); // menna
    friend bool operator<(const BigInt &lhs, const BigInt &rhs);  // lasheen
    friend BigInt operator+(BigInt lhs, const BigInt &rhs);       // fares
    friend bool operator<=(const BigInt &lhs, const BigInt &rhs); // fares
    friend BigInt operator-(BigInt lhs, const BigInt &rhs);
    friend BigInt operator/(BigInt lhs, const BigInt &rhs);
    friend bool operator>(const BigInt &lhs, const BigInt &rhs);
    friend BigInt operator*(BigInt lhs, const BigInt &rhs); 
    friend bool operator>=(const BigInt &lhs, const BigInt &rhs);

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

    // pad with zeros to make equal length
    int maxLength = max(num1.length(), num2.length());
    num1 = string(maxLength - num1.length(), '0') + num1;
    num2 = string(maxLength - num2.length(), '0') + num2;

    string difference;
    int borrow = 0;
    for (int i = maxLength - 1; i >= 0; i--) {
        int digit1 = (num1[i] - '0') - borrow;
        int digit2 = num2[i] - '0';

        if (digit1 < digit2) {
            digit1 += 10;
            borrow = 1;
        }
        else {
            borrow = 0;
        }

        int diff = digit1 - digit2;
        difference = to_string(diff) + difference;
    }

    // Remove leading zeros
    while (difference.length() > 1 && difference[0] == '0') {
        difference.erase(0, 1);
    }

    result.number = difference;
    result.isNegative = (difference != "0") && resultNegative;
    return result;
}

// Binary multiplication operator (x * y)
// helper: add leading zeros for shifting
static string shiftLeft(const string &s, int zeros) {
    return s + string(zeros, '0');
}

// helper: normalize string (remove leading zeros)
static string stripLeadingZeros(const string &s) {
    size_t pos = s.find_first_not_of('0');
    if (pos == string::npos) return "0";
    return s.substr(pos);
}

// schoolbook multiplication for small inputs
static string multiplySimple(const string &a, const string &b) {
    int n = a.size(), m = b.size();
    vector<int> result(n + m, 0);

    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            int prod = (a[i] - '0') * (b[j] - '0');
            int sum = prod + result[i + j + 1];
            result[i + j + 1] = sum % 10;
            result[i + j] += sum / 10;
        }
    }

    string s;
    for (int x : result) s.push_back(x + '0');
    return stripLeadingZeros(s);
}

// karatsuba recursive multiplication
static string karatsuba(const string &x, const string &y) {
    int n = max(x.size(), y.size());

    // small numbers → use schoolbook
    if (n <= 32) return multiplySimple(x, y);

    // pad to equal length
    int half = n / 2;
    string x1 = x.substr(0, x.size() - half);
    string x0 = x.substr(x.size() - half);
    string y1 = y.substr(0, y.size() - half);
    string y0 = y.substr(y.size() - half);

    if (x1.empty()) x1 = "0";
    if (y1.empty()) y1 = "0";

    string z0 = karatsuba(x0, y0);
    string z2 = karatsuba(x1, y1);

    // (x1+x0)(y1+y0) - z0 - z2
    BigInt bx1(x1), bx0(x0), by1(y1), by0(y0);
    BigInt sumx = bx1 + bx0;
    BigInt sumy = by1 + by0;
    string z1 = karatsuba(sumx.toString(), sumy.toString());

    BigInt bz1(z1), bz0(z0), bz2(z2);
    BigInt mid = bz1 - bz0 - bz2;

    string part1 = shiftLeft(bz2.toString(), 2 * half);
    string part2 = shiftLeft(mid.toString(), half);
    BigInt result(part1);
    result += BigInt(part2);
    result += BigInt(z0);

    return result.toString();
}

// final operator*
BigInt operator*(BigInt lhs, const BigInt &rhs) {
    BigInt result;
    result.isNegative = lhs.isNegative ^ rhs.isNegative;

    result.number = karatsuba(lhs.number, rhs.number);
    result.removeLeadingZeros();
    if (result.number == "0") result.isNegative = false; // avoid "-0"
    return result;
}



// Binary division operator (x / y)
BigInt operator/(BigInt lhs, const BigInt &rhs)
{ // lasheen
    if (rhs.number == "0") {
        throw runtime_error("Division by zero");
    }

    bool negativeResult = lhs.isNegative != rhs.isNegative;
    
    // Use absolute values
    BigInt numerator = lhs;
    numerator.isNegative = false;
    BigInt denominator = rhs;
    denominator.isNegative = false;

    if (numerator < denominator) {
        return BigInt(0);
    }

    string resultStr;
    BigInt current("0");
    
    for (size_t i = 0; i < numerator.number.length(); i++) {
        current = current * BigInt(10) + BigInt(string(1, numerator.number[i]));
        current.removeLeadingZeros();
        
        int count = 0;
        while (current >= denominator) {
            current = current - denominator;
            count++;
        }
        
        resultStr += to_string(count);
    }

    // Remove leading zeros
    while (resultStr.length() > 1 && resultStr[0] == '0') {
        resultStr.erase(0, 1);
    }

    BigInt result(resultStr);
    result.isNegative = (resultStr != "0") && negativeResult;
    return result;
}

// Binary modulus operator (x % y)
BigInt operator%(BigInt lhs, const BigInt &rhs)
{ // hafez
    if (rhs.number == "0") {
        throw runtime_error("Modulus by zero");
    }

    BigInt quotient = lhs / rhs;
    BigInt product = rhs * quotient;
    BigInt result = lhs - product;
    
    return result;
}

// Equality comparison operator (x == y)
bool operator==(const BigInt &lhs, const BigInt &rhs)
{ // lasheen
    // TODO: Implement this operator
    if((lhs.isNegative==rhs.isNegative)&&(lhs.number==rhs.number))
        return true;
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

bool operator>(const BigInt &lhs, const BigInt &rhs)
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
    return !(lhs < rhs);
}

int main()
{
    cout << "=== BigInt Class Test Program ===" << endl
         << endl;
    cout << "NOTE: All functions are currently empty." << endl;
    cout << "Your task is to implement ALL the functions above." << endl;
    cout << "The tests below will work once you implement them correctly." << endl
         << endl;

    
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
    

    return 0;
}