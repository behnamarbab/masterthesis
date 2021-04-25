static inline u8 has_new_max() {
  // #define MAX_CNT_MULT 1.05
  int ret = 0;
  for (int i = 0; i < ICNT_SIZE; i++) {
    if (unlikely(icnt_bits[i]) && unlikely(icnt_bits[i] > MAX_CNT_MULT*max_icnts[i])) {
      if(likely(ret<2)) {
        ret = 2;
        max_icnts[i] = icnt_bits[i];
      }
      else ret = 1;
    }
  }
  return ret;
}