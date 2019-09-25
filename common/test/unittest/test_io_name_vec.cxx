/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.
    Contributor(s): Cingulata team

    This software is governed by the CeCILL-C license under French law and
    abiding by the rules of distribution of free software.  You can  use,
    modify and/ or redistribute the software under the terms of the CeCILL-C
    license as circulated by CEA, CNRS and INRIA at the following URL
    "http://www.cecill.info".

    As a counterpart to the access to the source code and  rights to copy,
    modify and redistribute granted by the license, users are provided only
    with a limited warranty  and the software's author,  the holder of the
    economic rights,  and the successive licensors  have only  limited
    liability.

    The fact that you are presently reading this means that you have had
    knowledge of the CeCILL-C license and that you accept its terms.
*/

#include <ci_bit.hxx>
#include <io_name_vec.hxx>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace std;
using namespace cingulata;

class MockVecElem {
public:
  MOCK_METHOD0(read, void());
  MOCK_METHOD1(read, void(const string &p_name));
  MOCK_CONST_METHOD0(write, void());
  MOCK_CONST_METHOD1(write, void(const string &p_name));
  MOCK_METHOD0(encrypt, void());
  MOCK_METHOD0(decrypt, void());
  MOCK_CONST_METHOD0(get_val, bit_plain_t());
};

class FakeVec : public IoNameVec<FakeVec> {
public:
  vector<shared_ptr<MockVecElem>> m_vec;
  FakeVec(unsigned p_size) {
    for (int i = 0; i < p_size; ++i)
      m_vec.push_back(make_shared<MockVecElem>());
  }

  unsigned size() const { return m_vec.size(); }

  const MockVecElem &operator[](const int idx) const { return *m_vec[idx]; }
  MockVecElem &operator[](const int idx) { return *m_vec[idx]; }
};

template <> string IoNameVec<FakeVec>::m_name_fmt{"%s_%d"};

using namespace ::testing;

#define TEST_CALL_EACH_BIT0(TST_NAME, NAME)                                    \
  TEST(IoNameVec, TST_NAME) {                                                  \
    FakeVec vec(rand() % 128);                                                 \
    for (int i = 0; i < vec.size(); ++i) {                                     \
      EXPECT_CALL(vec[i], NAME());                                             \
    }                                                                          \
    vec.NAME();                                                                \
  }

TEST_CALL_EACH_BIT0(read_each_bit_call, read)
TEST_CALL_EACH_BIT0(write_each_bit_call, write)
TEST_CALL_EACH_BIT0(encrypt_each_bit_call, encrypt)
TEST_CALL_EACH_BIT0(decrypt_each_bit_call, decrypt)
TEST_CALL_EACH_BIT0(get_bits_val_each_bit_call, get_val)

#define TEST_CALL_EACH_BIT1(TST_NAME, NAME)                                    \
  TEST(IoNameVec, TST_NAME) {                                                  \
    FakeVec vec(rand() % 128);                                                 \
    for (int i = 0; i < vec.size(); ++i) {                                     \
      EXPECT_CALL(vec[i], NAME("a_" + to_string(i)));                          \
    }                                                                          \
    vec.NAME("a");                                                             \
  }

TEST_CALL_EACH_BIT1(read_name_each_bit_call, read)
TEST_CALL_EACH_BIT1(write_name_each_bit_call, write)
