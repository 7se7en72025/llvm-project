// RUN: export LSAN_OPTIONS=detect_leaks=0
// RUN: rm -rf %t && mkdir %t

// RUN: echo "%s -fcrash-diagnostics-dir=%t -fsyntax-only" | sed -e 's/\\/\\\\/g' > %t.rsp
// DEFINE: %{check_repro} = FileCheck %s --check-prefixes=REPRO,CHECK --implicit-check-not='BUG REPORT' --implicit-check-not='Crash backtrace'

// RUN: not %crash_opt %clang -DFATAL @%t.rsp -gen-reproducer=off    2>&1 | FileCheck %s --check-prefix=NOT
// RUN: not %crash_opt %clang -DFATAL @%t.rsp -fno-crash-diagnostics 2>&1 | FileCheck %s --check-prefix=NOT
// RUN: not %crash_opt %clang -DFATAL @%t.rsp                        2>&1 | FileCheck %s
// RUN: not %crash_opt %clang -DFATAL @%t.rsp -gen-reproducer=crash  2>&1 | FileCheck %s
// RUN: not %crash_opt %clang -DFATAL @%t.rsp -gen-reproducer=error  2>&1 | FileCheck %s
// RUN: not %crash_opt %clang -DFATAL @%t.rsp -gen-reproducer=always 2>&1 | FileCheck %s
// RUN: not %crash_opt %clang -DFATAL @%t.rsp -gen-reproducer        2>&1 | FileCheck %s

// RUN: not %clang -DERROR @%t.rsp -gen-reproducer=off    2>&1 | FileCheck %s --check-prefix=NOT
// RUN: not %clang -DERROR @%t.rsp -fno-crash-diagnostics 2>&1 | FileCheck %s --check-prefix=NOT
// RUN: not %clang -DERROR @%t.rsp                        2>&1 | FileCheck %s --check-prefix=NOT
// RUN: not %clang -DERROR @%t.rsp -gen-reproducer=crash  2>&1 | FileCheck %s --check-prefix=NOT
// RUN: not %clang -DERROR @%t.rsp -gen-reproducer=error  2>&1 | %{check_repro}
// RUN: not %clang -DERROR @%t.rsp -gen-reproducer=always 2>&1 | %{check_repro}
// RUN: not %clang -DERROR @%t.rsp -gen-reproducer        2>&1 | %{check_repro}

// RUN:     %clang         @%t.rsp -gen-reproducer=off    2>&1 | FileCheck %s --check-prefix=NOT --allow-empty
// RUN:     %clang         @%t.rsp -fno-crash-diagnostics 2>&1 | FileCheck %s --check-prefix=NOT --allow-empty
// RUN:     %clang         @%t.rsp                        2>&1 | FileCheck %s --check-prefix=NOT --allow-empty
// RUN:     %clang         @%t.rsp -gen-reproducer=crash  2>&1 | FileCheck %s --check-prefix=NOT --allow-empty
// RUN:     %clang         @%t.rsp -gen-reproducer=error  2>&1 | FileCheck %s --check-prefix=NOT --allow-empty
// RUN: not %clang         @%t.rsp -gen-reproducer=always 2>&1 | %{check_repro}
// RUN: not %clang         @%t.rsp -gen-reproducer        2>&1 | %{check_repro}

// RUN: not %clang -gen-reproducer=badvalue 2>&1 | FileCheck %s --check-prefix=BAD-VALUE
// BAD-VALUE: Unknown value for -gen-reproducer=: 'badvalue'

// REPRO:   Reproducer files:
// CHECK:   note: diagnostic msg: {{.*}}emit-reproducer-{{.*}}.c
// CHECK:   note: diagnostic msg: {{.*}}emit-reproducer-{{.*}}.sh
// NOT-NOT: note: diagnostic msg: {{.*}}emit-reproducer-{{.*}}.c

#ifdef FATAL
#pragma clang __debug crash
#elif ERROR
int main
#else
int main() {}
#endif
