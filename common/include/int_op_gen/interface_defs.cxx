
template<typename _impl>
CiBitVector IntOpGen<_impl>::add(const CiBitVector& lhs, const CiBitVector& rhs) const
{
  return impl_m.add(lhs, rhs);
}

template<typename _impl>
CiBitVector IntOpGen<_impl>::neg(const CiBitVector& lhs) const
{
  return impl_m.neg(lhs);
}

template<typename _impl>
CiBitVector IntOpGen<_impl>::mul(const CiBitVector& lhs, const CiBitVector& rhs) const
{
  return impl_m.mul(lhs, rhs);
}

template<typename _impl>
CiBit IntOpGen<_impl>::not_equal(const CiBitVector& lhs, const CiBitVector& rhs) const
{
  return impl_m.not_equal(lhs, rhs);
}

template<typename _impl>
CiBit IntOpGen<_impl>::lower(const CiBitVector& lhs, const CiBitVector& rhs) const
{
  return impl_m.lower(lhs, rhs);
}
