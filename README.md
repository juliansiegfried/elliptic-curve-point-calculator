# elliptic-curve-point-calculator
Just a small calculator you can use for point addition on cryptographic elliptic curves.

Please note that this program is designed to be used on elliptic curves of the form:<br/>
y² = x³ + ax + b mod p<br/>
where a, b are elements of the ring of integers modulo p.

Furthermore, the parameters a and b need to fulfill the following condition:<br/>
4 * a³ + 27 * b² ≠ 0 mod p<br/>
This program does not verify this requirement.

I have created this program as a part of a homework of Christof Paars lecture "Introduction to Cryptography 2". All calculations done by this program are based on his and Jan Pelzl's book "Understanding Cryptography: A Textbook for Students and Practitioners". Shout-outs to them!
