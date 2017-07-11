#include "turns.h"
#include "test.h"

#ifdef __unix__
#include <sys/time.h> 
#include <sys/resource.h>
#endif // __unix__

static void test_turns_allocation() {
  turns_t * turns = NULL;

  turns = turns_new();
  if (turns == NULL) {
    fprintf(stderr, "turns_new failed.\n");
    return;
  }

  turns_delete(turns);
}

static void test_turns_dup1() {
  turns_t *turns = NULL;
  et_t e;
  turns_t *dup = NULL;
  int count;

  turns = turns_new();
  e = turns_dup(turns, &dup);
  if (e) {
    fprintf(stderr, "tunrs_dup failed.\n");
    goto end;
  } else if (turns == dup) {
    fprintf(stderr, "Unexpected pointer: turns(%p) == dup(%p)\n", turns, dup);
    goto end;
  }
  count = turns_get_count(dup);
  if (count != 0) {
    fprintf(stderr, "Unexpected count: 0 != %d\n", count);
    goto end;
  }

  turns_delete(dup);
  dup = NULL;

  turns_record(turns, PHASE_BLACK, 1);
  e = turns_dup(turns, &dup);
  if (e) {
    fprintf(stderr, "2nd tunrs_dup failed.\n");
    goto end;
  }
  count = turns_get_count(dup);
  if (count != 1) {
    fprintf(stderr, "Unexpected count: 1 != %d\n", count);
    goto end;
  }

end:
  turns_delete(turns);
  if (dup) {
    turns_delete(dup);
  }
}

static void test_turns_dup2() {
  et_t e;
  turns_t *turns = NULL;
  turns_t *dup = NULL;

  e = turns_dup(NULL, &dup);
  if (e != ET_ILLARGS) {
    fprintf(stderr, "turns_dup must return %d but %d \n", ET_ILLARGS, e);
  }

  turns = turns_new();
  e = turns_dup(turns, NULL);
  if (e != ET_ILLARGS) {
    fprintf(stderr, "turns_dup must return %d but %d \n", ET_ILLARGS, e);
    goto end;
  }

  dup = (turns_t *) 0xBAD; /* dummy */
  e = turns_dup(turns, &dup);
  if (e != ET_ILLARGS) {
    fprintf(stderr, "turns_dup must return %d but %d \n", ET_ILLARGS, e);
    goto end;
  }

end:
  if (turns) {
    turns_delete(turns);
  }
}

static void test_turns_dup3() {
#ifdef __unix__
  struct rlimit lmt_mem;
  int syserr;
  et_t e;
  turns_t *turns = NULL;
  turns_t *dup = NULL;

  lmt_mem.rlim_cur = 10 * 1024;
  lmt_mem.rlim_max = 10 * 1024;
  syserr = setrlimit(RLIMIT_AS, &lmt_mem);
  if (syserr) {
    fprintf(stderr, "setrlimit(RLIMIT_AS) failed.\n");
    return;
  }

  turns = turns_new();
  while (turns) {
    e = turns_dup(turns, &dup);
    if (e == ET_OK) {
      dup = NULL;
      continue;
    }
    if (e == ET_NOMEM) {
      turns_delete(turns);
      break;
    }
  }
#endif // __unix__
}

static void test_turns_record1() {
  et_t e;
  turns_t *turns = turns_new();

  e = turns_record(NULL, PHASE_BLACK, 0);
  if (e != ET_ILLARGS) {
    fprintf(stderr, "turns_record(NULL) must return ET_ILLARGS but %d.\n", e);
    goto end;
  }

  e = turns_record(turns, PHASE_NONE, 0);
  if (e != ET_ILLARGS) {
    fprintf(stderr, "turns_record(phase PHASE_NONE) must return ET_ILLARGS but %d.\n", e);
    goto end;
  }

  e = turns_record(turns, PHASE_BLACK, 64);
  if (e != ET_ILLARGS) {
    fprintf(stderr, "turns_record(64) must return ET_ILLARGS but %d.\n", e);
    goto end;
  }

  e = turns_record(turns, PHASE_BLACK, 0);
  if (e != ET_OK) {
    fprintf(stderr, "turns_record() must return ET_OK but %d.\n", e);
    goto end;
  }

end:
  turns_delete(turns);
}

static void test_turns_record2() {
  et_t e;
  turns_t *turns = turns_new();
  int count = 0;

  e = turns_record(turns, PHASE_BLACK, 0);
  if (e != ET_OK) {
    fprintf(stderr, "turns_record() must return ET_OK but %d.\n", e);
    goto end;
  }

  count = turns_get_count(turns);
  if (count != 1) {
    fprintf(stderr, "turns_get_count must return 1 but %d.\n", count);
    goto end;
  }

  for ( ; count < 60; count++) {
    e = turns_record(turns, PHASE_BLACK, 0);
    if (e != ET_OK) {
      fprintf(stderr, "turns_record() must return ET_OK but %d on %d count.\n", e, count);
      goto end;
    }
  }

  /* It must be faield by illegal state */
  e = turns_record(turns, PHASE_BLACK, 0);
  if (e != ET_ILLSTATE) {
    fprintf(stderr, "turns_record() must return ET_ILLSTATE but %d.\n", e);
    goto end;
  }

end:
  turns_delete(turns);
}

void turns_test_main() {
  RUN(test_turns_allocation);
  RUN(test_turns_dup1);
  RUN(test_turns_dup2);
  RUN(test_turns_dup3);
  RUN(test_turns_record1);
  RUN(test_turns_record2);
}
