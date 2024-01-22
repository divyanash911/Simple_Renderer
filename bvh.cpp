#include "bvh.h"

#define EXTREMUM 1000000000
int called_count = 0;
Vector3f get_centroidf(Vector3f v1, Vector3f v2, Vector3f v3)
{
    Vector3f centroid;
    centroid.x = (v1.x + v2.x + v3.x) / 3;
    centroid.y = (v1.y + v2.y + v3.y) / 3;
    centroid.z = (v1.z + v2.z + v3.z) / 3;
    return centroid;
}

Bvh createBvh(std::vector<Surface> surfaces)
{

    Bvh root = new BvhNode;
    root->surfaces = surfaces;

    root->left = NULL;
    root->right = NULL;

    return root;
}

void divideBvh(Bvh root)
{

    std::vector<Surface> left;
    std::vector<Surface> right;
    Vector3f min = Vector3f(EXTREMUM, EXTREMUM, EXTREMUM);
    Vector3f max = Vector3f(-EXTREMUM, -EXTREMUM, -EXTREMUM);

    for (auto surface : root->surfaces)
    {

        // distribute each surface in left or right

        for (auto face : surface.indices)
        {

            Vector3f v1 = surface.vertices[face.x];
            Vector3f v2 = surface.vertices[face.y];
            Vector3f v3 = surface.vertices[face.z];

            Vector3f centroid = get_centroidf(v1, v2, v3);

            if (centroid.x < min.x)
            {
                min.x = centroid.x;
            }
            if (centroid.y < min.y)
            {
                min.y = centroid.y;
            }
            if (centroid.z < min.z)
            {
                min.z = centroid.z;
            }
            if (centroid.x > max.x)
            {
                max.x = centroid.x;
            }
            if (centroid.y > max.y)
            {
                max.y = centroid.y;
            }
            if (centroid.z > max.z)
            {
                max.z = centroid.z;
            }
        }
    }
    double x_d = max.x - min.x;
    double y_d = max.y - min.y;
    double z_d = max.z - min.z;

    int axis = 0;

    if (x_d > y_d && x_d > z_d)
    {
        axis = 0;
    }
    else if (y_d > x_d && y_d > z_d)
    {
        axis = 1;
    }
    else
    {
        axis = 2;
    }

    if (axis == 0)
    {

        for (auto& surface : root->surfaces)
        {
            double x_min_surface = EXTREMUM;
            double x_max_surface = -EXTREMUM;
            for (auto face : surface.indices)
            {

                Vector3f v1 = surface.vertices[face.x];
                Vector3f v2 = surface.vertices[face.y];
                Vector3f v3 = surface.vertices[face.z];

                Vector3f centroid = get_centroidf(v1, v2, v3);

                if (centroid.x < x_min_surface)
                {
                    x_min_surface = centroid.x;
                }

                if (centroid.x > x_max_surface)
                {
                    x_max_surface = centroid.x;
                }

            }


            if(x_max_surface < x_d/2 + min.x)
            {
                left.push_back(surface);
            }
            else
            {
                right.push_back(surface);
            }
            

        }
    }

    else if (axis == 1)
    {
        for (auto& surface : root->surfaces)
        {
            double y_min_surface = EXTREMUM;
            double y_max_surface = -EXTREMUM;

            for (auto face : surface.indices)
            {

                Vector3f v1 = surface.vertices[face.x];
                Vector3f v2 = surface.vertices[face.y];
                Vector3f v3 = surface.vertices[face.z];

                Vector3f centroid = get_centroidf(v1, v2, v3);

                if (centroid.y < y_min_surface)
                {
                    y_min_surface = centroid.y;
                }

                if (centroid.y > y_max_surface)
                {
                    y_max_surface = centroid.y;
                }

            }


            if(y_max_surface < y_d/2 + min.y)
            {
                left.push_back(surface);
            }
            else 
            {
                right.push_back(surface);
            }
            

        }
        
    }

    else
    {

        for (auto& surface : root->surfaces)
        {
            double z_min_surface = EXTREMUM;
            double z_max_surface = -EXTREMUM;
            for (auto face : surface.indices)
            {

                Vector3f v1 = surface.vertices[face.x];
                Vector3f v2 = surface.vertices[face.y];
                Vector3f v3 = surface.vertices[face.z];

                Vector3f centroid = get_centroidf(v1, v2, v3);

                if (centroid.z < z_min_surface)
                {
                    z_min_surface = centroid.z;
                }

                if (centroid.z > z_max_surface)
                {
                    z_max_surface = centroid.z;
                }

            }


            if(z_max_surface < z_d/2 + min.z)
            {
                left.push_back(surface);
            }
            else
            {
                right.push_back(surface);
            }

            
            

        }
    }

    // std::cout << left.size() << " " << right.size() << std::endl; 

    if(left.size() == 0 || right.size() == 0)
    {
        root->left = NULL;
        root->right = NULL;
        return;
    }

    // root->surfaces.clear();
    root->left = createBvh(left);   
    root->right = createBvh(right);

    divideBvh(root->left);
    divideBvh(root->right);


}

bool intersects_me(Bvh node , Ray &ray){

    double tmin = 1000000000;
    double tmax = -1000000000;
    Vector3f min = Vector3f(EXTREMUM, EXTREMUM, EXTREMUM);
    Vector3f max = Vector3f(-EXTREMUM, -EXTREMUM, -EXTREMUM);
    for (auto surface : node->surfaces)
    {
        for (auto face : surface.indices)
        {
            Vector3f v1 = surface.vertices[face.x];
            Vector3f v2 = surface.vertices[face.y];
            Vector3f v3 = surface.vertices[face.z];

            Vector3f centroid = get_centroidf(v1, v2, v3);

            if(centroid.x < min.x)
            {
                min.x = centroid.x;
            }
            
            if(centroid.x > max.x)
            {
                max.x = centroid.x;
            }
            if(centroid.y < min.y)
            {
                min.y = centroid.y;
            }
            if(centroid.y > max.y)
            {
                max.y = centroid.y;
            }
            if(centroid.z < min.z)
            {
                min.z = centroid.z;
            }
            if(centroid.z > max.z)
            {
                max.z = centroid.z;
            }

        }
    }

    double t1,t2;
    Vector3f invdir = Vector3f(1.0/ray.d.x,1.0/ray.d.y,1.0/ray.d.z);

    t1= (min.x - ray.o.x)*invdir.x;
    t2= (max.x - ray.o.x)*invdir.x;

    tmin = std::max(tmin,std::min(t1,t2));
    tmax = std::min(tmax,std::max(t1,t2));

    t1= (min.y - ray.o.y)*invdir.y;
    t2= (max.y - ray.o.y)*invdir.y;

    tmin = std::max(tmin,std::min(t1,t2));
    tmax = std::min(tmax,std::max(t1,t2));

    t1= (min.z - ray.o.z)*invdir.z;
    t2= (max.z - ray.o.z)*invdir.z;

    tmin = std::max(tmin,std::min(t1,t2));
    tmax = std::min(tmax,std::max(t1,t2));

    return tmin < tmax;

    


}

Interaction rayIntersectBvh(Bvh root, Ray &ray)
{
    Interaction siFinal;
    double tmin = ray.t;
    // naive ray triangle intersection
    for (auto surface : root->surfaces)
    {
        for (auto face : surface.indices)
        {
            Vector3f p1 = surface.vertices[face.x];
            Vector3f p2 = surface.vertices[face.y];
            Vector3f p3 = surface.vertices[face.z];

            Vector3f n1 = surface.normals[face.x];
            Vector3f n2 = surface.normals[face.y];
            Vector3f n3 = surface.normals[face.z];
            Vector3f n = Normalize(n1 + n2 + n3);
            total_faces++;
            Interaction si = surface.rayTriangleIntersect(ray, p1, p2, p3, n);
            if (si.t <= tmin && si.didIntersect)
            {
                siFinal = si;
                tmin = si.t;
            }
        }
    }

    if(root->left == NULL && root->right == NULL)
    {
        return siFinal;
    }

    if(intersects_me(root->left,ray))
    {
        Interaction si = rayIntersectBvh(root->left,ray);
        if(si.t <= tmin && si.didIntersect)
        {
            siFinal = si;
            tmin = si.t;
        }
        // return siFinal;
    }

    if(intersects_me(root->right,ray))
    {
        Interaction si = rayIntersectBvh(root->right,ray);
        if(si.t <= tmin && si.didIntersect)
        {
            siFinal = si;
            tmin = si.t;
        }
    }

    return siFinal;
}