#include <stdio.h>
#include "types.h"
#include "database.h"

db_connected connected;

int main() {
    createDB(NULL); // Cria banco padrão
    printf("uffsdb 16.1\nType \"help\" for help.\n\n");
    interface();
    return 0;
}
