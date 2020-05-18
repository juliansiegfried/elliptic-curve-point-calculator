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
			cin.clear();
			cout << "Is this point the neutral element? (1 for yes, 0 for no): "; cin >> isNeutralElement;
		} while (cin.fail()); // I actually tend to ignore this question so that's why I use this little sanity check to prevent an infinite loop

		if (!isNeutralElement)
		{
			cout << "x: "; cin >> x;
			cout << "y: "; cin >> y;
		}
	}

	bool isElementOfCurve(unsigned int a, unsigned int b, unsigned int p)
	{
		if ((y * y) % p == (x * x * x + a * x + b) % p)
			return true;

		cout << "This point is not an element of the curve you're using." << endl;
		return false;
	}

	void printAsResultingPoint()
	{
		if (isNeutralElement)
			cout << "The point you're looking for is: O" << endl;
		else
			cout << "The point you're looking for is: (" << x << ", " << y << ")" << endl;
	}
};

unsigned int findModularInverse(unsigned int a, unsigned int m)
{
	a %= m;
	for (unsigned int x = 1; x < m; x++)
		if ((a * x) % m == 1)
			return x;

	cout << endl << "Error: The modular inverse couldnt be found";
	exit(1);
}

unsigned int moduloOfNegative(int a, unsigned int m) // apparantly, % is not supposed to be used on negative numbers, i.e. my system says: -2 % 17 = 16
{
	return a + (unsigned int) ceil(abs((float) a / m)) * m;
}

unsigned int calculateS(Point point1, Point point2, unsigned int a, unsigned int p)
{
	if (point1.x == point2.x && point1.y == point2.y) // if points are equal, double the point
	{
		return (3 * point1.x * point1.x + a) * findModularInverse(2 * point1.y, p) % p;
	}
	else // add the points
	{
		int numerator = point2.y - point1.y;
		if (numerator < 0)
			numerator = moduloOfNegative(numerator, p);

		int denominator = point2.x - point1.x;
		if (denominator < 0)
			denominator = moduloOfNegative(denominator, p);

		return numerator * findModularInverse(denominator, p) % p;
	}
}

bool isCurveCorrect(unsigned int a, unsigned int b, unsigned int p)
{
	// are a, b elements of Z_p? Is p an allowed integer (will be negative if not)? Do a and b meet the condition 4a^3 + 27b^2 != 0?
	return a > 0 && a < p && b > 0 && b < p && p > 0 && (4 * a * a * a + 27 * b * b) % p != 0;
}

int main()
{
	int p = 0, a = 0, b = 0, s;

	// ask for correct user input
	while (!isCurveCorrect(a, b, p))
	{
		cout << "Please use parameters that are elements of Z_p:" << endl;
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

		if (p1.isNeutralElement || p2.isNeutralElement)
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

		s = calculateS(p1, p2, a, p); // this fn also determines wether the points are equal

		cout << "Using parameters: a = " << a << ", b = " << b << ", p = " << p << ", s = " << s << endl;

		// calculate the resulting point
		p3.x = (s * s - p1.x - p2.x);
		p3.y = (s * (p1.x - p3.x) - p1.y);

		if (p3.x >= 0)
			p3.x %= p;
		else
			p3.x = moduloOfNegative(p3.x, p);

		if (p3.y >= 0)
			p3.y %= p;
		else
			p3.y = moduloOfNegative(p3.y, p);

		p3.printAsResultingPoint();
	}
}
