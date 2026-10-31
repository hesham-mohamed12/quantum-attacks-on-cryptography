#include <iostream>
#include <vector>
#include <set>
#include <sstream>
using namespace std;

vector<long long> possible_a;
set<long long> possible_r;

long long gcd(long long x, long long y)
{
    while (y != 0)
    {
        long long temp = y;
        y = x % y;
        x = temp;
    }

    return x;
}

long long addMod(long long a, long long b, long long mod)
{
    a %= mod;
    b %= mod;

    if (a >= mod - b)
        return a - (mod - b);

    return a + b;
}

long long multiplyMod(long long a, long long b, long long mod)
{
    a %= mod;
    b %= mod;

    long long result = 0;

    while (b > 0)
    {
        if (b % 2 == 1)
            result = addMod(result, a, mod);

        a = addMod(a, a, mod);
        b /= 2;
    }

    return result;
}

long long powerMod(long long base, long long exp, long long mod)
{
    long long result = 1;
    base %= mod;

    while (exp > 0)
    {
        if (exp % 2 == 1)
            result = multiplyMod(result, base, mod);

        base = multiplyMod(base, base, mod);
        exp /= 2;
    }

    return result;
}

long long getD(long long e, long long phi)
{
    for (long long i = 1; i < phi; i++)
    {
        if (multiplyMod(e, i, phi) == 1)
            return i;
    }

    return 0;
}

long long P = 0, Q = 0;

void get_R(long long n)
{
    for (long long a = 2; a < n; a++)
    {
        if (gcd(a, n) != 1)
        {
            long long factor = gcd(a, n);

            if (factor > 1 && factor < n)
            {
                P = factor;
                Q = n / factor;

                cout << "\nFactors Found Directly!" << endl;
                return;
            }

            continue;
        }

        long long check1 = 1;
        long long r = 0;

        while (true)
        {
            check1 = multiplyMod(check1, a, n);
            r++;

            if (check1 == 1)
                break;
        }

        possible_r.insert(r);

        cout << "\na = " << a << " , r = " << r << endl;

        if (r % 2 == 0)
        {
            long long x = powerMod(a, r / 2, n);

            cout << "a^(r/2) mod N = " << x << endl;

            if (x != 1 && x != n - 1)
            {
                long long p = gcd(x - 1, n);
                long long q = gcd(x + 1, n);

                cout << "P = " << p << endl;
                cout << "Q = " << q << endl;

                if (p > 1 && q > 1 && n % p == 0 && n / p == q)
                {
                    cout << "Valid Factors Found!" << endl;

                    P = p;
                    Q = q;

                    return;
                }
                else
                {
                    cout << "Invalid Factors" << endl;
                }
            }
            else
            {
                cout << "Unsuccessful A" << endl;
            }
        }
        else
        {
            cout << "R is odd" << endl;
        }
    }
}

void print_all_word(long long d, vector<long long> encryption, long long N)
{
    string s = "";

    for (int i = 0; i < encryption.size(); i++)
    {
        long long char1 = powerMod(encryption[i], d, N);

        cout << encryption[i] << " -> " << char1;

        if (char1 == 0)
        {
            s += " ";
            cout << " -> SPACE" << endl;
        }
        else if (char1 >= 1 && char1 <= 26)
        {
            char letter = char('A' + char1 - 1);
            s += letter;

            cout << " -> " << letter << endl;
        }
        else
        {
            cout << " -> Invalid character value" << endl;
        }
    }

    cout << "\nDecrypted Message: " << s << endl;
}

int main()
{
    long long n, e;

    cin >> n;
    cin >> e;

    if (n < 3)
    {
        cout << "Invalid N";
        return 0;
    }

    get_R(n);

    if (P == 0 || Q == 0)
    {
        cout << "\nNo valid factors found!";
        return 0;
    }

    long long Euler_Totient = (P - 1) * (Q - 1);

    long long private_key = getD(e, Euler_Totient);

    if (private_key == 0)
    {
        cout << "\nInvalid E";
        return 0;
    }

    cout << "\nFinal P = " << P << endl;
    cout << "Final Q = " << Q << endl;
    cout << "Euler Totient = " << Euler_Totient << endl;
    cout << "Private Key = " << private_key << endl;
    cout << "\nEnter Encrypted Message: ";

    string line;
    vector<long long> encryption;

    getline(cin >> ws, line);

    stringstream ss(line);
    long long num;

    while (ss >> num)
        encryption.push_back(num);

    print_all_word(private_key, encryption, n);

    return 0;
}