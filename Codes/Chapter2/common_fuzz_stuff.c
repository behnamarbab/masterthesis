/* Write a modified test case, run program, process results. Handle error conditions, returning 1 if it's time to bail out. This is a helper function for fuzz_one(). */

EXP_ST u8 common_fuzz_stuff(char** argv, u8* out_buf, u32 len) {
  // Validate the file 
  // ...
  // Validate the execution
  fault = run_target(argv, exec_tmout);
  // If the file is "interesting", add it into queue
  queued_discovered += save_if_interesting(argv, out_buf, len, fault);
  // ...
  return 0;
}

/* Check if the result of an execve() during routine fuzzing is interesting, save or queue the input test case for further analysis if so. Returns 1 if entry is saved, 0 otherwise. */

static u8 save_if_interesting(char** argv, void* mem, u32 len, u8 fault) {
  if (fault == crash_mode) {
      hnb = has_new_bits(virgin_bits);
  }
  if(!hnb) return 0;

  queue_top->exec_cksum = hash32(trace_bits, MAP_SIZE, HASH_CONST);

  /* Try to calibrate inline; this also calls update_bitmap_score() when successful. */
  res = calibrate_case(argv, queue_top, mem, queue_cycle - 1, 0);

  // Save the file and return
}