#include <polygon_collider/vector_object_queue.h>
#include <polygon_collider/vector_object.h>
#include <stdlib.h>

#define QUEUE_SIZE 100
#define VECTOR_LIFESPAN 3.0f

struct VectorObjectQueue
{
    VectorObject *queue[QUEUE_SIZE];
    float destruction_time[QUEUE_SIZE];
    unsigned int left, right;
};

VectorObjectQueue *vector_object_queue_create()
{
    VectorObjectQueue *vector_object_queue = malloc(sizeof(VectorObjectQueue));

    vector_object_queue->left = vector_object_queue->right = 0;

    return vector_object_queue;
}

void vector_object_queue_destroy(VectorObjectQueue *vector_object_queue)
{
    vector_object_queue->left = vector_object_queue->right = 0;

    free(vector_object_queue);
    vector_object_queue = NULL;
}

void vector_object_queue_submit_vector(VectorObjectQueue *vector_object_queue, VectorObject *vector_object)
{
    vector_object_queue->queue[vector_object_queue->right] = vector_object;
    vector_object_queue->destruction_time[vector_object_queue->right] = VECTOR_LIFESPAN;
    vector_object_queue->right = (vector_object_queue->right + 1) % QUEUE_SIZE;
}

void vector_object_queue_update(VectorObjectQueue *vector_object_queue, float delta_time)
{
    for(int i = vector_object_queue->left; i != vector_object_queue->right; i = (i + 1) % QUEUE_SIZE)
        vector_object_queue->destruction_time[i] -= delta_time;

    while(vector_object_queue->destruction_time[vector_object_queue->left] < 0.0f)
        vector_object_destroy(vector_object_queue->queue[vector_object_queue->left++]);
}

void vector_object_queue_submit_to_renderer(VectorObjectQueue *vector_object_queue, Renderer *renderer)
{
    for(int i = vector_object_queue->left; i != vector_object_queue->right; i = (i + 1) % QUEUE_SIZE)
        renderer_submit_vector(renderer, vector_object_queue->queue[i]);
}
