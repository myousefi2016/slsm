/*
  Copyright (c) 2015-2017 Lester Hedges <lester.hedges+slsm@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _MESH_H
#define _MESH_H

#include <vector>

#include "Common.h"

/*! \file Mesh.h
    \brief A class for the level-set domain fixed-grid mesh.
 */

namespace slsm
{
    // ASSOCIATED DATA TYPES

    /* The following are implementations of strongly typed enums. This allows
       for better scoping and name sharing between enumerations. This can also
       be achieved in C++11 using "enum classes", although it's not currently
       supported by all compilers.
     */

    //! Whether a node lies inside, outside, or on the boundary.
    namespace NodeStatus
    {
        /* Left bit shift enumerated types to allow the creation of sets and
           simple bit masking operations. For example, to test whether a node
           is neither inside or outside...

             if (node.status ^ (NodeStatus::INSIDE|NodeStatus::OUTSIDE))
         */
        enum NodeStatus
        {
            NONE            = 0,                    //!< No status.
            INSIDE          = (1 << 0),             //!< Node lies inside the boundary.
            OUTSIDE         = (1 << 1),             //!< Node lies outside the boundary.
            BOUNDARY        = (1 << 2),             //!< Node lies on the boundary.
            CUT             = (INSIDE|OUTSIDE),     //!< Node pair is cut by the boundary.
        };
    }

    //! Whether a element lies completely inside or outside the structure.
    //! If not, whether the element centre lies inside or outside.
    namespace ElementStatus
    {
        enum ElementStatus
        {
            NONE            = 0,                    //!< No status.
            INSIDE          = (1 << 0),             //!< Element is entirely inside the structure.
            OUTSIDE         = (1 << 1),             //!< Element is entirely outside the structure.
            CENTRE_INSIDE   = (1 << 2),             //!< Element centre lies inside the structure.
            CENTRE_OUTSIDE  = (1 << 3),             //!< Element centre lies outside the structure.
        };
    }

    //! Structure containing attributes for an individual grid element.
    struct Element
    {
        //! Constructor.
        Element();

        Coord coord;                                //!< Element coordinate (centre).
        double area;                                //!< Material area fraction.
        std::vector<unsigned int> nodes;            //!< Indices for nodes of the element.
        std::vector<unsigned int> boundarySegments; //!< Indices for boundary segments associated with the element.
        unsigned int nBoundarySegments;             //!< The number of boundary segments associated with the element.
        ElementStatus::ElementStatus status;        //!< Whether the element (or its centre) lies inside or outside the structure.
    };

    //! Structure containing attributes for an individual grid node.
    struct Node
    {
        //! Constructor.
        Node();

        Coord coord;                                //!< Node coordinate.
        std::vector<unsigned int> neighbours;       //!< Indices of nearest neighbour nodes.
        std::vector<unsigned int> elements;         //!< Indices of elements the node is connected to.
        unsigned int nElements;                     //!< Number of elements that the node is connected to.
        std::vector<unsigned int> boundaryPoints;   //!< Indices of boundary points associated with the node.
        unsigned int nBoundaryPoints;               //!< The number of boundary points associated with the node.
        bool isActive;                              //!< Whether the node is active (part of narrow band, and not fixed).
        bool isDomain;                              //!< Whether the node lies on the domain boundary.
        bool isMasked;                              //!< Whether the node lies in a masked region.
        bool isMine;                                //!< Whether the node lies on the edge of the narrow band.
        NodeStatus::NodeStatus status;              //!< Whether node is outside, inside, or on the boundary.
    };

    // MAIN CLASS

    /*! \brief A class for the level-set domain fixed-grid mesh.

        Stores connectivty information between grid elements and nodes.
        The grid is assumed to be two-dimensional and is comprised of
        square elements of unit side.

        Elements are comprised of four nodes, labelled in anticlockwise order
        from the bottom left, i.e. bottom left, bottom right, top right, top left.

        Each node has four nearest neighbours ordered as left, right, down, up.
        Diagonal neighbours can be accessed by looking at neighbours of neighbours,
        e.g. for the lower left diagonal of node i

        node = nodes[nodes[i].neighbours[0]].neighbours[2];

        Note that diagonal nodes can be accessed in multiple ways, e.g.
        for the lower left diagonal of node i we could go left then down,
        or down then left (shortest paths).

        The mesh is non-periodic. Neighbours that are outside of the domain are
        given the value nNodes, i.e. one past the end of the node array, which
        runs from 0 to nNodes - 1.

        Note that this mesh is store information related to the nodes and
        elements of the level-set domain and is not related to the mesh used
        in finite element calculations (which may be a different geometry or
        resolution).
     */
    class Mesh
    {
    public:
        //! Constructor.
        /*! \param width_
                The width of the mesh.

            \param height_
                The height of the mesh.
         */
        Mesh(unsigned int, unsigned int);

        //! For a given x-y coordinate, find the index of the closest node.
        /*! \param point
                The x-y coordinates of the point.

            \return
                The index of the closest node.
         */
        unsigned int getClosestNode(const Coord&) const;

        //! For a given x-y coordinate, find the index of the closest node.
        /*! \param x
                The x position of the point.

            \param y
                The y position of the point.

            \return
                The index of the closest node.
         */
        unsigned int getClosestNode(double, double) const;

        //! For a given x-y coordinate, find the element that contains the point.
        /*! \param point
                The x-y position of the point.

            \return
                The index of the containing element.
         */
        unsigned int getElement(const Coord&) const;

        //! For a given x-y coordinate, find the element that contains the point.
        /*! \param x
                The x position of the point.

            \param y
                The y position of the point.

            \return
                The index of the containing element.
         */
        unsigned int getElement(double, double) const;

        std::vector<Element> elements;  //!< Fixed-grid elements (cells).
        std::vector<Node> nodes;        //!< Fixed-grid nodes.

        const unsigned int width;       //!< The grid width (number of elements in x).
        const unsigned int height;      //!< The grid height (number of elements in y).
        const unsigned int nElements;   //!< The total number of grid elements.
        const unsigned int nNodes;      //!< The total number of nodes.

        /// Mapping between (x, y) coordinates and one dimensional nodes indices.
        std::vector<std::vector<unsigned int> > xyToIndex;

    private:
        //! Initialise mesh nodes.
        void initialiseNodes();

        //! Initialise mesh elements.
        void initialiseElements();

        //! Compute the nearest neighbours of a node.
        /*! \param node
                The node index.

            \param x
                The x coordinate of the node.

            \param y
                The y coordinate of the node.
         */
        void initialiseNeighbours(unsigned int, unsigned int, unsigned int);
    };
}

#endif  /* _MESH_H */
