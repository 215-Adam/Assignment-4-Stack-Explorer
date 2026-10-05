/*
 * Stack Explorer Assignment
 * Computer Science XII - Computer Systems
 * 
 * This program explores how function calls work at the stack level,
 * demonstrating recursion, stack growth, stack overflow, and callbacks.
 */

#include <stdio.h>
#include <stdlib.h>

// Global variable for tracking maximum recursion depth
int max_depth = 0;

// =============================================================================
// PART 1: BASIC RECURSION
// =============================================================================
// Implement a recursive factorial function that visualizes stack growth.

// TODO: Implement factorial function
// This function should:
// - Accept an integer n
// - Print "Entering factorial(n)" when called
// - If n <= 1, print "Base case reached" and return 1
// - Otherwise, recursively call factorial(n-1) and multiply by n
// - Print "Returning from factorial(n) = result" before returning
// - Return the factorial result
//
// Hint: int factorial(int n) { ... }
// Example output for factorial(3):
//   Entering factorial(3)
//   Entering factorial(2)
//   Entering factorial(1)
//   Base case reached
//   Returning from factorial(1) = 1
//   Returning from factorial(2) = 2
//   Returning from factorial(3) = 6

// Note: int overflows past factorial(12), so keep n <= 12.
int factorial(int n) {
    printf("Entering factorial(%d)\n", n);      // a new stack frame is pushed

    int result;
    if (n <= 1) {
        printf("Base case reached\n");
        result = 1;
    } else {
        result = n * factorial(n - 1);          // this frame waits for the one above it
    }

    printf("Returning from factorial(%d) = %d\n", n, result);  // frame is popped
    return result;
}

// =============================================================================
// PART 2: STACK DEPTH TRACKING
// =============================================================================
// Implement recursive Fibonacci that tracks how deep the recursion goes.

// TODO: Implement fibonacci function with depth tracking
// This function should:
// - Accept two parameters: int n (the Fibonacci number to calculate)
//                          int depth (current recursion depth)
// - Update the global max_depth if current depth is greater
// - Print the current depth and n value
// - If n <= 1, return n (base case)
// - Otherwise, return fibonacci(n-1, depth+1) + fibonacci(n-2, depth+1)
//
// Hint: int fibonacci(int n, int depth) { ... }
// The depth parameter tracks how many levels deep we are in recursion

// fib(30) makes over 2.6 million calls, so per-call printing can be switched
// off for large n. fib_calls counts every call so we can compare total work
// (which grows exponentially) against max_depth (which only grows linearly).
int fib_trace = 1;
long fib_calls = 0;

int fibonacci(int n, int depth) {
    fib_calls++;
    if (depth > max_depth) {
        max_depth = depth;
    }

    if (fib_trace) {
        // Indent by depth so the shape of the call tree is visible
        printf("%*sdepth %d: fibonacci(%d)\n", depth * 2, "", depth, n);
    }

    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1, depth + 1) + fibonacci(n - 2, depth + 1);
}

// Resets the trackers, runs fibonacci(n), and reports the stats
void run_fibonacci(int n, int trace) {
    max_depth = 0;
    fib_calls = 0;
    fib_trace = trace;

    int result = fibonacci(n, 1);
    printf("fibonacci(%d) = %d | max stack depth = %d | total calls = %ld\n",
           n, result, max_depth, fib_calls);
}

// =============================================================================
// PART 3: STACK OVERFLOW DEMONSTRATION
// =============================================================================
// Create a function that causes stack overflow, then fix it.

// TODO: Implement infinite_recursion (buggy version)
// This function should:
// - Accept an integer n
// - Print "Call n"
// - Recursively call itself with n+1 (NO BASE CASE - this will overflow!)
//
// This will crash! That's intentional - you'll observe what stack overflow looks like.
// Hint: void infinite_recursion(int n) { ... }

void infinite_recursion(int n) {
    printf("Call %d\n", n);
    infinite_recursion(n + 1);   // no base case: every call pushes another frame
}

// TODO: Implement safe_recursion (fixed version)
// This function should:
// - Accept an integer n and int max_depth
// - Print "Call n"
// - If n >= max_depth, print "Stopping at max depth" and return (BASE CASE)
// - Otherwise, recursively call itself with n+1
//
// This version has a proper base case and won't overflow.
// Hint: void safe_recursion(int n, int max_depth) { ... }

// (The max_depth parameter shadows the global of the same name inside this function.)
void safe_recursion(int n, int max_depth) {
    printf("Call %d\n", n);
    if (n >= max_depth) {
        printf("Stopping at max depth\n");
        return;                  // base case: the stack starts unwinding here
    }
    safe_recursion(n + 1, max_depth);
}

// =============================================================================
// PART 4: FUNCTION POINTERS AND CALLBACKS
// =============================================================================
// Use function pointers to create flexible array processing.

// TODO: Implement callback functions for array processing
// Create three functions that each accept an integer and return an integer:
// 1. double_value: returns n * 2
// 2. square_value: returns n * n
// 3. negate_value: returns -n
//
// Hint: int double_value(int n) { return n * 2; }

int double_value(int n) { return n * 2; }

int square_value(int n) { return n * n; }

int negate_value(int n) { return -n; }

// TODO: Implement process_array function
// This function should:
// - Accept: int* array, int size, int (*callback)(int)
// - The callback parameter is a function pointer to a function that takes int and returns int
// - Loop through the array and apply the callback to each element
// - Print each result
//
// Hint: void process_array(int* arr, int size, int (*callback)(int)) { ... }
// Example: process_array(arr, 5, double_value); applies double_value to each element

void process_array(int* arr, int size, int (*callback)(int)) {
    for (int i = 0; i < size; i++) {
        printf("%d ", callback(arr[i]));   // which function runs is decided by the caller
    }
    printf("\n");
}

// =============================================================================
// PART 5: EVENT CALLBACK SYSTEM
// =============================================================================
// Build a simple event system using function pointers.

#define MAX_CALLBACKS 10

// Event system structure
typedef struct EventSystem {
    void (*callbacks[MAX_CALLBACKS])(int);  // Array of function pointers
    int callback_count;
} EventSystem;

// TODO: Implement event_system_init
// This function should:
// - Accept an EventSystem* pointer
// - Set callback_count to 0
// - Initialize all callback pointers to NULL
//
// Hint: void event_system_init(EventSystem* es) { ... }

void event_system_init(EventSystem* es) {
    es->callback_count = 0;
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        es->callbacks[i] = NULL;
    }
}

// TODO: Implement event_system_register
// This function should:
// - Accept an EventSystem* pointer and a function pointer void (*callback)(int)
// - If callback_count < MAX_CALLBACKS, add the callback to the array
// - Increment callback_count
// - Print "Callback registered" or "Max callbacks reached"
//
// Hint: void event_system_register(EventSystem* es, void (*callback)(int)) { ... }

void event_system_register(EventSystem* es, void (*callback)(int)) {
    if (es->callback_count < MAX_CALLBACKS) {
        es->callbacks[es->callback_count] = callback;
        es->callback_count++;
        printf("Callback registered\n");
    } else {
        printf("Max callbacks reached\n");
    }
}

// TODO: Implement event_system_trigger
// This function should:
// - Accept an EventSystem* pointer and an int event_value
// - Loop through all registered callbacks and call each one with event_value
// - Print "Triggering X callbacks with value Y"
//
// Hint: void event_system_trigger(EventSystem* es, int value) { ... }

void event_system_trigger(EventSystem* es, int value) {
    printf("Triggering %d callbacks with value %d\n", es->callback_count, value);
    for (int i = 0; i < es->callback_count; i++) {
        es->callbacks[i](value);
    }
}

// Example callback functions for the event system
void on_score_update(int score) {
    printf("  Score callback: New score is %d\n", score);
}

void on_level_change(int level) {
    printf("  Level callback: Now entering level %d\n", level);
}

void on_health_change(int health) {
    printf("  Health callback: Health is now %d\n", health);
}

// =============================================================================
// MAIN FUNCTION
// =============================================================================

int main() {
    printf("=============================================================\n");
    printf("            STACK EXPLORER: Function Call Mechanics\n");
    printf("=============================================================\n");
    
    // TODO: Test your implementations here
    // 1. Test factorial with a small value (like 5) to see stack visualization
    // 2. Test fibonacci with increasing values (5, 10, 15) and print max_depth
    // 3. Test infinite_recursion briefly (comment out after observing crash!)
    // 4. Test safe_recursion to show the fixed version works
    // 5. Create an array and test process_array with different callbacks
    // 6. Create an EventSystem, register callbacks, and trigger events
    
    printf("\n--- Part 1: Factorial with Stack Visualization ---\n");
    printf("factorial(5) = %d\n\n", factorial(5));
    printf("factorial(10) = %d\n", factorial(10));

    printf("\n--- Part 2: Fibonacci with Depth Tracking ---\n");
    // Full trace for a small n so the call tree is readable
    run_fibonacci(5, 1);
    // Larger values: stats only (fibonacci(30) would print ~2.7 million lines)
    printf("\n");
    run_fibonacci(10, 0);
    run_fibonacci(20, 0);
    run_fibonacci(30, 0);

    printf("\n--- Part 3: Stack Overflow Demo (comment out after testing!) ---\n");
    // Observed: prints "Call 0", "Call 1", ... then the program dies with no
    // error message. On Windows (gcc 10.3, default 1 MB stack) it got to roughly
    // Call 43,000 before crashing with exit code 0xC00000FD (STATUS_STACK_OVERFLOW);
    // on Linux/macOS the same bug shows up as "Segmentation fault".
    // printf("Attempting infinite recursion...\n");
    // infinite_recursion(0);  // WILL CRASH - comment out after observing!
    printf("infinite_recursion(0) is commented out (it crashes the program).\n");

    printf("\n--- Part 3: Safe Recursion (Fixed Version) ---\n");
    safe_recursion(0, 10);

    printf("\n--- Part 4: Function Pointers and Callbacks ---\n");
    int numbers[] = {1, 2, 3, 4, 5};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    printf("Original: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    printf("Doubled:  ");
    process_array(numbers, size, double_value);
    printf("Squared:  ");
    process_array(numbers, size, square_value);
    printf("Negated:  ");
    process_array(numbers, size, negate_value);

    printf("\n--- Part 5: Event System ---\n");
    EventSystem events;
    event_system_init(&events);
    event_system_register(&events, on_score_update);
    event_system_register(&events, on_level_change);
    event_system_register(&events, on_health_change);

    event_system_trigger(&events, 100);
    event_system_trigger(&events, 3);

    printf("\n=============================================================\n");
    printf("Stack exploration complete!\n");
    printf("=============================================================\n");
    
    return 0;
}