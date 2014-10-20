#include <iostream>

#include <PSim/PSim.h>

int main()
{
    PSim::CoordinateInitialValueParameter p;
    p.setSerializeAllDefaults(true);
    p.print("testing.xml");
    std::cout << "Hello world!" << std::endl;
}
