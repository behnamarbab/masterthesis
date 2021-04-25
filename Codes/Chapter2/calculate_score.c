/* Calculate case desirability score to adjust the length of havoc fuzzing. A helper function for fuzz_one(). Maybe some of these constants should go into config.h. */

static u32 calculate_score(struct queue_entry* q) {
  u32 perf_score = 100;

  /* Adjust score based on execution speed of this path, compared to the global average. Multiplier ranges from 0.1x to 3x. Fast inputs are less expensive to fuzz, so we're giving them more air time. */

  if (q->exec_us * 0.1 > avg_exec_us) perf_score = 10;
  else if (q->exec_us * 4 < avg_exec_us) perf_score = 300;
  // Check other conditions in between   

  /* Adjust score based on bitmap size. The working theory is that better coverage translates to better targets. Multiplier from 0.25x to 3x. */
  if (q->bitmap_size * 0.3 > avg_bitmap_size) perf_score *= 3;
  else if (q->bitmap_size * 3 < avg_bitmap_size) perf_score *= 0.25;
  // Check other bitmap_sizes in between

  /* Adjust score based on handicap. Handicap is proportional to how late in the game we learned about this path. Latecomers are allowed to run for a bit longer until they catch up with the rest. */

  /* Final adjustment based on input depth, under the assumption that fuzzing deeper test cases is more likely to reveal stuff that can't be discovered with traditional fuzzers. */

  switch (q->depth) {
    case 0 ... 3:   break;
    case 14 ... 25: perf_score *= 4; break;
    // Check other cases in between
    default:        perf_score *= 5;
  }

  /* Make sure that we don't go over limit. */
  if (perf_score > HAVOC_MAX_MULT * 100) perf_score = HAVOC_MAX_MULT * 100;

  return perf_score;
}