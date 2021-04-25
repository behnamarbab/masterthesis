havoc_stage:
  /* The havoc stage mutation code is also invoked when splicing files; if the splice_cycle variable is set, generate different descriptions and such. */
  
  if (!splice_cycle) {
    stage_max   = (doing_det ? HAVOC_CYCLES_INIT : HAVOC_CYCLES) *
                  perf_score / havoc_div / 100;
  } 
  else {
    stage_max   = SPLICE_HAVOC * perf_score / havoc_div / 100;
  }

  /* We essentially just do several thousand runs (depending on perf_score) where we take the input file and make random stacked tweaks. */
  for (stage_cur = 0; stage_cur < stage_max; stage_cur++) {
    u32 use_stacking = 1 << (1 + UR(HAVOC_STACK_POW2));
    for (i = 0; i < use_stacking; i++) {
      switch (UR(15 + ((extras_cnt + a_extras_cnt) ? 2 : 0))) {
        case 0:
          /* Flip a single bit somewhere. Spooky! */
        case 1: 
          /* Set byte to interesting value. */
        case 2:
          /* Set word to interesting value, randomly choosing endian. */
        case 3:
          /* Set dword to interesting value, randomly choosing endian. */
        case 4:
          /* Randomly subtract from byte. */
        case 5:
          /* Randomly add to byte. */
        case 6:
          /* Randomly subtract from word, random endian. */
        case 7:
          /* Randomly add to word, random endian. */
        case 8:
          /* Randomly subtract from dword, random endian. */
        case 9:
          /* Randomly add to dword, random endian. */
        case 10:
          /* Just set a random byte to a random value. Because, why not. We use XOR with 1-255 to eliminate the possibility of a no-op. */
        case 11 ... 12:
          /* Delete bytes. We're making this a bit more likely than insertion (the next option) in hopes of keeping files reasonably small. */
        case 13:
          /* Clone bytes (75%) or insert a block of constant bytes (25%). */
        case 14:
          /* Overwrite bytes with a randomly selected chunk (75%) or fixed bytes (25%). */

          /* Values 15 and 16 can be selected only if there are any extras present in the dictionaries. */
        case 15:
          /* Overwrite bytes with an extra. */
        case 16:
          /* Insert an extra. Do the same dice-rolling stuff as for the previous case. */
      }
    }
    /* Write a modified test case, run program, process results. Handle error conditions, returning 1 if it's time to bail out. This is a helper function for fuzz_one(). */
  }