#include <ci_bit.hxx>
#include <io_name_vec.hxx>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace cingulata;

class MockVecElem
{
public:
  MOCK_METHOD1(set_name, void (const string&));
  MOCK_METHOD0(clr_name, void ());
  MOCK_METHOD0(read, void ());
  MOCK_METHOD1(read, void (const string& p_name));
  MOCK_METHOD0(write, void ());
  MOCK_METHOD1(write, void (const string& p_name));
  MOCK_METHOD0(encrypt, void ());
  MOCK_METHOD0(decrypt, void ());
};


class FakeVec : public IoNameVec<FakeVec>
{
public:
  vector<shared_ptr<MockVecElem>> m_vec;
  FakeVec(unsigned p_size) {
    for (int i = 0; i < p_size; ++i)
      m_vec.push_back(make_shared<MockVecElem>());
  }

  unsigned size() { return m_vec.size(); }

  const MockVecElem& operator [] (const int idx) const { return *m_vec[idx]; }
  MockVecElem& operator [] (const int idx) { return *m_vec[idx]; }

};

template<>
string IoNameVec<FakeVec>::m_name_fmt{ "%s_%d" };

using namespace ::testing;

#define TEST_CALL_EACH_BIT0(TST_NAME, NAME)  \
TEST(IoNameVec, TST_NAME) {                 \
  FakeVec vec(rand()%128);                  \
  for (int i = 0; i < vec.size(); ++i) {    \
    EXPECT_CALL(vec[i], NAME());            \
  }                                         \
  vec.NAME();                               \
}

TEST_CALL_EACH_BIT0(clr_name_each_bit_call, clr_name)
TEST_CALL_EACH_BIT0(read_each_bit_call, read)
TEST_CALL_EACH_BIT0(write_each_bit_call, write)
TEST_CALL_EACH_BIT0(encrypt_each_bit_call, encrypt)
TEST_CALL_EACH_BIT0(decrypt_each_bit_call, decrypt)

#define TEST_CALL_EACH_BIT1(TST_NAME, NAME) \
TEST(IoNameVec, TST_NAME) {                 \
  FakeVec vec(rand()%128);                  \
  for (int i = 0; i < vec.size(); ++i) {    \
    EXPECT_CALL(vec[i],                     \
      set_name(StrEq("a_" + to_string(i))));\
    EXPECT_CALL(vec[i], NAME());            \
  }                                         \
  vec.NAME("a");                               \
}

TEST_CALL_EACH_BIT1(read_name_each_bit_call, read)
TEST_CALL_EACH_BIT1(write_name_each_bit_call, write)


TEST(IoNameVec, set_format_name_each_bit_call) {
  {
    FakeVec vec(rand()%128);
    for (int i = 0; i < vec.size(); ++i) {
      EXPECT_CALL(vec[i], set_name(StrEq("a_" + to_string(i))));
    }
    vec.set_name("a");
  }
  {
    IoNameVec<FakeVec>::set_format("%s:%d");

    FakeVec vec(rand()%128);
    for (int i = 0; i < vec.size(); ++i) {
      EXPECT_CALL(vec[i], set_name(StrEq("b:" + to_string(i))));
    }
    vec.set_name("b");

    IoNameVec<FakeVec>::set_format("%s_%d");
  }
}

