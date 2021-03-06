// RUN: %clang_cc1 -triple i686-pc-win32 -std=c++11 -fms-extensions -verify %s

__thread __declspec(thread) int a; // expected-error {{already has a thread-local storage specifier}}
__declspec(thread) __thread int b; // expected-error {{already has a thread-local storage specifier}}
__declspec(thread) int c(); // expected-warning {{only applies to variables}}
__declspec(thread) int d;
int foo();
__declspec(thread) int e = foo(); // expected-error {{must be a constant expression}} expected-note {{thread_local}}

struct HasCtor { HasCtor(); int x; };
__declspec(thread) HasCtor f; // expected-error {{must be a constant expression}} expected-note {{thread_local}}

struct HasDtor { ~HasDtor(); int x; };
__declspec(thread) HasDtor g; // expected-error {{non-trivial destruction}} expected-note {{thread_local}}

struct HasDefaultedDefaultCtor {
  HasDefaultedDefaultCtor() = default;
  int x;
};
__declspec(thread) HasDefaultedDefaultCtor h;

struct HasConstexprCtor {
  constexpr HasConstexprCtor(int x) : x(x) {}
  int x;
};
__declspec(thread) HasConstexprCtor i(42);

int foo() {
  __declspec(thread) int a; // expected-error {{must have global storage}}
  static __declspec(thread) int b;
}

extern __declspec(thread) int fwd_thread_var;
__declspec(thread) int fwd_thread_var = 5;

extern int fwd_thread_var_mismatch; // expected-note {{previous declaration}}
__declspec(thread) int fwd_thread_var_mismatch = 5; // expected-error-re {{thread-local {{.*}} follows non-thread-local}}

extern __declspec(thread) int thread_mismatch_2; // expected-note {{previous declaration}}
int thread_mismatch_2 = 5; // expected-error-re {{non-thread-local {{.*}} follows thread-local}}

typedef __declspec(thread) int tls_int_t; // expected-warning {{only applies to variables}}
