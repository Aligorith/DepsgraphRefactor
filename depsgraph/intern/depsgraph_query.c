/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2013 Blender Foundation.
 * All rights reserved.
 *
 * Original Author: Joshua Leung
 * Contributor(s): None Yet
 *
 * ***** END GPL LICENSE BLOCK *****
 *
 * Implementation of Querying and Filtering API's
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MEM_guardedalloc.h"

#include "BLI_blenlib.h"
#include "BLI_ghash.h"
#include "BLI_utildefines.h"

#include "BKE_depsgraph.h"
#include "BKE_depsgraph_query.h"

#include "RNA_access.h"
#include "RNA_types.h"

#include "depsgraph_types.h"
#include "depsgraph_intern.h"

/* ************************************************ */
/* Filtering API - Basically, making a copy of the existing graph */

/* Create filtering context */
// TODO: allow passing in a number of criteria?
DepsgraphCopyContext *DEG_filter_init()
{
	DepsgraphCopyContext *dcc = MEM_callocN(sizeof(DepsgraphCopyContext), "DepsgraphCopyContext");
	
	/* init hashes for easy lookups */
	dcc->nodes_hash = BLI_ghash_ptr_new("Despgraph Filter NodeHash");
	dcc->rels_hash = BLI_ghash_ptr_new("Despgraph Filter Relationship Hash"); // XXX?
	
	/* store filtering criteria? */
	// xxx...
	
	return dcc;
}

/* Cleanup filtering context */
void DEG_filter_cleanup(DepsgraphCopyContext *dcc)
{
	/* sanity check */
	if (dcc == NULL)
		return;
		
	/* free hashes - contents are weren't copied, so are ok... */
	BLI_ghash_free(dcc->nodes_hash);
	BLI_ghash_free(dcc->rels_hash);
	
	/* clear filtering criteria */
	// ...
	
	/* free dcc itself */
	MEM_freeN(dcc);
}

/* -------------------------------------------------- */

/* Create a copy of provided node */
// FIXME: the handling of sub-nodes and links will need to be subject to filtering options...
// XXX: perhaps this really shouldn't be exposed, as it will just be a sub-step of the evaluation process?
DepsNode *DEG_copy_node(DepsgraphCopyContext *dcc, const DepsNode *src)
{
	const DepsNodeTypeInfo *nti = DEG_get_node_typeinfo(type);
	DepsNode *dst;
	
	/* sanity check */
	if (src == NULL)
		return NULL;
	
	/* allocate new node, and brute-force copy over all "basic" data */
	// XXX: need to review the name here, as we can't have exact duplicates...
	dst = DEG_create_node(src->type, src->name);
	memcpy(dst, src, nti->size);
	
	/* add this node-pair to the hash... */
	BLI_ghash_insert(dcc->node_hash, src, dst);
	
	/* now, fix up any links in standard "node header" (i.e. DepsNode struct, that all 
	 * all others are derived from) that are now corrupt 
	 */
	{
		/* not assigned to graph... */
		dst->next = dst->prev = NULL;
		dst->owner = NULL;
		
		/* relationships to other nodes... */
		// FIXME: how to handle links? We may only have partial set of all nodes still?
		// XXX: the exact details of how to handle this are really part of the querying API...
		
		// XXX: BUT, for copying subgraphs, we'll need to define an API for doing this stuff anyways
		// (i.e. for resolving and patching over links that exist within subtree...)
		dst->inlinks.first = dst->inlinks.last = NULL;
		dst->outlinks.first = dst->outlinks.last = NULL;
		
		/* clear traversal data */
		dst->valency = 0;
		dst->lasttime = 0;
	}
	
	/* fix up type-specific data (and/or subtree...) */
	if (nti->copy_data) {
		nti->copy_data(dcc, dst, src);
	}
	
	/* fix links */
	// XXX...
	
	/* return copied node */
	return dst;
}

/* Make a copy of a relationship */
DepsRelation DEG_copy_relation(const DepsRelation *src)
{
	DepsRelation *dst = MEM_dupallocN(src);
	
	/* clear out old pointers which no-longer apply */
	dst->next = dst->prev = NULL;
	
	/* return copy */
	return dst;
}

/* ************************************************ */
/* Querying API */

/* ************************************************ */
