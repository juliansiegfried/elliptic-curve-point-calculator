#include <iostream>

using namespace std;

class Point
{
public:
	int x, y;
	bool isNeutralElement;
	Point() : x(0), y(0), isNeutralElement(false) {}

	void askCoordinates()
	{
		do
		{
			cin.clear(); // clear failbit if input is faulty
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad characters

			cout << "Is this point the neutral element? (1 for yes, 0 for no): ";
		} while (!(cin >> isNeutralElement)); // I actually tend to ignore this question so that's why I use this little sanity check to prevent an infinite loop

		if (!isNeutralElement)
		{
			cout << "x: "; cin >> x;
			cout << "y: "; cin >> y;
		}
	}

	bool isElementOfCurve(int a, int b, int p)
	{
		if ((y * y) % p == (x * x * x + a * x + b) % p) // y == x mod p?
			return true;

		cout << "This point is not an element of the curve you're using." << endl;
		return false;
	}

	void printAsResultingPoint()
	{
		if (isNeutralElement)
			cout << "The point you're looking for is: O (the neutral element)" << endl;
		else
			cout << "The point you're looking for is: (" << x << ", " << y << ")" << endl;
	}
};

int findModularInverse(int a, int m)
{
	a %= m;
	for (int x = 1; x < m; x++)
		if ((a * x) % m == 1)
			return x;

	return 0; // this means that there is no modular inverse: P + (-P) = O -> the resulting point will be the neutral element
}

int modulo(int a, int m) // apparantly, % is not supposed to be used on negative numbers, i.e. my system says: -2 % 17 = 16
{
	return a >= m ? a % m : a >= 0 ? a : (a % m + m) % m;
}

int calculateS(Point point1, Point point2, int a, int p)
{
	if (point1.x == point2.x && point1.y == point2.y) // if points are equal, double the point (tangent)
		return (3 * point1.x * point1.x + a) * findModularInverse(2 * point1.y, p) % p;
	else // else add the points (secant)
		return modulo(point2.y - point1.y, p) * findModularInverse(modulo(point2.x - point1.x, p), p) % p;
}

bool isPrimeNumber(int n) // 6k +- 1 optimized trial division as seen on: https://en.wikipedia.org/wiki/Primality_test
{
	// filter some trivial cases
	if (n <= 3)
		return n > 1;
	if (n % 2 == 0 || n % 3 == 0)
		return false;

	for (int i = 5; i * i <= n; i = i + 6) // the actual optimized trial division
		if (n % i == 0 || n % (i + 2) == 0)
			return false;

	return true;
}

bool isCurveCorrect(int a, int b, int p)
{
	// are a, b elements of Z_p? Is p an allowed integer (will be negative if not)? Is p prime? Do a and b meet the condition 4a^3 + 27b^2 != 0?
	return a >= 0 && a < p && b >= 0 && b < p && p > 0 && isPrimeNumber(p) && (4 * a * a * a + 27 * b * b) % p != 0;
}

int main()
{
	int p = 0, a = 0, b = 0, s;

	// ask for correct user input
	while (!isCurveCorrect(a, b, p))
	{
		cout << "Please use correct parameters (see documentation):" << endl;
		cout << "Enter parameter p: "; cin >> p;
		cout << "Enter parameter a: "; cin >> a;
		cout << "Enter parameter b: "; cin >> b;
	}

	// do multiple calculations on the same curve
	while (true)
	{
		Point p1, p2, p3; // p1, p2 are user inputs, p3 will be the result

		cout << endl << "Enter the first point:" << endl;
		p1.askCoordinates();
		if (!p1.isNeutralElement && !p1.isElementOfCurve(a, b, p))
			continue;

		cout << "Enter the second point:" << endl;
		p2.askCoordinates();
		if (!p2.isNeutralElement && !p2.isElementOfCurve(a, b, p))
			continue;

		if (p1.isNeutralElement || p2.isNeutralElement) // if one point is O: P + O = P; if both points are O: O + O = O
		{
			if (p1.isNeutralElement && p2.isNeutralElement)
			{
				p3.isNeutralElement = true;
				p3.printAsResultingPoint();
				continue;
			}

			if (p1.isNeutralElement)
			{
				p2.printAsResultingPoint();
				continue;
			}

			if (p2.isNeutralElement)
			{
				p1.printAsResultingPoint();
				continue;
			}
		}

		if (p1.x == p2.x && p1.y != p2.y) // if both points aren't O but are self-inverse: P + (-P) = O
		{
			p3.isNeutralElement = true;
			p3.printAsResultingPoint();
			continue;
		}

		s = calculateS(p1, p2, a, p); // this fn also determines wether the points are equal

		cout << "Using parameters: a = " << a << ", b = " << b << ", p = " << p << ", s = " << s << endl;

		// calculate the resulting point
		p3.x = modulo(s * s - p1.x - p2.x, p);
		p3.y = modulo(s * (p1.x - p3.x) - p1.y, p);

		p3.printAsResultingPoint();
	}
}
