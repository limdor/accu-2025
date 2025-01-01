#include <iostream>

unsigned long compute(unsigned long n)
{
    if (n == 0)
    {
        return 0;
    }
    return 1 + compute(n - 1);
}

int main()
{
    int n;
    do
    {
        std::cout << "Number to be computed: ";
        std::cin >> n;
        std::cout << "Computed number: " << compute(n) << "\n";
    } while (n != 0);
    return 0;
}
