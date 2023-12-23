#define read_input(str) char* file_name = argc > 1 ? argv[1] : str;\
Lines* file = read_lines(file_name);\
if (!file) { \
    printf("Can't open file"); \
    return 1; \
}

