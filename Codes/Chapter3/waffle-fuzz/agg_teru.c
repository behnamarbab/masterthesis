static inline u32 agg_teru() {
  curr_teru = 0;
  u64* current = (u64*)trace_bits;
  u32* curreru = ERUs;

  // Find the TERU
  u32 i = (MAP_SIZE >> 3);
  while(i--) {
    if (unlikely(*current)) {
      for(u32 j=0; j<8; j++) {
        curr_teru += curreru[j];
      }
    }
    current ++;
    curreru += 8;
  }

  // Check if a new max is found
  if(curr_teru > max_TERU) {
    max_TERU = curr_teru;
    return 1;
  }
  
  return 0;  
}