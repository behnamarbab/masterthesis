/* Check if the current execution path brings anything new to the table. Update virgin bits to reflect the finds. Returns 1 if the only change is the hit-count for a particular tuple; 2 if there are new tuples seen. Updates the map, so subsequent calls will always return 0.

   This function is called after every exec() on a fairly large buffer, so it needs to be fast. We do this in 32-bit and 64-bit flavors. */

static inline u8 has_new_bits(u8* virgin_map) {
  u64* current = (u64*)trace_bits;
  u64* virgin  = (u64*)virgin_map;
  u32  i = (MAP_SIZE >> 3);
  while (i--) {

  /* Optimize for (*current & *virgin) == 0 - i.e., no bits in current bitmap that have not been already cleared from the virgin map - since this will almost always be the case. */

    if (unlikely(*current) && unlikely(*current & *virgin)) {
      if (likely(ret < 2)) {
        u8* cur = (u8*)current;
        u8* vir = (u8*)virgin;

        /* Looks like we have not found any new bytes yet; see if any non-zero bytes in current[] are pristine in virgin[]. */
        if ((cur[0] && vir[0] == 0xff) || (cur[1] && vir[1] == 0xff) ||
            (cur[2] && vir[2] == 0xff) || (cur[3] && vir[3] == 0xff) ||
            (cur[4] && vir[4] == 0xff) || (cur[5] && vir[5] == 0xff) ||
            (cur[6] && vir[6] == 0xff) || (cur[7] && vir[7] == 0xff)) ret = 2;
        else ret = 1;
      }
      *virgin &= ~*current;
    }
    current++; virgin++;
  }

  if (ret && virgin_map == virgin_bits) bitmap_changed = 1;
  return ret;
}