/* Check if the result of an execve() during routine fuzzing is interesting,
   save or queue the input test case for further analysis if so. Returns 1 if
   entry is saved, 0 otherwise. */

static u8 save_if_interesting(char** argv, void* mem, u32 len, u8 fault) {

  u8  *fn = "";
  u8  hnb = 0;
  u8  hni_icnt = 0;
  s32 fd;
  u8  keeping = 0, res;
  u32 cksum = hash32(trace_bits, MAP_SIZE, HASH_CONST);
  // u32 cksum = hash32(icnt_bits, ICNT_SIZE*sizeof(u32), HASH_CONST); 

  if (fault == crash_mode) {

    /* Keep only if there are new bits in the map, add to queue for
       future fuzzing, etc. */

    hnb = has_new_bits(virgin_bits);
    hni_icnt = has_new_icnt();

    if (!hnb) {
      if (crash_mode) total_crashes++;
      return 0;
    }    
   

#ifndef SIMPLE_FILES

    fn = alloc_printf("%s/queue/id:%06u,%s%s", out_dir, queued_paths,
                      describe_op(hnb), hni_icnt ? ",+icnt": "");

#else

    fn = alloc_printf("%s/queue/id_%06u", out_dir, queued_paths);

#endif /* ^!SIMPLE_FILES */
  
    // DEBUG("Q.add: %02u hni_icnt: %d\n", queued_paths, hni_icnt);

	if (queue_cur->exec_cksum == cksum){ // (wcventure)
		add_to_queue(fn, len, 0);
	} else {
		delete_from_queue(cksum);
		add_to_queue(fn, len, 0);
	}

  if (hnb & 2) {
    queue_top->has_new_cov = 1;
    queued_with_cov++;
  }

  if (hni_icnt & 2) {
    queue_top->has_new_icnt = 1;
    queued_with_icnt++;
  }

  queue_top->exec_cksum = hash32(trace_bits, MAP_SIZE, HASH_CONST);    
  queue_top->icnt_cksum = hash32(icnt_bits, ICNT_SIZE*sizeof(icnt_bits[0]), HASH_CONST); 

  /* Try to calibrate inline; this also calls update_bitmap_score() when
      successful. */

  res = calibrate_case(argv, queue_top, mem, queue_cycle - 1, 0);

  if (res == FAULT_ERROR)
    FATAL("Unable to execute target application");

  fd = open(fn, O_WRONLY | O_CREAT | O_EXCL, 0600);
  if (fd < 0) PFATAL("Unable to create '%s'", fn);
  ck_write(fd, mem, len, fn);
  close(fd);

  keeping = 1;

  }

  switch (fault) {

    case FAULT_TMOUT:

      /* Timeouts are not very interesting, but we're still obliged to keep
         a handful of samples. We use the presence of new bits in the
         hang-specific bitmap as a signal of uniqueness. In "dumb" mode, we
         just keep everything. */

      total_tmouts++;

      if (unique_hangs >= KEEP_UNIQUE_HANG) return keeping;

      if (!dumb_mode) {

#ifdef __x86_64__
        simplify_trace((u64*)trace_bits);
#else
        simplify_trace((u32*)trace_bits);
#endif /* ^__x86_64__ */

        if (!has_new_bits(virgin_tmout)) return keeping;

      }

      unique_tmouts++;

      /* Before saving, we make sure that it's a genuine hang by re-running
         the target with a more generous timeout (unless the default timeout
         is already generous). */

      if (exec_tmout < hang_tmout) {

        u8 new_fault;
        write_to_testcase(mem, len);
        new_fault = run_target(argv, hang_tmout);
        ReadMemStatus();

        /* A corner case that one user reported bumping into: increasing the
           timeout actually uncovers a crash. Make sure we don't discard it if
           so. */

        if (!stop_soon && new_fault == FAULT_CRASH) goto keep_as_crash;

        if (stop_soon || new_fault != FAULT_TMOUT) return keeping;

      }

#ifndef SIMPLE_FILES

      fn = alloc_printf("%s/hangs/id:%06llu,%s", out_dir,
                        unique_hangs, describe_op(0));

#else

      fn = alloc_printf("%s/hangs/id_%06llu", out_dir,
                        unique_hangs);

#endif /* ^!SIMPLE_FILES */

      unique_hangs++;

      last_hang_time = get_cur_time();

      break;

    case FAULT_CRASH:

keep_as_crash:

      /* This is handled in a manner roughly similar to timeouts,
         except for slightly different limits and no need to re-run test
         cases. */

      total_crashes++;

      if (unique_crashes >= KEEP_UNIQUE_CRASH) return keeping;

      if (!dumb_mode) {

#ifdef __x86_64__
        simplify_trace((u64*)trace_bits);
#else
        simplify_trace((u32*)trace_bits);
#endif /* ^__x86_64__ */

        if (!has_new_bits(virgin_crash)) return keeping;

      }

      if (!unique_crashes) write_crash_readme();

#ifndef SIMPLE_FILES

      fn = alloc_printf("%s/crashes/id:%06llu,sig:%02u,%s", out_dir,
                        unique_crashes, kill_signal, describe_op(0));

#else

      fn = alloc_printf("%s/crashes/id_%06llu_%02u", out_dir, unique_crashes,
                        kill_signal);

#endif /* ^!SIMPLE_FILES */

      unique_crashes++;

      last_crash_time = get_cur_time();
      last_crash_execs = total_execs;

      break;

    case FAULT_ERROR: FATAL("Unable to execute target application");

    default: return keeping;

  }

  /* If we're here, we apparently want to save the crash or hang
     test case, too. */

  fd = open(fn, O_WRONLY | O_CREAT | O_EXCL, 0600);
  if (fd < 0) PFATAL("Unable to create '%s'", fn);
  ck_write(fd, mem, len, fn);
  close(fd);

  ck_free(fn);

  return keeping;

}
