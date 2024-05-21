#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/////////////////////////////////////////
/////////////// VARIABLES ///////////////
/////////////////////////////////////////

typedef struct vars {
// %s
    // generated
    bool h;
    bool prev_h;

    bool d;
    bool prev_d;
} Vars;

bool rand_bool() {
    return rand() % 2;
}

void init_vars(Vars* vars) {
// %s
    // generated
    vars->h = vars->prev_h = rand_bool();
    vars->d = vars->prev_d = rand_bool();
    printf("h: %d, d: %d\n", vars->h, vars->d);
}

void update_vars(Vars* vars) {
// %s
    // generated
    vars->prev_h = vars->h;
    vars->h = rand_bool();

    vars->prev_d = vars->d;
    vars->d = rand_bool();
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

typedef struct requirement {
    bool trigger;
    bool invariant;
    bool final;
    bool delay;
    bool reaction;
    bool release;
} Requirement;

// %s
// generated
void calc_attrs_for_first_req(Requirement* req, Vars* vars) {
    req->trigger = re(vars->prev_h, vars->h) && !vars->d;
    req->invariant = true;
    req->final = true;
    req->delay = true;
    req->reaction = vars->d;
    req->release = false;
}

// generated
void calc_attrs_for_second_req(Requirement* req, Vars* vars) {
    req->trigger = re(vars->prev_h, vars->h) && !vars->d;
    req->invariant = true;
    req->final = true;
    req->delay = true;
    req->reaction = vars->d;
    req->release = false;
}

//////////////////////////////////////////////////////
///////////// REQUIREMENTS VERIFICATION //////////////
//////////////////////////////////////////////////////

bool check_requirement(void (*calc_attrs)(Requirement*, Vars*));

// generated
bool check_requirement_first() {
    return check_requirement(calc_attrs_for_first_req);
}

// generated
bool check_requirement_second() {
    return check_requirement(calc_attrs_for_second_req);
}

bool a(Requirement* req, Vars* vars, void (*calc_attrs)(Requirement*, Vars*));
bool b(Requirement* req, Vars* vars, void (*calc_attrs)(Requirement*, Vars*));

bool check_requirement(void (*calc_attrs)(Requirement*, Vars*)) {
    Vars vars;
    Requirement req;
    init_vars(&vars);
    while(true) {
        calc_attrs(&req, &vars);
        if (!req.trigger) {
            update_vars(&vars);
            continue;
        }

        return a(&req, &vars, calc_attrs);
    }
}

bool a(Requirement* req, Vars* vars, void (*calc_attrs)(Requirement*, Vars*)) {
    while (true) {
        if (req->release) {
            return true;
        }

        if (req->final) {
            return b(req, vars, calc_attrs);
        }

        if (!req->invariant) {
            printf("a(): not final and not invariant\n");
            return false;
        }

        update_vars(vars);
        calc_attrs(req, vars);
    }
}

bool b(Requirement* req, Vars* vars, void (*calc_attrs)(Requirement*, Vars*)) {
    while (true) {
        if (req->delay) {
            if (!req->invariant) {
                printf("b(): delay, but not invariant\n");
                return false;
            }

            if (!req->reaction) {
                printf("b(): delay, but not reaction\n");
                return false;
            }

            return true;
        }

        if (!req->invariant) {
            printf("b(): not delay and not invariant\n");
            return false;
        }

        if (req->reaction) {
            return true;
        }

        update_vars(vars);
        calc_attrs(req, vars);

        if (req->release) {
            return true;
        }
    }
}

void verify_requirement(char* req_name, bool (*check_req)(void)) {
    printf("Verifying requirement \'%s\'\n", req_name);
    bool success = check_req();
    if (success) {
        printf("Verification for requirement \'%s\' has succeeded", req_name);
    } else {
        printf("Verification for requirement \'%s\' has failed", req_name);
    }

    printf("\n\n");
}

int main(int argc, char *argv[]) {
    srand(time(NULL)); 
    // generated
    verify_requirement("first", check_requirement_first);
    // generated
    verify_requirement("second", check_requirement_second);
}