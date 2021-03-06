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

#include "slsm.h"

int testMeshSize()
{
    // Initialise a 3x3 mesh.
    slsm::Mesh mesh(3, 3);

    // Set error number.
    errno = 0;

    slsm_check(mesh.width == 3, "Mesh width is incorrect!");
    slsm_check(mesh.height == 3, "Mesh height is incorrect!");
    slsm_check(mesh.nElements == 9, "Number of elements is incorrect!");
    slsm_check(mesh.nNodes == 16, "Number of nodes is incorrect!");

    return 0;

error:
    return 1;
}

int testNodeCoordinates()
{
    // Initialise a 3x3 mesh.
    slsm::Mesh mesh(3, 3);

    // Set error number.
    errno = 0;

    // Check coordinates of 0th node (bottom left).
    slsm_check(mesh.nodes[0].coord.x == 0, "x coordinate of node 0 is incorrect!");
    slsm_check(mesh.nodes[0].coord.y == 0, "y coordinate of node 0 is incorrect!");

    // Check coordinates of 5th node (bulk).
    slsm_check(mesh.nodes[5].coord.x == 1, "x coordinate of node 5 is incorrect!");
    slsm_check(mesh.nodes[5].coord.y == 1, "y coordinate of node 5 is incorrect!");

    // Check coordinates of 15th node (top right).
    slsm_check(mesh.nodes[15].coord.x == 3, "x coordinate of node 5 is incorrect!");
    slsm_check(mesh.nodes[15].coord.y == 3, "y coordinate of node 5 is incorrect!");

    return 0;

error:
    return 1;
}

int testNodeConnectivity()
{
    // Initialise a 3x3 mesh.
    slsm::Mesh mesh(3, 3);

    // Initialise a 3x4 mesh.
    slsm::Mesh nsMesh(3, 4);

    // Set error number.
    errno = 0;

    /********** Square Mesh Test **********/

    // Check nearest neighbours of 0th node (bottom left).
    slsm_check(mesh.nodes[0].neighbours[0] == mesh.nNodes, "Square mesh: Neighbour 0 of node 0 is incorrect!");
    slsm_check(mesh.nodes[0].neighbours[1] == 1, "Square mesh: Neighbour 1 of node 0 is incorrect!");
    slsm_check(mesh.nodes[0].neighbours[2] == mesh.nNodes, "Square mesh: Neighbour 2 of node 0 is incorrect!");
    slsm_check(mesh.nodes[0].neighbours[3] == 4, "Square mesh: Neighbour 3 of node 0 is incorrect!");

    // Check nearest neighbours of 5th node (bulk).
    slsm_check(mesh.nodes[5].neighbours[0] == 4, "Square mesh: Neighbour 0 of node 5 is incorrect!");
    slsm_check(mesh.nodes[5].neighbours[1] == 6, "Square mesh: Neighbour 1 of node 5 is incorrect!");
    slsm_check(mesh.nodes[5].neighbours[2] == 1, "Square mesh: Neighbour 2 of node 5 is incorrect!");
    slsm_check(mesh.nodes[5].neighbours[3] == 9, "Square mesh: Neighbour 3 of node 5 is incorrect!");

    // Check nearest neighbours of 15th node (top right).
    slsm_check(mesh.nodes[15].neighbours[0] == 14, "Square mesh: Neighbour 0 of node 15 is incorrect!");
    slsm_check(mesh.nodes[15].neighbours[1] == mesh.nNodes, "Square mesh: Neighbour 1 of node 15 is incorrect!");
    slsm_check(mesh.nodes[15].neighbours[2] == 11, "Square mesh: Neighbour 2 of node 15 is incorrect!");
    slsm_check(mesh.nodes[15].neighbours[3] == mesh.nNodes, "Square mesh: Neighbour 3 of node 15 is incorrect!");

    /*********** Non-square Mesh Test ***********/

    // Check nearest neighbours of 0th node (bottom left).
    slsm_check(nsMesh.nodes[0].neighbours[0] == nsMesh.nNodes, "Non-square mesh: Neighbour 0 of node 0 is incorrect!");
    slsm_check(nsMesh.nodes[0].neighbours[1] == 1, "Non-square mesh: Neighbour 1 of node 0 is incorrect!");
    slsm_check(nsMesh.nodes[0].neighbours[2] == nsMesh.nNodes, "Non-square mesh: Neighbour 2 of node 0 is incorrect!");
    slsm_check(nsMesh.nodes[0].neighbours[3] == 4, "Non-square mesh: Neighbour 3 of node 0 is incorrect!");

    // Check nearest neighbours of 5th node (bulk).
    slsm_check(nsMesh.nodes[5].neighbours[0] == 4, "Non-square mesh: Neighbour 0 of node 5 is incorrect!");
    slsm_check(nsMesh.nodes[5].neighbours[1] == 6, "Non-square mesh: Neighbour 1 of node 5 is incorrect!");
    slsm_check(nsMesh.nodes[5].neighbours[2] == 1, "Non-square mesh: Neighbour 2 of node 5 is incorrect!");
    slsm_check(nsMesh.nodes[5].neighbours[3] == 9, "Non-square mesh: Neighbour 3 of node 5 is incorrect!");

    // Check nearest neighbours of 19th node (top right).
    slsm_check(nsMesh.nodes[19].neighbours[0] == 18, "Non-square mesh: Neighbour 0 of node 19 is incorrect!");
    slsm_check(nsMesh.nodes[19].neighbours[1] == nsMesh.nNodes, "Non-square mesh: Neighbour 1 of node 19 is incorrect!");
    slsm_check(nsMesh.nodes[19].neighbours[2] == 15, "Non-square mesh: Neighbour 2 of node 19 is incorrect!");
    slsm_check(nsMesh.nodes[19].neighbours[3] == nsMesh.nNodes, "Non-square mesh: Neighbour 3 of node 19 is incorrect!");

    return 0;

error:
    return 1;
}

int testReverseNodeConnectivity()
{
    // Initialise a 3x3 mesh.
    slsm::Mesh mesh(3, 3);

    // Set error number.
    errno = 0;

    // Make sure opposite neighbour of each neighbour map back to node, e.g. the
    // right-hand neighbour of a node's left-hand neighbour should point to the node.
    slsm_check(mesh.nodes[mesh.nodes[5].neighbours[0]].neighbours[1] == 5,
        "Reverse connectivity: Mapping between neighours 0 and 1 incorrect!");
    slsm_check(mesh.nodes[mesh.nodes[5].neighbours[1]].neighbours[0] == 5,
        "Reverse connectivity: Mapping between neighours 1 and 0 incorrect!");
    slsm_check(mesh.nodes[mesh.nodes[5].neighbours[2]].neighbours[3] == 5,
        "Reverse connectivity: Mapping between neighours 2 and 3 incorrect!");
    slsm_check(mesh.nodes[mesh.nodes[5].neighbours[3]].neighbours[2] == 5,
        "Reverse connectivity: Mapping between neighours 3 and 2 incorrect!");

    return 0;

error:
    return 1;
}

int testElementNodeConnectivity()
{
    // Initialise a 3x3 mesh.
    slsm::Mesh mesh(3, 3);

    // Set error number.
    errno = 0;

    // Check the four nodes of the 0th element.
    slsm_check(mesh.elements[0].nodes[0] == 0, "Node 0 of element 0 is incorrect!");
    slsm_check(mesh.elements[0].nodes[1] == 1, "Node 1 of element 0 is incorrect!");
    slsm_check(mesh.elements[0].nodes[2] == 5, "Node 2 of element 0 is incorrect!");
    slsm_check(mesh.elements[0].nodes[3] == 4, "Node 3 of element 0 is incorrect!");

    return 0;

error:
    return 1;
}

int testNodeElementConnectivity()
{
    // Initialise a 3x3 mesh.
    slsm::Mesh mesh(3, 3);

    // Set error number.
    errno = 0;

    // Check the elements connected to the 0th node (one element connected).
    slsm_check(mesh.nodes[0].nElements == 1, "Number of elements connected to node 0 is incorrect!");
    slsm_check(mesh.nodes[0].elements[0] == 0, "Index of element 0 connected to node 0 is incorrect!");

    // Check the elements connected to the 7th node (two elements connected).
    slsm_check(mesh.nodes[7].nElements == 2, "Number of elements connected to node 0 is incorrect!");
    slsm_check(mesh.nodes[7].elements[0] == 2, "Index of element 0 connected to node 7 is incorrect!");
    slsm_check(mesh.nodes[7].elements[1] == 5, "Index of element 0 connected to node 7 is incorrect!");

    // Check the elements connected to the 10th node (four elements connected).
    slsm_check(mesh.nodes[10].nElements == 4, "Number of elements connected to node 0 is incorrect!");
    slsm_check(mesh.nodes[10].elements[0] == 4, "Index of element 0 connected to node 7 is incorrect!");
    slsm_check(mesh.nodes[10].elements[1] == 5, "Index of element 1 connected to node 7 is incorrect!");
    slsm_check(mesh.nodes[10].elements[2] == 7, "Index of element 2 connected to node 7 is incorrect!");
    slsm_check(mesh.nodes[10].elements[3] == 8, "Index of element 3 connected to node 7 is incorrect!");

    return 0;

error:
    return 1;
}

int testCoordinateMapping()
{
    // Initialise a 2x2 mesh.
    slsm::Mesh mesh(2, 2);

    // Set error number.
    errno = 0;

    // Check that point (0.5, 0.5) lies in bottom left (zeroth) element.
    slsm_check(mesh.getElement(0.5, 0.5) == 0, "Point lies in incorrect element!");

    // Check that point (1.5, 0.5) lies in bottom right (first) element.
    slsm_check(mesh.getElement(1.5, 0.5) == 1, "Point lies in incorrect element!");

    // Check that point (0.5, 1.5) lies in upper left (second) element.
    slsm_check(mesh.getElement(0.5, 1.5) == 2, "Point lies in incorrect element!");

    // Check that point (1.5, 1.5) lies in upper right (third) element.
    slsm_check(mesh.getElement(1.5, 1.5) == 3, "Point lies in incorrect element!");

    // Check that point (0.2, 0.2) lies closest to the bottom left (zeroth) node.
    slsm_check(mesh.getClosestNode(0.2, 0.2) == 0, "Point lies closest to incorrect node!");

    // Check that point (0.6, 0.2) lies closest to the first node.
    slsm_check(mesh.getClosestNode(0.6, 0.2) == 1, "Point lies closest to incorrect node!");

    // Check that point (0.2, 0.6) lies closest to the width + 1 node.
    slsm_check(mesh.getClosestNode(0.2, 0.6) == (mesh.width + 1), "Point lies closest to incorrect node!");

    // Check that point (0.6, 0.6) lies closest to the width + 2 node.
    slsm_check(mesh.getClosestNode(0.6, 0.6) == (mesh.width + 2), "Point lies closest to incorrect node!");

    return 0;

error:
    return 1;
}

int all_tests()
{
    mu_suite_start();

    mu_run_test(testMeshSize);
    mu_run_test(testNodeCoordinates);
    mu_run_test(testNodeConnectivity);
    mu_run_test(testReverseNodeConnectivity);
    mu_run_test(testElementNodeConnectivity);
    mu_run_test(testNodeElementConnectivity);
    mu_run_test(testCoordinateMapping);

    return 0;
}

RUN_TESTS(all_tests);
