#include <stdio.h>
#include <stdlib.h>

int main()
{
    freopen("logo.jpg", "rb", stdin);
    freopen("logo.cpp", "w", stdout);
    printf("#ifdef __cplusplus\nextern \"C\"\n{\n#endif\nstatic unsigned char logo_res[] = {\n    ");
    for (int i=0; ; ++i)
    {
        int c = getchar();
        if (c == EOF)
        {
            break;
        }
        printf("%d,", c);
        if (i==16)
        {
            printf("\n    ");
            i = 0;
        }
    }

    puts("\n};\nunsigned long getlogodatasize() {\n    return sizeof(logo_res);\n}\n\nchar* getlogodata() {\n    return (char*)logo_res;\n}\n#ifdef __cplusplus\n}\n#endif\n");
    return 0;
}
