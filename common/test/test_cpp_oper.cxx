/**
 * Application to test signedness and bit-size of native types in C++
 */
#include <iostream>
#include <cstdint>

using namespace std;

template<typename T> string typeid_str(T a) { return "UNKNOWN"; }
template<> string typeid_str(bool a) { return "bool"; }
template<> string typeid_str(int8_t a) { return "int8_t"; }
template<> string typeid_str(int16_t a) { return "in16_t"; }
template<> string typeid_str(int32_t a) { return "int32_t"; }
template<> string typeid_str(int64_t a) { return "int64_t"; }
template<> string typeid_str(uint8_t a) { return "uint8_t"; }
template<> string typeid_str(uint16_t a) { return "uin16_t"; }
template<> string typeid_str(uint32_t a) { return "uint32_t"; }
template<> string typeid_str(uint64_t a) { return "uint64_t"; }

template<typename T1, typename T2>
void f(T1 a, T2 b) {
  auto c = a + b;
  cout << typeid_str(a) << " + " << typeid_str(b) << " = " << typeid_str(c) << endl;
}

template<typename T1, typename T2>
void f1(T1 a) {
  T2 c = (T2)a;
  cout << "(" << typeid_str(c) << ")" << typeid_str(a) << " : ";
  cout << "(" << typeid_str(c) << ")" << a << " = " << c << endl;
}

int main()
{
  cout << "2-input operators" << endl;
  f<int32_t, int32_t>(-3, 2);
  f<int32_t, int64_t>(-3, 2);
  f<int64_t, int32_t>(-3, 2);
  f<int64_t, int64_t>(-3, 2);
  cout << endl;

  f<int32_t, uint32_t>(-3, 2);
  f<int32_t, uint64_t>(-3, 2);
  f<int64_t, uint32_t>(-3, 2);
  f<int64_t, uint64_t>(-3, 2);
  cout << endl;

  f<uint32_t, int32_t>(-3, 2);
  f<uint32_t, int64_t>(-3, 2);
  f<uint64_t, int32_t>(-3, 2);
  f<uint64_t, int64_t>(-3, 2);
  cout << endl;

  f<uint32_t, uint32_t>(-3, 2);
  f<uint32_t, uint64_t>(-3, 2);
  f<uint64_t, uint32_t>(-3, 2);
  f<uint64_t, uint64_t>(-3, 2);
  cout << endl;

  cout << "cast operators" << endl;
  f1<int32_t, int32_t>(-3);
  f1<int32_t, int64_t>(-3);
  f1<int64_t, int32_t>(-3);
  f1<int64_t, int64_t>(-3);
  cout << endl;

  f1<int32_t, uint32_t>(-3);
  f1<int32_t, uint64_t>(-3);
  f1<int64_t, uint32_t>(-3);
  f1<int64_t, uint64_t>(-3);
  cout << endl;

  f1<uint32_t, int32_t>(-3);
  f1<uint32_t, int64_t>(-3);
  f1<uint64_t, int32_t>(-3);
  f1<uint64_t, int64_t>(-3);
  cout << endl;

  f1<uint32_t, uint32_t>(-3);
  f1<uint32_t, uint64_t>(-3);
  f1<uint64_t, uint32_t>(-3);
  f1<uint64_t, uint64_t>(-3);
  cout << endl;
}
