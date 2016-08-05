# BigNbr
A library for calculating with big numbers written in plain C. (GiantNbr's small brother)

### Why developing two library for doing the same thing?
GiantNbr use integer arrays (multiple digits) for storing the numbers, thats why it is a little bit more complicated. BigNbr uses char arrays for storing digit by digit values. Thats why this is easy to understand and implement.

### How it started
To be onest i tried to figure out how division can be implemented in GiantNbr, but it was to complicated. I started developing BigNbr to figure out a simple algorithm for doing division and thought this could be worth sharing with others.
