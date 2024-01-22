#include "scene.h"
// #include "bvh.h"
long long int total_surfaces = 0;
Scene::Scene(std::string sceneDirectory, std::string sceneJson)
{
    nlohmann::json sceneConfig;
    try
    {
        sceneConfig = nlohmann::json::parse(sceneJson);
    }
    catch (std::runtime_error e)
    {
        std::cerr << "Could not parse json." << std::endl;
        exit(1);
    }

    this->parse(sceneDirectory, sceneConfig);
}

Scene::Scene(std::string pathToJson)
{
    std::string sceneDirectory;

#ifdef _WIN32
    const size_t last_slash_idx = pathToJson.rfind('\\');
#else
    const size_t last_slash_idx = pathToJson.rfind('/');
#endif

    if (std::string::npos != last_slash_idx)
    {
        sceneDirectory = pathToJson.substr(0, last_slash_idx);
    }

    nlohmann::json sceneConfig;
    try
    {
        std::ifstream sceneStream(pathToJson.c_str());
        sceneStream >> sceneConfig;
    }
    catch (std::runtime_error e)
    {
        std::cerr << "Could not load scene .json file." << std::endl;
        exit(1);
    }

    this->parse(sceneDirectory, sceneConfig);
}

void Scene::parse(std::string sceneDirectory, nlohmann::json sceneConfig)
{
    // Output
    try
    {
        auto res = sceneConfig["output"]["resolution"];
        this->imageResolution = Vector2i(res[0], res[1]);
    }
    catch (nlohmann::json::exception e)
    {
        std::cerr << "\"output\" field with resolution, filename & spp should be defined in the scene file." << std::endl;
        exit(1);
    }

    // Cameras
    try
    {
        auto cam = sceneConfig["camera"];

        this->camera = Camera(
            Vector3f(cam["from"][0], cam["from"][1], cam["from"][2]),
            Vector3f(cam["to"][0], cam["to"][1], cam["to"][2]),
            Vector3f(cam["up"][0], cam["up"][1], cam["up"][2]),
            float(cam["fieldOfView"]),
            this->imageResolution);
    }
    catch (nlohmann::json::exception e)
    {
        std::cerr << "No camera(s) defined. Atleast one camera should be defined." << std::endl;
        exit(1);
    }

    // Surface
    try
    {
        auto surfacePaths = sceneConfig["surface"];

        uint32_t surfaceIdx = 0;
        std::vector<Vector3f> box_vertices;

        for (std::string surfacePath : surfacePaths)
        {
            surfacePath = sceneDirectory + "/" + surfacePath;

            auto surf = createSurfaces(surfacePath, /*isLight=*/false, /*idx=*/surfaceIdx);

            this->surfaces.insert(this->surfaces.end(), surf.begin(), surf.end());

            surfaceIdx = surfaceIdx + surf.size();
        }
        int box_count = 0;
        std::vector<Surface> box_surfaces;
        for (auto &surface : this->surfaces)
        {
            
            box_surfaces.push_back(surface);

        }

        Bvh root = createBvh(box_surfaces);
        divideBvh(root);

        this->m_bvh = root;
        


        BoxNode *root_m = createBox(box_vertices);

        divide_box(root_m);

        this->m_root = root_m;
    }
    catch (nlohmann::json::exception e)
    {
        std::cout << "No surfaces defined." << std::endl;
    }
}

Interaction Scene::rayIntersect(Ray &ray)
{
    int choice = -1;

    Interaction siFinal;

    if(choice == -1){
        for (auto& surface : this->surfaces) {
        Interaction si = surface.rayIntersect(ray);
        if (si.t <= ray.t) {    
            siFinal = si;
            ray.t = si.t;
        }
    }
    }


    else if(choice == 1){
    for (auto &surface : this->surfaces)
    {
        BoxNode *root = surface.main_box;
        double tmin  = 1000000000;
        
        if (slabtest(ray, root))
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
    }
    }
    else{

       Interaction si = rayIntersectBvh(this->m_bvh, ray); 
        siFinal = si;
    }
    
    
    return siFinal;

}