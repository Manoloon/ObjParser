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
dynarray_t* vertices = NULL;

void parse_vertex(const char *input)
{
    vec4_t vector;
    char* endptr = (char*)input;
    vector.x = strtof(input+2,&endptr);
    if(sscanf(input+2,"%f %f %f %f", &vector.x, &vector.y, &vector.z, &vector.w) == 4)
    {
        printf("x:%.2f, y=%.2f, z:%.2f, w:%.2f\n", vector.x, vector.y, vector.z,vector.w);
        dynArray_push_back(vertices,&vector);
        return;
    }
    if(sscanf(input + 2, "%f %f %f", &vector.x, &vector.y, &vector.z) == 3)
    {
        printf("x:%.2f, y=%.2f, z:%.2f\n", vector.x, vector.y, vector.z);
        vector.w = 1.0f;
        dynArray_push_back(vertices,&vector);
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
    FILE* file = NULL;
    const char* filename = "Assets/cube.obj";
    file = fopen(filename,"r");
    if(file == NULL)
    {
        fprintf(stderr,"failed to open file\n");
        return EXIT_FAILURE;
    }
    vertices = dynArray_create(sizeof(vec4_t));

    int ch = 0;
    int parsing = 1;
    dynarray_t* line_buffer = dynArray_create(sizeof(char));
    if(line_buffer == NULL)
    {
        fprintf(stderr,"line_buffer push_back failed\n");
        fclose(file);
        return EXIT_FAILURE;
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

    size_t size = dynArray_get_length(vertices);
    const vec4_t* items = dynArray_get_items(vertices);
    if(items == NULL)
    {
        fprintf(stderr,"Items from vertices is NULL\n");
        fclose(file);
        dynArray_free(&line_buffer);
        line_buffer = NULL;
        return EXIT_FAILURE;
    }
    printf("Vertices\n");

    for(size_t i = 0; i < size; i++)
    {
        vec4_t item = items[i];
        printf("x:%.2f, y=%.2f, z:%.2f, w:%.2f\n", item.x, item.y, item.z,item.w);
    }
    fclose(file);
    dynArray_free(&line_buffer);
    line_buffer = NULL;
    return 0;
}