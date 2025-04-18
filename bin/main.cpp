#include <iostream>
#include "../lib/CCircularBuffer.h"


int main() {
    CCircularBuffer<int> test_buffer(150);

    CCircularBuffer<int>::iterator it;  // Default-constructed
    it = test_buffer.begin();

    for (int i = 0; i < 100; ++i) test_buffer.push_back(i);

    CCircularBuffer<int>::iterator ptr = test_buffer.begin();
    
    for (CCircularBuffer<int>::iterator ptr = test_buffer.end(); ptr != test_buffer.begin(); ptr--) {}

    return 0;
}
