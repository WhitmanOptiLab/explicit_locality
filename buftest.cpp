#include "buffers.hpp"
#include <iostream>

int main() {
  int test[4] = {1, 2, 3, 4};
  global_buffer<int> gbuf(test, 4);
  local_buffer<int, 2> lbuf;
  lbuf.copy_from(gbuf, 0, 2);
  std::cout << lbuf[0] << ',' << lbuf[1] << std::endl;
  lbuf.copy_from(gbuf, 2, 2);
  std::cout << lbuf[0] << ',' << lbuf[1] << std::endl;
  lbuf[0] = 42;
  lbuf[1] = 1024;
  lbuf.copy_to(gbuf, 0, 2);

  int result[4] = {42, 1024, 3, 4};
  std::cout << gbuf.validate(result) << std::endl;

  return 0;
}
