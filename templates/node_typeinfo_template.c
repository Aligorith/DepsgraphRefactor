/** 
 * This file defines a template to use for defining typeinfo for new node types.
 *
 * == Usage ==
 * Simply copy the code below into depsgraph_type_defines.c, and modify the 
 * function name + node type declarations. That is:
 *   1) replace("mynode", "mynewnode")
 *   2) replace("MyNode", "MyNewNode")
 *
 * == Version ==
 * Last Update: 3 August 2013
 */

/* MyNode Node ========================================== */

/* Initialise 'mynode' node - from pointer data given */
static void dnti_mynode__init_data(DepsNode *node, ID *id)
{
	
}

/* Free 'mynode' node */
static void dnti_mynode__free_data(DepsNode *node)
{
	
}

/* Copy 'mynode' node - Assume that the mynode doesn't get copied for now... */
static void dnti_mynode__copy_data(DepsgraphCopyContext *dcc, DepsNode *dst, const DepsNode *src)
{
	const MyNodeDepsNode *src_node = (const MyNodeDepsNode *)src;
	MyNodeDepsNode *dst_node       = (MyNodeDepsNode *)dst;
	
}

/* Add 'mynode' node to graph */
static void dnti_mynode__add_to_graph(Depsgraph *graph, DepsNode *node, ID *id)
{
	
}

/* Remove 'mynode' node from graph */
static void dnti_mynode__remove_from_graph(Depsgraph *graph, DepsNode *node)
{
	
}

/* Validate mynode links... */
static void dnti_mynode__validate_links(Depsgraph *graph, DepsNode *node)
{
	
}

/* MyNode Type Info */
static DepsNodeTypeInfo DNTI_SUBGRAPH = {
	/* type */               DEPSNODE_TYPE_MYNODE,
	/* size */               sizeof(MyNodeDepsNode),
	/* name */               "MyNode Node",
	
	/* init_data() */        dnti_mynode__init_data,
	/* free_data() */        dnti_mynode__free_data,
	/* copy_data() */        dnti_mynode__copy_data,
	
	/* add_to_graph() */     dnti_mynode__add_to_graph,
	/* remove_from_graph()*/ dnti_mynode__remove_from_graph,
	
	/* validate_links() */   dnti_mynode__validate_links
};
