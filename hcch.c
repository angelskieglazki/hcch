#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <time.h>

#define SOURCE "/**\n* \\file %s\n* \\author %s\n*\n*\n* Created on %s*/\n\n#ifndef %s_H\n#define %s_H\n\n#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n\n\n\n#ifdef __cplusplus\n}\n#endif\n\n#endif"

const char *get_user_name()
{
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if (pw) {
        return pw->pw_name;
    }
    return "";
}

char *get_time()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    return asctime(tm);
}

int main(int argc, char* argv[]) 
{
    char *name;
    if (argc == 2) {
        name = malloc(strlen(argv[1]));
        strncpy(name, argv[1], strlen(argv[1]));
        char name_h[50];
        char name_c[50];
        snprintf(name_h, 100, "%s.h", name);
        snprintf(name_c, 100, "%s.c", name);

        int i;
        for (i =0; i<strlen(name); i++)
            name[i] = toupper(name[i]);
        char source[200];
        int cx;
        cx = snprintf(source, 200, SOURCE, name_h, get_user_name(), get_time(), name, name);
        
        int fd;
        
        if ((fd = open(name_h, O_RDWR|O_CREAT, 0644))<0) {
            perror("Error to open create file");
            exit(1);
        }
        write(fd, (const void*)source, strlen(source));
        close(fd);
        if ((fd = open(name_c, O_RDWR|O_CREAT, 0644))<0) {
            perror("Error to open create file");
            exit(1);
        }
        close(fd);


//        printf("%s", source);
    } else {
        perror("CMD LINE ARGS ERROR!!!\n");
        exit(1);
    }
    
    return 0;   
}
