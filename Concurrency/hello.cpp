#include <iostream>
#include <thread>
using namespace std;
void hello() {
  cout << "Hello World, subond" << endl;
  cout << "It is a thread " << endl;
}
int main() {
  thread myt(hello);
  myt.join();
  return 0;
}
