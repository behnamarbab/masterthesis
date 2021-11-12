static inline u8 is_exhaustive() {
  u32 teru = 0;
  u64* current = (u64*)trace_bits;
  u32* curreru = ERUs;
  u32 i = (MAP_SIZE >> 3);

  while(i--) {
    if (unlikely(*current)) {
      for(u32 j=i<<3; j<i<<3+8; j++) {
        teru += *(curreru+j);
      }
    }
    current ++;
    curreru += 8;
  }

  return teru >= (max_TERU>>1);
}