#pragma once
#include "math.hpp"
#include "MeshData.h"
#include <vector>
#include <memory>
/**
 * @class Node
 * @brief Class representing a node in a scene graph.
 *
 * The Node class is designed to be part of a scene graph, containing transformation
 * information as well as a list of child nodes. It's capable of constructing a
 * hierarchical representation of a 3D scene, where each node can have many children
 * but only one parent.
 */
class Node : public std::enable_shared_from_this<Node>
{
public:
    std::vector<std::shared_ptr<Node>> children;  ///< This node's children
    std::weak_ptr<Node> parent;                   ///< This node's parent

    glm::mat4 transformation;  ///< Stores the transformation matrix for this node

    /**
     * @brief Constructor for Node.
     *
     * Creates a new Node with a given transformation matrix. If no matrix is
     * provided, defaults to the identity matrix.
     *
     * @param transformation Initial transformation matrix for this node.
     */
    Node(const glm::mat4& transformation = glm::mat4(1.0f))
        : transformation(transformation)
    {}

    /**
    * @brief Adds a child to this node.
    *
    * The child node will be added to the list of children for this node,
    * and its parent will be set to this node.
    *
    * @param child The child node to be added.
    */
    void addChild(std::shared_ptr<Node> child)
    {
        child->parent = shared_from_this();
        children.push_back(child);
    }

};
