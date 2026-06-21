#include <polygon_collider/vector_object.h>
#include <stdlib.h>

struct VectorObject
{
    Vector vector;
    Vector position;
    Color color;
};

VectorObject *vector_object_create()
{
    VectorObject *vector_object = malloc(sizeof(VectorObject));

    vector_object->vector = vector_object->position = vector_get(0.0f, 0.0f);
    vector_object->color = color_get(0.0f, 0.0f, 0.0f);

    return vector_object;
}

void vector_object_destroy(VectorObject *vector_object)
{
    vector_object->vector = vector_object->position = vector_get(0.0f, 0.0f);
    vector_object->color = color_get(0.0f, 0.0f, 0.0f);

    free(vector_object);
    vector_object = NULL;
}

void vector_object_set_vector(VectorObject *vector_object, Vector vector)
{
    vector_object->vector = vector;
}

void vector_object_set_position(VectorObject *vector_object, Vector position)
{
    vector_object->position = position;
}

void vector_object_set_color(VectorObject *vector_object, Color color)
{
    vector_object->color = color;
}

Edge vector_object_get_edge(VectorObject *vector_object)
{
    Edge edge;
    edge.initial_point = vector_object->position;
    edge.terminal_point = vector_get_sum(vector_object->position, vector_object->vector);
    return edge;
}

Color vector_object_get_color(VectorObject *vector_object)
{
    return vector_object->color;
}

void vector_object_copy_tip(VectorObject *vector_object, Vector *tip_destination)
{
    const float tip_width = 0.1f, tip_length = 0.2f;
    Vector head = vector_get_sum(vector_object->position, vector_object->vector);

    tip_destination[0] = head;
    tip_destination[1] = vector_get_sum(head, vector_get_scaled(vector_get_normalized(vector_get_rotated(vector_object->vector, 3.14f * (1.0f - tip_width))), tip_length));
    tip_destination[2] = vector_get_sum(head, vector_get_scaled(vector_get_normalized(vector_get_rotated(vector_object->vector, -3.14f * (1.0f - tip_width))), tip_length));
}
