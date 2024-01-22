#include "box.h"
#define EXTREMUM 1000000000
#define MIN_SIZE 2

int total_boxes = 0;

BoxNode *createBox(std::vector<Vector3f> v)
{
    struct BoxNode *node = new BoxNode();
    node->v = v;
    node->left = NULL;
    node->right = NULL;
    node->isLeaf = true;
    return node;
}

void divide_box(BoxNode *node)
{

    if (node->v.size() <= MIN_SIZE)
    {
        return;
    }
    total_boxes++;

    double x_min = EXTREMUM;
    double x_max = -EXTREMUM;
    double y_min = EXTREMUM;
    double y_max = -EXTREMUM;
    double z_min = EXTREMUM;
    double z_max = -EXTREMUM;

    Vector3f left_min = Vector3f(EXTREMUM, EXTREMUM, EXTREMUM);
    Vector3f left_max = Vector3f(-EXTREMUM, -EXTREMUM, -EXTREMUM);
    Vector3f right_min = Vector3f(EXTREMUM, EXTREMUM, EXTREMUM);
    Vector3f right_max = Vector3f(-EXTREMUM, -EXTREMUM, -EXTREMUM);

    for (int i = 0; i < node->v.size(); i++)
    {

        if (node->v[i].x < x_min)
        {
            x_min = node->v[i].x;
        }
        if (node->v[i].x > x_max)
        {
            x_max = node->v[i].x;
        }
        if (node->v[i].y < y_min)
        {
            y_min = node->v[i].y;
        }
        if (node->v[i].y > y_max)
        {
            y_max = node->v[i].y;
        }
        if (node->v[i].z < z_min)
        {
            z_min = node->v[i].z;
        }
        if (node->v[i].z > z_max)
        {
            z_max = node->v[i].z;
        }
    }
    double x_length = x_max - x_min;
    double y_length = y_max - y_min;
    double z_length = z_max - z_min;

    int axis = 0;
    if (x_length >= y_length && x_length >= z_length)
    {
        axis = 0;
    }
    else if (y_length >= x_length && y_length >= z_length)
    {
        axis = 1;
    }
    else
    {
        axis = 2;
    }
    std::vector<Vector3f> left_v;
    std::vector<Vector3f> right_v;
    if (axis == 0)
    {
        double x_mid = (x_max + x_min) / 2;
        for (int i = 0; i < node->v.size(); i++)
        {
            if (node->v[i].x < x_mid)
            {
                left_v.push_back(node->v[i]);

                if (node->v[i].x < left_min.x)
                {
                    left_min.x = node->v[i].x;
                }
                if (node->v[i].x > left_max.x)
                {
                    left_max.x = node->v[i].x;
                }
                if (node->v[i].y < left_min.y)
                {
                    left_min.y = node->v[i].y;
                }
                if (node->v[i].y > left_max.y)
                {
                    left_max.y = node->v[i].y;
                }
                if (node->v[i].z < left_min.z)
                {
                    left_min.z = node->v[i].z;
                }
                if (node->v[i].z > left_max.z)
                {
                    left_max.z = node->v[i].z;
                }

            }
            else
            {
                right_v.push_back(node->v[i]);
                if (node->v[i].x < right_min.x)
                {
                    right_min.x = node->v[i].x;
                }
                if (node->v[i].x > right_max.x)
                {
                    right_max.x = node->v[i].x;
                }
                if (node->v[i].y < right_min.y)
                {
                    right_min.y = node->v[i].y;
                }
                if (node->v[i].y > right_max.y)
                {
                    right_max.y = node->v[i].y;
                }
                if (node->v[i].z < right_min.z)
                {
                    right_min.z = node->v[i].z;
                }
                if (node->v[i].z > right_max.z)
                {
                    right_max.z = node->v[i].z;
                }
                

            }
        }
    }
    else if (axis == 1)
    {
        double y_mid = (y_max + y_min) / 2;
        for (int i = 0; i < node->v.size(); i++)
        {
            if (node->v[i].y < y_mid)
            {
                left_v.push_back(node->v[i]);
                if (node->v[i].x < left_min.x)
                {
                    left_min.x = node->v[i].x;
                }
                if (node->v[i].x > left_max.x)
                {
                    left_max.x = node->v[i].x;
                }
                if (node->v[i].y < left_min.y)
                {
                    left_min.y = node->v[i].y;
                }
                if (node->v[i].y > left_max.y)
                {
                    left_max.y = node->v[i].y;
                }
                if (node->v[i].z < left_min.z)
                {
                    left_min.z = node->v[i].z;
                }
                if (node->v[i].z > left_max.z)
                {
                    left_max.z = node->v[i].z;
                }
            }
            else
            {
                right_v.push_back(node->v[i]);
                if (node->v[i].x < right_min.x)
                {
                    right_min.x = node->v[i].x;
                }
                if (node->v[i].x > right_max.x)
                {
                    right_max.x = node->v[i].x;
                }
                if (node->v[i].y < right_min.y)
                {
                    right_min.y = node->v[i].y;
                }
                if (node->v[i].y > right_max.y)
                {
                    right_max.y = node->v[i].y;
                }
                if (node->v[i].z < right_min.z)
                {
                    right_min.z = node->v[i].z;
                }
                if (node->v[i].z > right_max.z)
                {
                    right_max.z = node->v[i].z;
                }
            }
        }
    }
    else
    {
        double z_mid = (z_max + z_min) / 2;
        for (int i = 0; i < node->v.size(); i++)
        {
            if (node->v[i].z < z_mid)
            {
                left_v.push_back(node->v[i]);
                if (node->v[i].x < left_min.x)
                {
                    left_min.x = node->v[i].x;
                }
                if (node->v[i].x > left_max.x)
                {
                    left_max.x = node->v[i].x;
                }
                if (node->v[i].y < left_min.y)
                {
                    left_min.y = node->v[i].y;
                }
                if (node->v[i].y > left_max.y)
                {
                    left_max.y = node->v[i].y;
                }
                if (node->v[i].z < left_min.z)
                {
                    left_min.z = node->v[i].z;
                }
                if (node->v[i].z > left_max.z)
                {
                    left_max.z = node->v[i].z;
                }
            }
            else
            {
                right_v.push_back(node->v[i]);
                if (node->v[i].x < right_min.x)
                {
                    right_min.x = node->v[i].x;
                }
                if (node->v[i].x > right_max.x)
                {
                    right_max.x = node->v[i].x;
                }
                if (node->v[i].y < right_min.y)
                {
                    right_min.y = node->v[i].y;
                }
                if (node->v[i].y > right_max.y)
                {
                    right_max.y = node->v[i].y;
                }
                if (node->v[i].z < right_min.z)
                {
                    right_min.z = node->v[i].z;
                }
                if (node->v[i].z > right_max.z)
                {
                    right_max.z = node->v[i].z;
                }
            }
        }
    }

    node->left = (createBox(left_v));
    node->right = (createBox(right_v));
    node->left->min = left_min;
    node->left->max = left_max;
    divide_box((node->left));
    divide_box((node->right));
    node->isLeaf = false;
    return;
}

std::vector<BoxNode *> get_all_boxes(BoxNode *node)
{
    std::vector<BoxNode *> boxes;
    if (node->isLeaf)
    {
        boxes.push_back(node);
        return boxes;
    }
    else
    {
        std::vector<BoxNode *> left_boxes = get_all_boxes(node->left);
        std::vector<BoxNode *> right_boxes = get_all_boxes(node->right);
        boxes.insert(boxes.end(), left_boxes.begin(), left_boxes.end());
        boxes.insert(boxes.end(), right_boxes.begin(), right_boxes.end());
        return boxes;
    }
}

bool slabtest(Ray ray, BoxNode *box)
{
    
    double tmin = 0.0,tmax = EXTREMUM;

    double t1,t2;
    Vector3f invdir = Vector3f(1.0/ray.d.x,1.0/ray.d.y,1.0/ray.d.z);

    t1= (box->min.x - ray.o.x)*invdir.x;
    t2= (box->max.x - ray.o.x)*invdir.x;

    tmin = std::max(tmin,std::min(t1,t2));
    tmax = std::min(tmax,std::max(t1,t2));

    t1= (box->min.y - ray.o.y)*invdir.y;
    t2= (box->max.y - ray.o.y)*invdir.y;

    tmin = std::max(tmin,std::min(t1,t2));
    tmax = std::min(tmax,std::max(t1,t2));

    t1= (box->min.z - ray.o.z)*invdir.z;
    t2= (box->max.z - ray.o.z)*invdir.z;

    tmin = std::max(tmin,std::min(t1,t2));
    tmax = std::min(tmax,std::max(t1,t2));

    return tmin < tmax;




}