
#include "nost/vm.h"
#include "nost/val.h"
#include "nost/gc.h"
#include "nost/bytecode.h"
#include "nost/fiber.h"
#include "nost/src.h"
#include "nost/reader.h"
#include "nost/str.h"
#include "nost/ast.h"
#include "nost/compiler.h"
#include "nost/debug.h"
#include "nost/analysis.h"
#include "nost/sym.h"
#include "nost/fn.h"
#include "nost/list.h"
#include "nost/pkg.h"

int main() {

    nost_vm vm;
    nost_initVM(&vm);

    nost_ref pkg = NOST_PUSH_BLESSING(&vm, nost_makePkg(&vm));

    printf("Welcome to Nost!\n\n");
    while(true) {

        char line[1024];

        printf("> ");
        if(!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            continue;
        }

        nost_ref src = NOST_PUSH_BLESSING(&vm, nost_makeSrc(&vm));
        nost_initSrc(&vm, nost_getRef(&vm, src), line); 

        nost_reader reader;
        nost_initReader(&reader, src);

        while(!reader.eof) {

            nost_errors errors;
            nost_initDynarr(&errors);

            nost_ref fiber = NOST_PUSH_BLESSING(&vm, nost_makeFiber(&vm));
            nost_val res = nost_readAndEval(&vm, pkg, fiber, &reader, &errors);

            if(nost_isNone(res)) {
                if(errors.cnt > 0) {
                    for(int i = 0; i < errors.cnt; i++) {
                        nost_str errStr;
                        nost_initStr(&errStr);
                        nost_writeError(&vm, &errStr, &errors.vals[i]);
                        fprintf(stderr, "%s\n", errStr.str);
                        nost_freeStr(&vm, &errStr);
                        nost_freeError(&vm, &errors.vals[i]);
                    } 
                }
            } else {
                nost_str valStr;
                nost_initStr(&valStr);
                nost_writeVal(&vm, &valStr, res);
                printf("%s\n", valStr.str);
                nost_freeStr(&vm, &valStr);
            }

            NOST_POP_BLESSING(&vm, fiber);
            nost_freeDynarr(&vm, &errors);

        }

        nost_freeReader(&vm, &reader);

        NOST_POP_BLESSING(&vm, src);

    }

    NOST_POP_BLESSING(&vm, pkg);

    printf("BLESSINGS: %d\n", vm.blessed.cnt);
#ifdef NOST_BLESS_TRACK
    for(int i = 0; i < vm.blessed.cnt; i++) {
        printf("%s\n", vm.blessedRefs.vals[i].loc);
    }
#endif

    nost_freeVM(&vm);

#ifdef NOST_MEM_TRACK
    printf("ALLOCATED MEMORY: %zu\n", vm.allocatedMem);
    for(nost_memTracker* track = vm.trackers; track != NULL; track = track->next)
        if(track->size != 0)
            printf("%s => %zu\n", track->loc, track->size);    
#endif

}
