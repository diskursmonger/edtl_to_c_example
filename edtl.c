#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/////////////////////////////////////////
/////////////// VARIABLES ///////////////
/////////////////////////////////////////

bool H;
bool PREV_H;

bool D;
bool PREV_D;

bool rand_bool() {
    return rand() % 2;
}

void init_vars() {
    H = PREV_H = rand_bool();
    D = PREV_D = rand_bool();
}

void update_vars() {
    PREV_H = H;
    H = rand_bool();

    PREV_D = D;
    D = rand_bool();
}

/////////////////////////////////////////////////////
////////////////// EDTL OPERATORS ///////////////////
/////////////////////////////////////////////////////

bool re(bool prev_v, bool v) {
    return prev_v && !v;
}

bool fe(bool prev_v, bool v) {
    return !prev_v && v;
}

bool high(bool prev_v, bool v) {
    return prev_v && v;
}

bool low(bool prev_v, bool v) {
    return !prev_v && !v;
}

////////////////////////////////////////////////////////
///////////// REQUIREMENTS AND ATTRIBUTES //////////////
////////////////////////////////////////////////////////

bool TRIGGER;
bool INVARIANT;
bool FINAL;
bool DELAY;
bool REACTION;
bool RELEASE;

void calc_attrs_for_first_req() {
    TRIGGER = re(PREV_H, H) && !D;
    INVARIANT = true;
    FINAL = true;
    DELAY = true;
    REACTION = D;
    RELEASE = false;
}

// void calc_attrs_for_second_req() {
//     trigger = re(PREV_H && PREV_D, H && D) && !D;
//     invariant = true;
//     final = false;
//     delay = true;
//     reaction = false;
//     release = true;
// }

//////////////////////////////////////////////////////
///////////// REQUIREMENTS VERIFICATION //////////////
//////////////////////////////////////////////////////

bool a(void (*calc_attrs)(void));
bool b(void (*calc_attrs)(void));

bool check_requirement(void (*calc_attrs)(void)) {
    init_vars();
    while(true) {
        calc_attrs();
        if (!TRIGGER) {
            update_vars();
            continue;
        }

        return a(calc_attrs);
    }
}

bool a(void (*calc_attrs)(void)) {
    if (RELEASE) {
        return true;
    }

    if (FINAL) {
        return b(calc_attrs);
    }

    if (!INVARIANT) {
        printf("a(): not final and not invariant\n");
        return false;
    }

    update_vars();
    calc_attrs();

    return a(calc_attrs);
}

bool b(void (*calc_attrs)(void)) {
    if (DELAY) {
        if (!INVARIANT) {
            printf("b(): delay, but not invariant\n");
            return false;
        }

        if (!REACTION) {
            printf("b(): delay, but not reaction\n");
            return false;
        }

        return true;
    }

    if (!INVARIANT) {
        printf("b(): not delay and not invariant\n");
        return false;
    }

    if (REACTION) {
        return true;
    }

    update_vars();
    calc_attrs();

    if (RELEASE) {
        return true;
    }

    return b(calc_attrs);
}

void verify_requirement(char* req_name, void (*calc_attrs)(void)) {
    printf("Verifying requirement \'%s\'\n", req_name);
    bool success = check_requirement(calc_attrs);
    if (success) {
        printf("Verification for requirement \'%s\' has succeeded", req_name);
    } else {
        printf("Verification for requirement \'%s\' has failed", req_name);
    }

    printf("\n\n");
}

int main(int argc, char *argv[]) {
    srand(time(NULL)); 
    verify_requirement("first", calc_attrs_for_first_req);
    // verify_requirement("second", calc_attrs_for_second_req);
    // verify_requirement("third", calc_attrs_for_third_req);
}