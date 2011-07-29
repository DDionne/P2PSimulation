/*   This file is part of Graph-C++
 *
 *   Copyright (C) 2006,  Supelec
 *
 *   Author: George Adrian Drumea
 *
 *   Contributor:
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or any later version.
 *   
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *   Lesser General Public License for more details.
 *   
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *   Contact: 
 *		drumea_adr@metz.supelec.fr 
 *		or
 *		adi_drumea@yahoo.com
 * 
 *	 Changes history:
 *	
 *	 Version 1.01, added this file
 * 		- implements the conected components algorithm un undirected graphs
 */

#ifndef __GRAPH_COMP_H
#define __GRAPH_COMP_H

#include "graph-c++.h"

#include <list>

namespace graph  
{

/**
 * @brief Connected components algorithm.
 *
 * This algo works on undirected graphs. It assumes that the node tags are of type int.
 *
 * @param NODE The node type (node labels)
 * @param EDGE The edge type (edge labels)
 */
template<typename NODE, typename EDGE>
class AlgoConnectedComponents
{
	public:
		/** This algorithm supports only undirected graphs with int node tags */
		typedef graph::Graph<NODE, EDGE, false, int> GRAPH;
		/** The graph node iterator */
		typedef typename GRAPH::node_iterator node_iterator;
		/** The graph edge iterator */
		typedef typename GRAPH::edge_iterator edge_iterator;

		/** A list of %node_iterator. This is an actual component */ 
		typedef std::list<node_iterator> list_nodes;
		/** A list of %list_nodes pointers. This is a group of components, the actual result of the algorithm. */
		typedef std::list< list_nodes *> list_list_nodes;
		/** The iterator for %list_list_nodes. */
		typedef typename list_list_nodes::iterator list_list_nodes_iterator;
		
	public:
		/** 
		 * @brief Dummy constructor 
		 */
		AlgoConnectedComponents()
		{
		}

		/** 
		 * @brief Deletes any computed components
		 */
		~AlgoConnectedComponents()
		{
			destroy_components();
		}

	private:
		/** The result of the algorithm. */
		list_list_nodes components;
		/** This keeps the current component being constructed. */
		list_nodes *current_component;

	private:
		/** 
		 * @brief Gets the 'visited' status of the given node. 
		 * 
		 * @param ni The node to be checked.
		 *
		 * @returns True if the node has been visited by the DF traversal.
		 */
		bool get_visited(node_iterator ni)
		{
			return ni->get_tag() != 0;
		}

		/**
		 * @brief Sets the 'visited' status of the given node.
		 *
		 * @param ni The node to be marked as visited.
		 */
		void set_visited(node_iterator ni)
		{
			ni->set_tag(1);
		}

		/** 
		 * @brief Clears the visited status of all the nodes of the graph.
		 */
		void clear_visited(GRAPH & g)
		{
			for (node_iterator ni = g.begin(); ni != g.end(); ni++)
				ni->set_tag(0);
		}

		/**
		 * @brief Deallocates the components result.
		 */
		void destroy_components()
		{
			for (list_list_nodes_iterator i = components.begin(); i != components.end(); i++)
				delete *i;

			components.clear();
		}

	private:
		/**
		 * @brief DF traversal recursive function.
		 */
		void df(node_iterator ni)
		{
			if (get_visited(ni))
				return;

			set_visited(ni);

			if (current_component == NULL)
				current_component = new list_nodes();
			current_component->push_back(ni);

			for (edge_iterator ei = ni->begin(); ei != ni->end(); ei++)
			{
				node_iterator ni2 = ei.to();
				if (ni2 == ni)
					ni2 = ei.from();

				df(ni2);
			}
		}

	public:
		/**
		 * @brief Computes the conected components.
		 *
		 * @param g The %Graph whose components are to be computed.
		 *
		 * @returns A list of components (list of lists of %node_iterators)
		 */
		list_list_nodes & compute(GRAPH & g)
		{
			destroy_components();

			clear_visited(g);

			for (node_iterator ni = g.begin(); ni != g.end(); ni++)
			{
				current_component = NULL;
				df(ni);
				if (current_component == NULL)
					continue;

				components.push_back(current_component);
			}

			return components;
		}
};

} //namespace graph

#endif

