#include "surface.h"
// #include "box.h"
#include "bvh.h"
#define EXTREMUM 1000000000
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"
int ray_plane_intersections = 0;
int total_faces = 0;
int triangles_intersected = 0;
int o_edges = 0;
int s_edges = 0;
int t_edges = 0;
int total_faces_f = 0;
BoxNode m_root;
Vector3f get_centroid(Vector3f v1, Vector3f v2, Vector3f v3)
{
    Vector3f centroid;
    centroid.x = (v1.x + v2.x + v3.x) / 3;
    centroid.y = (v1.y + v2.y + v3.y) / 3;
    centroid.z = (v1.z + v2.z + v3.z) / 3;
    return centroid;
}

std::vector<Surface> createSurfaces(std::string pathToObj, bool isLight, uint32_t shapeIdx)
{
    std::string objDirectory;
    const size_t last_slash_idx = pathToObj.rfind('/');
    if (std::string::npos != last_slash_idx)
    {
        objDirectory = pathToObj.substr(0, last_slash_idx);
    }

    std::vector<Surface> surfaces;

    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig reader_config;
    if (!reader.ParseFromFile(pathToObj, reader_config))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();

    // Loop over shapes
    std::vector<Vector3f> v_box;

    for (size_t s = 0; s < shapes.size(); s++)
    {   
        Surface surf;
        surf.isLight = isLight;
        surf.shapeIdx = shapeIdx;
        std::set<int> materialIds;

        // Loop over faces(polygon)
        size_t index_offset = 0;

        std::vector<Vector3f> v_min;
        std::vector<Vector3f> v_max;
        double x_min = EXTREMUM;
        double x_max = -EXTREMUM;
        double y_min = EXTREMUM;
        double y_max = -EXTREMUM;
        double z_min = EXTREMUM;
        double z_max = -EXTREMUM;

        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {   

            std::vector<Vector3f> vertices_triangle;

            total_faces_f++;
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            if (fv > 3)
            {
                std::cerr << "Not a triangle mesh" << std::endl;
                exit(1);
            }

            std::vector<Vector3f> vertices_f;
            // Vector3f[3] faces_triangle;
            // Loop over vertices in the face. Assume 3 vertices per-face
            Vector3f vertices[3], normals[3];
            Vector2f uvs[3];
            for (size_t v = 0; v < fv; v++)
            {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];



                if (idx.normal_index >= 0)
                {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];

                    normals[v] = Vector3f(nx, ny, nz);
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0)
                {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

                    uvs[v] = Vector2f(tx, ty);
                }

                vertices[v] = Vector3f(vx, vy, vz);

                Vector3f centroid = get_centroid(vertices[v], vertices[v], vertices[v]);
            
            if (centroid.x < x_min)
            {
                x_min = centroid.x;
            }
            if (centroid.x > x_max)
            {
                x_max = centroid.x;
            }
            if (centroid.y < y_min)
            {
                y_min = centroid.y;
            }
            if (centroid.y > y_max)
            {
                y_max = centroid.y;
            }
            if (centroid.z < z_min)
            {
                z_min = centroid.z;
            }
            if (centroid.z > z_max)
            {
                z_max = centroid.z;
            }

                vertices_f.push_back(Vector3f(vx, vy, vz));
            }

            // v_box.push_back(centroid);
   
            int vSize = surf.vertices.size();
            Vector3i findex(vSize, vSize + 1, vSize + 2);

            surf.vertices.push_back(vertices[0]);
            surf.vertices.push_back(vertices[1]);
            surf.vertices.push_back(vertices[2]);

            surf.normals.push_back(normals[0]);
            surf.normals.push_back(normals[1]);
            surf.normals.push_back(normals[2]);

            surf.uvs.push_back(uvs[0]);
            surf.uvs.push_back(uvs[1]);
            surf.uvs.push_back(uvs[2]);

            surf.indices.push_back(findex);
            // per-face material
            materialIds.insert(shapes[s].mesh.material_ids[f]);

            index_offset += fv;
             
        }

        Vector3f min(x_min, y_min, z_min);
        Vector3f max(x_max, y_max, z_max);

        BoxNode* main_c= createBox(v_box);
        main_c->min = min;
        main_c->max = max;

        surf.main_box = main_c;
        if (materialIds.size() > 1)
        {
            std::cerr << "One of the meshes has more than one material. This is not allowed." << std::endl;
            exit(1);
        }

        if (materialIds.size() == 0)
        {
            std::cerr << "One of the meshes has no material definition, may cause unexpected behaviour." << std::endl;
        }
        else
        {
            // Load textures from Materials
            auto matId = *materialIds.begin();
            if (matId != -1)
            {
                auto mat = materials[matId];

                surf.diffuse = Vector3f(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
                if (mat.diffuse_texname != "")
                    surf.diffuseTexture = Texture(objDirectory + "/" + mat.diffuse_texname);

                surf.alpha = mat.specular[0];
                if (mat.alpha_texname != "")
                    surf.alphaTexture = Texture(objDirectory + "/" + mat.alpha_texname);
            }
        }


        surfaces.push_back(surf);
        shapeIdx++;
    }
    return surfaces;
}

bool Surface::hasDiffuseTexture() { return this->diffuseTexture.data != 0; }

bool Surface::hasAlphaTexture() { return this->alphaTexture.data != 0; }

Interaction Surface::rayPlaneIntersect(Ray ray, Vector3f p, Vector3f n)
{
    Interaction si;

    double dDotN = Dot(ray.d, n);
    if (dDotN != 0.0)
    {
        double t = -Dot((ray.o - p), n) / dDotN;
        if (t >= 0.0)
        {
            ray_plane_intersections++;
            si.didIntersect = true;
            si.t = t;
            si.n = n;
            si.p = ray.o + ray.d * si.t;
        }
    }

    return si;
}

Interaction Surface::rayTriangleIntersect(Ray ray, Vector3f v1, Vector3f v2, Vector3f v3, Vector3f n)
{
    Interaction si = this->rayPlaneIntersect(ray, v1, n);

    if (si.didIntersect)
    {
        bool edge1 = false, edge2 = false, edge3 = false;
        bool e_edge_1 = false;
        // // Check edge 1
        {

            Vector3f nIp = Cross((si.p - v1), (v3 - v1));
            Vector3f nTri = Cross((v2 - v1), (v3 - v1));

            edge1 = Dot(nIp, nTri) > 0;
            if (edge1)
                o_edges++;
        }

        // Check edge 2
        {
            Vector3f nIp = Cross((si.p - v1), (v2 - v1));
            Vector3f nTri = Cross((v3 - v1), (v2 - v1));
            edge2 = Dot(nIp, nTri) > 0;
            if (edge2)
                s_edges++;
        }

        // Check edge 3
        {

            Vector3f nIp = Cross((si.p - v2), (v3 - v2));
            Vector3f nTri = Cross((v1 - v2), (v3 - v2));
            edge3 = Dot(nIp, nTri) > 0;
            if (edge3)
                t_edges++;
        }

        if (edge1 && edge2 && edge3)
        {
            // Intersected triangle!
            triangles_intersected++;
            si.didIntersect = true;
        }
        else
        {
            si.didIntersect = false;
        }
    }

    return si;
}

Interaction Surface::rayIntersect(Ray ray)
{
    Interaction siFinal;
    double tmin = ray.t;
    // naive ray triangle intersection
    for (auto face : this->indices)
    {

        Vector3f p1 = this->vertices[face.x];
        Vector3f p2 = this->vertices[face.y];
        Vector3f p3 = this->vertices[face.z];

        Vector3f n1 = this->normals[face.x];
        Vector3f n2 = this->normals[face.y];
        Vector3f n3 = this->normals[face.z];
        Vector3f n = Normalize(n1 + n2 + n3);
        total_faces++;
        Interaction si = this->rayTriangleIntersect(ray, p1, p2, p3, n);
        if (si.t <= tmin && si.didIntersect)
        {
            siFinal = si;
            tmin = si.t;
        }
    }

    return siFinal;

    




}