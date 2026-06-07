#include <polygon_collider/app.h>
#include <stddef.h>

int main()
{
    App *app = app_create();
    if(app == NULL)
        return 1;

    app_run(app);

    app_destroy(app);
    app = NULL;

    return 0;
}
