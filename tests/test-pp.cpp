#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/catch.hpp"
#include "lib/mstats-utils.h"

// Testers 1-5
TEST_CASE("tester1 against libc", "[weight=10][part=5][suite=week2][timeout=30]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester1 evaluate");
  system("./mstats tests/testers_exe/tester1 evaluate libc");

  mstats_result * student_result = read_mstats_result("mstats_result.txt");
  mstats_result * libc_result = read_mstats_result("mstats_result_libc.txt");

  system("rm mstats_result.txt");
  system("rm mstats_result_libc.txt");

  REQUIRE(student_result->status == 1);
  REQUIRE(student_result->max_heap_used <= libc_result->max_heap_used);
  REQUIRE(student_result->avg_heap_used <= libc_result->avg_heap_used);
  REQUIRE(student_result->time_taken < libc_result->time_taken);
}

TEST_CASE("tester2 against libc", "[weight=10][part=5][suite=week2][timeout=30]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester2 evaluate");
  system("./mstats-libc tests/testers_exe/tester2 evaluate libc");
  
  mstats_result * student_result = read_mstats_result("mstats_result.txt");
  mstats_result * libc_result =read_mstats_result("mstats_result_libc.txt");

  system("rm mstats_result.txt");
  system("rm mstats_result_libc.txt");

  REQUIRE(student_result->status == 1);
  REQUIRE(student_result->max_heap_used < libc_result->max_heap_used);
  REQUIRE(student_result->avg_heap_used < libc_result->avg_heap_used);
  REQUIRE(student_result->time_taken < libc_result->time_taken);
}

TEST_CASE("tester3 against libc", "[weight=10][part=5][suite=week2][timeout=30]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester3 evaluate");
  system("./mstats-libc tests/testers_exe/tester3 evaluate libc");
  
  mstats_result * student_result = read_mstats_result("mstats_result.txt");
  mstats_result * libc_result =read_mstats_result("mstats_result_libc.txt");

  system("rm mstats_result.txt");
  system("rm mstats_result_libc.txt");

  REQUIRE(student_result->status == 1);
  REQUIRE(student_result->max_heap_used < libc_result->max_heap_used);
  REQUIRE(student_result->avg_heap_used < libc_result->avg_heap_used);
  REQUIRE(student_result->time_taken < libc_result->time_taken);
}

TEST_CASE("tester4 against libc", "[weight=10][part=5][suite=week2][timeout=30]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester4 evaluate");
  system("./mstats-libc tests/testers_exe/tester4 evaluate libc");
  
  mstats_result * student_result = read_mstats_result("mstats_result.txt");
  mstats_result * libc_result =read_mstats_result("mstats_result_libc.txt");

  system("rm mstats_result.txt");
  system("rm mstats_result_libc.txt");

  REQUIRE(student_result->status == 1);
  REQUIRE(student_result->max_heap_used < libc_result->max_heap_used);
  REQUIRE(student_result->avg_heap_used < libc_result->avg_heap_used);
  REQUIRE(student_result->time_taken < libc_result->time_taken);
}

TEST_CASE("tester5 against libc", "[weight=10][part=5][suite=week2][timeout=30]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester5 evaluate");
  system("./mstats-libc tests/testers_exe/tester5 evaluate libc");
  
  mstats_result * student_result = read_mstats_result("mstats_result.txt");
  mstats_result * libc_result =read_mstats_result("mstats_result_libc.txt");

  system("rm mstats_result.txt");
  system("rm mstats_result_libc.txt");

  REQUIRE(student_result->status == 1);
  REQUIRE(student_result->max_heap_used < libc_result->max_heap_used);
  REQUIRE(student_result->avg_heap_used < libc_result->avg_heap_used);
  REQUIRE(student_result->time_taken < libc_result->time_taken);
}
