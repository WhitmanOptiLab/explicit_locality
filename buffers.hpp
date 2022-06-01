#ifndef BUFFERS_HPP
#define BUFFERS_HPP
#include <cstddef>
#include <vector>
#include <array>

template<class T, std::size_t N> class local_buffer;

template<class T>
class global_buffer {
  template<class U, std::size_t N> friend class local_buffer;

 private:
  std::vector<T> contents;

 public:
  global_buffer(T* data, std::size_t size) : contents(data, data+size) {};
  bool validate(T* data) {
    return std::equal(contents.begin(), contents.end(), data);
  }
};

template<class T, std::size_t N>
class local_buffer : public std::array<T,N> {
 private:
 
 public:
  void copy_to(global_buffer<T>& buf, std::size_t base, std::size_t len) {
    std::copy(this->begin(), this->begin()+len, buf.contents.begin()+base);
  }
  void copy_from(global_buffer<T>& buf, std::size_t base, std::size_t len) {
    std::copy(buf.contents.begin()+base, buf.contents.begin()+base+len, this->begin());
  }
  //Copy the 2-D tile of indexes [base_r][base_c] to 
  // [base_r+len_r][base_c+len_c], where the whole row length is 
  // g_rstride in the global buffer
  void copy2D_to(global_buffer<T>& buf, std::size_t base_r, std::size_t len_r, 
                 std::size_t g_rstride, std::size_t base_c, std::size_t len_c) {
    for (std::size_t i = base_r; i < base_r+len_r; i++) {
      std::copy(this->begin()+i*len_c, this->begin()+(i+1)*len_c, 
          buf.contents.begin()+i*g_rstride+base_c);
    }
  }
  void copy2D_from(global_buffer<T>& buf, std::size_t base_r, std::size_t len_r, 
                 std::size_t g_rstride, std::size_t base_c, std::size_t len_c) {
    for (std::size_t i = base_r; i < base_r+len_r; i++) {
      std::copy(buf.contents.begin()+i*g_rstride+base_c, 
                  buf.contents.begin()+i*g_rstride+base_c+len_c,
                  this->begin()+i*len_c);
    }
  }
};

#endif //BUFFERS_HPP
