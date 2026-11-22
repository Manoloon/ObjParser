#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "dynamicArray.h"

typedef struct
{
    float x;
    float y;
    float z;
    float w;
}vec4_t;

void parse_vertex(const char *input)
{
    vec4_t vector;
    if (sscanf(input + 2, "%f %f %f", &vector.x, &vector.y, &vector.z) == 3)
    {
        printf("x:%.2f, y=%.2f, z:%.2f\n", vector.x, vector.y, vector.z);
    }
}

void parse_line(dynarray_t* input)
{
    if(input == NULL)
    {
        fprintf(stderr,"input is NULL\n");
        return;
    }
    size_t size = dynArray_get_length(input);
    const char *items = dynArray_get_items(input);
    char space = items[1];
    if (size > 2) 
    {
      // vertex
      if (items[0] == 'v' && items[1] == ' ')
      {
        parse_vertex(items);
      }
    }
    dynArray_clear(input);
}

int main(int argc,char* argv[]) {
    if(argc > 2)
    {
        char* input = argv[1];
    }
    FILE* file;
    const char* filename = "Assets/cube.obj";
    file = fopen(filename,"r");
    if(file == NULL)
    {
        fprintf(stderr,"failed to open file\n");
        return -1;
    }
    int ch = 0;
    int parsing = 1;
    dynarray_t* line_buffer = dynArray_create(sizeof(char));
    if(line_buffer == NULL)
    {
        fprintf(stderr,"line_buffer push_back failed\n");
        return 1;
    }
    // lectura char por char
    while(parsing == 1)
    {
        // avanza automaticamente character por character
        ch = fgetc(file);
        if(ch == EOF)
        {
            parsing = 0;
            continue;
        }
        if(ch != '\n')
        {
            if(dynArray_push_back(line_buffer,&ch) != SUCCESS)
            {
                fprintf(stderr,"line_buffer push_back failed\n");
            }
        }
        else
        {
            char end = '\0';
            dynArray_push_back(line_buffer,&end);
            parse_line(line_buffer);
        }
    }
    fclose(file);
    dynarray_t* vertex = NULL;
    dynArray_free(&line_buffer);
    line_buffer = NULL;
    return 0;
}