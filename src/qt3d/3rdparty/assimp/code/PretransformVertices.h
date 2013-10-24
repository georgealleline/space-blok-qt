/*
Open Asset Import Library (ASSIMP)
----------------------------------------------------------------------

Copyright (c) 2006-2010, ASSIMP Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the ASSIMP team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the ASSIMP Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------
*/

/** @file PretransformVertices.h
 *  @brief Defines a post processing step to pretransform all
 *    vertices in the scenegraph
 */
#ifndef AI_PRETRANSFORMVERTICES_H_INC
#define AI_PRETRANSFORMVERTICES_H_INC

#include "BaseProcess.h"
#include "../include/aiMesh.h"

class PretransformVerticesTest;
namespace Assimp {

// ---------------------------------------------------------------------------
/** The PretransformVertices pretransforms all vertices in the nodegraph
 *  and removes the whole graph. The output is a list of meshes, one for
 *  each material.
*/
class ASSIMP_API PretransformVertices : public BaseProcess
{
    friend class Importer;
    friend class ::PretransformVerticesTest;

protected:
    /** Constructor to be privately used by Importer */
    PretransformVertices ();

    /** Destructor, private as well */
    ~PretransformVertices ();

public:

    // -------------------------------------------------------------------
    // Check whether step is active
    bool IsActive( unsigned int pFlags) const;

    // -------------------------------------------------------------------
    // Execute step on a given scene
    void Execute( aiScene* pScene);

    // -------------------------------------------------------------------
    // Setup import settings
    void SetupProperties(const Importer* pImp);


    // -------------------------------------------------------------------
    /** @brief Toggle the 'keep hierarchy' option
     *  @param d hm ... difficult to guess what this means, hu!?
     */
    void KeepHierarchy(bool d) {
        configKeepHierarchy = d;
    }

    // -------------------------------------------------------------------
    /** @brief Check whether 'keep hierarchy' is currently enabled.
     *  @return ...
     */
    bool IsHierarchyKept() const {
        return configKeepHierarchy;
    }

private:

    // -------------------------------------------------------------------
    // Count the number of nodes
    unsigned int CountNodes( aiNode* pcNode );

    // -------------------------------------------------------------------
    // Get a bitwise combination identifying the vertex format of a mesh
    unsigned int GetMeshVFormat(aiMesh* pcMesh);

    // -------------------------------------------------------------------
    // Count the number of vertices in the whole scene and a given
    // material index
    void CountVerticesAndFaces( aiScene* pcScene, aiNode* pcNode,
        unsigned int iMat,
        unsigned int iVFormat,
        unsigned int* piFaces,
        unsigned int* piVertices);

    // -------------------------------------------------------------------
    // Collect vertex/face data
    void CollectData( aiScene* pcScene, aiNode* pcNode,
        unsigned int iMat,
        unsigned int iVFormat,
        aiMesh* pcMeshOut,
        unsigned int aiCurrent[2],
        unsigned int* num_refs);

    // -------------------------------------------------------------------
    // Get a list of all vertex formats that occur for a given material
    // The output list contains duplicate elements
    void GetVFormatList( aiScene* pcScene, unsigned int iMat,
        std::list<unsigned int>& aiOut);

    // -------------------------------------------------------------------
    // Compute the absolute transformation matrices of each node
    void ComputeAbsoluteTransform( aiNode* pcNode );

    // -------------------------------------------------------------------
    // Simple routine to build meshes in worldspace, no further optimization
    void BuildWCSMeshes(std::vector<aiMesh*>& out, aiMesh** in,
        unsigned int numIn, aiNode* node);

    // -------------------------------------------------------------------
    // Apply the node transformation to a mesh
    void ApplyTransform(aiMesh* mesh, const aiMatrix4x4& mat);

    // -------------------------------------------------------------------
    // Reset transformation matrices to identity
    void MakeIdentityTransform(aiNode* nd);

    // -------------------------------------------------------------------
    // Build reference counters for all meshes
    void BuildMeshRefCountArray(aiNode* nd, unsigned int * refs);



    //! Configuration option: keep scene hierarchy as long as possible
    bool configKeepHierarchy, configNormalize;

};

} // end of namespace Assimp

#endif // !!AI_GENFACENORMALPROCESS_H_INC