#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <math.h>

struct __test_framework_entry {
    const char* test_name;
    const char* file_name;
    int line_number;

    void (*test_function) ();
};

struct __test_framework_state {
    __test_framework_entry *tests;
    size_t used = 0;
    size_t size = 0;

    int status; //  < 0 --> last test failed
                //  > 0 --> last test passed
                // == 0 --> last test hasn't been marked yet

    __test_framework_entry *running_test;
};

static __test_framework_state __test_framework_current_state { NULL, 0, 0, 0, NULL };

static inline bool __test_framework_init_test_list() {
    __test_framework_state *state = &__test_framework_current_state;

    const size_t initial_size = 2;

    state->tests = (__test_framework_entry*)
        malloc(initial_size * sizeof(__test_framework_entry));

    state->used = 0;
    state->size = initial_size;

    return true;
}

static inline void __test_framework_free_test_list() {
    __test_framework_state *state = &__test_framework_current_state;

    free(state->tests);
    state->tests = NULL;

    state->used = 0;
    state->size = 0;
}

static inline bool __test_framework_add_test_entry(__test_framework_entry entry) {
    if (__test_framework_current_state.tests == NULL)
        __test_framework_init_test_list();

    __test_framework_state *state = &__test_framework_current_state;

    if (state->used == state->size) {
        state->size *= 2 /* Grow coefficient */;
        __test_framework_entry *new_space = (__test_framework_entry*)
            realloc(state->tests, state->size * sizeof(__test_framework_entry));

        if (new_space == NULL)
            return false; // Reallocation failed

        state->tests = new_space;
    }

    state->tests[state->used ++] = entry;
    return true; // Finished successfully
}

static inline bool __test_framework_assert_epsilon_equal(double x, double y) {
    return fabs(x - y) <= 1e-9 /* EPSILON */;
}

static inline void __test_framework_get_test_name_with_spaces(const char* const name,
                                                                 char* const new_name) {

    const size_t length = strlen(name); 
    for (int i = 0; i < length; ++ i) {
        char symbol = name[i];

        if (symbol == '_')
            new_name[i] = ' ';
        else
            new_name[i] = symbol;
    }

    new_name[length] = '\0';
}

#define TEXT_RED    "\033[31m"
#define TEXT_GREEN  "\033[32m"
#define TEXT_YELLOW "\033[33m"
#define TEXT_BLUE   "\033[34m"

#define TEXT_BOLD   "\033[1m"
#define TEXT_RESET  "\033[0m"

#define TEXT_INFO(string) TEXT_BOLD TEXT_BLUE string TEXT_RESET
#define TEXT_FAILED(string) TEXT_BOLD TEXT_RED string TEXT_RESET
#define TEXT_WARNING(string) TEXT_BOLD TEXT_YELLOW string TEXT_RESET
#define TEXT_PASSED(string) TEXT_BOLD TEXT_GREEN string TEXT_RESET

#define ASSERT_TRUE_WITH_EXPECTATION(cond, format, actual, expected)                         \
    do {                                                                                     \
        __test_framework_state *state = &__test_framework_current_state;                     \
        if (!(cond)) {                                                                       \
            const char* test_name = state->running_test->test_name;                          \
                                                                                             \
            char name_with_spaces[strlen(test_name) + 1];                                    \
            __test_framework_get_test_name_with_spaces(test_name, name_with_spaces);         \
                                                                                             \
            printf(TEXT_FAILED("[==> FAILED! <==] Test %s") "\n", name_with_spaces);         \
                                                                                             \
            printf("In " TEXT_WARNING("%s:%u\n"), __FILE__, __LINE__);                       \
            printf("In check %s:\n", TEXT_FAILED(#actual " == " #expected));                 \
            printf("Expected: " TEXT_INFO(format) "\n", expected);                           \
            printf("  Actual: " TEXT_FAILED(format) "\n", actual);                           \
                                                                                             \
            state->status = -1;                                                              \
            return;                                                                          \
        } else state->status = 1;                                                            \
    } while(false)

#define ASSERT_EPSILON_EQUAL(actual, expected)                                               \
    ASSERT_TRUE_WITH_EXPECTATION(                                                            \
        __test_framework_assert_epsilon_equal(actual, expected), "%lf", actual, expected)

#define ASSERT_EQUAL(actual, expected)                                                       \
    ASSERT_TRUE_WITH_EXPECTATION(actual == expected, "%d", actual, expected)

#define TEST_FRAMEWORK_INITIALIZER(name)                                                     \
    static void __test_framework_initialize_##name() __attribute__((constructor));           \
    static void __test_framework_initialize_##name()

#define TEST(name)                                                                           \
    void __test_framework_test_##name(void);                                                 \
    TEST_FRAMEWORK_INITIALIZER(name) {                                                       \
        __test_framework_entry entry {                                                       \
            #name, __FILE__, __LINE__,                                                       \
            &__test_framework_test_##name                                                    \
        };                                                                                   \
        __test_framework_add_test_entry(entry);                                              \
    }                                                                                        \
    void __test_framework_test_##name(void)                                                  \

#define TEST_MAIN()                                                                          \
    int main(void) {                                                                         \
        __test_framework_state *state = &__test_framework_current_state;                     \
        printf(TEXT_INFO("[==> MESSAGE <==] Running %zu tests") "\n", state->used);          \
                                                                                             \
        for (int i = 0; i < state->used; ++ i) {                                             \
            __test_framework_entry* entry = state->tests + i;                                \
                                                                                             \
            state->running_test = entry; /* Mark test running */                             \
            entry->test_function();      /* Run test */                                      \
                                                                                             \
            const char* test_name = entry->test_name;                                        \
                                                                                             \
            char name_with_spaces[strlen(test_name) + 1];                                    \
            __test_framework_get_test_name_with_spaces(test_name, name_with_spaces);         \
                                                                                             \
            if (state->status > 0)                                                           \
                printf(TEXT_PASSED("[==> PASSED! <==] Test \"%s\"") "\n", name_with_spaces); \
                                                                                             \
            if (state->status == 0)                                                          \
                printf(TEXT_WARNING("[==> WARNING <==] Test \"%s\" asserts nothing") "\n",   \
                       name_with_spaces);                                                    \
                                                                                             \
            state->status = 0;                                                               \
        }                                                                                    \
        __test_framework_free_test_list();                                                   \
    }
