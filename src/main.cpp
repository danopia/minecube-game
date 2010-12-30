#include <octree.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    Octree<float> tree = Octree<float>(5.0);
    cout << "Created Octree with value of " << tree.value << "\n";
    cin;
}