#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    int current_uid = getuid();

    printf("UID: %d\n", current_uid);

    if (setuid(0))
    {
        perror("setuid");
        return 1;
    }

    printf("UID: %d\n", getuid());

    setuid(current_uid);

    printf("UID: %d\n", getuid());
    return 0;
}

//skompiluj jako user
//przejdz na roota
//zmien wlasciciela pliku (chown root:root nazwa)
//nadaj uprawnienia (chmod +s nazwa)
//wyjdz z roota
//odpal jako user
