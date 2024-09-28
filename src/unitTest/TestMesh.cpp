#include <framework/EasyDefine.hpp>
#include <framework/FeatureEnvMgr.hpp>
#include <geometry/MeshMemStorage.hpp>

#include <vector>

ANONYMOUS_SCOPE_BEGIN

template<MeshStorageType MeshStorageTypeValue>
bool TouchCheck(MeshMemStorage<MeshStorageTypeValue>& rMeshMemStorage, size_t i)
{
    MeshDataType meshDataType = rMeshMemStorage.GetMeshDataType();
    if (meshDataType & MeshDataType::POSITION)
    {
        auto position = rMeshMemStorage.GetPosition(i);
        rMeshMemStorage.SetPosition(i, position + position);
    }

    if (meshDataType & MeshDataType::COLOR)
    {
        auto color = rMeshMemStorage.GetColor(i);
        rMeshMemStorage.SetColor(i, color + color);
    }

    if (meshDataType & MeshDataType::NORMAL)
    {
        auto normal = rMeshMemStorage.GetNormal(i);
        rMeshMemStorage.SetNormal(i, normal + normal);
    }

    return true;
}

template<MeshStorageType MeshStorageTypeValue>
bool TestMeshMemStorage(MeshMemStorage<MeshStorageTypeValue>& rMeshMemStorage)
{
    size_t uVertexCount = rMeshMemStorage.GetVertexCount();
    assert(uVertexCount > 0);
    if (MeshStorageTypeValue != rMeshMemStorage.GetMeshStorageType())
    {
        return false;
    }
    //touch mem
    for (size_t i = 0; i < uVertexCount; ++i)
    {
        if (!TouchCheck(rMeshMemStorage, i))
        {
            return false;
        }
    }

    return true;
}

void TestMeshMemStorage_MIX()
{
    MeshMemStorage<MeshStorageType::MIX> mixMeshMemStorage;
    std::vector<GLfloat> datas = {
        //Vertex A
        1.0, 1.0, 1.0,  //pos
        2.0, 2.0, 2.0,  //color
        3.0, 3.0, 3.0,  //normal
        //Vertex B
        4.0, 4.0, 4.0,  //pos
        5.0, 5.0, 5.0,  //color
        6.0, 6.0, 6.0   //normal
    };
    mixMeshMemStorage.SetMeshStorage(reinterpret_cast<unsigned char*>(datas.data()), 2, MeshDataType::POSITION | MeshDataType::COLOR | MeshDataType::NORMAL);
    assert(TestMeshMemStorage(mixMeshMemStorage));
}

void TestMeshMemStorage_SEPARATE()
{
    MeshMemStorage<MeshStorageType::SEPARATE> separateMeshMemStorage;
    std::vector<GLfloat> datas = {
        //Pos
        1.0, 1.0, 1.0,  //A
        4.0, 4.0, 4.0,  //B
        //Color
        2.0, 2.0, 2.0,  //A
        5.0, 5.0, 5.0,  //B
        //Normal
        3.0, 3.0, 3.0,  //A
        6.0, 6.0, 6.0   //B
    };
    separateMeshMemStorage.SetMeshStorage(reinterpret_cast<unsigned char*>(datas.data()), 2, MeshDataType::POSITION | MeshDataType::COLOR | MeshDataType::NORMAL);
    assert(TestMeshMemStorage(separateMeshMemStorage));
}


template<MeshStorageType A, MeshStorageType B>
bool SameCheck(MeshMemStorage<A>& rMeshMemStorageA, MeshMemStorage<B>& rMeshMemStorageB, size_t i)
{
    MeshDataType meshDataTypeA = rMeshMemStorageA.GetMeshDataType();
    MeshDataType meshDataTypeB = rMeshMemStorageB.GetMeshDataType();
    if (meshDataTypeA != meshDataTypeB)
    {
        return false;
    }

    if (meshDataTypeA & MeshDataType::POSITION)
    {
        if (rMeshMemStorageA.GetPosition(i) != rMeshMemStorageB.GetPosition(i))
            return false;
    }

    if (meshDataTypeA & MeshDataType::COLOR)
    {
        if (rMeshMemStorageA.GetColor(i) != rMeshMemStorageB.GetColor(i))
            return false;
    }

    if (meshDataTypeA & MeshDataType::NORMAL)
    {
        if (rMeshMemStorageA.GetNormal(i) != rMeshMemStorageB.GetNormal(i))
            return false;
    }

    return true;
}

template<MeshStorageType A, MeshStorageType B>
bool TestSameMeshMemStorage(MeshMemStorage<A>& rMeshMemStorageA, MeshMemStorage<B>& rMeshMemStorageB)
{
    size_t uVertexCountA = rMeshMemStorageA.GetVertexCount();
    size_t uVertexCountB = rMeshMemStorageB.GetVertexCount();
    if(uVertexCountA != uVertexCountB)
        return false;

    for (size_t i = 0; i < uVertexCountA; ++i)
    {
        if (!SameCheck(rMeshMemStorageA, rMeshMemStorageB, i))
        {
            return false;
        }
    }

    return true;
}

void TestSameMeshMemStorage_MIX_SEPARATE()
{
    MeshMemStorage<MeshStorageType::MIX> mixMeshMemStorage;
    std::vector<GLfloat> mixDatas = {
        //Vertex A
        1.0, 1.0, 1.0,  //pos
        2.0, 2.0, 2.0,  //color
        3.0, 3.0, 3.0,  //normal
        //Vertex B
        4.0, 4.0, 4.0,  //pos
        5.0, 5.0, 5.0,  //color
        6.0, 6.0, 6.0   //normal
    };
    mixMeshMemStorage.SetMeshStorage(reinterpret_cast<unsigned char*>(mixDatas.data()), 2, MeshDataType::POSITION | MeshDataType::COLOR | MeshDataType::NORMAL);

    MeshMemStorage<MeshStorageType::SEPARATE> separateMeshMemStorage;
    std::vector<GLfloat> separateDatas = {
        //Pos
        1.0, 1.0, 1.0,  //A
        4.0, 4.0, 4.0,  //B
        //Color
        2.0, 2.0, 2.0,  //A
        5.0, 5.0, 5.0,  //B
        //Normal
        3.0, 3.0, 3.0,  //A
        6.0, 6.0, 6.0   //B
    };
    separateMeshMemStorage.SetMeshStorage(reinterpret_cast<unsigned char*>(separateDatas.data()), 2, MeshDataType::POSITION | MeshDataType::COLOR | MeshDataType::NORMAL);
    assert(TestSameMeshMemStorage(mixMeshMemStorage, separateMeshMemStorage));
}

void TestMesh()
{
    TestMeshMemStorage_MIX();
    TestMeshMemStorage_SEPARATE();
    TestSameMeshMemStorage_MIX_SEPARATE();
}

RegisterFeaturetest(TestMesh);
ANONYMOUS_SCOPE_END