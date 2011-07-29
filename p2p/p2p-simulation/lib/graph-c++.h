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
 *	 Version 1.01, 10 may 2006
 *		- added TAG template argument to keep generic node tags
 *		- added const_node_iterator and const_edge_iterator to be able to
 *	      work on const Graphs
 *		- fixed double delete bug when removing node to self edges
 *	 Version 1.00 - initial release
 */

/**
 * @example graph-c++-demo.cc
 */

#ifndef __GRAPH_H
#define __GRAPH_H

#include <stdio.h>
#include <map>

namespace graph
{

/**
 * @brief Represents a general graph and allows manipulation of nodes and edges.
 *
 * The graph may be directed or undirected. The nodes labels may not be unique,
 * e.g. you may have more than one node with the same label. There may be more
 * than one edge between the same two nodes. There may also be edges from one
 * node to itself.
 *
 * @param NODE A type that will be used to label nodes.
 * @param EDGE A type that will be used to label edges.
 * @param DIRECTED Boolean parameter, if set to false the graph is directed, else
 *  	it is undirected. By default, DIRECTED = true
 * @param TAG A type that will be attached to each node and can be modified.
 */
template<typename NODE, typename EDGE, bool DIRECTED = true, typename TAG = int>
class Graph
{
    private:
	/** Shortcut for the instantiated Graph type */
	typedef Graph<NODE, EDGE, DIRECTED, TAG> _SELF_TYPE;

    public:
	/** Class used to manipulate a node's links */
	class node;
	/** Iterator used to browse and modify the nodes of the graph. */
	class node_iterator;
	/** Const iterator used to browse the nodes of a graph. */
	class const_node_iterator;
	/** Common base class for %node_iterator and %const_node_iterator. */
	class node_iterator_base;
	/** Iterator used to browse and modify the edges of a node. */
	class edge_iterator;
	/** Const iterator used to browse the edges of a node. */
	class const_edge_iterator;
	/** Common base class for %edge_iterator and %const_edge_iterator. */
	class edge_iterator_base;

    private:
	class _Edge;

    private:
	/* Aliases for the collections and iterators types. */
	typedef typename std::multimap<NODE, node *> _NODES_TYPE;
	typedef typename _NODES_TYPE::iterator _NODES_IT_TYPE;
	typedef typename std::multimap<node *, _Edge *> _EDGES_TYPE;
	typedef typename _EDGES_TYPE::iterator _EDGES_IT_TYPE;

    private:
	/**
	 * @brief Internal class. Used to store an edge.
	 *
	 * This will not be used directly from outside the %Graph class.
	 * The motivation of using a separate structure to keep the edge label
	 * end endpoints is because in the case of undirected graphs, we
	 * have two references to the same edge. Also, by keeping a collection
	 * with edges 'going into' a node, we double the references to the edges.
	 */
	class _Edge
	{
	    /* Friendship with classes that need unconstrained access to private
		 * members.
		 */
	    friend class node;
	    friend class edge_iterator_base;
		friend class edge_iterator;
		friend class const_edge_iterator;
		friend class Graph<NODE, EDGE, DIRECTED, TAG>;

	    private:
		/**
		 * The 'source' node of the edge (in case of directed graphs).
		 * For undirected graphs, it's just one of the two extremities.
		 */
		node *n1;
		/**
		 * The 'destination' node of the edge (in case of directed graphs).
		 * For undirected graphs, it's just one of the two extremities.
		 */
		node *n2;

	    private:
		/** The actual edge label is kept here. Only one copy for each edge. */
		EDGE edge;

	    private:
		/** The constructor is called only by friend classes */
		_Edge(node *n1, node *n2, const EDGE & edge)
		{
		    this->n1 = n1;
		    this->n2 = n2;
		    this->edge = edge;
		}
	};

    public:
	/**
	 * @brief Used to manipulate a node externally (modify it's edges) and
	 * to store a node internally.
	 *
	 * A node cannot be instantiated from the outside, but its public methods
	 * may be accessed via the -> operator of the %node_iterator.
	 *
	 * This stores a collection of edges leaving the node and another collection
	 * of edges entering the node. In the case of undirected graphs, the two
	 * collections are the same.
	 *
	 * The node label is not kept in the node because it's stored
	 * as a key in the nodes multimap. For quick retrieval of the
	 * node label when a node * exists, the contained node_iterator should
	 * be used, with the * operator.
	 *
	 * The fact that a node_iterator is contained in this class is due
	 * to the property of the multimap that the node_iterator does not change
	 * when its contents are being modified. This does not mean that it is safe
	 * to iterate a multimap and to remove nodes at the same time,
	 * as the relative order of the nodes may change.
	 */
	class node
	{
	    /* Friendship with classes that need unconstrained access to private
		 * members.
		 */
	    friend class node_iterator_base;
		friend class node_iterator;
		friend class const_node_iterator;
	    friend class edge_iterator;
		friend class edge_iterator_base;
		friend class const_edge_iterator;
	    friend class Graph<NODE, EDGE, DIRECTED, TAG>;

	    private:
		/** Multimap of edges leaving the node. The key is a node *,
		 * because it uniquely identifies a node. */
		mutable _EDGES_TYPE edges;
		/** Multimap of edges entering the node. The key is a node *,
		 * because it uniquely identifies a node. */
		mutable _EDGES_TYPE rev_edges;

	    private:
		/**
		 * @brief For performance reasons, a %node_iterator pointing to this node
		 * is cached.
		 *
		 * Node iterators may be cached because:
		 * "Multimap has the important property that inserting an element into
		 * the multimap does not invalidate iterators that point to existing elements.
		 * Erasing an element from the multimap also does not invalidate any iterators
		 * except, of course, for iterators that actually point to the element
		 * that is being erased"
		 */
		node_iterator ni;

	    private:
		/** Pointer to the containing graph. */
		_SELF_TYPE *g;

	    private:
		/** A generic tag to be used in various algos */
		TAG tag;

		private:
		/**
		 * @brief Private constructor called only from Graph class.
		 *
		 * @param _ni A %node_iterator pointing to this node. The multimap
		 * 	specifications guarantees us that it will not change during
		 *	modifications made to the map.
		 * @param g A pointer to the containing %Graph.
		 */
		inline node(node_iterator _ni, _SELF_TYPE *g) : ni(_ni)
		{
		    this->g = g;
		}

	    private:
		/**
		 * @brief Node destructor.
		 *
		 * The destructor removes all references to edges linking this node
		 * to other nodes of the graph. It also destroys these edges and frees
		 * memory
		 */
		inline ~node()
		{
		    erase_all_edges();
		}

	    private:
		/**
		 * @brief Make the copy constructor private, so we don't accidentally copy
		 * nodes around */
		node(const node & n)
		{
			throw "Graph::node should not be copied around.";
		}

	    public:
		/**
		 * @brief Gets the beginning of the collection of edges leaving the node.
		 *
		 * @returns An %edge_iterator pointing to the beginning of the collection.
		 */
		inline edge_iterator begin()
		{
		    return edge_iterator(edges.begin());
		}

		/**
		 * @brief Gets the beginning of the collection of edges leaving the node.
		 *
		 * @returns A %const_edge_iterator pointing to the beginning of the collection.
		 */
		inline const_edge_iterator begin() const
		{
			return const_edge_iterator(edges.begin());
		}

		/**
		 * @brief Gets the end of the collection of edges leaving the node.
		 *
		 * @returns An %edge_iterator pointing to the end of the collection.
		 */
		inline edge_iterator end()
		{
		    return edge_iterator(edges.end());
		}

		/**
		 * @brief Gets the end of the collection of edges leaving the node.
		 *
		 * @returns An %const_edge_iterator pointing to the end of the collection.
		 */
		inline const_edge_iterator end() const
		{
			return const_edge_iterator(edges.end());
		}

	    public:
		/**
		 * @brief Gets the beginning of the collection of edges entering the node.
		 *
		 * @returns An %edge_iterator pointing to the beginning of the collection.
		 */
		inline edge_iterator begin_rev()
		{
		    return edge_iterator(rev_edges.begin());
		}

		/**
		 * @brief Gets the beginning of the collection of edges entering the node.
		 *
		 * @returns A %const_edge_iterator pointing to the beginning of the collection.
		 */
		inline const_edge_iterator begin_rev() const
		{
			return const_edge_iterator(rev_edges.begin());
		}

		/**
		 * @brief Gets the end of the collection of edges leaving the node.
		 *
		 * @returns An %edge_iterator pointing to the end of the collection.
		 */
		inline edge_iterator end_rev()
		{
		    return edge_iterator(rev_edges.end());
		}

		/**
		 * @brief Gets the end of the collection of edges leaving the node.
		 *
		 * @returns An %const_edge_iterator pointing to the end of the collection.
		 */
		inline const_edge_iterator end_rev() const
		{
			return const_edge_iterator(rev_edges.end());
		}

	    private:
		/**
		 * @brief INTERNAL. Erase a specified edge from the given collection.
		 *
		 * @param e The edge to be deleted.
		 * @param n A %node * representing the other endpoint of the edge. Used for search hinting.
		 *
		 * This method is only used to remove an edge reference from the given collection.
		 * It does not deallocate the _Edge pointer.
		 */
		void erase_edge_no_delete(_Edge *e, node *n, _EDGES_TYPE & col)
		{
		    for (edge_iterator ei = col.lower_bound(n); ei != col.upper_bound(n); ei++)
			if (ei.it->second == e)
			{
			    col.erase(ei.it);
			    return;
			}
		}

		/**
		 * @brief INTERNAL. Erase a specified edge from both collections.
		 *
		 * @param e The edge to be deleted.
		 * @param n A %node * representing the other endpoint of the edge. Used for search hinting.
		 *
		 * This method is only used to remove an edge reference from both edges and rev_edges collections.
		 * It does not deallocate the _Edge pointer.
		 */
		inline void erase_edge_no_delete(_Edge *e, node *n)
		{
		    erase_edge_no_delete(e, n, edges);
		    erase_edge_no_delete(e, n, rev_edges);
		}

		/**
		 * @brief INTERNAL. Find the iterator pointing to a specified edge in the given collection.
		 *
		 * @param e The edge to be found.
		 * @param n A %node * representing the other endpoint of the edge. Used for search hinting.
		 * @param col The multimap we're searching in.
		 */
		_EDGES_IT_TYPE find(_Edge *e, node *n, _EDGES_TYPE & col)
		{
		    for (_EDGES_IT_TYPE it = col.lower_bound(n); it != col.upper_bound(n); it++)
			if (it->second == e)
			    return it;

		    return col.end();
		}

		/**
		 * @brief INTERNAL. Removes an edge from the %Graph and deallocates it.
		 *
		 * @param e Pointer to the edge to be removed.
		 * @param n The other end of the edge, used for search hinting.
		 *
		 * This method removes the specified edge between this node and node n.
		 * If the graph is directed, it does not remove edges going from node n to
		 * this node. If the graph is undirected, it removes the simetric edge
		 * from node n to this node.
		 */
		void erase_edge(_Edge *e, node *n)
		{
		    /* remove e from n's collections */
		    n->erase_edge_no_delete(e, this);
		    /* remove e from this' collections */
		    this->erase_edge_no_delete(e, n);

		    /* delete the actual edge structure */
		    delete e;
    	}

		/**
		 * @brief INTERNAL. Removes and deallocates all edges between two nodes.
		 *
		 * @param n The other end of the edge.
		 *
		 * This method removes all edges between this node and node n. If the graph
		 * is directed, it does not remove edges going from node n to this node. If
		 * the graph is undirected, all edges between this and n are removed.
		 */
		void erase_all_edges(node *n)
		{
		    while (1)
		    {
				edge_iterator ei = edges.find(n);
				if (ei.it == edges.end())
			    	return;

				erase_edge(ei.it->second, n);
		    }
		}

	    public:
		/**
		* @brief Erases all edges linking this node to the given node.
		*
		* For directed graphs, it removes only edges going from this node to the
		* node received as argument. It does not remove edges going from ni to this.
		*
		* @param ni A %node_iterator pointing to the node whose links are to be removed.
		*/
		inline void erase_all_edges(node_iterator_base ni)
		{
		    erase_all_edges(ni.it->second);
		}

	    private:
		/**
		 * @brief INTERNAL. Erases all the edges leaving or entering this node.
		 *
		 * Also handles memory deallocation for them.
		 */
		void erase_all_edges()
		{
		    /* for each edge... */
		    for (edge_iterator ei = begin(); ei != end(); ei++)
		    {
				/* get _Edge pointer */
				_Edge *pe = ei.it->second;

				/* get the other end of the edge */
				node *n2 = pe->n1 == this ? pe->n2 : pe->n1;

				/* remove references to this in n2's collections */
				if (n2 != this)
					n2->erase_edge_no_delete(pe, this);

				/* deallocate edge */
				delete pe;
		    }

		    /* clear collections */
		    edges.clear();
		    rev_edges.clear();
		}

	    private:
		/**
		 * @brief INTERNAL. Add directed link.
		 *
		 * @param n2 A %node * to link this node with.
		 * @param e Edge created previously, will be referenced in the edges/rev_edges multimaps.
		 *
		 * @returns An %edge_iterator to the newly inserted edge. The iterator is for the out edges collection.
		 */
		edge_iterator insert_edge(node *n2, _Edge *e)
		{
		    /* add link to this node */
		    edge_iterator ret = edges.insert(std::make_pair(n2, e));

		    /* add link to the other end's reverse links */
		    n2->rev_edges.insert(std::make_pair(this, e));

			return ret;
		}

	    public:
		/**
		 * @brief Inserts an edge linking this node to the specified node
		 * 	and with a given label.
		 *
		 * @param ni The iterator to the other node.
		 * @param edge The edge label.
		 *
		 * @returns An %edge_iterator pointing to the newly inserted edge
		 */
		edge_iterator insert_edge(node_iterator ni, const EDGE & edge)
		{
		    /* get node pointer */
		    node *n2 = ni.it->second;

		    /* create new edge */
		    _Edge *e = new _Edge(this, n2, edge);

		    /* insert new edge into this' collections */
		    edge_iterator ret = this->insert_edge(n2, e);

		    /* If not DIRECTED and it is not link to self, add the simetric edge. */
		    if (!DIRECTED && this != n2)
				n2->insert_edge(this, e);

			return ret;
		}

	    public:
		/**
		 * @brief Erases edge given by the iterator
		 *
		 * @param ei Iterator to an edge to be deleted.
		 */
		void erase_edge(edge_iterator ei)
		{
		    _Edge *pe = ei.it->second;
		    node *n2 = pe->n1 == this ? pe->n2 : pe->n1;

		    this->erase_edge(pe, n2);
		}

	    public:
		/**
		 * @brief Finds one edge (if exists) to the given node.
		 *
		 * In the case of directed graphs, this method returns an edge going
		 * from 'this' node to the node specified by ni. In the case of
		 * undirected graphs, the method returns the same edge regardless if
		 * it is called from 'this' or from 'ni' node.
		 *
		 * @param ni A %node_iterator pointing to a node whose edge is to be found.
		 *
		 * @returns An iterator to an edge linking this node and the node
		 * specified by ni. If more than one edge exist, this method
		 * will return one of them.
		 */
		inline edge_iterator find_edge(node_iterator_base ni)
		{
		    return edge_iterator(edges.find(ni.it->second));
		}

		/**
		 * @brief Finds one edge (if exists) to the given node. Const version.
		 *
		 * @returns A %const_edge_iterator to the found node, or this->end().
		 */
		inline const_edge_iterator find_edge(node_iterator_base ni) const
		{
			return const_edge_iterator(edges.find(ni.it->second));
		}

	    public:
		/**
		 * @brief Finds one edge (if exists) from the given node.
		 *
		 * In the case of undirected graphs, this function is equivalent to %find_edge.
		 *
		 * @returns An %edge_iterator pointing to the found edge.
		 */
		inline edge_iterator find_edge_rev(node_iterator_base ni)
		{
		    return edge_iterator(rev_edges.find(ni.it->second));
		}

		/**
		 * @brief Finds one edge (if exists) from the given node. Const version.
		 *
		 * @returns A %const_edge_iterator to the found node, or this->end_rev().
		 */
		const_edge_iterator find_edge_rev(node_iterator_base ni) const
		{
			return const_edge_iterator(rev_edges.find(ni.it->second));
		}

		public:
		/**
		 * @brief Gets the size of the out edges collection
		 *
		 * In the case of undirected graphs, the size is equal to that of in edges.
		 *
		 * @returns The number of edges going out of the node.
		 */
		inline int count_edges() const
		{
			return (int)edges.size();
		}

		/**
		 * @brief Gets the size of the in edges collection
		 *
		 * In the case of undirected graphs, the size is equal to that of out edges.
		 *
		 * @returns The number of edges going into the node.
		 */
		inline int count_edges_rev() const
		{
			return (int)rev_edges.size();
		}

		public:
		/**
		 * @brief Sets the user-defined tag for the node
		 */
		inline void set_tag(const TAG & tag)
		{
			this->tag = tag;
		}

		/**
		 * @brief Gets the user-defined tag for the node
		 */
		inline TAG get_tag() const
		{
			return this->tag;
		}
	};

	/**
	 * @brief The edge iterator base implementation.
	 *
	 * The edge iterator is used to browse and modify the edges of a node.
	 * This class contains the common operations of its descendents:
	 * %edge_iterator and %const_edge_iterator.
	 */
	class edge_iterator_base
	{
	    /* Friendship with classes that need unconstrained access to private members. */
	    friend class Graph<NODE, EDGE, DIRECTED, TAG>;
	    friend class node;

	    friend class _Edge;

	    protected:
		/** The multimap iterator. Always a iterator (non-const). */
		_EDGES_IT_TYPE it;

	    protected:
		/** Protected constructor used to initialize th eiterator from a multimap iterator. */
		inline edge_iterator_base(_EDGES_IT_TYPE it)
		{
		    this->it = it;
		}

		public:
		/** Copy-constructor for use from derived classes */
		inline edge_iterator_base(const edge_iterator_base & ei) : it(ei.it)
		{
		}

	    protected:
		/**
		 * @brief Default constructor.
		 *
		 * Useful for declaring edge_iterators without initialization.
		 */
		inline edge_iterator_base() : it()
		{
		}

		/**
		 * @brief Compares an edge_iterator with another one.
		 *
		 * @param ei The edge_iterator to compare this to.
		 *
		 * @returns True if the two iterators point to the same position in the edges collection.
		 */
		public:
		inline bool operator==(const edge_iterator_base & ei) const
		{
		    return it == ei.it;
		}

		/**
		 * @brief Compares an edge_iterator with another one
		 *
		 * @param ei The edge_iterator to compare this to.
		 *
		 * @returns True if the two iterators are different, i.e. they do not point to the same
		 * position in the edges collection.
		 */
		public:
		inline bool operator!=(const edge_iterator_base & ei) const
		{
		    return it != ei.it;
		}

		/**
		 * @brief Gets the label of the start node of the edge.
		 *
		 * In the case of undirected graphs, it gets the label of one of
		 * the two nodes connected by the edge. To get the other node's label,
		 * or the end node label in the case of directed graphs, use operator +.
		 *
		 * @returns The label of the start node.
		 */
		public:
		inline const NODE & operator-() const
		{
		    return it->second->n1->ni.it->first;
		}

		/**
		 * @brief Gets the label of the end node of the edge.
		 *
		 * In the case of undirected graphs, it gets the label of one of
		 * the two nodes connected by the edge. To get the other node's label,
		 * or the start node label in the case of directed graphs, use operator -.
		 *
		 * @returns The label of the end node.
		 */
		public:
		inline const NODE & operator+() const
		{
		    return it->second->n2->ni.it->first;
		}

		/**
		 * @brief INTERNAL. Copies multimap iterator from argument.
		 */
		protected:
		inline void copy(const edge_iterator_base & ei)
		{
			this->it = ei.it;
		}
	};

	/**
	 * @brief The edge iterator implementation.
	 *
	 * The edge iterator may be used to perform insertions and deletions of edges
	 * and to change the labels of the edges. It is used as a return value when
	 * searching for edges.
	 */
	class edge_iterator : public edge_iterator_base
	{
	    /* Friendship with classes that need unconstrained access to private members. */
	    friend class Graph<NODE, EDGE, DIRECTED, TAG>;
	    friend class node;
	    friend class _Edge;

	    private:
		/** Private constructor used to initialize th eiterator from a multimap iterator. */
		inline edge_iterator(_EDGES_IT_TYPE it) : edge_iterator_base(it)
		{
		}

		/**
		 * @brief Copy constructor.
		 *
		 * Useful for statements like: edge_iterator ei = ...
		 */
		public:
		inline edge_iterator(const edge_iterator & ei) : edge_iterator_base(ei.it)
		{
		}

		/**
		 * @brief Assignment.
		 *
		 * Useful for asignment operations.
		 */
		public:
		edge_iterator & operator=(const edge_iterator & ei)
		{
			if (this == &ei)
				return *this;

			this->it = ei.it;
			return *this;
		}

		/**
		 * @brief Default constructor.
		 *
		 * Useful for declaring edge_iterators without initialization.
		 */
		public:
		inline edge_iterator() : edge_iterator_base()
		{
		}

		/**
		 * @brief Gets the label of the edge.
		 *
		 * Using this operator one can modify the label of
		 * the edge by a simple assignment operation, e.g.
		 * *ei = "value";
		 *
		 * @returns A reference to the label of the edge.
		 */
		public:
		inline EDGE & operator*()
		{
		    return this->it->second->edge;
		}

		/**
		 * @brief Gets a pointer to the edge label.
		 *
		 * If the label type is a complex type, this operator is
		 * useful to perform operations directly on the edge type
		 * by using a simple syntax, e.g.: ei->c_str(), where ei is
		 * the edge iterator and the type is std::string
		 *
		 * @returns A pointer to the label of the edge.
		 */
		public:
		inline EDGE * operator->()
		{
		    return &this->it->second->edge;
		}

		/**
		 * @brief Gets the start node of the edge.
		 *
		 * For undirected graphs, it just gets one of the two endpoints.
		 * Use to() to get the other endpoint. This method is synonymous to
		 * using the [] operator, with index = 0, e.g.: ei.from() == ei[0]
		 *
		 * @returns A %node_iterator to the start node.
		 */
		public:
		inline node_iterator from()
		{
		    return this->it->second->n1->ni;
		}

		/**
		 * @brief Gets the end node of the edge.
		 *
		 * For undirected graphs, it just gets one of the two endpoints.
		 * Use from() to get the other endpoint. This method is synonymous to
		 * using the [] operator, with index = 1, e.g.: ei.to() == ei[1]
		 *
		 * @returns An iterator to the end node.
		 */
		public:
		inline node_iterator to()
		{
		    return this->it->second->n2->ni;
		}

		/**
		 * @brief Gets the endpoints of the edge
		 *
		 * @param index This may have two values: 0 - gets the start of the edge,
		 * 1 - gets the end of the edge. In the case of undirected graphs, the meaning
		 * of 'start'/'end' of the edge is lost, and this method may be used to return
		 * any of the two endpoints.
		 *
		 * @returns An iterator to the required node. To obtain the node value, just
		 * add * in front of the result, e.g.: *ei[0] gives the label of the start node.
		 */
		public:
		inline node_iterator operator[](int index)
		{
		    switch (index)
			{
			case 0: return this->it->second->n1->ni;
			case 1: return this->it->second->n2->ni;
			default: throw "An edge has only two nodes";
			}
		}

		/**
		 * @brief Advance to the next edge in the collection.
		 *
		 * @returns An %edge_iterator previous to the increment operation.
		 */
		public:
		inline edge_iterator operator++(int)
		{
		    edge_iterator tmp = *this;
		    this->it++;
		    return tmp;
		}

		/**
		 * @brief Advance to the next edge in the collection.
		 *
		 * @returns An %edge_iterator after the increment operation.
		 */
		public:
		inline edge_iterator & operator++()
		{
		    this->it++;
		    return *this;
		}

		/**
		 * @brief Go back to the previous edge in the collection.
		 *
		 * @returns An %edge_iterator previous to the decrement operation.
		 */
		public:
		inline edge_iterator operator--(int)
		{
		    edge_iterator tmp = *this;
		    this->it--;
		    return tmp;
		}

		/**
		 * @brief Go back to the previous edge in the collection.
		 *
		 * @returns An %edge_iterator after the increment operation.
		 */
		public:
		inline edge_iterator & operator--()
		{
		    this->it--;
		    return *this;
		}
	};

	/**
	 * @brief The const edge iterator implementation.
	 *
	 * The edge iterator may be used to browse the edges of a node.
	 * It may not be used to modify the edges or their labels.
	 */
	class const_edge_iterator : public edge_iterator_base
	{
	    /* Friendship with classes that need unconstrained access to private members. */
	    friend class Graph<NODE, EDGE, DIRECTED, TAG>;
	    friend class node;
	    friend class _Edge;

		/** Private constructor used to initialize th eiterator from a multimap iterator. */
		private:
		inline const_edge_iterator(_EDGES_IT_TYPE it) : edge_iterator_base(it)
		{
		}

		/**
		 * @brief Copy constructor.
		 *
		 * Useful for statements like: edge_iterator ei = ...
		 *
		 * @param ei An %edge_iterator or a %const_edge_iterator.
		 */
		public:
		inline const_edge_iterator(const edge_iterator_base & ei) : edge_iterator_base(ei.it)
		{
		}

		/**
		 * @brief Assignment.
		 *
		 * Useful for asignment operations.
		 */
		public:
		inline const_edge_iterator & operator=(const edge_iterator_base & ei)
		{
			if (this != &ei)
				this->copy(ei.it);
			return *this;
		}

		/**
		 * @brief Default constructor.
		 *
		 * Useful for declaring edge_iterators without initialization.
		 */
	 	public:
		inline const_edge_iterator() : edge_iterator_base()
		{
		}

		/**
		 * @brief Gets the label of the edge (const).
		 *
		 * Using this operator one retrieve the edge label.
		 *
		 * @returns A const reference to the label of the edge.
		 */
		public:
		inline const EDGE & operator*() const
		{
		    return this->it->second->edge;
		}

		/**
		 * @brief Gets a pointer to the edge label (const).
		 *
		 * If the label type is a complex type, this operator is
		 * useful to perform operations directly on the edge type
		 * by using a simple syntax, e.g.: ei->c_str(), where ei is
		 * the edge iterator and the type is std::string
		 *
		 * @returns A const pointer to the label of the edge.
		 */
		public:
		inline const EDGE * operator->() const
		{
		    return &this->it->second->edge;
		}

		/**
		 * @brief Gets the start node of the edge.
		 *
		 * For undirected graphs, it just gets one of the two endpoints.
		 * Use to() to get the other endpoint. This method is synonymous to
		 * using the [] operator, with index = 0, e.g.: ei.from() == ei[0]
		 *
		 * @returns A %const_node_iterator to the start node.
		 */
		public:
		inline const_node_iterator from() const
		{
		    return this->it->second->n1->ni;
		}

		/**
		 * @brief Gets the end node of the edge.
		 *
		 * For undirected graphs, it just gets one of the two endpoints.
		 * Use from() to get the other endpoint. This method is synonymous to
		 * using the [] operator, with index = 1, e.g.: ei.to() == ei[1]
		 *
		 * @returns An %const_node_iterator to the end node.
		 */
		public:
		inline const_node_iterator to() const
		{
		    return this->it->second->n2->ni;
		}

		/**
		 * @brief Gets the endpoints of the edge
		 *
		 * @param index This may have two values: 0 - gets the start of the edge,
		 * 1 - gets the end of the edge. In the case of undirected graphs, the meaning
		 * of 'start'/'end' of the edge is lost, and this method may be used to return
		 * any of the two endpoints.
		 *
		 * @returns An iterator to the required node. To obtain the node value, just
		 * add * in front of the result, e.g.: *ei[0] gives the label of the start node.
		 */
		public:
		inline const_node_iterator operator[](int index) const
		{
		    switch (index)
			{
			case 0: return this->it->second->n1->ni;
			case 1: return this->it->second->n2->ni;
			default: throw "An edge has only two nodes";
			}
		}

		/**
		 * @brief Advance to the next edge in the collection.
		 *
		 * @returns A %const_edge_iterator previous to the increment operation.
		 */
		public:
		inline const_edge_iterator operator++(int)
		{
		    const_edge_iterator tmp = *this;
		    this->it++;
		    return tmp;
		}

		/**
		 * @brief Advance to the next edge in the collection.
		 *
		 * @returns A %const_edge_iterator after the increment operation.
		 */
		public:
		inline const_edge_iterator & operator++()
		{
		    this->it++;
		    return *this;
		}

		/**
		 * @brief Go back to the previous edge in the collection.
		 *
		 * @returns A %const_edge_iterator previous to the decrement operation.
		 */
		public:
		inline const_edge_iterator operator--(int)
		{
		    const_edge_iterator tmp = *this;
		    this->it--;
		    return tmp;
		}

		/**
		 * @brief Go back to the previous edge in the collection.
		 *
		 * @returns A %const_edge_iterator after the increment operation.
		 */
		public:
		inline const_edge_iterator & operator--()
		{
		    this->it--;
		    return *this;
		}
	};

	/* the nodes collection */
	private:
	/* why use mutable? here's why:
	 	- if the Graph is const, then its members are const and
		nodes.begin() and nodes.end() return const_iterators.
		This conflicts with our design of having a node_iterator_base
		as a base class for node_iterator and const_node_iterator, because
		node_iterator_base accepts in its constructor only
		std::multimap<>::iterators (non-const). By changing this to mutable,
		we prevent it from being qualified with const when the container is const.
		It will always be modifiable from any method of Graph (be the method
		const or not), but that doesn't bother us because we are careful when
		to actually modify the multimap.
	*/
	mutable _NODES_TYPE nodes;

	public:
	/**
	 * @brief The base class for %node_iterator and %const_node_iterator.
	 *
	 * Contains all common operations.
	 */
	class node_iterator_base
	{
		friend class node;
		friend class Graph<NODE, EDGE, DIRECTED, TAG>;

		/** The multimap iterator. */
		protected:
		_NODES_IT_TYPE it;

		/** Protected default constructor so that we don't have declarations like:
		 * node_iterator_base x;
		 * elsewhere than in the derived classes.
		 */
		protected:
		inline node_iterator_base() : it()
		{
		}

		/**
		 * @brief Copy constructor.
		 *
		 * Useful in asigning iterators, e.g.: ni1 = ni2;
		 */
		public:
		inline node_iterator_base(const node_iterator_base & ni) : it(ni.it)
		{
		}

		/**
		 * @brief Protected constructor used by Graph to create an iterator from its nodes collection.
		 *
		 * @param it The multimap iterator.
		 */
		protected:
		inline node_iterator_base(_NODES_IT_TYPE it)
		{
		    this->it = it;
		}

		/**
		 * @brief Compares a node_iterator with another one.
		 *
		 * @param i The node_iterator to compare this to.
		 *
		 * @returns True if the two iterators point to the same position in the nodes collection.
		 */
		public:
		inline bool operator==(const node_iterator_base & i) const
		{
		    return it == i.it;
		}

		/**
		 * @brief Compares a node_iterator with another one
		 *
		 * @param i The node_iterator to compare this to.
		 *
		 * @returns True if the two iterators are different, i.e. they do not point to the same
		 * position in the nodes collection.
		 */
		public:
		inline bool operator!=(const node_iterator_base & i) const
		{
		    return it != i.it;
		}

		/**
		 * @brief Gets the label of the node.
		 *
		 * Gets a constant reference to the label of the node. This prevents
		 * modification of the label, as it is stated in the class description.
		 *
		 * @returns Constant reference to the label of the node.
		 */
		inline const NODE & operator*() const
		{
		    return it->first;
		}

		/**
		 * @brief Copy the it from the argument.
		 */
		protected:
		void copy(const node_iterator_base & ni)
		{
			this->it = ni.it;
		}
	};

    public:
	/**
	 * @brief The node iterator implementation.
	 *
	 * The node iterator allows finding and deleting nodes. It also permits
	 * modifying the edges of a node. You may not use the node iterator to
	 * rename a node (i.e. change it's value). This can only be accomplished
	 * by removing the node and re-inserting it with the new label.
	 */
	class node_iterator : public node_iterator_base
	{
	    /* Friendship with classes that need unconstrained access to private members. */
	    friend class node;
	    friend class edge_iterator_base;
		friend class edge_iterator;
		friend class const_edge_iterator;
	    friend class Graph<NODE, EDGE, DIRECTED, TAG>;

	    public:
		/**
		 * @brief Default constructor to allow declarations without initialization.
		 */
		node_iterator()
		{
		}

		/**
		 * @brief Copy constructor.
		 *
		 * Useful in asigning iterators, e.g.: node_iterator ni1 = ni2;
		 */
		public:
		inline node_iterator(const node_iterator & ni) : node_iterator_base(ni)
		{
		}

		/**
		 * @brief Assignment operation.
		 *
		 * @returns a reference to this node_iterator.
		 */
		inline node_iterator & operator=(const node_iterator & ni)
		{
			if (this != &ni)
				this->it = ni.it;
			return *this;
		}

		/**
		 * @brief Private constructor used by Graph to create an iterator from its nodes collection.
		 *
		 * @param it The multimap iterator.
		 */
		private:
		inline node_iterator(_NODES_IT_TYPE it) : node_iterator_base(it)
		{
		}

		/**
		 * @brief Accesses the node interface.
		 *
		 * The node interface allows operations of the node: inserting, removing,
		 * finding or changing edges.
		 *
		 * @returns A node interface pointer.
		 */
		public:
		inline node * operator->() const
		{
			return this->it->second;
		}

		/**
		 * @brief Advance to the next node in the collection.
		 *
		 * @returns An iterator previous to the increment operation.
		 */
		public:
		inline node_iterator operator++(int)
		{
		    node_iterator tmp = *this;
		    this->it++;
		    return tmp;
		}

		/**
		 * @brief Advance to the next node in the collection.
		 *
		 * @returns An iterator after the increment operation.
		 */
		public:
		inline node_iterator & operator++()
		{
		    this->it++;
		    return *this;
		}

		/**
		 * @brief Go back to the previous node in the collection.
		 *
		 * @returns An iterator previous to the decrement operation.
		 */
		public:
		inline node_iterator operator--(int)
		{
		    node_iterator tmp = *this;
		    this->it--;
		    return tmp;
		}

		/**
		 * @brief Go back to the previous node in the collection.
		 *
		 * @returns An iterator after the decrement operation.
		 */
		public:
		inline node_iterator & operator--()
		{
		    this->it++;
		    return *this;
		}
	};

    public:
	/**
	 * @brief The const node iterator implementation.
	 *
	 * The const node iterator allows browsing nodes. You may not use the
	 * node iterator to rename a node (i.e. change it's value).
	 * This can only be accomplished by removing the node and re-inserting
	 * it with the new label.
	 */
	class const_node_iterator : public node_iterator_base
	{
	    /* Friendship with classes that need unconstrained access to private members. */
	    friend class node;
	    friend class edge_iterator;
	    friend class Graph<NODE, EDGE, DIRECTED, TAG>;

		/**
		 * @brief Default constructor to allow declarations without initialization.
		 */
		public: const_node_iterator() : node_iterator_base()
		{
		}

		/**
		 * @brief Copy constructor.
		 *
		 * Useful in asigning iterators, e.g.: const_node_iterator ni1 = ni2;
		 *
		 * @param ni A %node_iterator_base to copy from. A const node iterator can
		 * be copy-constructed from another %const_node_iterator or from
		 * a %node_iterator
		 */
		public:
		inline const_node_iterator(const node_iterator_base & ni) : node_iterator_base(ni)
		{
		}

		/**
		 * @brief Assignment operation.
		 *
		 * The same observation applies as in the case of the copy constructor.
		 */
		public:
		inline const_node_iterator & operator=(const node_iterator_base & ni)
		{
			if (this != &ni)
				this->copy(ni);
			return *this;
		}

		/**
		 * @brief Private constructor used by Graph to create an iterator from its nodes collection.
		 *
		 * @param it The multimap iterator.
		 */
		private:
		inline const_node_iterator(_NODES_IT_TYPE it) : node_iterator_base(it)
		{
		}

		/**
		 * @brief Accesses the node interface.
		 *
		 * The node interface allows operations of the node: inserting, removing,
		 * finding or changing edges.
		 *
		 * @returns A node iterface pointer.
		 */
		public:
		inline const node * operator->() const
		{
		    return this->it->second;
		}

		/**
		 * @brief Advance to the next node in the collection.
		 *
		 * @returns An iterator previous to the increment operation.
		 */
		public:
		inline const_node_iterator operator++(int)
		{
		    const_node_iterator tmp = *this;
		    this->it++;
		    return tmp;
		}

		/**
		 * @brief Advance to the next node in the collection.
		 *
		 * @returns An iterator after the increment operation.
		 */
		public:
		inline const_node_iterator & operator++()
		{
		    this->it++;
		    return *this;
		}

		/**
		 * @brief Go back to the previous node in the collection.
		 *
		 * @returns An iterator previous to the decrement operation.
		 */
		public:
		inline const_node_iterator operator--(int)
		{
		    const_node_iterator tmp = *this;
		    this->it--;
		    return tmp;
		}

		/**
		 * @brief Go back to the previous node in the collection.
		 *
		 * @returns An iterator after the decrement operation.
		 */
		public:
		inline const_node_iterator & operator--()
		{
		    this->it++;
		    return *this;
		}


	};

	/**
	 * @brief Gets the start of the nodes collection.
	 *
	 * @returns An iterator to the beginning of the collection.
	 */
	public:
	node_iterator begin()
	{
	    return node_iterator(nodes.begin());
	}

	/**
	 * @brief Gets the start of the nodes collection.
	 *
	 * @returns An iterator to the beginning of the collection.
	 */
	public:
	const_node_iterator begin() const
	{
	    return const_node_iterator(nodes.begin());
	}

	/**
	 * @brief Gets the end of the nodes collection.
	 *
	 * @returns An iterator to the end of the collection.
	 */
	public:
	node_iterator end()
	{
	    return node_iterator(nodes.end());
	}

	/**
	 * @brief Gets the end of the nodes collection.
	 *
	 * @returns An iterator to the end of the collection.
	 */
	public:
	const_node_iterator end() const
	{
	    return const_node_iterator(nodes.end());
	}

	/**
	 * @brief Inserts a node into the graph.
	 *
	 * @param label The label of the node to be insert. Will be copied into the
	 * %Graph structures and it may not be modified later.
	 *
	 * @returns A %node_iterator pointing to the newly inserted node.
	 */
	public:
	node_iterator insert(const NODE & label)
	{
	    /* Create a node with an invalid iterator. The iterator will be set after
	       the node is inserted into the multimap. */
	    node *n = new node(nodes.end(), this);
	    _NODES_IT_TYPE it = nodes.insert(std::make_pair(label, n));

	    /* Set the iterator for the newly added node */
	    n->ni = node_iterator(it);

	    return n->ni;
	}

    private:
	/* Erases a node from the nodes collection and destroys it. */
	void erase(_NODES_IT_TYPE it)
	{
	    /* delete node *, this also deletes all links */
	    delete it->second;

	    /* remove from map */
	    nodes.erase(it);
	}

	/**
	 * @brief Erase a node from the %Graph
	 *
	 * @param ni A %node_iterator or %const_node_iterator pointing to the node to be removed.
	 */
	public:
	void erase(node_iterator_base ni)
	{
	    this->erase(ni.it);
	}

	/**
	 * @brief Erase one or more nodes from the %Graph
	 *
	 * Note that a graph may have multiple nodes with the same
	 * label. In this case, calling this function will remove all of
	 * them.
	 *
	 * @param node The label of the node to be deleted.
	 */
	public:
	void erase(const NODE & node)
	{
	    _NODES_IT_TYPE it;

	    /* delete node * */
	    for (it = nodes.lower_bound(node); it != nodes.upper_bound(node); it++)
			delete it->second;

	    /* erase from map */
	    nodes.erase(node);
	}

    /**
	 * @brief Erase a range of nodes
	 *
	 * This method erases all the nodes contained between the two iterators
	 * supplied as arguments.
	 *
	 * @param first Iterator to the lower bound of the range of nodes to be deleted.
	 * @param last Iterator to the upper bound of the range of nodes to be deleted.
	 */
	public:
	void erase(node_iterator_base first, node_iterator_base last)
	{
	    /* at first delete nodes, this also handles links */
	    for (; first != last; first++)
			delete first.it->second;

	    /* erase from map */
	    nodes.erase(first.it, last.it);
	}

	/* Inserts an edge linking two nodes, labeled with the specified edge label */
	private:
	void add_edge(node *n1, node *n2, const EDGE & edge)
	{
	    n1->links.insert(std::make_pair(n2, edge));

	    n2->rev_links.insert(n1);
	}

	/**
	 * @brief Inserts an edge into the graph.
	 *
	 * @param n1 A %node_iterator or %const_node_iterator pointing to the start node of the edge.
	 *  If the %Graph is undirected, this argument represents one end of the edge.
	 * @param n2 A %node_iterator or %const_node_iterator pointing to the end node of the edge.
	 * If the %Graph is undirected, this argument represents the other end of the edge.
	 * @param edge The label of the edge to be inserted.
	 *
	 * @returns A %edge_iterator pointing to the newly inserted edge.
	 */
	public:
    edge_iterator insert_edge(node_iterator_base n1, node_iterator_base n2, const EDGE & edge)
	{
	    return n1->insert(n2, edge);
	}

	/**
	 * @brief Erases all the edges connecting two nodes.
	 *
	 * @param n1 A %node_iterator or %const_node_iterator pointing to the start node of the edge(s) to be removed.
	 * If the %Graph is undirected, this argument represents one end of the edge.
	 * @param n2 A %node_iterator or %const_node_iterator pointing to the end node of the edge(s) to be removed.
	 * If the %Graph is undirected, this argument represents the other end of the edge.
	 */
	public:
	inline void erase_edge(node_iterator_base n1, node_iterator_base n2)
	{
	    n1->erase_all_edges(n2);
	}

    /* Finds a node labeled with 'node' and equal to ptr (uses node as a search hint) */
	private:
	node_iterator ptr2iterator(const NODE & node, const node *ptr)
	{
	    for (_NODES_IT_TYPE it = nodes.lower_bound(node); it != nodes.upper_bound(node); it++)
			if (it->second == ptr)
		    	return node_iterator(it);

	    return this->end();
	}

	/**
	 * @brief Finds a node by its label.
	 *
	 * @param label The label to be searched.
	 *
	 * Note that a %Graph may contain several nodes with the same label.
	 * In that case, the method will arbitrarily return one of them.
	 *
	 * @returns A %node_iterator pointing to the found node, or %Graph::end()
	 *	in case it doesn't find anything.
	 */
    public:
	inline node_iterator find(const NODE & label)
	{

	    node_iterator nit = node_iterator(nodes.lower_bound(label));
	    //note : lowerbound may return a node with a different label (first where key >= input key) so we need to check if it's the same label
	    if (*nit== label)
	    	return nit;
	    else
	    	return end();
	}

	/**
	 * @brief Finds a node by its label (const version)
	 *
	 * @param label The label to be seached.
	 */
	inline const_node_iterator find(const NODE & label) const
	{
		const_node_iterator nit = const_node_iterator(nodes.lower_bound(label));
		//see above
		if (*nit== label)
		   	return nit;
		else
	    	return end();
	}

	/* Make a copy of the graph */
	private:
	void copy(const _SELF_TYPE & g)
	{
	    const_node_iterator cni;

		/* clear this graph */
		this->clear();

	    /* keep a node * to node * mapping to find nodes easier later on */
	    std::map<const node *, node *> tmpmap;

	    /* add nodes from g */
	    for (cni = g.begin(); cni != g.end(); cni++)
	    {
			node_iterator ni = this->insert(*cni);
			tmpmap.insert(std::make_pair(cni.it->second, ni.it->second));
	    }

	    /* add edges from g */
	    for (cni = g.begin(); cni != g.end(); cni++)
	    {
			typename std::map<const node *, node *>::iterator it1 = tmpmap.find(cni.it->second);
			node *newn1 = it1->second;

			for (const_edge_iterator cei = cni->begin(); cei != cni->end(); cei++)
			{
		    	const _Edge *pe = cei.it->second;
		    	const node *n1 = pe->n1 == cni.it->second ? pe->n1 : pe->n2;
		    	const node *n2 = pe->n2 != cni.it->second ? pe->n2 : pe->n1;

		    	/* 	For undirected graphs, do a trick:
				Compare node pointers and only copy the edge going out of the smaller pointer.
		        Links from nodes to self are handled properly because they are added only once,
				even though the graph is undirected. By contrast, an edge linking two different nodes
				is added twice for an undirected graph.
		        For directed graphs we copy the edge directly.
				*/
		    	if (DIRECTED || n1 < n2)
		    	{
					/* find the new node * corresponding to the old node * */
					typename std::map<const node *, node *>::iterator it2 = tmpmap.find(n2);
					node *newn2 = it2->second;

					newn1->insert_edge(newn2->ni, *cei);
		    	}
			}
	    }
	}

	/**
	 * @brief Copy constructor
	 *
	 * @param g A %Graph to be copied.
	 *
	 * Creates a new graph by copying all the structures and labels of the
	 * argument. This may be used in assignment operations, e.g.: g1 = g2;
	 */
	public:
	Graph(const _SELF_TYPE & g)
	{
	    this->copy(g);
	}

	/**
	 * @brief Assignment operator.
	 *
	 * @param g A %Graph to be assigned from.
	 *
	 * Copies all the nodes and edges of the graph given as argument.
	 *
	 * @returns A reference to the right hand side of the assignment.
	 */
	public:
	_SELF_TYPE & operator=(const _SELF_TYPE & g)
	{
		this->copy(g);
	}

	/**
	 * @brief Default constructor.
	 *
	 * Permits declaration without initialization of a %Graph object, e.g.: Graph<...> g;
	 * Initializes the _name field with a string of the form 'GraphX', where X is incremented
	 * upon each %Graph object creation.
	 */
	public:
	Graph()
	{
	}

	/**
	 * @brief Destructor. Deletes all nodes and edges of a graph
	 */
	public:
	~Graph()
	{
	    clear();
	}

	/**
	 * @brief Returns the number of nodes.
	 *
	 * @returns The number of nodes in the graph.
	 */
	public:
	int count_nodes() const
	{
	    return (int)nodes.size();
	}

	/**
	 * @brief Deletes all nodes and edges of a graph.
	 */
	public:
	void clear()
	{
	    _NODES_IT_TYPE it;

	    for (it = nodes.begin(); it != nodes.end(); it++)
			/* deallocate node */
			delete it->second;

	    nodes.clear();
	}
};

//namespace graph
}

#endif
