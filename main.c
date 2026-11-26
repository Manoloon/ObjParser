#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dynamicArray.h"
/*
face_t cube_faces[N_CUBE_FACES]=
{
    // front
    {.a = 1, .b = 2, .c = 3},
    {.a = 1, .b = 3, .c = 4},
    // right
    {.a = 4, .b = 3, .c = 5},
    {.a = 4, .b = 5, .c = 6},
    // back
    {.a = 6, .b = 5, .c = 7},
    {.a = 6, .b = 7, .c = 8},
    // left
    {.a = 8, .b = 7, .c = 2},
    {.a = 8, .b = 2, .c = 1},
    // top
    {.a = 2, .b = 7, .c = 5},
    {.a = 2, .b = 5, .c = 3},
    // botton
    {.a = 6, .b = 8, .c = 1},
    {.a = 6, .b = 1, .c = 4}
};*/

typedef struct
{
    float x;
    float y;
    float z;
    float w;
}vec4_t;
typedef struct
{
    int a;
    int b;
    int c;
}vec3_t;
typedef struct 
{
    vec3_t vertex_index;
    vec3_t texture_index;
    vec3_t normal_index;
}face_t;

dynarray_t* vertices = NULL;
dynarray_t* faces = NULL;
void parse_face(const char* input)
{
    face_t face;

    const int type = 10;
    char* endptr = NULL;
    // face
    face.vertex_index.a = strtol(input+2,&endptr,type);
    face.vertex_index.b = strtol(endptr+1,&endptr,type);
    face.vertex_index.c = strtol(endptr+1,&endptr,type);
    
    face.texture_index.a = strtol(endptr+1,&endptr,type);
    face.texture_index.b = strtol(endptr+1,&endptr,type);
    face.texture_index.c = strtol(endptr+1,&endptr,type);

    face.normal_index.a = strtol(endptr+1,&endptr,type);
    face.normal_index.b = strtol(endptr+1,&endptr,type);
    face.normal_index.c = strtol(endptr+1,&endptr,type);
 
    //printf("face v1 a:%d, b=%d, c:%d\n", face.v1.a, face.v1.b, face.v1.c);
    //printf("face v2 a:%d, b=%d, c:%d\n", face.v2.a, face.v2.b, face.v2.c);
    //printf("face v3 a:%d, b=%d, c:%d\n", face.v3.a, face.v3.b, face.v3.c);
    dynArray_push_back(faces,&face);
}
void parse_vertex(const char *input)
{
    vec4_t vector;
    char* endptr = NULL;
    vector.x = strtof(input+2,&endptr);
    vector.y = strtof(endptr,&endptr);
    vector.z = strtof(endptr,&endptr);
    vector.w = strtof(endptr,&endptr);
    //printf("x:%.2f, y=%.2f, z:%.2f, w:%.2f\n", vector.x, vector.y, vector.z,vector.w);
    dynArray_push_back(vertices,&vector);
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
      switch (items[0])
      {
      case 'v':
        if(items[1] == ' ')
            {
                parse_vertex(items);
            }
        break;
      case 'f':
        if(items[1] == ' ')
        {
            parse_face(items);
        }
        break;
      default:
        break;
      }
    }
    dynArray_clear(input);
}
bool render_vertex(void)
{
    size_t size = dynArray_get_length(vertices);
    const vec4_t *items = dynArray_get_items(vertices);
    if (items == NULL)
    {
        fprintf(stderr, "Items from vertices is NULL\n");
        return false;
    }
    printf("Vertices\n");

    for (size_t i = 0; i < size; i++)
    {
        vec4_t item = items[i];
        printf("x:%.2f, y=%.2f, z:%.2f, w:%.2f\n", item.x, item.y, item.z, item.w);
    }
    return true;
}
bool render_faces(void)
{
    size_t size = dynArray_get_length(faces);
    const face_t *items = dynArray_get_items(faces);
    if (items == NULL)
    {
        fprintf(stderr, "Items from faces is NULL\n");
        return false;
    }
    printf("faces\n");
    printf("vertex index :\n");
    for (size_t i = 0; i < size; i++)
    {
        face_t item = items[i];
        printf("a:%d, b=%d, c:%d\n", item.vertex_index.a, item.vertex_index.b, item.vertex_index.c);
    }
    printf("texture index :\n");
    for (size_t i = 0; i < size; i++)
    {
        face_t item = items[i];
        printf("a:%d, b=%d, c:%d\n", item.texture_index.a, item.texture_index.b, item.texture_index.c);
    }
    printf("normal index :\n");
    for (size_t i = 0; i < size; i++)
    {
        face_t item = items[i];
        printf("a:%d, b=%d, c:%d\n", item.normal_index.a, item.normal_index.b, item.normal_index.c);
    }
    return true;
}
bool start_parsing(FILE* file)
{
    if(file == NULL)
    {
        fprintf(stderr,"failed to open file\n");
        return EXIT_FAILURE;
    }
    vertices = dynArray_create(sizeof(vec4_t));
    faces = dynArray_create(sizeof(face_t));

    int ch = 0;
    int parsing = 1;
    dynarray_t* line_buffer = dynArray_create(sizeof(char));
    if(line_buffer == NULL)
    {
        fprintf(stderr,"line_buffer push_back failed\n");
        return false;
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
                return false;
            }
        }
        else
        {
            char end = '\0';
            dynArray_push_back(line_buffer,&end);
            parse_line(line_buffer);
        }
    }
    dynArray_free(&line_buffer);
    return true;
}
int main(int argc,char* argv[]) {
    char* filename = "Assets/cube.obj";
    if(argc > 1)
    {
        filename = argv[1];
    }
    FILE* file = NULL;
    printf("filename is %s\n",filename);
    file = fopen(filename,"r");
    bool bsuccess = start_parsing(file);
    if(bsuccess)
    {
        render_vertex();
        render_faces();
    }
    if(file != NULL)
    {
        fclose(file);
    }

    dynArray_free(&vertices);
    vertices = NULL;
    dynArray_free(&faces);
    faces = NULL;
    //printf("Programa terminado. Pulsa ENTER para salir...");
    //getchar();
    return EXIT_SUCCESS;
}